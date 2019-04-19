# Linux下gcc编译生成动态链接库*.so文件并调用它 - xqhrs232的专栏 - CSDN博客
2017年02月22日 17:22:13[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：314
原文地址::[http://blog.csdn.net/u011057439/article/details/51459620](http://blog.csdn.net/u011057439/article/details/51459620)
相关文章
1、[Linux下的动态共享库(so)开发调用](http://blog.csdn.net/educast/article/details/9398935)----[http://blog.csdn.net/educast/article/details/9398935](http://blog.csdn.net/educast/article/details/9398935)
2、[Linux下C程序调用.so(动态链)的一个例子](http://blog.csdn.net/smallsky_keke/article/details/7372184)----[http://blog.csdn.net/smallsky_keke/article/details/7372184](http://blog.csdn.net/smallsky_keke/article/details/7372184)
动态库*.so在linux下用c和c++编程时经常会碰到，最近在网站找了几篇文章介绍动态库的编译和链接，总算搞懂了这个之前一直不太了解得东东，这里做个笔记，也为其它正为动态库链接库而苦恼的兄弟们提供一点帮助。
1、动态库的编译
下面通过一个例子来介绍如何生成一个动态库。这里有一个头文件：so_test.h，三个.c文件：test_a.c、test_b.c、test_c.c，我们将这几个文件编译成一个动态库：libtest.so。
//so_test.h：
#include "stdio.h"
void test_a();
void test_b();
void test_c();
//test_a.c：
#include "so_test.h"
void test_a()
{
  printf("this is in test_a...\n");
}
//test_b.c：
#include "so_test.h"
void test_b()
{
  printf("this is in test_b...\n");
}
//test_c.c：
#include "so_test.h"
void test_c()
{
  printf("this is in test_c...\n");
}
将这几个文件编译成一个动态库：libtest.so
$ gcc test_a.c test_b.c test_c.c -fPIC -shared -o libtest.so
2、动态库的链接
在1、中，我们已经成功生成了一个自己的动态链接库libtest.so，下面我们通过一个程序来调用这个库里的函数。程序的源文件为：test.c。
test.c：
#include "so_test.h"
int main()
{
test_a();
test_b();
test_c();
return 0;
}
将test.c与动态库libtest.so链接生成执行文件test：
$ gcc test.c -L. -ltest -o test
测试是否动态连接，如果列出libtest.so，那么应该是连接正常了
$ ldd test
执行test，可以看到它是如何调用动态库中的函数的。
3、编译参数解析
最主要的是GCC命令行的一个选项:
-shared该选项指定生成动态连接库（让连接器生成T类型的导出符号表，有时候也生成弱连接W类型的导出符号），不用该标志外部程序无法连接。相当于一个可执行文件
-fPIC：表示编译为位置独立的代码，不用此选项的话编译后的代码是位置相关的所以动态载入时是通过代码拷贝的方式来满足不同进程的需要，而不能达到真正代码段共享的目的。
-L.：表示要连接的库在当前目录中
-ltest：编译器查找动态连接库时有隐含的命名规则，即在给出的名字前面加上lib，后面加上.so来确定库的名称
LD_LIBRARY_PATH：这个环境变量指示动态连接器可以装载动态库的路径。
当然如果有root权限的话，可以修改/etc/ld.so.conf文件，然后调用 /sbin/ldconfig来达到同样的目的，不过如果没有root权限，那么只能采用输出LD_LIBRARY_PATH的方法了。
4、注意
调用动态库的时候有几个问题会经常碰到，有时，明明已经将库的头文件所在目录 通过 “-I” include进来了，库所在文件通过 “-L”参数引导，并指定了“-l”的库名，但通过ldd命令察看时，就是死活找不到你指定链接的so文件，这时你要作的就是通过修改 LD_LIBRARY_PATH或者/etc/ld.so.conf文件来指定动态库的目录。通常这样做就可以解决库无法链接的问题了。
在linux下可以用export命令来设置这个值，在linux终端下输入:
export LD_LIBRARY_PATH=/opt/au1200_rm/build_tools/bin: $LD_LIBRARY_PATH: 　　
然后再输入:export 　　
即会显示是否设置正确 　　
export方式在重启后失效，所以也可以用 vim /etc/bashrc ，修改其中的LD_LIBRARY_PATH变量。 　　
例如：LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/au1200_rm/build_tools/bin。
