// testmdiView.cpp : implementation of the CTestmdiView class
//

#include "stdafx.h"
#include "testmdi.h"

#include "testmdiDoc.h"
#include "testmdiView.h"

/////////////////////////////////////////////////////////////////////////////
// CTestmdiView

//IMPLEMENT_DYNCREATE(CTestmdiView, CView)

BEGIN_MESSAGE_MAP(CTestmdiView, CView)
	//{{AFX_MSG_MAP(CTestmdiView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestmdiView construction/destruction

CTestmdiView::CTestmdiView()
{
	// TODO: add construction code here

}

CTestmdiView::~CTestmdiView()
{
}

BOOL CTestmdiView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTestmdiView drawing

void CTestmdiView::OnDraw(CDC* pDC)
{
	CTestmdiDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CTestmdiView printing

BOOL CTestmdiView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTestmdiView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTestmdiView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTestmdiView diagnostics

#ifdef _DEBUG
void CTestmdiView::AssertValid() const
{
	CView::AssertValid();
}

void CTestmdiView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestmdiDoc* CTestmdiView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestmdiDoc)));
	return (CTestmdiDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestmdiView message handlers
