
////////////////
// plot.cpp

#include "stdafx.h"
#include "plot.h"

IMPLEMENT_DYNAMIC (CPlot, CFrameWnd)

BEGIN_MESSAGE_MAP(CPlot, CFrameWnd)
	ON_WM_CHAR()
	ON_WM_CLOSE()
	ON_COMMAND_RANGE(IDM_ZOOMIN,IDM_SCALE_AUTO,OnPopup)
	ON_WM_CREATE()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_SIZE()
END_MESSAGE_MAP()

CZoom::CZoom (long AleftIDX,long ArightIDX,double AxScale,double AyScale,CRect& AselRect,double AyBaseline)
{
	leftIDX     = AleftIDX; 
	rightIDX    = ArightIDX; 
	xScale      = AxScale; 
	yScale      = AyScale; 
	selectRect  = AselRect; 
	yBaseline   = AyBaseline;
}

CPlot::CPlot (CWnd *pWnd, CString xLabel, CString yLabel, double xTickScale, double yTickScale) 
{
	int i=0;

	m_bInvertOldSel = FALSE;
	m_botMargin = 0;
	m_bSelectionExists = FALSE;
	m_lastDeviceX = 0;
	for (i=0; i<_CPLOT_MAXTRACES; i++) {
		m_lastDeviceY[i] = 0;
	}
	m_lastPageX = 0;
	for (i=0; i<_CPLOT_MAXTRACES; i++) {
		m_lastPageY[i] = 0;
	}
	m_leftIDX = 0;
	m_leftMargin = 0;
	m_leftSelIDX = 0;
	m_localYMax = 10e-300;
	m_localYMin = 10e+300;
	m_majorXTick = 5;
	m_majorYTick = 5;
	m_nPageSize = 0;
	m_nScrollPos = 0;
	m_numScreens = 1;
	m_numTraces = 0;
	m_numXTicks = 20;
	m_numYTicks = 20;
	m_oldXScale = XSCALE;
	m_oldYScale = YSCALE;
	m_origLeftIDX = 0;
	m_origRightIDX = 0;
	m_plotRect = CRect(0,0,0,0);
	m_pMenuMain = new CMenu;
	m_pMenuPopup = new CMenu;
	m_pOwnerWnd = pWnd;
	m_pPenArray[0] = new CPen(PS_SOLID,1,COLOR_BLACK);
	m_pPenArray[1] = new CPen(PS_SOLID,1,COLOR_RED);
	m_pPenArray[2] = new CPen(PS_SOLID,1,COLOR_BLUE);
	m_pPenArray[3] = new CPen(PS_SOLID,1,COLOR_GREEN);
	m_pScaleHistory = new CList<CZoom,CZoom&>(5);
	// m_pYHistory = new CList<double,double&>(500);
	m_yHistories.SetSize(0);
	m_rect = CRect(0,0,0,0);
	m_rightIDX = 0;
	m_rightMargin = 0;
	m_rightSelIDX = 0;
	m_selectRect = CRect(0,0,0,0);
	m_thisDeviceX = 0;
	m_thisDeviceY = 0;
	m_thisPageX = 0;
	m_thisPageY = 0;
	m_topMargin = 0;
	m_xAxisLabel = xLabel;
	m_xScale = XSCALE;
	m_xTickScale = xTickScale;
	m_yAxisLabel = yLabel;
	m_yBaseline = 0;
	m_yScale = YSCALE;
	m_yTickScale = yTickScale;
}

CPlot::~CPlot () 
{
	int i=0;
	for (i=0; i<NUM_PENS; i++)
		delete m_pPenArray[i];
	for (i=0; i<m_numTraces; i++) {
		((CList<double,double&> *)m_yHistories[i])->RemoveAll();
		delete m_yHistories[i];
	}
	m_pScaleHistory->RemoveAll();
	delete m_pScaleHistory;
	delete m_pMenuMain;
	delete m_pMenuPopup;
}


void CPlot::AddPoint (int traceNum, double yVal, short penChoice) 
{
	m_pDC = GetDC();
	if (traceNum < 0 || traceNum >= m_numTraces) return;
	((CList<double,double&> *)m_yHistories[traceNum])->AddTail(yVal);
	m_tracePens[traceNum] = penChoice;
	if (yVal>m_localYMax) m_localYMax=yVal;
	if (yVal<m_localYMin) m_localYMin=yVal; 
	m_pDC->SelectObject(m_pPenArray[penChoice]);
	m_thisPageY = m_thisDeviceY = (long)(m_yBaseline - (yVal * m_yScale));
	if (traceNum == m_numTraces-1) {
		m_lastPageX   = m_thisPageX;
		m_lastDeviceX = m_thisDeviceX;
		m_thisPageX += m_xScale;
		m_thisDeviceX += m_xScale;
		m_rightIDX++;
		m_origRightIDX = m_rightIDX;
		if (m_thisDeviceX > m_rightMargin) {
			// for memory safety, uncomment these lines to delete stored y-values at end of each screen.
			// remove this line when h-scrolling is enabled
			// for (int i=0; i<m_numTraces; i++)
			//	((CList<double,double&> *)m_yHistories[i])->RemoveAll();
			m_numScreens++;
			//m_pDC->SetWindowOrg((int)m_grandX,0);
			m_thisDeviceX = m_leftMargin + m_xScale;
			m_lastDeviceX = m_leftMargin;
			Clear(m_pDC);
			m_leftIDX = m_rightIDX;
			m_origLeftIDX = m_leftIDX;
			m_bSelectionExists = FALSE;
			m_pScaleHistory->RemoveAll();
			m_oldXScale = m_xScale;
			m_oldYScale = m_yScale;
			m_localYMax = 10e-300;
			m_localYMin = 10e+300;
			// Update scrollbar
			SCROLLINFO si;
			GetScrollInfo(SB_HORZ,&si);
			int ptsPerScreen = (int)((double)m_plotRect.Width()/m_xScale);
			si.nMax = m_numScreens * ptsPerScreen;
			si.nPos = (m_numScreens-1) * ptsPerScreen;
			si.nPage = ptsPerScreen;
			SetScrollInfo(SB_HORZ,&si,TRUE);
			m_nPageSize = si.nPage;
			m_nScrollPos = si.nPos;
		}
	}
	m_pDC->MoveTo((long)m_lastDeviceX, (long)m_lastDeviceY[traceNum]);
	m_pDC->LineTo((long)m_thisDeviceX, (long)m_thisDeviceY);
	m_lastDeviceY[traceNum] = m_thisDeviceY;
}

void CPlot::AddTrace()
{
	m_yHistories.Add(new CList<double, double&>(500));
	m_numTraces = m_yHistories.GetSize();
}

void CPlot::Clear (CDC *pDC) 
{
	CPen* pCurPen = m_pDC->GetCurrentPen();
	CBrush* pCurBrush = m_pDC->GetCurrentBrush();

	m_pDC->SelectStockObject(WHITE_BRUSH);
	m_pDC->SelectStockObject(WHITE_PEN);
	m_pDC->Rectangle(&m_rect);
	DrawAxes(m_pDC);

	m_pDC->SelectObject(pCurPen);
	m_pDC->SelectObject(pCurBrush);
}

void CPlot::DrawAxes (CDC *pDC) 
{
	CPen* pCurPen = m_pDC->GetCurrentPen();
	CBrush* pCurBrush = m_pDC->GetCurrentBrush();

	// Draw frame with dotted baseline
	m_pDC->SelectStockObject(BLACK_PEN);
	m_pDC->SelectStockObject(WHITE_BRUSH);
	m_pDC->Rectangle(
		(long)(LEFTMARGIN*m_rect.right),
		(long)(TOPMARGIN*m_rect.bottom),
		(long)(RIGHTMARGIN*m_rect.right),
		(long)(BOTMARGIN*m_rect.bottom));
	CPen pen(PS_DOT,1,RGB(0,0,0));
	m_pDC->SelectObject(&pen);
	m_pDC->MoveTo((long)(LEFTMARGIN*m_rect.right),m_yBaseline);
	m_pDC->LineTo((long)(RIGHTMARGIN*m_rect.right)-1,m_yBaseline);

	// Draw tickmarks
	int i=0;
	int tickLen=0;
	double xTickSpace = (RIGHTMARGIN-LEFTMARGIN)*m_rect.Width()/(m_numXTicks-1);
	double yTickSpace = (BOTMARGIN-TOPMARGIN)*m_rect.Height()/(m_numYTicks-1);
	m_pDC->SelectStockObject(BLACK_PEN);
	for (i=1; i<=m_numXTicks; i++) {
		tickLen = (int)(XTICKLEN*m_rect.Height());
		if (i%m_majorXTick==0) tickLen *= 2;
		m_pDC->MoveTo((long)(m_leftMargin+i*xTickSpace),m_botMargin-1);
		if ( (long)(m_leftMargin+i*xTickSpace) < RIGHTMARGIN*m_rect.right)
			m_pDC->LineTo((long)(m_leftMargin+i*xTickSpace),m_botMargin-tickLen-1);
	}
	i=1;
	while ( (long)(m_yBaseline-i*yTickSpace) > TOPMARGIN*m_rect.bottom) {
		tickLen = (int)(YTICKLEN*m_rect.Width());
		if (i%m_majorYTick==0) tickLen *= 2;
		m_pDC->MoveTo(m_leftMargin,(long)(m_yBaseline-i*yTickSpace));
		m_pDC->LineTo(m_leftMargin+tickLen,(long)(m_yBaseline-i*yTickSpace));
		i++;
	}
	i=1;
	while ( (long)(m_yBaseline+i*yTickSpace) < (BOTMARGIN*m_rect.bottom - yTickSpace)) {
		tickLen = (int)(YTICKLEN*m_rect.Width());
		if (i%m_majorYTick==0) tickLen *= 2;
		m_pDC->MoveTo(m_leftMargin,(long)(m_yBaseline+i*yTickSpace));
		m_pDC->LineTo(m_leftMargin+tickLen,(long)(m_yBaseline+i*yTickSpace));
		i++;
	}

	// Draw axis labels
	HFONT hfnt, hfntPrev; 
	PLOGFONT plf = (PLOGFONT) LocalAlloc(LPTR, sizeof(LOGFONT)); 
 	lstrcpy(plf->lfFaceName, "Arial"); 
	plf->lfWeight = FW_NORMAL; 
	plf->lfHeight = (long)(0.40*(1-BOTMARGIN)*m_rect.bottom);
  	m_pDC->SetBkMode(TRANSPARENT); 
	plf->lfEscapement = 900; 
	hfnt = CreateFontIndirect(plf); 
	hfntPrev = (HFONT)SelectObject(m_pDC->m_hDC,hfnt);
	CSize labSize = m_pDC->GetTextExtent(m_yAxisLabel);
	m_pDC->LPtoDP(&labSize);
	m_pDC->TextOut(2, (long)(m_plotRect.top + (double)m_plotRect.Height()/2 +((double)labSize.cx)/2), m_yAxisLabel); 
	DeleteObject(hfnt); 
	plf->lfEscapement = 0; 
	hfnt = CreateFontIndirect(plf);
	SelectObject(m_pDC->m_hDC,hfnt);
	labSize = m_pDC->GetTextExtent(m_xAxisLabel);
	m_pDC->LPtoDP(&labSize);
	m_pDC->TextOut((long)(0.50*m_rect.right-((double)labSize.cx)/2),m_rect.bottom-labSize.cy-1, m_xAxisLabel); 

	// Draw tick labels
	double val=0.0;
	char str[12];
	for (i=1; i<=m_numXTicks; i++) {
		if (i%m_majorXTick==0) {
			val = (xTickSpace/m_xTickScale) * ((m_numScreens-1)*(m_numXTicks-1) + i);
			sprintf (str,"%.2f",val);
			labSize = m_pDC->GetTextExtent(CString(str));
			m_pDC->LPtoDP(&labSize);
			if (m_leftMargin+i*xTickSpace+labSize.cx < m_rect.right)
				m_pDC->TextOut((long)(m_leftMargin+i*xTickSpace-((double)labSize.cx)/2),m_botMargin-1,str);
		}
	}
	i=0;
	do {
		if (i==0) {
			sprintf (str,"0.00");
			labSize = m_pDC->GetTextExtent(CString(str));
			m_pDC->LPtoDP(&labSize);
			m_pDC->TextOut(m_leftMargin-labSize.cx-1,(long)(m_yBaseline-((double)labSize.cy)/2),str);
		}
		else if (i>0 && i%m_majorYTick==0) {
			val = (double)i*yTickSpace/m_yTickScale;
			sprintf (str,"%.2f",val);
			labSize = m_pDC->GetTextExtent(CString(str));
			m_pDC->LPtoDP(&labSize);
			m_pDC->TextOut(m_leftMargin-labSize.cx-1,(long)(m_yBaseline-i*yTickSpace-((double)labSize.cy)/2),str);
		}
		i++;
	} while (m_yBaseline-i*yTickSpace-labSize.cy > m_rect.top);
	i=1;
	while (m_yBaseline+i*yTickSpace-labSize.cy < BOTMARGIN*m_rect.bottom - yTickSpace) {
		if (i%m_majorYTick==0) {
			val = (double)i*yTickSpace/m_yTickScale;
			sprintf (str,"-%.2f",val);
			labSize = m_pDC->GetTextExtent(CString(str));
			m_pDC->LPtoDP(&labSize);
			m_pDC->TextOut(m_leftMargin-labSize.cx-1,(long)(m_yBaseline+i*yTickSpace-((double)labSize.cy)/2),str);
		}
		i++;
	}

	DeleteObject(hfnt); 
	m_pDC->SetBkMode(OPAQUE); 
 	LocalFree((LOCALHANDLE) plf); 
	m_pDC->SelectObject(hfntPrev); 
	m_pDC->SelectObject(pCurPen);

	m_pDC->SelectObject(pCurBrush);
}

void CPlot::DrawSpike ()
{
	// CClientDC dc(this);
	m_pDC->SelectObject(m_pPenArray[BLUE_PEN]);
	m_pDC->MoveTo((long)m_thisDeviceX,(long)m_thisDeviceY);
	m_pDC->LineTo((long)m_thisDeviceX,(long)m_thisDeviceY-30);
}

void CPlot::InvertRect(CDC* pDC, LPCRECT lpRect)
{
	int nOldMode = m_pDC->SetROP2 (R2_NOT);
	m_pDC->SelectStockObject(NULL_BRUSH);
	m_pDC->Rectangle(lpRect);
	m_pDC->SetROP2(nOldMode);
}

void CPlot::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	CFrameWnd::OnChar(nChar, nRepCnt, nFlags);
	switch (nChar) {
	case (0x1b):	// escape key
		PostMessage(WM_COMMAND,IDM_BACKUP,0);
		break;
	}
}

void CPlot::OnClose( )
{
	WPARAM wParam;
	wParam = WM_CPLOT_DELETE;

	NMHDR nmhdr;
	nmhdr.hwndFrom = m_hWnd;
	nmhdr.idFrom = 0;
	nmhdr.code = WM_CPLOT_DELETE;

	::SendMessage(m_pOwnerWnd->m_hWnd,WM_NOTIFY,wParam,(LPARAM)&nmhdr);
	CFrameWnd::OnClose();
}

int CPlot::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pMenuMain->CreateMenu();
	m_pMenuPopup->CreatePopupMenu();

	m_pMenuPopup->AppendMenu(MF_STRING,IDM_ZOOMIN,"Zoom in\tF1");
	m_pMenuPopup->AppendMenu(MF_SEPARATOR);
	m_pMenuPopup->AppendMenu(MF_STRING,IDM_EXPAND_VERT,"Expand vertical\tF2");
	m_pMenuPopup->AppendMenu(MF_STRING,IDM_EXPAND_HORIZ,"Expand horizontal\tF3");
	m_pMenuPopup->AppendMenu(MF_STRING,IDM_SHRINK_VERT,"Shrink vertical\tF4");
	m_pMenuPopup->AppendMenu(MF_STRING,IDM_SHRINK_HORIZ,"Shrink horizontal\tF5");
	m_pMenuPopup->AppendMenu(MF_SEPARATOR);
	m_pMenuPopup->AppendMenu(MF_STRING,IDM_BACKUP,"Back up\tESC");
	m_pMenuPopup->AppendMenu(MF_STRING,IDM_SCALE_AUTO,"Auto scale\tAlt+A");

	SetMenu(m_pMenuMain);
	m_pMenuMain->Detach();

	GetClientRect(&m_rect);
	m_rect.NormalizeRect();
	m_pDC = GetDC();
	return 0;
}

void CPlot::OnLButtonDown( UINT nFlags, CPoint point )
{
	CFrameWnd::OnLButtonDown(nFlags, point);
	if (!IsIconic()) {
		// CClientDC dc(this);
		if (m_pDC->GetPixel(m_selectRect.left,m_selectRect.top) != RGB(255,255,255))
			InvertRect(m_pDC,&m_selectRect);
		m_selectRect.left = m_selectRect.right = point.x;
		m_selectRect.top  = m_selectRect.bottom = point.y;
		SetCapture(); 
	}
}

void CPlot::OnLButtonUp( UINT nFlags, CPoint point )
{
	CFrameWnd::OnLButtonUp(nFlags, point);
	if (GetCapture()==this) {
		m_selectRect.right  = point.x;
		m_selectRect.bottom = point.y;
		m_selectRect.NormalizeRect();

		double ptsOnScreen = ((RIGHTMARGIN-LEFTMARGIN)*m_rect.Width())/m_xScale;
		m_leftSelIDX = m_origLeftIDX + 
			(long)(ptsOnScreen*((double)m_selectRect.left - LEFTMARGIN*m_rect.Width())/
			((RIGHTMARGIN-LEFTMARGIN)*m_rect.Width()));
		m_rightSelIDX = m_leftSelIDX + (long)(ptsOnScreen*(double)m_selectRect.Width()/
			((RIGHTMARGIN-LEFTMARGIN)*m_rect.Width()));
		if (m_leftSelIDX < m_origLeftIDX) m_leftSelIDX = m_origLeftIDX;
		if (m_rightSelIDX > m_rightIDX) m_rightSelIDX = m_rightIDX;

		ReleaseCapture();
	}
}

void CPlot::OnMouseMove( UINT nFlags, CPoint point )
{
	CFrameWnd::OnMouseMove(nFlags, point);
	if (GetCapture()==this) {
		// CClientDC dc(this);
		if (m_bSelectionExists) InvertRect(m_pDC,&m_selectRect);
		m_selectRect.right = point.x;
		m_selectRect.bottom = point.y;
		InvertRect(m_pDC,&m_selectRect);
		m_bSelectionExists = TRUE;
	}
}

void CPlot::OnPaint ()
{
	CFrameWnd::OnPaint();
	if (IsWindowVisible()) {
		CClientDC dc (this);
		if (!((CList<double,double&> *)m_yHistories[0])->IsEmpty()) Redraw(&dc);
		else Clear(&dc);
	} 
}

void CPlot::OnHScroll (UINT nCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nDelta;
	int nMaxPos = (int)(m_thisPageX - (double)m_plotRect.Width()/m_xScale);
	switch (nCode) {
	case SB_LINELEFT:
		if (m_nScrollPos <= 0)
			return;
		nDelta = -(min(1,m_nScrollPos));
		break;
	case SB_PAGELEFT:
		if (m_nScrollPos <= 0)
			return;
		nDelta = -(min(m_nPageSize,m_nScrollPos));
		break;
	case SB_THUMBPOSITION:
		nDelta = (int) nPos - m_nScrollPos;
		break;
	case SB_PAGERIGHT:
		if (m_nScrollPos >= nMaxPos)
			return;
		nDelta = min(m_nPageSize,nMaxPos - m_nScrollPos);
		break;
	case SB_LINERIGHT:
		if (m_nScrollPos >= nMaxPos)
			return;
		nDelta = min(1,nMaxPos - m_nScrollPos);
		break;
	default:
		return;
	}
	m_nScrollPos += nDelta;
	SetScrollPos (SB_HORZ, m_nScrollPos, TRUE);
	ScrollWindow(-nDelta,0,NULL,&m_plotRect);
}

void CPlot::OnPopup(UINT nID)
{
	// CClientDC dc(this);
	double magFactor=1.0;
	switch (nID) {
	case IDM_ZOOMIN:
		m_pScaleHistory->AddTail(CZoom(m_leftIDX,m_rightIDX,m_xScale,m_yScale,m_selectRect,m_yBaseline));
		m_yBaseline = (long)((TOPMARGIN*m_rect.bottom) + (double)m_plotRect.Height()*((double)m_yBaseline - 
			m_selectRect.top)/m_selectRect.Height());
		m_origLeftIDX = m_leftIDX;
		m_origRightIDX = m_rightIDX;
		m_leftIDX = m_leftSelIDX;
		m_rightIDX = m_rightSelIDX;
		m_xTickScale = (m_xTickScale/((double)m_plotRect.Width()/(m_rightIDX - m_leftIDX)));
		m_xScale = (double)m_plotRect.Width()/(m_rightIDX - m_leftIDX);
		m_yScale = m_yScale*(double)m_plotRect.Height()/m_selectRect.Height();
		Redraw(m_pDC);
		break;
	case IDM_EXPAND_VERT:
		m_pScaleHistory->AddTail(CZoom(m_leftIDX,m_rightIDX,m_xScale,m_yScale,m_selectRect,m_yBaseline));
		magFactor = (double)m_rect.Height()/(double)m_selectRect.Height();
		m_yScale = m_yScale * magFactor;
		Redraw(m_pDC);
		break;
	case IDM_EXPAND_HORIZ:
		m_pScaleHistory->AddTail(CZoom(m_leftIDX,m_rightIDX,m_xScale,m_yScale,m_selectRect,m_yBaseline));
		magFactor = (double)m_rect.Width()/(double)m_selectRect.Width();
		m_xScale = m_xScale * magFactor;
		Redraw(m_pDC);
		break;
	case IDM_SHRINK_VERT:
		m_pScaleHistory->AddTail(CZoom(m_leftIDX,m_rightIDX,m_xScale,m_yScale,m_selectRect,m_yBaseline));
		magFactor = (double)m_rect.Height()/(double)m_selectRect.Height();
		m_yScale = m_yScale / magFactor;
		Redraw(m_pDC);
		break;
	case IDM_SHRINK_HORIZ:
		m_pScaleHistory->AddTail(CZoom(m_leftIDX,m_rightIDX,m_xScale,m_yScale,m_selectRect,m_yBaseline));
		magFactor = (double)m_rect.Width()/(double)m_selectRect.Width();
		m_xScale = m_xScale / magFactor;
		Redraw(m_pDC);
		break;
	case IDM_BACKUP:
		if (!m_pScaleHistory->IsEmpty()) {
			m_xTickScale = m_xTickScale / (m_pScaleHistory->GetTail().xScale/((double)m_plotRect.Width()/(m_rightIDX - m_leftIDX)));
			m_xScale      = m_pScaleHistory->GetTail().xScale;
			m_yScale      = m_pScaleHistory->GetTail().yScale;
			m_leftIDX     = m_pScaleHistory->GetTail().leftIDX;
			m_rightIDX    = m_pScaleHistory->GetTail().rightIDX;
			m_selectRect  = m_pScaleHistory->GetTail().selectRect;
			m_yBaseline   = (long)m_pScaleHistory->GetTail().yBaseline;
			m_pScaleHistory->RemoveTail();
			Redraw(m_pDC);
			InvertRect(m_pDC,&m_selectRect);
			m_bInvertOldSel = TRUE;
		} 
		else {
			if (m_bInvertOldSel) {
				// CClientDC dc(this);
				InvertRect(m_pDC,&m_selectRect);
				m_bInvertOldSel = FALSE;
				m_bSelectionExists = FALSE;
			}
		}
		break;
	case IDM_SCALE_AUTO:
		m_xScale    = 0.80*(RIGHTMARGIN-LEFTMARGIN)*m_rect.Width()/(m_origRightIDX - m_origLeftIDX);
		m_yScale    = 0.80*(BOTMARGIN-TOPMARGIN)*m_rect.Height()/(m_localYMax - m_localYMin);
		m_leftIDX   = m_origLeftIDX;
		m_rightIDX  = m_origRightIDX;
		m_yBaseline = (long)(TOPMARGIN*m_rect.bottom + (double)m_plotRect.Height()/2);
		m_pScaleHistory->RemoveAll();
		Redraw(m_pDC);
		break;
	}
	if (m_pScaleHistory->IsEmpty() && m_bSelectionExists)
		InvertRect(m_pDC,&m_selectRect);
	else if (!m_pScaleHistory->IsEmpty()) {
		m_bSelectionExists = FALSE;
	}
}

void CPlot::OnRButtonDown( UINT nFlags, CPoint point )
{
	CFrameWnd::OnRButtonDown( nFlags, point );
	if (!IsIconic()) {
		ClientToScreen(&point);
		m_pMenuPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_LEFTBUTTON, point.x, point.y, this);
	}
}

void CPlot::OnSize( UINT nType, int cx, int cy )
{
	CFrameWnd::OnSize(nType, cx, cy);

	if (!IsIconic()) {
		// Send a message to the owner
		// WPARAM wParam;
		// wParam = WM_CPLOT_SIZE;
		//::SendMessage(m_pOwnerWnd->m_hWnd,WM_NOTIFY,wParam,(LPARAM)&m_xScale);

		// Update scaling variables and redraw
		if (!IsWindowVisible()) {
			m_oldXScale = m_xScale;
			m_oldYScale = m_yScale;
			m_xScale = m_xScale * (double)cx/m_rect.Width();
			m_yScale = m_yScale * (double)cy/m_rect.Height();
		}
		else {
			m_xScale = m_xScale * (double)cx/m_rect.Width();
			m_yScale = m_yScale * (double)cy/m_rect.Height();
			m_oldXScale = m_xScale;
			m_oldYScale = m_yScale;
		}
		GetClientRect(&m_rect);
		m_rect.NormalizeRect();
		m_plotRect = CRect(
			(long)((LEFTMARGIN + YTICKLEN) * m_rect.Width()),
			(long)( TOPMARGIN * m_rect.bottom + 1),
			(long)( RIGHTMARGIN * m_rect.right - 1),
			(long)((BOTMARGIN - XTICKLEN) * m_rect.Height() - 5)
		);
		m_yBaseline = (long)((BOTMARGIN-TOPMARGIN)*((double)m_rect.Height())/2 + TOPMARGIN*m_rect.bottom);
		m_leftMargin = (long)(LEFTMARGIN * m_rect.Width());
		m_rightMargin = (long)(RIGHTMARGIN * m_rect.Width());
		m_topMargin = (long)(TOPMARGIN * m_rect.Height());
		m_botMargin = (long)(BOTMARGIN * m_rect.Height());
		m_lastDeviceX = m_leftMargin;
		m_thisDeviceX = m_leftMargin + m_xScale;
		for (int i=0; i<_CPLOT_MAXTRACES; i++) {
			m_lastDeviceY[i] = m_yBaseline;
		}
		// CClientDC dc(this);
		Clear(m_pDC);
		if (m_numTraces > 0)
			if (((CList<double,double&> *)m_yHistories[0])->GetCount() >= 1) Redraw(m_pDC); 
	}
}

void CPlot::Redraw(CDC *pDC)
{
	int i=0, LastPtOut = 0;
	double x=0, y=0;
	Clear(m_pDC);
	POSITION pos;
	for (int k=0; k<m_numTraces; k++) {
		pos = ((CList<double,double&> *)m_yHistories[k])->FindIndex(m_leftIDX);
		x = m_leftMargin;
		y = m_yBaseline - ((((CList<double,double&> *)m_yHistories[k])->GetAt(pos)) * m_yScale);
		if (y > m_botMargin-1) y = m_botMargin-1;
		if (y < m_topMargin+1) y = m_topMargin+1;
		m_pDC->MoveTo((long)x,(long)y);
		for (i = m_leftIDX + 1; i < m_rightIDX; i++) {
			y = m_yBaseline - ((((CList<double,double&> *)m_yHistories[k])->GetNext(pos)) * m_yScale);
			if (y > m_botMargin - 1) {
				m_pDC->LineTo((long)(x += m_xScale),(long)(m_botMargin - 1));
				m_pDC->SelectStockObject(NULL_PEN);
				LastPtOut = 1;
			}
			else if (y < m_topMargin + 1) {
				m_pDC->LineTo((long)(x += m_xScale),(long)(m_topMargin + 1));
				m_pDC->SelectStockObject(NULL_PEN);
				LastPtOut = -1;
			}
			else {
				if (LastPtOut == 1) {
					m_pDC->MoveTo((long)x,(long)(m_botMargin - 1));
				}
				else if (LastPtOut == -1) {
					m_pDC->MoveTo((long)x,(long)(m_topMargin + 1));
				}
				m_pDC->SelectObject(m_pPenArray[m_tracePens[k]]);
				m_pDC->LineTo((long)(x += m_xScale),(long)y);
				LastPtOut = 0;
			}
		}
		m_lastPageX = m_thisPageX = x;
		m_lastDeviceY[k] = m_thisDeviceY = y;
		m_thisDeviceX = x;
	}
}


void CPlot::Reset()
{
	int i=0;

	// CClientDC dc(this);
	Clear(m_pDC);
	for (i=0; i<m_numTraces; i++) {
		((CList<double,double&> *)m_yHistories[i])->RemoveAll();
	}
	m_bInvertOldSel = FALSE;
	m_bSelectionExists = FALSE;
	m_lastDeviceX = m_leftMargin;
	m_leftIDX = 0;
	m_leftSelIDX = 0;
	m_localYMax = 10e-300;
	m_localYMin = 10e+300;
	m_nPageSize = 0;
	m_nScrollPos = 0;
	m_numScreens = 1;
	m_oldXScale = XSCALE;
	m_oldYScale = YSCALE;
	m_origLeftIDX = 0;
	m_origRightIDX = 0;
	m_rightIDX = 0;
	m_rightSelIDX = 0;
	m_selectRect = CRect(0,0,0,0);
	m_thisDeviceX = m_lastDeviceX + m_xScale;
	m_thisDeviceY = 0;
	m_xScale = XSCALE;
	m_yBaseline = (long)((BOTMARGIN-TOPMARGIN)*((double)m_rect.Height())/2 + TOPMARGIN*m_rect.bottom);
	for (i=0; i<m_numTraces; i++) {
		m_lastDeviceY[i] = m_yBaseline;
	}
	m_yScale = YSCALE;
}
