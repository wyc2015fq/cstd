# WINCE在桌面创建快捷方式 - xqhrs232的专栏 - CSDN博客
2013年08月24日 23:30:34[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：803
原文地址::[http://blog.csdn.net/wen_hua/article/details/4842465](http://blog.csdn.net/wen_hua/article/details/4842465)
相关网帖
1、WinCE 6.0 桌面添加应用程序的快捷方式----[http://hi.baidu.com/jiandanbinxin/item/dcd16bce6cf138374494169d](http://hi.baidu.com/jiandanbinxin/item/dcd16bce6cf138374494169d)
2、[WINCE快捷方式详解](http://blog.csdn.net/xqhrs232/article/details/8536197)----[http://blog.csdn.net/xqhrs232/article/details/8536197](http://blog.csdn.net/xqhrs232/article/details/8536197)
客户要求在桌面添加一个自己软件的快捷方式，网上找了找，感觉应该不难的，没想到给一个小细节卡到了。莫名其妙的路径问题。现在贴出来跟大家分享。
**1.WINCE快捷方式结构**
WINCE的快捷方式和WIN2K,XP的不同,只是一个后缀名为.LNK的文本文件.
其格式如下:
数字#路径
比如,有个快捷方式写法如下:
23#/Windows/ToolViewer.exe
指的是指向WINDOWS文件夹下的Toolviewer.exe可执行文件.数字究竟代表的是什么意思,我也不太清楚;有一说法是#后的ACSII字符 的数量,但如果把上述例子改为1#/Windows/ToolViewer.exe也可正常运行,但去掉数字则系统无法识别.虽然可以正常使用,但建议还 是按照文档#之后的ACSII字符数量填写.
这个就是路径的问题所在，lnk文件的内容应为23#"/Windows/ToolViewer.exe"  .#号前面的数字可以任意。只是#前面的数字不符时查看快捷方式的属性会发现路径后面多了几个口口。不过这并不影响。
**2.如何把快捷方式加到内核中**
假设我们有一快捷方式EnglishExp.lnk,要将其加到内核中.
在PB环境中打开工程,在project.bib文件的FILES字段中添加:
EnglishExp.lnk $(_WINCEROOT)/PLATFORM/MyAPPC/EnglishExp.lnk NK H
其中 $(_WINCEROOT)指的是PC机子上PB安装的根目录,H指的是文件属性,意义代表如下:
S:系统
H:隐藏
R:压缩的资源文件
C:压缩文件
U:非压缩文件
**3.如何把快捷方式放到桌面**
语法如下:Directory(”欲拷贝至的文件夹”):-File(”更改的名称”,”要拷贝的文件”)
假设我们有一快捷方式EnglishExp.lnk,要将其放到桌面,并把快捷方式的名称改为”英语”
在PB的project.dat文件加入此句:
Directory(”/Windows/LOC_DESKTOP_DIR”):-File(”英语.lnk”,”/Windows/CEnglishExp.lnk”)
包含在内核里面的文件全部释放到”/Windows”文件夹下.
LOC_DESKTOP_DIR指的是本地桌面.在PB中,常用的还有如下宏定义:
LOC_DESKTOP_DIR: 桌面
LOC_MYDOCUMENTS_DIR: My Documents
LOC_PROGRAMFILES_DIR: Program Files
LOC_FAVORITES_DIR: Favorites
LOC_RECENT_DIR: Recent
LOC_MYDOCUMENTS_DIR: My Documents
LOC_HELP_DIR: Help
小结：创建快捷方式本来是很简单的，不过有些细节的东西没搞明白真的会让你进了死胡同。希望我的这篇文章能对大家有用。
