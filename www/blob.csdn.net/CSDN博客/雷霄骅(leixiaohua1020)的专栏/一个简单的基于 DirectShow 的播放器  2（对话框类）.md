# 一个简单的基于 DirectShow 的播放器  2（对话框类） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月26日 00:44:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：35
个人分类：[DirectShow](https://blog.csdn.net/leixiaohua1020/article/category/1645657)










上篇文章分析了一个封装DirectShow各种接口的封装类（CDXGraph）：[一个简单的基于 DirectShow 的播放器 1（封装类）](http://blog.csdn.net/leixiaohua1020/article/details/13019147)

本文继续上篇文章，分析一下调用这个封装类（CDXGraph）的对话框类（CSimplePlayerDlg），看看在MFC中如何使用这个类（CDXGraph）。

首先来看看CSimplePlayerDlg这个类的定义，瞧瞧SimplePlayerDlg.h这个头文件。



```cpp
/* 雷霄骅
 * 中国传媒大学/数字电视技术
 * leixiaohua1020@126.com
 *
 */
// SimplePlayerDlg.h : header file
//

#if !defined(AFX_SIMPLEPLAYERDLG_H__3599FE35_3322_4CC7_B30B_6D6050C2EDFF__INCLUDED_)
#define AFX_SIMPLEPLAYERDLG_H__3599FE35_3322_4CC7_B30B_6D6050C2EDFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSimplePlayerDlg dialog

#include <streams.h>
#include "CDXGraph.h"

#define SLIDER_TIMER   100

class CSimplePlayerDlg : public CDialog
{
// Construction
public:
	CSimplePlayerDlg(CWnd* pParent = NULL);	// standard constructor
	~CSimplePlayerDlg();

// Dialog Data
	//{{AFX_DATA(CSimplePlayerDlg)
	enum { IDD = IDD_SIMPLEPLAYER_DIALOG };
	CSliderCtrl	mSliderGraph;
	CStatic	mVideoWindow;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimplePlayerDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	CDXGraph *   mFilterGraph;     // Filter Graph封装
	CString      mSourceFile;      // 源文件
	UINT         mSliderTimer;     // 定时器ID
	//创建Graph
	void CreateGraph(void);        // 创建Filter Graph
	void DestroyGraph(void);       // 析构Filter Graph
	void RestoreFromFullScreen(void);

	// Just for testing...
	HRESULT FindFilterByInterface(REFIID riid, IBaseFilter** ppFilter);
	void ShowVRPropertyPage(void);

	// Generated message map functions
	//{{AFX_MSG(CSimplePlayerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//打开
	afx_msg void OnButtonOpen();
	//播放
	afx_msg void OnButtonPlay();
	//暂停
	afx_msg void OnButtonPause();
	//停止
	afx_msg void OnButtonStop();
	afx_msg void OnButtonGrab();
	afx_msg void OnButtonFullscreen();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButtonTest();
	//}}AFX_MSG
	afx_msg LRESULT OnGraphNotify(WPARAM inWParam, LPARAM inLParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMPLEPLAYERDLG_H__3599FE35_3322_4CC7_B30B_6D6050C2EDFF__INCLUDED_)
```

从头文件来看，和普通的MFC对话框类并没有什么不同，无非是一些消息响应函数，或者MFC控件对应的类。需要注意一下，有一个变量：

```cpp
CDXGraph *   mFilterGraph
```

接下来看看CSimplePlayerDlg函数的实现部分吧。
 OnButtonOpen()：打开媒体文件按钮的响应函数



```cpp
//打开
void CSimplePlayerDlg::OnButtonOpen() 
{
	// TODO: Add your control notification handler code here
	CString    strFilter = "AVI File (*.avi)|*.avi|";
	strFilter += "MPEG File (*.mpg;*.mpeg)|*.mpg;*.mpeg|";
	strFilter += "Mp3 File (*.mp3)|*.mp3|";
	strFilter += "Wave File (*.wav)|*.wav|";
	strFilter += "All Files (*.*)|*.*|";
	CFileDialog dlgOpen(TRUE, NULL, NULL, OFN_PATHMUSTEXIST | OFN_HIDEREADONLY, 
		strFilter, this);
	if (IDOK == dlgOpen.DoModal()) 
	{
		mSourceFile = dlgOpen.GetPathName();
		// Rebuild the file playback filter graph
		//创建Graph
		CreateGraph();
	}
}
```





其中CreateGraph()函数如下所示：



```cpp
//创建Graph
void CSimplePlayerDlg::CreateGraph(void)
{
	//（如果有）销毁Graph
	DestroyGraph();
	//新建一个核心类
	mFilterGraph = new CDXGraph();
	if (mFilterGraph->Create())
	{
		// Render the source clip
		mFilterGraph->RenderFile(mSourceFile);
		// Set video window and notification window
		mFilterGraph->SetDisplayWindow(mVideoWindow.GetSafeHwnd());
		mFilterGraph->SetNotifyWindow(this->GetSafeHwnd());
		// Show the first frame
		mFilterGraph->Pause();
	}
}
```


 与CreateGraph()相反的还有一个DestroyGraph()





```cpp
//（如果有）销毁Graph
void CSimplePlayerDlg::DestroyGraph(void)
{
	if (mFilterGraph)
	{
		// Stop the filter graph first
		mFilterGraph->Stop();
		mFilterGraph->SetNotifyWindow(NULL);

		delete mFilterGraph;
		mFilterGraph = NULL;
	}
}
```





OnButtonPlay()：播放按钮的响应函数



```cpp
//播放
void CSimplePlayerDlg::OnButtonPlay() 
{
	if (mFilterGraph)
	{
		mFilterGraph->Run();
		// Start a timer
		if (mSliderTimer == 0)
		{
			mSliderTimer = SetTimer(SLIDER_TIMER, 100, NULL);
		}
	}
}
```


 OnButtonPause()：暂停按钮的响应函数





```cpp
void CSimplePlayerDlg::OnButtonPause() 
{
	if (mFilterGraph)
	{
		mFilterGraph->Pause();
		// Start a timer
		if (mSliderTimer == 0)
		{
			mSliderTimer = SetTimer(SLIDER_TIMER, 100, NULL);
		}
	}
}
```


 OnButtonStop()：停止按钮的响应函数





```cpp
void CSimplePlayerDlg::OnButtonStop() 
{
	if (mFilterGraph)
	{
		mFilterGraph->SetCurrentPosition(0);
		mFilterGraph->Stop();
		// Stop the timer
		if (mSliderTimer)
		{
			KillTimer(mSliderTimer);
			mSliderTimer = 0;
		}
	}
}
```


 其他的函数不再一一列举，但意思都是一样的。



播放器源代码下载：[http://download.csdn.net/detail/leixiaohua1020/6453467](http://download.csdn.net/detail/leixiaohua1020/6453467)








