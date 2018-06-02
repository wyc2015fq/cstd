// testsdiDoc.h : interface of the CTestsdiDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTSDIDOC_H__F7E1908A_403A_4359_90F4_45D16AD09409__INCLUDED_)
#define AFX_TESTSDIDOC_H__F7E1908A_403A_4359_90F4_45D16AD09409__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTestsdiDoc : public CDocument
{
protected: // create from serialization only
	CTestsdiDoc();
	DECLARE_DYNCREATE(CTestsdiDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestsdiDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestsdiDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTestsdiDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTSDIDOC_H__F7E1908A_403A_4359_90F4_45D16AD09409__INCLUDED_)
