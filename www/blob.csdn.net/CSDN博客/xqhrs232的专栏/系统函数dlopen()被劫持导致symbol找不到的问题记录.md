# 系统函数dlopen()被劫持导致symbol找不到的问题记录 - xqhrs232的专栏 - CSDN博客
2016年05月03日 15:59:21[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2012
原文地址::[http://my.oschina.net/renhc/blog/295841](http://my.oschina.net/renhc/blog/295841)
相关文章
1、[调试dlopen试遇到undefined symbol问题的小技巧](http://blog.chinaunix.net/uid-686647-id-1745810.html) ----[http://blog.chinaunix.net/uid-686647-id-1745810.html](http://blog.chinaunix.net/uid-686647-id-1745810.html)
2、[dlopen与dlsym用法](http://blog.csdn.net/kingkong1024/article/details/8474210)----[http://blog.csdn.net/kingkong1024/article/details/8474210](http://blog.csdn.net/kingkong1024/article/details/8474210)
3、[dlopen函数详解](http://blog.chinaunix.net/uid-10540984-id-3508235.html) ----[http://blog.chinaunix.net/uid-10540984-id-3508235.html](http://blog.chinaunix.net/uid-10540984-id-3508235.html)
4、[动态dlopen失败：undefined symbol: _ZTVN10_cxxabiv120__si_class_type_infoE？](http://www.zhihu.com/question/27121692)----[http://www.zhihu.com/question/27121692/answer/35329646](http://www.zhihu.com/question/27121692/answer/35329646)
5、Linux 动态库 dlopen()失败，errno = 17, File exists----[http://bbs.csdn.net/topics/320221047](http://bbs.csdn.net/topics/320221047)
6、dlopen Linux 动态库失败原因与解决办法总结 ----[http://www.xue163.com/949/1/9497598.html](http://www.xue163.com/949/1/9497598.html)
7、请教：dlopen总是打不开动态库文件？？----[http://bbs.csdn.net/topics/70359966](http://bbs.csdn.net/topics/70359966)
由于C语言没有命名空间的概念，用户可以实现与系统函数同名的函数以覆盖系统函数，这个特性带来了无限的创造性的同时也带来了无限的伤害，影响大小完全取决你怎么使用，这正是C语言的魅力所在。 本文记录了由于改写系统函数dlopen()引发的一个问题及定位问题的思路。
[系统函数](http://www.oschina.net/search?scope=blog&q=%E7%B3%BB%E7%BB%9F%E5%87%BD%E6%95%B0)[dlopen](http://www.oschina.net/search?scope=blog&q=dlopen)[劫持](http://www.oschina.net/search?scope=blog&q=%E5%8A%AB%E6%8C%81)[symbol](http://www.oschina.net/search?scope=blog&q=symbol)
问题现象
我们实现了一个名叫libilvrfplugin.so的lib，该lib链接了libiubsntconflib.so, 而libiubsntconflib.so 又链接了libipconflib.so, libipconflib.so里面实现了一个方法check_vrf_r()用于检查VRF的合法性。
简单点来说，A lib链接了B lib，而B lib又链接了C lib，C lib实现了方法check_vrf_r().
某些场景下，系统会动态加载A lib, 但是A lib根本没用到方法check_vrf_r()。注意这里是动态加载的lib库，也就是lib的使用方运行时会使用dlopen()加载该lib。
在新发布的版本里，我们发现该lib竟然没起做用，好像该lib 根本就不存在一样。
我们在syslog里找到下面一条log：
> 
Apr 17 15:46:27.718075 info CFPU-0 Validator: CPluginManager : Unable to load plugin libilvrfplugin.so Error:/opt/nokiasiemens/lib64/libiubsntconflib.so: undefined symbol: check_vrf_r
从syslog 里可以看出动态加载libilvrfplugin.so（也就是 A lib）时失败，原因是找不到符号check_vrf_r。
疑惑一：这几个lib在上一个版本里工作正常，当前版本里没有任何改动，为什么会出错呢？
疑惑二：libilvrfplugin.so 中根本没用到符号check_vrf_r，为什么会报找不到符号呢？
查找问题
根据syslog里的线索，定位到出事的代码：
[?](http://my.oschina.net/renhc/blog/295841#)
```
```cpp
...
```
```cpp
lib_handle=dlopen(ep[count]->d_name,RTLD_LAZY);
```
```cpp
if
```
```cpp
(!lib_handle)
```
```cpp
{
```
```cpp
```
```cpp
error
 = dlerror();
```
```cpp
```
```cpp
TRACER(TRC_INFO)<<
```
```cpp
"CPluginManager
 : Unable to load plugin "
```
```cpp
```
```cpp
<<ep[count]->d_name
 <<
```
```cpp
"
 Error:"
```
```cpp
<<error<<std::endl;
```
```cpp
```
```cpp
free
```
```cpp
(ep[count]);
```
```cpp
```
```cpp
continue
```
```cpp
;
```
```cpp
}
```
```cpp
//
 if
```
```cpp
...
```
```
这里使用dlopen()来加载动态链接库，并设置flag为RTLD_LAZY，该flag控制了dlopen()加载lib时的解析方式，加载时不解析未定义的符号。本例中符号check_vrf_r就属于找不到定义的符号（因为A 库只是通过相应的include把符号包含了进来）。
顺便说一句，dlopen()还有一种解析方式是RTLD_NOW，这就要求所有的符号都需要解析到地址，不管该符号有没有用到。
但是本例中dlopen()解析方式是正确的，我们期望不去解析符号check_vrf_r，可是为什么还是去解析了呢？
原因可能是glibc的实现有变，有个bug也说不准，还有可能是哪里改变了dlopen的实现。顺便说一句由于C语言没有命名空间的概念，所以你可以定义一个与系统函数同名的函数以覆盖系统函数，大多数情况应该避免这么做。
经查glibc在我们发布的两个版本中没有变化，那么很可能是哪里改变了或影响了dlopen()。不得已，只能去翻看我们这个版本中所有代码变化。
我们惊奇的发现，在某个脚本里新增了这么一行代码：
[?](http://my.oschina.net/renhc/blog/295841#)
```
`export``LD_PRELOAD=``/opt/nokiasiemens/SS_FConfigure/lib/libdlopeninterceptor``.so`
```
从字面意义上看跟dlopen有关，"dlopen劫持者"，好霸气的名字！接着看这个代码会起什么作用。 
这里export了环境变量LD_PRELOAD，该环境变量声明了应用程序加载前优先加载的动态链接库，换句话说如果这里的动态链接库实现了与系统函数同名的函数的话，那么将覆盖系统函数。 
怀着激动的心情查看该动态lib的实现： 
[?](http://my.oschina.net/renhc/blog/295841#)
```
```cpp
#include
 <dlfcn.h>
```
```cpp
#include
 <syslog.h>
```
```cpp
#include
 <stdlib.h>
```
```cpp
#ifdef
 __cplusplus__
```
```cpp
extern
```
```cpp
"C"
```
```cpp
{
```
```cpp
#endif
```
```cpp
typedef
```
```cpp
void
```
```cpp
*
 (*dlopen_func_t)(
```
```cpp
const
```
```cpp
char
```
```cpp
*
 filename,
```
```cpp
int
```
```cpp
flag);
```
```cpp
static
```
```cpp
dlopen_func_t
 _glibc_dlopen = NULL;
```
```cpp
void
```
```cpp
*
 dlopen(
```
```cpp
const
```
```cpp
char
```
```cpp
*
 filename,
```
```cpp
int
```
```cpp
flag)
```
```cpp
{
```
```cpp
```
```cpp
int
```
```cpp
realflag
 = flag;
```
```cpp
```
```cpp
if
```
```cpp
(NULL
 == _glibc_dlopen) {
```
```cpp
```
```cpp
_glibc_dlopen
 = (dlopen_func_t)dlsym(RTLD_NEXT,
```
```cpp
"dlopen"
```
```cpp
);
```
```cpp
```
```cpp
if
```
```cpp
(NULL
 == _glibc_dlopen) {
```
```cpp
```
```cpp
syslog(LOG_CRIT,
```
```cpp
"dlopeninterceptor:Failed
 to resolve dlopen, got error:%s"
```
```cpp
,
 dlerror());
```
```cpp
```
```cpp
return
```
```cpp
NULL;
```
```cpp
```
```cpp
}
```
```cpp
```
```cpp
}
```
```cpp
```
```cpp
if
```
```cpp
(realflag
 & RTLD_LAZY) {
```
```cpp
```
```cpp
realflag
 = realflag & ~RTLD_LAZY;
```
```cpp
```
```cpp
realflag
 = realflag | RTLD_NOW;
```
```cpp
```
```cpp
syslog(LOG_DEBUG,
```
```cpp
"dlopeninterceptor:Changing
 dlopen flag from to %d to %d when opening %s"
```
```cpp
,
 flag, realflag, filename);
```
```cpp
```
```cpp
}
```
```cpp
```
```cpp
return
```
```cpp
_glibc_dlopen(filename,
 realflag);
```
```cpp
}
```
```cpp
#ifdef
 __cplusplus__
```
```cpp
}
```
```cpp
#endif
```
```
惊奇的发现该Lib确实重写了dlopen(), 如果dlopen()指定的flag时RTLD_LAZY将强制转换成RTLD_NOW。找到root cause了，松一口气。
最后
查找到root cause后就简单了，给相应的组织或部门报个issue，将你的分析结果放上去就OK了，问题很快就解决了。后来听说是某个同事误操作在脚本中加了一行代码让dlopen()劫持者生效了。
类似这样的问题，root cause是很简单的，去fix花费的effort也不大。稍有困难的是如何在庞杂的系统中逐步定位问题，而且需要去查看整个系统的代码实现，由于对系统其他模块的不熟悉，必要时还需要给矛支持。感谢在此过程中给予支持的同事，也很欣慰公司有种很好的机制或氛围，让你在必要时都能得到给力的support。
//===========================================================================================
备注::
1>想起来了，像这样的sharedlib 需要主程序定义的函数和变量（rcs）的情况需要加参数 －rdybmic进行链接
//mklib.sh
#! /bin/bash
gcc -fPIC -g -c policy.c -o libpolicy.o
gcc -g -shared -Wl,-soname,libpolicy.so -o libpolicy.so.1.0.0 libpolicy.o -lc
ln -s libpolicy.so.1.0.0 libpolicy.so.l
ln -s libpolicy.so.1.0.0 libpolicy.so
－－－－－－－－－－－－－－－－－－－－－－－
gcc -g -Wall main.c -o main -ldl -rdynamic
－－－－－－－－－－－－－－－－－～～～～

