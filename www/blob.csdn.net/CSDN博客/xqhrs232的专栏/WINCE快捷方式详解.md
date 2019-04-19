# WINCE快捷方式详解 - xqhrs232的专栏 - CSDN博客
2013年01月23日 23:16:45[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：661
原文地址::[http://www.hzlitai.com.cn/article/ARM9-article/system/1625.html](http://www.hzlitai.com.cn/article/ARM9-article/system/1625.html)
//=====================================================================================================
//TITLE:
// WINCE快捷方式详解
//AUTHOR:
// norains
//DATE:
// Saturday 15-April-2006
//=====================================================================================================
1.WINCE快捷方式结构
WINCE的快捷方式和WIN2K,XP的不同,只是一个后缀名为.LNK的文本文件.
其格式如下:
数字#路径
比如,有个快捷方式写法如下:
23#\Windows\ToolViewer.exe
指的是指向WINDOWS文件夹下的Toolviewer.exe可执行文件.数字究竟代表的是什么意思,我也不太清楚;有一说法是#后的ACSII字符 的数量,但如果把上述例子改为1#\Windows\ToolViewer.exe也可正常运行,但去掉数字则系统无法识别.虽然可以正常使用,但建议还 是按照文档#之后的ACSII字符数量填写.
2.如何把快捷方式加到内核中
假设我们有一快捷方式EnglishExp.lnk,要将其加到内核中.
在PB环境中打开工程,在project.bib文件的FILES字段中添加:
EnglishExp.lnk $(_WINCEROOT)\PLATFORM\MyAPPC\EnglishExp.lnk NK H
其中 $(_WINCEROOT)指的是PC机子上PB安装的根目录,H指的是文件属性,意义代表如下:
S:系统
H:隐藏
R:压缩的资源文件
C:压缩文件
U:非压缩文件
3.如何把快捷方式放到桌面
语法如下:Directory(”欲拷贝至的文件夹”):-File(”更改的名称”,”要拷贝的文件”)
假设我们有一快捷方式EnglishExp.lnk,要将其放到桌面,并把快捷方式的名称改为”英语”
在PB的project.dat文件加入此句:
Directory(”\Windows\LOC_DESKTOP_DIR”):-File(”英语.lnk”,”\Windows\CEnglishExp.lnk”)
包含在内核里面的文件全部释放到”\Windows”文件夹下.
LOC_DESKTOP_DIR指的是本地桌面.在PB中,常用的还有如下宏定义:
LOC_DESKTOP_DIR: 桌面
LOC_MYDOCUMENTS_DIR: My Documents
LOC_PROGRAMFILES_DIR: Program Files
LOC_FAVORITES_DIR: Favorites
LOC_RECENT_DIR: Recent
LOC_MYDOCUMENTS_DIR: My Documents
LOC_HELP_DIR: Help
更多的定义可以在ceshellfe.str文件中找到
4.消除快捷方式的小箭头
在PC机的操作系统中可以通过修改注册表来取消快捷方式左下角的小箭头,但在WINCE中注册表没有关于取消小箭头的键值(可能我没找到^_^).在这里我采用的是直接修改资源文件的做法.
打开shcore.res文件,其中文的路径在 $(_WINCEROOT)\PUBLIC\COMMON\OAK\LIB\ARMV4I\RETAIL\0804\,然后将1205的ICO的小箭头删 掉即可.注意,不是删除ID为1205的ICO图标,是删除ICO图标中的内容.
5.微软应用程序的快捷方式
如果在PB中添加了微软的应用软件,便会在桌面和程序中添加其快捷方式;如果觉得快捷方式碍眼,可以将其注释掉再编译即可.
现在列出一些dat文件中定义的微软应用软件的快捷方式:
wceappsfe.dat (C:\WINCE420\PUBLIC\WCEAPPSFE\OAK\FILES\),内含:pmail,wordpad
wceshellfe.dat (C:\WINCE420\PUBLIC\WCESHELLFE\OAK\FILES\),内含:iexplore,帮助文档在”help”文件夹的快捷方式
viewers.dat (C:\WINCE420\PUBLIC\VIEWERS\OAK\FILES\), 内含:pdfviewer,imageviewer,Presviewer,Docviewer
directx.dat (C:\WINCE420\PUBLIC\DIRECTX\OAK\FILES\),内含:Media Player,DVD Player
6. 修改默认shell
其实让一个程序在wince里启动和windows里差不多，直接设置其为启动项，这个有几个方法。一个就是制作一个快捷方式，指向我们的应用程序如app.exe，然后将快捷方式放到\windows\startup下面。
步骤如下：（假设app.exe已经拷贝到windows下面）
在pb中创建一个文件，文件类型选txt，然后命名为.lnk后缀，假设名字为test.lnk
编辑其内容为: 16#\windows\app.exe
备注：前面的16是# 后面所有字符的总和，包括空格。Wince的帮助文档上说这么定义就行，但是我尝试后，最后down到目标机上面时提示找不到文件，在wince里查看这 么创建的test.lnk的属性，发现其指向\windows\app.exe后面还有两个方框，因此不对，我的解决方法是修改test.lnk的内容为 16#”\Windows\app.exe”
编辑好lnk文件内容后在pb中修改project.bib
在files段后面添加下面一行：(和添加别的文件到image中类似，也要在pb的flatform菜单的setting下添加build语句，同上，不再赘述)
test.lnk $(_FLATRELEASEDIR)\test.lnk NK S
project.dat
增加下面一行：
Directory(”\Windows\startup”):-File(”test.lnk”,”\Windows\test.lnk”)
这样后系统启动后就会自动启动我们的程序了。
另外一种方法是编辑注册表：在project.reg中添加如下内容
[HKEY_LOCAL_MACHINE\init]
“Launch80″=”app.exe”
“Depend80″=hex:14,00,1e,00
这个是设定启动顺序，launch后面的数字越大的越是后启动，Depend80后面的指定依赖项，为16进制，上面的语句表明依赖项为launch20 定义的device.exe和launch30中定义的gwes.exe， 注意Launch后面的数字范围为0到99 ，此范围之外的将不会有效果。
这样两种方法的效果都是系统都是系统先启动资源管理器explorer.exe（就是看到的默认桌面），然后启动我们的程序，（如果利用 taskman shell然后去掉任务栏那么效果更好）但是这样还不够，我们如何不显示桌面，直接显示我们的程序呢？
网上有人介绍的方法是去掉standard shell，但是我编译总是报错。我采用的方法是替换注册表中lauch50中的explorer.exe为我的app.exe，即搞定。
修改注册表的方法：先把带KITL的系统跑起来，在PB的TOOLS->Remote registry editor里修改,验证有效后,再去修改platfrom.reg, 或者自己写个REG文件，然后在platform.reg里INCLUDE进来 SYSGEN后确认PBWORKSPACE里相关项目的REL目录里reginit.ini文件里包含了自己做的修改后make image然后DOWNLOAD下去就OK了。
值得补充的是，我们前面介绍的步骤中那个修改平台setting，添加语句的，是因为我每次都是重新sysgen和build，如果只是简单的 make image的话（都是pb中的build OS菜单下的命令），那么将直接用release中的内容，因此也可以直接将文件放到release文件夹，然后改project.bib等实现往 image中添加文件。 同样，也可以直接修改release中的shell.reg中的launch50值为我们自己的程序（或者类似修改reginit.ini文件，
 reginit.ini文件存放有所有wince的静态注册表，来达到去掉桌面，直接启动我们程序的效果）。
注意，这么启动的程序，如果点击关闭，就会死机的，因为没有窗口运行了。实际运用中，当然不会让用户关闭我们的程序，除非他一起关闭系统。
如果也需要build的话，可以通过往image中添加文件的方法将我们改好的shell.reg添加到release目录。
总之，今天是把定制shell算是基本完成了。
