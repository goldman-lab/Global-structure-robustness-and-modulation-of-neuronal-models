// RunPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRunPage dialog

class CRunPage : public CPropertyPage
{
	DECLARE_SERIAL(CRunPage)

// Construction
public:
	CRunPage();
	~CRunPage();

// Attributes
	BOOL m_bChangeMade; //TRUE when apply button enabled
	BOOL m_bPaused; //true if user hits pause button
	double m_MaxInj_arr[_MAX_COMPARTS][_MAX_INJ_TIMES]; //array of injections into each compartment
	
// cell controls
	CButton& ctrlRunButt() { return *(CButton *) GetDlgItem (IDC_RUN_BUTTON); }
	CButton& ctrlPauseButt() { return *(CButton *) GetDlgItem (IDC_PAUSE_BUTTON); }
	CButton& ctrlEndButt() { return *(CButton *) GetDlgItem (IDC_END_RUN_BUTTON); }
	CButton& ctrlApplyButt() { return *(CButton *) GetDlgItem (IDC_APPLY_BUTT); }

// Dialog Data
	//{{AFX_DATA(CRunPage)
	enum { IDD = IDD_RUNPAGE };
	double	m_tmax;
	double	m_InjOffTime;
	double	m_InjOnTime;
	double	m_InjOnTime2;
	int		m_SaveFlag;
	int		m_DisplayFlag;
	double	m_MaxInjSoma;
	double	m_MaxInjSoma2;
	double	m_MaxInjHillock;
	double	m_MaxInjHillock2;
	double	m_dtmax;
	double	m_dtmin;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRunPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	public:
	void Serialize(CArchive& ar);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRunPage)
	afx_msg void OnRunButton();
	afx_msg void OnEndRunButton();
	afx_msg void OnPauseButton();
	afx_msg void OnApplyButt();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
