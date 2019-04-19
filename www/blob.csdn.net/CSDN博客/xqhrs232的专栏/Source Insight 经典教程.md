# Source Insight 经典教程 - xqhrs232的专栏 - CSDN博客
2010年05月12日 10:36:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1054标签：[reference																[makefile																[linux内核																[linux																[windows](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=linux内核&t=blog)](https://so.csdn.net/so/search/s.do?q=makefile&t=blog)](https://so.csdn.net/so/search/s.do?q=reference&t=blog)
个人分类：[SI/Notepad++/EditPlus](https://blog.csdn.net/xqhrs232/article/category/906927)
原文地址::[http://blog.csdn.net/dclchj/article/details/6789243](http://blog.csdn.net/dclchj/article/details/6789243)

文章转自：[http://blog.21ic.com/user1/4251/archives/2008/53650.html](http://blog.21ic.com/user1/4251/archives/2008/53650.html)
Source Insight实质上是一个支持多种开发语言（java,c ,c等等）的编辑器，只不过由于其查找、定位、彩色显示等功能的强大，常被我们当成源代码阅读工具使用。
作为一个开放源代码的操作系统，Linux附带的源代码库使得广大爱好者有了一个广泛学习、深入钻研的机会，特别是Linux内核的组织极为复杂，同时，又不能像windows平台的程序一样，可以使用集成开发环境通过察看变量和函数，甚至设置断点、单步运行、调试等手段来弄清楚整个程序的组织结构，使得Linux内核源代码的阅读变得尤为困难。
当然Linux下的vim和emacs编辑程序并不是没有提供变量、函数搜索，彩色显示程序语句等功能。它们的功能是非常强大的。比如，vim和emacs就各自内嵌了一个标记程序，分别叫做ctag和etag，通过配置这两个程序，也可以实现功能强大的函数变量搜索功能，但是由于其配置复杂，linux附带的有关资料也不是很详细，而且，即使建立好标记库，要实现代码彩色显示功能，仍然需要进一步的配置（在另一片文章，我将会讲述如何配置这些功能），同时，对于大多数爱好者来说，可能还不能熟练使用vim和emacs那些功能比较强大的命令和快捷键。
为了方便的学习Linux源程序，我们不妨回到我们熟悉的window环境下，也算是“师以长夷以制夷”吧。但是在Window平台上，使用一些常见的集成开发环境，效果也不是很理想，比如难以将所有的文件加进去，查找速度缓慢，对于非Windows平台的函数不能彩色显示。于是笔者通过在互联网上搜索，终于找到了一个强大的源代码编辑器，它的卓越性能使得学习Linux内核源代码的难度大大降低，这便是Source Insight3.0，它是一个Windows平台下的共享软件，可以从[http://www.sourceinsight.com/](http://www.sourceinsight.com/)上边下载30天试用版本。由于Source
 Insight是一个Windows平台的应用软件，所以首先要通过相应手段把Linux系统上的程序源代码弄到Windows平台下，这一点可以通过在linux平台上将/usr/src目录下的文件拷贝到Windows平台的分区上，或者从网上光盘直接拷贝文件到Windows平台的分区来实现。
下面主要讲解如何使用Source Insight，考虑到阅读源程序的爱好者都有相当的软件使用水平，本文对于一些琐碎、人所共知的细节略过不提，仅介绍一些主要内容，以便大家能够很快熟练使用本软件，减少摸索的过程。
安装Source Insight并启动程序，可以进入图1界面。在工具条上有几个值得注意的地方，如图所示，图中内凹左边的是工程按钮，用于显示工程窗口的情况；右边的那个按钮按下去将会显示一个窗口，里边提供光标所在的函数体内对其他函数的调用图，通过点击该窗体里那些函数就可以进入该函数所在的地方。
**图1 Source Insight界面图**
[](http://www-128.ibm.com/developerworks/cn/linux/l-tip-prompt/tip17/1.jpg)
![](http://hi.csdn.net/attachment/201109/19/0_1316400520hhZh.gif)
由于Source Insight实质上是一个支持多种开发语言（java,c ,c++等等）的编辑器，只不过由于其查找、定位、彩色显示等功能的强大，而被我们当成源代码阅读工具使用。所以，为了有效的阅读源程序，首先必须选择功能菜单上的 “Project”选项的子菜单“New Project”新建一个项目，项目名称可以自由选定，当然也可以选择删除（Remove）一个项目。当删除一个项目的时候，并不删除原有的源代码文件,只是将该软件生成的那些工程辅助文件删除。设定之后，将会弹出一个对话框如图2，接受默认选择，如果，硬盘空间足够，可以将第一个复选框选上，该选项将会需要与源代码大致同等的空间来建立一个本地数据库以加快查找的速度。
**图2 工程设置**
![](http://hi.csdn.net/attachment/201109/19/0_1316400558wrar.gif)[](http://www-128.ibm.com/developerworks/cn/linux/l-tip-prompt/tip17/2.jpg)
点击“OK”按钮，接受选择后，将会有一个新的对话框弹出，在这个对话框里，可以选择将要阅读的文件加入工程，一种方式是通过在File Name中输入要阅读源代码文件的名称，点击“Add”按钮将其加入，也可以通过其中“Add All”和“Add Tree”两个按钮可以将选中目录的所有文件加入到工程中，其中“Add All”选项会提示加入顶层文件和递归加入所有文件两种方式，而“Add Tree”相当于“Add All”选项的递归加入所有文件，可以根据需要使用，就我来说，更喜欢“Add Tree”一些。由于该程序采用了部分打开文件的方式，没有用到的文件不会打开，所以，加入数千个文件也不用担心加入的文件超出程序的所能容忍的最大值，我就是采用“Add
 Tree”的方式将Linux2.4内核的四千五百九十一个文件加入的。
**图3 添加文件**
[](http://www-128.ibm.com/developerworks/cn/linux/l-tip-prompt/tip17/3.jpg)
![](http://hi.csdn.net/attachment/201109/19/0_1316400601z9HT.gif)
加入文件后，点击一个文件，可以出现使用界面，如图4所示，其中，右边的那个窗口（Linux Project，即工程窗口）缺省按照字母顺序列出当前工程中所有的文件。
**图4 工作窗口**
[](http://www-128.ibm.com/developerworks/cn/linux/l-tip-prompt/tip17/4.jpg)
![](http://hi.csdn.net/attachment/201109/19/0_1316400629aS44.gif)
点击一个文件就可以打开该文件，显示如图5所示，进入到右边的那个窗口分别可以以文件列表的方式，列出所有的文件，每个窗体下边有一排按钮，左边的窗口（21142.c）从左至右分别为：按字母顺序排列所有标记、按照文件中行数顺序排列标记、按照类型排列标记、浏览本地文件标记、标记窗口属性。右边的窗口（Linux Project）从左至右分别为：按字母顺序文件列表、显示文件夹、按照文件类型归类文件、全部文件的所有标记列表、按照标记类型归类标记、跳转到定义处、显示标记信息、浏览工程标记、查找函数调用、工程属性，其中全部文件的所有标记列表选项可能要一段时间抽取标记，同步到数据库去，如果开始选择了建立标记数据库，将会在今后节省同步时间，最有用的莫过于浏览标记信息和查找函数调用，前者可以通过“Jump”按钮在不同的地方查找同样的标志，还可以通过“Reference”按钮结合后者进行全局的标记查找。
[](http://www-128.ibm.com/developerworks/cn/linux/l-tip-prompt/tip17/5.jpg)
![](http://hi.csdn.net/attachment/201109/19/0_1316400662h5Ik.gif)
Reference功能是Source Insight的特色之一，它可以在速度极快的在整个工程中找到所有的标记，并且在该行程序的前边加上红色箭头的小按钮链接上。图6是一个Reference搜索后的结果，它可以有两种模式，一种集中显示结果，图6显示的就是这种模式，在这种模式下，可以通过前边的红色箭头小按钮进入另外一种模式，该标记的具体的所在处，也可以通过标记的具体所在处点击红色箭头小按钮进入警种模式，还可以通过工具条上的两个红色小箭头直接在第二种模式下前后移动，察看相应信息。它的这个强大的功能使得阅读Linux源程序有如神助。但是要注意的是，当进行了第二次“Reference”时，它会提示你将结果集附加在第一个结果集的后边还是取代第一个结果集。如果选择前者，不能对结果集根据前后两次搜索结果进行分类，然后在其子类里进行移动，只能在整个结果集里移动；如果，选择后者，结果集将会被替换为第二次搜索的结果，略微有些不方便。
**图6 Reference的搜索结果**
[](http://www-128.ibm.com/developerworks/cn/linux/l-tip-prompt/tip17/6.jpg)
![](http://hi.csdn.net/attachment/201109/19/0_1316400708Rn61.gif)
   Source Insight设置
1、背景色选择 
    要改变背景色Options->preference->windows background->color设置背景色
2、解决字符等宽对齐问题。
    SIS默认字体是VERDANA，很漂亮。这网页上应该也是用的VERDANA字体。但由于美观的缘故，VERDANA字体是不等宽的。比如下面两行
    llllllllll
    MMMMMMMMMM
    同样10个字符，长度差多了.用VERDANA来看程序，有些本应该对齐的就歪了。解放方法是使用等宽的字体，但肯定比较丑。比较推荐的是用Courier New。
3、解决TAB键缩进问题
    Options-> Document Options里面的右下角Editing Options栏里，把Expand tabs勾起来，然后确定。OK，现在TAB键的缩进和四个空格的缩进在SIS里面看起来就对齐咯
4、SI中的自动对齐设置:
    在C程序里, 如果遇到行末没有分号的语句,如IF, WHILE, SWITCH等, 写到该行末按回车，则新行自动相对上一行缩进两列。
    Option->Document option下的Auto Indient中Auto Indient Type有三种类型 None,Simple,Smart。个人推荐选用Simple类型。
5、向项目中添加文件时，只添加特定类型的文件（文件类型过滤器）
    编辑汇编代码时，在SIS里建立PROJECT并ADD TREE的时候，根据默认设置并不会把该TREE里面所有汇编文件都包含进来
    只加了.inc和.asm后缀的，.s后缀的没有。而且用SIS打开.s的文件，一片黑白没有色彩，感觉回到DOS的EDIT时代了…… 
    解决方法是在Options->Document Options里面，点左上的Document Type下拉菜单，选择x86 Asm Source File，
    然后在右边的File filter里*.asm；*.inc；的后面加上*.s；接着CLOSE就可以了。
    上面问题解决了，但注意加入*.s后还需要重新ADD TREE一遍才能把这些汇编加到PROJECT里面。
6、添加文件类型
    用户可以定义自己的类型，Options->Document Options->add type,定义文件类型名以及文件名后缀。
    勾选include when adding to projects在添加目录下文件到工程是该类文件就会添加进SI的工程。
    如果需要将所有文件添加进SI的工程，可以定义一种文件类型*.*。  
7、恢复ctrl+a的全选功能
    通过关键词save 找到save all，更改为ctrl+shift+a，通过关键词select找到select all，更改为ctrl +a
Source Insight常用的快捷键：
    Ctrl+= :Jump to definition
    Alt+/ :Look up reference 
    F3 : search backward
    F4 : search forward
    F5: go to Line
    F7 :Look up symbols
    F8 :Look up local symbols
    F9 :Ident left
    F10 :Ident right
    Alt+, :Jump backword
    Alt+. : Jump forward
    Shift+F3 : search the word under cusor backward
    Shift+F4 : search the word under cusor forward
    F12 : incremental search
    Shift+Ctrl+f: search in project
    shift+F8 : hilight word
Source Insight的窗口操作:
    project window Ctrl+O打开 
    symbol window Alt+F8打开和关闭
    Contex Window 自定义键打开和关闭
    Relation Window 自定义键打开　先锁定再刷新联系
在Source Insight中添加自定义功能的步骤如下：
1.Source Insight中,Options->Custom Commands...->Add...,New Command name 随便写,我的是"Edit with Vim"
2.Run中写入: "C:\Program Files\Vim\vim63\gvim.exe" --remote-silent +%l %f 
意思是在当前已经打开的gvim窗口里面打开当前的文件,并且跳转到指定行 
%l为当前的行号,%f为文件名 
使用 --remote-silent 的作用是,如果已经打开了对应文件,就不会打开第二次,而是在已经打开的文件里跳转到对应行 
3.还是同一个对话框里面,选择Keys->Assign New Key...->按F12,如果你已经将F12设置给其他命令,选择其他的按键就行了
下面是一些常用自定义功能：( CUSTOM COMMANDS )
打开资源管理器并选中当前文件
ShellExecute open explorer /e,/select,%f
查看log
"C:\Program Files\TortoiseSVN\bin\TortoiseProc.exe" /command:log /path:%f /notempfile /closeonend
diff
"C:\Program Files\TortoiseSVN\bin\TortoiseProc.exe" /command:diff /path:%f /notempfile /closeonend
取得锁定(check out)
"C:\Program Files\TortoiseSVN\bin\TortoiseProc.exe" /command:lock /path:%f /notempfile /closeonend
提交（check in）
"C:\Program Files\TortoiseSVN\bin\TortoiseProc.exe" /command:commit /path:%f /notempfile /closeonend
更新（update）
"C:\Program Files\TortoiseSVN\bin\TortoiseProc.exe" /command:update /path:%f /notempfile /closeonend
更新整个目录（update all）
"C:\Program Files\TortoiseSVN\bin\TortoiseProc.exe" /command:update /path:*.* /notempfile /closeonend
取消锁定（undo check out）
"C:\Program Files\TortoiseSVN\bin\TortoiseProc.exe" /command:revert /path:%f /notempfile /closeonend
在ultriEdit中编辑
"C:\Program Files\UltraEdit-32/uedit32" %f
在vim中编辑并定位到当前行
"C:\Program Files\Vim\vim63\gvim.exe" --remote-silent +%l %f
汇总其他小技巧：
让{ 和 } 不缩进：
Options->Document Options->Auto Indent->Indent Open Brace/Indent Close Brace
hao space: SourceInsight 小技巧
1、按住"ctrl", 再用鼠标指向某个变量，点击一下，就能进入这个变量的定义。
2、今天把一个用sourceinsight排版整齐的C文件，偶然用VC打开一看，全乱了。研究了半天，发现SI对每个字符的宽度不太一致。
    请教同事发现选上"view --> draft view"， 就可以让每个字符的宽度一致了。快捷键是 "Alt + F12"
3、"shift+F8" 标亮所有文本中光标所在位置的单词
4、跳到某一行："ctrl + g"
Source Insight是阅读和编写代码的好东东，基本上也算得上是经典之作了，虽然还有一点点小bug，不过对于我们这些C程序员来说可是一旦拥有别无所求。下 列小技巧是在工作中同事整理总结的，对提高工作效率多少有点帮助，其中有些是对应于SVN的，没有使用SVN做版本管理的人就不要白费力气了。
ShellExecute open explorer /e,/select,%f
        /*作用是在资源管理器中打开当前编辑文件并选中*/
        /*可以设置快捷键如ctrl+e，这样能很方便的在资源管理器打开对应的文件，并进行tortoiseSVN的相关操作*/
X:\Progra~1\TortoiseSVN\bin\TortoiseProc.exe /command:log /path:% /notempfile /closeonend
        /*使用前注意更改对应的bin安装路径*/
        /*作用是直接查看当前文件的svn log*/
        /*可以设置快捷键如ctrl+l*/
X:\Progra~1\TortoiseSVN\bin\TortoiseProc.exe /command:diff /path:% /notempfile /closeonend
        /*使用前注意更改对应的bin安装路径*/
        /*作用是直接查看当前文件和基准版本的比较*/
        /*可以设置快捷键如ctrl+d*/
                                       Source Insight中的文件过滤器
遇到在新建工程的时候，需要加入一些除了.c .h 等之外的文件，比如.s，.scf ，Makefile和ReleaseNotes等文件，而每次新建工程的时候，即使取消了**shown only known document types**,和在点击Add All之后选择了**Recusively add lower sub-directories** ，也**还是不能正常识别这类文件**，也就不能加入进工程，只能我们自己手动双击添加到工程中，如果文件少还无所谓，但是包含了很多子文件夹的大工程，这样实在不可行。
百度了下，终于发现解决办法了：
打开Source Insight，在新建工程之前，进入
**Options**-> **Document Options... Alt-T**    -> 点击**Document Type**的下拉框，然后选择Make
 File，在右边的**File Filter**中，在原先的*.mak后面加上一个分号，即**多个不同过滤规则以分号间隔开**，再加上*makefile，变成 *.mak;*makefile，并且选中**Include when adding to projects**,这样，以后再新建工程的时候，就可以识别makefile或Makefile了（好像此处Source Insight并不区分大小写）。
类似的原理，给其他你想要加入的不同的类型的文件，分别加入到原先的一些文件类型后面，注意要用分号隔开，或者直接新建一个文件类型，然后写上对应的顾虑规则，比如
点击 Add Type，填入新文件类型的名字Scatter File，File Filter中写上*.scf，注意再选中下面的Include when adding to projects,这样就建立了一个新的文件类型， 以后新建工程加入文件时候，系统就能够识别后缀是scf的文件了。
当然感兴趣的，还可以对你新建立的文件类型进行一些格式化设置。包括Parsing，Tab等等设置。


