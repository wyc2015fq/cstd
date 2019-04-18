# DirectX9.0 入门手册(1) - 深之JohnChen的专栏 - CSDN博客

2005年12月19日 14:35:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3311标签：[null																[windows																[callback																[编程																[timer																[struct](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=timer&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=callback&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)
个人分类：[游戏空间](https://blog.csdn.net/byxdaz/article/category/160195)

这一章我先不写有关DX的东西,我先从最基本的窗口创建讲起,然后再慢慢讲解使用DX的一些内容.
我写这个指南的主要目的是为了学习。我希望自己可以通过写这个指南更快地学会DirectX。同时，我也希望为其他想学习的同伴提供一些学习资料。在编程方面,我并不是很强的人，再加上人总是会犯错的，如果我这些文字给贻笑大方的话,我接受大家对我提出有建设性的批评，如果你有更好的想法要和我交流，可以联系我:fowenler@126.com

    下面正式开始吧,先讲窗口类,创建窗口,销毁窗口,窗口消息处理函数.




·窗口类WNDCLASS











**style****：**用来定义窗口的行为。如果打算共同使用GDI和D3D的话，可以使用CS_OWNDC作为参数。



**lpfnWndProc****：**一个函数指针，指向与这个窗口类绑定在一起的处理窗口消息的函数。



**cbClsExtra****和****cbWndExtra****：**为窗口和为分配内存空间。很少使用到这两个参数，一般设为0；



**hInstance****：**应用程序的实例句柄。你可以使用GetModuleHandle()来得到它，也可以从Win32程序的入口函数WinMain那里得到它。当然，你也可以把它设为NULL（不知有什么用）



**hIcon****，****hCursor****，****hbrBackground****：**设置默认的图标、鼠标、背景颜色。不过在这里设置这些其实并不怎么重要，因为我们可以在后面定制自己的渲染方法。



**lpszMenuName****：**用来创建菜单



**lpszClassName****：**窗口类的名字。我们可以通过这个名字来创建以这个窗口类为模板的窗口。甚至可以通过这个名字来得到窗口的句柄。



设置好窗口类结构的内容后，使用RegisterClass(const WNDCLASS *lpWndClass)函数来注册它。关闭窗口后可以用UnregisterClass(LPCSTR lpClassName, HINSTANCE hInstance)来撤销注册。



·创建窗口CreateWindow










**lpClassName****：**窗口类的名字。即窗口类结构体中的lpszClassName成员。



**lpWindowName****：**如果你的应用程序有标题栏，这个就是你标题栏上显示的内容。



**dwStyle****：**窗口的风格决定你的窗口是否有标题栏、最大最小化按钮、窗口边框等属性。在全屏的模式下，WS_POPUP|WS_VISIBLE是常用的设置，因为它产生一个不带任何东西的全屏窗口。在窗口的模式下，你可以设置很多窗口的风格，具体可以查看相关资料，这里不详细说明，不过WS_OVERLAPPED|WS_SYSMENU|WS_VISIBLE是一组常用的风格。



**x****和****y****：**窗口创建的位置。(x，y)表示窗口的左上角位置。



**nWidth****和****nHeight****：**用来设置窗口的宽度和高度，以像素为单位。如果你想创建一个全屏的窗口，使用GetSystemMetrics(SM_CXSCREEN)和GetSystemMetrics(SM_CYSCREEN)可以得到当前显示器屏幕的大小



**hWndParent****：**指定这个新建窗口的父窗口。在D3D应用程序中很少用，一般设为NULL。



**hMenu****：**菜单句柄。



**hInstance****：**应用程序的实例句柄。你可以使用GetModuleHandle()来得到它，也可以从Win32程序的入口函数WinMain那里得到它。当然，你也可以把它设为NULL（不知有什么用）



**lpParam****：**一个很神秘的参数。除非你知道自己在做什么，否则还是把它设为NULL吧。



·销毁窗口DestroyWindow



销毁窗口有两种方法，一种是隐式的，一种是显式的。我们都知道Windows操作系统是一个基于消息驱动的系统。流动于系统中的消息使我们的窗口跑起来。在很多软件开发特别是商业软件的开发过程中，窗口的产生和销毁都是交由系统去做的，因为这些不是这类开发的关注所在。但是游戏开发不一样，尽管你也可以只向系统发送一条WM_DESTROY消息来销毁窗口，我们还是希望窗口是销毁的明明白白的。由于窗口的注册、产生和使用都是由我们亲手来做的，那么当然窗口的销毁也得由我们亲自来做。不过还是得说明一点，使用WM_DESTROY消息和DestroyWindow函数来销毁窗口在本质上并无太大差别，使用哪种方法可以说是根据个人的爱好吧。



销毁窗口后是不是就完事了呢？不，还没有，因为应用程序的消息队列里可能还有没处理完的消息，为了彻底的安全，我们还得把那些消息都处理完。所以结束应用程序的时候，可以使用以下方法：



MSG msg;



DestroyWindow(h_wnd);



while(PeekMessage(&msg , NULL , 0 , 0 , PM_REMOVE))



{



TranslateMessage(&msg);



DispatchMessage(&msg);



}



·窗口消息处理过程



窗口消息的处理函数是一个回调函数，什么是回调函数？就是由操作系统负责调用的函数。CALLBACK这个宏其实就是__stdcall，这是一种函数调用的方式，在这里不多说这些了，有兴趣的可以参考一些Windows编程的书籍，里面会有很详尽的说明。



Windows里面有很多消息，这些消息都跑去哪里了呢？其实它们都在自己的消息队列里等候。消息是怎么从队列里出去的呢？就是通过GetMessage和PeekMessage这两个函数。那么消息从队列里出去后又到哪里了呢？嗯，这时候消息就正式进入了我们的窗口消息处理过程，也即是窗口类中lpfnWndProc所指定的函数。一个消息处理函数有四个参数，下面分别说说：



参数1：HWND p_hWnd



消息不都是传到以窗口类为模板产生的窗口吗？为什么还要使用窗口句柄来指明窗口呢？别忘了一个窗口类是可以产生多个窗口的呀，如果一个应用程序里面有多个窗口，并且它们之中的一些窗口是共用一个窗口类的，那么就得用一个窗口句柄来指明究竟这个消息是哪个窗口发过来的。



参数2：UINT p_msg



这是一个消息类型，就是WM_KEYDOWN , WM_CLOSE , WM_TIMER这些东东。



参数3：WPARAM p_wparam



这个参数内容就是消息的主要内容。如果是WM_KEYDOWN消息，那么p_wparam就是用来告诉你究竟是哪个键被按下。



参数4：LPARAM p_lparam



这个参数的内容一般是消息的一些附加内容。



最后说明一下DefWindowProc的作用。有时候我们把一个消息传到窗口消息处理函数里面，但是里面没有处理这个消息的内容。怎么办？很容易，交给DefWindowProc处理就对了。







嗯，这一章就说到这了，下一章介绍如何创建一个D3D应用程序。



