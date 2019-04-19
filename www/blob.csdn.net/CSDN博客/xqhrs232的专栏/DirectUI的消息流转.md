# DirectUI的消息流转 - xqhrs232的专栏 - CSDN博客
2012年09月22日 20:47:51[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：944
原文地址::[http://blog.csdn.net/mail_cm/article/details/7793516](http://blog.csdn.net/mail_cm/article/details/7793516)
Windows是一个基于消息循环的系统，DirectUI同样遵循这样的消息流转。当界面呈现、用户点击、定时器等各种各样的消息一旦进入windows消息循环队列，系统自动调用该窗口的WndProc过程。对于DirectUI程序来说，因为所有的界面都是逻辑界面，均基于程序运行之初通过CreateWindowEx生成的原始窗口，因此，所有的消息最开始都会先流转到DirectUI中CWindowWnd类的__WndProc函数中，然后再统一的分发。
可以在Duilib的应用中通过设置断点来跟踪整个消息的流程，大体上，下图表示了这个流转过程：
![DirectUI的消息流转 - 愉悦爸 - 每一天都是值得纪念的](http://img0.ph.126.net/d43BH6NyE4tceVhrIvDZLQ==/1290844243212787830.jpg)
值得注意的是，多个界面，如CWindowWnd, Container等，可以共用同一个CPaintManagerUI来管理界面呈现与消息分发，只需要调用同一个CPaintManagerUI的AddNotifier【这个一般意味着你需要定义一个全局的CPaintMagagerUI变量而不是一个类的变量】。
尤其值得注意的是，共用同一个CPaintManagerUI的情况下，很多时候可能是使用界面元素如Button的Name来区分是哪一个元素，因此，务必注意每个元素的名字是唯一的。
下面是才代码摘录出来的片段：
LRESULT CALLBACK CWindowWnd::__WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){CWindowWnd* pThis = NULL;if( uMsg == WM_NCCREATE ){        LPCREATESTRUCT lpcs =reinterpret_cast<LPCREATESTRUCT>(lParam);        pThis =static_cast<CWindowWnd*>(lpcs->lpCreateParams);        pThis->m_hWnd = hWnd;::SetWindowLongPtr(hWnd, GWLP_USERDATA,reinterpret_cast<LPARAM>(pThis));}else{        pThis =reinterpret_cast<CWindowWnd*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));if( uMsg == WM_NCDESTROY && pThis != NULL ){            LRESULT lRes =::CallWindowProc(pThis->m_OldWndProc, hWnd, uMsg, wParam, lParam);::SetWindowLongPtr(pThis->m_hWnd, GWLP_USERDATA,0L);if( pThis->m_bSubclassed ) pThis->Unsubclass();            pThis->m_hWnd = NULL;            pThis->OnFinalMessage(hWnd);return lRes;}}if( pThis != NULL ){return pThis->HandleMessage(uMsg, wParam, lParam);}else{return::DefWindowProc(hWnd, uMsg, wParam, lParam);}}
将自身加入到消息收听队列：
g_PM->AddNotifier(this);//g_PM为全局的CPaintManagerUI变量
HandleMassage的实现：
 LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam){  LRESULT lRes =0;  BOOL bHandled = TRUE;switch( uMsg ){case WM_CREATE:        lRes =OnCreate(uMsg, wParam, lParam, bHandled);break;case WM_CLOSE:         lRes =OnClose(uMsg, wParam, lParam, bHandled);break;case WM_DESTROY:       lRes =OnDestroy(uMsg, wParam, lParam, bHandled);break;case WM_NCACTIVATE:    lRes =OnNcActivate(uMsg, wParam, lParam, bHandled);break;case WM_NCCALCSIZE:    lRes =OnNcCalcSize(uMsg, wParam, lParam, bHandled);break;case WM_NCPAINT:       lRes =OnNcPaint(uMsg, wParam, lParam, bHandled);break;case WM_NCHITTEST:     lRes =OnNcHitTest(uMsg, wParam, lParam, bHandled);break;case WM_SIZE:          lRes =OnSize(uMsg, wParam, lParam, bHandled);break;case WM_GETMINMAXINFO: lRes =OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled);break;case WM_SYSCOMMAND:    lRes =OnSysCommand(uMsg, wParam, lParam, bHandled);break;case WM_KEYDOWN:OutputDebugString(_T("It is a key down\r\n"));break;default:  bHandled = FALSE;}if( bHandled )return lRes;if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes))return lRes;returnCWindowWnd::HandleMessage(uMsg, wParam, lParam);}
遍历收听者并发送通知：
......
if(!bAsync ){// Send to all listenersif(Msg.pSender != NULL ){if(Msg.pSender->OnNotify)Msg.pSender->OnNotify(&Msg);}for(int i =0; i < m_aNotifiers.GetSize(); i++){static_cast<INotifyUI*>(m_aNotifiers[i])->Notify(Msg);}}
......
Notify的实现：
voidSettingUI::Notify(TNotifyUI& msg){CStdString name = msg.pSender->GetName();if(msg.sType == _T("windowinit")){}elseif( msg.sType == _T("click")){
}
.....

