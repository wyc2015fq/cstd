# error LNK2005: _malloc already defined in LIBCMT.lib(malloc.obj) - xqhrs232的专栏 - CSDN博客
2014年01月14日 18:00:08[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：918
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://blog.csdn.net/magictong/article/details/6105160](http://blog.csdn.net/magictong/article/details/6105160)

1>Linking...
1>msvcrt.lib(ti_inst.obj) : error LNK2005: "private: __thiscall type_info::type_info(class type_info const &)" (??0type_info@@AAE@ABV0@@Z) already defined in LIBCMT.lib(typinfo.obj)
1>msvcrt.lib(ti_inst.obj) : error LNK2005: "private: class type_info & __thiscall type_info::operator=(class type_info const &)" (??4type_info@@AAEAAV0@ABV0@@Z) already defined in LIBCMT.lib(typinfo.obj)
1>msvcrt.lib(MSVCR80.dll) : error LNK2005: _malloc already defined in LIBCMT.lib(malloc.obj)
1>msvcrt.lib(MSVCR80.dll) : error LNK2005: _free already defined in LIBCMT.lib(free.obj)
**1>LIBCMT.lib(crt0init.obj) : warning LNK4098: defaultlib 'msvcrt.lib' conflicts with use of other libs; use /NODEFAULTLIB:library**
1>../Output/ImpactLib/TSIptLibBuilder.exe : fatal error LNK1169: one or more multiply defined symbols found
      神马玩意？纠结的链接报错，不过看加粗那段，它给了个解决方法……把vc6的工程转成vs2005的时候经常出现这个问题，原因就是你使用的某些外部静态库编译时使用的标准库和当前工程使用的标准库的冲突了。解决方法就是忽略冲突的标准库，有点蛋疼。其实具体到vc6和vs2005的话，vs2005已经没有单线程编译选项了。
      vs2005的设置方法如下图所示：打开工程属性对话框，选择链接下Input，在Ignore Specific Library里面输入要忽略的库即可（有多个 的话用逗号隔开）。另外具体这里的话，如果是release版本要忽略LIBCMT.lib，debug版本要忽略LIBCMTD.lib。
![](http://hi.csdn.net/attachment/201012/29/0_1293606707qx70.gif)
      到MSDN上搜一下LNK4098（讲的很详细，慢慢看）：
#### Error Message
defaultlib 'library' conflicts with use of other libs; use /NODEFAULTLIB:library
You are trying to link with incompatible libraries.
The run-time libraries now contain directives to prevent mixing different types. You will receive this warning if you try to use different types or debug and non-debug versions of the run-time library in the same program. For example, if you compiled one
 file to use one kind of run-time library and another file to use another kind (for example, single-threaded versus multithreaded) and tried to link them, you will get this warning. You should compile all source files to use the same run-time library. See the [Use
 Run-Time Library]() (**/MD**, **/MT**,**/LD**) compiler options for more information.
You can use the linker's [/VERBOSE:LIB]() switch to determine which libraries the linker is searching. If you receive LNK4098 and want to create an executable file that uses, for
 example, the single-threaded, non-debug run-time libraries, use the **/VERBOSE:LIB** option to find out which libraries the linker is searching. The linker should print LIBC.lib and not LIBCMT.lib, MSVCRT.lib, LIBCD.lib, LIBCMTD.lib, or MSVCRTD.lib
 as the libraries searched. You can tell the linker to ignore the incorrect run-time libraries by using [/NODEFAULTLIB]() for each library you want to ignore.
The table below shows which libraries should be ignored depending on which run-time library you want to use.
|To use this run-time library|Ignore these libraries|
|----|----|
|Single-threaded (libc.lib)|libcmt.lib, msvcrt.lib, libcd.lib, libcmtd.lib, msvcrtd.lib|
|Multithreaded (libcmt.lib)|libc.lib, msvcrt.lib, libcd.lib, libcmtd.lib, msvcrtd.lib|
|Multithreaded using DLL (msvcrt.lib)|libc.lib, libcmt.lib, libcd.lib, libcmtd.lib, msvcrtd.lib|
|Debug Single-threaded (libcd.lib)|libc.lib, libcmt.lib, msvcrt.lib, libcmtd.lib, msvcrtd.lib|
|Debug Multithreaded (libcmtd.lib)|libc.lib, libcmt.lib, msvcrt.lib, libcd.lib, msvcrtd.lib|
|Debug Multithreaded using DLL (msvcrtd.lib)|libc.lib, libcmt.lib, msvcrt.lib, libcd.lib, libcmtd.lib|
For example, if you received this warning and you want to create an executable file that uses the non-debug, single-threaded version of the run-time libraries, you could use the following options with the linker:
