
# R︱Linux+Rstudio Server尝鲜笔记（打造最佳Rstudio体验+报错的解决方案） - 素质云笔记-Recorder... - CSDN博客

2016年11月29日 12:17:06[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：5083



Rstudio Server 是Rstudio开发的基于R语言的网页版（只能在Linux），你在手机上都可以运行R，还是挺方便的。就是配置起来有点麻烦。
官方下载链接：https://www.rstudio.com/products/rstudio/download-server/

本文列举了8个报错提醒以及如何将本机与Rstudio Server实现交互。

————————————————————————————————

1、为啥笔者觉得Rstudio Server适合Linux？

如果你有Linux桌面系统，且你可以自由操作自己家公司的服务器当然直接来个Rstudio也是不错的选择，但是呢，如果服务器公用，且没有桌面系统，且你喜欢轻量化的Rstudio。我觉得，你没有理由不喜欢Rstudio Server。


2、RStudio Desktop 与 RStudio Server 有什么区别？
RStudio Desktop 是一个 R IDE，与您已安装在您的本地 Windows，Mac OS X 或 Linux 工作站上的 R 配合工作。RStudio 桌面版是一个独立的桌面应用程序，不需要以任何方式连接到 RStudio 服务器版。
RStudio Server 是一个 Linux 服务器上的应用程序，提供了在服务器上运行的 R 环境，而用户通过浏览器来访问。
参考：https://github.com/wangyuchen/rserver-manual/blob/master/01_r_and_rstudio-server.md
————————————————————————————————

# 一、Rstudio Server安装

艾玛，不知道的请戳我的另外一篇博客：

# [网页版Rstudio︱RStudio Server多人在线协作开发](http://blog.csdn.net/sinat_26917383/article/details/50681156)

当然，Rstudio Server的教程博客只服张丹老师的博客。
————————————————————————————————

# 二、遇见的问题

#### 1、问题一：安装包时候报错：couldn't resolve host name
报错：
`Warning in install.packages    status was 'Couldn't resolve host name'`
修改CRAN mirror的不同路径，我是用china北京、合肥以及globe等都试了一下，某一个会显灵的。

#### 2、问题二：安装包时报错:package "xxx" is not available
代表着你的R版本有点低，你得升级一下，当然这个问题也会出现在windows系统中。
`Warning in install.packages :
  package ‘reshape’ is not available (for R version 3.1.1)`

#### 3、问题三：安装包时报错：had non-zero exit status

这个报错代表着，你安装的这个包，需要一些基础的包，这些基础包好像没按上。所以你得看看你的包还需要其他的什么包，一个一个试试，自己安装一下。
这个问题在windows也会出现，但是呢，我怎么感觉linux上这样的报错更多？
难道linux衍生包安装这一步被忽略了吗？

`rstudio server installation of package ‘wordcloud’ had non-zero exit status`
还有这样的报错：

`Warning in install.packages :
dependencies ‘highlight’, ‘parser’ are not available
also installing the dependency ‘formatR’`
#### 4、问题四：如何在Rstudio server加装rJava包
Rstudio server是搭载在linux之上的，rJava在Rstudio server 下载，需要先在Linux下安装JDK（Java环境），然后才能安装rJava，相关可参考我的博客：[R语言·文本挖掘︱Rwordseg/rJava两包的安装（安到吐血）](http://blog.csdn.net/sinat_26917383/article/details/50867851)
同时可能的报错：
`"Java configuration variables are not set"`

#### 5、问题五：Rtools的问题
好像不用下载就自己解决了，之前在windows中，如何要下载github中的包就必须要过rtools这关。

#### 6、问题六：.bin文件读入问题

`Error in readChar(a, 1) : invalid UTF-8 input in readChar()`
只能先导入到workspace里面才能继续使用。如果是大型的词向量内容，可能需要内存比较大的机器，同时注意memory.limit()内存，让本机给R的内存变大。（可参考：
# [R语言︱大数据集下运行内存管理](http://blog.csdn.net/sinat_26917383/article/details/51114265)
）
笔者是在windows分析出现的bin文件，然后导入Linux服务器然后再想读入Rstudio server，看来是windows下面分析的问题了。
特别是词向量的问题，如果是python训练的词向量，格式会发生改变，所以读入linux会出现如下报错，也没有特别好的解决办法。

#### 7、问题七：
#### require和
#### library的区别？

用library(pkg)或require(pkg)加载包时，R会到.libPaths()的路径下去搜索，如果该库不存在就会报错。library()和require()的最大区别：找不到包时，library()抛出错误，require()打印警告信息并返回FALSE。


#### 8、问题八：Rstudio server可以共享多人，那么一个人安装了所有的包，其他也可以用吗？

Rstudio server设置管理权限的时候就是分开目录，自己用自己目录下的安装包，所以不是共享的。

解决方案：
(1)当然也可以设置成所有人共享，安装包的时候就设置
`install.packages("ggplot2","/usr/local/lib64/R/library")`

(2)或者管理员自己将一个人的library复制到其他的目录之中。

在rstudio下，安装包时，默认的库路径是"/home/enn_james/R/x86_64-unknown-linux-gnu-library/3.2"，而R软件本身的默认路径是/usr/local/lib64/R/library

————————————————————————————————————

# 三、
# 如何导入本地的文件/代码至Rstudio Server之中

Rstudio Server是搭建在Linux之中的，虽然可以将本地代码复制到Rstudio Server，但是对于文件、数据源来说，不能直接拖进去。
所以，笔者的解决办法就是找到Linux系统中rstudio server根目录，然后导入文件到这个根目录就可以使用了。
一篇博客有提到：
## 1、查看库文件路径

在rstudio窗口，查看库文件的路径：

`> .libPaths()
[1] "/home/enn_james/R/x86_64-unknown-linux-gnu-library/3.2" "/usr/local/lib64/R/library"`
但是/usr/local/lib64/R/library这一路径的组权限默认是”staff”的，因此想要安装到这个目录还有赋予权限才行。


## 2、查看包安装路径
在R的命令窗口输入

`library()`
可以看到全部的包，分为系统包和用户包，用户包在"/home/enn_james/R/x86_64-unknown-linux-gnu-library/3.2"，系统包在 "/usr/local/lib64/R/library"。

`[root@hadoop1-mysql /]#  sudo ls -all /usr/local/lib64/R/library
总用量 172
drwxr-xr-x 43 root root 4096 6月   1 07:52 .
drwxr-xr-x 10 root root 4096 5月  26 05:06 ..
drwxr-xr-x  7 root root 4096 5月  26 05:06 base
drwxr-xr-x  8 root root 4096 5月  26 05:06 boot
drwxr-xr-x  8 root root 4096 5月  26 05:06 class
drwxr-xr-x  9 root root 4096 5月  26 05:06 cluster`可见，/usr/local/lib64/R/library下的包是root权限的，是全局的；而/home/enn_james/R/x86_64-unknown-linux-gnu-library/3.2下的包的权限不是root权限的。
`[root@hadoop1-mysql /]#  sudo ls -all /home/enn_james/R/x86_64-unknown-linux-gnu-library/3.2
总用量 152
drwxr-xr-x 38 enn_james enn_james 4096 6月   2 03:02 .
drwxr-xr-x  3 enn_james enn_james 4096 5月  26 08:18 ..
drwxr-xr-x  7 enn_james enn_james 4096 5月  27 00:38 assertthat
drwxr-xr-x  7 enn_james enn_james 4096 5月  28 06:43 base64enc
drwxr-xr-x  6 enn_james enn_james 4096 5月  27 00:38 BH
drwxr-xr-x  7 enn_james enn_james 4096 5月  27 10:05 bitops
drwxr-xr-x  6 enn_james enn_james 4096 5月  27 10:13 brew`
本节参考：rstudio server包的安装位置



