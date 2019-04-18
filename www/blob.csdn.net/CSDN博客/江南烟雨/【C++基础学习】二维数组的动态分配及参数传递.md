# 【C++基础学习】二维数组的动态分配及参数传递 - 江南烟雨 - CSDN博客
2011年09月27日 20:52:59[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：2059
**1. C语言动态分配二维数组**
(1)已知第二维
```cpp
char (*a)[N];//指向数组的指针
a = (char (*)[N])malloc(sizeof(char *) * m);
printf("%d\n", sizeof(a));//4，指针
printf("%d\n", sizeof(a[0]));//N，一维数组
free(a);
```
(2)已知第一维
```cpp
char* a[M];//指针的数组
int i;
for(i=0; i<M; i++)
a[i] = (char *)malloc(sizeof(char) * n);
printf("%d\n", sizeof(a));//4*M，指针数组
printf("%d\n", sizeof(a[0]));//4，指针
for(i=0; i<M; i++)
    free(a[i]);
```
(3)已知第一维，一次分配内存(保证内存的连续性)
```cpp
char* a[M];//指针的数组
int i;
a[0] = (char *)malloc(sizeof(char) * M * n);
for(i=1; i<M; i++)
a[i] = a[i-1] + n;
printf("%d\n", sizeof(a));//4*M，指针数组
printf("%d\n", sizeof(a[0]));//4，指针
free(a[0]);
```
(4)两维都未知
```cpp
char **a;
int i;
a = (char **)malloc(sizeof(char *) * m);//分配指针数组
for(i=0; i<m; i++)
{
a[i] = (char *)malloc(sizeof(char) * n);//分配每个指针所指向的数组
}
printf("%d\n", sizeof(a));//4，指针
printf("%d\n", sizeof(a[0]));//4，指针
for(i=0; i<m; i++)
{
free(a[i]);
}
free(a);
```
(5)两维都未知，一次分配内存(保证内存的连续性)
```cpp
char **a;
int i;
a = (char **)malloc(sizeof(char *) * m);//分配指针数组
a[0] = (char *)malloc(sizeof(char) * m * n);//一次性分配所有空间
for(i=1; i<m; i++)
{
a[i] = a[i-1] + n;
}
printf("%d\n", sizeof(a));//4，指针
printf("%d\n", sizeof(a[0]));//4，指针
free(a[0]);
free(a);
```
**2.C++动态分配二维数组**
(1)已知第二维
```cpp
char (*a)[N];//指向数组的指针
a = new char[m][N];
printf("%d\n", sizeof(a));//4，指针
printf("%d\n", sizeof(a[0]));//N，一维数组
delete[] a;
```
(2)已知第一维
```cpp
char* a[M];//指针的数组
for(int i=0; i<M; i++)
    a[i] = new char[n];
printf("%d\n", sizeof(a));//4*M，指针数组
printf("%d\n", sizeof(a[0]));//4，指针
for(i=0; i<M; i++)
    delete[] a[i];
```
(3)已知第一维，一次分配内存(保证内存的连续性)
```cpp
char* a[M];//指针的数组
a[0] = new char[M*n];
for(int i=1; i<M; i++)
a[i] = a[i-1] + n;
printf("%d\n", sizeof(a));//4*M，指针数组
printf("%d\n", sizeof(a[0]));//4，指针
delete[] a[0];
```
(4)两维都未知
```cpp
char **a;
a = new char* [m];//分配指针数组
for(int i=0; i<m; i++)
{
a[i] = new char[n];//分配每个指针所指向的数组
}
printf("%d\n", sizeof(a));//4，指针
printf("%d\n", sizeof(a[0]));//4，指针
for(i=0; i<m; i++)
delete[] a[i];
delete[] a;
```
(5)两维都未知，一次分配内存(保证内存的连续性)
```cpp
char **a;
a = new char* [m];
a[0] = new char[m * n];//一次性分配所有空间
for(int i=1; i<m; i++)
{
a[i] = a[i-1] + n;//分配每个指针所指向的数组
}
printf("%d\n", sizeof(a));//4，指针
printf("%d\n", sizeof(a[0]));//4，指针
delete[] a[0];
delete[] a;
```
多说一句：new和delete要注意配对使用，即有多少个new就有多少个delete，这样才可以避免内存泄漏！
**3.静态二维数组作为函数参数传递**
如果采用上述几种方法动态分配二维数组，那么将对应的数据类型作为函数参数就可以了。这里讨论静态二维数组作为函数参数传递，即按照以下的调用方式：
int a[2][3];
func(a);
C语言中将静态二维数组作为参数传递比较麻烦，一般需要指明第二维的长度，如果不给定第二维长度，则只能先将其作为一维指针传递，然后利用二维数组的线性存储特性，在函数体内转化为对指定元素的访问。
首先写好测试代码，以验证参数传递的正确性：
(1)给定第二维长度
```cpp
void func(int a[][N])
{
printf("%d\n", a[1][2]);
}
```
(2)不给定第二维长度
```cpp
void func(int* a)
{
printf("%d\n", a[1 * N + 2]);//计算元素位置
}
```
注意：使用该函数时需要将二维数组首地址强制转换为一维指针，即func((int*)a);
