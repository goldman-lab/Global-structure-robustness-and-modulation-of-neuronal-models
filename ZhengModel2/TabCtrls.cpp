// TabCtrls.cpp : implementation file
//

#include "stdafx.h"
#include "GlobalDefs.h"
#include "ZhengModel.h"
#include "RunPage.h"
#include "NeuronPage.h"
#include "DisplayPage.h"
#include "TabCtrls.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabCtrls

IMPLEMENT_DYNAMIC(CTabCtrls, CPropertySheet)

CTabCtrls::CTabCtrls(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_pRunPage = new CRunPage;
	m_pNeuronPage = new CNeuronPage;
	m_pDisplayPage = new CDisplayPage;

	AddPage(m_pRunPage);
	AddPage(m_pNeuronPage);
	AddPage(m_pDisplayPage);
}

CTabCtrls::CTabCtrls(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_pRunPage = new CRunPage;
	m_pNeuronPage = new CNeuronPage;
	m_pDisplayPage = new CDisplayPage;
	
	
	AddPage(m_pRunPage);
	AddPage(m_pNeuronPage);
	AddPage(m_pDisplayPage);
}

CTabCtrls::~CTabCtrls()
{
	delete m_pRunPage;
	delete m_pNeuronPage;
	delete m_pDisplayPage;
}


BEGIN_MESSAGE_MAP(CTabCtrls, CPropertySheet)
	//{{AFX_MSG_MAP(CTabCtrls)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabCtrls message handlers

BOOL CTabCtrls::RestoreWindowState()
{
	CString version = "Version 1.0";

	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(&wp);

	if (((wp.rcNormalPosition.left = 
			AfxGetApp()->GetProfileInt (version, "CD.x1", -1)) != -1) &&
		((wp.rcNormalPosition.top = 
			AfxGetApp()->GetProfileInt (version, "CD.y1", -1)) != -1) &&
		((wp.rcNormalPosition.right = 
			AfxGetApp()->GetProfileInt (version, "CD.x2", -1)) != -1) &&
		((wp.rcNormalPosition.bottom = 
			AfxGetApp()->GetProfileInt (version, "CD.y2", -1)) != -1)) {
		
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

void CTabCtrls::SaveWindowState()
{
	CString version = "Version 1.0";

	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(&wp);

	AfxGetApp()->WriteProfileInt (version,"CD.y1",wp.rcNormalPosition.top);
	AfxGetApp()->WriteProfileInt (version,"CD.x2",wp.rcNormalPosition.right);
	AfxGetApp()->WriteProfileInt (version,"CD.y2",wp.rcNormalPosition.bottom);
	AfxGetApp()->WriteProfileInt (version,"CD.x1",wp.rcNormalPosition.left);
}