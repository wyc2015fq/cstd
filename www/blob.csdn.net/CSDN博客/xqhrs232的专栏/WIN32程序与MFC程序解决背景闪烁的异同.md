# WIN32程序与MFC程序解决背景闪烁的异同 - xqhrs232的专栏 - CSDN博客
2012年06月26日 18:17:46[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：962

最近弄WIN32程序，发现以前的认识的一个错误，以前写MFC程序多一点，也没怎么在意一些细节部分。
以前经常在写MFC程序的时候把贴背景的操作放在WM-ERASEBKGND消息的响应函数里面，要刷新按钮就用INVALIDATERECT函数来激发系统的消息（WM-ERASEBKGND消息+WM-PAINT消息），这样在MFC下居然是没有问题的；但在WIN32程序下也这样做，发现很难实现界面按钮的对应状态显示。（要不就是按钮状态更新的不对，要不就是只画了当前按下的那个按钮，其它按钮都画不出来了），但同样逻辑的程序在MFC程序下不会有这样的问题。
  （都是先响应WM-ERASEBKGND消息，然后才是WM-PAINT消息，按道理/逻辑是没有问题的，但在WIN32下只能会出当前被点击的按钮，但在MFC程序下这样的流程是OK的）
    其实我怀疑以前在MFC程序下的做法是错误的，其实在WM-ERASEBKGND消息的响应函数里面都应该直接返回TRUE才对。然后贴背景跟贴按钮的图标一起贴，只是他们有一个先后罢了，先贴背景再贴其它图标，都只是在WM-PAINT消息的响应函数里面来完成。
（还有一种可能就是MFC框架针对WM-ERASEBKGND消息+WM-PAINT消息的处理自己加了对应的一个逻辑判断，免掉了一些不需要重绘区域的重绘，也就没WIN32程序下出现的那样的问题）
    有太多图标要贴的时候就用用双缓冲技术，把贴好背景图的DC程序创建一份出来然后把要贴的图标先贴到这个新建的DC上，贴完再把这个DC贴到系统的DC上，这样就不闪了，并且这样对于背景的操作也是对的。
 //========================================
[VC 双缓冲绘图](http://blog.csdn.net/xqhrs232/article/details/7680184)----[http://blog.csdn.net/xqhrs232/article/details/7680184](http://blog.csdn.net/xqhrs232/article/details/7680184)
VC 双缓冲绘图
BOOL CGraphDlg::OnDraw(CDC* pDC)
{
       CRect rc;
       GetClientRect(&rc); 
       CDC  MemDC;
       CBitmap  MemBitmap;  
       MemDC.CreateCompatibleDC(NULL);      
       MemBitmap.CreateCompatibleBitmap(pDC, rc.Size().cx,rc.Size().cy);
       CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);      
       MemDC.BitBlt(0,0,rc.width, rc.height, pDC,0,0,SRCCOPY);
       m_pGraph->SetGraphDC(&MemDC);
       m_pGraph->DoGraphShowEvent();  
       pDC->BitBlt(0,0, rc.Size().cx,rc.Size().cy,&MemDC,0,0,SRCCOPY);  
       MemDC.SelectObject(pOldBit);
       MemBitmap.DeleteObject();
       MemDC.DeleteDC();
}
