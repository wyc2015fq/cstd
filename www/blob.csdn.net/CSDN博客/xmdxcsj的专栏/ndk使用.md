# ndk使用 - xmdxcsj的专栏 - CSDN博客





2015年05月27日 18:41:29[xmucas](https://me.csdn.net/xmdxcsj)阅读数：646








一、过程

1.下载

官方下载NDK的地址为：http://developer.android.com/tools/sdk/ndk/index.html#Downloads

但是如果需要下载旧版本的NDK，google没有直接提供链接。可以通过下面的链接下载旧版本的NDK:

**http://dl.google.com/android/ndk/android-ndk-<version>-<platform>.<fileextension>**

其中<version>为NDK的版本，如r7/r7b/r7c/r8/r8b等内容，<platform>为目标操作系统，可以为:windows/darwin-x86/linux-x86，分别对应于:Windows,Mac OS X (intel),Linux 32/64-bit (x86)平台,<fileextension>为文件后缀，如果<platform>为windows，则为"zip"，否则为"tar.bz2"。

举例，下载Linux下的NDKr8版本，则可以使用链接:

http://dl.google.com/android/ndk/android-ndk-**r8**-**linux-x86**.**tar.bz2**




**2.解压**

bzip2 -d  gcc-4.1.0.tar.bz2

---上面解压完之后执行下面的命令。

tar -xvf gcc-4.1.0.tar 或 tar -xvf *.tar

解完之后会出现多一个文件夹 gcc-4.1.0




3.官方说明文档

docs文件夹下面，重要的文档包括：

OVERVIEW.html

ANDROID-MK.html         有关Android.mk的用法

APPLICATION-MK.html   有关Application.mk的用法

NDK-BUILD.html             有关ndk-build命令的用法




二、遇到的问题

1. version `GLIBC_2.14' not found 

使用的ndk最新版r10e，原因是本地的glibc库文件太低，由于是公司服务器，没敢修改库文件，选择使用ndk的r7版本，有篇文章参考：

[http://blog.csdn.net/force_eagle/article/details/8684669](http://blog.csdn.net/force_eagle/article/details/8684669)

2. 版本问题

r7版本以后支持APP_ABI := all

r5版本不支持这种用法



