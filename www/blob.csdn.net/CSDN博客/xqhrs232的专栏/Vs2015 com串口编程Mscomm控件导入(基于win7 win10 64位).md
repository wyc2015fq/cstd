# Vs2015 com串口编程Mscomm控件导入(基于win7/win10 64位) - xqhrs232的专栏 - CSDN博客
2018年09月12日 10:54:31[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：199
原文地址::[https://blog.csdn.net/youxishaonian/article/details/52590316](https://blog.csdn.net/youxishaonian/article/details/52590316)
相关文章
1、VC2012/VS2012 MFC串口通讯上位机程序教程笔记----[https://blog.csdn.net/u014488388/article/details/50937114](https://blog.csdn.net/u014488388/article/details/50937114)
2、[VS2010下MFC的串口编程](https://www.cnblogs.com/gaohongchen01/p/4432922.html)----[https://www.cnblogs.com/gaohongchen01/p/4432922.html](https://www.cnblogs.com/gaohongchen01/p/4432922.html)
重新学习编程，由于近期需要和同事做一个基于com口设备通讯的项目，因上放弃了VC6.0换成了VS2015。
但发现VS2015已经没有以前的Mscomm控件（系统原因），由于发现需要在VS2015里面重新导入Mscomm组件。
首先需要下载相关组件：
http://download.csdn.net/download/rabbit200808/3236756
里面包含四个文件
### MSCOMM.SRGMSCOMM32.DEPMSCOMM32.ocaMscomm32.ocx
下载后解压到c:\windows\system32 目录下
同时需要把Mscomm32.ocx复制到c:\windows\syswow64 目录下（win7 64bit）
之后以管理员身份运行cmd.exe, 在c:\windows\system32输入以下指令
Regsvr32  C:\WINDOWS\system32\MSCOMM32.OCX 
成功后如下图所示：
![](https://img-blog.csdn.net/20160919232624231?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
接着在输入regedit打开注册表，找到HKEY_CLASSES_ROOT\Licenses\4250E830-6AC2-11cf-8ADB-00AA00C00905确认键值为“kjljvjjjoquqmjjjvpqqkqmqykypoqjquoun”即OK
（正常导入后可以查到以上信息，若没有则找到HKEY_CLASSES_ROOT\Licenses添加主项命名为“4250E830-6AC2-11cf-8ADB-00AA00C00905”，并将键值修改为“kjljvjjjoquqmjjjvpqqkqmqykypoqjquoun”，输入时无视双引号）
完成以下工作后， 打开vs2015新建一个MFC项目，下图为基于对话框的项目：
![](https://blog.csdn.net/youxishaonian/article/details/52590316)![](https://img-blog.csdn.net/20160919233005601?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在对话框上右击选择“打开ActiveX控件”，英文版“InsertActiveX Control”
![](https://blog.csdn.net/youxishaonian/article/details/52590316)![](https://img-blog.csdn.net/20160919233028128?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
找到Microsoft Communications Control，version 6.0，选择确定
![](https://blog.csdn.net/youxishaonian/article/details/52590316)![](https://img-blog.csdn.net/20160919233044492?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
此时在对话框中出现一个电话一下的按钮，这个就是MSComm控件。
![](https://blog.csdn.net/youxishaonian/article/details/52590316)![](https://img-blog.csdn.net/20160919233058960?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
同样可以把MSComm增加到工具箱，方法为点击菜单栏：工具->选择工具箱项，在com组件页找到“Microsoft Communications Control，version 6.0”选中打勾并确定，
![](https://blog.csdn.net/youxishaonian/article/details/52590316)![](https://img-blog.csdn.net/20160919233116410?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
即可在工具箱找到该控件:
![](https://blog.csdn.net/youxishaonian/article/details/52590316)![](https://img-blog.csdn.net/20160919233132273?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
以上。
