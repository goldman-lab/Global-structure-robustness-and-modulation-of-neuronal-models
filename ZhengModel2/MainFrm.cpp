// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ZhengModelHeaders.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CZhengModelApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_ENDSESSION()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
	//for (int i=0; i<_MAX_PLOTS; i++)
		//delete m_PlotArray[i];
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	// Create main control dialog
    //DELETE RestoreWindowState() LINES IF WINDOW SIZES GET MESSED UP!!!!
	m_pCtrlSheet = new CTabCtrls("Main Controls", this, 0);
	m_pCtrlSheet->Create(this);
	m_pCtrlSheet->RestoreWindowState();
	m_pCtrlSheet->ShowWindow(SW_SHOWNORMAL);
	// Create plotting dialog
	m_pPlotsDialog = new CPlotsDialog(this);
	m_pPlotsDialog->Create(IDD_PLOTSDIALOG, this);
	m_pPlotsDialog->RestoreWindowState();
	m_pPlotsDialog->ShowWindow(SW_SHOWNORMAL);
	// Create plotting windows
	/*char str[40];
	m_PlotArray.SetSize(0, _MAX_PLOTS);
	for (int i = 0; i < _MAX_PLOTS; i++) {
		m_PlotArray.Add(new CPlot(this, CString ("time")));
		sprintf(str, "Kick ass plot #%d thanks to Josh", i+1);
		((CPlot*) m_PlotArray[i])->Create(NULL, str, WS_OVERLAPPEDWINDOW|WS_HSCROLL);
		((CPlot*) m_PlotArray[i])->ShowWindow(SW_SHOWNORMAL);
	}*/
	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CMDIFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnEndSession(BOOL bEnding) 
{
	if (bEnding) {
		SaveWindowState();
		m_pCtrlSheet->SaveWindowState();
		m_pPlotsDialog->SaveWindowState();
		//delete m_pCtrlSheet;
		//delete m_pPlotsDialog;
	}

	CMDIFrameWnd::OnEndSession(bEnding);
}

BOOL CMainFrame::RestoreWindowState()
{
	CString version = "Version 1.0";

	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(&wp);

	if (((wp.flags = 
			theApp.GetProfileInt (version, "flags", -1)) != -1) &&
		((wp.showCmd = 
			theApp.GetProfileInt (version, "showCmd", -1)) != -1) &&
		((wp.rcNormalPosition.left = 
			theApp.GetProfileInt (version, "MF.x1", -1)) != -1) &&
		((wp.rcNormalPosition.top = 
			theApp.GetProfileInt (version, "MF.y1", -1)) != -1) &&
		((wp.rcNormalPosition.right = 
			theApp.GetProfileInt (version, "MF.x2", -1)) != -1) &&
		((wp.rcNormalPosition.bottom = 
			theApp.GetProfileInt (version, "MF.y2", -1)) != -1)) {

		wp.rcNormalPosition.left = min(wp.rcNormalPosition.left,
			::GetSystemMetrics(SM_CXSCREEN) - 
			::GetSystemMetrics(SM_CXICON));
		wp.rcNormalPosition.top = min (wp.rcNormalPosition.top,
			::GetSystemMetrics(SM_CYSCREEN) -
			::GetSystemMetrics(SM_CYICON));

		SetWindowPlacement (&wp);
		return TRUE;
	}
	return FALSE;
}

void CMainFrame::SaveWindowState()
{
	CString version = "Version 1.0";

	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(&wp);

	theApp.WriteProfileInt (version,"flags",wp.flags);
	theApp.WriteProfileInt (version,"showCmd",wp.showCmd);
	theApp.WriteProfileInt (version,"MF.x1",wp.rcNormalPosition.left);
	theApp.WriteProfileInt (version,"MF.y1",wp.rcNormalPosition.top);
	theApp.WriteProfileInt (version,"MF.x2",wp.rcNormalPosition.right);
	theApp.WriteProfileInt (version,"MF.y2",wp.rcNormalPosition.bottom);
}


void CMainFrame::OnClose() 
{
	SaveWindowState();
	m_pCtrlSheet->SaveWindowState();
	m_pPlotsDialog->SaveWindowState();
	m_pCtrlSheet->EndDialog(0);
	::SendMessage(m_pCtrlSheet->m_hWnd, WM_DESTROY, 0, 0);
	::SendMessage(m_pCtrlSheet->m_hWnd, WM_NCDESTROY, 0, 0);
	delete m_pCtrlSheet;
	m_pPlotsDialog->EndDialog(0);
	::SendMessage(m_pPlotsDialog->m_hWnd, WM_DESTROY, 0, 0);
	::SendMessage(m_pPlotsDialog->m_hWnd, WM_NCDESTROY, 0, 0);
	delete m_pPlotsDialog;
	CMDIFrameWnd::OnClose();
}

void CMainFrame::PositionPlots()
{
	CString version = "Version 1.0";
	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	CWnd *pWnd;

	//Move plot display window to stored position
	pWnd = GetViewWnd(); 
	pWnd->GetWindowPlacement(&wp);
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

		pWnd->SetWindowPlacement (&wp);
	}
}

