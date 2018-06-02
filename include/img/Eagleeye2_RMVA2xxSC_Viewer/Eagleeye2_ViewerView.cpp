// Eagleeye2_ViewerView.cpp : implementation of the CEagleeye2_ViewerView class
//

#include "stdafx.h"
#include "Eagleeye2_Viewer.h"
#include "MainFrm.h"
#include "Eagleeye2_ViewerDoc.h"
#include "Eagleeye2_ViewerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ImHeight	1232
#define ImWidth		1616

extern BUFFNODE		listJpgBuff[NUM_BUFF];	// Buffer list
extern LONG			PrevBuff;				// Index of previously written buffer (for display)
extern LONG			CurrBuff;				// Index of Currently being written buffer (for receive)
extern EagleEye2ImgInfStruct	EagleEye2_ImgInf;
/////////////////////////////////////////////////////////////////////////////
// CEagleeye2_ViewerView

IMPLEMENT_DYNCREATE(CEagleeye2_ViewerView, CScrollView)

BEGIN_MESSAGE_MAP(CEagleeye2_ViewerView, CScrollView)
	//{{AFX_MSG_MAP(CEagleeye2_ViewerView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_MESSAGE(WM_RECVTRGIMAGEOK, OnRecvImageOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEagleeye2_ViewerView construction/destruction

CEagleeye2_ViewerView::CEagleeye2_ViewerView()
{
	// TODO: add construction code here
	m_gEraseBk = TRUE;
}

CEagleeye2_ViewerView::~CEagleeye2_ViewerView()
{
}

BOOL CEagleeye2_ViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CEagleeye2_ViewerView drawing

void CEagleeye2_ViewerView::OnDraw(CDC* pDC)
{
	CString	strFileName,strDirectory;
	CEagleeye2_ViewerDoc* pDoc = GetDocument();

	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	//Draw the image uploaded from Eagleeye2 camera
	if(pDoc->m_bIsDataFromCamera){
		if( !pDoc->m_bIsJpeg){
			if( !pDoc->m_bIsHighFR){
				// Achieve the handle of bitmap
				HBITMAP	hBmp = (HBITMAP) pDoc->m_Dib.GetDDB_Handle();
				// Draw the DDB bitmap
				if (pDoc->m_DDB.GetHandle() != NULL){
					::DeleteObject(pDoc->m_DDB.GetHandle ());
				}
				pDoc->m_DDB.Attach (hBmp);
				if (pDoc->m_DDB.GetHandle()){
					// In real size
					pDoc->m_DDB.ScaleDraw (pDC->GetSafeHdc(), GetScrollPos(SB_HORZ), GetScrollPos(SB_VERT), m_iClientW, m_iClientH);
				}
			}
			else{
				// Achieve the handle of bitmap
				HBITMAP	hBmp = (HBITMAP) pDoc->m_Dib.GetDDB_Handle();
				// Draw the DDB bitmap
				if (pDoc->m_DDB.GetHandle() != NULL){
					::DeleteObject(pDoc->m_DDB.GetHandle ());
				}
				pDoc->m_DDB.Attach (hBmp);
				if (pDoc->m_DDB.GetHandle()){
					// In real size
					pDoc->m_DDB.ScaleDraw (pDC->GetSafeHdc(), GetScrollPos(SB_HORZ), GetScrollPos(SB_VERT), m_iClientW, m_iClientH);
				}
			}
			return;
		}
		else{
			// Load JPEG image from buffer
			pDoc->m_fPic.LoadJpeg((BYTE*)listJpgBuff[PrevBuff].pBuff, listJpgBuff[PrevBuff].sizeBuff);
      if (0) {
        int i, n;
        FILE* pf = fopen("./asdf.txt", "wb");
        if (pf) {
          int* p = (int*)listJpgBuff[PrevBuff].pBuff;
          n = listJpgBuff[PrevBuff].sizeBuff;
          for (i=0; i<n/4; ++i) {
            fprintf(pf, "0x%08x,\r\n", p[i]);
          }
          fclose(pf);
        }
      }
			// Achieve the handle of bitmap
			HBITMAP	hBmp = (HBITMAP) pDoc->m_fPic.GetDDB_Handle();
			
			// Draw the DDB bitmap
			if (pDoc->m_DDB.GetHandle () != NULL)
				::DeleteObject (pDoc->m_DDB.GetHandle ());
			
			pDoc->m_DDB.Attach (hBmp);
			if (pDoc->m_DDB.GetHandle()){
				// In real size
				pDoc->m_DDB.ScaleDraw (pDC->GetSafeHdc(), this->GetScrollPos(SB_HORZ), this->GetScrollPos(SB_VERT), m_iClientW, m_iClientH);
			}
			return;
			
		}
	}
	// Draw the still image
	if(pDoc->m_DDB.GetHandle()){
		if(this->GetDocument()->m_bFirstShow){
			SHOWEFFECT	shParam ;
			switch (rand()%3){
				case 0 :
					shParam.dwMethod = SHOWDIB_SCAN ;
					shParam.dwSubMethod = 1 << (rand() % 8) ;
					shParam.dwInterval = 350 ;
					break ;
				case 1 :
					shParam.dwMethod = (rand()%2) ? SHOWDIB_HSHUTTER : SHOWDIB_VSHUTTER ;
					shParam.dwBlockLen = 16 ;
					shParam.dwInterval = 14 ;
					break ;
				case 2 :
					shParam.dwMethod = SHOWDIB_MOSAIC ;
					shParam.dwBlockLen = 15 ;
					shParam.dwInterval = 70 ;
					break ;
			}
			this->GetDocument()->m_DDB.DrawEx (pDC->GetSafeHdc(),shParam) ;
			this->GetDocument()->m_bFirstShow = FALSE ;
		}
		else
			pDoc->m_DDB.ScaleDraw (pDC->GetSafeHdc(), this->GetScrollPos(SB_HORZ), this->GetScrollPos(SB_VERT), m_iClientW, m_iClientH) ;
	}
}

void CEagleeye2_ViewerView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx =ImWidth; sizeTotal.cy = ImHeight;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CEagleeye2_ViewerView printing

BOOL CEagleeye2_ViewerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CEagleeye2_ViewerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CEagleeye2_ViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CEagleeye2_ViewerView diagnostics

#ifdef _DEBUG
void CEagleeye2_ViewerView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CEagleeye2_ViewerView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CEagleeye2_ViewerDoc* CEagleeye2_ViewerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEagleeye2_ViewerDoc)));
	return (CEagleeye2_ViewerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEagleeye2_ViewerView message handlers

LRESULT CEagleeye2_ViewerView::OnRecvImageOK(WPARAM wParam, LPARAM lParam)
{
	CEagleeye2_ViewerDoc* pDoc = GetDocument();
	Invalidate( FALSE );
	return 0L;
}

void CEagleeye2_ViewerView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	m_iClientW = cx; m_iClientH = cy;	
	m_gEraseBk = TRUE;
}

BOOL CEagleeye2_ViewerView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_gEraseBk)
	{
		pDC->FillSolidRect(0,0,m_iClientW,m_iClientH,RGB(255,255,255));
		m_gEraseBk = FALSE;
	}
	
	return FALSE;
}

void CEagleeye2_ViewerView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_gEraseBk = TRUE;

	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
