# 如何使用新的glibc来编译自己的程序 - xqhrs232的专栏 - CSDN博客
2018年04月23日 10:12:28[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1193
原文地址::[http://www.sysnote.org/2015/08/25/use-new-glibc/](http://www.sysnote.org/2015/08/25/use-new-glibc/)

通常情况下我们都是直接使用glibc提供的一些库函数，但是某些特殊的情况，比如要修改glibc的一些代码或者加入glibc的一些编译选项或者要使用其他版本的glibc，我们就需要重新编译glibc。
编译glibc时特别要注意，不能去替换系统自带的glibc，因为glibc作为linux系统的核心库，很多底层模块都依赖它，稍有不慎就会把系统搞挂掉。因此我们编译glibc时最好要指定prefix=/new/path，
这样编译完了之后make install的时候就不会去覆盖系统自带的版本。那么问题来了，编译好glibc后，如何让我们的程序使用这个新编译的glibc呢？下面就以我们实际编译glibc的步骤来说明。
# [](http://www.sysnote.org/2015/08/25/use-new-glibc/#1-%E4%B8%8B%E8%BD%BDglibc%E6%BA%90%E4%BB%A3%E7%A0%81)1.下载glibc源代码
[http://www.gnu.org/software/libc/，在官网上有各个glibc的发行版，这里我选用的是glibc-2.17](http://www.gnu.org/software/libc/%EF%BC%8C%E5%9C%A8%E5%AE%98%E7%BD%91%E4%B8%8A%E6%9C%89%E5%90%84%E4%B8%AAglibc%E7%9A%84%E5%8F%91%E8%A1%8C%E7%89%88%EF%BC%8C%E8%BF%99%E9%87%8C%E6%88%91%E9%80%89%E7%94%A8%E7%9A%84%E6%98%AFglibc-2.17)
# [](http://www.sysnote.org/2015/08/25/use-new-glibc/#2-%E8%A7%A3%E5%8E%8Bglibc%E5%88%B0%E5%BD%93%E5%89%8D%E7%9B%AE%E5%BD%95)2.解压glibc到当前目录
```
root@xxx:~# tar zxvf glibc-2.17.tar.gz
```
# [](http://www.sysnote.org/2015/08/25/use-new-glibc/#3-%E5%88%9B%E5%BB%BAglibc%E7%9A%84build%E7%9B%AE%E5%BD%95)3.创建glibc的build目录
不能在glibc-2.17源码目录下./configure，会报错“configure: error: you must configure in a separate build directory”
```
root@xxx:~# mkdir -p build/glibc-build
```
# [](http://www.sysnote.org/2015/08/25/use-new-glibc/#4-configure-glibc)4.configure glibc
这里我指定了自己需要的一些编译选项，并且指定prefix目录
```
root@xxx:~/build/glibc-build# /root/glibc-2.17/configure CFLAGS="-fno-builtin-strlen -ggdb -O2" FEATURES="preserve-libs nostrip splitdebug" --prefix=/root/build/glibc-build
```
# [](http://www.sysnote.org/2015/08/25/use-new-glibc/#5-%E7%BC%96%E8%AF%91%E5%AE%89%E8%A3%85)5.编译安装
```
root@xxx:~/build/glibc-build# make -j10
root@xxx:~/build/glibc-build# make install
```
可以看到glibc相关的库都在glibc-build目录下了
```
root@xxx:~/build/glibc-build# ls lib
audit              libanl.so               libcidn.so     libdl.a     libm.so.6        libnss_db.so.2           libnss_nis-2.17.so      libpthread.so      librt.so.1           Scrt1.o
crt1.o              libanl.so.1           libcidn.so.1     libdl.so     libnsl-2.17.so        libnss_dns-2.17.so     libnss_nisplus-2.17.so  libpthread.so.0      libSegFault.so
crti.o              libBrokenLocale-2.17.so  libc_nonshared.a  libdl.so.2     libnsl.a        libnss_dns.so           libnss_nisplus.so       libresolv-2.17.so  libthread_db-1.0.so
crtn.o              libBrokenLocale.a        libcrypt-2.17.so  libg.a         libnsl.so        libnss_dns.so.2        libnss_nisplus.so.2     libresolv.a      libthread_db.so
gconv              libBrokenLocale.so       libcrypt.a     libieee.a     libnsl.so.1        libnss_files-2.17.so   libnss_nis.so           libresolv.so      libthread_db.so.1
gcrt1.o              libBrokenLocale.so.1     libcrypt.so     libm-2.17.so     libnss_compat-2.17.so    libnss_files.so        libnss_nis.so.2           libresolv.so.2      libutil-2.17.so
ld-2.17.so          libbsd-compat.a           libcrypt.so.1     libm.a         libnss_compat.so    libnss_files.so.2      libpcprofile.so           librpcsvc.a      libutil.a
ld-linux-x86-64.so.2  libc-2.17.so           libc.so         libmcheck.a     libnss_compat.so.2    libnss_hesiod-2.17.so  libpthread-2.17.so      librt-2.17.so      libutil.so
libanl-2.17.so          libc.a               libc.so.6     libmemusage.so  libnss_db-2.17.so    libnss_hesiod.so       libpthread.a           librt.a          libutil.so.1
libanl.a          libcidn-2.17.so           libdl-2.17.so     libm.so     libnss_db.so        libnss_hesiod.so.2     libpthread_nonshared.a  librt.so          Mcrt1.o
```
# [](http://www.sysnote.org/2015/08/25/use-new-glibc/#6-%E4%BD%BF%E7%94%A8%E6%96%B0%E7%9A%84glibc%E6%9D%A5%E7%BC%96%E8%AF%91%E7%A8%8B%E5%BA%8F%EF%BC%8C%E8%BF%99%E9%87%8C%E6%88%91%E7%BC%96%E8%AF%91%E7%9A%84%E6%98%AFvalgrind)6.使用新的glibc来编译程序，这里我编译的是valgrind
```
root@xxx:~# tar jxvf valgrind-3.10.1.tar.bz2
root@xxx:~# mkdir -p build/valgrind-build
root@xxx:~# cd valgrind-3.10.1
root@xxx:~/valgrind-3.10.1# ./configure CFLAGS="-I/root/build/glibc-build/include" LDFLAGS="-L/home/nbs/glibc/lib -Wl,--rpath=/root/build/glibc-build/lib -Wl,--dynamic-linker=/root/build/glibc-build/lib/ld-linux-x86-64.so.2" --prefix=/root/build/valgrind-build
```
下面对configure的选项进行说明：
*CFLAGS=”-I/root/build/glibc-build/include”* 这个是指定头文件的查找路径，去新编译的glibc里查找
*LDFLAGS*: Makefile的链接选项，其中
**-L**: 告诉链接器先从指定的路径查找库来链接，如果没找到，再从默认的地方找。编译时的-L选项并不影响环境变量LD_LIBRARY_PATH，-L只是指定了程序编译连接时库的路径，并不影响程序执行时库的路径，系统还是会到默认路径下查找该程序所需要的库，如果找不到，还是会报错，类似cannot open shared object file。
可以设定LD_LIBRARY_PATH来让程序在运行时查找除默认路径（默认是先搜索/lib和/usr/lib这两个目录，然后按照/etc/ld.so.conf里面的配置搜索绝对路径）之外的其他路径，不过LD_LIBRARY_PATH的设定作用是全局的，建议使用gcc的的-R或-rpath选项来在编译时就指定库的查找路径，并且该库的路径信息保存在可执行文件中，运行时它会直接到该路径查找库，避免了使用LD_LIBRARY_PATH环境变量查找。
因此我在上面指定了-Wl,–rpath=/root/build/glibc-build/lib，说明一下，-Wl,option是将选项传给链接器
另外，也需要使用新编译的链接器，使用*–dynamic-linker*指定链接器
然后再使用make && make install就可以编译完成，完成后，可以看到安装的东西都在/root/build/valgrind-build/下。

