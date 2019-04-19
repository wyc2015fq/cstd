# Notepad++使用技法 - xqhrs232的专栏 - CSDN博客
2016年03月25日 17:32:26[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：495
个人分类：[SI/Notepad++/EditPlus](https://blog.csdn.net/xqhrs232/article/category/906927)
原文地址：[http://www.cnblogs.com/exmyth/archive/2013/04/16/3024158.html](http://www.cnblogs.com/exmyth/archive/2013/04/16/3024158.html)
相关文章
1、notepad++ 设置及使用技巧----[http://www.thinkphp.cn/topic/6489.html](http://www.thinkphp.cn/topic/6489.html)
2、Notepad++使用技巧：[4]极其方便的操作技巧----[http://jingyan.baidu.com/article/1974b289adb7d3f4b1f774b5.html](http://jingyan.baidu.com/article/1974b289adb7d3f4b1f774b5.html)
TextFX,Function List,JSMin,JSLint,Light Explorer,Npp Export,JSON Viewer
Alt+H　　隐藏行
Ctrl+Tab  实现在多个打开的窗口间切换
Ctrl+Shift+Q区块注释
Ctrl+K行注释(取消Ctrl+Shift+K)
文件 
新建文件 Ctrl+N 
打开文件 Ctrl+O 
保存文件 Ctrl+S 
另存为 Ctrl+Alt+S 
全部保存 Ctrl+Shift+S 
关闭当前文件 Ctrl+W 
打印文件 Ctrl+P 
退出 Alt+F4
编辑 
撤销 Ctrl+Z 
恢复 Ctrl+Y 
剪切 Ctrl+X 
复制 Ctrl+C 
删除 Del 
全选 Ctrl+A 
列编辑 Alt+C
缩进 Tab 
删除缩进 Shift+Tab 
转为大写 Ctrl+Shift+U 
转为小写 Ctrl+U
复制当前行 Ctrl+D
删除当前行 Ctrl+L
分割行 Ctrl+I 
合并行 Ctrl+J 
上移当前行 Ctrl+Shift+Up 
下移当前行 Ctrl+Shift+Down
添加/删除单行注释 Ctrl+Q 
设置行注释 Ctrl+K 
取消行注释 Ctrl+Shift+K 
区块注释 Ctrl+Shift+Q
函数自动完成 Ctrl+Space
搜索 
查找 Ctrl+F 
在文件中搜索 Ctrl+Shift+F 
查找下一个 F3 
查找上一个 Shift+F3 
选定并找下一个 Ctrl+F3
行定位 Ctrl+G 
定位匹配括号 Ctrl+B 
设置/取消书签 Ctrl+F2
下一书签 F2 
上一书签 Shift+F2
视图 
全屏 F11 
便签模式 F12 
折叠所有层次 Alt+0 
展开所有层次 Alt+Shift+0
折叠当前层次 Ctrl+Alt+F
展开当前层次 Ctrl+Alt+Shift+F
隐藏行 Alt+H
从视图激活 F8
文本比较工具 
打开的文件比较 Alt+D 
关闭文件比较 Ctrl+Alt+D 
跟上次保存的文件比较 Alt+S 
跟SVN的文件比较 Alt+B 
上一个差异地方 Ctrl+Page UP 
下一个差异地方 Ctrl+Page Down 
第一个差异地方 Ctrl+Shift+Page Up 
最后一个差异地方 Ctrl+Shift+Page Down
Function List是Notepad++诸多很酷的功能之一。但最新的版本需要额外的步骤来正确设置Function List。笔者就将介绍如何安装Function List，以及如何设置Function List。
首先，Function List插件并没有在Notepad++自带的插件清单里，也没有在Plugin Manager的Available List里 = =|||。所以要安装Function List，还得去[Plugin_Central](http://sourceforge.net/apps/mediawiki/notepad-plus/index.php?title=Plugin_Central)下载，笔者下载的最新版本是2.1.0.1。
将下载的压缩包解压后就有诸多的文件了，首先复制FunctionList.dll到<notepad++ install dir>/plugins目录，再把FunctionListRules.xml,Gmod Lua.bmp和C++.flb复制到<notepad++ install dir>/plugins/config，到这里安装部分就完成了。
接下来就需要打开Notepad++对Function List进行设置了。Plugins->Function List->Language Parsing Rules。然后，在语言列表中选择C/C++，勾选上“Bitmap List”checkbox。浏览到plugins/config目录，并选择C++.flb，于是乎就万事大吉了。
notepad++挂接Python和gcc
Python挂接：
Run -> Run... 填写python可执行文件的路径以及notepad++预定义的源文件地址： D:\Python26\pythonw.exe "$(FULL_CURRENT_PATH)"
然后建议点下面的save附加快捷键
gcc挂接
Run -> Run... 填写***，我的是 D:\mingw\bin\gcc.exe "$(FULL_CURRENT_PATH)" -o "$(NAME_PART).exe"
因为编译要生成文件，通过Plug-Ins -> NppExec -> Follow $(CURRENT_DIRECTORY)可以使生成的exe在源文件目录里。
最近在用Notepad++，发现的确是很不错的工具，具体特色，看了下面介绍就知道了。
【notepad++简介】
Notepad++是旨在替代Windows默认的notepad而生，比notepad的功能强大很多很多。
（1）Notepad++的版本：ANSI和Unicode
Notepad++有两个版本，一个是ANSI版本，一个是UNICODE版本。
对于大多数人把Notepad++作为notepad的增强版，而需要的普通应用的话，ANSI版本和UNICODE版本，其实没啥差别。
（2）Notepad++支持丰富的插件
notepad++支持插件，添加对应不同的插件，以支持不同的功能。里面除了一些常见的插件之外，还有一些好玩的插件，比如将文字读出来的speech插件。
（3）Notepad++特点概述
Notepad++属于轻量级的文本编辑类软件，比其他一些专业的文本编辑类工具，比如UltraEdit等要启动更快，占用资源更少，但是从功能使用等方面来说，不亚于那些专业工具，而windows默认的notepad虽然更“轻量”，但是功能就太少。所以一般用notepad++，或者另一个工具notepad2来代替notapad，同样也代替太复杂和庞大的UltraEdit。
【notepad，notepad2，notepad++，ultraEdit比较】
根据个人对一些文本类编辑工具的使用感觉来看，notepad2比notepad++相对启动更快，占用资源更少，但是相对来说，功能比较少，所以，很多常用的方便的功能，还是没法使用。而ultraEdit虽然功能多，但是太消耗资源，用起来太麻烦了，关键还是收费的，而notepad++是免费的。总的概括如下：
notepad：windows自带的，占用资源最少，启动最快，但是功能太弱，以至于满足需求。是免费的。
notepad2：占用资源稍多，启动也很快，但是功能上，没有notepad++多，更没有ultraEdit多。是免费的。
notepad++：占用资源有点多，启动要停顿一下，但是功能很强大，尤其是支持插件模式，使得扩展性非常好，支持很多的其他高级功能，而且使用起来也很方便。是免费的。
ultraEdit：功能强大，但是安装文件很大，占用资源多，启动慢，而且是收费的。
因此，根据启动速度，占用资源，功能支持，等方面来综合衡量，对于notepad，notepad2，notepad++，ultraEdit来说，无疑notepad++是最好的选择了。
【notepad++插件添加方法】
方法1：可以手动去notepad++插件的官方网站下载：
[http://sourceforge.net/projects/npp-plugins/files/](http://sourceforge.net/projects/npp-plugins/files/)
方法2.让notepad的插件管理器自动下载和安装
点击：Plugins -> Plugin Manager -> Show Plugin Manager-> Avaliable一栏显示当前可用的插件列表，选中你要的插件，然后点击下面的Install即可自动下载和安装对应插件，很是方便。
![【详细】Notepad++使用心得和特色功能介绍 - notepad/ultraedit的最好的替代品，最好用的轻量级文本编辑器 v2011-12-20 - crifan - work and job](http://img7.ph.126.net/LPaqi_Qisnk40MVOtyOorQ==/638948197150344687.jpg)
![【详细】Notepad++使用心得和特色功能介绍 - notepad/ultraedit的最好的替代品，最好用的轻量级文本编辑器 v2011-12-20 - crifan - work and job](http://img9.ph.126.net/_5_rETIkdr2PMaUbwhfOog==/2545659689388324509.jpg)
下面说说个人的使用体会和使用心得：
【查看当前notepad++的版本】
查看自己当前Notepad++是什么版本的话，点击工具栏上的 ？-> About Notepad++，弹出对话框会显示“Notepad++ v5.8.6 （ANSI）”字样，其中(ANSI)即代表当前是ANSI版本。
个人觉得一些好用，值得一提的特性是：
1.支持语法高亮和颜色自定义
关键是支持的语言狂多，除了常见的c/c++,java,php,而且还支持python和xml。
而且对于xml，还支持点击关键字可以展开或收缩，很是方便。后来得知，这个特性叫做代码折叠，囧。。。
![【详细】Notepad++使用心得和特色功能介绍 - notepad/ultraedit的最好的替代品，最好用的轻量级文本编辑器 v2011-12-20 - crifan - work and job](http://img3.ph.126.net/Y6L22d_wVjCRWSWLlg_6Lg==/2794765043777250853.jpg)
还支持makefile，这样对于查看makefile文件的的话，就很方便了。
而且甚至连一般人不常用的Tex/LaTex的语法高亮，都支持了，很强大。
对于语法高亮时显示的颜色，也可以自定义，点击:
Settings -> Style Configurator
就可以根据自己需要配置不同的颜色了。
![【详细】Notepad++使用心得和特色功能介绍 - notepad/ultraedit的最好的替代品，最好用的轻量级文本编辑器 v2011-12-20 - crifan - work and job](http://img1.ph.126.net/nNOZpu0SvpAaSKuoNO9YUw==/2538904289947275661.jpg)
【自定义除默认后缀之外的其他后缀文件为某类型文件以实现语法高亮】
而且对于不同类型的文件的后缀，除了默认的文件后缀之外，设置自定义的文件后缀，比如我遇到的，对于普通的makefile，其默认的后缀是是.mak，而我此处自己的.mk也是makefile文件，所以可以通过这样来设置：
Settings -> Style Configurator -> 在Language中下拉找到Makefile，选择makefile，左下角会出现：
Default ext. 和user ext.,默认的扩展名（default extension）即后缀名.对应的是.mak,在user ext.所对应的框中填写.mk，
![【详细】Notepad++使用心得和特色功能介绍 - notepad/ultraedit的最好的替代品，最好用的轻量级文本编辑器 v2011-12-20 - crifan - work and job](http://img9.ph.126.net/J9pg5VaMOqtPmlYIt2xvPw==/2789698494196464029.jpg)
然后点击右边的 Save&Close，就可以实现notepad++自动识别.mk文件为makefile文件，然后引用对应的语法高亮了。
当然，如果不设置的话，单独打开.mk类型文件，notepad++虽然不会自动识别，没有语法高亮，但是也可以自己手动选择的：
language -> M -> Makefile，即可实现同样的功能，只是要麻烦自己每次都要手动设置而已。
2.添加hex editor插件以支持16进制编辑
notepad++支持添加HEX-Editor插件以支持16进制的查看与编辑，很是方便。
【Hex Editor插件的添加方法】
先通过上面的【查看当前notepad++的版本】的方法得知你自己的notepad++是ANSI还是UNICODE方法，
然后根据上面的 【notepad++插件添加方法】 去Notepad++插件的官网，找到并下载对应版本的Hex Editor，把插件文件HexEditor.dll，放到NotePad++路径下的 npp.5.8.6.bin\ansi\plugins文件夹里面即可。
当前也可以通过上面介绍的插件管理器，直接通过软件帮你自动下载并安装。
然后重启notepad++，就可以在Plugins中看到对应的Hex-Editor插件了。
值得一提的是，当前的最新版本（5.8.7）还支持了添加插件后，对应地在工具栏上也添加一个快捷图标，对于HexEditor来说，就是一个大写的H的图标，点击该图标，即可实现16进制查看，再次点击，又恢复到正常模式，很是方便。
![【详细】Notepad++使用心得和特色功能介绍 - notepad/ultraedit的最好的替代品，最好用的轻量级文本编辑器 v2011-12-20 - crifan - work and job](http://img7.ph.126.net/7Ptzkuw3OVgapDJl2s0kUw==/674976994169307417.jpg)
![【详细】Notepad++使用心得和特色功能介绍 - notepad/ultraedit的最好的替代品，最好用的轻量级文本编辑器 v2011-12-20 - crifan - work and job](http://img9.ph.126.net/zldIaLToVohPfsML9L6BeQ==/1043146263706847699.jpg)
3. 支持列模式编辑
按住Alt之后，就处于列模式了，然后比如你选取一列，然后点击右键，选中删除，就可以删除那些列了，松掉ALT就离开了ALT恢复普通模式了。非常好用，越来越觉得notepad++牛X了，啥功能都支持啊。。。
列操作模式中的在整个列中，整个都插入一些内容的话，可以这样操作：
Edit -> Column Editor (Alt C)
然后在Text to insert或Number to insert对应的的框中写入要插入的内容，notepad++就会自动帮你插入整列的对应的内容了。
![【详细】Notepad++使用心得和特色功能介绍 - notepad/ultraedit的最好的替代品，最好用的轻量级文本编辑器 v2011-12-20 - crifan - work and job](http://img9.ph.126.net/wIhOntpg9Ndp2-5vzHAB3A==/2654027555421932129.jpg)
4.选中某单词即关键字后，notepad++会自动高亮对应选中的关键字，对于文件查看来说，很是方便。
5.打开多个文件的时候，上面会显示对应的Tab，默认的话，双击Tab是不能像其他一些工具一样关闭该文件的，但是可以设置的：
Settings-> Preference ->General -> Tab Bar中，选中：
Double Click to close document, 然后双击对应文件的tab就可以关闭文件了。
类似地，还有个选项：show close button on each tab，选中后，每个tab上会出现关闭按钮，也很方便使用。
6。显示文件行号：Settings-> Preference ->General -> Editing-> 选中Display Line Number，即可显示行号
7.是否打印行号：Settings-> Preference ->General -> Printing-> 是否选中Printing Line Number，默认是不选的，如果需要，可以选中，这样打印时候就可以也打印行号了，还是很方便用户根据自己需要而去设置的。
8.编辑文件后，是否备份：类似Ultra Editor，编辑一个文件后，默认会生成.bak文件的，对于notepad++来说，也是支持此功能的，不过默认是关闭的。需要的话，可以去这里设置：Settings-> Preference ->General -> Backup/Auto-completion
 -> 默认是None，其他还有Simple和verbose backup，可以根据自己需要而设置。
9.自动完成功能：Settings-> Preference ->General -> Backup/Auto-completion，自动完成：Auto-Completion，可以根据自己需要而设置，默认是关闭的。
10.选中某文字，然后调用google搜索：
run -> google search
除了google搜索此项功能，于此相关类似的还有，选中文字后，调用其他程序或功能，比如直接调用wiki查询该单词/文字:
run -> wikipedia search
或者直接从notepad++中打开当前文件所在文件夹：
run -> open containing folder
从notepad++中打开windows的cmd，并且已经处于当前文件夹：
run -> open current dir cmd
直接把该文件作为附件，然后打开Windows的Outlook发送邮件
run -> send via Outlook
等等，都是非常实用好用的功能。
11.暂时隐藏某些行
如果在查看文件的时候，暂时觉得需要将某些行隐藏掉，在需要的时候再正常显示，就可以先选中那些行，然后：
View -> Hide Lines
即可。然后左边标签那列就会显示对应的标示，点击即可取消隐藏。
12.支持鼠标滚轮控制缩放
notepad++不仅支持快捷键Ctrl + Num+/Num- 去控制缩放，而且还支持Ctrl+鼠标滚轮去控制当前显示内容的缩放，很是方便。
13.全屏显示
有时候显示文档的话，希望显示更多的内容，希望用到全屏显示，通过view -> Toogle Full Screen Mode F11或者Post it F12，都可以实现全屏显示，也很是方便。
14.集成了轻量级的资源管理器
通过安装插件（默认好像是已经安装了）LightExplorer后：
Plugins -> Light Explorer -> Light Explorer,就可以在左侧打开一个轻量级的资源管理器，方法打开想要打开的文件，很是方便啊。
15.导出成其他类型文件 -> 实现彩色代码的粘贴
在添加了对应的NppExport插件后，就可以导出成不同的格式，比如HTML和RTF：
plugins -> NppExport -> Export to RTF或Export to HTML
其中HTML就是普通的HTML，RTF就可以理解为简洁版本的word，所以html和rtf，两者都是支持彩色代码/字符的，因此，这个功能就可以实现我们所需要的，将已经高亮显示，彩色显示，着色后的代码，导出到word或者其他文档中了。这样的功能，是很实用的。至少我个人觉得很好用，比如将一些C代码打开后，导出成html，然后打开html复制内容后粘贴到word里面，就是彩色的代码了。当然，你也可以导出成rtf，然后本身就是和word兼容的，windows下，也就是默认用word打开的了，就省去了从html拷贝粘贴过来的步骤了。
甚至，NppExport还提供了更方便的功能：
plugins -> NppExport -> Copy HTML to clipboard 或Copy RTF to clipboard
即，直接选择内容后，拷贝该内容到剪贴板，然后你就可以在其他word文件中直接粘贴了，即如此简单地就实现了彩色代码的支持，多么爽啊。
16.代码折叠
包括对C/C++等代码，可以以单个函数为整体进行代码的折叠和展开，其他的类型的，比如XML，根据关键字所包含的内容为整体进行折叠和展开等等。
17.资源管理器中，右键点击文件，可以直接用notepad++打开该文件
这个功能类似于UltraEdit，在资源管理器中添加了右键支持，右键点击某文件后，会出现“Edit With Notepad++”，就可以直接用notepad++打开该文件了，个人觉得这个功能还是很实用的。
18.Ctrl+Tab实现在多个打开的窗口间切换
![【详细】Notepad++使用心得和特色功能介绍 - notepad/ultraedit的最好的替代品 - carifan - work and job](http://img.bimg.126.net/photo/OG0zjtGModZm4BBFTt3y2w==/5776992422009677299.jpg)
19. 支持设置是否显示列边界（右边线）
设置->首选项->编辑->列边界，可以设置是否显示列边界，而且也支持自定义“边界宽度”的大小，默认是100。
（注：一般Linux下面常用的是80.）
而且对于列边界显示模式，还支持两种：
一种是从上到下的竖边线：
![【详细】Notepad++使用心得和特色功能介绍 - notepad/ultraedit的最好的替代品，最好用的轻量级文本编辑器 v2011-12-20 - crifan - work and job](http://img8.ph.126.net/dO2DOrPVjyuKhpomzWpypQ==/2892718335672556338.jpg)
另一种是把超过列边界的内容加上一定的底色：
![【详细】Notepad++使用心得和特色功能介绍 - notepad/ultraedit的最好的替代品，最好用的轻量级文本编辑器 v2011-12-20 - crifan - work and job](http://img9.ph.126.net/4H_it7CNTEpyKcliiPq8vw==/1081708335516198029.jpg)
20.可以给代码批量添加或取消注释
选中一段代码，然后：编辑 -> 注释/取消注释 -> 添加/删除单行注释，即可批量地队选中的每一行，添加或者删除注释。
此功能，对于常写代码的人来说，还是很实用的。
![【详细】Notepad++使用心得和特色功能介绍 - notepad/ultraedit的最好的替代品，最好用的轻量级文本编辑器 v2011-12-27 - crifan - work and job](http://img2.ph.126.net/pXIO5PizhUVdHQ2cFXoU0w==/2764084271315805582.jpg)
notepad++的更多的其他功能，感兴趣的自己去探索。
总之，个人觉得，Notepad++，是个非常好用的，轻量级的，但是功能足够丰富的，文字/代码编辑工具，在此，非常地感谢那些开发者，谢谢其为我们带来这个好用的工具。
1. 可以消除文件中的空行
    选中所有文本, 
    TextFX--->Edit--->Delete Blank lines2. 整理xml文本格式
    这个功能超好，经常遇到几百KB左右的xml文本就是一行，可以使用
它很快将一行文本整理成规范的xml文件。
    选中所有文本
    TextFX--->HTML Tidy--->Tidy: Reindent XML3. 直接在你的文本行前自动添加行号：
   选中所有文本
   TextFX--->Tools--->Insert Line Numbers
4. 可以用来查看16进制格式
5. 有ftp插件，还不错
6. 好像还有一个Explorer插件，没怎么用过
7. 它的函数列表也蛮不错, 常用，至少看java源程序的时候要用到
8. 可以定制外部命令，我有时候用它来编译java源程序 按F6，就有一个定制窗口。
　这是我的javac命令： jc5.bat $(FULL_CURRENT_PATH)
下边是我的运行java的命令
rj5.bat $(NAME_PART)
我只要在外边PATH路径上的某个目录里把jc5.bat和rj5.bat设置好就可以使用了。
比如，我在这里把d:/shell添加到PATH环境变量里，里边可以放很多自己喜欢的批处理脚本。
jc5.bat内容大致如下：
jc5.bat
echooff
setlocal
set JAGUAR_JDK15=d:sharedjdk1.5.0_01
set JAVA_HOME=%JAGUAR_JDK15%
setPATH=%JAVA_HOME%bin;%JAVA_HOME%jrebin;%PATH%
set OLD_CLASSPATH=%CLASSPATH%
set CLASSPATH=.;%JAVA_HOME%libdt.jar;
set CLASSPATH=%CLASSPATH%;%JAVA_HOME%libtools.jar;
set CLASSPATH=%CLASSPATH%;%JAVA_HOME%jrelibrt.jar
set CLASSPATH=%CLASSPATH%;%OLD_CLASSPATH%
"%JAVA_HOME%injavac"  -g %*
endlocal

