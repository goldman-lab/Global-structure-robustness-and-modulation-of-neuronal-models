
///////////////
// plot.h

#define APSTUDIO_INVOKED
#undef APSTUDIO_READONLY_SYMBOLS
#include "resource.h"
#undef APSTUDIO_INVOKED
#include <afxtempl.h>

#ifndef _PLOT_H
	#define _PLOT_H
#endif

#define BLCK_PEN			0
#define RED_PEN				1
#define BLUE_PEN			2
#define GREEN_PEN			3
#define NUM_PENS			4
#define COLOR_BLACK			RGB(0,0,0)
#define COLOR_RED			RGB(255,0,0)
#define COLOR_BLUE			RGB(0,0,160)
#define COLOR_GREEN			RGB(0,100,0)
#define WM_CPLOT_DELETE		WM_USER+1
#define WM_CPLOT_SIZE		WM_USER+2
#define XSCALE				1.0
#define YSCALE				1.0
#define XTICKLEN			0.01
#define YTICKLEN			0.006
#define LEFTMARGIN			0.10
#define RIGHTMARGIN			0.99
#define TOPMARGIN			0.01
#define BOTMARGIN			0.90
#define _CPLOT_MAXTRACES	16

#define IDM_ZOOMIN			(_APS_NEXT_COMMAND_VALUE + 1)
#define IDM_EXPAND_VERT		(_APS_NEXT_COMMAND_VALUE + 2)
#define IDM_EXPAND_HORIZ	(_APS_NEXT_COMMAND_VALUE + 3)
#define IDM_SHRINK_VERT		(_APS_NEXT_COMMAND_VALUE + 4)
#define IDM_SHRINK_HORIZ	(_APS_NEXT_COMMAND_VALUE + 5)
#define IDM_BACKUP			(_APS_NEXT_COMMAND_VALUE + 6)
#define IDM_SCALE_AUTO		(_APS_NEXT_COMMAND_VALUE + 7)

class CZoom
{
public:
	CZoom (long AleftIDX=0,long ArightIDX=0,double AxScale=1.0,double AyScale=1.0,CRect& AselRect=CRect(0,0,0,0),
		double AvalBaseline=0);
public:
	long leftIDX;
	long rightIDX;
	double xScale;
	double yScale;
	CRect selectRect;
	double yBaseline;
};

class CPlot : public CFrameWnd
{
	DECLARE_DYNAMIC (CPlot)

private:
	BOOL m_bInvertOldSel;
	long m_botMargin;
	BOOL m_bSelectionExists;
	double m_lastDeviceX;
	double m_lastDeviceY[_CPLOT_MAXTRACES];
	double m_lastPageX;
	double m_lastPageY[_CPLOT_MAXTRACES];
	long m_leftIDX;
	long m_leftSelIDX;
	long m_leftMargin;
	double m_localYMax;
	double m_localYMin;
	int m_majorXTick;
	int m_majorYTick;
	int m_nPageSize;
	int m_nScrollPos;
	int m_numScreens;
	int m_numTraces;
	int m_numXTicks;
	int m_numYTicks;
	double m_oldXScale;
	double m_oldYScale;
	long m_origLeftIDX;
	long m_origRightIDX;
	CDC* m_pDC;
	CRect m_plotRect;
	CMenu* m_pMenuMain;
	CMenu* m_pMenuPopup;
	CWnd* m_pOwnerWnd;
	CPen* m_pPenArray[NUM_PENS];
	CList<CZoom,CZoom&>* m_pScaleHistory;
	//CList<double,double&>* m_pYHistory;
	CObArray m_yHistories;
	CRect m_rect;
	long m_rightIDX;
	double m_rightMargin;
	long m_rightSelIDX;
	CRect m_selectRect;
	double m_thisDeviceX;
	double m_thisDeviceY;
	double m_thisPageX;
	double m_thisPageY;
	double m_topMargin;
	short m_tracePens[_CPLOT_MAXTRACES];
	CString m_xAxisLabel;
	CString m_yAxisLabel;
	long m_yBaseline;
	double m_xScale;
	double m_xTickScale;
	double m_yScale;
	double m_yTickScale;

private:
	void DrawAxes (CDC* pDC);
	void InvertRect(CDC* pDC, LPCRECT lpRect);
	void Redraw(CDC *pDC);

public:
	CPlot(CWnd *pWnd=NULL, CString xLabel=CString("x-axis label"), CString yLabel=CString("y-axis label"),
		double xTickScale=XSCALE, double yTickScale=YSCALE);
	virtual ~CPlot();
	void AddPoint (int traceNum, double yVal, short penChoice);
	void AddTrace ();
	void Clear (CDC* pDC);
	void DrawSpike ();
	void Reset();
	void SetAxes (CString xLabel=CString("x-axis label"),CString yLabel=CString("y-axis label")) 
		{ m_xAxisLabel = xLabel; m_yAxisLabel = yLabel; }
	void SetScale (double xScale=XSCALE, double yScale=YSCALE)
		{ m_xTickScale = xScale; m_yTickScale = yScale; }

protected:
	afx_msg void OnChar( UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnClose( );
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHScroll(UINT, UINT, CScrollBar*);
	afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnLButtonUp( UINT nFlags, CPoint point );
	afx_msg void OnMouseMove( UINT nFlags, CPoint point );
	afx_msg void OnPaint( );
	afx_msg void OnPopup(UINT nID);
	afx_msg void OnRButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnSize( UINT nType, int cx, int cy );

	DECLARE_MESSAGE_MAP()
};