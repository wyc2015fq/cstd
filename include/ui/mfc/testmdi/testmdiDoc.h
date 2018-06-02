// testmdiDoc.h : interface of the CTestmdiDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTMDIDOC_H__5808D754_4FB3_4852_A943_CFB6877E64DD__INCLUDED_)
#define AFX_TESTMDIDOC_H__5808D754_4FB3_4852_A943_CFB6877E64DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTestmdiDoc : public CDocument
{
	IMPLEMENT_DYNCREATE(CTestmdiDoc, CDocument)
protected: // create from serialization only
	CTestmdiDoc();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestmdiDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestmdiDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTestmdiDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTMDIDOC_H__5808D754_4FB3_4852_A943_CFB6877E64DD__INCLUDED_)
