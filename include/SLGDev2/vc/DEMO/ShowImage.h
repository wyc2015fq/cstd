#if !defined(AFX_SHOWIMAGE_H__486EEA2D_3D06_47E0_8B74_BAE6CDB295F7__INCLUDED_)
#define AFX_SHOWIMAGE_H__486EEA2D_3D06_47E0_8B74_BAE6CDB295F7__INCLUDED_
#include "Picture.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShowImage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShowImage window



class CShowImage : public CStatic
{
// Construction
public:
	CShowImage();
	
// Attributes
public:

	//
	BOOL AllocMemory( int nMemoryLen );
	
	//
	void FreeMemory();

	//
	void SetPicPath( CString path );

	//
	CString getpicPath();
	
	//
	void GetLoadState( BOOL& bState ){ bState = bLoad; };

	//
	void SetLoadState( BOOL bState ){ bLoad = bState; };

	//
	void ResetPic();
	
public:
	CString			m_path;								//			
	long			m_controlID;						//
	int				m_nReadLen;							//
	int				m_nMemoryLen;						//
	int				iPos;								//;
	CPicture		m_Pic;								//
	HGLOBAL			m_hGlobal;							//

	//CriticalSection	m_Lock;								//

protected:
	BOOL bLoad;											//

//	STATICDBL pDrawInfo;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShowImage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CShowImage();

	// Generated message map functions
protected:
	//{{AFX_MSG(CShowImage)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG
protected:
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOWIMAGE_H__486EEA2D_3D06_47E0_8B74_BAE6CDB295F7__INCLUDED_)
