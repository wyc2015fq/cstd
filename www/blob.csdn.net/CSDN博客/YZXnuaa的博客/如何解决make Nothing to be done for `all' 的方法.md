# 如何解决make: Nothing to be done for `all' 的方法 - YZXnuaa的博客 - CSDN博客
2018年05月14日 09:31:37[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：229
个人分类：[ubuntu操作](https://blog.csdn.net/YZXnuaa/article/category/7388698)
其实嘛，出现这个的原因在于，已经编译过啦，没有任何改动，那还烦劳编译啥呢。
那Linux又是如何知道已经编译过了呢？
那就要看makefile的规则啦。makefile的规则是所想产生的文件需要依赖很多 .o文件。若文件没有改动，.o文件也没有改动，则Linux认为，我不需要对所有的文件做任何事情。
那如何让make重新编译源文件呢？
有时候，因为系统的不同，导致运行库版本不同，则需要重新编译源文件。方法如下：
>make clean（清除上次make命令所产生的object文件（后缀为“.o”的文件）及可执行文件。）
>ldconfig  （该命令通常在系统启动时运行，确保动态链接库为系统所共享。当用户安装了一个新的动态链接库时，则需手工运行该命令。）
>make（执行makefile文件）
这样就能够重新编译
