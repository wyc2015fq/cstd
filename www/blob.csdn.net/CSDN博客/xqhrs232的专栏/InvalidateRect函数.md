# InvalidateRect函数 - xqhrs232的专栏 - CSDN博客
2012年09月07日 23:09:25[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：795标签：[null																[api																[360](https://so.csdn.net/so/search/s.do?q=360&t=blog)](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://www.cnblogs.com/sherlockhua/archive/2008/11/25/1340733.html](http://www.cnblogs.com/sherlockhua/archive/2008/11/25/1340733.html)
相关网帖
1.InvalidateRect function----[http://msdn.microsoft.com/en-us/library/windows/desktop/dd145002(v=vs.85).aspx](http://msdn.microsoft.com/en-us/library/windows/desktop/dd145002(v=vs.85).aspx)
2.请教InValidateRect()的用法？－－－－－－在线等。。。。。。----[http://topic.csdn.net/t/20060216/17/4559985.html](http://topic.csdn.net/t/20060216/17/4559985.html)
3.[UpdateWindow与InvalidateRect (2)](http://www.cnblogs.com/ztfei/archive/2012/08/23/2652351.html)----[http://www.cnblogs.com/ztfei/archive/2012/08/23/2652351.html](http://www.cnblogs.com/ztfei/archive/2012/08/23/2652351.html)
4.[关于Win32
 API的RedrawWindow函数和InvalidateRect函数](http://softwaredev.yo2.cn/articles/%E5%85%B3%E4%BA%8Ewin32-api%E7%9A%84redrawwindow%E5%87%BD%E6%95%B0%E5%92%8Cinvalidaterect%E5%87%BD%E6%95%B0.html)----[http://softwaredev.yo2.cn/articles/%E5%85%B3%E4%BA%8Ewin32-api%E7%9A%84redrawwindow%E5%87%BD%E6%95%B0%E5%92%8Cinvalidaterect%E5%87%BD%E6%95%B0.html](http://softwaredev.yo2.cn/articles/%E5%85%B3%E4%BA%8Ewin32-api%E7%9A%84redrawwindow%E5%87%BD%E6%95%B0%E5%92%8Cinvalidaterect%E5%87%BD%E6%95%B0.html)
5.Invalidate,InvalidateRect,UpdateWindow的一些细节----[http://www.360doc.com/content/12/0907/17/10660072_234859352.shtml](http://www.360doc.com/content/12/0907/17/10660072_234859352.shtml)
InvalidateRect函数
该函数添加一个区域到指定窗口的更新区域。更新区域代表窗口客户端的一部分，这部分必须进行重新绘制。
BOOL InvalidateRect( 
HWND hWnd, 
const RECT *lpRect, 
BOOL bErase
);
参数：
hWnd
指向需要重新绘制的窗口句柄。如果该参数为空，系统将更新和重新绘制所有的窗口，然后在该函数返回之前发送WM_ERASEBKGND给窗口过程.
lpRect
一个指向RECT的结构体，该结构包含了要添加到更新区域的长方形区域的坐标。如果该参数为NULL，整个客户端区域将会被添加到更新区域。
bErase
该参数为布尔型。指定了当更新区域重新绘制时，更新区域的背景是否被擦除掉。
返回值
函数执行成功时返回非零， 失败时返回零。要获取更多的关于错误的信息，可以调用GetLastError();
注意事项
参数hWnd不能为NULL
无效区域不会马上更新，相反会在更新区域“累积”，直到当下一个WM_PAINT消息发生时或者该区域已经被ValidateRect函数更新过。
当应用程序队列中没有其他的消息时，并且更新区域不为空时，系统将向窗口发送WM_PAINTA消息 。如果bErase为TRUE，则整个背景将会被擦除，而不是给定的区域。
