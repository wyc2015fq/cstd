# UVA 11971 Polygon 多边形（连续概率） - xcw0754 - 博客园
# [UVA 11971 Polygon 多边形（连续概率）](https://www.cnblogs.com/xcw0754/p/4755446.html)
题意：
　　一根长度为n的木条，随机选k个位置将其切成k+1段，问这k+1段能组成k+1条边的多边形的概率？
思路：
　　数学题。要求的是概率，明显与n无关。
　　将木条围成一个圆后再开切k+1刀，得到k+1段。组不成多边形就是其中有一段特别长，比其他k段加起来还要长。先算出不能围成多边形的概率，那么就是圆上面必须要有一段的长度大于半个圆周长，且其他的k-1个位置都要在同一边。
　　第一个点随机选，概率为1，假设这个点就是木条要组成圆的那两端。接下来要选其他的k个点的位置，他们都在同一个半圆上的概率是(1/2)k。假设分成这样的k+1段，A0A1A2....AK。那么A0--A1就是一段了。假设是这一段最大且超过n的一半。那么其他的k-1个位置就必须在同一边且在偏短的那一边。共有k+1段，都有可能是最长的那段，所以概率(k+1)*(1/2)k。答案为1-(k+1)*(1/2)k。
　　这题其实在考逻辑和证明能力啊，不是很懂的感觉。但就大概这样理解过来的。建议参考一下这些证明：http://www.zhihu.com/question/25408010/answer/30732054 
```
1 #include <bits/stdc++.h>
 2 #define pii pair<int,int>
 3 #define INF 0x3f3f3f3f
 4 #define LL long long
 5 using namespace std;
 6 const int N=33;
 7 
 8 int main()
 9 {
10     //freopen("input.txt", "r", stdin);
11     int Case=0, n, k, t;
12     cin>>t;
13     while(t--)
14     {
15         scanf("%d%d", &n, &k);
16         LL up=((LL)1<<k)-k-1;
17         LL down=((LL)1<<k);
18 
19         LL g=__gcd(up, down);
20         up/=g;
21         down/=g;
22         printf("Case #%d: %lld/%lld\n", ++Case, up, down);
23 
24     }
25     return 0;
26 }
AC代码
```

