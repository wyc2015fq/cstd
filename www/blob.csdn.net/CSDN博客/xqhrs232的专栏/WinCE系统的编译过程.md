# WinCE系统的编译过程 - xqhrs232的专栏 - CSDN博客
2013年01月23日 22:48:02[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：581
原文地址::[http://tech.e800.com.cn/articles/2009/54/1241414050579_1.html](http://tech.e800.com.cn/articles/2009/54/1241414050579_1.html)
在WinCE系统中，当我们完成了相关的开发和系统定制工作以后，会编译WinCE系统，最后生成NK.bin和NK.nb0。我现在用WinCE6.0 在自己的PC上面编译一次用时19分16秒(有一天无聊，就测了一下)。下面介绍一下WinCE系统的编译过程，大致分为4个阶段：编译阶段 (Compile phase)，系统产生阶段(Sysgen phase)，文件拷贝阶段(Release copy phase)和生成映像阶段(Make Run-time image phase)。
**   过程如图：**
![](http://www.icembed.com/UploadFiles/200898141229734.JPG)
       从图中可以看出，整个编译都是通过调用Blddemo.bat来实现的，实际上也是这样，编译WinCE就是用Blddemo.bat，只不过后面可以跟不同的参数。编译阶段和系统产生阶段由Cebuild.bat完成，文件拷贝阶段由Buildrel.bat完成，最后的生成映像阶段由 Makeimg.exe完成。下面介绍一下各个阶段。
**   1. 编译阶段(Compile phase)**
       这个过程指对WinCE路径下的Private和Public目录进行编译，将里面的源代码编译成库文件等，这个过程可能会花费几个小时。除非改动了 Private或Public目录下的源码，否则是不需要编译的。一般我们编译自己的工程的时候，不需要这个步骤。
**       2. 系统产生阶段(Sysgen phase)**
       在这个过程中，系统会根据你在PB中Catalog中的选项，删除和设置相应的SYSGEN环境变量，链接相应的静态库，过滤头文件，为SDK创建所需的导入库，同时还会为WinCE系统创建一些配置文件。最后会编译当前的BSP和应用程序。
** 3. 文件拷贝阶段(Release Copy phase)**
       拷贝所有所需的文件到工程的release目录下面，主要是前面系统产生阶段所产生的所有文件。
** 4. 生成映像阶段(Make run-time image phase)**
       该过程首先调用cenlscmp.exe根据国际语系与地区设定生成wince.nls文件。然后调用Fmerge.exe合并一些配置文件，合并所有 bib文件为ce.bib，合并所有的reg文件为Reginit.ini，合并所有的dat文件为Initobj.dat，合并所有的db文件为 Initdb.ini。之后调用Regcomp.exe压缩reginit.ini为default.fdf。根据LOCAL环境变量的设置调用 Res2exe.exe更新所有的dll，exe和cpl文件中的资源文件，主要是更新其中的语言部分。再调用Txt2ucde.exe转换所有
 ASCII码字符串为UNICODE。最后调用Romimage.exe合并所有文件为NK.bin。
        在整个编译过程中有时会遇到编译错误，这个可以通过察看Build.log，Build.err和Build.wrn文件来分析问题，这三个文件应该是位于”\WINCE600”目录下面。编译错误可能在不同的编译阶段产生，我们也可以根据这一点来分析问题。
         一般在系统产生阶段(Sysgen phase)出现错误很可能是由于丢失组件或文件造成的，这时候Build.log会提供更多信息帮助分析问题。在这个阶段产生错误，很可能是由于在当前工程中添加或者删除WinCE组件造成的，其中有些组件的更改是需要进行”clean sysgen”的，而不能只使用”sysgen”命令。所以我的建议是每次添加或删除组件都做”clean sysgen”。
         如上面介绍系统产生阶段(Sysgen phase)也会编译BSP和部分应用。所以如果错误出现在系统产生阶段的编译过程中，一般一种可能就是你的代码有语法错误，当然这种错误很好查。还有一种可能出现的错误是连接错误，有可能是丢失了lib库文件或者链接错了库文件，也有可能是调用了错误的API函数，还有就是设置了错误的环境变量，这些查起来相对困难一点。
         如果错误产生在文件拷贝阶段(Release copy phase)，一种常见的问题就是和硬盘驱动器有关，检查一下release目录所在磁盘的磁盘空间。
         如果问题出现在生成映像阶段(Make run-time image phase)，根据编译的输出窗口的输出信息判断问题出在哪个子阶段。一种可能是你的bib文件或者reg文件中存在语法错误导致合并文件时出错，还有就是注意你的image的大小是否超过了config.bib文件中的设定，还有就是可能丢失了某个或者某些文件，这些丢失的文件很可能是由于在前面的编译过程中出现错误导致的。
         当然，也有时候会遇到一些奇怪的问题，这些问题可能是由于没有正确的安装WinCE造成的，比如在安装WinCE的时候，路径中不要有中文或者空格或者其他比较奇怪的字符。补丁要按照顺序来打，要不也可能会出现问题。
