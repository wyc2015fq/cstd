# 《BREW进阶与精通——3G移动增值业务的运营、定制与开发》连载之83——BREW后台应用 - 我相信...... - CSDN博客





2010年09月29日 14:08:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1301








在BREW 3.x 版本以前，后台应用根本不响应suspend和resume事件，也处理有终端用户相关的交互操作，例如按键事件等等。

在BREW 3.x 以后，任何应用都可以对按键事件进行处理，可以简单地通过注册掩码为NMASK_SHELL_KEY的通知来实现。通过ISHELL_StartBackgroundApplet()直接启动一个后台应用，后台应用可以直接处理EVT_APP_START_BACKGROUND 事件而不是EVT_APP_START事件。在BREW 2.x中使一个应用处于后台运行状态的方法是，在调用ISHELL_CloseApplet() 且接收到EVT_APP_STOP时, 将 *(dwParam) 设置为FALSE，例如(*(oolean *)dwParam) = FALSE。

一个BREW应用与后台应用，或者后台应用与另一个后台应用之间一般采用事件传递的方式进行通信。终止一个后台应用的后台运行状态有三种情况，第一种是接收到其他应用的发来的事件，一种是后台应用执行完相关操作后调用ISHELL_CloseApplet()来关闭自己，另一种是后台应用调用ISHELL_StartApplet()使自己处于前台运行状态，然后通过用户的交互来关闭自己。

下面是BREW2.X中使用后台应用的示例：

typedef struct _bgApp

{

AEEApplet a;

oolean m_bGoBg; // used to toggle whether to run in background mode

} bgApp;

……

switch (eCode)

{

case EVT_APP_START:

if(pMe->m_bGoBg)

ISHELL_CloseApplet(pMe->a.m_pIShell, FALSE); // send applet to background

return TRUE;

case EVT_APP_STOP:

if(pMe->m_bGoBg)

*((oolean*) dwParam) = FALSE; // set dwParam to run in bg

return TRUE;

case EVT_USER:

if(pMe->m_bGoBg)

{

pMe->m_bGoBg = FALSE;

ISHELL_StartApplet(pMe->a.m_pIShell, AEECLSID_BGAPP); // make applet active

}else{

pMe->m_bGoBg = TRUE;

// trigger EVT_APP_STOP to send app to background

ISHELL_CloseApplet(pMe->a.m_pIShell, FALSE); 

}

return TRUE;

}



