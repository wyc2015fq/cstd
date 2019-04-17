# nyoj744——异或（sb题） - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






## 蚂蚁的难题(一)

时间限制：1000 ms  |  内存限制：65535 KB

难度：2



- 描述

小蚂蚁童鞋最近迷上了位运算，他感觉位运算非常神奇。不过他最近遇到了一个难题：

给定一个区间[a,b],在区间里寻找两个数x和y，使得x异或y最大。来，帮帮他吧！


- 输入
有多组测试数据（以EOF结尾）。
每组数据输入两个数a,b.(0<=a<b<2^63)- 输出
输出a到b之间，异或最大的值。- 样例输入
1 2
8 9- 样例输出
3
1- 来源
[蚂蚁系列](http://acm.nyist.edu.cn/JudgeOnline/search_result.php?source=%E8%9A%82%E8%9A%81%E7%B3%BB%E5%88%97)- 上传者
[ACM_李如兵](http://acm.nyist.edu.cn/JudgeOnline/profile.php?userid=ACM_%E6%9D%8E%E5%A6%82%E5%85%B5)
```
#include <bits/stdc++.h>
using namespace std;
typedef long long  ll;
const int INF = 0x3f3f3f3f;
const int maxn = 1000;
const int moder = 1000000007;

int main ()
{
    ll a,b;
    while(~scanf("%lld%lld",&a,&b)){
        ll m = a^b;
        ll cnt = 0;
        while(m){
            cnt++;
            m = m >> 1;
        }
        printf("%lld\n",((1ll)<<cnt)-1);  //注意1ll的用法，防止整数溢出
    }
    return 0;
}
```
令x=a^b; 这样做的目的是去除a和b的二进制的前面的相同的1，因为b是最大的。所以x的二进制的长度就是最大的数的二进制的长度。因为总有两个数可以异或之后二进制长度为x且全为1。










