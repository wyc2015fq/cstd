# Emacs-060-树莓派安装Emacs 25.3.1 - 小灰笔记 - CSDN博客





2018年01月16日 00:13:32[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：529








                
Emacs-060-树莓派安装Emacs 25.3.1
在一台Linux PC机上安装了Emacs并且远程ssh登录使用，感觉用着还比较顺手。寻思我自己的树莓派上应该也可以这样操作，于是把自己的Emacs配置全都传到了树莓派。然而使用的时候发现配置不能用，反复确认后感觉是Emacs版本太低导致。树莓派使用apt-get直接安装的Emacs版本是23，而我自己常用的是一个比较新的。
找了一下如何继续apt-get安装最新版本或者把现在的Emacs 23升级到更新的版本，没有找到相应的方案。于是转向GNU Emacs的官网，这里给出的功法定然是玄门正宗了！看了一下，没有找到Linux下安装好直接可以用的软件包，源代码倒是提供了好几份。于是找来源代码自己编译安装，在configure执行的时候报出了很多错误。针对错误，需要安装部分其他的依赖。我自己找了一部分，网络顺手一搜又发现了一个别人整理的更全的。直接借用了过来，具体如下：

sudo apt-get install texinfo libncurses5-dev

sudo apt-get install xorg-dev libpng12-dev libjpeg8-dev libgif-dev libtiff5 libtiff5-dev
之后，按照说明自己编译安装：

1.运行configure，这回检查配置并生成makefile

2.make

3.make install

说起来，我还是省略了一些更加精细的步骤。其实，上面的操作也不是一次性就通过了，中间出现了几次脚本问题，简单的设置处理之后通过。

最后，Emacs 最新版本安装成功。第一次启动的时候，很多elisp脚本又执行了编译。从第二次启动，一切正常。

树莓派毕竟还是个小东西，CPU的性能确实是有限。整个编译以及问题处理耗费了我大约1小时20分钟的时间，其中80以上的时间是在编译。问题的定位处理时间分长短，关键时刻性能还是有用的。另外，最新版本的Emacs在树莓派上的运行并不是那么流畅，不知道是因为软件规模大有变化还是Emacs 23的版本针对树莓派进行了专门的优化？不过，软件总算是安装好了，这也不是一个开发使用的助理平台，纯粹就是个折腾而已，图个乐！

最后贴一个我的树莓派远程使用Emacs的截图：

![](https://img-blog.csdn.net/20180116001326284?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




