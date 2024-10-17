// ZhengModelView.h : interface of the CZhengModelView class
//
/////////////////////////////////////////////////////////////////////////////

class CZhengModelView : public CView
{
protected: // create from serialization only
	CZhengModelView();
	DECLARE_DYNCREATE(CZhengModelView)

// Attributes
public:
	CZhengModelDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZhengModelView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CZhengModelView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CZhengModelView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ZhengModelView.cpp
inline CZhengModelDoc* CZhengModelView::GetDocument()
   { return (CZhengModelDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
