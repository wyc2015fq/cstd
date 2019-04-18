# Eclips使用秘技简述 - weixin_33985507的博客 - CSDN博客
2006年04月26日 17:15:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
前言：  
本来我都是使用JBuilderX当主力IDE、但使用了Eclipse后  
发现...Eclipse原来也这么好用...渐渐的就爱上了它......  
Eclipse优点：免费、程序代码排版功能、有中文化包、可增  
设许多功能强大的外挂、支持多种操作系统(Windows、  
Linux、Solaris、Mac OSX)..等等。  
开此篇讨论串的目的，是希望能将Eclipse的一些使用技巧集  
合起来...欢迎大家继续补充下去...由于Eclipse的版本众多  
，希望补充的先进们能顺便说明一下您所使用的版本～  
Eclipse网站：[http://www.eclipse.org/](http://www.eclipse.org/) ; 
Eclipse中文化教学：JavaWorld站内文章参考  
(使用版本：Eclipse 2.1.2 Release + 中文化)  
热键篇：  
Template：Alt + /  
修改处：窗口->喜好设定->工作台->按键->编辑->内容辅助。  
个人习惯：Shift+SPACE(空白)。  
简易说明：编辑程序代码时，打sysout +Template启动键，就  
会自动出现：System.out.println(); 。  
设定Template的格式：窗口->喜好设定->Java->编辑器->模板。  
程序代码自动排版：Ctrl+Shift+F  
修改处：窗口->喜好设定->工作台->按键->程序代码->格式。  
个人习惯：Alt+Z。  
自动排版设定：窗口->喜好设定->Java->程序代码格式制作程序。  
样式页面->将插入tab(而非空格键)以内缩，该选项取消勾选  
，下面空格数目填4，这样在自动编排时会以空格4作缩排。  
快速执行程序：Ctrl + F11  
个人习惯：ALT+X  
修改处：窗口->喜好设定->工作台->按键->执行->启动前一次的启动作业。  
简易说明：第一次执行时，它会询问您执行模式，  
设置好后，以后只要按这个热键，它就会快速执行。  
<ALT+Z(排版完)、ATL+X(执行)>..我觉得很顺手^___^  
自动汇入所需要的类别：Ctrl+Shift+O  
简易说明：  
假设我们没有Import任何类别时，当我们在程序里打入：  
BufferedReader buf =  
new BufferedReader(new InputStreamReader(System.in));  
此时Eclipse会警示说没有汇入类别，这时我们只要按下Ctrl+Shift+O  
，它就会自动帮我们Import类别。  
查看使用类别的原始码：Ctrl+鼠标左键点击  
简易说明：可以看到您所使用类别的原始码。  
将选取的文字批注起来：Ctrl+/  
简易说明：Debug时很方便。  
修改处：窗口->喜好设定->工作台->按键->程序代码->批注  
视景切换：Ctrl+F8  
个人习惯：Alt+S。  
修改处：窗口->喜好设定->工作台->按键->窗口->下一个视景。  
简易说明：可以方便我们快速切换编辑、除错等视景。  
密技篇：  
一套Eclipse可同时切换，英文、繁体、简体显示：  
1.首先要先安装完中文化包。  
2.在桌面的快捷方式后面加上参数即可，  
英文-> -nl "zh_US"  
繁体-> -nl "zh_TW"  
简体-> -nl "zh_CN"。  
(其它语系以此类推)  
像我2.1.2中文化后，我在我桌面的Eclipse快捷方式加入参数-n1 "zh_US"。  
"C:\Program Files\eclipse\eclipse.exe" -n "zh_US"  
接口就会变回英文语系噜。  
利用Eclipse，在Word编辑文书时可不必将程序代码重新编排：  
将Eclipse程序编辑区的程序代码整个复制下来(Ctrl+C)，直接贴(Ctrl+V)到  
Word或WordPad上，您将会发现在Word里的程序代码格式，跟Eclipse  
所设定的完全一样，包括字型、缩排、关键词颜色。我曾试过JBuilder  
、GEL、NetBeans...使用复制贴上时，只有缩排格式一样，字型、颜  
色等都不会改变。  
外挂篇：  
外挂安装：将外挂包下载回来后，将其解压缩后，您会发现features、  
plugins这2个数据夹，将里面的东西都复制或移动到Eclipse的features  
、plugins数据夹内后，重新启动Eclipse即可。  
让Eclipse可以像JBuilderX一样使用拖拉方式建构GUI的外挂：  
1.Jigloo SWT/Swing GUI Builder ：  
[http://cloudgarden.com/jigloo/index.html](http://cloudgarden.com/jigloo/index.html) ; 
下载此版本：Jigloo plugin for Eclipse (using Java 1.4 or 1.5)  
安装后即可由档案->新建->其它->GUI Form选取要建构的GUI类型。  
2.Eclipse Visual Editor Project：  
[http://www.eclipse.org/vep/](http://www.eclipse.org/vep/) ; 
点选下方Download Page，再点选Latest Release 0.5.0进入下载。  
除了VE-runtime-0.5.0.zip要下载外，以下这2个也要：  
EMF build 1.1.1: (build page) (download zip)  
GEF Build 2.1.2: (build page) (download zip)  
3.0 M8版本，请下载：  
EMF build I200403250631  
GEF Build I20040330  
VE-runtime-1.0M1  
安装成功后，便可由File->New->Visual Class开始UI设计。  
安装成功后，即可由新建->Java->AWT与Swing里选择  
所要建构的GUI类型开始进行设计。VE必须配合着对应  
版本，才能正常使用，否则即使安装成功，使用上仍会  
有问题。  
使用Eclipse来开发JSP程序：  
外挂名称：lomboz(下载页面)  
[http://forge.objectweb.org/project/showfiles.php?group_id=97](http://forge.objectweb.org/project/showfiles.php?group_id=97) ; 
请选择适合自己版本的lomboz下载，lomboz.212.p1.zip表示2.1.2版，  
lomboz.3m7.zip表示M7版本....以此类推。  
lomboz安装以及设置教学：  
Eclipse开发JSP-教学文件  
Java转exe篇：  
实现方式：Eclipse搭配JSmooth(免费)。  
1.先由Eclipse制作包含Manifest的JAR。  
制作教学  
2.使用JSmooth将做好的JAR包装成EXE。  
JSmooth下载页面：  
[http://jsmooth.sourceforge.net/index.php](http://jsmooth.sourceforge.net/index.php) ; 
3.制作完成的exe文件，可在有装置JRE的Windows上执行。  
Eclipse-Java编辑器最佳设定：  
编辑器字型设定：工作台->字型->Java编辑器文字字型。  
(建议设定Courier New -regular 10)  
编辑器相关设定：窗口->喜好设定->Java->编辑器  
外观：显示行号、强调对称显示的方括号、强调显示现行行、  
显示打印边距，将其勾选，Tab宽度设4，打印编距字段设80。  
程序代码协助：采预设即可。  
语法：可设定关键词、字符串等等的显示颜色。  
附注：采预设即可。  
输入：全部字段都勾选。  
浮动说明：采预设即可。  
导览：采预设即可。  
使自动排版排出来的效果，最符合Java设计惯例的设定：  
自动排版设定：窗口->喜好设定->Java->程序代码制作格式。  
换行：全部不勾选。  
分行：行长度上限设：80。  
样式：只将强制转型后插入空白勾选。  
内缩空格数目：设为4。  
Eclipse的教学文件：  
Eclipse 3.0系列热键表 - 中英对照解说版 (by sungo) ~New~  
Window+GCC+CDT用Eclipse开发C、C++ (by sungo) ~New~  
其它：  
扩充Eclipse的Java 开发工具(中文)  
使用Eclipse开发J2EE 应用程序(中文)  
使用Eclipse平台进行除错(中文)  
用Eclipse进行XML 开发(中文)  
开发Eclipse外挂程序(中文)  
国际化您的Eclipse外挂程序(英文)  
将Swing编辑器加入Eclipse(英文)  
如何测试你的Eclipse plug-in符合国际市场需求(英文)  
Eclipse的相关网站：  
[http://eclipse-plugins.2y.net/eclipse/index.jsp](http://eclipse-plugins.2y.net/eclipse/index.jsp) ; 
[http://www.eclipseplugincentral.com/](http://www.eclipseplugincentral.com/) ; 
Eclipse相关教学[简体]  
写程序写到很累了，想休息一下？？玩玩小Game是  
不错的选择，下面介绍使用Eclipse玩Game的Plug-in。  
补充外挂篇：  
Eclipse-Games：  
[http://eclipse-games.sourceforge.net/](http://eclipse-games.sourceforge.net/) ; 
版本选：Latest Release 3.0.1 (Release Notes) Sat, 3 Jan 2004  
外挂安装完后，重新开启Eclipse。  
窗口->自订视景->其它->勾选Game Actions。  
再将Eclipse关闭，重新再启动，就可以开始玩噜。  
(共有4种：采地雷I、采地雷II、贪食蛇、仓库番。)  
(Eclipse 2.1.2 +中文化 玩Game -仓库番)    
补充：(于Eclipse使用assertion机制)  
Eclipse版本：2.1.3 release。  
JDK1.4版新加入的assertion机制(关键词：assert)，由于JDK1.4编译器  
预设是兼容1.3，所以要使用assert必须在编译时加上-source 1.4的参数。  
C:\>javac -source 1.4 XXX.java 
执行时则必须加-ea 或-enableassertions参数启动。  
C:\>java -ea XXX 
如要在Eclipse中使用assertion机制，请作以下设定：  
设定一：(编译设定)  
Windows->Preferance->Java->Compiler->Compliance and Classfiles  
页面。将..JDK Compliance level->Compiler compliance level调成1.4。  
设定二：(执行设定)  
Run->Run->(x)=Arguments页面，在VM arguments加入-da参数，按下  
Run button便可看到启动assertion后的执行结果。  
(Eclipse 2.1.3 release + assertion测试)  
<assert判别为false，所以show出AssertionError>    
新版(m8+)的eclipse可以设vm arguments  
另外提供一种设法，是在eclipse启动时加入vm arguments(跟加大eclipse预设内存大小的方式一样)  
这样就不用每次run都得需去设vm arguments
