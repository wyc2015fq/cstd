# nyoj520——素数序列 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






## 最大素因子

时间限制：1000 ms  |  内存限制：65535 KB

难度：2



- 描述

GreyAnts最近正在学习数论中的素数，但是现在他遇到了一个难题：给定一个整数n,要求我们求出n的最大素因子的序数,例如：2的序数是1,3的序数是2,5的序数是3，以此类推. 研究数论是需要很大的耐心的，为了惩罚那些没有耐心读完题目的童鞋，我们规定：1的最大素因子序数是0.


- 输入
有多组测试数据,每一行输入一个数字n.(0<n<=1000000)- 输出
在接下来的一行，输出结果.- 样例输入
2
3
4
5- 样例输出
1
2
1
3- 来源
[hdu改编](http://acm.nyist.edu.cn/JudgeOnline/search_result.php?source=hdu%E6%94%B9%E7%BC%96)- 上传者
[ACM_李如兵](http://acm.nyist.edu.cn/JudgeOnline/profile.php?userid=ACM_%E6%9D%8E%E5%A6%82%E5%85%B5)
```
#include <bits/stdc++.h>
using namespace std;
typedef long long  ll;
const int INF = 0x3f3f3f3f;
const int maxn = 1000010;
const int moder = 1000000007;

int a[maxn];
int main ()
{
    memset(a,0, sizeof(a));
    int cnt = 0;
    for(int i=2;i < maxn;i++){
        if(a[i] == 0){
            cnt++;
            for(int j = i;j < maxn;j = j+i){
                a[j] = cnt;              //不断覆盖，在标记素数的同时完成了计数
            }
        }
    }
    int n;
    while(~scanf("%d",&n)){
        printf("%d\n",a[n]);
    }
    return 0;
}
```

——AC这辈子都不可AC了，自己写只有暴力吃T，只有看看大神题解才能过下去。











