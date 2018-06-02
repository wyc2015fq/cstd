// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// RibbonMFCView.cpp : implementation of the CRibbonMFCView class
//

#include "stdafx.h"
#include "RibbonMFC.h"

#include "RibbonMFCDoc.h"
#include "RibbonMFCView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRibbonMFCView

IMPLEMENT_DYNCREATE(CRibbonMFCView, CView)

BEGIN_MESSAGE_MAP(CRibbonMFCView, CView)
END_MESSAGE_MAP()

// CRibbonMFCView construction/destruction

CRibbonMFCView::CRibbonMFCView()
{
	// TODO: add construction code here

}

CRibbonMFCView::~CRibbonMFCView()
{
}

BOOL CRibbonMFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CRibbonMFCView drawing

void CRibbonMFCView::OnDraw(CDC* /*pDC*/)
{
	CRibbonMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CRibbonMFCView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CRibbonMFCView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CRibbonMFCView diagnostics

#ifdef _DEBUG
void CRibbonMFCView::AssertValid() const
{
	CView::AssertValid();
}

void CRibbonMFCView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRibbonMFCDoc* CRibbonMFCView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRibbonMFCDoc)));
	return (CRibbonMFCDoc*)m_pDocument;
}
#endif //_DEBUG


// CRibbonMFCView message handlers
