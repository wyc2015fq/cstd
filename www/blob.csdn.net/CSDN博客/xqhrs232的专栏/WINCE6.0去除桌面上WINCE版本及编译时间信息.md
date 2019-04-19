# WINCE6.0去除桌面上WINCE版本及编译时间信息 - xqhrs232的专栏 - CSDN博客
2014年03月20日 11:16:54[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：838
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://cky0612.blog.163.com/blog/static/27478916201282371215341/](http://cky0612.blog.163.com/blog/static/27478916201282371215341/)
相关文章
1、[WINCE--串口打印信息的屏蔽与打开](http://www.cnblogs.com/urbetter/archive/2010/05/17/1737770.html)----[http://www.cnblogs.com/urbetter/archive/2010/05/17/1737770.html](http://www.cnblogs.com/urbetter/archive/2010/05/17/1737770.html)
2、WinCE6.0 build options----[http://blog.sina.com.cn/s/blog_539238aa0100p8rp.html](http://blog.sina.com.cn/s/blog_539238aa0100p8rp.html)
3、wince6 串口调试信息输出功能取消 ----[http://bbs.csdn.net/topics/350207043](http://bbs.csdn.net/topics/350207043)

WINCE在进入桌面后，我们的系统可以看到“Windows Embedded CE v6.00 (Build 3122 on
 Sept 22 2012 )”这些WINCE版本及编译时间的信息，怎么去掉这些信息呢？我们先来看这部分信息是哪部分代码输出的，在WINCE600\PUBLIC\SHELL\OAK\HPC\CESHELL\UI\ desktopview.cpp文件的PaintBackground函数中有源码：
#ifndef SHIP_BUILD
   WCHAR wszVersion[256];
   OSVERSIONINFO VerInfo;
   VerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
   ::GetVersionEx(&VerInfo);
   ::wsprintf(wszVersion, L"Windows Embedded CE v%d.%02d (Build %d on %s) ",
              VerInfo.dwMajorVersion, VerInfo.dwMinorVersion, VerInfo.dwBuildNumber, TEXT(__DATE__));
   if (m_hbmpDesktop && !fTileBackgroundImage)
   {
     rc.top = rcBlt.bottom + 5;
   }
   else
   {
     rc.top = rc.top + (rc.bottom - rc.top) / 2 - 8;
   }
   ::DrawText(hdc, wszVersion, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_TOP);
   #endif
上面的代码就是如果没有定义SHIP_BUILD这个环境变量，就不会编译这部分代码，自然就不会再桌面上显示这些信息了，下面总结如何去除这些信息的办法：
1.       定义环境变量SHIP_BUILD
定义环境变量SHIP_BUILD，需要在WINCE6.0系统工程“property page -> build options”下面对“Enble build ship”打钩，如下图所示：
![WINCE6.0去除桌面上WINCE版本及编译时间信息 - 男儿当自强 - 男儿当自强的博客](http://img2.ph.126.net/CBjdbXEjGlph_VXCeJ_tLw==/6597143433936610332.gif)
图1
这样就定义了环境变量SHIP_BUILD，从而就不编译上面的代码，也不会显示这些信息了。另外我注意到选择“Enble build ship”前后编译nk.bin的大小分别为22848KB和22665KB，少了183KB，这是因为选上“Enble build ship”后会把RETAILMSG和DEBUGMSG都给mask掉了，这样自然生成的系统会少一些，不便之处就是系统不能输出调试信息。
2.       改变编译上面代码的条件
因为我们需要系统可以输出调试信息，所以第1种办法就不太合适(不过倒是适合于编译发布的系统)，我把上面代码的编译条件由#ifndef SHIP_BUILD改为#ifdef SHIP_BUILD，然后到\WINCE600\PUBLIC\SHELL\OAK\HPC\CESHELL\UI目录下rebuild，然后选择clean sysgen，这样就可以保证串口调试信息的输出，也可以不再显示上面的信息。
3.       屏蔽掉上面的代码
当然，如果我们不需要在桌面上显示那些信息，可以把上面的代码注释掉。
//=======================================================================================================================
备注：
1》我这样试了怎么老是不行呢？打印消息还是存在，不知道为什么？那个地方没设置好，我甚至把OS重新编译了一遍但还是不行！！！----后来应该是可以了，后来不把OEMWriteDebugByte++++OEMReadDebugByte这2个函数置空BSP层也打印不了消息，但APP/DLL层用RETAILMSG还是可以打印出消息！！！
2》其实很多时候就是在oal/debug.c文件里面把OEMWriteDebugByte++++OEMReadDebugByte这2个函数置空就可以了，但这样的话RETAILMSG还是会去执行啊！
这样应该不是最好的方案！
3》WINCE5下选择了Enable ship build应该再选择clean before buiding ,在这个前提下再执行build and sysgen才可以把BSP包中的打印消息完全去掉！！！
4》改变了Enable ship build设置，最好在clean before buiding选中的情况下，执行build and sysgen和build and sysgen current
 bsp这2个编译选项！！！
5》clean before buiding选项的作用----因为OS都是一个个LIB/DLL组成的，可能原来的LIB/DLL是Enable
 ship build使能跟禁止的情况下编译的，所以要clean before buiding
选中，对所有的LIB/DLL在新的Enable
 ship build条件下进行来一遍编译！！！

