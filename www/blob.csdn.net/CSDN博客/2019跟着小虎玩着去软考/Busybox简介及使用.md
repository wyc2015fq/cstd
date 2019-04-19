# Busybox简介及使用 - 2019跟着小虎玩着去软考 - CSDN博客
2011年08月03日 17:37:05[littletigerat](https://me.csdn.net/littletigerat)阅读数：1820标签：[linux内核																[linux																[http服务器																[applet																[build																[工具](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=build&t=blog)](https://so.csdn.net/so/search/s.do?q=applet&t=blog)](https://so.csdn.net/so/search/s.do?q=http服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=linux内核&t=blog)
个人分类：[Linux																[Android																[手机开发](https://blog.csdn.net/littletigerat/article/category/649495)](https://blog.csdn.net/littletigerat/article/category/613910)](https://blog.csdn.net/littletigerat/article/category/613914)
**Busybox简介及使用**
# 一．busybox简介 
　　busybox是一个集成了一百多个最常用linux命令和工具的软件,他甚至还集成了一个http服务器和一个telnet服务器,而所有这一切功能却只有区区1M左右的大小.我们平时用的那些linux命令就好比是分立式的电子元件,而busybox就好比是一个集成电路,把常用的工具和命令集成压缩在一个可执行文件里,功能基本不变,而大小却小很多倍,在嵌入式linux应用中,busybox有非常广的应用,另外,大多数linux发行版的安装程序中都有busybox的身影,安装linux的时候案ctrl+alt+F2就能得到一个控制台,而这个控制台中的所有命令都是指向busybox的链接.
　　Busybox的小身材大作用的特性,给制作一张软盘的linux带来了及大方便.
# 二．busybox的用法
　　可以这样用busybox
　　#busybox ls
　　他的功能就相当运行ls命令
　　最常用的用法是建立指向busybox的链接,不同的链接名完成不同的功能.
　　#ln -s busybox ls
　　#ln -s busybox rm
　　#ln -s busybox mkdir
　　然后分别运行这三个链接:
　　#./ls
　　#./rm
　　#./mkdir
　　就可以分别完成了ls rm 和mkdir命令的功能.虽然他们都指向同一个可执行程序busybox
　　但是只要链接名不同,完成的功能就不同,busybox就是这么的神奇.
　　很多linux网站都提供busybox的源代码下载.目前版本是busybox1.0正式版.
# 三．配置busybox
　　busybox的配置程序和linux内核菜单配置方式简直一模一样.熟悉用make menuconfig方式配置linux内核的朋友很容易上手.
　　#cp busybox-1.00.tar.gz /babylinux
　　#cd /babylinux
　　#tar xvfz busybox-1.00.tar.gz
　　#cd busybox-1.00
　　#make menuconfig
　　下面是需要编译进busybox的功能选项,其他的可以根据需要自选,但是同样不要太贪心.
　　General Configuration应该选的选项
　　Show verbose applet usage messages
　　Runtime SUID/SGID configuration via /etc/busybox.conf
　　Build Options
　　Build BusyBox as a static binary (no shared libs)
　　这个选项是一定要选择的,这样才能把busybox编译成静态链接的可执行文件,运行时才独立于其他函数库.否则必需要其他库文件才能运行,在单一个linux内核不能使他正常工作.
　　Installation Options
　　Don't use /usr
　　这个选项也一定要选,否则make install 后busybox将安装在原系统的/usr下,这将覆盖掉系统原有的命令.选择这个选项后,make install后会在busybox目录下生成一个叫_install的目录,里面有busybox和指向他的链接.
　　其他选项都是一些linux基本命令选项,自己需要哪些命令就编译进去,一般用默认的就可以了.
　　配置好后退出并保存.
# 四．编译并安装busybox
　　#make
　　#make install
　　编译好后在busybox目录下生成子目录_install,里面的内容:
　　drwxr-xr-x 2 root root 4096 11月 24 15:28 bin
　　lrwxrwxrwx 1 root root 11 11月 24 15:28 linuxrc -> bin/busybox
　　drwxr-xr-x 2 root root 4096 11月 24 15:28 sbin
　　其中可执行文件busybox在bin目录下,其他的都是指向他的符号链接
            
