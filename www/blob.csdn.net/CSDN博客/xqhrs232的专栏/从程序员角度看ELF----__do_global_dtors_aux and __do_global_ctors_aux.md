# 从程序员角度看ELF----__do_global_dtors_aux and __do_global_ctors_aux - xqhrs232的专栏 - CSDN博客
2018年04月09日 14:18:05[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：311
原文地址::[http://blog.chinaunix.net/uid-20605433-id-1617450.html](http://blog.chinaunix.net/uid-20605433-id-1617450.html)
相关文章
1、glibc全局构造与析构(解释了_do_global_ctors_aux)----[http://www.xuebuyuan.com/839077.html](http://www.xuebuyuan.com/839077.html)
2、[[问题解析] 记一次__do_global_ctors_aux导致的coredump](https://segmentfault.com/a/1190000008412227)----[https://segmentfault.com/a/1190000008412227](https://segmentfault.com/a/1190000008412227)
3、__do_global_dtors_aux and __do_global_ctors_aux----[http://www.it1352.com/298012.html](http://www.it1352.com/298012.html)
4、[__do_global_dtors_aux
 and __do_global_ctors_aux](https://stackoverflow.com/questions/6477494/do-global-dtors-aux-and-do-global-ctors-aux)----[https://stackoverflow.com/questions/6477494/do-global-dtors-aux-and-do-global-ctors-aux](https://stackoverflow.com/questions/6477494/do-global-dtors-aux-and-do-global-ctors-aux)
创建时间：2001-09-15
文章属性：翻译
文章来源：[http://www.xfocus.org](http://www.xfocus.org/)
文章提交：[alert7](https://www.xfocus.net/bbs/index.php?lang=cn&act=Profile&do=03&MID=768) (sztcww_at_sina.com)
原文:《 ELF:From The Programmer's Perspective》
作者：Hongjiu Lu 
    NYNEX Science & Technology, Inc. 
    500 Westchester Avenue 
    White Plains, NY 10604, USA 
翻译：alert7 <mailto: alert7@21cn.com="" alert7@xfocus.org
主页: [http://www.xfocus.org](http://www.xfocus.org/)
时间: 2001-9-10
**★概要：**
这片文档从程序员的角度讨论了linux的ELF二进制格式。介绍了一些ELF执行
文件在运行控制的技术。展示了如何使用动态连接器和如何动态装载ELF。
我们也演示了如何在LINUX使用GNU C/C++编译器和一些其他工具来创建共享的
C/C++库。
**★1 前言**
最初，UNIX系统实验室(USL)开发和发布了Executable and linking Format
(ELF)这样的二进制格式。在SVR4和Solaris 2.x上，都做为可执行文件默认的
二进制格式。ELF比a.out和COFF更强大更灵活。结合一些适当的工具，程序员
使用ELF就可以在运行时控制程序的流程。
**★2 ELF类型**
三种主要的ELF文件类型：
.可执行文件：包含了代码和数据。具有可执行的程序。 
    例如这样一个程序
    # file dltest
    dltest: ELF 32-bit LSB executable, Intel 80386, version 1, 
        dynamically linked (uses shared libs), not stripped
.可重定位文件：包含了代码和数据（这些数据是和其他重定位文件和共享的
    object文件一起连接时使用的）
    例如这样文件
    # file libfoo.o
    libfoo.o: ELF 32-bit LSB relocatable, Intel 80386, version 1, 
       not stripped
.共享object文件（又可叫做共享库）：包含了代码和数据（这些数据是在连接
    时候被连接器ld和运行时动态连接器使用的）。动态连接器可能称为
    ld.so.1,libc.so.1 或者 ld-linux.so.1。
    例如这样文件
    # file libfoo.so
    libfoo.so: ELF 32-bit LSB shared object, Intel 80386, version
    1, not stripped
ELF section部分是非常有用的。使用一些正确的工具和技术，程序员就能
熟练的操作可执行文件的执行。
**★3 .init和.fini sections**
在ELF系统上，一个程序是由可执行文件或者还加上一些共享object文件组成。
为了执行这样的程序，系统使用那些文件创建进程的内存映象。进程映象
有一些段(segment),包含了可执行指令，数据，等等。为了使一个ELF文件
装载到内存，必须有一个program header(该program header是一个描述段
信息的结构数组和一些为程序运行准备的信息)。
一个段可能有多个section组成.这些section在程序员角度来看更显的重要。
每个可执行文件或者是共享object文件一般包含一个section table,该表
是描述ELF文件里sections的结构数组。这里有几个在ELF文档中定义的比较
特别的sections.以下这些是对程序特别有用的：
.fini
    该section保存着进程终止代码指令。因此，当一个程序正常退出时，        
    系统安排执行这个section的中的代码。
.init    
    该section保存着可执行指令，它构成了进程的初始化代码。
    因此，当一个程序开始运行时，在main函数被调用之前(c语言称为
    main)，系统安排执行这个section的中的代码。
.init和.fini sections的存在有着特别的目的。假如一个函数放到
.init section，在main函数执行前系统就会执行它。同理，假如一
个函数放到.fini section，在main函数返回后该函数就会执行。
该特性被C++编译器使用，完成全局的构造和析构函数功能。
当ELF可执行文件被执行，系统将在把控制权交给可执行文件前装载所以相关
的共享object文件。构造正确的.init和.fini sections,构造函数和析构函数
将以正确的次序被调用。
**★3.1 在c++中全局的构造函数和析构函数**
在c++中全局的构造函数和析构函数必须非常小心的处理碰到的语言规范问题。
构造函数必须在main函数之前被调用。析构函数必须在main函数返回之后
被调用。例如，除了一般的两个辅助启动文件crti.o和crtn.o外，GNU C/C++
编译器--gcc还提供两个辅助启动文件一个称为crtbegin.o，还有一个被称为
crtend.o。结合.ctors和.dtors两个section，c++全局的构造函数和析构函数
能以运行时最小的负载，正确的顺序执行。
.ctors
    该section保存着程序的全局的构造函数的指针数组。
.dtors
    该section保存着程序的全局的析构函数的指针数组。    
ctrbegin.o
    有四个section:
    1 .ctors section
    local标号__CTOR_LIST__指向全局构造函数的指针数组头。在
    ctrbegin.o中的该数组只有一个dummy元素。
    [译注：
    # objdump -s -j .ctors                 
    /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/crtbegin.o
    /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/crtbegin.o:
    file format elf32-i386
    Contents of section .ctors:
    0000 ffffffff                             ....
    这里说的dummy元素应该就是指的是ffffffff
    ]
    2 .dtors section
    local标号__DTOR_LIST__指向全局析构函数的指针数组头。在
    ctrbegin.o中的该数组仅有也只有一个dummy元素。
    3 .text section
    只包含了__do_global_dtors_aux函数,该函数遍历__DTOR_LIST__
    列表，调用列表中的每个析构函数。
函数如下：
Disassembly of section .text:
00000000 <__do_global_dtors_aux>:
   0:   55                      push   %ebp
   1:   89 e5                   mov    %esp,%ebp
   3:   83 3d 04 00 00 00 00    cmpl   $0x0,0x4
   a:   75 38                   jne    44 <__do_global_dtors_aux+0x44>
   c:   eb 0f                   jmp    1d <__do_global_dtors_aux+0x1d>
   e:   89 f6                   mov    %esi,%esi
  10:   8d 50 04                lea    0x4(%eax),%edx
  13:   89 15 00 00 00 00       mov    %edx,0x0
  19:   8b 00                   mov    (%eax),%eax
  1b:   ff d0                   call   *%eax
  1d:   a1 00 00 00 00          mov    0x0,%eax
  22:   83 38 00                cmpl   $0x0,(%eax)
  25:   75 e9                   jne    10 <__do_global_dtors_aux+0x10>
  27:   b8 00 00 00 00          mov    $0x0,%eax
  2c:   85 c0                   test   %eax,%eax
  2e:   74 0a                   je     3a <__do_global_dtors_aux+0x3a>
  30:   68 00 00 00 00          push   $0x0
  35:   e8 fc ff ff ff          call   36 <__do_global_dtors_aux+0x36>
  3a:   c7 05 04 00 00 00 01    movl   $0x1,0x4
  41:   00 00 00
  44:   c9                      leave
  45:   c3                      ret
  46:   89 f6                   mov    %esi,%esi
    4 .fini section
    它只包含一个__do_global_dtors_aux的函数调用。请记住，它仅是
    一个函数调用而不返回的，因为crtbegin.o的.fini section是这个
    函数体的一部分。
函数如下：
Disassembly of section .fini:
00000000 <.fini>:
   0:   e8 fc ff ff ff          call   1 <.fini+0x1>
crtend.o
    也有四个section:
    1 .ctors section
    local标号__CTOR_END__指向全局构造函数的指针数组尾部。
    2 .dtors section
    local标号__DTOR_END__指向全局析构函数的指针数组尾部。
    3 .text section
    只包含了__do_global_ctors_aux函数,该函数遍历__CTOR_LIST__
    列表，调用列表中的每个构造函数。
函数如下：
00000000 <__do_global_ctors_aux>:
   0:   55                      push   %ebp
   1:   89 e5                   mov    %esp,%ebp
   3:   53                      push   %ebx
   4:   bb fc ff ff ff          mov    $0xfffffffc,%ebx
   9:   83 3d fc ff ff ff ff    cmpl   $0xffffffff,0xfffffffc
  10:   74 0c                   je     1e <__do_global_ctors_aux+0x1e>
  12:   8b 03                   mov    (%ebx),%eax
  14:   ff d0                   call   *%eax
  16:   83 c3 fc                add    $0xfffffffc,%ebx
  19:   83 3b ff                cmpl   $0xffffffff,(%ebx)
  1c:   75 f4                   jne    12 <__do_global_ctors_aux+0x12>
  1e:   8b 5d fc                mov    0xfffffffc(%ebp),%ebx
  21:   c9                      leave
  22:   c3                      ret
  23:   90                      nop
    4 .init section
    它只包含一个__do_global_ctors_aux的函数调用。请记住，它仅是
    一个函数调用而不返回的，因为crtend.o的.init section是这个函
    数体的一部分。
函数如下：
Disassembly of section .init:
00000000 <.init>:
   0:   e8 fc ff ff ff          call   1 <.init+0x1>
crti.o
    在.init section中仅是个_init的函数标号。
    在.fini section中的_fini函数标号。
crtn.o
    在.init和.fini section中仅是返回指令。
Disassembly of section .init:
00000000 <.init>:
   0:   8b 5d fc                mov    0xfffffffc(%ebp),%ebx
   3:   c9                      leave
   4:   c3                      ret
Disassembly of section .fini:
00000000 <.fini>:
   0:   8b 5d fc                mov    0xfffffffc(%ebp),%ebx
   3:   c9                      leave
   4:   c3                      ret
编译产生可重定位文件时，gcc把每个全局构造函数挂在__CTOR_LIST上
（通过把指向构造函数的指针放到.ctors section中）。
它也把每个全局析构函挂在__DTOR_LIST上（通过把指向析构函的指针
放到.dtors section中）。
连接时，gcc在所有重定位文件前处理crtbegin.o,在所有重定位文件后处理
crtend.o。另外，crti.o在crtbegin.o之前被处理，crtn.o在crtend.o之后
被处理。
当产生可执行文件时，连接器ld分别的连接所有可重定位文件的ctors 和
.dtors section到__CTOR_LIST__和__DTOR_LIST__列表中。.init section
由所有的可重定位文件中_init函数组成。.fini由_fini函数组成。
运行时，系统将在main函数之前执行_init函数，在main函数返回后执行
_fini函数。
**★4 ELF的动态连接与装载**
**★4.1 动态连接**
当在UNIX系统下，用C编译器把C源代码编译成可执行文件时，c编译驱动器一般
将调用C的预处理，编译器，汇编器和连接器。
.     c编译驱动器首先把C源代码传到C的预处理器，它以处理过的宏和
    指示器形式输出纯C语言代码。
.    c编译器把处理过的C语言代码翻译为机器相关的汇编代码。
.    汇编器把结果的汇编语言代码翻译成目标的机器指令。结果这些
    机器指令就被存储成指定的二进制文件格式，在这里，我们使用的
    ELF格式。
.    最后的阶段，连接器连接所有的object文件，加入所有的启动代码和
    在程序中引用的库函数。
    下面有两种方法使用lib库
    --static library
    一个集合，包含了那些object文件中包含的library例程和数据。用
    该方法，连接时连接器将产生一个独立的object文件(这些
    object文件保存着程序所要引用的函数和数据)的copy。
    --shared library
    是共享文件，它包含了函数和数据。用这样连接出来的程序仅在可执行
    程序中存储着共享库的名字和一些程序引用到的标号。在运行时，动态
    连接器（在ELF中也叫做程序解释器）将把共享库映象到进程的虚拟
    地址空间里去，通过名字解析在共享库中的标号。该处理过程也称为
    动态连接(dynamic linking)
程序员不需要知道动态连接时用到的共享库做什么，每件事情对程序员都是
透明的。
**★4.2 动态装载(Dynamic Loading)**
动态装载是这样一个过程：把共享库放到执行时进程的地址空间，在库中查找
函数的地址，然后调用那个函数，当不再需要的时候，卸载共享库。它的执行
过程作为动态连接的服务接口。
在ELF下，程序接口通常在中被定义。如下：
void *dlopen(const char * filename,int flag);
const char * dlerror(void);
const void * dlsym (void handle*,const char * symbol);
int dlclose(void * handle);
这些函数包含在libdl.so中。
下面是个例子，展示动态装载是如何工作的。
主程序在运行时动态的装载共享库。
一方面可指出哪个共享库被使用，哪个函数被调用。一方面也能在访问共享库中的数据。 
[alert7@redhat62 dl]# cat**dltest.c**
#include 
#include 
#include 
#include 
#include 
typedef void (*func_t) (const char *);
void dltest(const char *s)
{
    printf("From dltest:");
    for (;*s;s++)
    {    
        putchar(toupper(*s));
    }
    putchar('\n');
}
main(int argc,char **argv)
{
void *handle;
func_t fptr;
char * libname = "./libfoo.so";
char **name=NULL;
char *funcname = "foo";
char *param= "Dynamic Loading Test";
int ch;
int mode=RTLD_LAZY;    
while ((ch = getopt(argc,argv,"a:b:f:l:"))!=EOF)
{
    switch(ch)
    {
    case 'a':/*argument*/
        param=optarg;
        break;
    case 'b':/*how to bind*/
        switch(*optarg)
        {
        case 'l':/*lazy*/
        mode = RTLD_LAZY;
        break;
        case 'n':/*now*/
        mode = RTLD_NOW;
        break;
        }
        break;
    case 'l':/*which shared library*/
        libname= optarg;
        break;
    case 'f':/*which function*/
        funcname= optarg;
    }
   }
handle = dlopen(libname,mode);
if (handle ==NULL)
{
fprintf(stderr,"%s:dlopen:'%s'\n",libname,dlerror());
exit(1);
}
fptr=(func_t)dlsym(handle,funcname);
if (fptr==NULL)
{
fprintf(stderr,"%s:dlsym:'%s'\n",funcname,dlerror());
exit(1);
}
name = (char **) dlsym(handle,"libname");
if (name==NULL)
{
fprintf(stderr,"%s:dlsym:'libname'\n",dlerror());
exit(1);
}
printf("Call '%s' in '%s':\n",funcname,*name);
/*call that function with 'param'*/
(*fptr)(param);
dlclose(handle);
return 0;
}
这里有两个共享库，一个是libfoo.so，一个是libbar.so。
每个都用同样的全局字符串变量libname,分别各自有foo和bar函数。
通过dlsym，对程序来说,他们都是可用的。
[alert7@redhat62 dl]# cat **libbar.c**
#include 
extern void dltest(const char *);
const char * const libname = "libbar.so";
void bar (const char *s)
{
dltest("Called from libbar.");
printf("libbar:%s\n",s);
}
[alert7@redhat62 dl]# cat **libfoo.c**
#include 
extern void dltest (const char *s);
const char *const libname="libfoo.so";
void foo(const char *s)
{
    const char *saved=s;
    dltest("Called from libfoo");
    printf("libfoo:");
    for (;*s;s++);
    for (s--;s>=saved;s--)
    {
    putchar (*s);
    }
    putchar ('\n');
}
使用Makefile文件来编译共享库和主程序是很有用的。
因为libbar.so和libfoo.so也调用了主程序里的dltest函数。
[alert7@redhat62 dl] #cat **Makefile**
CC=gcc
LDFLAGS=-rdynamic
`#-rdynamic : http://gcc.gnu.org/onlinedocs/gcc/Link-Options.html`
#Pass the flag -export-dynamic to the ELF linker, on targets that support
 it.
#This instructs the linker to add all symbols, not only used ones, to the
#dynamic symbol table. This option is needed for some uses of `dlopen` or to 
#allow obtaining backtraces from within a program.
SHLDFLAGS=
RM=rm
all:dltest
libfoo.o:libfoo.c
    $(CC) -c -fPIC $<
#-fPIC : position-independent code  
#If supported for the target machine,emit position-independent code,
#suitable for dynamic linking,even if branches need large displacements.     
#产生位置无关代码（PIC），一般创建共享库时用到。     
#在x86上，PIC的代码的符号引用都是通过ebx进行操作的。
libfoo.so:libfoo.o
    $(CC) $(SHLDFLAGS) -shared -o $@ $^
libbar: libbar.c
    $(CC) -c -fPIC $<
libbar.so:libbar.o
    $(CC) $(SHLDFLAGS) -shared -o $@ $^
#-shared：生成一个共享目标文件,他可以和其他目标文件连接产生可执行文件.只有部分系统支持该选项.
dltest: dltest.o libbar.so libfoo.so
    $(CC) $(LDFLAGS) -o $@ dltest.o -ldl
clean:
    $(RM) *.o *.so dltest
**处理流程：**
[alert7@redhat62 dl]# export ELF_LD_LIBRARY_PATH=.
[alert7@redhat62 dl]# ./dltest
Call 'foo' in 'libfoo.so':
From dltest:CALLED FROM LIBFOO
libfoo:tseT gnidaoL cimanyD
[alert7@redhat62 dl]# ./dltest -f bar
bar:dlsym:'./libfoo.so: undefined symbol: bar'
[alert7@redhat62 dl]# ./dltest -f bar -l ./libbar.so
Call 'bar' in 'libbar.so':
From dltest:CALLED FROM LIBBAR.
libbar:Dynamic Loading Test
在动态装载进程中调用的第一个函数就是dlopen,它使得共享可库对运行着的进程可用。
dlopen返回一个handle,该handle被后面的dlsym和dlclose函数使用。
dlopen的参数为NULL有特殊的意思---它使得在程序导出的标号和当前已经装载进内存的共享库导出的标号通过dlsym就可利用。
在一个共享库已经装载进运行着的进程的地址空间后，dlsym可用来获得在共享库中导出的标号地址。然后就可以通过dlsym返回的地址来访问里面的函数和数据。
当一个共享库不再需要使用的时候，就可以调用dlclose卸载该函数库。
假如共享库在启动时刻或者是通过其他的dlopen调用被装载的话，该共享库不会从调用的进程的地址空间被移走。
假如dlclose操作成功，返回为0。dlopen和dlsym如果有错误，将返回为NULL。
为了获取诊断信息，可调用dlerror.
**★5 支持ELF的LINUX上的编译器GNU GCC**
感谢Eric Youngdale (eric@aib.com),lan Lance Taylor (ian@cygnus.com)还有许多为gcc支持ELF功能的默默做贡献的人。
我们能用gcc和GNU的二进制工具很容易的创建ELF可执行文件和共享库。
**★5.1 共享C库 Shared C Library**
在ELF下构造一个共享库比其他的容易的多。但是需要编译器，汇编器，连接器的支持。
首先，需要产生位置无关(position-independent)代码。
要做到这一点，gcc需要加上编译选项-fPIC
[alert7@redhat62 dl]# gcc -fPIC -O -c libbar.c
这时候就适合构造共享库了，加上-shared编译选项
[alert7@redhat62 dl]# gcc -shared -o libbar.so libbar.o
现在我们构造的libbar.so就可以被连接器(link editor)和动态连接器(dynamic linker)使用。
只要编译时带上-fPIC编译选项,可以把许多重定位文件加到共享库中。
为了把baz.o和共享库连接在一起，可以如下操作：
# gcc -O -c baz.c
# gcc -o baz baz.o -L. -lbar
在把libbar.so安装到动态连接器能找到的正确位置上之后，运行baz将使libbar.so映象到baz的进程地址空间。
内存中libbar.so的一份拷贝将被所有的可执行文件(这些可执行程序连接时和它一块儿连接的或者在运行时动态装载的)共享。
**★5.2 共享C++库 Shared C++ Library**
在共享c++库中主要的困难是如何对待构造函数和析构函数。
在SunOS下，构造和使用一个共享的ELF C库是容易的，但是在SunOS下不能
构造共享的C++库，因为构造函数和析构函数有特别的需求。为止，在ELF
中的.init和.init section提供了完美的解决方法。
当构造共享C++库时，我们使用crtbegin.o和crtend.o这两个特殊的版本，
(它们已经是经过-fPIC的)。对于连接器(link editor)来说，构造共享
的C++库几乎是和一般的可执行文件一样的。全局的构造函数和析构函数
被.init和.fini section处理(在上面3.1节中已经讨论过)。
但一个共享库被映射到进程的地址空间时，动态连接器将在传控制权给程序
之前执行_init函数，并且将为_fini函数安排在共享库不再需要的时候被
执行。
连接选项-shared是告诉gcc以正确的顺序放置必要的辅助文件并且告诉它将
产生一个共享库。-v选项将显示什么文件什么选项被传到了连接器
(link editor).
[alert7@redhat62 dl]# gcc -v -shared -o libbar.so libbar.o
Reading specs from /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/specs
gcc version egcs-2.91.66 19990314/Linux (egcs-1.1.2 release)
/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/collect2 -m elf_i386 
-shared -o libbar.so /usr/lib/crti.o /usr/lib/gcc-lib/i386-redhat
    -linux/egcs-2.91.66/crtbeginS.o 
-L/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66 
-L/usr/i386-redhat-linux/lib libbar.o -lgcc -lc --version-script 
/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/libgcc.map 
-lgcc /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/crtendS.o 
/usr/lib/crtn.o
crtbeginS.o和crtendS.o用-fPIC编译的两个特殊的版本。带上-shared
创建共享库是重要的，因为那些辅助的文件也提供其他服务。我们将在
5.3节中讨论。
**★5.3 扩展的GCC特性**
GCC有许多扩展的特性。有些对ELF特别的有用。
其中一个就是__attribute__。
使用__attribute__可以使一个函数放到__CTOR_LIST__或者__DTOR_LIST__里。
例如：
[alert7@redhat62 dl]# cat **miss.c**
#include 
#include 
static void foo(void) __attribute__ ((constructor));
static void bar(void) __attribute__ ((destructor));
int main(int argc, char *argv[])
{
        printf("foo == %p\n", foo);
        printf("bar == %p\n", bar);
        exit(EXIT_SUCCESS);
}
void foo(void)
{
        printf("hi dear njlily!\n");
}
void bar(void)
{
        printf("missing u! goodbye!\n");
}
[alert7@redhat62 dl]# gcc -o miss miss.c
[alert7@redhat62 dl]# ./miss
hi dear njlily!
foo == 0x8048434
bar == 0x8048448
missing u! goodbye!
我们来看看是否加到了.ctors和.dtors中。
[alert7@redhat62 dl]# **objdump -s -j .ctors miss**
miss:     file format elf32-i386
Contents of section .ctors:
8049504 ffffffff 34840408 00000000           ....4.......
[alert7@redhat62 dl]# objdump -s -j .dtors miss
miss:     file format elf32-i386
Contents of section .dtors:
8049510 ffffffff 48840408 00000000           ....H.......
已经把foo和bar地址分别放到了.ctors和.dors
显示34840408只是因为x86上是LSB编码的，小端序。
__attribute__ ((constructor))促使函数foo在进入main之前会被自动调用。
__attribute__ ((destructor))促使函数bar在main返回或者exit调用之后会被自动调用。
foo和bar必须是不能带参数的而且必须是static void类型的函数。
在ELF下，这个特性在一般的可执行文件和共享库中都能很好的工作。
我们也可以创建自己的section,在这里我创建了一个alert7 section.
[alert7@redhat62 dl]# cat **test.c**
#include 
#include 
static void foo(void) __attribute__ ((section ("alert7")));
static void bar(void) __attribute__ ((section ("alert7")));
int main(int argc, char *argv[])
{
        foo();
        printf("foo == %p\n", foo);
        printf("bar == %p\n", bar);
        bar();
        exit(EXIT_SUCCESS);
}
void foo(void)
{
        printf("hi dear njlily!\n");
}
void bar(void)
{
        printf("missing u! goodbye!\n");
}
[alert7@redhat62 dl]# gcc -o test test.c
[alert7@redhat62 dl]# ./test
hi dear njlily!
foo == 0x804847c
bar == 0x8048490
missing u! goodbye!
[alert7@redhat62 dl]# objdump -x test
....
Sections:
Idx Name          Size      VMA       LMA       File off  Algn
  0 .interp       00000013  080480f4  080480f4  000000f4  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
...
12 alert7        00000026  0804847c  0804847c  0000047c  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
...
[alert7@redhat62 dl]# objdump -D test
Disassembly of section alert7:
0804847c :
804847c:       55                      push   %ebp
804847d:       89 e5                   mov    %esp,%ebp
804847f:       68 de 84 04 08          push   $0x80484de
8048484:       e8 a3 fe ff ff          call   804832c <_init+0x70>
8048489:       83 c4 04                add    $0x4,%esp
804848c:       c9                      leave
804848d:       c3                      ret
804848e:       89 f6                   mov    %esi,%esi
08048490 :
8048490:       55                      push   %ebp
8048491:       89 e5                   mov    %esp,%ebp
8048493:       68 ef 84 04 08          push   $0x80484ef
8048498:       e8 8f fe ff ff          call   804832c <_init+0x70>
804849d:       83 c4 04                add    $0x4,%esp
80484a0:       c9                      leave
80484a1:       c3                      ret
在这里，我创建了一个自己的alert7 section,并把foo,bar两个函数放到了这个section中。
一般定义的函数都会放在.text section中。
**★5.3.1 在C库中的初始化函数**
另外一个GCC的特性是__attribute__( section ("sectionname") ).
使用这个，能把一个函数或者是数据结构放到任何的section中。
static void
foo (int argc,char **argc,char **envp)
    __attribute__ ((section ("_libc_foo")));
static void 
foo (int argc,char **argv,char **envp)
{
}
static void 
bar (int argc,char **argv,char **envp)
{
}
static void * __libc_subinit_bar__
    __attribute__ (( section ("_libc_subinit")))=&(bar);
这里，我们把foo放到了_libc_foo section，把__libc_subinit_bar__放到了_libc_subinit section中。
在Linux C库中，_libc_subinit 是一个特别的section，它包含了一个函数指针（有如下原型）的数组。
void (*) (int argc,char **argv,char **envp);
这里的argc,argv,envp跟在main中的有同样的意义。
该section中的函数在进入main函数之前就会被调用。
这是很有用的，可用来在Linux C库中初始化一些全局变量。
译注：
_libc_subinit section真有这个特别的功能吗？我是没有试成功，如果有人试成功或者认为我理解有误的地方，千万记得mail给我:) 
测试程序如下：
    #include 
    #include 
    static void foo(int argc,char **argv,char **envp)
    {
        printf("hi dear njlily!\n");
    }
    int main(int argc, char *argv[])
    {
        printf("foo == %p\n", foo);
        exit(EXIT_SUCCESS);
    }
    static void * __libc_subinit_bar__
            __attribute__ (( section ("_libc_subinit")))=&(foo);
    [alert7@redhat62 dl]# gcc -o test1 test1.c
    [alert7@redhat62 dl]# ./test1
    foo == 0x8048400
用objdump，显示已经创建了一个_libc_subinit section,并且该section前四个字节就是foo地址0x8048400
**★5.4 利用GCC和GNU ld**
这一些命令行的选项对GCC和GNU ld创建ELF时特别有用。
**-shared**告诉gcc产生一个共享库，该共享库能在连接时和其他的共享文件一起形成可执行文件，该共享库也能在运行时装载进可执行文件的地址空间。
使用-shared是创建一个共享ELF库的首选方法。
另外一个有用的命令行选项是**-Wl,ldoption**,传递参数ldoption作为连接器的选项。假如ldoption包含多个逗号，将分离成多个选项。
**-static**选项将产生一个和static库一道连接的可执行文件。
当没有开启-static选项时，连接器首先试着用共享库，假如共享版本不可用，然后再试着用静态(static)库。
这里还有些特别的命令行选项对ELF来说特别的或者说是有用的。
**-dynamic-linker file**
设置动态连接器(dynamic linker)的名字。默认的动态连接器是/usr/lib/libc.so.1或者是/usr/lib/libd1.so.1
**-export-dynamic**
告诉连接器使在可执行文件中的所有标号对动态连接器可用。当一个动态装载进的共享库参考可执行文件中的标号，该标号一般在动态连接时是不可用时，这时候就特别有用。
**-lfile**
加文件到需要连接的列表中。该选项可用在许多时候。
ld将搜索它的path-list查找文件libfile.so(也就是说假如库为libbar.so，那么使用的时候就这样使用，-lbar),或者libfile.a（static版本的）。
一些情况下，共享库名libfile.so会被存储在resulting executable或者是共享库中。
当resulting executable或者是共享库被装载进内存，动态连接器也将把使用记录过的共享库装载到进程的地址空间去。
在以后的事情情况下，把必要的函数和数据被拷贝到可执行文件，减少代码长度。
**-m emulation**
仿效emulation连接器r.-V参数可列出所有可用的选项.
**-M | -Map mapfile**
把连接map输出到标准输出或者一个mapfile文件里
该连接map含有关于标号被ld映象到了哪里的一些诊断信息，还有全局共同的存储分配信息。
**-rpath directory**
加一个目录到运行时library的搜索路径。所有的-rpath参数被连接在一起然后传给动态连接器。
它们被用来在运行时定位共享库。
**-soname name**
当创建一个共享库时，指定的名字被放在共享库中。
当和这个共享库连接的可执行文件被运行，动态连接器将试着map记录着的指定名字的共享库而不是传给连接器的文件名。
**-static**
告诉连接器不要和任何共享库连接。
**-verbose**
告诉连接器打印它每个要打开的文件名。
linux下gcc beta版本使用-dynamic-linker file选项设置动态连接器为/lib/ld-linker.so.1。
该选项可以使ELF和a.out共享库很好的共存。
有件事情是另人感兴趣的。
[alert7@redhat62 dl]# gcc -shared -o libbar.so libbar.o -lfoo
假如libfoo.so被用来创建共享库时，有趣的时候就会发生了。
当libbar.so被映象到进程的地址空间的时候，动态连接器也把libfoo.so映象到内存。
假如libbar.so需要libfoo.so的时候，这个特性非常有用。
实际上使用libbars.o库的程序编译时是不需要-lfoo的。
假如archive版本的libfoo.a被使用，当在libbar.a中的标号被libbar.o引用时，它将会被搜索到。
假使在libbar.so中包含libfoo.a甚至它们根本不被libbar.o使用，在这样的情况下必须逐步把.o文件加到libbar.o中：
# rm -rf /tmp/foo
# mkdir /tmp/foo
# (cd /tmp/foo/;ar -x ....../libfoo.a)
# gcc -shared -o libbar.so libbar.o /tmp/foo/*.o
# rm -rf /tmp/foo
在libfoo.a中的.o文件必须用-fPIC编译或者至少和PIC（位置无关）是兼容的。
当使用static void * __libc_subinit_bar__
    __attribute__    ((section ("_libc_subinit")))=&(bar);
来把一个标号放到一个没有被连接器定义的section中（在这里是_libc_subinit).连接器将所有在_libc_subinit section中的标号共同创建两个标号，一个是__start__libc_subinit和__stop__libc_subinit,它们作为C的标志符被使用。
警告：
下面是完全可能的：连接器可能不能搜索到包含_libc_subinit section的文件(该section中没有程序执行需要的标号)。
这就使程序要确定使_libc_subinit section能被连接器搜索得到。
一种解决的办法是：把一个dummy标号放到_libc_subinit section中，
在文件中定义它，使它参考引用_libc_subinit section.
**★5.5 Linux下的ELF**
在Linux下ELF的执行有独特的特性，这些特性对Linux的使用者来说是很有用的。
一些Linux自己的扩展跟Solaris ELF的执行很类似。
**★5.5.1 ELF的宏(macros)**
中，定义了能维护标号的一些宏。
elf_alias(name1,name2)
    为标号name1定义一个化名name2.当文件中标号名已经被定义的时候
    应该是有很用的。
weak_alias(name1,name2)
    为标号name1定义一个弱化名name2。仅当name2没有在任何地方定义
    时，连接器就会用name1解析name2相关的符号。在文件中定义的
    标号name1也会同样处理。
elf_set_element(set,symbol)
    强迫标号成为set集合的元素。为每个set集合创建一个section.
symbol_set_declare(set)
    在该模块中宣告一个集合set.事实上宣告了两个标号：
    1  一个set的开始标号
    extern void * const __start_set
    2  一个set的结尾标号
    extern void * const __stop_set
symbol_set_first_element(set)
    返回一个指针(void * const *),指向set集合第一个元素。
symbol_set_end_p(set,ptr)
    假如ptr(void * const *)逐渐增加指向set的最后一个元素，就返回为true.
使用这些宏，程序员能任意从不同的源文件中创建列表。
**★5.5.2 library(库)的定位和搜索路径**
在Linux下，大部分系统的library库被安装在/usr/lib目录下。
只有一些基本的共享库被安装在/lib目录下。
例如：libc.so,libcurses.so,libm.so,libtermcap.so(各个版本对应的文件会有些不同)，
在其他部分被mount上之前，那些文件是启动Linux系统所必须的。
连接器默认的搜索路径是
/lib,/usr/lib,/usr/local/lib,/usr/i486-linux/lib。
环境变量LD_LIBRARY_PATH也可保存目录列表，用(:)分开，该变量被动态连接器检查并用该变量指出的目录查找共享库。
例如：/usr/X11R6/lib:/usr/local/lib:告诉动态连接器查找共享库除了现在在默认的目录查找外，然后在/usr/X11R6/lib目录，然后再是/usr/local/lib目录，然后再是当前目录。
新的环境变量ELF_LD_LIBRARY_PATH扮演着和LD_LIBRARY_PATH类似的角色。
因为LD_LIBRARY_PATH也被老的a.out DLL linux的共享库使用。为了避免来自DLL连接器的不必要的警告，对于在Linux下ELF的动态连接器来说，最好使用LD_LIBRARY_PATH环境变量。
另外一个特性是/etc/ld.so.conf文件，该文件包含了一些目录列表。
例如：
/usr/X11R6/lib
/usr/lib
/usr/kerberos/lib
/usr/i486-linux-libc5/lib
/usr/lib/gconv
/usr/lib/qt-2.1.0/lib
/usr/lib/qt-1.45/lib
程序ldconfig将把/etc/ld.so.conf文件中列出的搜索目录中的所有的共享库存储到/etc/ld.so.cache中。
假如共享库已经被从默认的目录中移走，Linux ELF动态连接库将在/etc/ld.so.cache文件中找该共享库。
**★5.5.3 共享库的版本**
在ELF系统上，假如两个共享库有同样的应用程序二进制接口(ABI)的子集的话,对那些仅仅使用那些ABI子集的程序来说，这两个共享库是可以互相通用的（当然那两个共享库有同样的函数功能）。
当一个库被改变，只要新的ABI和前面一个版本的共享库有100%的兼容的话，所有和前面版本连接的程序在新的共享库下也能很好的运行。
为了支持这，foo库必须小心的维护：
1.这个共享库应该如下构造：
[alert7@redhat62 dl]# gcc -shared -Wl,-soname,libfoo.so.major \
    -o libfoo.so.major.minor.patch-level libfoo.o
动态连接器运行时将试着定位和映象libfoo.so.major而不管事实上用的共享文件名libfoo.so.major.patch-level。
2.一个符号连接应该指向正确的共享库
[alert7@redhat62 dl]# ln -s libfoo.so.major.minor.patch-level \
    libfoo.so.major
3.当ABI改变和原来版本不兼容的时，主(major)版本号应该升级。
当搜索共享库的时候，Linux连接器将使用最新的共享库（它们有最高的major,minor和patch level的版本号）。
**★5.5.4 共享(shared)库和静态(static)库的混合连接**
默认情况下，假如共享库可用，连接器会使用共享库。
但是-Bdynamic和-Bstatic提供了很好控制库的方法。它们可以决定用共享库还是用静态库。
传-Bdynamic和-Bstatic选项给连接器，如下操作：
# gcc -o main main.o -Wl,-Bstatic \
    -lfoo -Wl,-Bdynamic -lbar
# gcc -o main main.o -Wl,-Bstatic
告诉连接器所有的库(象libc等等)都使用静态的版本。
**★5.5.5 装载附加的共享库**
在ELF系统上，为了执行一个ELF文件，内核要把控制权交给动态连接器ld-linux.so.1
(在linux上动态连接器是ld-linux.so.1，版本不同也会不同的，在默认的redhat6.2上是/lib/ld-linux.so.2)。
在绝对路径/lib/ld-linux.so.1以二进制存放着。
假如动态连接器不存在，没有哪个ELF可执行文件能运行。
动态连接器执行以下步骤完成从程序到进程映象：
    1.分析可执行文件中的动态信息section，决定需要哪些库。
    2.定位和映象(map)那些共享库，并且分析它们动态信息section决定是否需要附加的共享库。
    3.为可执行程序和那些需要的共享库执行重定位。
    4.调用共享库中提供的任何初始化函数并且安排共享库提供的清除(cleanup)函数在共享库卸栽出进程空间的时候运行。
    5.传控制给程序
    6.为应用程序提供函数的迟延装定服务
    7.为应用程序提供动态转载服务。
环境变量LD_PRELOAD设置共享库名或者用":"把文件名隔开。
动态连接器在任何那些请求的共享库之前把环境变量LD_PRELOAD的共享库装载到进程地址空间去。
例如：
# LD_PRELOAD=./mylibc.so myprog
这里./mylibc.so将第一时间map到程序myprog的空间。
因为动态连接器在找寻标号的时候总是使用第一次碰到的标号，所以我们可以使用LD_PRELOAD来覆盖标准共享库中的函数。
这个特性对程序员来说是很有用的，可用来在还没有建好整个共享库的时候对单个函数功能先做调试实验。
我们可以这样：
#gcc -c -fPIC -O3 print.c
#gcc -shared print.o -o print.so.1.0
创建自己的共享连接库
**★5.5.6 Linux下动态装载(Dynamic loading)**
_dlinfo是动态连接接口库的一个函数。它列出所有映射到执行程序和通过dlopen打开的每个共享库。它的输出类试以下：
List of loaded modules
    00000000 50006163 50006200 Exe 1
    50007000 5000620c 50006200 Lib 1 /lib/elf/libd1.so.1
    5000a000 500062c8 50006200 Lib 2 /lib/elf/libc.so.4
    50000000 50006000 00000000 Int 1 /lib/elf/ld-linux.so.1
    500aa000 08006f00 08005ff0 Mod 1 ./libfoo.so
Modules for application (50006200):
    50006163
    5000620c /lib/elf/libdl.so.1
    500062c8 /lib/elf/libc.so.4
    50006000 /lib/ld-linux.so.1
Modules for handle 8005ff0
    08006f00 ./libfoo.so
    500062c8 /lib/elf/lib.so.4
    50006163 
    5000620c /lib/elf/libd1.so.1
    500062c8 /lib/elf/libc.so.4
    50006000 /lib/elf/ld-linux.so.1
以上可被用来解释动态的连接和动态的装载。
在linux支持ELF上配置的GCC假如使用了-rdynamic选项，它将把-export-dynamic传给连接器。
强烈建议使用动态装载。这就是为什么在我们的Makefile例子中使用了LDFLAGS=-rdynamic。
暂时，这个选项只能在linux下使用。
但是-Wl,-export-dynamic能在其他的平台上把-export-dynamic传给GNU的连接器。
你能在GNU link editor的[3]和[4]部分找到它详细的描述。
**★6 位置无关代码(PIC)的汇编语言编程**
当用gcc指定-fPIC的时候，gcc将从C源代码中产生PIC的汇编语言代码。
但有时候，我们需要用汇编语言来产生PIC代码。
在ELF下，PIC的实现是使用基寄存器(base register)。
在PIC下，所有的标号引用都是通过基寄存器实现的，
为此，要用汇编写PIC的话，必须保存基寄存器(base register)。
由于位置无关代码，控制传送指令的目的地址必须被替换或者是在PIC情况下计算的。
对X86机器来说，该基寄存器(base register)就是ebx.
这里我们将介绍在X86上安全的PIC汇编代码的两种方法。
这些技术在Linux C库中也被使用到。
**★6.1 在C中内嵌汇编**
gcc支持内嵌汇编的声明，可让程序员在C语言中使用汇编语言。
当写LINUX系统调用接口的时候这是很有用的，而无须使用机器相关指令。
在linux 下系统调用是通过int $0x80的。
一般的，系统调用会有三个参数:
#include 
extern int errno;
int read( int fd,void *buf ,size count)
{
long ret;
__asm__ __volatile__ ("int $0x80"
        :"=a"(ret)
        :"O"(SYS_read),"b"((long)fd),
         "c"((long)buf),"d"((long)count):"bx");
    if (ret>=0)
    {
    return (int) ret:
    }
    errno=-ret;
retrun -1;
}
以上汇编代码把系统调用号SYS_read放到了eax中，fd到ebx中，buf到ecx中，count到edx中，从int $0x80中返回值ret放在eax中。
在不用-fPIC的情况下，这样定义运行良好。
带-fPIC的gcc应该要检查ebx是否被被改变，并且应该在汇编代码里保存和恢复ebx。
但是不幸的是，事实上不是这样的。我们为了支持PIC必须自己写汇编代码。
#include 
extern int errno;
int read( int fd,void *buf ,size count)
{
long ret;
__asm__ __volatile__ ("pushl %%ebx\n\t"
        "movl %%esi,%%ebx\n\t"        
        "int $0x80\n\t"
        "popl %%ebx"
        :"=a"(ret)
        :"O"(SYS_read),"S"((long)fd),
         "c"((long)buf),"d"((long)count):"bx");
    if (ret>=0)
    {
    return (int) ret:
    }
    errno=-ret;
return -1;
}
这里首先把fd放到esi中，然后保存ebx,把esi移到ebx，在int $0x80后恢复ebx。这样保证ebx不被改变(除了在int $0x80中断调用中)。
同样的原则也适用于其他内嵌的汇编。
在任何时候，当ebx可能要被改变时，千万要记得保存和恢复ebx.
**★6.2 用汇编语言编程**
假如我们在系统调用时需要传5个参数时候，内嵌的汇编代码即使是PIC的，也不能工作，因为x86没有足够的寄存器。
我们需要直接用汇编语言编写。
syscall(int syscall_number,...)的一般汇编代码如下：
    .file "syscall.S"
    .text
    .global syscall
    .global errno
    .align 16
syscall:
    pushl 5ebp
    movl %esp,%ebp
    pushl %edi
    pushl %esi
    pushl %ebx
    movl 8(%ebp),%eax    
    movl 12(%ebp),%ebx    
    movl 16(%ebp),%ecx    
    movl 20(%ebp),%edx    
    movl 24(%ebp),%esi    
    movl 28(%ebp),%edi    
    int $0x80
    test %eax,%eax
    jpe .LLexit
    negl %eax
    movl %eax,errno
    movl $-1, %eax
.LLexit:
    popl %ebx
    popl %esi
    popl %edi
    movl %ebp,%esp
    popl %ebp
    ret
    .type syscall,@function
.L_syscall_end:
    .size syscall,.L_syscall_end -syscall
在PIC下，我们必须通过GOT(global offset table)来访问任何全局变量(除了保存在基寄存器ebx中的)。
修改的代码如下：
.file "syscall.S"
.text
.global syscall
.global errno
.align 16
syscall:
    pushl %ebp
    movl %esp,%ebp
    pushl %edi
    pushl %esi
    pushl %ebx
    call .LL4
.LL4:
    popl %ebx
    addl $_GLOBAL_OFFSET_TABLE_+[.- .LL4],%ebx
    pushl %ebx
    movl 8(%ebp),%eax
    movl 12(%ebp),%ebx
    movl 16(%ebp),%ecx
    movl 20(%ebp),%edx
    movl 24(%ebp),%esi
    movl 28(%ebp),%edi
    int $0x80
    popl %ebx
    movl %eax,%edx
    test %edx,%edx
    jge .LLexit
    negl %edx
    movl errno@GOT(%ebx),%eax
    movl %edx,(%eax)
    movl $-1,%eax
.LLexit:
    popl %ebx
    popl %esi
    popl %edi
    movl %ebp,%esp
    popl %ebp
    ret
    .type syscall,@function
.L_syscall_end:
    .size syscall,.L_syscall_end-syscall
假如要得到PIC的汇编代码，但是又不知道如何写，你可以写一个C的，
然后如下编译：
#gcc -O -fPIC -S foo.c
它将告诉gcc产生汇编代码foo.s输出,根据需要，可以修改它。
**★7 结束语**
根据以上讨论的，我们可以得出这样的结论：ELF是非常灵活的二进制格式。
它提供了非常有用的功能。这种规范没有给程序和程序员太多限制。
它使创建共享库容易，使动态装载和共享库的结合更加容易。
在ELF下，在C++中，全局的构造函数和析构函数在共享库和静态库中用同样方法处理。
[译注：
到此，文章是翻译好了，但里面的一些东西看起来可能有点问题，
比如说_libc_subinit section没有他说的那个功能，
-dynamic-linker选项在默认的redhat 6.2系统上不能用，
_dlinfo动态连接接口库函数好象在linux没有实现等等一系列问题，欢迎讨论指正
mailto: alert7@21cn.com  alert7@xfocus.org
]
参考：
1. Operating System API Reference:UNIX SVR4.2,UNIX Press,1992
2. SunOs 5.3 Linker and Libraries Manual,SunSoft ,1993.
3. Richard M.Stallman,Using and porting GNU CC for version 2.6,
   Free Software Foundation,September 1994.
4. Steve Chamberlain and Roland Pesch,Using ld:The GNU linker,ld
   version 2,Cygnus Support,January 1994.
