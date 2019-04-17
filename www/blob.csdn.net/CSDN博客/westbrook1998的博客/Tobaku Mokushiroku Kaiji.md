# Tobaku Mokushiroku Kaiji - westbrook1998的博客 - CSDN博客





2018年10月01日 18:02:03[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：68标签：[水题](https://so.csdn.net/so/search/s.do?q=水题&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
链接：[https://www.nowcoder.com/acm/contest/201/A](https://www.nowcoder.com/acm/contest/201/A)

来源：牛客网

题目描述

Kaiji正在与另外一人玩石头剪刀布。双方各有一些代表石头、剪刀、布的卡牌，每局两人各出一张卡牌，根据卡牌的内容决定这一局的胜负。胜负规则为：石头赢剪刀、剪刀赢布、布赢石头、相同为平局。每张卡牌至多被使用一次。

已知双方的卡牌数量，问Kaiji最多赢几局？

输入描述:

一行六个数字0 ≤ a, b, c, d, e, f ≤ 50，a,b,c分别表示Kaiji的石头、剪刀、布的牌的数量，d,e,f分别表示此时另一人的石头、剪刀、布的牌的数量。

输出描述:

一个整数表示Kaiji最多赢几局。

示例1

输入

复制

29 7 41 14 12 42

输出

复制

33
emmm暴力

代码：

```
#include <cstdio>
#include <algorithm>
using namespace std;
int a,b,c,d,e,f;
int main(void){
    scanf("%d%d%d%d%d%d",&a,&b,&c,&d,&e,&f);
    printf("%d\n",min(a,e)+min(b,f)+min(c,d));
    return 0;
}
```





