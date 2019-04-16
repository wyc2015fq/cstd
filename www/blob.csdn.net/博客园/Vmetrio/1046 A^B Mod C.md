# 1046 A^B Mod C - Vmetrio - 博客园







# [1046 A^B Mod C](https://www.cnblogs.com/wangmengmeng/p/5435651.html)







[1046 A^B Mod C](https://www.51nod.com/onlineJudge/questionCode.html#!problemId=1046)![](https://file.51nod.com/images/icon/ok.png)

基准时间限制：1 秒 空间限制：131072 KB 分值: 0 [难度：基础题](https://www.51nod.com/onlineJudge/problemList.html#!groupId=1)



![](https://file.51nod.com/images/icon/star.png) 收藏

![](https://file.51nod.com/images/icon/plus.png) 关注




给出3个正整数A B C，求A^B Mod C。



例如，3 5 8，3^5 Mod 8 = 3。





Input
3个正整数A B C，中间用空格分隔。(1 <= A,B,C <= 10^9)
Output
输出计算结果
Input示例
3 5 8
Output示例
3
相关问题

X^A Mod P 

320

[](https://www.51nod.com/onlineJudge/questionCode.html#!problemId=1038)
X^2 Mod P 

10

[](https://www.51nod.com/onlineJudge/questionCode.html#!problemId=1014)
X^A Mod B 

640

[](https://www.51nod.com/onlineJudge/questionCode.html#!problemId=1123)
X^3 Mod P 

640






快速幂......



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
17 ll mod_pow(ll x,ll y,ll mod)
18 {
19     ll ans=1;
20     while(y>0){
21         if(y&1)
22             ans=ans*x%mod;
23         x=x*x%mod;
24         y>>=1;
25     }
26     return ans;
27 }
28 
29 int main()
30 {
31     int a,b,c,n;
32     scanf("%d%d%d",&a,&b,&c);
33     printf("%d\n",mod_pow(a,b,c));
34     return 0;
35 }
```














