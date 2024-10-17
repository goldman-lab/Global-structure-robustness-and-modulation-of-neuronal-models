// DisplayPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDisplayPage dialog

class CDisplayPage : public CPropertyPage
{
	DECLARE_SERIAL(CDisplayPage)

// Construction
public:
	CDisplayPage();
	~CDisplayPage();

// Member vars

// display page controls
	CButton& ctrlApplyButt() { return *(CButton *) GetDlgItem (IDC_APPLY_BUTT); }


    BOOL m_bChangeMade; //TRUE if apply button enabled
// Dialog Data
	//{{AFX_DATA(CDisplayPage)
	enum { IDD = IDD_DISPLAYPAGE };
	int  m_NumPlotsEdit;
	double	m_max1;
	double	m_max2;
	double	m_max3;
	double	m_max4;
	double	m_max5;
	double	m_max6;
	double	m_min1;
	double	m_min3;
	double	m_min2;
	double	m_min4;
	double	m_min5;
	double	m_min6;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDisplayPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	public:
	void Serialize(CArchive& ar);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDisplayPage)
	afx_msg void OnApplyButt();
	afx_msg void OnChangeNumPlotsEdit();
	afx_msg void OnChangeMax1Edit();
	afx_msg void OnChangeMax2Edit();
	afx_msg void OnChangeMax3Edit();
	afx_msg void OnChangeMax4Edit();
	afx_msg void OnChangeMax5Edit();
	afx_msg void OnChangeMax6Edit();
	afx_msg void OnChangeMin1Edit();
	afx_msg void OnChangeMin2Edit();
	afx_msg void OnChangeMin3Edit();
	afx_msg void OnChangeMin4Edit();
	afx_msg void OnChangeMin5Edit();
	afx_msg void OnChangeMin6Edit();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	
};
