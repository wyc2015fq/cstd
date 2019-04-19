# C语言assert关键字 - Koma Hub - CSDN博客
2017年12月03日 17:33:59[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：194
这是C语言的assert.h头文件的定义
```cpp
#include "cvidef.h"                                                               
#include "cvirte.h"                                                               
                                                                                  
#ifndef _ASSERT_H_                                                                
#define _ASSERT_H_                                                                
                                                                                  
#ifdef __cplusplus                                                                
    extern "C" {                                                                  
#endif                                                                            
                                                                                  
#undef assert                                                                     
                                                                                  
#ifdef NDEBUG                                                                     
#define assert(exp) ((void)  0)                                                   
                                                                                  
#else                                                                             
void CVIANSI _assert(char *, char *, int);                                        
                                                                                  
#define assert(exp) ((exp) ? (void) 0 : _assert(#exp, __FILE__, __LINE__))        
#endif                                                                            
                                                                                  
#ifdef __cplusplus                                                                
    }                                                                             
#endif                                                                            
                                                                                  
#endif /* _ASSERT_H_ */
```
关键是看 这句
```cpp
#define assert(exp) ((exp) ? (void) 0 : _assert(#exp, __FILE__, __LINE__))
```
1，如果 exp为真，则返回0，不进行任何操作。
2，如果exp为假，则调用_assert(#exp, __FILE__, __LINE__)
```cpp
_assert(#exp, __FILE__, __LINE__)
```
就是打印 assert断言的内容： 主要是 exp的字符串， 当前文件的名字， 当前行数。
具体例子如下：
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#define DEBUG
//#undef DEBUG
/**
assert宏的原型定义在<assert.h>中，其作用是如果它的条件返回错误，则终止程序执行，原型定义：
#include <assert.h>
void assert( int expression );
assert的作用是现计算表达式 expression ，如果其值为假（即为0），那么它先向stderr打印一条出错信息，
*/
void main(){
#ifdef DEBUG
    printf("rong\n");
#endif
    
    fprintf(stderr,"error\n");
    assert(1);
    int a = 1;
    assert(a == 1);
}
```
编译与运行：
当assert()内为真时：
```
-bash-4.1$ gcc -o a Demo.c  
-bash-4.1$ ./a
rong
error
-bash-4.1$
```
当assert()内为假时：
```
-bash-4.1$ gcc -o a Demo.c  
-bash-4.1$ ./a
rong
error
a: Demo.c:24: main: Assertion `0' failed.
已放弃 (core dumped)
-bash-4.1$
```

