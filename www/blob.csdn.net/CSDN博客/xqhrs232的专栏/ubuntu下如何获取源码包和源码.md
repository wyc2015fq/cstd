# ubuntu下如何获取源码包和源码 - xqhrs232的专栏 - CSDN博客
2017年03月08日 11:02:20[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：271
原文地址::[http://blog.csdn.net/yuanlu837/article/details/7750492](http://blog.csdn.net/yuanlu837/article/details/7750492)
相关文章
1、[Linux下获取软件源码的几种方法](http://blog.csdn.net/u014717036/article/details/51916767)----[http://blog.csdn.net/u014717036/article/details/51916767](http://blog.csdn.net/u014717036/article/details/51916767)
如何获取ubuntu源码包里面的源码？
1、在获取源码包之前，确保在软件源配置文件/etc/apt/sources.list中添加了deb-src项
2、使用如下命令获取xxx源码包的详细信息:
 sudo apt-cache showsrc xxx 这用来查询当前镜像站点中是否有该源码包。
3、源码包中通常包含3个文件，分别以dsc，orig.tar.gz和diff.gz为后缀名。
 sudo apt-get source  xxx    命令来获取源码包，它会将源码包下载到用户当前目录
并在命令执行过程中，调用dpkg-source命令，根据dsc文件中的信息，将源码包解压到同名目录中，应用程序的源代码就在这里面。
sudo apt-get source xxx
要强调的是，在下载源码包前，必须确保安装了dpkg-dev（执行”apt-get install dpkg-dev”来安装），
否则，只会下载源码包的3个文件，但不会解压缩源码包。当然你也可以自己用dpkg-source命令去解压缩源码包。
4、在编译源码包前，需要安装具有依赖关系的相关软件包。使用”apt-get build-dep”命令可以主动获取并安装所有相关的软件包。
sudo apt-get build-dep xxx
5、现在可以来编译源码包了，首先进入源码所在目录，使用dpkg-buildpackage命令来编译源码包，它会将生成的Deb软件包放置在上层目录中。
cd xxx
sudo dpkg-buildpackage
这样就会编译生成xxx_i386.deb
6、安装软件包。使用”dpkg –i”命令来安装生成的Deb软件包。
sudo dpkg -i  xxx_i386.deb
//================================================================================================
备注::
[hpsystem@localhost test_helloworld]$ rpm -qi glibc 
Name        : glibc                        Relocations: (not relocatable) Version     : 2.12.90                           Vendor: Fedora Project Release     : 17                      
      Build Date: Tue 19 Oct 2010 06:33:22 PM CST
Install Date: Tue 25 Mar 2014 10:46:01 PM CST      Build Host: x86-15.phx2.fedoraproject.org
Group       : System Environment/Libraries   Source RPM: glibc-2.12.90-17.src.rpm
Size        : 13980110                         License: LGPLv2+ and LGPLv2+ with exceptions and GPLv2+
Signature   : RSA/SHA256, Wed 20 Oct 2010 06:12:35 AM CST, Key ID 421caddb97a1071f
Packager    : Fedora Project URL         : http://www.gnu.org/software/glibc/ 
Summary     : The GNU libc libraries Description : The glibc package contains standard libraries which are used by multiple programs on the system. In order to save disk space and memory,
 as well as to make upgrading easier, common system code is kept in one place and shared between programs. This particular package contains the most important sets of shared libraries: the standard C library and the standard math library. Without these two
 libraries, a Linux system will not function.

