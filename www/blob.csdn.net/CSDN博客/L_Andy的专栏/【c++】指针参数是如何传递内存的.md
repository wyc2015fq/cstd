# 【c++】指针参数是如何传递内存的 - L_Andy的专栏 - CSDN博客

2015年02月06日 09:43:59[卡哥](https://me.csdn.net/L_Andy)阅读数：790


参数策略

如果函数的参数是一个指针，不要指望用该指针去动态申请内存。如下：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

void GetMemory(char *p, int num)
{
    p = (char *)malloc(sizeof(char) * num);
}
void Test(void)
{
    char *str = NULL;
    GetMemory(str, 100);     //str仍未NULL
    strcpy(str, "hello");    //运行错误
}

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

原因是编译器总是为每个参数制作临时副本。指针参数p, 其副本为_p,使_p=p。如果改变了_p所指的内容，相应的p所指的内容也跟着改变（毕竟指向同样的地方）。但是在GetMemory中动态分配内存空间，改变了_p的内容。在调用函数中的p还是指向NULL。再者，因为函数GetMemory中动态分配了空间，但是没释放，这样调用一次函数，就泄露了一次内存。图示：
![](http://images.cnitblog.com/blog/408927/201308/08212002-6caa2b2bb75b4e059df696aa23ffa66c.png)

如果非得用指针参数申请内存，可以用指针的指针作为参数申请内存

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

void GetMemory(char **p, int num)
{
    *p = (char *)malloc(sizeof(char) * num);
}
void Test(void)
{
    char *str = NULL;
    GetMemory(&str, 100);     //记得加地址符
   strcpy(str, "hello");    
    free(str)
 }

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

原理是一样的，比较难理解，图示表示：
![](http://images.cnitblog.com/blog/408927/201308/08213443-0d52d3de68184231a2e33312fa4e39ec.png)

比较好的方法是传指针的引用

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
using namespace std;
void GetMemory(char *&p, int num)
{
    p = (char *)malloc(sizeof(char) * num);
}

void Test(void)
{
    char *str = NULL;
    GetMemory(str, 100);
    strcpy(str, "hello");
    cout << str << endl;
    free(str);
}
int main()
{
    Test();
}

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

 这里注意指针的引用 为char* &a，要是不好理解可以这样：

```
typedef char* pchar;
    pchar &a
```

返回值策略

可以用函数返回值来传递动态内存。这中方法比“指针的指针”简单多了

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

char *GetMemory(int num)
{
     char *p = (char *)malloc(sizeof(char) * num);
     return p;
}
void Test(void)
{
    char *str = NULL;
    str = GetMemory(100);  //str指向了动态分配的空间
    strcpy(str, "hello"); 
    free(str)
 }

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

在使用返回值时，千万别返回指向“栈内存”的指针、引用，因为该内存在函数结束时自动消亡了，返回的指针是个野指针了。例如

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

char *GetString()
{
     char p[] = "hello world"; //数组内容存储在栈区，函数结束时，会释放掉
     return p;
}
void Test(void)
{
    char *str = NULL;
    str = GetString();      //因为非配的内存早已释放掉，此时的str是个野指针，内容是垃圾
   cout << str << endl;
 }

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

在函数中不定义数组，定义指针，示例：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

char *GetString()
{
     char *p = "hello world"; //数组内容存储在静态区，函数结束时，不会释放掉
     return p;
}
void Test(void)
{
    char *str = NULL;
    str = GetString();      
    cout << str << endl;
 }

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

此时的程序是正确的，但是有一点，此时分配的内存处于静态区，是只可以读取但是不可以修改的。

