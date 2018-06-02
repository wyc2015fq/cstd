// TiffTestDoc.h : interface of the CTiffTestDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIFFTESTDOC_H__BB0D9BE1_221F_4CAA_89C5_3EACA7F55A28__INCLUDED_)
#define AFX_TIFFTESTDOC_H__BB0D9BE1_221F_4CAA_89C5_3EACA7F55A28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "TiffStruct.h"

class CTiffTestDoc : public CDocument
{
protected: // create from serialization only
	CTiffTestDoc();
	DECLARE_DYNCREATE(CTiffTestDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTiffTestDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	DWORD m_dwBmSize;	//图象的数据部分的大小
	CPalette m_palDIB;	//BMP图象调色板
	HANDLE m_hDIB;		//BMP图象内存块句柄
	CSize m_sizeDoc;	//图象的长和宽
	virtual ~CTiffTestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTiffTestDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL CreateBmpBuffer();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIFFTESTDOC_H__BB0D9BE1_221F_4CAA_89C5_3EACA7F55A28__INCLUDED_)
