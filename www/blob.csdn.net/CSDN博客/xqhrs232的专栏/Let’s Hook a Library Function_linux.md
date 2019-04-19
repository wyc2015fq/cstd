# Let’s Hook a Library Function_linux - xqhrs232的专栏 - CSDN博客
2016年10月24日 10:51:55[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：338
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://opensourceforu.com/2011/08/lets-hook-a-library-function/](http://opensourceforu.com/2011/08/lets-hook-a-library-function/)

If you are a developer, and want to change the way a library function works, this article will give you a basic idea of how to get started — just enough knowledge to be able to experiment with your library functions. All code here is in C, and tested with GCC
 on Linux.
According to [Wikipedia](http://en.wikipedia.org/wiki/Hooking), “In computer programming, the term hooking covers
 a range of techniques used to alter or augment the behaviour of an operating system, applications, or other software components, by intercepting function calls or messages or events passed between software components. Code that handles such intercepted function
 calls, events or messages is called a hook.”
Intercepting a library call, and calling your own wrapper code, is also called Function Interposition.
Hooking has two benefits:
- You don’t have to search for the function definition in the library, such as `libc` (`glibc` is
 the GNU C Library, and `libc` is almost half of the size of `glibc`)
 and change it. Seriously, this is a very nasty technical task (at least for me!).
- You don’t need to recompile the library’s source code.
## Library functions and system calls
Please look at Figures 1 and 2 for a graphical representation of what happens when a library function is hooked.
![Library function](http://i2.wp.com/www.opensourceforu.com/wp-content/uploads/2011/08/figure-1-e1322584503522-590x548.jpg?resize=590%2C548)
Figure 1: Library function
![Library function with hook](http://i1.wp.com/www.opensourceforu.com/wp-content/uploads/2011/08/figure-2-e1322584473979-590x381.jpg?resize=590%2C381)
Figure 2: Library function with hook
Now let’s look at hooking a library function. The simple `prog1.c` program
 below just allocates 10 bytes of memory from the heap, and frees it:
```
`#include<stdio.h>`
`#include<malloc.h>`
`#include<stdlib.h>`
`int`
```
main(
```
`void``)`
`{`
`    ``int`
```
*p;
```
`    ``printf``(`
```
"calling
 from main...\n"
```
`);`
`    ``p=(``int`
```
*)
```
`malloc``(10);`
`    ``if``(!p)`
`    ``{`
`        ``printf``(`
```
"Got
 allocation error...\n"
```
`);`
`        ``exit``(1);`
`    ``}`
`    ``printf``(`
```
"returning
 to main...\n"
```
`);`
`    ``free``(p);                           `
```
/*
 freeing memory from heap */
```
`    ``printf``(`
```
"freeing
 memory...\n"
```
`);`
`    ``return`
```
0;
```
`}`
```
When we compile and run the above program, the output is as shown below:
```
[root@workbenchsvr
 malloc_hook]# gcc -o prog1 prog1.c
```
```
[root@workbenchsvr
 malloc_hook]# ./prog1
```
```
calling
 from main...
```
```
returning
 to main...
```
```
freeing
 memory...
```
```
[root@workbenchsvr
 malloc_hook]#
```
The next program, called `prog2.c`, is a simple hook for the `malloc()` function:
```
```
#define
 _GNU_SOURCE
```
```
#include
 <stdio.h>
```
```
#include
 <stdint.h>
```
```
#include
 <dlfcn.h>                               /* header required for dlsym() */
```
```
/*
 lcheck() is for memory leak check; its code is not shown
```
` `
```
here
 */
```
`void`
```
lcheck(
```
`void``);`
`void``* ``malloc``(``size_t`
```
size)
```
`{`
`    ``static`
```
void
```
```
*
 (*my_malloc)(
```
`size_t`
```
)
 = NULL;
```
`    ``printf``(`
```
"inside
 shared object...\n"
```
`);`
`    ``if`
```
(!my_malloc)
```
`    `
```
my_malloc
 = dlsym(RTLD_NEXT,
```
`"malloc"``);  `
```
/*
 returns the object reference for malloc */
```
`    ``void`
```
*p = my_malloc(size);
```
```
/*
 call malloc() using function pointer my_malloc */
```
`    ``printf``(`
```
"malloc(%d)
 = %p\n"
```
```
,
 size, p);
```
`    ``lcheck();                                `
```
/*
 calling do_your_stuff function */
```
`    ``printf``(`
```
"returning
 from shared object...\n"
```
`);`
`    ``return`
```
p;
```
`}`
`void`
```
lcheck(
```
`void``)`
`{`
`    ``printf``(`
```
"displaying
 memory leaks...\n"
```
`);`
`    `
```
/*
 do required stuff here */
```
`}`
```
Compiling and running the above, goes like this:
```
[root@workbenchsvr
 malloc_hook]# gcc -shared -ldl -fPIC prog2.c -o libprog2.so
```
```
[root@workbenchsvr
 malloc_hook]# LD_PRELOAD=/home/dibyendu/malloc_hook/libprog2.so ./prog1
```
```
calling
 from main...
```
```
inside
 shared object...
```
```
malloc(10)
 = 0x8191008
```
```
displaying
 memory leaks...
```
```
returning
 from shared object...
```
```
returning
 to main...
```
```
freeing
 memory...
```
```
[root@workbenchsvr
 malloc_hook]#
```
So, let’s take a closer look at our first hook. The `dlsym()` function
 takes two parameters: the first is a handle returned by `dlopen()`.
 Here, we must use `RTLD_NEXT` for function interposition.
This tells the dynamic linker to find the next reference to the specified function, not the one that is calling `dlsym()`.
 The second parameter is the symbol name (`malloc`, in this case), as
 a character string. `dlsym()` returns the address of the symbol specified
 as the second parameter. While compiling, `fPIC` is used to create
 a position-independent object.
The `LD_PRELOAD` environment variable gives the loader a list of libraries
 to load before anything else. Our use of it will load `libprog2.so` and
 dynamically link it with the `prog1` binary. Don’t forget to provide
 an absolute path to the `.so`, in `LD_PRELOAD`.
 And, of course, include `_GNU_SOURCE` if you want to use certain extensions
 while using the GNU C library, because some extensions may not be available on other non-GNU systems and adding this `#define`will
 increase portability.
## Can we hook every function with dlsym()?
The above hook method will not work if you want to wrap `dlsym()` itself,
 or wrap any library functions that internally call `dlsym()`. So, is
 there any way to interpose `dlsym()`? Yes, but you can’t use the same
 hook procedure — check the output if you try that. First, create a shared object `libfile.so` from `file1.c` and `file2.c` (below).
 Then compile it with `gcc -rdynamic -o dl_prog1 dl_prog1.c -ldl`. Yes,
 the output is obvious:
```
/*
 file1.c */
```
`void`
```
file1(
```
`int`
```
*i)
```
`{`
`    ``*i=100;`
`}`
```
/*
 file2.c  */
```
`void`
```
file2(
```
`int`
```
*i)
```
`{`
`    ``*i=200;`
`}`
The following `dl_prog1.c` is a simple program to show the functionality
 of `dlopen()` and `dlsym()`.
 The functions`file1()` and `file2()` are
 defined in `file1.c` and `file2.c`.
```
`#include<stdio.h>`
`#include<dlfcn.h>`
`#include<stdlib.h>`
`void`
```
file1(
```
`int`
```
*i);
```
`void`
```
file2(
```
`int`
```
*i);
```
`int`
```
main(
```
`void``)`
`{`
`    ``void`
```
*handler;
```
`    ``int`
```
(*fn) (
```
`int`
```
*);
```
`    ``int`
```
x;
```
`    ``char`
```
*error;
```
`    `
```
handler
 = dlopen(
```
`"/home/dibyendu/dlsym_hook/libfile.so"`
```
,
 RTLD_LAZY);
```
`    ``if`
```
(!handler)
```
`    ``{`
`        ``fprintf``(stderr,``"%s\n"`
```
,
 dlerror());
```
`        ``exit``(1);`
`    ``}`
`    `
```
fn
 = dlsym(handler,
```
`"file1"``);     `
```
/*
 getting the handle of file1 through dlsym() */
```
`    ``if`
```
((error = dlerror()) != NULL)
```
```
/*
 checking error through dlerror() */
```
`    ``{`
`        ``fprintf``(stderr,``"%s\n"`
```
,
 error);
```
`        ``exit``(1);`
`    ``}`
`    ``(*fn)(&x);                            `
```
/*
 Calling file1() to resolve x */
```
`    ``printf``(`
```
"The
 value of x is %d\n"
```
```
,
 x);
```
`    ``dlclose(handler);                 `
```
/*
 closing the file handle */
```
`    ``return`
```
0;
```
`}`
```
```
[root@workbenchsvr
 dlsym_hook]# gcc -shared -ldl -fPIC file1.c file2.c -o libfile.so
```
```
[root@workbenchsvr
 dlsym_hook]# gcc -rdynamic -o dl_prog1 dl_prog1.c -ldl
```
```
[root@workbenchsvr
 dlsym_hook]# ./dl_prog1
```
```
The
 value of x is 100
```
```
[root@workbenchsvr
 dlsym_hook]#
```
Now try to hook `dlsym()`, and you will get a segmentation fault, due
 to recursive calls (`dlsym()` will call itself). The following `dl_prog2.c` will
 be a recursive `dlsym()` call to itself, which causes a stack overflow
 and segmentation fault:
```
```
#define
 _GNU_SOURCE
```
```
#include
 <stdio.h>
```
```
#include
 <stdint.h>
```
```
#include
 <dlfcn.h>
```
`void`
```
*dlsym(
```
`void`
```
*handle,
```
`const`
```
char
```
```
*name)
```
`{`
`    ``void`
```
*(*dlsym_fn)(
```
`void`
```
*,
```
`const`
```
char
```
```
*)=NULL;
```
`    ``printf``(`
```
"inside
 shared object::before dlsym()...\n"
```
`);`
`    ``dlsym_fn=dlsym(RTLD_NEXT, ``"dlsym"``);                     `
```
/*
 this will call itself again and again */
```
`    ``printf``(`
```
"inside
 shared object::after dlsym()...\n"
```
`);`
`    ``return`
```
(*dlsym_fn)(handle, name);
```
`}`
```
Output:
```
[root@workbenchsvr
 dlsym_hook]# gcc -shared -ldl -fPIC dl_prog2.c -o libdl_prog2.so
```
```
[root@workbenchsvr
 dlsym_hook]# LD_PRELOAD=/home/dibyendu/dlsym_hook/libdl_prog2.so ./dl_prog1
```
```
inside
 shared object::before dlsym()...
```
`…...............................................................`
```
inside
 shared object::before dlsym()...
```
```
Segmentation
 fault
```
```
[root@workbenchsvr
 dlsym_hook]#
```
Here is the code (`dl_prog3.c`) that successfully interposes `dlsym()`:
```
```
#define
 __USE_GNU
```
```
#include
 <stdio.h>
```
```
#include
 <stdlib.h>
```
```
#include
 <dlfcn.h>
```
`extern`
```
void
```
```
*__libc_dlsym (
```
`void`
```
*,
```
`const`
```
char
```
```
*);
```
`void`
```
*dlsym(
```
`void`
```
*handle,
```
`const`
```
char
```
```
*symbol)
```
`{`
`    ``printf``(`
```
"Ha
 Ha...dlsym() Hooked\n"
```
`);`
`    ``void`
```
*
 result = __libc_dlsym(handle, symbol);
```
```
/*
 now, this will call dlsym() library function */
```
`    ``return`
```
result;
```
`}`
```
Output:
```
[root@workbenchsvr
 dlsym_hook]# gcc -shared -ldl -fPIC dl_prog3.c -o libdl_prog3.so
```
```
[root@workbenchsvr
 dlsym_hook]# LD_PRELOAD=/home/dibyendu/dlsym_hook/libdl_prog3.so ./dl_prog1
```
```
Ha
 Ha...dlsym() Hooked
```
```
The
 value of x is 100
```
```
[root@workbenchsvr
 dlsym_hook]#
```
## What else can we do?
I have already interposed other functions like `getaddrinfo()`, `open()`,
 etc, so you can intercept any function you want. But there are a few limitations:
- Do beware of functions that themselves call `dlsym()`,
 when you need to call `__libc_dlsym` (handle, symbol) in the hook.
- Ensure that the SUID bit is not set, otherwise you can’t use `LD_PRELOAD`.
- Also, internal library function calls are resolved before runtime — say, if some function in `libc` calls`getaddrinfo()` or `malloc()`,
 it will never call the hook from a different library.
##### References
- Tutorial: [Function Interposition in Linux](http://www.jayconrod.com/cgi/view_post.py?23) by Jay Conrod
- Linux man pages: `dlopen()`, `dlsym()`, `dlerror()`, `dlclose()`
### Related Posts:
- [The
 Importance of Memory Management](http://opensourceforu.com/2015/12/the-importance-of-memory-management/)
- [How
 do Arrays Decay into Pointers?](http://opensourceforu.com/2016/09/decayintopointers/)
- [Understanding
 Object Slicing in C++](http://opensourceforu.com/2016/03/understanding-object-slicing-in-c/)
- [Write
 Your Own conio.h for GNU/Linux](http://opensourceforu.com/2014/03/write-conio-h-gnulinux/)
- [Analyse
 Your Network Packets with LibPCAP](http://opensourceforu.com/2014/02/analyse-network-packets-libpcap/)
### Share this:
