# CodeBlocks安装及配置编译器问题 - 心纯净，行致远 - CSDN博客





2018年11月30日 22:14:32[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：67标签：[C																[C  																[code：blocks](https://so.csdn.net/so/search/s.do?q=code：blocks&t=blog)
个人分类：[【资源工具】](https://blog.csdn.net/zhanshen112/article/category/7500595)





心血来潮复习C语言，放下两年的东西再捡起来还是有点困难，甚至从安装环境就开始出问题。为与其他菜鸟级别玩家共同学习，特开此贴将自己遇到的问题和解决方法全部记录下来，以吸取教训。

1. 下载安装包

      习惯了一键式傻瓜安装：百度搜索官网，官网直接点击download，下载界面直接选择第一条，下载完安装包进入安装界面疯狂下一步下一步。看似完成了codeblocks的安装，但在建立newproject时发现居然“hello world”都蹦不出来？仔细一看窗口下方的报错：Tried to run compiler executable '-- No Compiler --/bin/', but failed! 顿时感觉智商受到嘲讽，启动“无编译器”？于是我又跑到官网仔细看了一下下载界面下的NOTE，只见最后一行赫然写道：If unsure, please use codeblocks-17.12mingw-setup.exe! 翻译过来就是说：你要是什么都不会，你就下载第四排（注意一定是带mingw的，注意一定是带mingw的，注意一定是带mingw的！）的可执行程序（.exe）再猛点下一步！

       官网提供了windows系统下的六种安装包，其中第四种是直接将GCC/G++ compiler和GDB debugger跟着安装下来的，我们直接下载如下所示的加粗字样的安装包猛点下一步就可以了。



2. 配置编译器

      重新安装之后还要修改一下编译器的路径，因为codeblocks默认路径是C:\MinGW，但跟着codeblocks安装的编译器实际是在它自己的目录下的，所以我们要点击"Settings->Compiler...->Global compiler settings->GNU GCC Compiler->Toolchain executables"修改编辑器路径。

       无奈改完之后仍然发现报错（！）：



       这时可能是因为compiler settings下的compiler flags选择了Target x86_64(64 bit) [-m86]，把它改选成Target x86 (32bit)就解决了。



3. Build and Run



成功！

---------------------

作者：李橡树

来源：CSDN

原文：https://blog.csdn.net/weixin_42507051/article/details/80909349

版权声明：本文为博主原创文章，转载请附上博文链接！](https://so.csdn.net/so/search/s.do?q=C  &t=blog)](https://so.csdn.net/so/search/s.do?q=C&t=blog)




