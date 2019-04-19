# 解决libtinfo.so.5  libstdc++.so.5 的问题！ - 三少GG - CSDN博客
2010年05月01日 23:55:00[三少GG](https://me.csdn.net/scut1135)阅读数：15384标签：[library																[file																[deprecated																[object																[path																[symlink](https://so.csdn.net/so/search/s.do?q=symlink&t=blog)](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=deprecated&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)
个人分类：[开源硬件Raspberry](https://blog.csdn.net/scut1135/article/category/712785)
出现的问题解决方法：
> libtinfo seems to be deprecated and the functions inside that lib has been moved to core libncurses.
solution for this is to symlink libtinfo.so.5 -> libncurses.so
by doing: 
- **sudo ln -s /lib/libncurses.so.5 /usr/lib/libtinfo.so.5                   ---只需要这么一句话！原因参考下文转载**
*复制代码*
> @localhost:~$ asy
Welcome to Asymptote version 1.55 (to view the manual, type help)
> 
> 
有益参考资料如下：
**http://zhidao.baidu.com/question/66095681.html**
**1.linux执行脚本文件的时候报错！libtinfo.so.5在哪有下载？**
**![](http://img.baidu.com/img/iknow/icn_point.gif)**悬赏分：20 -
解决时间：2008-8-29 16:15
usr/screen-4.0.2/bin/screen-4.0.2: error while loading shar
ed libraries: libtinfo.so.5: cannot open shared object file: No such file or direc
tory 
提问者： [p463593995](http://passport.baidu.com/?business&aid=6&un=p463593995#2) - [二级](http://www.baidu.com/search/zhidao_help.html#%E5%A6%82%E4%BD%95%E9%80%89%E6%8B%A9%E5%A4%B4%E8%A1%94)
最佳答案
现在的广告啊~~！
应该是缺少函数库文件的问题，至于它所处的位置应该是/usr/lib，当然，前提是你安装了它。
其实在安装系统的时候，如果安装了必要的组件的话，应该是拥有很全的函数库的，缺失的话，当然可以通过补充安装组件来解决，但是，经常因为我们不知道是缺少哪一个组件，而无法进行。
但是其实可以直接下载函数库，放在/usr/lib下。
[http://rpmfind.net/linux/rpm2html/search.php?query=libtinfo.so.5](http://rpmfind.net/linux/rpm2html/search.php?query=libtinfo.so.5)

这个是一个网址，提供了那个函数库的rpm包的下载，安装那个rpm包应该就可以了，不需要手动设置别的东西。
但是这个网址提供的版本比较少，都是fedora的，不知道你的能用不？如果你的系统不是fedora，也可以尝试一下，当然，那就不一定成功了 
**2. http://biaoming.javaeye.com/blog/155194**
### [解决libstdc++.so.5问题](http://biaoming.javaeye.com/blog/155194)
error while loading shared libraries: libstdc++.so.5: cannot open shared object file: No such file or directory
apt-get install libstdc++5 
##### 评论
1 楼 [Sev7en_jun](http://learning.javaeye.com/) 2009-03-14   [引用](http://biaoming.javaeye.com/blog/155194)
解决办法1: 在下面包里有 libstdc++.so.5 然后复制到/usr/lib里
http://www.linuxidc.com/upload/2008_06/08062807082818.zip 
该压缩包里有在Ubuntu系统下用锐捷进行认证的必需文件：myxrgsu, libpcap.so.0.6.2, libstdc++.so.5等 。。。 
3. http://linux.chinaunix.net/techdoc/net/2009/07/05/1122014.shtml
在linux下运行程序时，发现了error while loading shared libraries这种错误，一时间不知道解决办法，在网上搜索，终于解决了：
./tests: error while loading shared libraries: xxx.so.0:cannot open shared object file: No such file or directory
出现这类错误表示，系统不知道xxx.so放在哪个目录下，这时候就要在/etc/ld.so.conf中加入xxx.so所在的目录。
一般而言，有很多的so会存放在/usr/local/lib这个目录底下，去这个目录底下找，果然发现自己所需要的.so文件。
所以，在/etc/ld.so.conf中加入/usr/local/lib这一行，保存之后，再运行：/sbin/ldconfig –v更新一下配置即可。
[url=http://blog.csdn.net/dumeifang/archive/2008/09/22/2963223.aspx][/url]
4.[libstdc++.so.5丢失的处理办法](http://liumin1939.javaeye.com/blog/326084) 今天在安装realplay时提示这个共享库找不到，错误信息为：error while loading shared libraries: libstdc++.so.5: cannot open shared object file: No such file or directory在/usr/lib下面发现有
 /usr/lib/libstdc++.so.6.0.3 于是运行 ln -s /usr/lib/libstdc++.so.6.0.3 /usr/lib/libstdc++.so.5 创建一个符合连接，这个错误信息没有了。但是又出现错误信息：undefined symbol: _ZNSt24__default_alloc_templateILb1ELi0EE12_S_free_listE看来不行。还是删除了连接： rm /usr/lib/libstdc++.so.5 然后下载：[url]http://mirror.centos.org/centos/4/os/i386/CentOS/RPMS/compat-libstdc++-33-3.2.3-47.3.i386.rpm
 [/url]再安装 rpm -ivh compat-libstdc++-33-3.2.3-47.3.i386.rpm 再重装producer，问题解决
**附加知识！！！**
> 原帖由 *cvgmt* 于 2008-12-2 17:12 发表 ![](http://bbs.ctex.org/images/common/back.gif)
我的操作系统是 Ubuntu 8.04，用 
tar -C / -zxf asymptote-*.**.i386.tgz
装 Asymptote 的。自从 1.53 版本以来一直都这个出错提示，说缺少  libtinfo.so.5，因此每次都启动不了，没办法，只有拉 svn 版本的 ...
一般 Linux 系统把 /lib 和 /usr/lib 两个目录作为默认的库搜索路径，所以使用这两个目录中的库时不需要进行设置搜索路径即可直接使用。对于处于默认库搜索路径之外的库，需要将库的位置添加到库的搜索路径之中。你要先确定libtinfo.so.5在哪个位置，假设在/usr/local/lib中，如果不想使用root权限，可以用
export LD_LIBRARY_PATH=/usr/local/lib: $LD_LIBRARY_PATH添加路径，另一种方法是在./configure时添加路径，具体见INSTALL文件。如果你是用bash，可以在用户目录下.bash_profile中添加LD_LIBRARY_PATH=/usr/local/lib: $LD_LIBRARY_PATH和export LD_LIBRARY_PATH两行，应该就没有问题了。
gc-7.1这个包你确定下了的？在Makefile.direct Makefile.dj中有编译termlib的选项。
g**oogle得知libtinfo.so.5这个是由ncurses提供的，**但是一般ncurses都是默认安装的，所以你再试一下从源代码编译安装ncurses？
**网址: http://bbs.ctex.org/viewthread.php?tid=46873**
