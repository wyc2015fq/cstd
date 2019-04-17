# Linux多版本gcc共存 - 李鑫o_O - CSDN博客





2017年05月18日 17:09:32[hustlx](https://me.csdn.net/HUSTLX)阅读数：1638








                
先看看我们系统用的gcc和g++是什么版本

gcc -v

sudo apt-get install gcc-4.5 gcc-4.5-multilib g++-4.5 g++-4.5-multilib

安装好后输入以下指令：

sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.4 50

sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.5 40

接着输入：

sudo update-alternatives --config gcc

会看到如下的选项

有 3 个候选项可用于替换 gcc (提供 /usr/bin/gcc)。

  选择      路径            优先级  状态

------------------------------------------------------------

* 0            /usr/bin/gcc-4.4  50        自动模式

  1            /usr/bin/gcc-4.4  50        手动模式

  2            /usr/bin/gcc-4.5  40        手动模式

  3            /usr/bin/gcc-4.6  30        手动模式

要维持当前值[*]请按回车键，或者键入选择的编号：

要想用哪个gcc就输入编号吧。

同样也要设置一下g++的

sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.4 50

sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.5 40

如果想删除可选项的话可以键入以下指令：

sudo update-alternatives --remove gcc /usr/bin/gcc-4.5

再用gcc -v 来看一下版本是否改变了。
            


