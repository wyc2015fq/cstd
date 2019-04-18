# errno多线程安全 - ljx0305的专栏 - CSDN博客
2012年12月28日 17:49:46[ljx0305](https://me.csdn.net/ljx0305)阅读数：744
个人分类：[Linux																[Linux程序及使用																[linux下软件安装](https://blog.csdn.net/ljx0305/article/category/440323)](https://blog.csdn.net/ljx0305/article/category/394700)](https://blog.csdn.net/ljx0305/article/category/364133)
**一、errno的由来**     在C编程中，errno是个不可缺少的变量，特别是在网络编程中。如果你没有用过errno，那只能说明你的程序不够健壮。当然，如果你是WIN32平台的GetLastError()，效果也是一样的。
     为什么会使用errno呢？个人认为，这是系统库设计中的一个无奈之举，他更多的是个技巧，而不是架构上的需要。我们观察下函数结构，可以发现，函数的参数返回值只有一个，这个返回值一般可以携带错误信息，比如负数表示错误，而正数表述正确的返回值，比如recv函数。但是对于一些返回指针的函数，如： char *get_str()；这个方法显然没有用的。NULL可以表示发生错误，但是发生什么错误却毫无办法。于是，errno就诞生了。全局变量errno可以存放错误原因，当错误发生时，函数的返回值是可以通过非法值来提示错误的发生。
**二、errno的线程安全**     errno是全局变量，但是在多线程环境下，就会变得很恐怖。当你调用一个函数时，发现这个函数发生了错误，但当你使用错误原因时，他却变成了另外一个线程的错误提示。想想就会觉得是件可怕的事情。
     将errno设置为线程局部变量是个不错的主意，事实上，GCC中就是这么干的。他保证了线程之间的错误原因不会互相串改，当你在一个线程中串行执行一系列过程，那么得到的errno仍然是正确的。
    看下，bits/errno.h的定义：
# ifndef __ASSEMBLER__
/* Function to get address of global `errno' variable.  */
extern int *__errno_location (void) __THROW __attribute__ ((__const__));
 #  if !defined _LIBC || defined _LIBC_REENTRANT
/* When using threads, errno is a per-thread value.  */
#   define errno (*__errno_location ())
#  endif
# endif /* !__ASSEMBLER__ */
     而errno.h中是这样定义的：
/* Declare the `errno' variable, unless it's defined as a macro by
   bits/errno.h.  This is the case in GNU, where it is a per-thread
   variable.  This redeclaration using the macro still works, but it
   will be a function declaration without a prototype and may trigger
   a -Wstrict-prototypes warning.  */
#ifndef errno
extern int errno;
#endif
     显然，errno实际上，并不是我们通常认为的是个整型数值，而是通过整型指针来获取值的。这个整型就是线程安全的。
**三、errno的实现**
static pthread_key_t key;
static pthread_once_t key_once = PTHREAD_ONCE_INIT;
static void make_key()
{
    (void) pthread_key_create(&key, NULL);
}
int *_errno()
{
    int *ptr ;
    (void) pthread_once(&key_once, make_key);
    if ((ptr = pthread_getspecific(key)) == NULL) 
    {
        ptr = malloc(sizeof(int));        
        (void) pthread_setspecific(key, ptr);
    }
    return ptr ;
}
** 四、errno的应用**    errno在库中得到广泛的应用，但是，错误编码实际上不止那么多。我们需要在自己的系统中增加更多的错误编码。一种方式就是直接利用errno，另外一种方式就是定义自己的user_errno。
   使用errno，strerror可能无法解析，这需要自己解决。但errno使用线程变量的方式值得借鉴。
转自[http://stallman.blogbus.com/logs/20445819.html](http://stallman.blogbus.com/logs/20445819.html)
