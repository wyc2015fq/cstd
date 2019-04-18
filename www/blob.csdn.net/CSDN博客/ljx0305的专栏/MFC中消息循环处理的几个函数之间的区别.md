# MFC中消息循环处理的几个函数之间的区别 - ljx0305的专栏 - CSDN博客
2008年03月06日 23:03:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：942标签：[mfc																[windows																[编程](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)
# MFC中消息循环处理的几个函数之间的区别
### [开发者在线 Builder.com.cn](http://www.builder.com.cn/)
Windows编程中一个比较原始的问题就是消息循环，现在很少有人关心这些了; 
　　我个人对消息循环比较感兴趣，对底层的很多细节想深入了解!
　　以下说明几个消息循环中的常用函数进行对比
　　1 PostMessage 与 SendMessage 函数对比
　　SendMessage把消息直接发送到窗口，并调用此窗口的相应消息处理函数，等消息处理函数结束后SendMessage才返回!SendMessage发送的消息不进入系统的消息队列;SendMessage函数有返回值
　　PostMessage将消息发送到与创建窗口的线程相关联的消息队列后立即返回;PostMessage函数没有返回值;
　　2 GetMessage 与 PeekMessage函数的对比
　　GetMessage(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax)
　　PeekMessage(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax,UINT wRemoveMsg)
　　根据参数可以看出以上2个函数的区别，参数wRemoveMsg的作用是指定消息获取的方式，如果设为PM_NOREMOVE,那么消息将不会从消息队列中被移出，如果设为PM_REMOVE，那么消息将会从消息队列中被移出;
　　还有区别：
　　他们如果没有捕获到消息，程序的主线程会被操作系统挂起。当操作系统再次回来照顾此线程时，发现消息队列中仍然没有消息可取，此时两个函数的行为就不同了：
　　GetMessage : 过门不入，操作系统再次挂起此线程，去照顾别的线程;
　　PeekMessage: 取回控制权，使程序执行一段时间，等待可能的消息进入消息队列并将其捕获;这时程序进入空闲时间阶段; 
引用[http://www.builder.com.cn/2007/1027/583154.shtml](http://www.builder.com.cn/2007/1027/583154.shtml)
