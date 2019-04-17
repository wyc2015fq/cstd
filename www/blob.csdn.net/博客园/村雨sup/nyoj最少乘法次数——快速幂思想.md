# nyoj最少乘法次数——快速幂思想 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






## 最少乘法次数

时间限制：1000 ms  |  内存限制：65535 KB

难度：3



- 描述

给你一个非零整数，让你求这个数的n次方，每次相乘的结果可以在后面使用，求至少需要多少次乘。如24：2*2=22（第一次乘），22*22=24（第二次乘），所以最少共2次；




- 输入
第一行m表示有m(1<=m<=100)组测试数据；
每一组测试数据有一整数n（0<n<=10000）;- 输出
输出每组测试数据所需次数s;- 样例输入
3
2
3
4- 样例输出
1
2
2- 上传者
[李剑锋](http://acm.nyist.edu.cn/JudgeOnline/profile.php?userid=%E6%9D%8E%E5%89%91%E9%94%8B)
```
#include <bits/stdc++.h>
using namespace std;
typedef long long  ll;
const int INF = 0x3f3f3f3f;
const int maxn = 2000;

int main()
{
    int t;
    scanf("%d",&t);
    while(t--){
        int n;
        scanf("%d",&n);
        int cnt = 0;
        int num = 0;
        while(n){
            cnt++;
            if(n&1) num++;
            n >>= 1;
        }
        int ans = cnt+num-2;
        printf("%d\n",ans);
    }

    return 0;
}
```

——

蛮有思考性的一道题，cnt ：最高位数-1 表示的是获取最大数需要乘的次数，num：表示有多少个1，num-1就是这几个  1 乘起来的次数。











