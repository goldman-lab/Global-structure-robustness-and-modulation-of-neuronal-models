// DisplayPage.cpp : implementation file
//

#include "stdafx.h"
#include "ZhengModelHeaders.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDisplayPage property page

IMPLEMENT_SERIAL(CDisplayPage, CPropertyPage, _VERSION_NUMBER)

CDisplayPage::CDisplayPage() : CPropertyPage(CDisplayPage::IDD)
{
	m_bChangeMade = FALSE;
	//{{AFX_DATA_INIT(CDisplayPage)
	m_NumPlotsEdit = _MAX_PLOTS;
	m_max1 = _MAXY;
	m_max2 = _MAXY;
	m_max3 = _MAXY;
	m_max4 = _MAXY;
	m_max5 = _MAXY;
	m_max6 = _MAXY;
	m_min1 = _MINY;
	m_min2 = _MINY;
	m_min3 = _MINY;
	m_min4 = _MINY;
	m_min5 = _MINY;
	m_min6 = _MINY;
	//}}AFX_DATA_INIT
}

CDisplayPage::~CDisplayPage()
{
}

void CDisplayPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDisplayPage)
	DDX_Text(pDX, IDC_NUM_PLOTS_EDIT, m_NumPlotsEdit);
	//DDX_Control(pDX, IDC_NUM_PLOTS_EDIT, m_NumPlotsEdit); //this was auto-generated code
	//DDV_MinMaxDouble(pDX, m_NumPlotsEdit, 0, _MAX_PLOTS); //maybe MinMaxInt?? look up in help
	DDX_Text(pDX, IDC_MAX1_EDIT, m_max1);
	DDV_MinMaxDouble(pDX, m_max1, -10000., 10000.);
	DDX_Text(pDX, IDC_MAX2_EDIT, m_max2);
	DDX_Text(pDX, IDC_MAX3_EDIT, m_max3);
	DDX_Text(pDX, IDC_MAX4_EDIT, m_max4);
	DDX_Text(pDX, IDC_MAX5_EDIT, m_max5);
	DDX_Text(pDX, IDC_MAX6_EDIT, m_max6);
	DDX_Text(pDX, IDC_MIN1_EDIT, m_min1);
	DDX_Text(pDX, IDC_MIN3_EDIT, m_min3);
	DDX_Text(pDX, IDC_MIN2_EDIT, m_min2);
	DDX_Text(pDX, IDC_MIN4_EDIT, m_min4);
	DDX_Text(pDX, IDC_MIN5_EDIT, m_min5);
	DDX_Text(pDX, IDC_MIN6_EDIT, m_min6);
	//}}AFX_DATA_MAP
	
}


BEGIN_MESSAGE_MAP(CDisplayPage, CPropertyPage)
	//{{AFX_MSG_MAP(CDisplayPage)
	ON_BN_CLICKED(IDC_APPLY_BUTT, OnApplyButt)
	ON_EN_CHANGE(IDC_NUM_PLOTS_EDIT, &CDisplayPage::OnChangeNumPlotsEdit)
	ON_EN_CHANGE(IDC_MAX1_EDIT, OnChangeMax1Edit)
	ON_EN_CHANGE(IDC_MAX2_EDIT, OnChangeMax2Edit)
	ON_EN_CHANGE(IDC_MAX3_EDIT, OnChangeMax3Edit)
	ON_EN_CHANGE(IDC_MAX4_EDIT, OnChangeMax4Edit)
	ON_EN_CHANGE(IDC_MAX5_EDIT, OnChangeMax5Edit)
	ON_EN_CHANGE(IDC_MAX6_EDIT, OnChangeMax6Edit)
	ON_EN_CHANGE(IDC_MIN1_EDIT, OnChangeMin1Edit)
	ON_EN_CHANGE(IDC_MIN2_EDIT, OnChangeMin2Edit)
	ON_EN_CHANGE(IDC_MIN3_EDIT, OnChangeMin3Edit)
	ON_EN_CHANGE(IDC_MIN4_EDIT, OnChangeMin4Edit)
	ON_EN_CHANGE(IDC_MIN5_EDIT, OnChangeMin5Edit)
	ON_EN_CHANGE(IDC_MIN6_EDIT, OnChangeMin6Edit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDisplayPage message handlers

BOOL CDisplayPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_bChangeMade = FALSE;
	ctrlApplyButt().EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDisplayPage::OnApplyButt() 
{
	//fill in plot dialog values
	GetPlotsDialog()->m_NumPlots = m_NumPlotsEdit;
	GetPlotsDialog()->m_minY[0] = m_min1;
	GetPlotsDialog()->m_minY[1] = m_min2;
	GetPlotsDialog()->m_minY[2] = m_min3;
	GetPlotsDialog()->m_minY[3] = m_min4;
	GetPlotsDialog()->m_minY[4] = m_min5;
	GetPlotsDialog()->m_minY[5] = m_min6;
	GetPlotsDialog()->m_maxY[0] = m_max1;
	GetPlotsDialog()->m_maxY[1] = m_max2;
	GetPlotsDialog()->m_maxY[2] = m_max3;
	GetPlotsDialog()->m_maxY[3] = m_max4;
	GetPlotsDialog()->m_maxY[4] = m_max5;
	GetPlotsDialog()->m_maxY[5] = m_max6;
	GetPlotsDialog()->ClearPlots();

	ctrlApplyButt().EnableWindow(FALSE);
	m_bChangeMade = FALSE;
	
}

void CDisplayPage::OnChangeNumPlotsEdit()
{
	if (IsWindowVisible()) {
		UpdateData(TRUE);
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
	}	
}

void CDisplayPage::OnChangeMax1Edit() 
{
	if (IsWindowVisible()) {
		UpdateData(TRUE);
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
	}		
}

void CDisplayPage::OnChangeMax2Edit() 
{
	if (IsWindowVisible()) {
		UpdateData(TRUE);
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
	}	
}

void CDisplayPage::OnChangeMax3Edit() 
{
	if (IsWindowVisible()) {
		UpdateData(TRUE);
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
	}
	
}

void CDisplayPage::OnChangeMax4Edit() 
{
	if (IsWindowVisible()) {
		UpdateData(TRUE);
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
	}
	
}

void CDisplayPage::OnChangeMax5Edit() 
{
	if (IsWindowVisible()) {
		UpdateData(TRUE);
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
	}
	
}

void CDisplayPage::OnChangeMax6Edit() 
{
	if (IsWindowVisible()) {
		UpdateData(TRUE);
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
	}
	
}

void CDisplayPage::OnChangeMin1Edit() 
{
	if (IsWindowVisible()) {
		UpdateData(TRUE);
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
	}
	
}

void CDisplayPage::OnChangeMin2Edit() 
{
	if (IsWindowVisible()) {
		UpdateData(TRUE);
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
	}
	
}

void CDisplayPage::OnChangeMin3Edit() 
{
	if (IsWindowVisible()) {
		UpdateData(TRUE);
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
	}
	
}

void CDisplayPage::OnChangeMin4Edit() 
{
	if (IsWindowVisible()) {
		UpdateData(TRUE);
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
	}
	
}

void CDisplayPage::OnChangeMin5Edit() 
{
	if (IsWindowVisible()) {
		UpdateData(TRUE);
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
	}
	
}

void CDisplayPage::OnChangeMin6Edit() 
{
	if (IsWindowVisible()) {
		UpdateData(TRUE);
		ctrlApplyButt().EnableWindow(TRUE);
		m_bChangeMade = TRUE;
	}
	
}

/////////////////////////////////////////////////////////////////////////////
// CDisplayPage serialization

void CDisplayPage::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_bChangeMade;
		//if (_VERSION_NUMBER == 4) {
			//ar >> m_NumPlotsEdit; //may need Version check on this line
		//}
		ar << m_max1;
		ar << m_max2;
		ar << m_max3;
		ar << m_max4;
		ar << m_max5;
		ar << m_max6;
		ar << m_min1;
		ar << m_min2;
		ar << m_min3;
		ar << m_min4;
		ar << m_min5;
		ar << m_min6;	
	}	
	else
	{
		ar >> m_bChangeMade;
		//if (_VERSION_NUMBER == 4) {
			//ar >> m_NumPlotsEdit; //may need Version check on this line
		//}
		ar >> m_max1;
		ar >> m_max2;
		ar >> m_max3;
		ar >> m_max4;
		ar >> m_max5;
		ar >> m_max6;
		ar >> m_min1;
		ar >> m_min2;
		ar >> m_min3;
		ar >> m_min4;
		ar >> m_min5;
		ar >> m_min6;
		if (IsWindow(m_hWnd)) {
			UpdateData(FALSE); //read member variable values into edit boxes
			ctrlApplyButt().EnableWindow(m_bChangeMade);
		}
	}
}


