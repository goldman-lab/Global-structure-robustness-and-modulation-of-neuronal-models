// ZhengModelDoc.h : interface of the CZhengModelDoc class
//
/////////////////////////////////////////////////////////////////////////////

class CZhengModelDoc : public CDocument
{
protected: // create from serialization only
	CZhengModelDoc();
	DECLARE_SERIAL(CZhengModelDoc)

// Attributes
public:
	CNeuron* m_pTheNeuron;
	double m_dt; //current step size
	double m_dtmin; //min step size--received from interface
	double m_dtmax; //max step size--received from interface
	double m_MaxInj[_MAX_COMPARTS][_MAX_INJ_TIMES]; //maximum injection per capacitance per area
	double m_IInject[_MAX_COMPARTS]; //injection per capacitance per area at any given time into compartments
	BOOL m_bEndRun; //set to true by interface when want to finish
// Operations
public:
	void RunEngine(double tmax, BOOL SaveFlag, BOOL DisplayFlag, double InjOnTime, double InjOnTime2, double InjOffTime);
	BOOL PeekAndPump(); //pauses to let user do stuff
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZhengModelDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CZhengModelDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CZhengModelDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
