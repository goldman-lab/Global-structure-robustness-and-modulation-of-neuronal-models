// PlotsDialog.h : header file
//

//#define NUM_PLOTS 4  //IF USING AS FREE-STANDING PLOTTER CODE, THIS COULD BE USED

#define LEFT_MARGIN .1
#define RIGHT_MARGIN .05
#define TOP_MARGIN .02
#define BOTTOM_MARGIN .06
#define TEXT_HEIGHT 16  //I don't know how to change this but DrawText returns it
#define NUM_HORIZ_SECT 5 //for grid lines on plots
#define NUM_VERT_SECT 10 

//#define MAX_V (60.0)  //60 mV scale for voltage plots
//#define MAX_I (40.0) //40 nA scale
//#define MAX_Ca (20.0) //20uM scale

#define NUM_PEN 8
#define GRAY_PEN 7
#define RED RGB(255, 0, 0)
#define GREEN RGB(0, 150, 0)
#define BLUE RGB(0, 0, 255)
#define VIOLET RGB(200, 0, 200)
#define ORANGE RGB(200, 70, 0) 
#define BABY_BLUE RGB(0, 225, 225)
#define YELLOW RGB(255, 255, 0)
#define GRAY RGB(100, 100, 100)

/////////////////////////////////////////////////////////////////////////////
// CPlotsDialog dialog

class CPlotsDialog : public CDialog
{
// Construction
	DECLARE_SERIAL(CPlotsDialog)

public:
	CPlotsDialog() {}
	CPlotsDialog(CWnd* pParent);   // standard constructor
	~CPlotsDialog();
	void SaveWindowState();
	BOOL RestoreWindowState();

// Attributes
public:
	CRect m_rect;  //to avoid typing this all the time
	CRect m_rectPlot[_MAX_PLOTS];  //containers for plots
	static CPen* m_pPenArray[NUM_PEN];
	double m_minY[_MAX_PLOTS]; //min value for this plot, read in from interface
	double m_maxY[_MAX_PLOTS];
	int m_NumPlots; //number of plots
	int m_PlotHeight; //size of individual plots (not including labels)
	int m_PlotWidth;
	int m_screen_num; //number of _TIME_PER_SCREEN length screenfuls that have been run
	int m_screen_num_old;
	int m_ZeroX;      //location of point (0,0) on each plot
	//int m_ZeroY[_MAX_PLOTS];
	int m_BottomY[_MAX_PLOTS]; //location of bottom edge of plot rect
	int m_LastX;			//last point plotted
	int m_LastY[_MAX_PLOTS]; //last point plotted in each plot

//cell controls
	CStatic& ctrlPlotsPict() { return *(CStatic*) GetDlgItem (IDC_PLOTSPICTURE); }

// Operations
public:
	void DoPlots(double t);
	void FitPictToFrame();
	void ClearPlots(); 
	
// Dialog Data
	//{{AFX_DATA(CPlotsDialog)
	enum { IDD = IDD_PLOTSDIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlotsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	public:
	void Serialize(CArchive& ar);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPlotsDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
