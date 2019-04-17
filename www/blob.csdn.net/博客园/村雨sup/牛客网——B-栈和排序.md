# 牛客网——B-栈和排序 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





链接：[https://www.nowcoder.com/acm/contest/58/B](https://www.nowcoder.com/acm/contest/58/B)
来源：牛客网



## 题目描述



给你一个1->n的排列和一个栈，入栈顺序给定

你要在不打乱入栈顺序的情况下，对数组进行从大到小排序

当无法完全排序时，请输出字典序最大的出栈序列


## 输入描述:
第一行一个数n
第二行n个数，表示入栈的顺序，用空格隔开，结尾无空格
## 输出描述:
输出一行n个数表示答案，用空格隔开，结尾无空格

示例1



## 输入

5
2 1 5 3 4



## 输出

5 4 3 1 2



## 说明

2入栈；1入栈；5入栈；5出栈；3入栈；4入栈；4出栈；3出栈；1出栈；2出栈




## 备注:
对于100%的数据，有1<=n<=1000000，保证给的是一个排列
```
#include<bits/stdc++.h>
using namespace std;
int n,now;
stack<int>s;
int main() {
    scanf("%d",&n);now=n;
    for(int i=1,x;i<=n;i++) {
        scanf("%d",&x);
        s.push(x);
        if(x==now) {
            printf("%d ",x);
            s.pop();
            now--;
        }
    }
    while(!s.empty())
        printf("%d",s.top()),s.pop(),putchar(s.empty()?'\n':' ');
}
```

想不到诶。












