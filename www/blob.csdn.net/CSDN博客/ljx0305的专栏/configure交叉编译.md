# configure交叉编译 - ljx0305的专栏 - CSDN博客
2012年11月20日 14:38:22[ljx0305](https://me.csdn.net/ljx0305)阅读数：1855
                
交叉编译某个应用程序时，通常需要 ./configure 来生成对应的 Makefile
./configure 最关键的三个选项是：
--host=HOST
指定软件运行的系统平台.如果没有指定,将会运行`config.guess'来检测.
--build=BUILD
指定软件包安装的系统平台.如果没有指定,默认值将是'--host'选项的值.
--target=GARGET
指定软件面向(target to)的系统平台.这主要在程序语言工具如编译器和汇编器上下文中起作用.如果没有指定,默认将使用'--host'选项的值. 
一般而言，我们只要指定 --host 就可以了
记住：--host 指定的是交叉编译工具链的前缀
##################################################################################################################################
在 i686 开发机上交叉编译出 ethtool，让其在powerpc开发板上运行
1.下载源代码并解压
#cd /home/wanghui/
#tar xvfz ethtool-6.tar.gz
#cd ethtool-6
2.交叉编译
确保交叉编译工具链的bin文件在PATH环境变量里
#echo $PATH
/usr/local/bin:/bin:/usr/bin:/opt/montavista41/montavista/cge/devkit/ppc/85xx/bin/:/home/wanghui/bin
#./configure --host=ppc-linux
#make
3.查看生成文件
#file ethtool
ethtool: ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), for GNU/Linux 2.2.5, dynamically linked (uses shared libs), not stripped
shit! 交叉编译失败，肿么还是X86的bin文件，肯定是configure出了问题，导致Makefile用的不是交叉编译工具链的gcc
4.查看config.log
有这么一句：
configure:1790: checking for ppc-linux-gcc
configure:1819: result: no
configure:1828: checking for gcc
configure:1844: found /usr/bin/gcc
我擦，没找到ppc-linux-gcc
cd /opt/montavista41/montavista/cge/devkit/ppc/85xx/bin
到里面一看，崩溃了，原来是 ppc_85xx-gcc,所有bin文件前缀是 ppc_85xx
5.重新交叉编译
#./configure --host=ppc_85xx
#make
6.查看生成文件
#file ethtool
ethtool: ELF 32-bit MSB executable, PowerPC or cisco 4500, version 1 (SYSV), for GNU/Linux 2.4.17, dynamically linked (uses shared libs), not stripped
查看cofig.log
configure:1662: checking for ppc_85xx-strip
configure:1678: found /opt/montavista41/montavista/cge/devkit/ppc/85xx/bin//ppc_85xx-strip
configure:1688: result: ppc_85xx-strip
configure:1757: checking whether to enable maintainer-specific portions of Makefiles
configure:1766: result: no
configure:1790: checking for ppc_85xx-gcc
configure:1806: found /opt/montavista41/montavista/cge/devkit/ppc/85xx/bin//ppc_85xx-gcc
configure:1816: result: ppc_85xx-gcc
7.把ethtool文件放到开发板下运行，OK
