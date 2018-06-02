// testsdiView.cpp : implementation of the CTestsdiView class
//

#include "stdafx.h"
#include "testsdi.h"

#include "testsdiDoc.h"
#include "testsdiView.h"

/////////////////////////////////////////////////////////////////////////////
// CTestsdiView

IMPLEMENT_DYNCREATE(CTestsdiView, CView)

BEGIN_MESSAGE_MAP(CTestsdiView, CView)
	//{{AFX_MSG_MAP(CTestsdiView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestsdiView construction/destruction

CTestsdiView::CTestsdiView()
{
	// TODO: add construction code here

}

CTestsdiView::~CTestsdiView()
{
}

BOOL CTestsdiView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTestsdiView drawing

void CTestsdiView::OnDraw(CDC* pDC)
{
	CTestsdiDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CTestsdiView printing

BOOL CTestsdiView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTestsdiView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTestsdiView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTestsdiView diagnostics

#ifdef _DEBUG
void CTestsdiView::AssertValid() const
{
	CView::AssertValid();
}

void CTestsdiView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestsdiDoc* CTestsdiView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestsdiDoc)));
	return (CTestsdiDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestsdiView message handlers
