# 教程：CXIMAGE 移植 CE 平台 - xqhrs232的专栏 - CSDN博客
2010年03月02日 17:06:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2235标签：[平台																[documentation																[microsoft																[wince																[配置管理																[7zip](https://so.csdn.net/so/search/s.do?q=7zip&t=blog)](https://so.csdn.net/so/search/s.do?q=配置管理&t=blog)](https://so.csdn.net/so/search/s.do?q=wince&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=documentation&t=blog)](https://so.csdn.net/so/search/s.do?q=平台&t=blog)
个人分类：[DirectUI界面技术](https://blog.csdn.net/xqhrs232/article/category/1243806)
原文地址：：[http://www.armce.com/bbs/redirect.php?tid=794&goto=lastpost](http://www.armce.com/bbs/redirect.php?tid=794&goto=lastpost)
相关网帖
1.請問WINCE下使用CXIMAGE的問題----[http://www.programmer-club.com.tw/ShowSameTitleN/opengl/2670.html](http://www.programmer-club.com.tw/ShowSameTitleN/opengl/2670.html)
2.CxImage 移植 到 wince 问题----[http://topic.csdn.net/u/20081009/18/69a67ad7-f361-4c02-956b-c5dc253d97a0.html](http://topic.csdn.net/u/20081009/18/69a67ad7-f361-4c02-956b-c5dc253d97a0.html)
3.[教程：CXIMAGE 移植 CE 平台](http://www.armce.com/bbs/thread-794-1-1.html)----[http://www.armce.com/bbs/thread-794-1-1.html](http://www.armce.com/bbs/thread-794-1-1.html)
4.[两天来移植CxImage终结](http://blog.csdn.net/mfc5158/article/details/6885673)----[http://blog.csdn.net/mfc5158/article/details/6885673](http://blog.csdn.net/mfc5158/article/details/6885673)
1、下载 CXIMAGE
[http://www.xdp.it/download.htm](http://www.xdp.it/download.htm)
选择此项下载，内容为 PPC2003 平台
**cximage600_ce** : full source code with documentation and small demo for PocketPC2003 (Microsoft Visual Studio 2005 solution)
1.37 Mb 
2、解压文件
这个不用细说了 ，找 7Zip 软件解压
在
[http://www.7-zip.org/](http://www.7-zip.org/)
此处下载。
3、打开项目 CxImgLib_wince.sln
找到 解决方案管理器 -> 在项目上右键 -> 配置管理器（如图）
4、活动解决方案平台 -> 下拉框选择新建 -> 选择你的平台 ->选择从PPC2003这里复制 ->点击确定
5、CXImageCrtDLL 和 CXImageMFCDLL  属性页-> 配置属性 -> 连接器 -> 输入
在附加倚赖项中 ccrtrtti.lib  secchk.lib这两项去掉 
6、CXImageCrtDLL 和 CXImageMFCDLL （demo_ce可选） 属性页-> 配置属性 -> 连接器 -> 命令行
在附加选项
/MACHINE:ARM 去掉
（demo_ce可选）
/subsystem:windowsce,4.20 /machine:ARM /ARMPADCODE
改为 /subsystem:windowsce 或者 /subsystem:$(CESubsystem) 
7、 基本告一段落
如果你的SDK带有些头文件
CXImage/wcecompat/include
目录下有重复定义的头文件更改后缀名
如目录下的 xxxxx.h_old
避免从复定义
还有什么问题，可以单独提问！
//========================================================================
备注::  一些编译技巧总结
1》可以一个个工程先分别编译，采用各个击破的方针
2》把所有工程的警告关闭----要不然警告太多，你不知道那个工程编译出问题了，那个没有问题
3》把cximage600_ce\wcecompat\include目录下跟你自己平台下有相同名称的头文件屏蔽掉----修改为NOT
  比如assert.h  ctype.h  float.h limits.h malloc.h  memory.h setjmp.h stdarg.h stddef.h stdio.h  stdlib.h  string.h  time.h（对应WINCE60平台，这些头文件都已经存在了）
4》demo_ce工程的链接器配置/subsystem:windowsce,4.20 /machine:ARM /ARMPADCODE
  应该修改为/subsystem:windowsce /machine:THUMB /ARMPADCODE才可以编译通过
5》cximage++++CxImageCrtDll++++cximagemfcdll这三个工程的区别与联系！！！
6》按上面的设置基本就可以编译通过了，但不排除还有其它BUG
    A。官网下载的DEBUG/RELEASE两个版本都编译不了，可能跟我的加密电脑有关
        1。DEBUG----cannot open ximadisp.obj
        2.。RELEASE----cannot open sys/types.h(tiff project  can compile successfully by oneself)
    B。从CSDN下载的RELEASE可以编译通过，DEBUG编译不了，打不打RSPXXX.rsp文件
7》下一步就是怎么使用了，还得找点资料看看！！！
