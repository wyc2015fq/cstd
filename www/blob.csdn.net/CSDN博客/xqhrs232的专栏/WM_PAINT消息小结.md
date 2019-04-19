# WM_PAINT消息小结 - xqhrs232的专栏 - CSDN博客
2012年06月21日 11:16:42[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：795标签：[wince																[mfc																[windows																[工作																[优化](https://so.csdn.net/so/search/s.do?q=优化&t=blog)](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=wince&t=blog)
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://www.51cuco.com/study/otherprom/618.html](http://www.51cuco.com/study/otherprom/618.html)
WM_PAINT是Windows窗口系统中一条重要的消息，应用程序通过处理该消息实现在窗口上的绘制工作。
## 1. 系统何时发送WM_PAINT消息？
系统会在多个不同的时机发送WM_PAINT消息：当第一次创建一个窗口时，当改变窗口的大小时，当把窗口从另一个窗口背后移出时，当最大化或最小化窗口时，等等，这些动作都是由 系统管理的，应用只是被动地接收该消息，在消息处理函数中进行绘制操作；大多数的时候应用也需要能够主动引发窗口中的绘制操作，比如当窗口显示的数据改变的时候，这一般是通过InvalidateRect和 InvalidateRgn函数来完成的。InvalidateRect和InvalidateRgn把指定的区域加到窗口的Update Region中，当应用的消息队列没有其他消息时，如果窗口的Update
 Region不为空时，系统就会自动产生WM_PAINT消息。
系统为什么不在调用Invalidate时发送WM_PAINT消息呢？又为什么非要等应用消息队列为空时才发送WM_PAINT消息呢？这是因为系统把在窗口中的绘制操作当作一种低优先级的操作，于是尽 可能地推后做。不过这样也有利于提高绘制的效率：两个WM_PAINT消息之间通过InvalidateRect和InvaliateRgn使之失效的区域就会被累加起来，然后在一个WM_PAINT消息中一次得到 更新，不仅能避免多次重复地更新同一区域，也优化了应用的更新操作。像这种通过InvalidateRect和InvalidateRgn来使窗口区域无效，依赖于系统在合适的时机发送WM_PAINT消息的机
 制实际上是一种异步工作方式，也就是说，在无效化窗口区域和发送WM_PAINT消息之间是有延迟的；有时候这种延迟并不是我们希望的，这时我们当然可以在无效化窗口区域后利用SendMessage 发送一条WM_PAINT消息来强制立即重画，但不如使用Windows GDI为我们提供的更方便和强大的函数：UpdateWindow和RedrawWindow。UpdateWindow会检查窗口的Update Region，当其不为空时才发送WM_PAINT消息；RedrawWindow则给我们更多的控制：是否重画非客户区和背景，是否总是发送WM_PAINT消息而不管Update
 Region是否为空等。
## 2. BeginPaint
BeginPaint和WM_PAINT消息紧密相关。试一试在WM_PAINT处理函数中不写BeginPaint会怎样？程序会像进入了一个死循环一样达到惊人的CPU占用率，你会发现程序总在处理一个接 一个的WM_PAINT消息。这是因为在通常情况下，当应用收到WM_PAINT消息时，窗口的Update Region都是非空的（如果为空就不需要发送WM_PAINT消息了），BeginPaint的一个作用就是把该Update Region置为空，这样如果不调用BeginPaint，窗口的Update Region就一直不为空，如前所述，系统就会一直发送WM_PAINT消息。
BeginPaint和WM_ERASEBKGND消息也有关系。当窗口的Update Region被标志为需要擦除背景时，BeginPaint会发送WM_ERASEBKGND消息来重画背景，同时在其返回信息里有一个标志表明窗口背景是否被重画过。当我们用InvalidateRect和InvalidateRgn来把指定区域加到Update Region中时，可以设置该区域是否需要被擦除背景，这样下一个BeginPaint就知道是否需要发送WM_ERASEBKGND消息了。
另外要注意的一点是，BeginPaint只能在WM_PAINT处理函数中使用。
//======================================================
备注::
 1>BeginPaint只能在WM_PAINT处理函数中使用
2>WM_PAINT消息先产生，WM_ERASEBKGND消息后产生，WM_ERASEBKGND消息由处理WM_PAINT消息时产生，由BeginPaint函数判断当前窗口的Update Region标志是否为需要擦除背景，如果需要擦除背景就产生WM_ERASEBKGND消息来重画背景，否则不产生WM_ERASEBKGND消息，只继续执行WM_PAINT消息的响应代码。
(但亲自做实验不是这样的结果----PC上的WIN32程序先产生WM_ERASEBKGND消息，并且产生2次，WINCE上的WIN32程序才是先产生WM_PAINT消息，WM_ERASEBKGND消息也只产生1次，怎么会有这样的区别，还真是困惑了，PC跟WINCE在这个上有这样的区别？不可能吧！？但运行跑起来后用InvalidateRect函数来做实验整个逻辑又是WM_PAINT消息先产生，WM_ERASEBKGND消息后产生，WM_ERASEBKGND消息先被响应执行完，PC环境下对消息的捕捉不准，因为还有其它程序/系统运行的存在，比如程序最后响应的是WM_ERASEBKGND消息，但程序并没有显示WM_ERASEBKGND消息中的背景，而是WM_PAINT消息中的背景)
3>虽然WM_PAINT消息先产生，WM_ERASEBKGND消息后产生，但如果两个消息都产生了，WM_ERASEBKGND消息会先被执行完，然后再是WM_PAINT消息除BeginPaint函数以外的其它代码的执行-----这一点可以通过打印消息追踪一下，或者在WM_PAINT和WM_ERASEBKGND消息处理的代码都分别贴一张全屏图片看最后显示的谁的图片就可以知道谁先执行完，谁执行完在后
4>WIN32的程序跟MFC的程序在WM_PAINT和WM_ERASEBKGND消息的处理上有区别，MFC封装了具体的细节，只能追踪到他们的消息执行体，所以会造成WM_ERASEBKGND消息先于WM_PAINT消息产生的错觉！！！
