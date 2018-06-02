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

	//分配内存长度
	BOOL AllocMemory( int nMemoryLen );
	
	//释放内存
	void FreeMemory();

	//通过路径加载图片
	void SetPicPath( CString path );

	//返回图片路径
	CString getpicPath();
	
	//获取是否加载图片
	void GetLoadState( BOOL& bState ){ bState = bLoad; };

	//设置是否加载图片标识
	void SetLoadState( BOOL bState ){ bLoad = bState; };

	//得新刷新显示
	void ResetPic();
	
public:
	CString			m_path;								//保存图片的路径			
	long			m_controlID;						//保存控件ID
	int				m_nReadLen;							//读取的文件大小
	int				m_nMemoryLen;						//内存长度
	int				iPos;								//图片是那个队列中的;
	CPicture		m_Pic;								//图片显示
	HGLOBAL			m_hGlobal;							//动态内存

	//CriticalSection	m_Lock;								//锁住数据更新

protected:
	BOOL bLoad;											//是否回载了图片

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
