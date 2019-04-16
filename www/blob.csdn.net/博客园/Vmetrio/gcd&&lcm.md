# gcd&&lcm - Vmetrio - 博客园







# [gcd&&lcm](https://www.cnblogs.com/wangmengmeng/p/5429944.html)







[1011 最大公约数GCD](https://www.51nod.com/onlineJudge/questionCode.html#!problemId=1011)![](https://file.51nod.com/images/icon/ok.png)

基准时间限制：1 秒 空间限制：131072 KB 分值: 0 [难度：基础题](https://www.51nod.com/onlineJudge/problemList.html#!groupId=1)



![](https://file.51nod.com/images/icon/star.png) 收藏

![](https://file.51nod.com/images/icon/plus.png) 关注





输入2个正整数A，B，求A与B的最大公约数。








Input
2个数A,B，中间用空格隔开。(1<= A,B <= 10^9)
Output
输出A与B的最大公约数。
Input示例
30 105
Output示例
15




代码:

```
1 #include <vector>
 2 #include <map>
 3 #include <set>
 4 #include <algorithm>
 5 #include <iostream>
 6 #include <cstdio>
 7 #include <cmath>
 8 #include <cstdlib>
 9 #include <string>
10 #include <cstring>
11 #include <queue>
12 #include <stack>
13 using namespace std;
14 
15 typedef long long ll;
16 
17 ll gcd(ll a,ll b)
18 {
19     if(b==0)
20         return a;
21     return gcd(b,a%b);
22 }
23 
24 int main()
25 {
26     ll a,b;
27     scanf("%lld%lld",&a,&b);
28     printf("%lld\n",gcd(a,b));
29     return 0;
30 }
```





[1012 最小公倍数LCM](https://www.51nod.com/onlineJudge/questionCode.html#!problemId=1012)![](https://file.51nod.com/images/icon/ok.png)

基准时间限制：1 秒 空间限制：131072 KB 分值: 0 [难度：基础题](https://www.51nod.com/onlineJudge/problemList.html#!groupId=1)



![](https://file.51nod.com/images/icon/star.png) 收藏

![](https://file.51nod.com/images/icon/plus.png) 关注





输入2个正整数A，B，求A与B的最小公倍数。








Input
2个数A,B，中间用空格隔开。(1<= A,B <= 10^9)
Output
输出A与B的最小公倍数。
Input示例
30 105
Output示例
210




代码:

```
1 #include <vector>
 2 #include <map>
 3 #include <set>
 4 #include <algorithm>
 5 #include <iostream>
 6 #include <cstdio>
 7 #include <cmath>
 8 #include <cstdlib>
 9 #include <string>
10 #include <cstring>
11 #include <queue>
12 #include <stack>
13 using namespace std;
14 
15 typedef long long ll;
16 
17 ll gcd(ll a,ll b)
18 {
19     if(b==0)
20         return a;
21     return gcd(b,a%b);
22 }
23 
24 ll lcm(ll a,ll b)
25 {
26     return a*b/gcd(a,b);
27 }
28 
29 int main()
30 {
31     ll a,b;
32     scanf("%lld%lld",&a,&b);
33     printf("%lld\n",lcm(a,b));
34     return 0;
35 }
```





## 1. 定义

最大公约数，也称最大公因数、最大公因子，指两个或多个整数共有约数中最大的一个。求最大公约数有多种方法，常见的有质因数分解法、短除法、辗转相除法、更相减损法。 
最小公倍数（Least Common Multiple，缩写L.C.M.），如果有一个自然数a能被自然数b整除，则称a为b的倍数，b为a的约数，对于两个整数来说，指该两数共有倍数中最小的一个。计算最小公倍数时，通常会借助最大公约数来辅助计算。

## 2. 辗转相除法（欧几里德算法）求最大公约数

核心： 
把上一轮有余数的除法计算中， 除数变为下一轮计算的被除数， 余数变为下一轮计算的除数， 一直这样计算下去， 直到最后一次计算余数为零， 在最后一轮计算中的被除数，即为所求的最大公约数 

## 3. 最小公倍数

最小公倍数常常借助于最大公约数的计算——最小公倍数等于两数之积除以其最大公约数












