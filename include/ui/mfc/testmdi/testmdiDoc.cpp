// testmdiDoc.cpp : implementation of the CTestmdiDoc class
//

#include "stdafx.h"
#include "testmdi.h"

#include "testmdiDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CTestmdiDoc

//IMPLEMENT_DYNCREATE(CTestmdiDoc, CDocument)

BEGIN_MESSAGE_MAP(CTestmdiDoc, CDocument)
	//{{AFX_MSG_MAP(CTestmdiDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestmdiDoc construction/destruction

CTestmdiDoc::CTestmdiDoc()
{
	// TODO: add one-time construction code here

}

CTestmdiDoc::~CTestmdiDoc()
{
}

BOOL CTestmdiDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTestmdiDoc serialization

void CTestmdiDoc::Serialize(CArchive& ar)
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
// CTestmdiDoc diagnostics

#ifdef _DEBUG
void CTestmdiDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTestmdiDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestmdiDoc commands
