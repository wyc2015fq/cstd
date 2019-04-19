# 编程基础-----c++与c调用so文件 - xqhrs232的专栏 - CSDN博客
2016年05月03日 18:26:38[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：303
原文地址::[http://blog.csdn.net/yf210yf/article/details/11712999](http://blog.csdn.net/yf210yf/article/details/11712999)
相关文章
1、使用dlopen，实现C++的多态调用----[http://blog.sina.com.cn/s/blog_5eb8ebcb0101kn6w.html](http://blog.sina.com.cn/s/blog_5eb8ebcb0101kn6w.html)
2、
## dlerror : undefined symbol functionname
----[http://blog.sina.com.cn/s/blog_664ffc6b01014ctj.html](http://blog.sina.com.cn/s/blog_664ffc6b01014ctj.html)
1.制作so文件：libadd_c.so
add.c：
**[cpp]**[view
 plain](http://blog.csdn.net/yf210yf/article/details/11712999#)[copy](http://blog.csdn.net/yf210yf/article/details/11712999#)
[print](http://blog.csdn.net/yf210yf/article/details/11712999#)[?](http://blog.csdn.net/yf210yf/article/details/11712999#)
- int add(int a, int b)  
- {  
- return a + b;  
- }  
编译：
gcc -shared -fpic -lm -ldl -o libadd_c.so add.c
2.编写测试函数
test.cpp
**[cpp]**[view
 plain](http://blog.csdn.net/yf210yf/article/details/11712999#)[copy](http://blog.csdn.net/yf210yf/article/details/11712999#)
[print](http://blog.csdn.net/yf210yf/article/details/11712999#)[?](http://blog.csdn.net/yf210yf/article/details/11712999#)
- #include <stdio.h>
- #include <dlfcn.h>
- #include <stdlib.h>
- #include <iostream>
- usingnamespace std;  
- int main()  
- {  
- int a = 0;  
- 
- void *handle = dlopen("./libadd_c.so", RTLD_LAZY);  
- 
- if(!handle)  
-     {  
-         printf("open lib error\n");  
-         cout<<dlerror()<<endl;  
- return -1;  
-     }  
- 
- typedefint (*add_t)(int a, int b);  
-     add_t add = (add_t) dlsym(handle, "add");  
- if(!add)  
-     {  
-         cout<<dlerror()<<endl;  
-         dlclose(handle);  
- return -1;  
-     }  
- 
- 
-     a = add(3, 4);  
-     printf("a = %d\n",a);  
-     dlclose(handle);  
- return 0;  
- }  
编译：
g++ test.cpp -ldl -o test
3.运行
./test
参考：
介绍一下上面用到的接口函数
**1) dlopen**
**函数原型：**void *dlopen(const char *libname,int flag);
**功能描述：**dlopen必须在dlerror，dlsym和dlclose之前调用，表示要将库装载到内存，准备使用。如果要装载的库依赖于其它库，必须首先装载依赖库。如果dlopen操作失败，返回NULL值；如果库已经被装载过，则dlopen会返回同样的句柄。
参数中的libname一般是库的全路径，这样dlopen会直接装载该文件；如果只是指定了库名称，在dlopen会按照下面的机制去搜寻：
a.根据环境变量LD_LIBRARY_PATH查找
b.根据/etc/ld.so.cache查找
c.查找依次在/lib和/usr/lib目录查找。
flag参数表示处理未定义函数的方式，可以使用RTLD_LAZY或RTLD_NOW。RTLD_LAZY表示暂时不去处理未定义函数，先把库装载到内存，等用到没定义的函数再说；RTLD_NOW表示马上检查是否存在未定义的函数，若存在，则dlopen以失败告终。
**2) dlerror**
**函数原型：**char *dlerror(void);
**功能描述：**dlerror可以获得最近一次dlopen,dlsym或dlclose操作的错误信息，返回NULL表示无错误。dlerror在返回错误信息的同时，也会清除错误信息。
**3) dlsym**
**函数原型：**void *dlsym(void *handle,const char *symbol);
**功能描述：**在dlopen之后，库被装载到内存。dlsym可以获得指定函数(symbol)在内存中的位置(指针)。如果找不到指定函数，则dlsym会返回NULL值。但判断函数是否存在最好的方法是使用dlerror函数，
**4) dlclose**
**函数原型：**int dlclose(void *);
**功能描述：**将已经装载的库句柄减一，如果句柄减至零，则该库会被卸载。如果存在析构函数，则在dlclose之后，析构函数会被调用。
好了，现在来编译打包，命令如下：
$ g++ -shared -fPIC -o libhello.so hello.cpp
$ g++ main.cpp -ldl
在上面dlopen函数中，看到我们传的第一个参数并没有指定路径，只给出了库的名称。那是因为已经在环境变量LD_LIBRARY_PATH中指定了 ./ 目录，如下图所示。
如果你想放在其他目录，修改该环境变量即可。

