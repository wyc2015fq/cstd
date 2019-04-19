# linux中的ldd命令简介 - xqhrs232的专栏 - CSDN博客
2016年05月04日 10:53:11[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：249
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/stpeace/article/details/47069215](http://blog.csdn.net/stpeace/article/details/47069215)

      在linux中， 有些命令是大家通用的， 比如ls, rm, mv, cp等等， 这些我觉得没有必要再细说了。 而有些命令， 只有开发人员才会用到的， 这类命令， 作为程序员的我们， 是有必要了解的， 有的甚至需要熟练使用。
        有的人总说， 这些命令不重要， 用的时候去查就行了， 这么多么扯淡的说法啊。 具体用法细节是可以可查， 但至少得知道有ldd这个东西吧。连ldd都不知道， 怎么知道ldd是干啥的呢？
       在本文中， 我们来介绍一下ldd命令， 尽管它非常简单。  哦， 我突然想起来， 我有个朋友， 她的名字的是三个字， 首写字母刚好是l, d, d, 有点意思。  在linux中， ldd是list, dynamic, dependencies的缩写， 意思是， 列出动态库依赖关系。  当然， 你也可以用ldd --help或者man ldd来看其用法。 下面， 我们也来看看：
       test.h的内容为：
**[cpp]**[view
 plain](http://blog.csdn.net/stpeace/article/details/47069215#)[copy](http://blog.csdn.net/stpeace/article/details/47069215#)
- void print();  
       test.c的内容为：
**[cpp]**[view
 plain](http://blog.csdn.net/stpeace/article/details/47069215#)[copy](http://blog.csdn.net/stpeace/article/details/47069215#)
- #include <stdio.h>
- #include "test.h"
- 
- void print()  
- {  
- printf("rainy days\n");  
- }  
       main.c的内容为：
**[cpp]**[view
 plain](http://blog.csdn.net/stpeace/article/details/47069215#)[copy](http://blog.csdn.net/stpeace/article/details/47069215#)
- #include "test.h"
- 
- int main()  
- {  
-     print();  
- return 0;  
- }  
      进行一系列的编译， 并用ldd命令， 得到：
**[plain]**[view
 plain](http://blog.csdn.net/stpeace/article/details/47069215#)[copy](http://blog.csdn.net/stpeace/article/details/47069215#)
- [taoge@localhost learn_ldd]$ ls  
- main.c  test.c  test.h  
- [taoge@localhost learn_ldd]$ gcc -c main.c test.c  
- [taoge@localhost learn_ldd]$ gcc main.o test.o  
- [taoge@localhost learn_ldd]$ ls  
- a.out  main.c  main.o  test.c  test.h  test.o  
- [taoge@localhost learn_ldd]$ ./a.out   
- rainy days  
- [taoge@localhost learn_ldd]$   
- [taoge@localhost learn_ldd]$   
- [taoge@localhost learn_ldd]$   
- [taoge@localhost learn_ldd]$ ldd *  
- a.out:  
-     linux-gate.so.1 =>  (0x00ba1000)  
-     libc.so.6 => /lib/libc.so.6 (0x0087e000)  
-     /lib/ld-linux.so.2 (0x00858000)  
- main.c:  
- ldd: warning: you do not have execution permission for `./main.c'  
-     not a dynamic executable  
- main.o:  
- ldd: warning: you do not have execution permission for `./main.o'  
-     not a dynamic executable  
- test.c:  
- ldd: warning: you do not have execution permission for `./test.c'  
-     not a dynamic executable  
- test.h:  
- ldd: warning: you do not have execution permission for `./test.h'  
- lddlibc4: cannot read header from `./test.h'  
- test.o:  
- ldd: warning: you do not have execution permission for `./test.o'  
-     not a dynamic executable  
- [taoge@localhost learn_ldd]$   
      可以看到a.out依赖于libc.so.6这个库，
 而这个库的路径为/lib/libc.so.6
      我们继续看使用静态链接库的情形：
**[plain]**[view
 plain](http://blog.csdn.net/stpeace/article/details/47069215#)[copy](http://blog.csdn.net/stpeace/article/details/47069215#)
- [taoge@localhost learn_ldd]$ ls  
- main.c  test.c  test.h  
- [taoge@localhost learn_ldd]$ gcc -c test.c  
- [taoge@localhost learn_ldd]$ ar rcs libtest.a test.o  
- [taoge@localhost learn_ldd]$ gcc main.c -L. -ltest  
- [taoge@localhost learn_ldd]$ ls  
- a.out  libtest.a  main.c  test.c  test.h  test.o  
- [taoge@localhost learn_ldd]$ ./a.out  
- rainy days  
- [taoge@localhost learn_ldd]$   
- [taoge@localhost learn_ldd]$   
- [taoge@localhost learn_ldd]$   
- [taoge@localhost learn_ldd]$ ldd *  
- a.out:  
-     linux-gate.so.1 =>  (0x00e7c000)  
-     libc.so.6 => /lib/libc.so.6 (0x0087e000)  
-     /lib/ld-linux.so.2 (0x00858000)  
- libtest.a:  
- ldd: warning: you do not have execution permission for `./libtest.a'  
-     not a dynamic executable  
- main.c:  
- ldd: warning: you do not have execution permission for `./main.c'  
-     not a dynamic executable  
- test.c:  
- ldd: warning: you do not have execution permission for `./test.c'  
-     not a dynamic executable  
- test.h:  
- ldd: warning: you do not have execution permission for `./test.h'  
- lddlibc4: cannot read header from `./test.h'  
- test.o:  
- ldd: warning: you do not have execution permission for `./test.o'  
-     not a dynamic executable  
- [taoge@localhost learn_ldd]$   
       这次用静态库， 结果还是差不多， 就没什么好说的了。
       我们继续看使用动态链接库时的情形：
**[plain]**[view
 plain](http://blog.csdn.net/stpeace/article/details/47069215#)[copy](http://blog.csdn.net/stpeace/article/details/47069215#)
- [taoge@localhost learn_ldd]$ ls  
- main.c  test.c  test.h  
- [taoge@localhost learn_ldd]$ gcc -c test.c  
- [taoge@localhost learn_ldd]$ gcc -shared -fPIC -o libtest.so test.o  
- [taoge@localhost learn_ldd]$ gcc main.c -L. -ltest  
- [taoge@localhost learn_ldd]$ ls  
- a.out  libtest.so  main.c  test.c  test.h  test.o  
- [taoge@localhost learn_ldd]$ ./a.out   
- ./a.out: error while loading shared libraries: libtest.so: cannot open shared object file: No such file or directory  
- [taoge@localhost learn_ldd]$   
- [taoge@localhost learn_ldd]$   
- [taoge@localhost learn_ldd]$   
- [taoge@localhost learn_ldd]$ ldd *  
- a.out:  
-     linux-gate.so.1 =>  (0x00f3d000)  
-     libtest.so => not found  
-     libc.so.6 => /lib/libc.so.6 (0x0087e000)  
-     /lib/ld-linux.so.2 (0x00858000)  
- libtest.so:  
-     linux-gate.so.1 =>  (0x0031d000)  
-     libc.so.6 => /lib/libc.so.6 (0x00110000)  
-     /lib/ld-linux.so.2 (0x00858000)  
- main.c:  
- ldd: warning: you do not have execution permission for `./main.c'  
-     not a dynamic executable  
- test.c:  
- ldd: warning: you do not have execution permission for `./test.c'  
-     not a dynamic executable  
- test.h:  
- ldd: warning: you do not have execution permission for `./test.h'  
- lddlibc4: cannot read header from `./test.h'  
- test.o:  
- ldd: warning: you do not have execution permission for `./test.o'  
-     not a dynamic executable  
- [taoge@localhost learn_ldd]$   
- [taoge@localhost learn_ldd]$ su root  
- Password:   
- [root@localhost learn_ldd]# cp libtest.so /usr/lib/  
- [root@localhost learn_ldd]# ./a.out   
- rainy days  
- [root@localhost learn_ldd]# exit  
- exit  
- [taoge@localhost learn_ldd]$ ./a.out   
- rainy days  
- [taoge@localhost learn_ldd]$  
- [taoge@localhost learn_ldd]$  
- [taoge@localhost learn_ldd]$  
- [taoge@localhost learn_ldd]$ ldd a.out   
-     linux-gate.so.1 =>  (0x00510000)  
-     libtest.so => /usr/libtest.so (0x00fe3000)  
-     libc.so.6 => /lib/libc.so.6 (0x0087e000)  
-     /lib/ld-linux.so.2 (0x00858000)  
- [taoge@localhost learn_ldd]$   
        首先， 我们可以看到， a.out依赖于libtest.so这个库， 但是， 结果是not found， 找不到。 为什么呢？ 因为在/usr/lib下面没有libtest.so， 后来， 我把libtest.so拷贝过去（需要root权限）， 就OK了。 另外， 我们也应该看到， libtest.so的依赖库也是可以通过ldd命令找到的。
         当然， 如果不想自己写程序， 但想试一下ldd命令， 那也可以， 直接如下：
**[plain]**[view
 plain](http://blog.csdn.net/stpeace/article/details/47069215#)[copy](http://blog.csdn.net/stpeace/article/details/47069215#)
- [taoge@localhost learn_ldd]$ ldd /bin/ls  
-     linux-gate.so.1 =>  (0x0052b000)  
-     libselinux.so.1 => /lib/libselinux.so.1 (0x00b52000)  
-     librt.so.1 => /lib/librt.so.1 (0x00a5c000)  
-     libcap.so.2 => /lib/libcap.so.2 (0x0489c000)  
-     libacl.so.1 => /lib/libacl.so.1 (0x048c9000)  
-     libc.so.6 => /lib/libc.so.6 (0x0087e000)  
-     libdl.so.2 => /lib/libdl.so.2 (0x00a0c000)  
-     /lib/ld-linux.so.2 (0x00858000)  
-     libpthread.so.0 => /lib/libpthread.so.0 (0x00a13000)  
-     libattr.so.1 => /lib/libattr.so.1 (0x04d99000)  
- [taoge@localhost learn_ldd]$ ldd /bin/mv  
-     linux-gate.so.1 =>  (0x00944000)  
-     libselinux.so.1 => /lib/libselinux.so.1 (0x00b52000)  
-     librt.so.1 => /lib/librt.so.1 (0x00a5c000)  
-     libacl.so.1 => /lib/libacl.so.1 (0x048c9000)  
-     libattr.so.1 => /lib/libattr.so.1 (0x04d99000)  
-     libc.so.6 => /lib/libc.so.6 (0x00110000)  
-     libdl.so.2 => /lib/libdl.so.2 (0x00a0c000)  
-     /lib/ld-linux.so.2 (0x00858000)  
-     libpthread.so.0 => /lib/libpthread.so.0 (0x00a13000)  
- [taoge@localhost learn_ldd]$   
        在实际linux开发与调试中， 要经常查看动态库依赖关系， ldd用得还是比较多的， 特别是出现故障的时候。OK, ldd命令就简单介绍到这里了， 虽然简单， 但很实用， 故不可不知。
        好了， 今天也写了不少文章了， 就此搁笔。 收拾一下， 看看书， 早点休息， 迎接明天的周一。
//===================================================================================================
备注::
1>linux下相同的命令有nm/strace/dlsym/dumpbin
