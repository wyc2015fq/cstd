// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__46E3C581_CBE0_4226_96B1_27A4D6E8735D__INCLUDED_)
#define AFX_STDAFX_H__46E3C581_CBE0_4226_96B1_27A4D6E8735D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcview.h>
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <shlwapi.h>
#include <afxmt.h>
#include <afxtempl.h>
#include <vfw.h>

#include "iseemem.h"
#include "iseeio.h"
#include "iseeimn.h"
#include "iseeirw.h"

#include "btnst.h"


/////////////////////////////////////////////////////////////////////////////////
// ISeeTsVC 内用消息定义

#define WM_DANI_QUIT			(WM_APP+327)			// 命令动画线程退出
#define WM_DANI_DIABLE			(WM_APP+326)			// 废弃当前命令
#define WM_DANI_NEWCOMM			(WM_APP+325)			// 新图象命令

#define WM_DANI_NEXT			(WM_APP+321)			// 显示下一幅子图象
#define WM_DANI_INIT			(WM_APP+320)			// 动画线程完成初始化

#define WM_DISP_FAILD			(WM_APP+306)			// 读写过程结束但失败
#define WM_DISP_OK				(WM_APP+305)			// 读写过程结束并成功
#define WM_DISP_OVER			(WM_APP+304)			// 结束一个单帧图象的读取
#define WM_DISP_PROCESS			(WM_APP+303)			// 步进
#define WM_DISP_BUILDSI			(WM_APP+302)			// 数据包已经建立子图象
#define WM_DISP_BUILDMI			(WM_APP+301)			// 数据包已经建立主图象
#define WM_DISP_INTOPLUG		(WM_APP+300)			// 控制进入插件

#define WM_TVN_DEFERFILL		(WM_APP+255)			// 延迟填充消息
#define WM_TVN_SELCHANGED		(WM_APP+254)			// 选择修改消息
#define WM_TVN_PROCCREATE		(WM_APP+253)			// 延迟线程创建消息
#define WM_TVN_UPDATE			(WM_APP+252)			// 根据引擎信息刷新列表内容

#define WM_TVN_DEFER			(WM_APP+250)			// 延迟消息
#define WM_TVN_QUEUECLR			(WM_APP+249)			// 清除消息
#define WM_TVN_QUIT				(WM_APP+248)			// 退出消息

#define WM_LVN_FILLBEGIN		(WM_APP+245)			// 填充开始消息
#define WM_LVN_FILLEND			(WM_APP+244)			// 填充结束消息
#define WM_LVN_SUBITEMDATA		(WM_APP+243)			// 子项填充命令消息
#define WM_LVN_PROCCREATE		(WM_APP+242)			// 子项线程创建消息
#define WM_LVN_QUIT				(WM_APP+241)			// 退出消息
#define WM_LVN_NEWCOMM			(WM_APP+240)			// 新命令消息

#define WM_LVN_AUTOSEL			(WM_APP+235)			// 自动播放图象的触发信息
#define WM_LVN_AUTONEXT			(WM_APP+234)			// 自动播放下一幅图片

#define WM_RV_CREATED			(WM_APP+230)			// 所有窗口创建完毕通知




/////////////////////////////////////////////////////////////////////////////////
// ISeeTsVC 注册表键名定义

#define	ITSVCMAIN_RKEYSEC			("Information")		// 主键名称

#define	ITSVCMAIN_RKEY_WINPOS		("WinPosition")		// 软件关闭时的窗口位置
#define	ITSVCMAIN_RKEY_LEFTWINPOS	("TreeViewSize")	// 及尺寸
#define	ITSVCMAIN_RKEY_RIGHTWINPOS	("ListViewSize")
#define	ITSVCMAIN_RKEY_DISPWINPOS	("DispViewSize")

#define	ITSVCDISP_RKEY_AUTORESIZE	("AutoResize")		// 自动缩放状态
#define	ITSVCDISP_RKEY_RSROT		("Ratio")			// 缩放倍数
#define	ITSVCDISP_RKEY_INITPOS		("InitPosition")	// 对齐方式
#define	ITSVCDISP_RKEY_SWITCH		("SwitchType")		// 切换方式

#define	ITSVCDOC_RKEY_LASTPATH		("LastPath")		// 最后浏览的路径
#define	ITSVCRIGHT_RKEY_LISTYPE		("ListStyle")		// 列表视的风格
#define	ITSVCLIST_RKEY_AUTOPLAY		("AutoPlay")		// 自动播放标志



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__46E3C581_CBE0_4226_96B1_27A4D6E8735D__INCLUDED_)
