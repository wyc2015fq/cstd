# Can't locate Tk.pm - Orisun - 博客园







# [Can't locate Tk.pm](https://www.cnblogs.com/zhangchaoyang/articles/2055136.html)





今天安装Perl的调试工具， [Devel-ptkdb-1.1091.tar.gz](http://search.cpan.org/CPAN/authors/id/A/AE/AEPAGE/Devel-ptkdb-1.1091.tar.gz)解压后make test时报错：Can't locate Tk.pm。

This's because x11/tk doesn't contain Tk.pm.  You need to install x11/p5-Tk.

具体怎么做呢？

1）sudo apt-get install libx11-dev

2）sudo cpan -i Tk　　　　　　　　　　##会自动地从CPAN上下载安装Tk.pm

此时还是会报错：make[1]: *** [Xlib_f.o] Error 1

make[1]: Leaving directory `/home/orisun/.cpan/build/Tk-804.029-NXYYkx/pTk'

make: *** [pTk/libpTk.a] Error 2 

SREZIC/Tk-804.029.tar.gz 

/usr/bin/make -- NOT OK

一切搞定后再安装Devel-ptkdb。












