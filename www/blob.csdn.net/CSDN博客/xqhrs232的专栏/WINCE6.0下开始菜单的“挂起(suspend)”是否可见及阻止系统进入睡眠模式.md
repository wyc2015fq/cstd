# WINCE6.0下开始菜单的“挂起(suspend)”是否可见及阻止系统进入睡眠模式 - xqhrs232的专栏 - CSDN博客
2014年11月03日 17:58:19[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1315
原文地址::[http://blog.csdn.net/loongembedded/article/details/6715980](http://blog.csdn.net/loongembedded/article/details/6715980)
相关网帖
1、[解决WinCE自动挂起的问题](http://blog.csdn.net/loongembedded/article/details/8645886)----[http://blog.csdn.net/loongembedded/article/details/8645886](http://blog.csdn.net/loongembedded/article/details/8645886)
2、如何禁止WinCE设备休眠?----[http://bbs.csdn.net/topics/360205063](http://bbs.csdn.net/topics/360205063)
3、请问在Wince下如何实现系统休眠与唤醒？----[http://bbs.csdn.net/topics/250001995](http://bbs.csdn.net/topics/250001995)
4、wince挂起和唤醒_relayon 
 ----[http://jimmyloveforever.blog.163.com/blog/static/11977524720095130395084/](http://jimmyloveforever.blog.163.com/blog/static/11977524720095130395084/)
5、wince如何取消自动挂起----[http://bbs.csdn.net/topics/340126772](http://bbs.csdn.net/topics/340126772)
6、请教下关于 WINCE的电源TIMEOUT问题。（重启后不关背光设置无效）----[http://bbs.csdn.net/topics/340110847](http://bbs.csdn.net/topics/340110847)
 ********************************LoongEmbedded********************************
作者：LoongEmbedded(kandi)
时间：2011.8.24
类别：WINCE 系统开发
********************************LoongEmbedded********************************
虽然在我们的WINCE6.0+S3C6410的产品上可以实现睡眠和唤醒了，但在唤醒的时候网口不能正常工作，而还有更重要的问题有待去结局，并且睡眠和唤醒功能并不是必需的，所以自然而然要去掉开始菜单中的“挂起”，而且还要保证系统不会在设备没有工作一段时间内进入睡眠状态，所以需要做下面的处理：
1.在注册表文件中添加suspend项来控制是否需要在开始菜单中显示“suspend”
有两种办法可以实现对开始菜单中是否显示“suspend”进行控制
1)在我们WINCE工程编译的release目录的shell.reg的HKEY_LOCAL_MACHINE\Explorer添加suspend子键来控制
[HKEY_LOCAL_MACHINE\Explorer]
"RecycleBinSize"=dword:A
"ShowExt"=dword:0
"ShowSys"=dword:0
"UseCompatibleBGImage"=dword:1
"UseRecycleBin"=dword:1
"ViewAll"=dword:0
添加
"Suspend"=dword:0
这样就可以去掉去掉开始菜单的“挂起(suspend)”功能，但如何需要恢复这个功能，只需要把“Suspend”对应的值改为1就可以了，很方便吧。
这种办法并不是很方便，因为只要我们重新编译整个BSP包或者编译整个工程的，release目录中shell.reg的内容都会被覆盖为原来的。
2) 在BSP\FILES\platform.reg中加入下面的内容也可以实现同样的控制
[HKEY_LOCAL_MACHINE\Explorer]
"Suspend"=dword:0
这样就不会出现上面会被覆盖的问题。
2.阻止系统在空闲一段时间内自动进入睡眠模式
把release下的common.reg中的下面内容
[HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Power\Timeouts]
"BattSuspend"=dword:12c             ; in seconds
改为
"BattSuspend"=dword:0            ; in seconds
这种方式也会有上面提到的被覆盖的情况，可以我像2)提到的方式在platform.reg中做同样的修改却无效，目前还不知道原因，后面找到原因会补上，在这里还望知道的朋友告知，谢谢。
