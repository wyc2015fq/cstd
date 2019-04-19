# 用CEImageDiff.exe工具备份WINCE的工程文件 - xqhrs232的专栏 - CSDN博客
2013年08月21日 23:01:06[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：821
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://blog.csdn.net/dijkstar/article/details/1748562](http://blog.csdn.net/dijkstar/article/details/1748562)
相关网帖
1、[WinCE5.0初级教程-CEImageDiff使用](http://blog.csdn.net/cudng/article/details/1723933)----[http://blog.csdn.net/cudng/article/details/1723933](http://blog.csdn.net/cudng/article/details/1723933)
用CEImageDiff.exe工具备份WINCE的工程文件
这段时间用PB生成WINCE系统，生成工作任务是和外单位合作完成的，别人创建了PB5工程，并编译成功，阶段性的可以使用，工程文件等全部留在本地计算机上，若修改该工程文件（*.pbxml），如何备份前面已经成功的呢？试着将工程文件*.pbxml拷贝到其他目录，双击启动PB，发现这样创建的工程编译有问题，试来试去，觉得pbxml这样的工程文件还和VC6下的DSW工程文件不一样。
       网上发问，得到解决办法：用CEImageDiff.exe工具备份WINCE的工程文件。试了一下，还是真好用。
如何使用CEImageDiff.exe工具，以下段落摘自别人的（[http://blog.csdn.net/cudng/archive/2007/08/03/1723933.aspx](http://blog.csdn.net/cudng/archive/2007/08/03/1723933.aspx)），这里代用了，大家不必再找了：
**CEImageDiff使用**
————研博WinCE初级教程
版权：研博科技作者：Bill Zhong  日期：2007-7-4
CEImageDiff介绍：制作模板
[[点击浏览该文件:]](http://bbs.wince.cn/viewFile.asp?BoardID=20&ID=14) | [[快车下载]](http://bbs.wince.cn/dispbbs.asp?boardID=20&ID=588&page=1)
内容：用CEImageDiff制作模板
用途：用于发布工程的模板，或需要建立重复的工程，可很好避免重复拖选组件而浪费的时间；
1，在新建平台向导时，在Step 4时，要求选择可用的模板，如下图所示：
此模板所对应的目录：（D：为安装PB的盘符）
D:/WINCE500/PUBLIC/COMMON/OAK/CATALOG/NEWPLATFORMWIZARDS
2，用CEImageDiff制作模板（CEImageDiff可在附件获取）
3，双击运行它，如下图所示：
注：此时会在WINCE500创建一个PBSYSGENS.xml文件
4，打开一个PB的工程文件，单击“File”--“Open
 PB Workspace”；
5，再选择一个.pbxml工程文件作为模板
6，打开后，可从看到该工程含有所选取组件的列表
7，创建模板，单击“File”—“Build
 Platform Wizard”
8，输入模板的名称，如：embo241020070704；
9，按OK，请留意标题的变化，若创建成功，那么标题处会显示模板的名称
10，退出该程序，检测模板目录是否存在新创建的模板，留意下图，找到“Embo241020070704”此刚才创建的模板
11，重新打开PB，再创建OSdesign，在Step
 4时，会找到Embo241020070704的模板，如下图所示：
**结束语：**
PB模板目录：
D:/WINCE500/PUBLIC/COMMON/OAK/CATALOG/NEWPLATFORMWIZARDS
好处：避免拖选组件而浪费的时间
注：PB6.0改善以前版本每选择一个组件时等待的过程，采用复选框形式
//=============================================================================================
备注::
1>PB工程可以通过xxx.pbxml文件保存该工程的对应OS组件，所以可以使用这个文件来保存工程而不用每次再重新去配置/选择OS组件！！！
2>xxx.pbxml文件内容选录如下......
   <Feature Name="Item" CatalogItemId="Item:MS:sysgen_audio" />
    <Feature Name="Item" CatalogItemId="Item:MS:sysgen_auth" />
    <Feature Name="Item" CatalogItemId="Item:MS:sysgen_auth_ntlm" />
    <Feature Name="Item" CatalogItemId="Item:MS:sysgen_auth_schannel" />
    <Feature Name="Item" CatalogItemId="Item:MS:sysgen_autoras" />
    <Feature Name="Item" CatalogItemId="Item:MS:sysgen_aygshell" />
    <Feature Name="Item" CatalogItemId="Item:MS:sysgen_battery" />
    <Feature Name="Item" CatalogItemId="Item:MS:sysgen_certs" />
    <Feature Name="Item" CatalogItemId="Item:MS:sysgen_connmc" />

