# linux64bit机器上运行32位的代码 - xqhrs232的专栏 - CSDN博客
2016年10月18日 16:09:04[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：212
原文地址::[http://blog.csdn.net/nemo2011/article/details/8440629](http://blog.csdn.net/nemo2011/article/details/8440629)
今天再64bit的centos下，运行32bit下的二进制代码。 然后爆出了各种错误。
后来网上找了许多资料。 发现这个还比较靠谱：
我们知道在64bit的机器上可以运行32bit的代码， 那这里需要什么支持吗？
当然需要， 如果你没有安装必要的包， 你使用-m32参数会报下面这样类似的错误：
/opt/x86_open64-4.2.3.2/lib/gcc-lib/x86_64-open64-[Linux](http://lib.csdn.net/base/linux)/4.2.3.2/mfef95:
 error while loading shared libraries: libgcc_s.so.1: cannot open shared object file: No such file or directory
不过我们安装了下面的几个包文件就可以正常运行了：
yum install glibc.i686
yum install glibc-devel
yum install libstdc++.i686
(记住也要安装i686的glibc和libstdc哟 ^-^)
转于：http://www.lupaworld.com/home-space-uid-434384-do-blog-id-241620.html
首先ldd filename  和file filename 分析二进制文件它属于多少位系统文件和它的依赖关系。
第二 ， 抓住两点就行了。 第一要有支持执行32bit下的c/c++文件的lib库。 比如libstdc++.so.6。
其实它是一个指向libstdc++.so.6.0.13的符号连接。（可以ls -l  filename查看）
去下载一个32bit的libstdc++.so.6.0.13的文件，放到某个目录下。 比如/usr/lib下。 
然后建立符号连接： 
ln -s libstdc++.so.6.0.13  ibstdc++.so.6
第三， 设置LIB库路径。 
我这以/usr/lib为例：
export LD_LIBRARY_PATH=/usr/lib:$LD_LIBRARY_PATH
