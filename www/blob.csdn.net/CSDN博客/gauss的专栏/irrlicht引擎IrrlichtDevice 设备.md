# irrlicht引擎IrrlichtDevice 设备 - gauss的专栏 - CSDN博客
2011年12月13日 01:07:17[gauss](https://me.csdn.net/mathlmx)阅读数：597标签：[引擎																[marshalling																[平台																[跨平台																[null																[windows](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=跨平台&t=blog)](https://so.csdn.net/so/search/s.do?q=平台&t=blog)](https://so.csdn.net/so/search/s.do?q=marshalling&t=blog)](https://so.csdn.net/so/search/s.do?q=引擎&t=blog)
个人分类：[客户端编程](https://blog.csdn.net/mathlmx/article/category/944323)
文章里面有一部分内容出自:http://blog.csdn.net/n5/article/category/217089/2
本篇讨论IrrlichtDevice，主要探讨以下几个问题：
->irrlicht对设备做了哪些抽象
->如何做到跨平台
->win32设备的实现细节
1）引擎的基石 - IrrlichtDevice
irrlicht 所指的设备是什么呢？探究IrrlichtDevice接口，irrlicht设备是对应用程序窗口环境的抽象，实际上对应了在各个平台上的应用程序框架。在IrrlichtDevice接口中，有run(),closeDevice(),yield(),sleep()这样的应用程序生命周期操作，也有setResizeAble(),setWindowCaption这样对窗口的操作，另外设备还持有了video driver,scene manager,gui enviroment, file system等对象，从接口看应该在实现的具体设备中完成这些对象的构造并管理这些对象。IrrlichtDevice可以算是引擎的基石，支撑起在目标平台上3D引擎的运行，如果我们设计的引擎想要跨平台则必须要做这样的抽象。
2）如何实现设备跨平台
![](http://hi.csdn.net/attachment/201112/13/0_1323791168dDMs.gif)
                                                                                                图一
看上面所有不同平台类继承公共基类CIrrDeviceStub，而CIrrDeviceStub继承接口IrrlichtDevice，接口里面主要是一些常用操作。
        virtual bool run() = 0;
        virtual void yield() = 0;
        virtual void sleep(u32 timeMs, bool pauseTimer=false) = 0
       //获取显卡绘制驱动实例指针
        virtual video::IVideoDriver* getVideoDriver() = 0;
      / /获取文件系统实例指针
        virtual io::IFileSystem* getFileSystem() = 0;
       //获取GUI系统管理类实例指针
        virtual gui::IGUIEnvironment* getGUIEnvironment() = 0;
      //获取默认场景管理类实例指针
        virtual scene::ISceneManager* getSceneManager() = 0;
        //获取光标实例指针
        virtual gui::ICursorControl* getCursorControl() = 0;
        //获取日志类实例指针
        virtual ILogger* getLogger() = 0;
       //获取显卡驱动模型实例指针
        virtual video::IVideoModeList* getVideoModeList() = 0;
       //获取操作系统信息
        virtual IOSOperator* getOSOperator() = 0;
        //获取时间系统实例指针
        virtual ITimer* getTimer() = 0;
      //设置事件监听类，即绘制窗体会把消息先传递给这个监听类    
     virtual void setEventReceiver(IEventReceiver* receiver) = 0;
从上面类图可以看出所有平台的设备类还继承了IImagePresenter抽象类，IImagePresenter里面只有一个接口
virtual bool present(video::IImage* surface, void* windowId=0, core::rect<s32>* src=0 ) = 0;
这个接口主要作用是把表面数据绘制到设备上面去，win32绘制到DC上面.
使用irrlicht引擎时，首先要调用的一个全局函数：createDevice()，他的实现在源文件Irrlicht.cpp中，实际上真正完成工作的是 createDeviceEx()，这个函数的声明和createDevice()一样都在irrlicht.h中，但实现在不同的平台的 CIrrDeviceXXX.cpp中。irrlicht通过编译选项保证只有一个平台的相关代码被编译，所以不会出现多次 createDeviceEx()的定义。在windows平台上，_IRR_USE_WINDOWS_DEVICE_被定义，而其他如
 _IRR_USE_LINUX_DEVICE_是没有定义的，所以设备相关的起作用的代码文件是CIrrDeviceWin32.h/.cpp。在 IrrCompileConfig.h中可以发现，如果定义了_IRR_USE_SDL_DEVICE_，就不会定义 _IRR_USE_WINDOWS_DEVICE_，此时将是SDL设备起作用。总之，只有一个平台会被定义，只有一种设备会被创建，只有一份 createDeviceEx()存在。通过编译选项和宏定义，编译器只会选择编译某一平台的代码，构建某一平台的irr设备。
3）Win32设备源码剖析
上面说了，在win32平台上，只有CIrrDeviceWin32.h/.cpp会被编译，查看源代码，首先发现：
class CIrrDeviceWin32 : public CIrrDeviceStub
查词典，“计算机编程中，RMI中将客户辅助对象称之为Stub”，CIrrDeviceStub是个所有设备类型都能用上的公共功能的集合，是个辅助的基类。那么先看下这个类，源码 CIrrDeviceStub.h/.cpp。这个类不会创建一个具体的IrrDevice（很显然，因为他是各平台共享的辅助类，当然不会关心某个具体的设备），没有惊讶，这个类持有并管理了一些IrrDevice接口上可以看出的对象，如FileSystem，当然FileSystem本身就是跨平台的实现，这是一种形式，另一种形式是stub提供了创建函数，但自己并没有调用它，具体的就是createGUIAndScene()，会创建
 GUIEnvironment和SceneManager，这个函数会在子类中调用。另外stub还实现了getXXX()系列的接口。
stub只是尽量提取了一些各平台共有的操作，真正的东西还是需要各自平台自己实现。
下面分析win32设备。
<1>设备创建
首先寻找createDeviceEx()，其中只是new了一个CIrrDeviceWin32对象，并检查是否成功，主要的创建还是在 CIrrDeviceWin32的构造函数中。构造函数中主要的工作是创建窗口(也可以使用外部传入的窗口HWND)，创建driver,创建GUI和 SceneMgr(调用stub中的createGUIAndScene())。这里的重点是创建driver，irrlicht可以同时支持多种 driver并在运行前选择，也就是说这些driver都是编译好的（而不是像device那样只编译了一个），在createDriver()中，会根据CreateParams决定创建哪种driver，其中创建硬件driver,如DX,OpenGL是在平台的代码中的，而创建软件driver和
 NULL driver是在stub中。在Linux版本的device中我们发现只有OpenGL这种硬件driver，当然了，Linux并不支持DX。
<2>运行和关闭设备
从最简短的例子可以看到，通过判断设备是否run()而进行游戏的主循环。win32版的run()如下：
view plaincopy to clipboardprint?
bool CIrrDeviceWin32::run()   
{   
    os::Timer::tick();   
    MSG msg;   
    bool quit = false;   
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))   
    {   
        TranslateMessage(&msg);   
        if (ExternalWindow && msg.hwnd == HWnd)   
            WndProc(HWnd, msg.message, msg.wParam, msg.lParam);   
        else  
            DispatchMessage(&msg);   
        if (msg.message == WM_QUIT)   
            quit = true;   
    }   
    if (!quit)   
        resizeIfNecessary();   
    if(!quit)   
        pollJoysticks();   
    _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;   
    return !quit;   
}  
bool CIrrDeviceWin32::run()
{
    os::Timer::tick();
    MSG msg;
    bool quit = false;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        if (ExternalWindow && msg.hwnd == HWnd)
            WndProc(HWnd, msg.message, msg.wParam, msg.lParam);
        else
            DispatchMessage(&msg);
        if (msg.message == WM_QUIT)
            quit = true;
    }
    if (!quit)
        resizeIfNecessary();
    if(!quit)
        pollJoysticks();
    _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
    return !quit;
} 
os::Timer::tick() 用来记录系统时间，这是全局的，irrlicht引擎是基于时间而不是基于固定帧的，后面会有一篇专门分析这个问题。再下面是一个常用的 PeekMessage式windows消息循环。重点是返回值，只有窗口收到WM_QUIT消息，run()才返回false,而窗口的quit既可以是直接关闭窗口，也可以是游戏调用device的closeDevice():
view plaincopy to clipboardprint?
void CIrrDeviceWin32::closeDevice()   
{   
    MSG msg;   
    PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);   
    PostQuitMessage(0);   
    PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);   
    DestroyWindow(HWnd);   
}  
void CIrrDeviceWin32::closeDevice()
{
    MSG msg;
    PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);
    PostQuitMessage(0);
    PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);
    DestroyWindow(HWnd);
} 
<3>其他接口
win32 device中内含的窗口消息处理函数WndProc中，会处理如键盘鼠标这样的事件，并转换成irrlicht的消息。device还实现了 IImagePresenter接口，这个接口只有一个方法present，只有软件driver会使用它，将buffer绘制到屏幕上。另外还有 sleep(),yield()这样的线程控制方法。
