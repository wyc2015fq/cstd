# _beginthread和_beginthreadex的区别 - xqhrs232的专栏 - CSDN博客
2013年05月03日 13:30:26[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：666
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://hi.baidu.com/aztack/item/66fa6095db1ac3bdcd80e590](http://hi.baidu.com/aztack/item/66fa6095db1ac3bdcd80e590)
相关网帖
1、[windows多线程CreateThread与_beginthreadex本质](http://qimo601.iteye.com/blog/1726262)----[](http://qimo601.iteye.com/blog/1726262)[http://qimo601.iteye.com/blog/1726262](http://qimo601.iteye.com/blog/1726262)
2、[CreateThread, AfxBeginThread，_beginthread, _beginthreadex的区别](http://www.cnblogs.com/chuncn/archive/2009/03/08/1406096.html)----[http://www.cnblogs.com/chuncn/archive/2009/03/08/1406096.html](http://www.cnblogs.com/chuncn/archive/2009/03/08/1406096.html)
**首先我们看看这两个函数都干了什么**
uintptr_r __cdecl _beginthreadex(...)
{
    //为即将创建的线程分配一个数据结构_ptiddata ptd(per-thread data)
    //初始化这个数据结构,其中ptd->_thandle = (uintptr_t)(-1)
    //如果初始化失败,**返回(uintptr_t)(0) [_beginthread返回-1**]
    //用传进来的参数,**调用CreateThread**
    //如果**创建成功返回CreateThread返回的代码**
     //如果创建失败则释放ptd,并返回(uintptr_t)(0) [_beginthread返回-1,而CreateThread失败返回0,非-1]
}
**然后再看看这两个函数有什么不同**
**1)**
**![](http://hiphotos.baidu.com/aztack/pic/item/17bdb5fdc1cef21e08244d72.jpg)**
参数列表不同, ex版本的参数和CreateThread差不多
HANDLE CreateThread(
**LPSECURITY_ATTRIBUTES** lpThreadAttributes,
  SIZE_T dwStackSize,
  LPTHREAD_START_ROUTINE lpStartAddress,
  LPVOID lpParameter,
  DWORD dwCreationFlags,
  LPDWORD lpThreadId
);
**2)**
**![](http://hiphotos.baidu.com/aztack/pic/item/7a8135d116869b309a502772.jpg)**

二者在初始化ptd失败时返回的值不同
这是MSDN给的函数原型
uintptr_t _beginthreadex(
   void *security,
   unsigned stack_size,
   unsigned ( __stdcall ***start_address** )( void * ),
   void *arglist,
   unsigned initflag,
   unsigned *thrdaddr
);
下面是真是的实现代码中的原型
uintptr_t __cdecl _beginthreadex (
        void *security,
        unsigned stacksize,
        unsigned (__stdcall * **initialcode**) (void *),
        void * argument,
        unsigned createflag,
        unsigned *thrdaddr
)
所以初始化工作其实是在检查线程函数入口是否有效
**3)**
**![](http://hiphotos.baidu.com/aztack/pic/item/559209d1be7ed0db562c8472.jpg)**
**_beginthread的参数缺少安全描述符. 而且它是创建线程的时候先以挂起状态创建 (CreateThread会填充ptd->_thandle和ptd->_tid) 然后再ResumeThread**
_beginthread是根据传进来的参数创建线程
**4)**
![](http://hiphotos.baidu.com/aztack/pic/item/496fca3d066819cf9e3d6271.jpg)
**失败返回值不同**,**ex版本的与Windows API CreateThread返回值是一直的,这也是提倡使用后者的原因之一**
