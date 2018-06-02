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

// RibbonMFCDoc.cpp : implementation of the CRibbonMFCDoc class
//

#include "stdafx.h"
#include "RibbonMFC.h"

#include "RibbonMFCDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRibbonMFCDoc

IMPLEMENT_DYNCREATE(CRibbonMFCDoc, CDocument)

BEGIN_MESSAGE_MAP(CRibbonMFCDoc, CDocument)
END_MESSAGE_MAP()


// CRibbonMFCDoc construction/destruction

CRibbonMFCDoc::CRibbonMFCDoc()
{
	// TODO: add one-time construction code here

}

CRibbonMFCDoc::~CRibbonMFCDoc()
{
}

BOOL CRibbonMFCDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CRibbonMFCDoc serialization

void CRibbonMFCDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CRibbonMFCDoc diagnostics

#ifdef _DEBUG
void CRibbonMFCDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRibbonMFCDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CRibbonMFCDoc commands
