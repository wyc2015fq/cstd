# c++用宏定义来编写函数#define - Koma Hub - CSDN博客
2017年12月14日 22:43:47[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：760
```cpp
#include<iostream>
#include<stdlib.h>
#include<assert.h>
#include"hello.h"
#ifdef _OPENMP
#include<omp.h>
#endif
#define CHECK(arg){                 \
                                    \
    cout<<"CHECK:"<<arg<<endl;      \
}
static int _T;
#define CHECKOPENMP(arg)    \
{                           \
    _T = 10;                \
}
using namespace std;
typedef struct Node{
    int data;
    struct Node *lchild;
    struct Node *rchild;
}Node, *BinTree;
static ssize_t
fun()
{
    cout<<"fun"<<endl;
}
int main(int argv,char*argc[])
{
    register int threadnum = 1;
    assert(argv-1);
    #ifdef _OPENMP
    if(argv > 1){
        threadnum = atoi(argc[1]);
        omp_set_num_threads(threadnum);
    }
    #pragma omp parallel for reduction(+:_T)
    for(int i=0;i<5;i++)
        _T += i;
    #endif
    fun();
    CHECK(__FILE__);
    CHECK(__LINE__);
    cout<<_T<<endl;
    CHECKOPENMP();
    _CHECK();
    cout<<_T<<endl;
    #ifdef _hello
    cout<<max(12,14)<<endl;
    #endif
    cout<<"Hello World!"<<endl;    
    return 1;    
}
```
hello.h如下：
```cpp
#ifndef _hello
#define _hello
int max(int a, int b){
    return a>b?a:b;
}
typedef struct cigbsegy{
    int a;
    int b;
    int c;
}cigbsegy, SEGY;
#endif
#define _CHECK(arg)  \
{                    \
    cout<<"_CHECK"<<endl; \
}
```
结果：
```
rongtao@rongtao:~/test$ g++ -o a hello.cpp -fopenmp
rongtao@rongtao:~/test$ ./a 3
fun
CHECK:hello.cpp
CHECK:54
10
_CHECK
10
14
Hello World!
rongtao@rongtao:~/test$
```
