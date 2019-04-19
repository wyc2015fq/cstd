# 使用CETK测试驱动程序 . - xqhrs232的专栏 - CSDN博客
2013年06月25日 14:28:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：725
原文地址::[http://blog.csdn.net/ok138ok/article/details/4354943](http://blog.csdn.net/ok138ok/article/details/4354943)
相关网帖
1、[CETK环境搭建及说明](http://blog.csdn.net/nanjianhui/article/details/2192161)----[http://blog.csdn.net/nanjianhui/article/details/2192161](http://blog.csdn.net/nanjianhui/article/details/2192161)
2、wince6下cetk下摄像头编译过程----[http://hi.baidu.com/cahbb/item/d1413b9cd4f9b0db1f4271af](http://hi.baidu.com/cahbb/item/d1413b9cd4f9b0db1f4271af)
3、[CETK的使用（USB连接方式）](http://www.cnblogs.com/wogoyixikexie/archive/2009/05/15/1457234.html)----[http://www.cnblogs.com/wogoyixikexie/archive/2009/05/15/1457234.html](http://www.cnblogs.com/wogoyixikexie/archive/2009/05/15/1457234.html)
**使用CETK测试驱动程序**
--作者：赖玉平(Auly)   aulyp@163.com
CETK是微软提供的用于测试基于其WINDOWS
 CE平台开发的驱动程序的测试工具，对于开发稳定可靠的驱动程序很有帮助，所以现在很多大公司在招这方面的人才时，都有要求会用CETK进行测试，协助开发。
  1 CE5.0平台
第一步，在系统中加入CETK组件，编译出NK
  Catalog—>Device Drivers—>USB Host –>Windows CE Test Kit
![](https://p-blog.csdn.net/images/p_blog_csdn_net/ok138ok/EntryImages/20090716/001.jpg)
第二步，把clientside.exe拷到WINCE平台上，放在根目录中。
  Clientside.exe在WINCE500/OTHERS/WCETK/目录下可以找到，相应的CPU下面，ARM
的平台，使用ARMV4I里面的程序。（其实如果用ActiveSync通过USB连接，此步可省）
第三步，用ActiveSync通过USB与目标平台同步，
第四步，在开始菜单，找到并打开Windows CE Test Kit，如下界面
![](https://p-blog.csdn.net/images/p_blog_csdn_net/ok138ok/EntryImages/20090716/002.jpg)
 图1 
![](https://p-blog.csdn.net/images/p_blog_csdn_net/ok138ok/EntryImages/20090716/003.jpg)
图2 
![](https://p-blog.csdn.net/images/p_blog_csdn_net/ok138ok/EntryImages/20090716/004.jpg)
图3
![](https://p-blog.csdn.net/images/p_blog_csdn_net/ok138ok/EntryImages/20090716/005.jpg)
设置完成，就可以点“Device Connection”界面的“Connect…”进行联接了
![](https://p-blog.csdn.net/images/p_blog_csdn_net/ok138ok/EntryImages/20090716/006.jpg)
当然这样使用的是默认的测试case，然后你想选择某个case或者某些case，你可以选"Edit
 Command Line..."，会弹出编辑对话框，然后编辑你要的case。举个例子：
比如原来默认的命令是：tux
 -o -d fsdtst -x1000-1080,5000-5050，这样要测试1000--1080和5000--5050这些case。
可以改成：tux
 -o -d fsdtst -x5050,5070，表示只测试5050和5070这两个case。
查看测试结果：
1)在测试完成以后，可以右击你测试的Item，然后选择"View
 Results"来查看测试结果。如果某个case没有通过，可以先看
 MSDN中关于这个case的介绍，了解他是测什么的。微软提供了CETK测试的源代码，可以在"/WINCE600/PRIVATE/TEST/"下找到，根据测试结果中打印的提示信息，你就可以很快找到CETK的相关源代码，然后就是读源代码，分析问题了。
2)还有一种情况是，"
 View Results "为灰暗，无法选择，这时如果要看测试结果，可以通过如下方法：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/ok138ok/EntryImages/20090716/007.jpg)
2  CE 6.0平台是一样的操作
**分析测试结果：**
1打开帮助文档，找到相关的驱动测试项，知道出错的测试ID的测试大概内容
例如：测试display驱动的Graphics Device Interface Test项ID218没通过
可以在帮助文档找到“**Test Cases for the Graphics Device Interface Test”**
里面有针对该测试的ID号解释，200至231
号测试ID
的描述
|Test case ID|Description|
|----|----|
|200-231|Draw: Calls functions that draw and functions that apply complex effects to drawing.These test cases perform blitting, line drawing, filling, color table manipulation, bitmap type creation, and device attribute modification.|
2打开"/WINCE600/PRIVATE/TEST/"，（建议在source
 insight建一工程来专门管理），找到出错的测试ID的测试内容，可能通过搜索打印消息来快速定位代码，测试不通过的具体原因。
例如，Graphics Device Interface Test测试出错的是ID－
218 打印如下：
StretchBlt - StretchBlt2 Test
   FAIL: Driver verification mismatch detected.
   Comparison exceeded threshold, incorrect pixel at (19, 11) expected (0x00195631) found (0x001A5732)
   Comparison exceeded threshold, incorrect pixel at (24, 11) expected (0x0036B869) found (0x0037B96A)
  ………..
 通过搜索“StretchBlt2 Test”，追踪代码到test/gwes/gdi/gdiapi  Draw.cpp文件，
StretchBltTest2()函数。这里有具体的测试操作，可以找出哪一个操作没有通过。
