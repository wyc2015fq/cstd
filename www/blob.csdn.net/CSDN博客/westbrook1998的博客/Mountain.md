# Mountain - westbrook1998的博客 - CSDN博客





2018年10月06日 19:18:11[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：72标签：[思维](https://so.csdn.net/so/search/s.do?q=思维&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
链接：[https://www.nowcoder.com/acm/contest/206/H](https://www.nowcoder.com/acm/contest/206/H)

来源：牛客网

题目描述

平面上有n座山，每座山都有左右两面，第i座山的高度为ai，现在弱弱在第一座山的左边山脚下（高度为0），他想要依此爬过这些山，到达第n座山的右边山脚下。

除了简单的爬上爬下，还有一种特殊操作。

如果弱弱目前在第i座山右面的海拔x的位置，且第j ( i < j )座山的海拔大于等于x，且第座山中没有一座山的海拔高于x，那么他可以使用绳索滑到第j座山左面海拔x的位置。

弱弱想找到一种方式，使得他在行程中海拔变化的幅度最小。请输出最小幅度。

输入描述:

第一行一个整数n（1≤ n≤ 1000）。

接下来一行n个整数ai（1≤ ai≤ 1000）表示每座山的高度。

输出描述:

一行一个整数表示答案。

示例1

输入

复制

5

1 3 5 4 2

输出

复制

10
又是一道签到题，差点又想复杂了，想了差不多十分钟就是模拟上下，然后发现其实只要找最高的就好了，中间的操作其实都等于最高的上一次和下一次

代码：

```
#include <cstdio>
#include <algorithm>
using namespace std;
int main(void){
    int n,a;
    int Max=0;
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        scanf("%d",&a);
        Max=max(Max,a);
    }
    printf("%d\n",2*Max);
    return 0;
}
```





