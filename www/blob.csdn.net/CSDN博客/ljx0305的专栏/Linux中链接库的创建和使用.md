# Linux中链接库的创建和使用 - ljx0305的专栏 - CSDN博客
2008年05月15日 22:29:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：645标签：[linux																[library																[linker																[archive																[gcc																[reference](https://so.csdn.net/so/search/s.do?q=reference&t=blog)](https://so.csdn.net/so/search/s.do?q=gcc&t=blog)](https://so.csdn.net/so/search/s.do?q=archive&t=blog)](https://so.csdn.net/so/search/s.do?q=linker&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Linux程序及使用](https://blog.csdn.net/ljx0305/article/category/394700)
在/usr/lib目录下面，我们会看到许多的文件形如:libxxx.so,libxxx.a这样的形式。libxxx.a我们称之为静态链接库,或者简称为链接库，libxxx.so称之为共享链接库，或者是动态链接库。
=== Archives ===
    我们编程的时候经常使用一些库，例如我们在编写网络捕获程序的时候，我么你使用libpcap,写网络发包函数的时候，我们使用libnet库。
    我们只需要在编译的时候加上链接的库就可以了.
    gcc test_pcap.c -lpcap
    编译器会自动的在pcap前面加上lib，在后面加上.a，也就是链接 libpcap.a 库文件
    如果能够自己编写库那该多好啊，下面我们就来介绍一下如何创建自己的链接库。
    .a称为归档，使用ar命令生成的，它是将一些.o文件归档在一起形成的，就像windows下的.lib文件一样。所以，我们在使用链接库的时候，实际上是链接的.a中的某个或某些.o文件，这样就跟gcc编译一样了。另外，需要注意一点，就是需要将链接库放在最后。
    gcc -o test_pcap test_pcap.o -lpcap
    这主要是因为linker会自动在后面寻找前面出现的符号，假如使用 gcc -o test_pcap -lpcap test_pcap.o。那么链接将会失败，因为linker就会找不到test_pcap.o中关于libpcap的一些函数。
    差一点忘记了一个重要的事情，那就是如何创建链接库:
    ar cr libtest.a test1.o test2.o
    就像上面说的，.a文件就是.o文件的归档。
=== Shared Libraries ===
    共享库跟上面提到的链接库差不多，但是他们还是有一些差别的，首先是如果一个程序链接共享库的话，在最后生成的可执行代码里面没有相应的代码，只有一个共享库的引用而已，减少可执行文件的大小，相应的在执行的时候，需要花费一些时间来通过引用获得相应的代码。
    使用共享库，能够减小可执行文件的大小，另外也方便库的版本升级，无须重新编译代码。
那么如何创建共享库呢？看一个例子：
gcc  -fpic -shared -o libtest.so  test1.o test2.o
是不是很简单啊 :-)
-fpic 使输出的对象模块是按照可重定位地址方式生成的。
-shared指定把对应的源文件生成对应的动态链接库文件libtest.so文件。
共享库的使用分为显示调用和隐示调用。
==== 隐示调用 ====
隐示调用就像上面链接库的使用一样:
gcc -o app app.o -L. -ltest
既然是这样子的，如果在-L指定的目录中，都存在.a和.so文件，linker应该链接哪一个文件呢？
这实际上就需要了解linker的链接顺序了，如果找到一个目录，其中只有.a和.so两种文件中的一个，那么就选择存在的那个文件；例如存在.a，那么就链接.a文件。如果两种文件都存在，那么就默认选择链接.so文件。
当然，还有一种情况需要注意，那就是--static的时候，那么他就使用.a来链接。
gcc -static -o app app.o -L. -ltest
上面提到了库的搜索路径问题，我们使用-L来添加一个临时的路径；另外，可以修改LD_LIBRARY_PATH环境变量来增加路径。
export LD_LIBRARY_PATH="/usr/lib/old:/opt/lib"
==== 显示调用 ====
其实显示的调用实际上就是使用在程序代码中，动态的加载和卸载库。
动态链接库，使用ld.so来进行加载。
    #include <dlfcn.h>
    void *dlopen(const char *filename, int flag); 
    void *dlsym(void *handle, const char *symbol); 
    int dlclose(void *handle); 
流程：
void* handle = dlopen (“libtest.so”, RTLD_LAZY);
void (*test)() = dlsym (handle, “my_function”);
(*test)();
dlclose (handle);
下面是man dlopen中的一个例子程序：
/* Filename: dl_call.c */
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
int main(void) 
{
    void *handle;    /* shared library 的 'handle' 指针*/
    double (*cosine)(double);   /* 指向 shared library 里的函数 */
    char *error;    /* 记录dynamic loader 的错误信息 */
    /* 打开shared library 'libm' */
    handle = dlopen ("libm.so", RTLD_LAZY);
    if (!handle) {
        fprintf (stderr, "%s/n", dlerror());
        exit(1);
    }
    dlerror();    /* Clear any existing error */
    /* 在 handle 指向的 shared library 里找到 "cos" 函数,
     * 并返回其内存地址
     */
    cosine = dlsym(handle, "cos");
    if ((error = dlerror()) != NULL)  {
        fprintf (stderr, "%s/n", error);
        exit(1);
    }
    /* indirect function call (函数指针间接调用),
     * 调用所指定的函数
     */
    printf ("%f/n", (*cosine)(2.0));
    dlclose(handle);
    return 0;
}
由于使用到 dlopen() 函数，编译的时候需要跟 libdl 链接
    $ gcc -o dl_call dl_call.c -ldl
以上的做法等于：
     printf ("%f/n", cos(2.0));
如果，共享库是一个C++写的，那么我们在一个C程序里面使用这个共享库的话，需要以下面的格式进行声明：
     extern “C” void foo ();
关于C++ dlopen的详细资料，可以参考Reference中C++ dlopen mini HowTo
libdl 的使用是很普遍的，许多的软件将自己的一些功能模块做成 shared library，并以 dlopen()载入后使用，由于 shared library是"可抽换"的，因此可通过 libdl 来实现"Plug-ins"的功能。
=== Reference ===
C++ dlopen mini HOWTO
http://www.isotton.com/devel/docs/C++-dlopen-mini-HOWTO/C++-dlopen-mini-HOWTO.html
http://www.linuxsir.org/bbs/printthread.php?t=266890
程序的链接和装入及Linux下动态链接的实现
http://www.ibm.com/developerworks/cn/linux/l-dynlink/index.html
=== Questions ===
1、如果在-L指定的目录中，都存在.a和.so文件，linker应该链接哪一个文件呢？
=== appendix ===
    One major advantage of a shared library is that it saves space on the system where
the program is installed. If you are installing 10 programs, and they all make use of the
same shared library, then you save a lot of space by using a shared library. If you used a
static archive instead, the archive is included in all 10 programs. So, using shared
libraries saves disk space. It also reduces download times if your program is being
downloaded from the Web.
    A related advantage to shared libraries is that users can upgrade the libraries without
upgrading all the programs that depend on them. For example, suppose that you
produce a shared library that manages HTTP connections. Many programs might
depend on this library. If you find a bug in this library, you can upgrade the library.
Instantly, all the programs that depend on the library will be fixed; you don’t have to
relink all the programs the way you do with a static archive.
    Those advantages might make you think that you should always use shared
libraries. However, substantial reasons exist to use static archives instead.The fact that
an upgrade to a shared library affects all programs that depend on it can be a disadvantage.
For example, if you’re developing mission-critical software, you might rather link
to a static archive so that an upgrade to shared libraries on the system won’t affect
your program. (Otherwise, users might upgrade the shared library, thereby breaking your 
program, and then call your customer support line, blaming you!)
    If you’re not going to be able to install your libraries in /lib or /usr/lib, you
should definitely think twice about using a shared library. (You won’t be able to install
your libraries in those directories if you expect users to install your software without
administrator privileges.) In particular, the -Wl,-rpath trick won’t work if you don’t
know where the libraries are going to end up. And asking your users to set
LD_LIBRARY_PATH means an extra step for them. Because each user has to do this
individually, this is a substantial additional burden.
    You’ll have to weigh these advantages and disadvantages for every program you
distribute.
引用：[http://blog.csdn.net/zhoujunyi/archive/2007/12/12/1930864.aspx](http://blog.csdn.net/zhoujunyi/archive/2007/12/12/1930864.aspx)
