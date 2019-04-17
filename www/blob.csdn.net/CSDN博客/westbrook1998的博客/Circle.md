# Circle - westbrook1998的博客 - CSDN博客





2018年10月06日 19:16:06[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：48标签：[思维](https://so.csdn.net/so/search/s.do?q=思维&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
链接：[https://www.nowcoder.com/acm/contest/206/C](https://www.nowcoder.com/acm/contest/206/C)

来源：牛客网

题目描述

现在我们要把这n个数字首尾连接组成一个环，使得相邻元素互质的对数尽可能多。请输出最大对数。

输入描述:

一行一个整数n(1≤ n≤ 1000)。

输出描述:

一行一个整数表示答案。

示例1

输入

复制

4

输出

复制

4

说明

样例的一种构造方法为1 4 3 2。
签到题

差点想复杂去搜索了，我突然提出这么多人过应该就是输入和输出吧，然后和队友突然就发现i和i+1肯定互质，然后1和n肯定也互质，那只要1到n连成一串就可以了

代码：

```
#include <cstdio>
#include <algorithm>
using namespace std;
int main(void){
    int n;
    scanf("%d",&n);
    printf("%d\n",n);
    return 0;
}
```





