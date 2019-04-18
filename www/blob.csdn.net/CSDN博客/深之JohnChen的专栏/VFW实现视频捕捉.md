# VFW实现视频捕捉 - 深之JohnChen的专栏 - CSDN博客

2009年07月16日 10:32:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：7301



**一、Vedio for Windows 简介**
Vedio for Windows 是WIN32 SDK 中多媒体编程SDK 的视频开发工具。在微软的Visual C ＋＋中提供了Vedio for Windows 的头文件vfw.h 和库文件vfw32.lib。表1 列出了VFW 中视频捕捉编程常用的一些VFW 函数和宏。

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090716/VFW1.jpg)

[](http://img.blog.163.com/photo/_D4M3kJuHKP920E6UNT-bw==/4011299893104009504.jpg)

**二、用VC ＋＋6.0 实现视频捕捉的编程实例**　　下面结合实例，说明对视频捕捉编程的具体实现方法。
　　1、用Visual C ＋＋的AppWizard 生成一个基于对话框的应用程序，并在StdAfx.h 中加入以下内容：
　　　　　　　　＃include < vfw.h >
　　　　　　　　＃pragma comment(lib,"vfw32.lib")
　　2、修改对话框资源，添加静态控制IDC_VIDEO 和按钮“捕捉”、“停止”、“设置格式”、“设置图像源”、“设置压缩”、“单帧捕获”、“定帧”、“退出”。

（其实最重要的是“设置格式”、“设置图像源”、“设置压缩”、“单帧捕获”、“退出”，其它可以将其设置成不可见，同时也可将其保留，以便将来扩展其功能时可用。）
　　3、在对话框的头文件中加入以下说明。
　 　HWND m_hCapWnd;// 预示窗口
　　 BOOL m_bInit;// 捕捉器初始化
　 　CAPDRIVERCAPS m_CapDrvCap;// CAPDRIVERCAPS 结构，定义驱动器性能
　 　CAPSTATUS m_CapStatus;// CAPSTATUS 结构，定义捕捉窗口当前状态
　　 CAPTUREPARMS m_Parms;// CAPTURE PARMS 结构，定义捕捉参数
　 　CString m_CapFileName;// 捕捉文件名称
　　4、在对话框类的OnInitDialog() 函数中加入捕捉器初始化以及连接驱动器。
　      m_bInit=FALSE;
　      CWnd *pWnd=AfxGetMainWnd() －>GetDlgItem(IDC_VIDEO); // 得到预示窗口指针
　      CRect rect;
　      pWnd －>GetWindowRect( ＆rect); // 得到窗口大小
　      m_hCapWnd=capCreateCaptureWindow((LPTSTR)TEXT("视频捕捉测试程序"),WS_CHILD|WS_VISIBLE|WS_EX_CLIENTEDGE|WS_EX_DLGMODALFRAME,0,0,rect.Width(),rect.Width(),pWnd －>GetSafeHwnd(),0); // 设置预示窗口
　　　ASSERT(m_hCapWnd);
　　　　　　if(capDriverConnect(m_hCapWnd,0)){// 连接第0 号驱动器
　 　 　m_bInit=TRUE;
　　　　　　// 得到驱动器的性能
　　 　 　capDriverGetCaps(m_hCapWnd,sizeof(CAPDRIVE　　RCAPS), ＆m_CapDrvCap);
　 　 　if(m_CapDrvCap.fCaptureInitialized){ 
　　　　　　　　 // 如果初始化成功
capGetStatus(m_hCapWnd, ＆m_CapStatus,sizeof(m_CapStatus)); // 得到驱动器状态
capPreviewRate(m_hCapWnd,30); // 设置预示帧频
capPreview(m_hCapWnd,TRUE); // 设置预示方式
　}
　else{// 初始化未成功
　AfxMessageBox("视频捕捉卡初始化失败!");
　AfxGetMainWnd() －>PostMessage (WM_CLOSE);
　　　　}
　　　　　　}
else{// 未能连接到驱动器
　 　 　 　AfxMessageBox("与视频捕捉卡连接失败!");
　 　 　 　AfxGetMainWnd() －>PostMessage(WM_CLOSE);
　　　　　　}
m_CapFileName="c://Capture.avi";// 设置捕获文件
capFileSetCaptureFile(m_hCapWnd,m_CapFileName.GetBuffer(255));
　　5、在对话框类中加入响应“设置格式”消息的函数OnFormat()。
　　capDlgVideoFormat(m_hCapWnd);// 设置格式对话框
　　6、在对话框类中加入响应“设置图像源”消息的函数OnSource()。
　　capDlgVideoSource(m_hCapWnd);// 设置图像源对话框
　　7、在对话框类中加入响应“设置压缩”消息的函数OnCompress()。
　　capDlgVideoCompression(m_hCapWnd);// 设置压缩对话框
　　8、在对话框类中加入响应“捕捉”消息的函数OnCapture()。
　　capCaptureGetSetup(m_hCapWnd, &m_Parms,sizeof(m_Parms));// 得到设置参数
　　if(capCaptureSetSetup(m_hCapWnd, &m_Parms,sizeof(m_Parms))==TRUE){
　　　　　　　　　　　　BOOL suc=TRUE;
　　　　suc=capCaptureSequence(m_hCapWnd); // 捕捉到文件
　　　　　　　　　　　　return suc};
       else
　　　　return FALSE; 

       注意点：在VideoCaptureDlg.h中把afx_msg void OnCapture();改为afx_msg BOOL OnCapture();

                     在VideoCaptureDlg.cpp 修改为BOOL CVideoCaptureDlg::OnCapture() //捕捉
　　9、在对话框类中加入响应“定帧”消息的函数OnFreezed()。
　　capPreview(m_hCapWnd,FALSE);// 定帧
　　10、在对话框类中加入响应“单帧捕获”消息的函数OnImage()。
　　capGrabFrameNoStop(m_hCapWnd);// 截获当前图像
　　capEditCopy(m_hCapWnd);// 将图像拷贝到剪贴板
　　11、在对话框类中加入响应“停止”消息的函数OnStop()。
　　capCaptureStop(m_hCapWnd);// 停止捕捉
　　12、在对话框类中加入响应“退出”消息的函数OnExit()退出前断开捕捉器与驱动器的连接，并关闭窗口。
　　capDriverDisconnect(m_hCapWnd); 
　   CDialog::OnCancel();

        附加说明：以上添加的按钮用于捕捉图像（button）；

                         另外添加（Static Text）用于创建并设置捕获窗口；

        保存图像的方法：点击“单帧捕获”——>打开附近里的“画图”——>“编辑”中的“粘贴”——>即可保存捕捉的位图。

         以上方法主要是采用了VFW的函数和宏，有兴趣的朋友可以尝试用消息的方法，同样也可以实现。                     

 如：SendMessage (hWndC, WM_CAP_DRIVER_CONNECT, 0, 0L);//用于连接设备

要做的就是根据相关的功能改变相应的参数就可以了。

当然，在这过程中肯定会遇到很多问题的，我刚开始编译的时候出现了一百多个错误，耐心调通也就不难了。

下面的图是几幅界面图，可做参考。

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090716/VFW2.jpg)

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090716/VFW3.jpg)

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090716/VFW4.jpg)

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090716/VFW5.jpg)

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090716/VFW6.jpg)

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090716/VFW7.jpg)

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090716/VFW8.jpg)

