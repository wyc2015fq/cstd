# CToolTipCtrl使用详细解说 - L_Andy的专栏 - CSDN博客

2014年04月24日 17:14:11[卡哥](https://me.csdn.net/L_Andy)阅读数：4207


                
很多的界面设计都需要有Tip提示，下面描述一下Tip的简单使用方法：

1. 首先要New一个CToolTipCtrl的对象m_pContentTip

2. 调用CToolTipCtrl的create函数，指定其父窗口。可以为某个控件，也可以为this。一般设为this。

m_pContentTip->Create( this, TTS_ALWAYSTIP );

3. AddTool。此函数是设置与某个控件关联的Tip文本。此处可以为多个控件添加不同的提示信息。

　 m_pButtonTip->AddTool( GetDlgItem( 控件ID ), 文本 ); //#add 一定要用AddTool 否则不会显示 AddTool(this,文本);也行.

4. SetTipBkColor设置背景颜色。

5. SetTipTextColor设置文本颜色。

6. 最重要的一步。在PreTranslateMessage函数中添加下面语句：

BOOL CTestToolTipDlg::PreTranslateMessage(MSG* pMsg)

{

// TODO: Add your specialized code here and/or call the base class

if(pMsg->message== WM_LBUTTONDOWN ||

pMsg->message== WM_LBUTTONUP ||

pMsg->message== WM_MOUSEMOVE)

m_pToolTipCtrl.RelayEvent(pMsg);

return CDialog::PreTranslateMessage(pMsg);

}

另外，还可以使用m_pContentTip->UpdateTipText函数来更新Tip信息。

动态改变ToolTip的显示内容的方法及步骤：

上面所讲的1、2、4步骤。

在增加ToolTip时不指定显示的字串，而是使用LPSTR_TEXTCALLBACK。

在窗口中增加消息映射 ON_NOTIFY_EX( TTN_NEEDTEXT, 0, SetTipText )。

在窗口中增加一个函数用于动态提供显示内容，其原型为 BOOL SetTipText( UINT id, NMHDR * pTTTStruct, LRESULT * pResult )，下面的代码可以根据传入的参数判定应该显示的内容。

BOOL CWndYour::SetTipText( UINT id, NMHDR * pTTTStruct, LRESULT * pResult )

{

TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pTTTStruct; 

UINT nID =pTTTStruct->idFrom; //得到相应窗口ID，有可能是HWND

if (pTTT->uFlags & TTF_IDISHWND) //表明nID是否为HWND

{

nID = ::GetDlgCtrlID((HWND)nID);//从HWND得到ID值，当然你也可以通过HWND值来判断

switch(nID)

{

case(IDC_YOUR_CONTROL1) 

strcpy(pTTT->lpszText,your_string1);//设置

return TRUE;

break;

case(IDC_YOUR_CONTROL2)

//设置相应的显示字串

return TRUE;

break;

}

}

return TRUE;

}

注:

实际项目中常常重写该类得到 不同风格,甚至是在其中添加图片,如CXInfoTip.

常用的扩展按钮控件一般均封装了一个CToolTipCtrl控件.


另一篇:


二,动态改变ToolTip的显示内容的方法及步骤： 

1.上面所讲的1、2、4步骤。 

2.在增加ToolTip时不指定显示的字串，而是使用LPSTR_TEXTCALLBACK。 

3.在窗口中增加消息映射 ON_NOTIFY_EX( TTN_NEEDTEXT, 0, SetTipText )。 

4.在窗口中增加一个函数用于动态提供显示内容，其原型为 BOOL SetTipText( UINT id, NMHDR * pTTTStruct, LRESULT * pResult )，下面的代码可以根据传入的参数判定应该显示的内容。 

BOOL CWndYour::SetTipText( UINT id, NMHDR * pTTTStruct, LRESULT * pResult ) 

{ 

TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pTTTStruct; 

UINT nID =pTTTStruct->idFrom; //得到相应窗口ID，有可能是HWND 

if (pTTT->uFlags & TTF_IDISHWND) //表明nID是否为HWND 

{ 

nID = ::GetDlgCtrlID((HWND)nID);//从HWND得到ID值，当然你也可以通过HWND值来判断 

switch(nID) 

case(IDC_YOUR_CONTROL1) 

strcpy(pTTT->lpszText,your_string1);//设置 

return TRUE; 

break; 

case(IDC_YOUR_CONTROL2) 

//设置相应的显示字串 

return TRUE; 

break; 

} 

return(FALSE); 

} 

ToolTip是Win32中一个通用控件，MFC中为其生成了一个类CToolTipCtrl，总的说来其使用方法是较简单的，下面讲一下它的一般用法和高级用法。

一般用法步骤： 

1.添加CToolTipCtrl成员变量 m_tt。 

2.在父窗口中调用EnableToolTips(TRUE); 

3.在窗口的OnCreate（或者其他适当的位置）中向ToolTip中添加需要显示Tip的子窗口，并同时指定相应的显示字串CToolTipCtrl::AddTool(pWnd,"string to display")。 

 4.重载父窗口的 BOOL PreTranslateMessage(MSG* pMsg) ，在函数中调用 m_tt.RelayEvent(pMsg)。


下面假设在窗口CWndYour中使用CToolTipCtrl 

在类定义中添加变量说明：

class CWndYour:xxx 

{ 

CToolTipCtrl m_tt; 

} 

在OnCreate中添加需要显示Tip的子窗口 

CWndYour::OnCreate(....) 

{ 

EnableToolTips(TRUE); 

m_tt.Create(this); 

m_tt.Activate(TRUE); 

CWnd* pW=GetDlgItem(IDC_CHECK1);//得到窗口指针

m_tooltip.AddTool(pW,"Check1");//添加 

........ 

} 

在BOOL PreTranslateMessage(MSG* pMsg)中添加代码 

BOOL CWndYour::PreTranslateMessage(MSG* pMsg) 

{ 

{ 

m_tt.RelayEvent(pMsg); 

} 

return CParentClass::PreTranslateMessage(pMsg); 

} 

这样当鼠标移动到相应的子窗口上时会显示出相应的ToolTip。
            

