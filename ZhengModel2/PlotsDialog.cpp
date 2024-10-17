// PlotsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ZhengModelHeaders.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlotsDialog dialog

IMPLEMENT_SERIAL(CPlotsDialog, CDialog, _VERSION_NUMBER)

CPen* CPlotsDialog::m_pPenArray[NUM_PEN];


CPlotsDialog::CPlotsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPlotsDialog::IDD, pParent)
{
	m_rect = CRect(0,0,0,0);
	for (int i=0; i < _MAX_PLOTS; i++) 
		m_rectPlot[i] = CRect(0,0,0,0);
	m_pPenArray[0] = new CPen(PS_SOLID, 1, RED);
	m_pPenArray[1] = new CPen(PS_SOLID, 1, GREEN);
	m_pPenArray[2] = new CPen(PS_SOLID, 1, BLUE);
	m_pPenArray[3] = new CPen(PS_SOLID, 1, VIOLET);
	m_pPenArray[4] = new CPen(PS_SOLID, 1, ORANGE);
	m_pPenArray[5] = new CPen(PS_SOLID, 1, BABY_BLUE);
	m_pPenArray[6] = new CPen(PS_SOLID, 1, YELLOW);
	m_pPenArray[7] = new CPen(PS_DOT, 1, GRAY);
	m_NumPlots = _MAX_PLOTS; 
	m_PlotHeight = 0;
	m_PlotWidth = 0;
	m_screen_num = -1;
	m_screen_num_old = -1;
	m_ZeroX = 0;
	m_LastX = 0;
	for (i=0; i < _MAX_PLOTS; i++) {
		m_minY[i] = _MINY;
		m_maxY[i] = _MAXY;
		m_BottomY[i] = 0;
		//m_ZeroY[i] = 0;
		m_LastY[i] = 0;
	}
	//{{AFX_DATA_INIT(CPlotsDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPlotsDialog::~CPlotsDialog() 
{
	for (int i = 0; i < NUM_PEN; i++) 
		delete m_pPenArray[i];
}

void CPlotsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlotsDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlotsDialog, CDialog)
	//{{AFX_MSG_MAP(CPlotsDialog)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlotsDialog message handlers

BOOL CPlotsDialog::RestoreWindowState()
{
	CString version = "Version 1.0";

	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(&wp);

	if (((wp.rcNormalPosition.left = 
			AfxGetApp()->GetProfileInt (version, "Plots.x1", -1)) != -1) &&
		((wp.rcNormalPosition.top = 
			AfxGetApp()->GetProfileInt (version, "Plots.y1", -1)) != -1) &&
		((wp.rcNormalPosition.right = 
			AfxGetApp()->GetProfileInt (version, "Plots.x2", -1)) != -1) &&
		((wp.rcNormalPosition.bottom = 
			AfxGetApp()->GetProfileInt (version, "Plots.y2", -1)) != -1)) {

		wp.rcNormalPosition.left = min(wp.rcNormalPosition.left,
			::GetSystemMetrics(SM_CXSCREEN) - 
			::GetSystemMetrics(SM_CXICON));
		wp.rcNormalPosition.top = min (wp.rcNormalPosition.top,
			::GetSystemMetrics(SM_CYSCREEN) -
			::GetSystemMetrics(SM_CYICON));

		SetWindowPlacement (&wp);
		ClearPlots();
		return TRUE;
	}
	return FALSE;
}

void CPlotsDialog::SaveWindowState()
{
	CString version = "Version 1.0";

	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(&wp);

	AfxGetApp()->WriteProfileInt (version,"Plots.x1",wp.rcNormalPosition.left);
	AfxGetApp()->WriteProfileInt (version,"Plots.y1",wp.rcNormalPosition.top);
	AfxGetApp()->WriteProfileInt (version,"Plots.x2",wp.rcNormalPosition.right);
	AfxGetApp()->WriteProfileInt (version,"Plots.y2",wp.rcNormalPosition.bottom);
}

BOOL CPlotsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//ClearPlots();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPlotsDialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (IsWindowVisible()) 
	{
		ClearPlots();
	}
	
}

/*void CPlotsDialog::DoPlots(int t_step, int num_steps_per_screen, int graph_frequency)
{
	//CDC* PlotsDC = ctrlPlotsPict().GetDC();
	CClientDC PlotsDC(&(ctrlPlotsPict()));
	//do x values, i.e. time, for all plot
	int T = t_step%num_steps_per_screen;
	int ThisX = (m_ZeroX + (int)(((double)T/num_steps_per_screen)*m_PlotWidth));
	if (T == 0) {
		ClearPlots();
		m_LastX = ThisX;
	}
	//do y values for each plot
	int ThisY[_MAX_PLOTS];  //actual y-value in picture box coordinates
	double YValue[_MAX_PLOTS]; //value of y as fraction of axis height
	double YVariable[_MAX_PLOTS]; //actual value of Y variable, in appropriate units for plotting
	double VSoma = GetCompart(_Soma)->m_V;
	YVariable[0] = VSoma; //Soma voltage [mV]
	YVariable[1] = GetCompart(_Hillock)->m_V; //Hillock voltage [mV]
	YVariable[2] = GetDoc()->m_IInject[_Soma]; //Soma current injection [nA]
	YVariable[3] = GetChan(_Soma, _I_CaT)->m_I(VSoma); //Soma CaT current
	YVariable[4] = GetChan(_Hillock, _I_Na)->m_gMax; //gMax for _I_Na
	YVariable[5] = GetChan(_Soma, _I_CaT)->m_gMax; //gMax for _I_CaT
	//YVariable[5] = GetCompart(_Soma)->m_ITotal(InjSoma); //Total current (in or out???) of soma [nA]
	for (int i = 0; i < m_NumPlots; i++) {
		YValue[i] = (YVariable[i] - m_minY[i])/(m_maxY[i] - m_minY[i]);
	}
	for (i = 0; i < m_NumPlots; i++) {
		PlotsDC.SelectObject(m_pPenArray[i]);
		ThisY[i] = (m_BottomY[i] - (int)(YValue[i]*m_PlotHeight));
		//ThisY[i] = (m_ZeroY[i] - (int)(YValue[i]*0.5*m_PlotHeight));
		if (T == 0) {
			m_LastY[i] = ThisY[i];
		}
		PlotsDC.MoveTo(m_LastX, m_LastY[i]);
		PlotsDC.LineTo(ThisX, ThisY[i]);
		m_LastY[i] = ThisY[i];
	}
	m_LastX = ThisX;
}*/

void CPlotsDialog::DoPlots(double t)
{
	//CDC* PlotsDC = ctrlPlotsPict().GetDC();
	CClientDC PlotsDC(&(ctrlPlotsPict()));
	//do x values, i.e. time, for all plots
	m_screen_num = (int)(t/_TIME_PER_SCREEN);  //start count at 0
	double T;
	int ThisX;
	T = t - (((double)m_screen_num) * _TIME_PER_SCREEN);
	ThisX = (m_ZeroX + (int)((T/_TIME_PER_SCREEN) * m_PlotWidth));
	if ((t == 0) OR (m_screen_num != m_screen_num_old)) {
		ClearPlots();
		m_LastX = ThisX;
		m_screen_num_old = m_screen_num;
	}
	//do y values for each plot
	int ThisY[_MAX_PLOTS];  //actual y-value in picture box coordinates
	double YValue[_MAX_PLOTS]; //value of y as fraction of axis height
	double YVariable[_MAX_PLOTS]; //actual value of Y variable, in appropriate units for plotting
	double VSoma = GetCompart(_Soma)->m_V;
	YVariable[0] = VSoma; //Soma voltage [mV]
	YVariable[1] = GetChan(_Soma, _I_CaT)->m_g(); //gMax for _I_CaT
	//YVariable[1] = GetCompart(_Hillock)->m_V; //Hillock voltage [mV]
	//YVariable[2] = GetDoc()->m_IInject[_Soma]; //Soma current injection [nA]
	//YVariable[2] = GetDoc()->m_dt;
	YVariable[2] = GetCompart(_Soma)->m_Ca;
	YVariable[3] = GetChan(_Soma, _I_KCa)->m_g(); //gMax for _I_CaT
	//YVariable[3] = GetChan(_Soma, _I_CaT)->m_I(VSoma); //Soma CaT current
	//YVariable[4] = GetChan(_Hillock, _I_Na)->m_gMax; //gMax for _I_Na
	YVariable[4] = GetDoc()->m_IInject[_Soma];
	//YVariable[5] = GetChan(_Soma, _I_KCa)->m_gMax; //gMax for _I_CaT
	//YVariable[5] = GetCompart(_Soma)->m_ITotal(InjSoma); //Total current (in or out???) of soma [nA]
	for (int i = 0; i < m_NumPlots; i++) {
		YValue[i] = (YVariable[i] - m_minY[i])/(m_maxY[i] - m_minY[i]);
	}
	for (i = 0; i < m_NumPlots; i++) {
		PlotsDC.SelectObject(m_pPenArray[i]);
		ThisY[i] = (m_BottomY[i] - (int)(YValue[i]*m_PlotHeight));
		//ThisY[i] = (m_ZeroY[i] - (int)(YValue[i]*0.5*m_PlotHeight));
		if (T == 0) {
			m_LastY[i] = ThisY[i];
		}
		PlotsDC.MoveTo(m_LastX, m_LastY[i]);
		PlotsDC.LineTo(ThisX, ThisY[i]);
		//PlotsDC.Ellipse(ThisX-3, ThisY[i]-3, ThisX+3, ThisY[i]+3);
		m_LastY[i] = ThisY[i];
	}
	m_LastX = ThisX;
}


void CPlotsDialog::ClearPlots()
{
	FitPictToFrame();
	//Clear screen
	//CDC* PlotsDC = ctrlPlotsPict().GetDC();
	CClientDC PlotsDC(&(ctrlPlotsPict()));
	PlotsDC.SelectStockObject(WHITE_BRUSH);
	PlotsDC.Rectangle(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
	//draw axes and time label
	CRect LabelRect;
	char str[50];
	for (int i = 0; i < m_NumPlots; i++) {
		//draw labels
		LabelRect.left = 0;
		LabelRect.top = m_rectPlot[i].bottom;
		LabelRect.right = m_rectPlot[i].right;
		LabelRect.bottom = (m_rectPlot[i].bottom + (int)(BOTTOM_MARGIN*m_rect.bottom));
		sprintf(str, "%g msec", ((double)m_screen_num + 1)*_TIME_PER_SCREEN);		
		PlotsDC.DrawText(str, LabelRect, DT_RIGHT);
		LabelRect.left = 0;
		LabelRect.top = (int)(m_rectPlot[i].bottom - 0.5*TEXT_HEIGHT);
		LabelRect.right = m_rectPlot[i].left;
		LabelRect.bottom = (int)(m_rectPlot[i].bottom + 0.5*TEXT_HEIGHT);
		sprintf(str, "%g", m_minY[i]);
		PlotsDC.DrawText(str, LabelRect, DT_RIGHT);
		LabelRect.left = 0;
		LabelRect.top = (int)(m_rectPlot[i].top - 0.5*TEXT_HEIGHT);
		LabelRect.right = m_rectPlot[i].left;
		LabelRect.bottom = (int)(m_rectPlot[i].top + 0.5*TEXT_HEIGHT);
		sprintf(str, "%g", m_maxY[i]);
		PlotsDC.DrawText(str, LabelRect, DT_RIGHT);
		//Calculate ranges for y's


		//draw axes
		PlotsDC.SelectObject(m_pPenArray[GRAY_PEN]);
		for (int j = 0; j < NUM_VERT_SECT; j++) {
			PlotsDC.MoveTo(m_rectPlot[i].left, m_rectPlot[i].top + (int)(((double)j/NUM_VERT_SECT)*m_PlotHeight));
			PlotsDC.LineTo(m_rectPlot[i].right, m_rectPlot[i].top + (int)(((double)j/NUM_VERT_SECT)*m_PlotHeight));
		}
		for (j = 0; j < NUM_HORIZ_SECT; j++) {
			PlotsDC.MoveTo(m_rectPlot[i].left + (int)(((double)j/NUM_HORIZ_SECT)*m_PlotWidth), m_rectPlot[i].top);
			PlotsDC.LineTo(m_rectPlot[i].left + (int)(((double)j/NUM_HORIZ_SECT)*m_PlotWidth), m_rectPlot[i].bottom);
		}
		PlotsDC.SelectStockObject(BLACK_PEN);
		//PlotsDC.MoveTo(m_rectPlot[i].left, m_ZeroY[i]);
		//PlotsDC.LineTo(m_rectPlot[i].right, m_ZeroY[i]);
		PlotsDC.MoveTo(m_rectPlot[i].left, m_rectPlot[i].top);
		PlotsDC.LineTo(m_rectPlot[i].left, m_rectPlot[i].bottom);
		PlotsDC.LineTo(m_rectPlot[i].right, m_rectPlot[i].bottom);
		PlotsDC.LineTo(m_rectPlot[i].right, m_rectPlot[i].top);
		PlotsDC.LineTo(m_rectPlot[i].left, m_rectPlot[i].top);
	}
}

void CPlotsDialog::FitPictToFrame()
{
	//size picture box to size of dialog box
	GetClientRect(&m_rect);
	ctrlPlotsPict().SetWindowPos(&wndTop, (int)(0.02*m_rect.right), (int)(0.02*m_rect.bottom),
				(int)(0.96*m_rect.right), (int)(0.96*m_rect.bottom), SWP_NOZORDER);
	UpdateWindow();
	
	//size areas where the individual plots will appear
	ctrlPlotsPict().GetClientRect(&m_rect);
	m_PlotHeight = (int)((((1. - TOP_MARGIN)/((double)m_NumPlots)) - BOTTOM_MARGIN)*m_rect.bottom);
	m_PlotWidth = (int)((1. - RIGHT_MARGIN - LEFT_MARGIN) * m_rect.right);
	for (int i = 0; i < m_NumPlots; i++) {
		m_rectPlot[i].left = (int)(LEFT_MARGIN * m_rect.right);
		m_rectPlot[i].right = (m_rectPlot[i].left + m_PlotWidth);
		m_rectPlot[i].top = (int)(TOP_MARGIN*m_rect.bottom + 
							(i*(m_PlotHeight + (BOTTOM_MARGIN*m_rect.bottom))));
		m_rectPlot[i].bottom = (m_rectPlot[i].top + m_PlotHeight);
		m_BottomY[i] = m_rectPlot[i].bottom;
		//m_ZeroY[i] = (m_rectPlot[i].top + (int)(0.5*m_PlotHeight));
	}
	m_ZeroX = m_rectPlot[0].left;
}

/////////////////////////////////////////////////////////////////////////////
// CPlotsDialog serialization

void CPlotsDialog::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_rect;
		for (int i=0; i < _MAX_PLOTS; i++) 
			ar << m_rectPlot[i];	
		ar << m_NumPlots; 
		ar << m_PlotHeight;
		ar << m_PlotWidth;
		ar << m_screen_num;
		ar << m_screen_num_old;
		ar << m_ZeroX;
		ar << m_LastX;
		for (i=0; i < _MAX_PLOTS; i++) {
			ar << m_minY[i];
			ar << m_maxY[i];
			ar << m_BottomY[i];
			ar << m_LastY[i];
		}	
	}	
	else
	{
		ar >> m_rect;
		for (int i=0; i < _MAX_PLOTS; i++) 
			ar >> m_rectPlot[i];	
		ar >> m_NumPlots; 
		ar >> m_PlotHeight;
		ar >> m_PlotWidth;
		ar >> m_screen_num;
		ar >> m_screen_num_old;
		ar >> m_ZeroX;
		ar >> m_LastX;
		for (i=0; i < _MAX_PLOTS; i++) {
			ar >> m_minY[i];
			ar >> m_maxY[i];
			ar >> m_BottomY[i];
			ar >> m_LastY[i];
		}			
	}
}
