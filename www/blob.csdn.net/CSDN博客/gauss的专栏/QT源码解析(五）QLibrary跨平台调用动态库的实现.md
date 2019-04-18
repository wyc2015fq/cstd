# QT源码解析(五）QLibrary跨平台调用动态库的实现 - gauss的专栏 - CSDN博客
2013年01月03日 18:15:39[gauss](https://me.csdn.net/mathlmx)阅读数：500
个人分类：[客户端编程](https://blog.csdn.net/mathlmx/article/category/944323)
转自：http://blog.csdn.net/tingsking18/article/details/5128307
1)win下动态库调用有关的函数包括： 
(1)LoadLibrary，装载动态库。 
(2)GetProcAddress，获取要引入的函数，将符号名或标识号转换为DLL内部地址。 
(3)FreeLibrary，释放动态链接库。
2)unix上与动态库调用有关的函数包括：
(1)_打开动态链接库：dlopen，函数原型void *dlopen (const char *filename, int flag); 
dlopen用于打开指定名字(filename)的动态链接库，并返回操作句柄。 
(2)取函数执行地址：dlsym，函数原型为: void *dlsym(void *handle, char *symbol); 
dlsym根据动态链接库操作句柄(handle)与符号(symbol)，返回符号对应的函数的执行代码地址。 
(3)关闭动态链接库：dlclose，函数原型为: int dlclose (void *handle); 
dlclose用于关闭指定句柄的动态链接库，只有当此动态链接库的使用计数为0时,才会真正被系统卸载。 
(4)动态库错误函数：dlerror，函数原型为: const char *dlerror(void); 当动态链接库操作函数执行失败时，dlerror可以返回出错信息，返回值为NULL时表示操作函数执行成功。
我们来分析下Qt的源代码，看看Qt是如何封装这两种不同的调用动态库的方法。
下面是我用vc编写的一个动态库中的函数add:
extern "C" __declspec(dllexport) int __stdcall add(int a,int b)
{
    return a+b;
}
下面我就用QLibrary来调用一下：
QLibrary lib("QtDllTest.dll");
if (lib.load())
{
    typedef int(*AddFunction)(int a,int b);
    AddFunction Add=(AddFunction)lib.resolve("add");
    if (!Add)
    {
        cout<<"failed"<<endl;
    }
    else
    {
       int m;
       m=Add(1,1); //来个计算1+1
       cout<<"result:"<<m<<endl;
    }
    lib.unload();
}
else
{
    cout<<"failed"<<endl;
}
首先将目录切换到QTDIR/src/corelib/plugin，这里面就是QLibrary实现的源代码，打开qlibrary_p.h(熟悉了Qt的常用手法，就知道，这就是QLibrary内部实现的代码)，可以看到
    bool load_sys();
    bool unload_sys();
    void *resolve_sys(const char *);
三个函数。在qlibrary.cpp中可以找到调用这三个函数的地方
bool QLibrary::load ()调用了load_sys；
bool QLibrary::unload ()调用了unload_sys；
void * QLibrary::resolve ( const char * symbol )调用了resolve_sys
但是并没有找到这三个函数的实现，这是这么回事呢？
打开QTDIR/src/corelib/plugin/plugin.pri文件，
win32 {
 SOURCES += plugin/qlibrary_win.cpp
}
unix {
 SOURCES += plugin/qlibrary_unix.cpp
}
原来如此啊。
我们仔细看下qlibrary_win.cpp文件，load_sys函数调用了LoadLibrary，unload_sys调用了FreeLibrary，resolve_sys调用了GetProcAddress。
而在qlibrary_unix.cpp文件中，各种linux平台又分好多种。但是基本上load_sys调用了dlopen，unload_sys调用了dlclose，resolve_sys调用了dlsym。
在HPUX中dlopen对应shl_load，dlclose对应shl_unload，dlsym对应shl_findsym。
原来QLibrary就是这样实现不同平台动态库的调用。
