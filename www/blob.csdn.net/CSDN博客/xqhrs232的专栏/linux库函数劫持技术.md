# linux库函数劫持技术 - xqhrs232的专栏 - CSDN博客
2016年10月19日 11:54:14[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：398
原文地址::[http://blog.csdn.net/fivedoumi/article/details/8126467](http://blog.csdn.net/fivedoumi/article/details/8126467)

原文地址：[linux库函数劫持技术](http://blog.chinaunix.net/uid-24585858-id-2125516.html)作者：[CUKdd](http://blog.chinaunix.net/uid/24585858.html)
众所周知，在Windows平台下可以使用挂钩（Hook）技术，将系统中的鼠标、键盘等事件拦截下来，以添加实现自己的功能。同样的，在[Linux](http://lib.csdn.net/base/linux)系统中也有类似的技术，都可以起到挂钩（Hook）拦截的作用。虽然可以实现拦截的功能，但是它们的实现原理是不同的（这点一定要注意）。
目前为止，笔者所知道的Linux系统下的一种挂钩技术，是通过libdl.so动态库中提供的一套函数dlopen(),dlsym(),dlerror(),dlclose()对动态共享链接库中的函数进行拦截的。这些函数的功能解析可以查看我的另一篇博文：[http://blogold.chinaunix.net/u3/119372/showart_2534631.html](http://blogold.chinaunix.net/u3/119372/showart_2534631.html)。更详细的使用说明建议在unix/linux平台下使用man去了解。
拦截技术是现实时，它通过环境变量LD_PRELOAD设置优先被加载器加载的动态库（以下简称拦截动态库），这里应该设置LD_PRELOAD=“xxx.so”,其中xxx.so是经过特殊功能封装的用于实现拦截功能的动态库。具体细节见下文。这样做的目的是为了在让那些使用了这些动态库的程序，在加载其中相应的函数时优先连接拦截动态库，之后根据需要来决定是否调用真正的目标函数。这些工作的实施其实都是在已经加载到内存的动态库中进行的。
用于劫持的动态链接库源文件：
```
/* Filename: dlsym.c */
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
int (*realopen)(const char *pathname, int flags);
int (*realclose)(int fd);
int open(const char *pathname, int flags) {
    void *handle;    
    char *error;
    handle = dlopen("libc.so.6", RTLD_LAZY);//获得libc.so.6的句柄
    if ((error = dlerror()) != NULL) {
        puts(error);
        exit(-1);
    }
    realopen = dlsym(handle, "open");//返回open函数在libc.so.6中的加载时地址
    if ((error = dlerror()) != NULL) {
        puts(error);
        exit(-1);
    }
    fputs("you will open
 the file: ", stderr);
    puts(pathname);
    return realopen(pathname, flags);//调用实际的open函数
}
int close(int fd) {
    void *handle;
    char *error;
    
    handle = dlopen("libc.so.6", RTLD_LAZY);
    if ((error = dlerror()) != NULL) {
        puts(error);
        exit(-1);
    }
    realclose = dlsym(handle, "close");
    if ((error = dlerror()) != NULL) {
        puts(error);
        exit(-1);
    }
    printf("the file's fd
 you will close is: %d\n", fd);
    
    return realclose(fd);
}
```
用于编译上述代码，并生成劫持库的脚本。
```
gcc -fpic -c -ldl
 dlsym.c #-fpic or -fPIC用于生成于位置无关的动态链接库
if [ -f dlsym.o ]; then
    gcc -shared -lc -o
 dlsym.so dlsym.o #-shared 用于指示生成共享库
fi
if [ -f dlsym.o ]; then
    rm dlsym.o
fi
export LD_PRELOAD=`pwd`"/dlsym.so"
#export LD_LIBRARY_PATH=`pwd`
#export DT_RPATH=`pwd`
#export | grep LD
#export | grep DT
```
ldconfig dlsym.so
```
exit 0
```
用于测试的源码：
```
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv) {
    int fd = open("dlsym.c", 0);
    if (fd != -1) close(fd);
    return 0;
}
```
[](http://blog.csdn.net/fivedoumi/article/details/8126467#)[](http://blog.csdn.net/fivedoumi/article/details/8126467#)[](http://blog.csdn.net/fivedoumi/article/details/8126467#)[](http://blog.csdn.net/fivedoumi/article/details/8126467#)[](http://blog.csdn.net/fivedoumi/article/details/8126467#)[](http://blog.csdn.net/fivedoumi/article/details/8126467#)
- 
顶
0- 
踩
