# VC中退出应用程序-几种很有用的方法 - xqhrs232的专栏 - CSDN博客
2013年09月03日 09:54:38[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：738
原文地址::[http://blog.csdn.net/xingyu19871124/article/details/6283658](http://blog.csdn.net/xingyu19871124/article/details/6283658)
相关网帖
1、vc6的OnInitDialog函数中直接退出----[http://dragoo747450.blog.163.com/blog/static/163606020118134390932/](http://dragoo747450.blog.163.com/blog/static/163606020118134390932/)
**1、OnOK()或OnCancel()//只对对话框有用**
**2、PostQuitMessage(0);//最常用**
**     PostMessage(WM_QUIT,0,0);//最常用**
**     ::SendMessage(AfxGetMainWnd()->m_hWnd,WM_CLOSE,0,0);//最常用     ::PostMessage(AfxGetMainWnd()->m_hWnd,WM_CLOSE,0,0);//最常用**
**3、ExitProcess(0);注意使用时先释放分配的内存,以免造成内存泄露**
**4、exit(0) 正常终止程序; exit(非0）非正常终止程序**
**5、OnClose();**
**void CMainFrame::OnClose() {        // TODO: Add your message handler code here and/or call default       if (MessageBox("确定要退出吗？","提示",MB_YESNO|MB_DEFBUTTON2)==IDYES)       {                CFrameWnd::OnClose();       }}**
**6、向主窗口发送WM_CLOSE消息       SendMessage(WM_CLOSE); 如果要在其它类关闭应用程序，则 AfxGetMainWnd->SendMessage(WM_CLOSE)**
