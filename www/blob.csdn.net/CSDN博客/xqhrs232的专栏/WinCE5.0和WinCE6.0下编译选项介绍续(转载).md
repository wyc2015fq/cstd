# WinCE5.0和WinCE6.0下编译选项介绍续(转载) - xqhrs232的专栏 - CSDN博客
2009年11月23日 12:08:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1480
  目前，在国内Windows CE的开发可以说是一种Windows CE.NET 4.2、Windows CE.NET 5.0、Windows Embedded CE 6.0多版本共存的情况。由于Windows CE.NET 4.2成熟的技术、丰富的I/O设备驱动以及被大多数的开发人员所熟悉，所以目前仍是嵌入式产品开发的主流。Windows CE.NET 5.0是CE 4.2的升级版。Windows Embedded CE 6.0是目前微软最新的嵌入式操作系统，强大的操作系统功能，于桌面开发系统开发工具的有效集成，以及与Windows
 CE.NET 5.0及其以前版本在开发技术上的一致性，都必将使它成为下一代智能设备的主流操作系统。所以，在学习Windows CE.NET 5.0的编译方法时，也不忘兼容的学习一下Windows Embedded CE 6.0的。以下是一篇转载文章：
      在WinCE5.0中，在"Build OS”菜单中，我们会看到不同的编译选项：
　　"Sysgen"，"Build and Sysgen"和"Build and Sysgen current BSP"
　　Sysgen：不用多说，当你在"Catalog"中添加或删除了新的item的时候，就用这个吧。
　　Build and Sysgen：当你更新了public目录下的源代码的时候，你就需要用这个了。一般比如在打patch以后，可能就需要进行Build and Sysgen了。
　　Build and Sysgen current BSP：当你只改变了你的BSP部分的代码，就可以用这个选项。据说，当你改变了platform目录下的代码，也可以用这个，具体没有试过。
========================================================================
**Platform Builder 5.0的Build OS菜单详解：**
很多新手对Platform. Builder 5.0（以下简称PB 5.0）的Build OS菜单有所疑惑，不知道实际使用中应该如何选择，在此结合本人平时的经验，略作一番解释。
    首先第一组是编译命令，一共有三个：
    1.Sysgen
     根据当前的系统[**设计**]()，生成一个系统映像。该操作不会更改之前编译（Build）完成的文件。这里所说的文件，指的是OS的文件，也就是位于WINCE500文件夹下面的各种lib库。
     有三点是必须要注意的：
     1）.在上一次调用Sysgen之后，如果你已经增加或移除文件以及相关的Catalog，强烈建议选中“Clean Before Building"，然后再开始新的一次Sysgen。因为这样会最大程度上减少出错的机会。
     2）.该命令也会编译包含于系统设计工作区（Workspace）的工程（Project）。
     3）.没有任何文件拷贝到目标（Release）文件夹。
    2. Build and Sysgen
     其实这是两个命令的组合，先是编译（Build），然后再调用Sysgen。该命令不仅编译公开（Public）代码，甚至连私有（Private）代码也不放过，所以耗费的时间会非常久。只有当你更改了OS的源文件，才建议选择该命令。    
     该命令和之前的Sysgen相同，也会编译包含于系统设计工作区（Workspace）的工程（Project）以及不将文件拷贝到目标（Release）文件夹。
**接下来三个是命令标签，用来部分控制命令运行时的行为：**
    3.Build and Sysgen Current BSP 
    编译（Build）然后Sysgen当前的BSP包。和之前的两个命令一样，该命令也不会将文件拷贝到目标（Release）文件夹。
    需要注意的一点是，根据实际的使用经验，因为命令的参数不同，有时候即使你能成功调用调用Sysgen生成系统，也有可能调用本命令编译BSP包失败。遇到此情况，只能根据具体情况做相应的处理。
    4.Clean Before Building
     如果选中该标签，那么在执行命令之前都会清除上一次Sysgen创建的文件。
    5.Copy Files to Release Directory
     如果选中该标签，则在编译完毕之后，会把生成的文件拷贝到目标（Release）文件夹中。通常这个目标（Release）文件夹是你的工程所在的目录。
     该标签默认是选中的。
    6.Make Run-Time Image After Build
     如果选中该标签，在编译完毕之后，将生成一个运行映像（run-time image）。最典型的的运行映像（run-time image）便是NK.BIN。
     在你建立一个新的工程之后，该标签默认是选中的。
**最后是一组辅助的命令：**
     7.Copy Files to Release Directory
      将生成的文件拷贝到目标（Release）文件夹。
     8.Make Run-Time Image
      生成运行映像（Run-Time Image）。最常见的，也就是将所编译生成的文件组合成nk.bin。
     9.Batch Build
      用来选择多种配置来生成运行映像（Run-Time Image）。
     10.Open Release Directory
      打开命令行窗口，默认路径为目标（Release）文件夹。
     11.Set Active Configuration
      从现有的配置选择一个作为当前可用的。
     12.Configurations
      你可以通过该命令增添或删除配置。
     在PB的文档中也有关于这些命令的说明，具体位置你可以查看： Platform. Builder User''s Guide > Platform. Builder IDE > Platform. Builder User Interface > Menu Descriptions 
========================================================================
　　在WinCE6.0中，在VS2005的"Build"菜单中，有一个“Advanced Build Commands”，其中有很多编译选项：
　　"Sysgen"：相当于执行命令"blddemo -q”，一般第一次编译或者是改变了"Catalog"中的item的时候，就用这个了。
　　"Clean Sysgen"：相当于执行命令"blddemo clean -q"，按照文档上的说明，当修改了 %_WINCEROOT%PublicCEBASEOAKMiscCesysgen.bat的时候，或者改变了以SYSGEN，BSP为前缀的环境变量的时候，需要使用这个来编译。
　　在此我说一下我个人的经验，一般只有第一次创建完工程的时候，我会用"Sysgen"命令，以后只要是改变了SYSGEN为前缀的环境变量的设置或者是"Catalog"中的item，我就会使用"Clean Sysgen"，而改变了以BSP为前缀的环境变量要看具体情况，也不一定就要用"Clean Sysgen"。继续...
　　"Build and Sysgen"：相当于执行命令"blddemo"，当改变了public目录下的代码，比如你打了WinCE的patch，你就需要用这个了。
　　"Rebuild and Sysgen Clean"：相当于执行命令"blddemo clean cleanplat -c"，相当于清除上一次编译生成的文件，然后重新编译public目录和你的工程。
　　"Build and Sysgen Current BSP"：相当于执行命令"blddemo -qbsp"，仅编译platform目录下的代码。所以当改变了platform目录下的代码的时候或者说改变了BSP的代码的时候，可以用这个来编译。
　　"Rebuild and Clean Sysgen Current BSP"：相当于执行命令"blddemo -qbsp -c"，相当于完全重新编译platform目录下要编译的代码。
