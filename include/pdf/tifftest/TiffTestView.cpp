// TiffTestView.cpp : implementation of the CTiffTestView class
//

#include "stdafx.h"
#include "TiffTest.h"

#include "TiffTestDoc.h"
#include "TiffTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTiffTestView

IMPLEMENT_DYNCREATE(CTiffTestView, CView)

BEGIN_MESSAGE_MAP(CTiffTestView, CView)
	//{{AFX_MSG_MAP(CTiffTestView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTiffTestView construction/destruction

CTiffTestView::CTiffTestView()
{
	// TODO: add construction code here

}

CTiffTestView::~CTiffTestView()
{
}

BOOL CTiffTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTiffTestView drawing

void CTiffTestView::OnDraw(CDC* pDC)
{
	CTiffTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	if(NULL == pDoc->m_hDIB)
	{
		return;
	}
	HDC hdc = pDC->m_hDC;
	BYTE* pBuf = (BYTE*)GlobalLock(pDoc->m_hDIB);
	pBuf += sizeof(BITMAPFILEHEADER);
	BYTE* pData = pBuf + sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD);
	
	CPalette* pOldPal = pDC->SelectPalette(&pDoc->m_palDIB, TRUE);
	pDC->RealizePalette();

	::SetStretchBltMode(hdc, COLORONCOLOR);
	::StretchDIBits(hdc, 10, 10, pDoc->m_sizeDoc.cx, pDoc->m_sizeDoc.cy,
		0, 0, pDoc->m_sizeDoc.cx, pDoc->m_sizeDoc.cy, pData, 
		(BITMAPINFO*)pBuf, DIB_RGB_COLORS, SRCCOPY);

	pDC->SelectPalette(pOldPal, FALSE);
	GlobalUnlock(pDoc->m_hDIB);
}

/////////////////////////////////////////////////////////////////////////////
// CTiffTestView printing

BOOL CTiffTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTiffTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTiffTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTiffTestView diagnostics

#ifdef _DEBUG
void CTiffTestView::AssertValid() const
{
	CView::AssertValid();
}

void CTiffTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTiffTestDoc* CTiffTestView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTiffTestDoc)));
	return (CTiffTestDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTiffTestView message handlers
