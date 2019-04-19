# C语言插件开发模式与分析 - Koma Hub - CSDN博客
2019年03月03日 18:40:12[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：104
**Table of Contents**
[第一种方法void *dlsym(void *handle, const char *symbol);](#%E7%AC%AC%E4%B8%80%E7%A7%8D%E6%96%B9%E6%B3%95void%20*dlsym(void%20*handle%2C%20const%20char%20*symbol)%3B)
[main.c](#main.c)
[dll.h](#dll.h)
[module1.c](#module1.c)
[module2.c](#module2.c)
[makefile](#makefile)
[运行](#%E8%BF%90%E8%A1%8C)
[第二种方法objdump](#%E7%AC%AC%E4%BA%8C%E7%A7%8D%E6%96%B9%E6%B3%95objdump)
[分析](#%E5%88%86%E6%9E%90)
[首先，我们查看以下动态库的符号表](#%E9%A6%96%E5%85%88%EF%BC%8C%E6%88%91%E4%BB%AC%E6%9F%A5%E7%9C%8B%E4%BB%A5%E4%B8%8B%E5%8A%A8%E6%80%81%E5%BA%93%E7%9A%84%E7%AC%A6%E5%8F%B7%E8%A1%A8)
[删掉符号表](#%E5%88%A0%E6%8E%89%E7%AC%A6%E5%8F%B7%E8%A1%A8)
简单来讲，就是**获取二进制文件中的地址**嘛！！
# 第一种方法void *dlsym(void *handle, const char *symbol);
大家可以自己查用户手册。
> 
NAME
       dlsym, dlvsym - obtain address of a symbol in a shared object or executable
SYNOPSIS
       #include <dlfcn.h>
       void *dlsym(void *handle, const char *symbol);
       #define _GNU_SOURCE
       #include <dlfcn.h>
       void *dlvsym(void *handle, char *symbol, char *version);
       Link with -ldl.
> 
NAME
       dlclose, dlopen, dlmopen - open and close a shared object
SYNOPSIS
       #include <dlfcn.h>
       void *dlopen(const char *filename, int flags);
       int dlclose(void *handle);
       #define _GNU_SOURCE
       #include <dlfcn.h>
       void *dlmopen (Lmid_t lmid, const char *filename, int flags);
       Link with -ldl.
以下代码来源：[https://blog.csdn.net/reage11/article/details/8606125](https://blog.csdn.net/reage11/article/details/8606125)（进行过修改）
## main.c
```cpp
#include <stdio.h>
#include <dlfcn.h>
#include "dll.h"
 
#define debug() printf("%s:%d\n", __func__, __LINE__);
int main(){
    void *dp;
    char *err;
    char fname[20];
	dll p;
	debug();
	void (*init)(dll *p);
    void (*Done)();
	printf("module1.so or moduel2.so");
    
	char tmp[20];
	scanf("%s", tmp);
    debug();
	
	printf("Open file: %s\n", tmp);
	sprintf(fname, "./%s", tmp);
	
	dp = dlopen(fname, RTLD_LAZY);//打开动态库。
    
	printf("Open file: %s\n", fname);
	
	if( NULL == dp) {
        printf("%s\n", dlerror());
        exit(1);
    }
	debug();
    init = dlsym(dp, "init");//找到init函数的地址。
    init(&p);
	Done = dlsym(dp, "Done");
	Done();
    printf("%s\n", p.name);
	
	return 0;
}
```
## dll.h
```cpp
typedef struct dll{
    char * name;
    void (*Done)() ;
}dll;
```
## module1.c
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dll.h"
 
void Done(){
    printf("This is test module 1!\n");
}
 
void init(dll *p){
    p->name = (char *)calloc(3, sizeof(char));
    strcpy(p->name, "module1.so init");
    p->Done = Done;
}
```
## module2.c
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dll.h"
 
void Done(){
    printf("This is test module 2!\n");
}
 
void init(dll *p){
    p->name = (char *)calloc(3, sizeof(char));
    strcpy(p->name, "module2.so");
    p->Done = Done;
}
```
## makefile
```
ALL:
	gcc -rdynamic -o main main.c dll.h -ldl  #-ldl (指定dl库)因为dlopen和dlsym在dl库中
	gcc -shared -o module1.so  module1.c dll.h -fpic
	gcc -shared -o module2.so module2.c dll.h -fpic
```
## 运行
```
$ ./main 
main:13
Input module1.so or moduel2.so: module1.so
main:20
Open file: module1.so
Open file: ./module1.so
main:34
This is test module 1!
module1.so init
```
# 第二种方法objdump
未完待续。。。
```cpp
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "dll.h"
typedef void (*funcp)(dll *p);
#define debug() printf("%s:%d\n", __func__, __LINE__);
int main()
{
	FILE *fp = popen("objdump -T module1.so | grep init | awk '{print $1}'", "r");
	uint64_t func_addr;
	char ptr[20];
	memset(ptr, 0, 20);
	fgets(ptr, 20, fp);
	printf("Ptr: %s\n", ptr);
	func_addr = strtoul(ptr, NULL, 16);
	printf("%lx\n", func_addr);
	funcp init;
	debug();
	init = (funcp)func_addr;
	debug();
	dll p;
	debug();
	init(&p); 
	debug();
	
	pclose(fp);
	return 0;
}
```
# 分析
```cpp
init = dlsym(dp, "init");
```
是根据二进制文件中的符号表查询得出的函数地址吗？
## 首先，我们查看以下动态库的符号表
```
$ nm module1.so 
0000000000201030 B __bss_start
                 U calloc@@GLIBC_2.2.5
0000000000201030 b completed.7696
                 w __cxa_finalize@@GLIBC_2.2.5
00000000000005b0 t deregister_tm_clones
0000000000000640 t __do_global_dtors_aux
0000000000200e10 t __do_global_dtors_aux_fini_array_entry
000000000000068a T Done
0000000000201028 d __dso_handle
0000000000200e18 d _DYNAMIC
0000000000201030 D _edata
0000000000201038 B _end
00000000000006f8 T _fini
0000000000000680 t frame_dummy
0000000000200e08 t __frame_dummy_init_array_entry
00000000000007e0 r __FRAME_END__
0000000000201000 d _GLOBAL_OFFSET_TABLE_
                 w __gmon_start__
0000000000000718 r __GNU_EH_FRAME_HDR
000000000000069d T init
0000000000000550 T _init
                 w _ITM_deregisterTMCloneTable
                 w _ITM_registerTMCloneTable
                 U puts@@GLIBC_2.2.5
00000000000005f0 t register_tm_clones
0000000000201030 d __TMC_END__
```
可以看到init的地址为000000000000069d， 那么dlsym是否根据这个符号找到他的函数地址的呢？
## 删掉符号表
```
$ strip module1.so 
$ nm module1.so 
nm: module1.so: no symbols
```
再次运行，发现仍然可以。
```
$ strip module1.so 
$ nm module1.so 
nm: module1.so: no symbols
$ ./main 
main:13
Input module1.so or moduel2.so: module1.so
main:20
Open file: module1.so
Open file: ./module1.so
main:34
This is test module 1!
module1.so init
```
行吧，就先这样吧。
