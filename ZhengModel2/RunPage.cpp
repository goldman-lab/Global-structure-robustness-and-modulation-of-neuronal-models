// RunPage.cpp : implementation file
//

#include "stdafx.h"
#include "fstream"
#include "ZhengModelHeaders.h"
#include "Plot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRunPage property page

IMPLEMENT_SERIAL(CRunPage, CPropertyPage, _VERSION_NUMBER)

CRunPage::CRunPage() : CPropertyPage(CRunPage::IDD)
{
	m_bPaused = FALSE;
	m_bChangeMade = TRUE;
	for (int step = 0; step < _MAX_INJ_TIMES; step++) {
		for (int i = 0; i < _MAX_COMPARTS; i++) {
			switch (i) {
				case _Soma:
					m_MaxInj_arr[i][step] = _MAX_INJECT; break;
				case _Hillock:
					m_MaxInj_arr[i][step] = 0.0; break;
			}
		}
	}

	//{{AFX_DATA_INIT(CRunPage)
	m_tmax = _TMAX;
	m_InjOffTime = _INJECT_OFF_TIME;
	m_InjOnTime = _INJECT_ON_TIME;
	m_InjOnTime2 = _INJECT_ON_TIME2;
	m_SaveFlag = _SAVE_TO_FILE;
	m_DisplayFlag = _DISPLAY_FLAG;
	m_MaxInjSoma = _MAX_INJECT;
	m_MaxInjHillock = _MAX_INJECT;
	m_MaxInjSoma2 = _MAX_INJECT;
	m_MaxInjHillock2 = _MAX_INJECT;
	m_dtmax = _DT;
	m_dtmin = _DT;
	//}}AFX_DATA_INIT
}

CRunPage::~CRunPage()
{
}

void CRunPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRunPage)
	DDX_Text(pDX, IDC_TMAX_EDIT, m_tmax);
	DDV_MinMaxDouble(pDX, m_tmax, 0., 100000000.);
	DDX_Text(pDX, IDC_INJECT_OFF_EDIT, m_InjOffTime);
	DDV_MinMaxDouble(pDX, m_InjOffTime, 0., 1000000.);
	DDX_Text(pDX, IDC_INJECT_ON_EDIT, m_InjOnTime);
	DDV_MinMaxDouble(pDX, m_InjOnTime, 0., 1000000.);
	DDX_Text(pDX, IDC_INJECT_ON_EDIT2, m_InjOnTime2);
	DDX_Radio(pDX, IDC_SAVE_RADIO, m_SaveFlag);
	DDX_Radio(pDX, IDC_DISPLAY_RADIO, m_DisplayFlag);
	DDX_Text(pDX, IDC_MAX_INJECT_SOMA_EDIT, m_MaxInjSoma);
	DDV_MinMaxDouble(pDX, m_MaxInjSoma, -100000., 100000.);
	DDX_Text(pDX, IDC_MAX_INJECT_SOMA_EDIT2, m_MaxInjSoma2);
	DDV_MinMaxDouble(pDX, m_MaxInjSoma2, -1000000., 1000000.);
	DDX_Text(pDX, IDC_MAX_INJECT_HILLOCK_EDIT, m_MaxInjHillock);
	DDV_MinMaxDouble(pDX, m_MaxInjHillock, -1000., 1000.);
	DDX_Text(pDX, IDC_MAX_INJECT_HILLOCK_EDIT2, m_MaxInjHillock2);
	DDV_MinMaxDouble(pDX, m_MaxInjHillock2, -1000000., 1000000.);
	DDX_Text(pDX, IDC_DTMAX_EDIT, m_dtmax);
	DDX_Text(pDX, IDC_DTMIN_EDIT, m_dtmin);
	//}}AFX_DATA_MAP

	//convert currents to be per capacitance per unit area; 0,1 refer to which injection step
	if (IsWindowVisible()) {
		m_MaxInj_arr[_Soma][0] = m_MaxInjSoma/(S_TO_mS * GetCompart(_Soma)->m_C * GetCompart(_Soma)->m_Area);
		m_MaxInj_arr[_Hillock][0] = m_MaxInjHillock/(S_TO_mS * GetCompart(_Hillock)->m_C * GetCompart(_Hillock)->m_Area);
		m_MaxInj_arr[_Soma][1] = m_MaxInjSoma2/(S_TO_mS * GetCompart(_Soma)->m_C * GetCompart(_Soma)->m_Area);
		m_MaxInj_arr[_Hillock][1] = m_MaxInjHillock2/(S_TO_mS * GetCompart(_Hillock)->m_C * GetCompart(_Hillock)->m_Area);
	}
}


BEGIN_MESSAGE_MAP(CRunPage, CPropertyPage)
	//{{AFX_MSG_MAP(CRunPage)
	ON_BN_CLICKED(IDC_RUN_BUTTON, OnRunButton)
	ON_BN_CLICKED(IDC_END_RUN_BUTTON, OnEndRunButton)
	ON_BN_CLICKED(IDC_PAUSE_BUTTON, OnPauseButton)
	ON_BN_CLICKED(IDC_APPLY_BUTT, OnApplyButt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRunPage message handlers

BOOL CRunPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	UpdateData(FALSE);
	m_bChangeMade = FALSE;
	ctrlApplyButt().EnableWindow(FALSE);
	ctrlPauseButt().EnableWindow(FALSE);
	ctrlEndButt().EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRunPage::OnApplyButt() 
{
	UpdateData(TRUE);	
	GetDoc()->m_dtmin = m_dtmin;
	GetDoc()->m_dtmax = m_dtmax;
	for (int step = 0; step < _MAX_INJ_TIMES; step++) {
		for (int i = 0; i < _MAX_COMPARTS; i++) {
			GetDoc()->m_MaxInj[i][step] = m_MaxInj_arr[i][step];
		}
	}
	m_bChangeMade = FALSE;
	ctrlApplyButt().EnableWindow(FALSE);
	
}

void CRunPage::OnRunButton() 
{
	
	//UpdateData(TRUE);
	
	ctrlRunButt().EnableWindow(FALSE);
	ctrlPauseButt().EnableWindow(TRUE);
	ctrlEndButt().EnableWindow(TRUE);
	/*
	ofstream mInfFile("mInf.dat");
	ofstream hInfFile("hInf.dat");
	ofstream TauMFile("TauM.dat");
	ofstream TauHFile("TauH.dat");
	for (int V = -100; V <= 100; V++) {
		mInfFile << V << " ";
		TauMFile << V << " ";
		hInfFile << V << " ";
		TauHFile << V << " ";
		for (int i = _I_Leak; i < _MAX_CHANNELS; i++) {
			if (GetChan(_Soma, i)->m_p != 0) {
				mInfFile << GetChan(_Soma, i)->m_m_inf((double)V, _CA_REST) << " ";
				TauMFile << GetChan(_Soma, i)->m_tau_m((double)V) << " ";
			}
			if (GetChan(_Soma, i)->m_q != 0) {
				hInfFile << GetChan(_Soma, i)->m_h_inf((double)V) << " ";
				TauHFile << GetChan(_Soma, i)->m_tau_h((double)V) << " ";
			}
		}
		mInfFile << endl;
		TauMFile << endl;
		hInfFile << endl;
		TauHFile << endl;
	}
	*/
	GetDoc()->RunEngine(m_tmax, (BOOL)m_SaveFlag, (BOOL)m_DisplayFlag, m_InjOnTime, m_InjOnTime2, m_InjOffTime);
	ctrlRunButt().EnableWindow(TRUE);
	ctrlPauseButt().EnableWindow(FALSE);
	ctrlEndButt().EnableWindow(FALSE);
	
}

BOOL CRunPage::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	UINT nID = LOWORD(wParam);
	INT nCODE = HIWORD(wParam);

	if ((nID == IDC_DTMIN_EDIT AND nCODE == EN_CHANGE) OR
		(nID == IDC_DTMAX_EDIT AND nCODE == EN_CHANGE) OR
		(nID == IDC_DISPLAY_RADIO AND nCODE == BN_CLICKED) OR
		(nID == IDC_DISPLAY_RADIOdummy AND nCODE == BN_CLICKED) OR
		(nID == IDC_SAVE_RADIO AND nCODE == BN_CLICKED) OR
		(nID == IDC_SAVE_RADIOdummy AND nCODE == BN_CLICKED) OR
		(nID == IDC_INJECT_ON_EDIT AND nCODE == EN_CHANGE) OR
		(nID == IDC_INJECT_ON_EDIT2 AND nCODE == EN_CHANGE) OR
		(nID == IDC_INJECT_OFF_EDIT AND nCODE == EN_CHANGE) OR
		(nID == IDC_TMAX_EDIT AND nCODE == EN_CHANGE) OR
		(nID == IDC_MAX_INJECT_SOMA_EDIT AND nCODE == EN_CHANGE) OR
		(nID == IDC_MAX_INJECT_SOMA_EDIT2 AND nCODE == EN_CHANGE) OR
		(nID == IDC_MAX_INJECT_HILLOCK_EDIT AND nCODE == EN_CHANGE) OR
		(nID == IDC_MAX_INJECT_HILLOCK_EDIT2 AND nCODE == EN_CHANGE)) {
		ctrlApplyButt().EnableWindow(TRUE);
	}
	
	return CPropertyPage::OnCommand(wParam, lParam);
}

void CRunPage::OnPauseButton() 
{
	m_bPaused = !m_bPaused;
	MSG msg;

	while (m_bPaused) {
		::PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE);
		AfxGetApp()->PumpMessage();
	}
	
}

void CRunPage::OnEndRunButton() 
{
	GetDoc()->m_bEndRun = TRUE;
	
}

/////////////////////////////////////////////////////////////////////////////
// CRunPage serialization

void CRunPage::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_bPaused;
		ar << m_bChangeMade;
		for (int step = 0; step < _MAX_INJ_TIMES; step++) {
			for (int i = 0; i < _MAX_COMPARTS; i++) {
				ar << m_MaxInj_arr[i][step];
			}
		}
		ar << m_dtmin;
		ar << m_dtmax;
		ar << m_tmax;
		ar << m_InjOffTime;
		ar << m_InjOnTime;
		ar << m_InjOnTime2;
		ar << m_SaveFlag;
		ar << m_DisplayFlag;
		ar << m_MaxInjSoma;
		ar << m_MaxInjHillock;
		ar << m_MaxInjSoma2;
		ar << m_MaxInjHillock2;
	}	
	else
	{
		ar >> m_bPaused;
		ar >> m_bChangeMade;
		for (int step = 0; step < _MAX_INJ_TIMES; step++) {
			for (int i = 0; i < _MAX_COMPARTS; i++) {
				ar >> m_MaxInj_arr[i][step];
			}
		}
		ar >> m_dtmin;
		ar >> m_dtmax;
		ar >> m_tmax;
		ar >> m_InjOffTime;
		ar >> m_InjOnTime;
		ar >> m_InjOnTime2;
		ar >> m_SaveFlag;
		ar >> m_DisplayFlag;
		ar >> m_MaxInjSoma;
		ar >> m_MaxInjHillock;
		ar >> m_MaxInjSoma2;
		ar >> m_MaxInjHillock2;
		if (IsWindow(m_hWnd)) {
			UpdateData(FALSE); //read member variable values into edit boxes
		}
	}
}



