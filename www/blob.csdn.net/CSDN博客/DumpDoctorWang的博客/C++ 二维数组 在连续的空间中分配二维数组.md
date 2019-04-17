# C++ 二维数组 在连续的空间中分配二维数组 - DumpDoctorWang的博客 - CSDN博客





2018年07月03日 19:47:39[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：375








在C++11中，定义二维数组的有四种方法，两种固定分配的二维数组和两种动态分配的二维数组。在这里顺便提一下new int[]和new int[]()的区别，在VS中，前一种只分配内存空间，不做初始化，后一种分配空间并同时将元素初始化为0；使用MinGW编译时，都会初始化为0；

一、固定分配二维数组有两种方式

1、第一种，常量

```cpp
const int M = 100;
const int N = 200;
int arr[M][N];
```

2、第二种，宏定义

```cpp
#define M 100
#define N 200
    
int arr[M][N];
```

二、动态分配二维数组有两种方式

1、第一种，二维数组内的空间不连续

```cpp
int M = 100;
int N = 200;
int **arr = new int*[M];
for(int i=0;i<M;i++)
    arr[i] = new int[N];
```

释放数组内存代码：

```cpp
for(int i=0;i<M;i++)
    delete[] arr[i];
delete[] arr;
arr = nullptr;
```

2、第二种，二维数组存放在一组连续空间内

```cpp
int M = 100;
int N = 200;
int **arr = new int*[M];
int *buffer = new int[M*N];
for(int i=0;i<M;i++)
    arr[i] = buffer+i*N;
```

在上面的代码中，arr是一二维数组指针，buffer才是实际存放数据的内存。arr[i]实际上是一个int*类型的指针，是一个地址，指向buffer中的某一段内存的起始地址A，从起始地址A开始共N个元素，就相当于arr中的第i行的N个元素。

释放数组内存代码：

```cpp
delete []buffer;
buffer = nullptr;
delete []arr;
arr = nullptr;
```

4、补充

对于需要动态分配的多维数组，可以使用嵌套的多重vector，这才是推荐的方式。



