# Windows CE下流驱动的动态加载 - xqhrs232的专栏 - CSDN博客
2013年08月19日 22:20:27[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：747
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://www.cnblogs.com/we-hjb/articles/1250838.html](http://www.cnblogs.com/we-hjb/articles/1250838.html)
相关网帖
1、[WinCE驱动的动态加载](http://blog.csdn.net/norains/article/details/5316923)----[http://blog.csdn.net/norains/article/details/5316923](http://blog.csdn.net/norains/article/details/5316923)
2、[WinCE驱动的动态卸载](http://blog.csdn.net/norains/article/details/5514351)----[http://blog.csdn.net/norains/article/details/5514351](http://blog.csdn.net/norains/article/details/5514351)
我想很多WinCE的开发人员，尤其是刚入门并且做驱动开发的工程师，都曾碰到这样一个问题，要编写一个外围设备的驱动，拿最简单的GPIO驱动来说，编写驱动本身可能只花了一会儿功夫，可要把编译生成的DLL打包到先前做好的操作系统映像当中，最简单也得MakeImg一下，还要修改BIB文件、注册表文件，以让系统启动的时候就加载该驱动，所有工作都做完了，还得花几分钟下载整个操作系统到内存去运行，这也得要个好几分钟。能力强的人一次成功，不走回头路也就算了。如果驱动编写得有问题，那又得改代码，重新编译，把刚刚所做的事情再做一遍。说出来不怕大家笑话，我刚开始做驱动时就这样，反反复复，半天下来，才调试好一个简单的驱动。而事实上很大一部分时间都浪费在重复操作上。这种调试驱动的方法实在效率太低了。想到Linux下面的驱动调试，多方便！直接INSMOD一下，应用程序就可以调用，出现问题就RMMOD，根本无须来回倒腾操作系统的映像文件。那么，WinCE下难道就没有这么简便的方法嘛？答案是肯定的。
       闲话少说，进入正题。查找EVC的帮助，发现函数ActivateDevice（）可用来加载驱动程序。而这个函数的使用是相当简单的。我就不多说了，贴上一段帮助最能说明问题。当然，你也可以用ActivateDeviceEx（）。
**HANDLE ActivateDevice (**
**LPCWSTR ***lpszDevKey***,**
**DWORD ***dwClientInfo*
**);**
**Parameters**
*lpszDevKey*
[in] Pointer to the registry path string of the device's driver key under **HKEY_LOCAL_MACHINE**. A driver key contains the DLL name, the device prefix, friendly name, and other device information.
*dwClientInfo*
[in] Data that will be stored in the device's **Active** key in the **ClientInfo** value. The registry path to the driver's**Active** key is passed in as the *context* parameter to the device's [XXX_Init](http://www.cnblogs.com/we-hjb/admin/javascript:hhobj_1.Click()) function.
 The value in *dwClientInfo* is stored in the registry before **XXX_Init** is called.
**Return Values**
On success, ActivateDevice returns a handle to the device that can be used in subsequent calls to [DeactivateDevice](http://www.cnblogs.com/we-hjb/admin/mk:@MSITStore:C:/Windows%20CE%20Tools/eVC4/Htmlhelp/emtools/wcesdk40.chm::/htm/cerefdeactivatedevice.htm).
**Remarks**
This function is superseded by [ActivateDeviceEx](http://www.cnblogs.com/we-hjb/admin/mk:@MSITStore:C:/Windows%20CE%20Tools/eVC4/Htmlhelp/emtools/wcesdk40.chm::/htm/cerefactivatedeviceex.htm).
ActivateDevice loads a device driver. ActivateDevice reads the registry key specified in lpszDevKey to get the DLL name, device prefix, index, and other values. Next it adds the device to the active device list in the registry branch**HKEY_LOCAL_MACHINE\Drivers\Active**,
 and stores the relevant values there. If no device index was specified in the key named in *lpszDevKey*, then it assigns a free index. Then it loads the device driver DLL in the process space of the Device Manager. Then it broadcasts a WM_DEVICECHANGE
 message for the new device and triggers a NOTIFICATION_EVENT_DEVICE_CHANGE event in the application notification system to notify applications of the presence of the new device.
从上面的描述中可以看到，在使用该函数时，我们只要给出第一个参数就可以，而这个参数是注册表中的一个路径字符串。所以，要想很方便的动态加载任意一个驱动，我们还要了解一下有关注册表的内容。但其中最核心的就是一条，把你驱动的相关注册表信息放到HKEY_LOCAL_MACHINE下，主要内容包括Prefix、DLL、Index、Order等信息。这里就不展开说明了。
       自己起初费了那么多冤枉时间，实在很郁闷。原理摸清后就做了一个小工具，实现动态加载流驱动，以提高开发效率，也能方便后来人。下面就详细介绍这个小工具的使用过程，让大家体验一下动态加载流驱动是多么畅快的一件事情。
       我们假设已经做好一个简单的流驱动DrvDemo.dll，其对应的注册表文件为DrvDemo.reg，用来测试驱动的应用程序DrvDemo.exe。我们把这三个文件和驱动调试助手(DM.exe)都拷贝到WinCE系统上，如下图所示。
![](http://images.cnblogs.com/cnblogs_com/we-hjb/image001.PNG)
     首先运行DrvDemo.exe，点击打开设备，或者其他按钮，我们发现打开设备失败，很正常，此时DrvDemo.dll还没有工作呢。
![](http://images.cnblogs.com/cnblogs_com/we-hjb/image002.PNG)
     运行驱动调试助手软件DM.exe，点击“驱动调试”菜单——选择“导入注册表”，浏览到我们准备好的注册表文件DrvDemo.reg，选中并点击OK按钮，这样，DrvDemo.reg中的内容就已经导入到系统中了，相应的信息能在“操作信息回显”中看到。
![](http://images.cnblogs.com/cnblogs_com/we-hjb/image003.PNG)
     此时你可以直接点击“驱动调试”菜单下的“激活驱动”，当然你也可以打开“驱动调试”——“驱动列表”，进而选择激活该驱动。正常情况下，操作回显中就会提示你“加载驱动成功”。
![](http://images.cnblogs.com/cnblogs_com/we-hjb/image004.PNG)
     这时，再到DrvDemo.exe点击打开设备，就完全没有问题了。
![](http://images.cnblogs.com/cnblogs_com/we-hjb/image005.PNG)
如果在测试时，发现驱动有问题，那么你可以选择“卸载驱动”，然后将修改好的新的驱动拷贝过来，再点击“激活驱动”菜单。如此往复，直至调试成功。
至此，我们采用了通过导入注册表文件的方法激活驱动。下面再说说在没有注册表文件时直接通过浏览DLL文件本身来加载驱动的方法。先把刚刚加载的驱动卸载了。
运行驱动调试助手软件，点击“驱动调试”菜单——选择“浏览DLL文件”，浏览到我们准备好的DrvDemo.dll文件。在弹出的对话框中设置正确的Prefix、Order、Index信息，点击“确定”。
![](http://images.cnblogs.com/cnblogs_com/we-hjb/image006.PNG)
同样，此时你可以直接通过“驱动调试”下的“激活驱动”直接激活该驱动，或者从“驱动列表”中激活.
其余的操作跟上面讲的完全一样了，不再重复。
![](http://images.cnblogs.com/cnblogs_com/we-hjb/image007.PNG)
     怎么样，这种方法就很类似于Linux下的Insmod和Rmmod了吧？^_^
     其实，如果在注册表中已经有了相应的键，你甚至可以直接选中它，然后在“驱动调试”中选择“激活驱动”，如果一切正常，也可以达到上面两种方法的效果，但这样就连导入文件都省了。有兴趣的同志可以自己试一下。
好了，我相信到这里，大家应该对动态加载流驱动的原理和方法都了解了，也能从此摆脱不断MakeImg和Download的梦魇，节省很多宝贵的开发时间。
    最后，针对驱动调试助手简单说明一下几个要注意的地方。
1.       在调试驱动时，加载完驱动后，千万不要关闭驱动调试助手，最小化就行，否则，它会在关闭时自动卸载掉你所加载的驱动。
2.       在WinCE4.2中，有关中断的驱动是无法直接用此方法来动态加载的，而在5.0和6.0中不会有此限制。这是因为WinCE4.2中的中断处理机制所致。
3.       驱动调试助手可以同时加载几个驱动，这时候建议将所有注册表信息写到一个文件中，并导入。剩下的工作就是从驱动列表中选择加载你所需要的驱动了。
4.       驱动调试助手中的注册表编辑功能经过几番修改，已基本完善，编辑、导入、导出都没有问题。如果系统支持HIVE-REG，在退出驱动调试助手时，可以选择保存此次的注册表修改。
5.      上面的截图是在WinCE6.0的模拟器上面做的，应该算是内核态的驱动，至于用户态的驱动也可以通过该方法加载，但是需要设定相应的注册表信息。
6.       做这样一个小工具，主要是想提高自己的工作效率。现在贴出来，希望也能为同道中人所用。在使用该程序时有任何问题，请发邮件到 [wince.he@gmail.com](mailto:wince.he@gmail.com)，我会尽力解决。本篇中的视频录像和最新版的驱动调试助手可以在[http://files.cnblogs.com/we-hjb/WINCEDM.rar](http://files.cnblogs.com/we-hjb/WINCEDM.rar)下载到。
上面所讲的并不是新鲜或者高深的技术，但相信对某些技术人员还是有一定的帮助。当然，文中有不尽如人意处，还请你见谅。以后争取多写一点东西，跟大家一起学习Windows Embedded。
最后，希望不要被认为是“广告性质的文章”。^_^
