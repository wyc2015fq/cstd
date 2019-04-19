# [记录]移植驱动完毕后加载时的version magic报错原因以及解决办法（invalid module format） - xqhrs232的专栏 - CSDN博客
2016年07月12日 15:36:35[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：874
原文地址::[http://blog.sina.com.cn/s/blog_6e5e78bf010105jj.html](http://blog.sina.com.cn/s/blog_6e5e78bf010105jj.html)
相关文章
1、加载模块出错：version magic '2.6.32-504.el6.i686 SMP mod_unload 686 ' should----[http://www.myexception.cn/program/1885736.html](http://www.myexception.cn/program/1885736.html)
2、加载模块报错 version magic mod_unload ARMv4 invalid module format----[http://blog.sina.com.cn/s/blog_752fa65f0100p6wc.html](http://blog.sina.com.cn/s/blog_752fa65f0100p6wc.html)
3、[linux的模块加载问题](http://www.21qa.net/questions/2145/2145)----[http://www.21qa.net/questions/2145/2145](http://www.21qa.net/questions/2145/2145)

History:2012-02-17
Author:yingru
移植rt3070的AP驱动到装有fedora14的PC机上时，模块编译完毕后，加载时提示invalid module format。
PC机环境介绍：
内核版本：2.6.35.6-45.fc14.i686
命令行输入dmesg查看最后的日志，发现如下错误记录：
rtutil3070ap:version magic '2.6.35.14-96.fc14.i686 SMP mod_unload 686 ' should be '2.6.35.6-45.fc14.i686 SMP mod_unload 686'
其余几个模块也是同样错误
version magic真的是个很恶心的东西。。
既然提示出错了就去找原因。
查证AP驱动的makefile里内核目录变量指向的是当前PC机所用内核目录。/lib/modules/`uname -r`/build
这一步没有错。那么说明驱动的makefile是正确的
切换到/lib/modules/`uname -r`/目录下，查看build（build在此处是一个符号链接）， ls -al 发现build指向的目录是/usr/src/kernels/2.6.35.14-96.fc14.i686，问题在这里，虽然我用的目录是/lib/modules/`uname -r`/build 这个目录貌似与我当前内核版本（uname -r 即2.6.35.6-45.fc14.i686）一致，但是build这个符号链接指向的内核源码却是2.6.35.14-96.fc14.i686的，这样造成了编译出来的模块所带的version
 magic是2.6.35.14-96.fc14.i686这一版本，加载时与uname -r不符。
那么要解决这一问题，就需要“篡改”一下内核源码里的version magic 。
驱动模块的version magic信息是怎么生成的：
2.6 内核下，在linux/vermagic.h中定义有VERMAGIC_STRING，VERMAGIC_STRING不仅包含内核版本号，还包含有内核 使用的gcc版本，SMP与PREEMPT等配置信息。模块在编译时，我们可以看到屏幕上会显示"MODPOST"。在此阶段， VERMAGIC_STRING会添加到模块的modinfo段。在内核源码目录下scripts\mod\modpost.c文件中可以看到模块后续处 理部分的代码。模块编译生成后，通过`modinfo mymodule.ko`命令可以查看此模块的vermagic等信息。2.6
 内核下的模块装载器里保存有内核的版本信息，在装载模块时，装载器会比较所保存的内核vermagic与此模块的modinfo段里保存的 vermagic信息是否一致，两者一致时，模块才能被装载。譬如Fedora core 4 与core 2 使用的都是2.6 版本内核，在Fedore Core 2下去加载Fedora Core4下编译生成的hello.ko，会出现"invalid module format" 错误。
http://www.ibm.com/developerworks/cn/linux/l-module26/
既然是这个道理，那么只需要把/usr/src/kernels/2.6.35.14-96.fc14.i686/目录下源码中的include/linux/vermagic.h中的VERMAGIC_STRING修改成与当前PC内核uname -r一致即可。
修改如下：
#define VERMAGIC_STRING                 \
"2.6.35.6-45.fc14.i686" ""      \
MODULE_VERMAGIC_SMP MODULE_VERMAGIC_PREEMPT  \
MODULE_VERMAGIC_MODULE_UNLOAD MODULE_VERMAGIC_MODVERSIONS \
MODULE_ARCH_VERMAGIC
（描红为修改处）
具体的格式可以在modinfo yourmodule.ko查看之后 再确定修改哪里 修改完后重新make即可 加载后没有version magic报错提示。
