# Invalidate、RedrawWindow与UpdateWindow的区别 - xqhrs232的专栏 - CSDN博客
2012年09月06日 16:08:29[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：806标签：[application																[function																[windows																[null																[system																[api](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=system&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://hi.baidu.com/aresial/item/1da770d995fc4936e2108fac](http://hi.baidu.com/aresial/item/1da770d995fc4936e2108fac)
相关网帖
1.VC窗口刷新InvalidateRect和UpdateWindow----[http://www.360doc.com/content/10/0916/10/2467325_54035145.shtml](http://www.360doc.com/content/10/0916/10/2467325_54035145.shtml)
2.Invalidate和UpdateWindow的区别----[http://www.360doc.com/content/11/1008/18/7691297_154392127.shtml](http://www.360doc.com/content/11/1008/18/7691297_154392127.shtml)（MSDN翻译）
3.[updatewindow invalidaterect](http://www.cnblogs.com/shuiren/archive/2012/02/28/2371550.html)----[http://www.cnblogs.com/shuiren/archive/2012/02/28/2371550.html](http://www.cnblogs.com/shuiren/archive/2012/02/28/2371550.html)
4.[RedrawWindow， UpdateWindow，InvalidateRect 用法](http://www.cnblogs.com/likebeta/archive/2012/07/30/redrawwindow-updatewindow-invalidaterect.html)----[http://www.cnblogs.com/likebeta/archive/2012/07/30/redrawwindow-updatewindow-invalidaterect.html](http://www.cnblogs.com/likebeta/archive/2012/07/30/redrawwindow-updatewindow-invalidaterect.html)
一：什么时候才会发生重绘窗口的消息？ 
      当需要更新或重新绘制窗口的外观时，应用程序就会发送WM_PAINT消息。对窗口进行重新绘制。 
二：Invalidate() -- RedrawWindow() -- UpdateWindow()三个函数有什么异同？ 
      Invalidate()是强制系统进行重画，但是不一定就马上进行重画。因为Invalidate()只是通知系统，此 时的窗口已经变为无效。强制系统调用WM_PAINT，而这个消息只是Post就是将该消息放入消息队列。当执行到WM_PAINT消息时才会对敞口进行重绘。
    UpdateWindow只向窗体发送WM_PAINT消息，在发送之前判断GetUpdateRect(hWnd,NULL,TRUE)看有无可绘制的客户区域，如果没有，则不发送WM_PAINT。
    RedrawWindow()则是具有Invalidate()和UpdateWindow()的双特性。声明窗口的状态为无效，并立即更新窗口，立即调用WM_PAINT消息处理。
三：刷新组合
InvalidateRect(hctrl,null,true) ; 
UpdateWindow(hctrl); 
这两个函数组合起来是什么意思呢？
InvalidateRect是会触发WM_PAINT事件，但是不是立即就触发，一般都会等当前操作的过程结束才触发， 如果需要立即触发，那么配合UpdateWindow()使用就可以了。先执行InvalidateRect，再执行UpdateWindow().
/************************************************************************************/
Invalidate在消息队列中加入一条WM_PAINT消息，其无效区为整个客户区。
UpdateWindow直接发送一个WM_PAINT消息，其无效区范围就是消息队列中WM_PAINT消息（最多只有一条）的无效区。
效果很明显，当调用Invalidate之后，屏幕不一定马上更新，因为WM_PAINT消息不一定在队列头部，而调用UpdateWindow会使WM_PAINT消息马上执行的，绕过了消息队列。
如果调用Invalidate之后想马上更新屏幕，那就加上UpdateWindow()这条语句。 
MSDN的解释 
UpdateWindow 
The UpdateWindow function updates the client area of the specified window by sending a WM_PAINT
message to the window if the window's update region is not empty. The function sends a WM_PAINT
 message directly to the window procedure of the specified window, bypassing the application queue.
If the update region is empty, no message is sent.  
InvalidateRect 
The system sends a WM_PAINT message to a window whenever its update region is not empty and
 there are no other messages in the application queue for that window.  
翻译成中文大概的解释如下： 
  UpdateWindow：如果有无效区，则马上sending a WM_PAINT message到窗口处理过程，不进消息队列进行排队等待，立即刷新窗口，否则，什么都不做。
 InvalidateRect：设置无效区，如果为NULL参数，则设置整个窗口为无效区。当应用程序的那个窗口的消息队列为空时，则sending a WM_PAINT message(即使更新区域为空).在sending a WM_PAINT message的所有InvalidateRect的更新区域会累加。
 1:设置无效区 
 InvalidateRect 
 2：立即刷新 
 UpdateWindow(); 
如果不调用 InvalidateRect就调用 UpdateWindow，那么UpdateWindow什么都不做。 ??????
如果调用 InvalidateRect 后不调用UpdateWindow，则系统会自动在窗口消息队列为空的时候，系统自动发送一WM_PAINT消息。 
调用UpdateWindow()时将会发送一个WM_PAINT消息，而应用程序在接收到WM_PAINT消息后，将自动地调用Invalidate(),所以，在程序代码中，不一定要出现Invalidate()!
UpdateWindow()就是立即发送WM_PAINT消息,只对声明无效的区域起作用，   
  Invalidate()则是声明无效的方式之一。
Invalidate()表示客户区域无效，在下次WM_PAINT发生时重绘。而WM_PAINT是由系统进行维护的，每当CWnd的更新区域不为空，并且在应用程序的窗口消息队列中没有其它消息时，Windows就发送一条WM_PAINT消息。  
  Invalidate里面有个bool型的参数，用来标识重绘的时候是否用背景色填充。是不是用SetBkcolor函数？下去继续研究。
 updateWindow则是要求系统对区域进行立即重绘。
 看到有人在网上提出问题，他在Invalidate后面又写了绘图的函数但是没有执行，因为invalidate执行过以后转到PAINT命令了。所以后面的都没有显示。
 也终于想通我绘的图一直在闪啊闪，因为我在PAINT里面用到Invalidate()函数，所以他不停的自嵌套，倒是绘的图不停的闪。
Invalidate让客户区处于可以重画的状态，而UpdateWindow开始重画，但是它先判断客户区是否为空，不空UpdateWindow不执行，为空才执行重画。
Invalidat最后也是调用InvalidatRect,在windows API里只有InvalidatRect的


