# WinCE系统的编译过程&快速编译(转载) - xqhrs232的专栏 - CSDN博客
2009年11月23日 12:10:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1226

在WinCE系统中，当我们完成了相关的开发和系统定制工作以后，会编译WinCE系统，最后生成NK.bin和NK.nb0。我现在用WinCE6.0在自己的PC上面编译一次用时19分16秒(有一天无聊，就测了一下)。下面介绍一下WinCE系统的编译过程，大致分为4个阶段：编译阶段(Compile phase)，系统产生阶段(Sysgen phase)，文件拷贝阶段(Release copy phase)和生成映像阶段(Make Run-time image phase)。
过程如图：
![](http://hiphotos.baidu.com/tommy19870903/pic/item/9efa503315a06363ad4b5fd0.jpg)
从图中可以看出，整个编译都是通过调用Blddemo.bat来实现的，实际上也是这样，编译WinCE就是用Blddemo.bat，只不过后面可以跟不同的参数。编译阶段和系统产生阶段由Cebuild.bat完成，文件拷贝阶段由Buildrel.bat完成，最后的生成映像阶段由Makeimg.exe完成。下面介绍一下各个阶段。
1. 编译阶段(Compile phase)
这个过程指对WinCE路径下的Private和Public目录进行编译，将里面的源代码编译成库文件等，这个过程可能会花费几个小时。除非改动了Private或Public目录下的源码，否则是不需要编译的。一般我们编译自己的工程的时候，不需要这个步骤。
2. 系统产生阶段(Sysgen phase)
在这个过程中，系统会根据你在PB中Catalog中的选项，删除和设置相应的SYSGEN环境变量，链接相应的静态库，过滤头文件，为SDK创建所需的导入库，同时还会为WinCE系统创建一些配置文件。最后会编译当前的BSP和应用程序。
3. 文件拷贝阶段(Release Copy phase)
拷贝所有所需的文件到工程的release目录下面，主要是前面系统产生阶段所产生的所有文件。
4. 生成映像阶段(Make run-time image phase)
该过程首先调用cenlscmp.exe根据国际语系与地区设定生成wince.nls文件。然后调用Fmerge.exe合并一些配置文件，合并所有bib文件为ce.bib，合并所有的reg文件为Reginit.ini，合并所有的dat文件为Initobj.dat，合并所有的db文件为Initdb.ini。之后调用Regcomp.exe压缩reginit.ini为default.fdf。根据LOCAL环境变量的设置调用Res2exe.exe更新所有的dll，exe和cpl文件中的资源文件，主要是更新其中的语言部分。再调用Txt2ucde.exe转换所有ASCII码字符串为UNICODE。最后调用Romimage.exe合并所有文件为NK.bin。
在整个编译过程中有时会遇到编译错误，这个可以通过察看Build.log，Build.err和Build.wrn文件来分析问题，这三个文件应该是位于”/WINCE600”目录下面。编译错误可能在不同的编译阶段产生，我们也可以根据这一点来分析问题。
一般在系统产生阶段(Sysgen phase)出现错误很可能是由于丢失组件或文件造成的，这时候Build.log会提供更多信息帮助分析问题。在这个阶段产生错误，很可能是由于在当前工程中添加或者删除WinCE组件造成的，其中有些组件的更改是需要进行”clean sysgen”的，而不能只使用”sysgen”命令。所以我的建议是每次添加或删除组件都做”clean sysgen”。
如上面介绍系统产生阶段(Sysgen phase)也会编译BSP和部分应用。所以如果错误出现在系统产生阶段的编译过程中，一般一种可能就是你的代码有语法错误，当然这种错误很好查。还有一种可能出现的错误是连接错误，有可能是丢失了lib库文件或者链接错了库文件，也有可能是调用了错误的API函数，还有就是设置了错误的环境变量，这些查起来相对困难一点。
如果错误产生在文件拷贝阶段(Release copy phase)，一种常见的问题就是和硬盘驱动器有关，检查一下release目录所在磁盘的磁盘空间。
如果问题出现在生成映像阶段(Make run-time image phase)，根据编译的输出窗口的输出信息判断问题出在哪个子阶段。一种可能是你的bib文件或者reg文件中存在语法错误导致合并文件时出错，还有就是注意你的image的大小是否超过了config.bib文件中的设定，还有就是可能丢失了某个或者某些文件，这些丢失的文件很可能是由于在前面的编译过程中出现错误导致的。
当然，也有时候会遇到一些奇怪的问题，这些问题可能是由于没有正确的安装WinCE造成的，比如在安装WinCE的时候，路径中不要有中文或者空格或者其他比较奇怪的字符。补丁要按照顺序来打，要不也可能会出现问题。
--------------------------------------------------------
这是很郁闷的事情，真的不知道微软是怎么回事的，我有时候在msdn的虚拟实验室编译还是会照样出问题的，没有致命错误也会有n个小问题提示的
--------------------------------------------------------
在WinCE下面，创建工程，然后编译是需要花很长时间的。当我们第一编译完工程以后，如果需要改我们的驱动或者BSP，我们是不需要重新编译整个工程的：
1. 如果更改了driver，以WinCE6.0为例，我们可以在菜单里面选择“build”->“Open Release Directory in Build Window” ，然后在弹出的命令行窗口中，通过dos命令切换到你要编译的driver的目录下面，然后运行“build”就可以了。也可以运行“build -c”会强制把所有的文件都编译一遍。被编译后的driver的dll会被自动拷贝到release目录下面，然后再切换的工程的release目录下面，运行一下“make image”就可以了。
2. 如果改变了OAL部分的代码，同样用上面介绍的方法，需要注意的是，OAL部分可能包含多个文件夹，如果改变了OAL里面的代码，不要进入OAL里面的文件夹去编译，一定要在OAL这层进行编译，这样OAL部分的lib，dll才会被重新编译并拷贝到release目录下面。
3. 如果改变了配置文件，比如config.bib，platform.reg文件，那么直接将这些文件拷贝到你的工程目录下面，然后运行一下“make image”就可以了。
4. 如果改变了eboot部分的代码，那按照步骤1的方法就可以了，可能你都不需要运行“make image”命令，因为可能你只需要eboot.bin或者eboot.nb0。
