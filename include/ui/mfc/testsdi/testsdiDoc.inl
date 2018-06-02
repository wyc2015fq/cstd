// testsdiDoc.cpp : implementation of the CTestsdiDoc class
//

#include "stdafx.h"
#include "testsdi.h"

#include "testsdiDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CTestsdiDoc

IMPLEMENT_DYNCREATE(CTestsdiDoc, CDocument)

BEGIN_MESSAGE_MAP(CTestsdiDoc, CDocument)
	//{{AFX_MSG_MAP(CTestsdiDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestsdiDoc construction/destruction

CTestsdiDoc::CTestsdiDoc()
{
	// TODO: add one-time construction code here

}

CTestsdiDoc::~CTestsdiDoc()
{
}

BOOL CTestsdiDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTestsdiDoc serialization

void CTestsdiDoc::Serialize(CArchive& ar)
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

/////////////////////////////////////////////////////////////////////////////
// CTestsdiDoc diagnostics

#ifdef _DEBUG
void CTestsdiDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTestsdiDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestsdiDoc commands
