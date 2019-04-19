# linux下的库函数.so和.a - xqhrs232的专栏 - CSDN博客
2016年10月27日 23:59:50[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：402
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/clarkchenhot/article/details/51894727](http://blog.csdn.net/clarkchenhot/article/details/51894727)
[Linux](http://lib.csdn.net/base/linux)下的.so是基于Linux下的动态链接,其功能和作用类似与windows下.dll文件。
下面是关于.so的介绍:
一、引言
通常情况下，对函数库的链接是放在编译时期（compile time）完成的。所有相关的对象文件（object file）与牵涉到的函数库（library）被链接合成一个可执行文件（executable file）。程序在运行时，与函数库再无瓜葛，因为所有需要的函数已拷贝到自己门下。所以这些函数库被成为静态库（static libaray），通常文件名为“libxxx.a”的形式。
.o 就相当于windows里的obj文件 ，一个.c或.cpp文件对应一个.o文件
.a 是好多个.o合在一起,用于静态连接 ，即STATIC mode，多个.a可以链接生成一个exe的可执行文件
.so 是shared object,用于动态连接的,和windows的dll差不多，使用时才载入。
其实，我们也可以把对一些库函数的链接载入推迟到程序运行的时期（runtime）。这就是如雷贯耳的动态链接库（dynamic link library）技术。
二、动态链接库的特点与优势
首先让我们来看一下，把库函数推迟到程序运行时期载入的好处：
1. 可以实现进程之间的资源共享。
什么概念呢？就是说，某个程序的在运行中要调用某个动态链接库函数的时候，[操作系统](http://lib.csdn.net/base/operatingsystem)首先会查看所有正在运行的程序，看在内存里是否已有此库函数的拷贝了。如果有，则让其共享那一个拷贝；只有没有才链接载入。这样的模式虽然会带来一些“动态链接”额外的开销，却大大的节省了系统的内存资源。C的标准库就是动态链接库，也就是说系统中所有运行的程序共享着同一个C标准库的代码段。
2. 将一些程序升级变得简单。用户只需要升级动态链接库，而无需重新编译链接其他原有的代码就可以完成整个程序的升级。Windows 就是一个很好的例子。
3. 甚至可以真正坐到链接载入完全由程序员在程序代码中控制。
程序员在编写程序的时候，可以明确的指明什么时候或者什么情况下，链接载入哪个动态链接库函数。你可以有一个相当大的软件，但每次运行的时候，由于不同的操作需求，只有一小部分程序被载入内存。所有的函数本着“有需求才调入”的原则，于是大大节省了系统资源。比如现在的软件通常都能打开若干种不同类型的文件，这些读写操作通常都用动态链接库来实现。在一次运行当中，一般只有一种类型的文件将会被打开。所以直到程序知道文件的类型以后再载入相应的读写函数，而不是一开始就将所有的读写函数都载入，然后才发觉在整个程序中根本没有用到它们。
三、动态链接库的创建
由于动态链接库函数的共享特性，它们不会被拷贝到可执行文件中。在编译的时候，编译器只会做一些函数名之类的检查。在程序运行的时候，被调用的动态链接库函数被安置在内存的某个地方，所有调用它的程序将指向这个代码段。因此，这些代码必须实用相对地址，而不是绝对地址。在编译的时候，我们需要告诉编译器，这些对象文件是用来做动态链接库的，所以要用地址不无关代码（Position Independent Code （PIC））。
对gcc编译器，只需添加上 -fPIC 标签，如：
gcc -fPIC -c file1.c
gcc -fPIC -c file2.c
gcc -shared libxxx.so file1.o file2.o
注意到最后一行，-shared 标签告诉编译器这是要建立动态链接库。这与静态链接库的建立很不一样，后者用的是 ar 命令。也注意到，动态链接库的名字形式为 “libxxx.so” 后缀名为 “.so”
四、动态链接库的使用
使用动态链接库，首先需要在编译期间让编译器检查一些语法与定义。
这与静态库的实用基本一样，用的是 -Lpath 和 -lxxx 标签。如：
gcc file1.o file2.o -Lpath -lxxx -o program.exe
编译器会先在path文件夹下搜索libxxx.so文件，如果没有找到，继续搜索libxxx.a（静态库）。
在程序运行期间，也需要告诉系统去哪里找你的动态链接库文件。在UNIX下是通过定义名为 LD_LIBRARY_PATH 的环境变量来实现的。只需将path赋值给此变量即可。csh 命令为：
setenv LD_LIBRARY_PATH   your/full/path/to/dll
一切安排妥当后，你可以用 ldd 命令检查是否连接正常。
ldd program.exe
五、举例
一使用库必要性
提高代码重用性。
二库的种类：
 1 静态库
一般以*.a命名。
程序编译时被加载，此后，只要程序不被重新编译，静态库就没有作用了（可以删掉）。
由于静态库的代码在编译过程中已经被载入可执行程序，因此体积较大，如果有多个应用程序都用了同一个静态库，在存放可执行程序的硬盘中就会有这个静态库的多份拷贝。如果他们同时在运行，那么在内存中也会有这个静态库的多份拷贝。但是如下面提到的动态库相比较，程序执行时间比较短，因为没有执行时库函数的加载。所谓“以空间换时间”。
下面我们用一个实例说明静态库的编程和使用。
     //库函数：hellowlib.c
     #include
void printhellow()    
{
           printf("hellow,now in lib routine\n");
           return ;
}
首先生成目标文件：gcc -c  printhellow.c  –o printhellow.o
然后使用ar（archive）命令把目标文件制作库文件：ar cqs libhello.a printhellow.o
注意库文件名一定是lib***.a格式，不要忘了加lib作为前缀。
下面我们写一个程序调用静态库libhello.a中的printhellow函数。
//testlib.c
int main(int arc, char **argv)
{
   printhellow();
    return ;
}
下面编译：gcc -o testlib testlib.c -L ./  -lhello
即可生成可执行文件testlib。
注意上面的-L（大写）指示库的路径在当前目录下。如果没有这个选项，就需要把库libhello.a加入到标准路径中。如/usr/lib中。-l（小写）只需要跟hello，其他字符全部不要，否则出错。
   2 动态库（共享库）
一般以.so命名（share object）
与静态库不同，共享库的代码是在可执行程序运行时才载入内存的，在编译过程中仅简单的引用，因此代码体积较小。与上面提到的静态库相比，很是节约空间。但运行时需要载入，因此运行时间相对静态库而言比较长。所谓“以时间换空间”。
动态链接的意思就是在程序装载内存的时候才真正的把库函数代码链接进行确定它们的地址，并且就算有几个程序同时运行，内存也只存在一份函数代码。
动态库的代码要实现这样的功能，必须满足这样一种条件：能够被加载到不同进程的不同地址，所以代码要经过特别的编译处理，我们把这种经过特别处理的代码叫做“位置无关代码（Position independed Code .PIC）”。
位置无关代码可以这样看，内存中的动态代码只有一份副本，但动态库的数据却可能有多份。由于每一个链接到动态的进程都可能会修改库的数据，所以每当有这种情况，操作系统就复制出一份数据副本，然后修改进程的地址空间映射，使它指向新的数据副本，这样，进程最后修改的只是属于自己的那份数据。
更详细来说，某个程序的在运行中要调用某个动态链接库函数的时候，操作系统首先会查看所有正在运行的程序，看在内存里是否已有此库函数的拷贝了。如果有，则让其共享那一个拷贝；只有没有才链接载入。这样的模式虽然会带来一些“动态链接”额外的开销，却大大的节省了系统的内存资源。我们所知C的标准库就是动态链接库，即系统中所有运行的程序共享着同一个C标准库的代码段。
正如刚才所说，由于动态链接库函数不会被拷贝到可执行文件中。编译的时编译器只会做一些函数名之类的检查。程序运行时，被调用的动态链接库函数被安置在内存的某个地方，所有调用它的程序将指向这个代码段。因此，这些代码必须是相对地址，而不是绝对地址。在编译的时候，我们需要告诉编译器，这些对象文件是用来做动态链接库的，所以要用地址无关代码（Position Independent Code （PIC））。
共享库又可分为动态链接（Dynamic linking）和动态加载（Dynamic loading）两种。
  2.1  动态链接（Dynamic linking）
    在编译程序时指定要连接的库，然后再程序运行时一开始就将库载入。这也称为隐式调用库函数。
     示例仍然使用钢材程序，编译库：
      gcc -fpic -shared -o libhello.so printhellow.c
      -fpic便指示了这是地址无关代码，-shared指示是一个共享库
编译程序，和静态库使用方法一样：gcc -o testlib testlib.c -L ./ -lhello
      这时候编译成功，但我们执行testlib却发生以下错误：
      ./testlib: error while loading shared libraries: libhello.so: cannot load shared object file: No such file or directory
   显然，程序加载时到标准路径中找不到库文件。
解决办法 1）把我们生成的库放到标准路径中去即可。
              mv libhello.so  /usr/lib
            2）建立符号连接 ln -s `pwd`/libhello.so /usr/lib/libhello.so
            3）将动态链接库所在目录名追加到动态链接库配置文件/etc/ld.so.conf中
               pwd >> /etc/ld.so.conf
               ldconfig
            4）利用动态链接库管理命令ldconfig,强制其搜索指定目录,并更新缓存文件,# ldconfig `pwd`
              但是这种方法只是暂时的，如果再次运行ldconfig, 缓存文件内容可能改变,所需的动态链接库可能不被系统共享了。
            5)编译时指定库的加载路径： gcc -o testlib testlib.c -L ./ -lhello -Wl, -rpath ./
             这里， -rpath 说明了程序运行时库的加载路径，因为-rpath是ld命令的选项，所以gcc调用它时需要使用gcc的 -Wl选项。
  2.2  动态加载（Dynamic loading）
程序不会自动加载库，需要在程序中由程序员指定何时加载。系统提供了一套动态加载API以方便我们使用。
先看一下这些API：
1）
#include
void *dlopen( const char *file, int mode );
dlopen第一个参数是共享库的名称，会在以下路径中查找指定的共享库：
①环境变量LD_LIBRARY_PATH中指定的
   ②文件/etc/ld.so.cache中找到的库的列表，由ldconfig命令刷新。
   ③目录usr/lib。
   ④目录/lib。
   ⑤当前目录。
第二个参数为打开共享库的方式。有两个值
   ①RTLD_NOW：将共享库中的所有函数加载到内存
   ②RTLD_LAZY：稍后进行共享库中的函数的加载，调用dlsym()时加载某函数
2）
char *dlerror();
用dlerror()函数测试是否打开成功，并进行错误处理;
3）
void *dlsym( void *restrict handle, const char *restrict name );
用dlsym获得函数地址,存放在一个函数指针中，用获得的函数指针进行函数调用。
4）
char *dlclose( void *handle );
程序结束时用dlclose关闭打开的动态库，防止资源泄露。
下面是testdyn.c程序实例：
#include
#include
#include
int main(int arc, char **argv)
{
    void (*pfunc)(); /* pointer of function */
    char *perr;  
    /* load shared library dynamicly */
    void *handle = dlopen("libhello.so", RTLD_LAZY);
    if(NULL == handle)
    {
              printf("dlopen error,errno = %d\n",errno);
              return -1;
    }
    perr = dlerror();
    if(NULL != perr)
    {
              printf("dlerror first \n");
              return -1;
    }
    /* get the address of printhellow function */
    pfunc = dlsym(handle, "printhellow");
    perr = dlerror();
    if(NULL != perr)
    {
              printf("dlerror when dlsym \n");
              return -1;
    }
    /* call the function */
    (*pfunc)();
    dlclose(handle);
    printf("load share library success!\n") ;  
    return -1;
}
编译：gcc -o testdyn testdyn.c **–ldl**
**注意加上–ldl参数。**
程序运行：
[root@VMwareLin7 fengxz]# ./testdyn
hellow,now in lib routine
load share library success!
