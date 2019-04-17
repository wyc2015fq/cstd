# C# 关于ActiveX(OCX)获取外部键盘鼠标事件 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年06月28日 11:23:54[boonya](https://me.csdn.net/boonya)阅读数：344








下面是网上看到的两篇关于OCX事件处理的文章，思路和方式没问题，关键是怎么转为C#，请有这方面经验的指点一下！



##### OCX控件在网页中如何获得键盘事件

原文地址：[https://blog.csdn.net/yh2008_chase/article/details/8155439](https://blog.csdn.net/yh2008_chase/article/details/8155439)



在网页中的ocx控件有时无法获得键盘事件，当网页中的ocx控件上有子窗口的时候，更是无法获得键盘事件，解决该问题的方法如下：




1 想让你的Activex捕捉到控制键,必须要让他成为整个屏幕Actice的. 在CxxxCtrl的OnCreate里面添加



OnActivateInPlace (TRUE, NULL); // == UI-Activate the control



因为IE不会自动激活首次加载的控件。





2在OnMouseActivate方法中添加OnActivateInPlace   (TRUE,   NULL);   这样Activex就可以捕获到控制键了。紧接着要做的就是在PreTranslateMessage中判断消息，然后分发给需要的Cwnd。




int CxxxCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
::PostMessage(wnd , WM_KEYDOWN , 0 , 0);  //wnd为接受信息的窗口、WM_KEYDOWN为发送的消息类型



return COleControl::OnMouseActivate(pDesktopWnd, nHitTest, message);
}



这样当网页一旦加载完成，ocx便自动获得了焦点。并且当鼠标在ocx的子窗口上点击，ocx也能获得键盘事件。

##### ActiveX控件在浏览器中的鼠标键盘事件处理方法

原文地址：[https://blog.csdn.net/colinchan/article/details/5516780](https://blog.csdn.net/colinchan/article/details/5516780)



ActiveX控件开发人员如需正确处理在浏览器中的鼠标键盘事件，可首先参考一下潘爱民老师的《COM原理与应用》第12章：ActiveX控制，第392页。其中系统阐述了ActiveX控件与其容器程序的键盘交互原理。

1. 控件初次加载时，进行实地激活。如果基于ATL的简单控件CComControl，则可在OnCreate中添加。如果基于ATL的复合控件CComCompositeControl，则可在OnInitDialog中添加。

 //Activate control in web browser immediately
 InPlaceActivate(OLEIVERB_UIACTIVATE);

2. 当鼠标点击控件时，应进行实地激活。此处主要响应WM_MOUSEACTIVATE事件。

LRESULT OnMouseActivate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
 //Activate control in web browser immediately
 InPlaceActivate(OLEIVERB_UIACTIVATE);
 return 0;
}

3. 然后就可在TranslateAccelerator中实现特点键盘事件处理了。

//**IOleInPlaceActiveObject::TranslateAccelerator()**

STDMETHOD(TranslateAccelerator)(MSG *pMsg)
{
if (
((pMsg->message >= WM_KEYFIRST) &&
    (pMsg->message <= WM_KEYLAST))
&&
((pMsg->wParam == VK_TAB) ||
    (pMsg->wParam == VK_RETURN))
)
{
CComQIPtr<IOleControlSite,&IID_IOleControlSite> 
spCtrlSite(m_spClientSite);
if(spCtrlSite)
{
return spCtrlSite->TranslateAccelerator(pMsg, 0);
}
}
return S_FALSE;
}

上述的代码是在子窗口的编辑框中处理TAB与ENTER键，如果你需要处理UP ARROW, DOWN ARROW, PAGE UP, and PAGE DOWN,可如下示例：

if((pMsg->wParam == VK_UP) ||
    (pMsg->wParam == VK_DOWN)||
   (pMsg->wParam == VK_LEFT) ||
       (pMsg->wParam == VK_RIGHT))
{
::IsDialogMessage(m_hWnd, pMsg);
return S_OK; 
}

如果Active X控件有滚动条，你需要处理VK_UP与VK_DOWN，如下示例：

if (pMsg->wParam == VK_UP)
{
::SendMessage(m_hWnd,WM_VSCROLL,
   SB_LINEUP,MAKELONG(0,m_hWnd));
return S_FALSE;
}

默认按钮的处理：当用户按下ENTER，你应该允许焦点转移到默认的按钮上（如果一个按钮设置为“默认”），那么你需要实现*IOleControl::GetControlInfo()*来接受ENTER与ESC键，ATL默认实现*IOleControlImpl::GetControlInfo()* 返回E_NOTIMPL,你需要覆盖它：

HRESULT STDMETHODCALLTYPE GetControlInfo(CONTROLINFO *pCI)
{
if(!pCI)
{
return E_POINTER;
}
pCI->hAccel = NULL; //load your accelerators here, if any   
pCI->cAccel = 0;    
pCI->dwFlags = 0;

return S_OK;
}

##### C#中调用Microsoft.VisualStudio.OLE.Interop

添加引用，从“管理NuGet程序包”中探索并安装。

![](https://img-blog.csdn.net/20180628112026128?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


安装后引用包：

![](https://img-blog.csdn.net/20180628112048455?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

不过拿到UserControl里面使用时很多都是接口没法调用，难道都需要自己实现？请有使用此包经验的朋友不吝赐教，谢谢！

目前我采用的是对控件注册处理键盘和鼠标事件：

![](https://img-blog.csdn.net/20180628112942400?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


注：此方法勉强解决了问题，但是使用文章中思路的方法尚待解决，记录之以备忘！




