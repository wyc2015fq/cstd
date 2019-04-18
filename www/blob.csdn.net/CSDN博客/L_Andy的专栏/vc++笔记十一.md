# vc++笔记十一 - L_Andy的专栏 - CSDN博客

2015年06月04日 10:25:32[卡哥](https://me.csdn.net/L_Andy)阅读数：844


                
 一、LNK1123: 转换到 COFF 期间失败: 文件无效或损坏

连接器LNK是通过调用cvtres.exe完成文件向coff格式的转换的，所以出现这种错误的原因就是cvtres.exe出现了问题。

## 解决方案：

 第一步：将 项目——项目属性——配置属性——连接器——清单文件——嵌入清单 “是”改为“否”。若还不能解决问题进入第二步。

第二步：在电脑里面搜索一下cvtres.exe，发现存在多个文件，使用最新的cvtres.exe替换老的文件即可，替换之前记得备份一下，如果不对，可以替换回来。

例如：我的电脑里面安装了vs2010,最近更新了系统，打了一些补丁，结果就出现这种错误了。在电脑里面搜索发现

C:\Program Files\Microsoft Visual Studio 10.0\VC\bin

C:\Windows\winsxs\x86_netfx-cvtres_for_vc_and_vb_b03f5f7f11d50a3a_6.1.7600.16385_none_ba476986f05abc65

C:\Windows\Microsoft.NET\Framework\v4.0.30319

这三个路径里面都有cvtres.exe文件，于是我尝试使用第二个路径里面的文件替换第一个路径的文件，问题解决。

//////////////////////////////////////////////////////////

VS2010编译错误：fatal error C1189: #error : This file requires _WIN32_WINNT to be #defined at least to 0x0403. Value 0x0501 or higher

monkey monkey 2012-07-12 17:35:24

 VS2010编译错误：fatal error C1189: #error : This file requires _WIN32_WINNT to be #defined at least to 0x0403. Value 0x0501 or higher

分类： VC 2010-08-13 16:09 3665人阅读 评论(0) 收藏 举报

下面是彻底解决方法：

在工程的stdafx.h中添加（如有类似语句，需注释掉）

#ifndef WINVER // Allow use of features specific to Windows 95 and Windows NT 4 or later.

#define WINVER 0x0501 // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.

#endif

#ifndef _WIN32_WINNT // Allow use of features specific to Windows NT 4 or later.

#define _WIN32_WINNT 0x0501 // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.

#endif

#ifndef _WIN32_WINDOWS // Allow use of features specific to Windows 98 or later.

#define _WIN32_WINDOWS 0x0501 // Change this to the appropriate value to target Windows Me or later.

#endif

#ifndef _WIN32_IE // Allow use of features specific to IE 4.0 or later.

#define _WIN32_IE 0x0601 // Change this to the appropriate value to target IE 5.0 or later.

#endif

//////////////////////////////////////////////////////////////////////////

 BitBlt和StretchBlt的比较

他们的不同就是一个不可以随便改大小，一个大小随便变。

///////////////////////////////////////////////////////

在CMainFrame的OnCreate()中添加如下代码：

long Style;

//去掉标题栏及其他样式

SetWindowLong(this->m_hWnd,GWL_STYLE,0);

//去掉边框及其他样式

SetWindowLong(this->m_hWnd,GWL_EXSTYLE,0);

//取消菜单栏

this->SetMenu(NULL);

在CView中的OnCreate()中也去掉边框

//去掉边框风格及其他样式

long Style;

SetWindowLong(this->m_hWnd,GWL_EXSTYLE,0);



方法二：使用CWnd成员函数ModifyStyle实现  

//  隐藏TitleBar  

ModifyStyle(WS_CAPTION,  0,  SWP_FRAMECHANGED);  

//  显示TitleBar  

ModifyStyle(0,  WS_CAPTION,  SWP_FRAMECHANGED);  

以上代码，放置在CMainFrame的OnCreate函数的尾部即可~~

///////////////////////////////////////////////////////

SendDlgItemMessage　　函数功能：该函数把一个消息发送给指定的对话框中的控制。

　　函数原型：LONG SendDlgItemMessage(

　　HWND hDlg, // handle of dialog box

　　int nIDDlgItem, // identifier of control

　　UINT Msg, // message to send

　　WPARAM wParam, // first message parameter

　　LPARAM lParam // second message parameter

　　);

　　参数：

　　hDlg:指定含有控制的对话框。

　　nIDDigItem:指定接收消息的控制的标识符。

　　Msg：指定将被发送的消息。

　　wParam:指定消息特定的其他信息。

　　lParam:指定消息特定的其他信息。

　　返回值：返回值指定消息处理的结果，且依赖于发送的消息。

　　备注：SendDlgItemMessage函数直到消息已经被处理时才返回。

　　使用SendDlgItemMessage函数同从一个指定的控制中检索句柄和调用SendMessagge函数一样。

///////////////////////////////////////////////////////////////////////////////

RegisterWindowMessage函数定义一个新的窗口消息，保证该消息在系统范围内是唯一的。通常调用SendMessage或者PostMessage函数时，可以使用该函数返回的消息值。
            

