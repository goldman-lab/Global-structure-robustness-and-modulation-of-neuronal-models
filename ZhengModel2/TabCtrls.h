// TabCtrls.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabCtrls

class CTabCtrls : public CPropertySheet
{
	DECLARE_DYNAMIC(CTabCtrls)

// Construction
public:
	void SaveWindowState();
	BOOL RestoreWindowState();
	CRunPage* m_pRunPage;
	CNeuronPage* m_pNeuronPage;
	CDisplayPage* m_pDisplayPage;
	CTabCtrls(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CTabCtrls(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabCtrls)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTabCtrls();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTabCtrls)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
