// ZhengModelView.cpp : implementation of the CZhengModelView class
//

#include "stdafx.h"
#include "GlobalDefs.h"
#include "ZhengModel.h"
#include "Neuron.h"
#include "RunPage.h"
#include "NeuronPage.h"
#include "DisplayPage.h"
#include "TabCtrls.h"
#include "ZhengModelDoc.h"
#include "ZhengModelView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZhengModelView

IMPLEMENT_DYNCREATE(CZhengModelView, CView)

BEGIN_MESSAGE_MAP(CZhengModelView, CView)
	//{{AFX_MSG_MAP(CZhengModelView)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZhengModelView construction/destruction

CZhengModelView::CZhengModelView()
{

}

CZhengModelView::~CZhengModelView()
{
}

BOOL CZhengModelView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CZhengModelView drawing

void CZhengModelView::OnDraw(CDC* pDC)
{
	CZhengModelDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CZhengModelView printing

BOOL CZhengModelView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CZhengModelView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CZhengModelView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CZhengModelView diagnostics

#ifdef _DEBUG
void CZhengModelView::AssertValid() const
{
	CView::AssertValid();
}

void CZhengModelView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CZhengModelDoc* CZhengModelView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CZhengModelDoc)));
	return (CZhengModelDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CZhengModelView message handlers

void CZhengModelView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

}