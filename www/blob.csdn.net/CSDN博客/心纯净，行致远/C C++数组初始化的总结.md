# C/C++数组初始化的总结 - 心纯净，行致远 - CSDN博客





2018年12月11日 16:19:15[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：1497








**目录**

[数组定义不初始化会被随机赋值](#%E6%95%B0%E7%BB%84%E5%AE%9A%E4%B9%89%E4%B8%8D%E5%88%9D%E5%A7%8B%E5%8C%96%E4%BC%9A%E8%A2%AB%E9%9A%8F%E6%9C%BA%E8%B5%8B%E5%80%BC)

[数组初始化的几种形式](#%E6%95%B0%E7%BB%84%E5%88%9D%E5%A7%8B%E5%8C%96%E7%9A%84%E5%87%A0%E7%A7%8D%E5%BD%A2%E5%BC%8F)

[数组初始化为某一固定值](#%E6%95%B0%E7%BB%84%E5%88%9D%E5%A7%8B%E5%8C%96%E4%B8%BA%E6%9F%90%E4%B8%80%E5%9B%BA%E5%AE%9A%E5%80%BC)

[memset(数组名，值，sizeof(数组名))](#memset(%E6%95%B0%E7%BB%84%E5%90%8D%EF%BC%8C%E5%80%BC%EF%BC%8Csizeof(%E6%95%B0%E7%BB%84%E5%90%8D)))

[fill(begin(),end(),value)](#fill(begin()%2Cend()%2Cvalue))

以前刚开始学数组初始化，一直都没搞太清楚，包括参加PAT考试，直到看了胡凡的《算法笔记》，逐渐清晰，是这样的：
- 
# 数组定义不初始化会被随机赋值


因此如果数组的所有元素在下面没有逐一赋值，但是又会使用到的话，最后不要只定义而不初始化。会带来问题。
- 
# 数组初始化的几种形式

- 可以直接用{}：a[10]={}，就可以让a[10]数组中的所有元素全为0；
- 也可以{}内包含0：a[10]={}，也可以让a[10]数组中的所有元素全为0；
- {}内写一个值只会让第一个元素为这个值，后面的依旧是0：a[3]={5}，数组实际上被初始化为{5，0，0}
- 
# 数组初始化为某一固定值


可以将数组初始化为某一固定值。有两种方法，一种是使用<string>(C++)或<string.h>(C)中的memset（）函数，还有一种是使用<algorithm>中的fill()函数，fill函数可以初始化容器和数组。
- 
## **memset(数组名，值，sizeof(数组名))**


只建议使用memset()的时候初始化为0或者-1。menset()函数的功能性不如fill（）函数，其优点在于执行速度更快。

程序实例：

```cpp
#include <stdio.h>
int main(){
    int a[10];
    memset(a,5,sizeof(a));
    for(int i=0;i<10;i++)
        printf("%d",a[i]);
    return 0;
}

//output:5555555555
```
- 
## fill(begin(),end(),value)


fill()可以把数组和容器中的某一段区间赋值为某个相同的值。和memset()不同的是这里的赋值可以是数组类型对应范围中的任意值。

程序实例：

```cpp
#include <iostream>
#include <algorithm>
using namespace std;
int main(){
    int a[10];
    fill(a,a+5,233);
    for(int i=0;i<10;i++)
        cout<<a[i]<<" ";
    return 0;
}

/*
233 233 233 233 233 0 31 0 0 0
--------------------------------
Process exited after 0.4914 seconds with return value 0
请按任意键继续. . .
*/
```

从这里也可以看出，a[]后面的元素是随机赋值的。

**建议采用fill（）而不是使用memset()。**



