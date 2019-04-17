# nyoj993——容斥 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






## How many integers can you find

时间限制：1000 ms  |  内存限制：65535 KB

难度：1



- 描述

给你三个数,n,m1,m2,找出所有小于n的能被m1或m2整除的数的个数。

![](http://acm.nyist.edu.cn/JudgeOnline/admin/kind/attached/20140722093632_78280.jpg)


- 输入
输入包含多组测试数据，每组数据占一行。
0<n<2^31,0<m1，m2<=10。- 输出
每组数据输出占一行。- 样例输入
12 2 3- 样例输出
7- 来源
[爱生活](http://acm.nyist.edu.cn/JudgeOnline/search_result.php?source=%E7%88%B1%E7%94%9F%E6%B4%BB)- 上传者
[TCM_张鹏](http://acm.nyist.edu.cn/JudgeOnline/profile.php?userid=TCM_%E5%BC%A0%E9%B9%8F)


```
#include <bits/stdc++.h>
using namespace std;
typedef long long  ll;
const int INF = 0x3f3f3f3f;
const int maxn = 2000;
const int moder = 1000000007;

int gcd(int a,int b)
{
    if(b == 0) return a;
    return gcd(b,a%b);
}


int main()
{
    int n,m1,m2;
    while(~scanf("%d%d%d",&n,&m1,&m2)){
        int m3 = m1*m2/gcd(m1,m2);
        int a,b,c;
        if(n%m1 == 0)
            a = n/m1-1;
        else
            a = n/m1;
        if(n%m2 == 0)
            b = n/m2-1;
        else
            b = n/m2;
        if(n%m3 == 0)
            c = n/m3-1;
        else
            c = n/m3;
        printf("%d\n",a+b-c);
    }
    return 0;
}
```

——注意的是，题目中说的是小于n，没有等于，容斥公式是把最后一个算进去的，所以要减一，细节要注意，wa了一发。











