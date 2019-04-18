# C++  sort函数 - 心纯净，行致远 - CSDN博客





2018年11月22日 10:39:49[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：189








# sort函数

用于C++中，对给定区间所有元素进行排序。头文件是#include <algorithm>

### （一）为什么要用c++标准库里的排序函数

Sort（）函数是c++一种排序方法之一，学会了这种方法也打消我学习c++以来使用的冒泡排序和选择排序所带来的执行效率不高的问题！因为它使用的排序方法是类似于快排的方法，时间复杂度为n*log2(n)，执行效率较高！

### （二）c++标准库里的排序函数的使用方法

I）Sort函数包含在头文件为#include<algorithm>的c++标准库中，调用标准库里的排序方法可以不必知道其内部是如何实现的，只要出现我们想要的结果即可！

II）Sort函数有三个参数：

（1）第一个是要排序的数组的起始地址。

（2）第二个是结束的地址（最后一位要排序的地址的下一地址）

（3）第三个参数是排序的方法，可以是从大到小也可是从小到大，还可以不写第三个参数，此时默认的排序方法是从小到大排序。

Sort函数使用模板:

**Sort(start,end,排序方法)**

下面就具体使用sort（）函数结合对数组里的十个数进行排序做一个说明！

**例一：sort函数没有第三个参数，实现的是从小到大**

```cpp
#include <iostream>

#include <algorithm>

using namespace std;

int main()

{

int a[10]={9,6,3,8,5,2,7,4,1,0};

for(int i=0;i<10;i++)

cout<<a[i]<<endl;

sort(a,a+10);

for(int i=0;i<10;i++)

cout<<a[i]<<endl;

return 0;

}
```



例二：通过上面的例子，会产生疑问：要实现从大到小的排序肿么办？

这就如前文所说需要在sort（）函数里的第三个参数里做文章了，告诉程序我要从大到小排序！

需要加入一个比较函数compare(),此函数的实现过程是这样的
|1234|bool compare(int a,int b){return a>b;}|
|----|----|

这就是告诉程序要实现从大到小的排序的方法！

```cpp
#include <iostream>
#include <algorithm>
using namespace std;
bool compare(int a,int b)
{
return a>b;
}
int main()
{
int a[10]={9,6,3,8,5,2,7,4,1,0};
for(int i=0;i<10;i++)
cout<<a[i]<<endl;
sort(a,a+10,compare);//在这里就不需要对compare函数传入参数了，
//这是规则
for(int i=0;i<10;i++)
cout<<a[i]<<endl;
return 0;
}
假设自己定义了一个结构体node
struct node
{
int a;
int b;
double c;
}
有一个node类型的数组node arr[100]，想对它进行排序：先按a值升序排列，如果a值相同，再按b值降序排列，如果b还相同，就按c降序排列。就可以写这样一个比较函数：
以下是代码片段：
bool cmp(node x,node y)
{
if(x.a!=y.a) return x.a<y.a;
if(x.b!=y.b) return x.b>y.b;
return x.c>y.c;
}
```





