
# DirectDraw编程方法与技巧 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月04日 17:27:56[seven-soft](https://me.csdn.net/softn)阅读数：694标签：[编程																](https://so.csdn.net/so/search/s.do?q=编程&t=blog)[windows																](https://so.csdn.net/so/search/s.do?q=windows&t=blog)[工作																](https://so.csdn.net/so/search/s.do?q=工作&t=blog)[数据结构																](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)[microsoft																](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)[interface																](https://so.csdn.net/so/search/s.do?q=interface&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=工作&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=工作&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=编程&t=blog)
1　概　述DirectX是Microsoft为软件开发人员提供的一套精心设计的接口，用于开发高性能、实时的应用程序。它以COM（component object modal）为基本结构［1］，位于硬件和软件之间，像gdi(graphics device interface)一样提供了硬件无关的API(application programming interface)接口；它和GDI有一重要不同点：DirectX是一套底层的API接口，它提供了直接访问硬件的能力，使得DirectX应用程序能充分发挥硬件的威力。
DirectDraw是DirectX中提供直接操纵显存、执行硬件映射、硬件覆盖及切换显示页等功能的组件。它不但兼容已有的Windows应用程序和驱动程序，而且还兼容许多显示卡：从简单的SVGA到支持图像剪裁、拉伸和非RGB格式图像的高档显示卡。DirectDrawHAL(hardware－abstraction layer)抽象了显示卡的硬件功能，以设备无关的方式提供了一些以前是设备相关的功能，如多显示页技术、访问并控制显示卡映射寄存器、支持3DZ-Buffer、支持Z－order硬件覆盖、访问图像拉伸硬件，以及同时访问标准显存和控制显存。正因为DirectDraw有这些优点，现在许多基于Windows95/98/NT的游戏程序都使用了DirectDraw；而它的设备无关性使开发者摆脱了繁重的显示卡接口工作，集中精力实现程序的主要功能。DirectX中的DDraw.dll实现了DirectDraw所需要的函数、对象及其接口。下面先介绍DirectDraw中主要的函数、对象和接口，然后再说明DirectDraw的使用方法。
2　DirectDraw中的函数、对象和接口简介
DirectDraw包括一个代表显示卡的主对象DirectDraw，由函数DirectDrawCreate创建，实现接口IDirectDraw和IDirectDraw2。如果机器上装有多个显示卡，可以为每个显示卡创建一个DirectDraw对象。也可以创建多个DirectDraw对象，它们各自独立并代表同一物理对象。IDirectDraw是为了和以前版本DirectX兼容而保留的接口，基于DirectX3以上版本的程序应该使用IDirectDraw2。IDirectDraw2和IDirectDraw接口所包含的方法定义基本相同，但具体实现不同。DirectDraw对象主要用于创建其它3个对象：DirectDrawSurface、DirectDrawPalette、DirectDrawClipper。
DirectDrawSurface对象由函数IDirectDraw2::CreateSurface创建，代表显存中一块线性区域，实现接口IDirectDrawSurface和IDirectDrawSurface2。IDirectDrawSurface是为了和以前版本DirectX兼容而保留的接口，基于DirectX3以上版本的程序应该使用IDirectDrawSurface2。二者的接口定义也基本相同。可以为一个DirectDraw对象创建多个IDirectDrawSurface对象，代表物理屏幕或逻辑屏幕，通过IDirectDrawSurface2::Flip、IDirectDrawSurface2::BltFast等方法切换显示页或映射部分屏幕内容。
DirectDrawPalette对象由函数IDirectDraw2::CreatePalette创建，实现接口IDirectDrawPalette。它代表显示卡的物理调色板，可以是16色或256色。每个DirectDrawPalette必须附着（attach）在一个DirectDrawSurface上，不同的DirectDrawSurface对象可以有不同的DirectDrawPalette。
DirectDrawClipper对象由函数IDirectDraw2::CreateClipper或DirectDrawCreateClipp
er创建，实现接口IDirectDrawClipper。DirectDraw用它来处理屏幕的剪贴。它常用于在Window模式（与全屏模式相对应）下运行的DirectDraw程序，在使用前也必须被附着在一个DirectDrawSurface上。
3　DirectDraw程序一般工作过程
和一般的Windows程序［2］一样，DirectDraw程序要先创建一个主窗口，然后进行DirectDraw的初始化：创建所需要的对象，设置程序的工作模式，建立必要的数据结构。在初始化工作完成后，就可以在主窗口的消息循环中根据用户的输入调用相应对象的方法。
3．1　初始化DirectDraw
3．1．1　创建DirectDraw对象
首先调用DirectDrawCreate创建代表某个显示卡的DirectDraw对象：
ddres=DirectDrawCreate(NULL,&lpDD,NULL);
DirectDrawCreate的第一项参数是代表显示卡驱动程序的GUID（globally unique identifier），若为NULL则表示采用系统默认的驱动程序。第二项参数是IDirectDraw接口类型指针的地址，用于接收指向由DirectDrawCreate所创建对象的指针。这个指针不需应用程序预先分配内存。DirectDrawCreate成功时会调用AddRef将对象的引用计数加1。如果要知道系统中所有驱动程序的GUID，可以调用DirectDrawEnumerate，它接收两个参数：回调函数的地址和传给回调函数的自定义数据的地址，其工作方式和Win32
 API Enum Windows类似。
3．1．2　设置DirectDraw的工作模式
创建DirectDraw对象后应该马上设置DirectDraw对象的工作模式：
ddres=lpDD→SetCooperativeLevel(hWnd,DDSCL＿EXCLUSIVE|DDSCLFUL＿LSCREEN);
SetCooperativeLevel的第一个参数是和DirectDraw对象关联的窗口句柄，一般是程序主窗口的句柄；第二个参数指明了DirectDraw对象的工作模式。DirectDraw对象有两种工作模式：普通模式（Windowedmode，参数DDSCL＿NORMAL）和独占模式（Full＿Screen　mode，参数DDSCL＿EXCLUSIVE）。普通模式下DirectDraw和普通Windows程序的区别不大，主要是DirectDraw程序可以随心所欲地读取整个屏幕的内容或在屏幕的任意位置输出，而其它的Windows程序毫无察觉。独占模式就是游戏“红色警报”和“赤壁”所采用的方式，并必须和全屏模式（参数DDSCL＿FULLSCREEN）联用，此时程序的主窗口被扩展为整个屏幕。其它应用程序都成为后台程序，使用Alt+Tab键可以在程序间切换。
3．1．3　得到IDirectDraw2类型的接口
接下来利用COM的重要方法QueryInterface，通过IDirectDraw接口得到一个IDirectDraw2类型的接口。QueryInterface成功时会将对象的引用计数加1，而我们也不再需要IDirectDraw接口，因此这里调用IDirectDraw::Release将对象的引用计数减1，也即释放先前得到的IDirectDraw接口。
3．1．4　根据需要切换屏幕显示模式
如果DirectDraw的工作模式设定为全屏独占模式，则可以根据需要切换屏幕显示模式：
ddres=lpDD2→SetDisplayMode(800,600,16);
SetDisplayMode的前两个参数是屏幕的横、纵分辨率，最后一个是每个像素点的颜色位数。上例将屏幕设为800×600，16位色。
3．1．5　创建DirectDrawSurface对象并得到IDirectDrawSurface2接口
创建DirectDraw对象后，下一步调用IDirectDraw2::CreateSurface创建代表物理屏幕或逻辑屏幕的DirectDrawSurface对象。IDirectDraw2::CreateSurface的第一个参数是DDSURFACEDESC结构的地址，第二个参数是一个IDirectDrawSurface接口类型的指针地址，第三个参数必须是NULL。数据结构DDSURFACEDESC包含了创建DirectDrawSurface所需信息。在得到一个IDirectDrawSurface类型接口后，仍使用QueryInterface得到一个IDirectDrawSurface2类型的接口，并释放先前得到的IDirectDrawSurface接口：
／／Get the IDirectDrawSurface2interface.
LPDIRECTDRAWSURFACE2lpDDSPrimary=NULL；
ddres=lpDDSPrimaryTemp>QueryInterface(IID＿IDirectDrawSurface2,(LPVOID)&lpDDSPrimary；
lpDDSPrimaryTemp→Release()；
上面得到的lpDDSPrimary指向代表物理屏幕的对象。还需调用IDirectDrawSurface2::GetAttachedSurface得到指向创建时附带的代表相关逻辑屏幕的对象指针。至此，DirectDraw初始化工作完成。
3．2　使用DirectDraw
3．2．1　使用GDI函数向物理屏幕或逻辑屏幕输出
调用IDirectDrawSurface2::GetDC可得代表某个屏幕的设备描述表的句柄，使用GDI函数输出，最后调用IDirectDrawSurface2::ReleaseDC释放句柄。为防止在对屏幕作图期间其它应用程序争夺显存，IDirectDrawSurface2::GetDC调用IDirectDrawSurface2::Lock得到Winl6 Lock。这意味着其它程序在该程序释放Win16Lock前都不能访问GDI和USER资源。IDirectDrawSurface2::ReleaseDC调用IDirectDrawSurface2::Unlock释放Win16
 Lock。于是，在调用IDirectDrawSurface2::GetDC和IDirectDrawSurface2::ReleaseDC期间，Windows将被挂起。因此，应用程序应尽量缩短这一对函数调用之间的间隔时间，而且调试程序也无法跟踪这段时间内执行的操作。
3．2．2　交替切换物理屏幕和逻辑屏幕或执行屏幕内容的映射
准备好内存中的逻辑屏幕后，可以调用IDirectDrawSurface2::Flip方法切换物理屏幕和逻辑屏幕，也可调用IDirectDrawSurface2::BltFast、IDirectDrawSurface2::Blt等方法执行部分屏幕内容的映射。一般情况下程序采用异步方式，在显示卡硬件执行切换动作的同时准备下一页屏幕，使CPU和显示卡硬件并行，提高整体执行速度。
使用Flip切换物理屏幕和逻辑屏幕后，原指向物理屏幕的指针仍然指向物理屏幕，原指向逻辑屏幕的指针仍然指向逻辑屏幕，即指针所指内容也被交换了，便于程序操纵各个屏幕而不至于混淆。
3．2．3　释放DirectDraw对象
程序结束之前要释放所创建的DirectDraw对象。这只要在相应接口上调用Release方法即可。
4　使用DirectDraw的技巧和注意事项
4．1　检查方法的返回值
正确执行DirectDraw方法时都返回DDOK。且其值是零。返回其它值表明发生了某种错误。一般地，程序应检查这些返回值以决定是否出错。
4．2　检查Flip和Blt的状态
如果在Flip或Blt操作的返回值是DDERR＿WASSTILLDRAWING情况，为提高效率，DirectDraw提供了IDirectDrawSurface2::GetFlipStatus和IDirectDrawSurface2::GetBltStatus方法。它们能立即返回当前的Flip和Blt状态，于是应用程序可以在上一操作完成之前执行某些其他的任务。
4．3　在位图映射中使用ColorKey
ColorKey是一种或几种颜色的集合，用于在位图映射操作中区分前景色和背景色。ColorKey包括两种：Source color key和Destination color key。前者是指源位图中代表透明色的颜色，在执行映射操作时将不被映射到目标位图上；后者是指目标位图中将被源位图中相应位置颜色取代的颜色，如果目标位图指定了Destination color key，则只有这些指定的颜色被替换。可以在创建DirectDrawSurface对象时指定Color Key；也可使用方法IDirectDrawSurface2::GetColorKey和IDirectDrawSurface2::SetColorKey以获取和设置已有的DirectDrawSurface对象的ColorKey。
4．4　GDI重定向
由于GDI在Windows系统启动时先于DirectDraw被装入，而DirectDraw工作时又绕过了GDI，因此GDI不知道DirectDraw对物理屏幕所进行的操作。即使DirectDraw调用了Flip方法将先前的物理屏幕切换为逻辑屏幕，如不采取措施，GDI将仍然向切换后的逻辑屏幕上输出。如果DirectDraw程序拥有菜单、滚动条等由GDI负责绘制的元素，那么在Flip完成后这些元素就会成为不可见，而在对应的逻辑屏幕被切换成物理屏幕时又会显示出来。为避免屏幕闪烁，DirectDraw提供了IDirectDraw2::GetGDISurface，用于确定当前被GDI认为是物理屏幕的DirectDrawSurface对象；以及IDirectDraw2::FlipToGDISurface，用于将GDI的输出重新定向到当前的物理屏幕上。如果需要，可以在每次Flip操作后调用它，以保证屏幕正常。
4．5　在显存中存放位图
由于从显存到显存的映射比从系统内存到显存的映射快，所以经常将需要映射的位图存放在显存中以提高速度。大多数显示卡在存放了物理屏幕和相关逻辑屏幕之外还有足够的内存可以用来存放位图。可以调用IDirectDraw2::GetCaps检查显存；在创建DirectDrawSurface对象时可以通过结构DDSURFACEDESC中的DDSCAPS域指定该对象存在于显存或系统内存中。如果指定在显存中创建对象，而显存又没有地方容纳该对象，IDirectDraw2::CreateSurface会返回错误信息DDERR＿OUTOFVIDEOMEMORY。如果没有指定创建的位置，DirectDraw总是尽量利用空闲的显存。
4．6　检查硬件的性能
虽然DirectDraw通过HAL和HEL屏蔽了硬件的具体特性，但应用程序也需根据硬件的不同性能来改变自身的执行方式。利用IDirectDraw2::GetCaps方法可以得到有关硬件性能的详细信息。
4．7　保持主窗口的消息循环畅通
在调用SetCooperativeLevel设置DirectDraw的工作模式时，应用程序为DirectDraw指定了主窗口。由于DirectDraw直接操纵硬件可能导致死机，因此DirectDraw在后台监视主窗口的消息循环，当消息循环长时间没有反应时，DirectDraw就释放所有的资源，结束应用程序的执行。所以DirectDraw程s序应该注意避免长时间封锁消息循环。


