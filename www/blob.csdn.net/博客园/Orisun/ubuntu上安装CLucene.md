# ubuntu上安装CLucene - Orisun - 博客园







# [ubuntu上安装CLucene](https://www.cnblogs.com/zhangchaoyang/articles/1995558.html)





1.去
[http://210.76.97.209/gf/?action=sourceid&sid=1347](http://210.76.97.209/gf/?action=sourceid&sid=1347)下载
[clucene-core-0.9.21b.tar.bz2](http://210.76.97.209/gf/?action=SourceDownload&vid=34852&sid=1347)

2.参照
[http://clucene.sourceforge.net/download.shtml#0_9_21b](http://clucene.sourceforge.net/download.shtml#0_9_21b)进行安装
Requirements:
GNU autotools is required:
Autoconf 
Automake 
Libtool 

Building
--------
The following will get you building assuming that you have sufficiently
recent build tools installed.  
1.) unpack tarball 
2.) cd into clucene
3.) if you downloaded a tar version skip to 5
4.) run ./autogen.sh
5.) run ./configure
6.) run make
3.按照2做下去你会发现有些地方不对,参见这位仁兄的
[http://gcgmh.javaeye.com/blog/306171](http://gcgmh.javaeye.com/blog/306171)


  (1)根据INSTALL 文档中的描述，安装完成后，在src/demo 目录下应改有文件名为cl_demo 的文件；但我安装完成后，却看不到这个文件，后来，在src/demo 目录下执行 make  后，终于看到了cl_demo。 
 (2)根据INSTALL 文档中的描述，安装完成后，在test 目录下应改有文件名为cl_test的文件；但我安装完成后，却看不到这个文件，后来，在test 目录下执行 make check 后，终于看到了cl_test。如果执行make而不是make check，则会出现下列错误。 
make[1] Entering directory /home/username/clucene-core-0.9.20/test 
make[1] nothing to be done for 'all-am' 
make[1] Leaving directory /home/username/clucene-core-0.9.20/test 
　(3) 根据INSTALL 文档中的描述，安装完成后，在src目录下应改有类似下列文件名的文件。 
   libclucene.so.0.0.0 
   libclucene.la 
   libclucene.a 
  但我却看不到，后经使用 find 命令查找发现  这些文件存在于 src/.libs/ 目录下。 

之所以出现 上述 4，5，6“错误”，大概是由于说明文件INSTALL是较早版本的，其中的内容与0.9.20版的clucene 已不符，test demo等程序在缺省安装方式下是不执行make 操作的。 

最终，利用test目录下的cl_test 测试，测试结果表明，安装成功。 

另外，CLucene 的源代码在src/Clucene目录下。 

4.网上还有哥们儿说src/demo/IndexFile.cpp的第56--60行有误,应把

r = fread(abuf,1,1023,fh);   abuf[r]=0;   STRCPY_AtoT(tbuf,abuf,r);   tbuf[r] = 0;   str.append(tbuf);

改为

r = fread(abuf,1024,1,fh);abuf[r]=0;STRCPY_AtoT(tbuf,abuf,r);tbuf[r];str.append(tbuf);












