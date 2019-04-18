# POJ 1845  Sumdiv （数学，乘法逆元） - xcw0754 - 博客园
# [POJ 1845  Sumdiv （数学，乘法逆元）](https://www.cnblogs.com/xcw0754/p/4844845.html)
题意：
　　给出数字A和B，要求AB的所有因子（包括AB和1）之和 mod 9901 的结果。
思路：
　　即使知道公式也得推算一阵子。
　　很容易知道，先把![](http://img.blog.csdn.net/20140613112543656)分解得到![](http://img.blog.csdn.net/20140613113059203)，那么得到![](http://img.blog.csdn.net/20140613113255234)，那么![](http://img.blog.csdn.net/20140613112622265)的所有因子之和的表达式如下：
![](http://img.blog.csdn.net/20140613132246375)
　　我们要做的就是计算出sum%9901的结果。
　　有两种方法：
　　（1）直接用快速幂计算对上面sum的第一步推算求结果，在计算过程中顺便取模。
　　（2）可以根据以下这条公式对上面sum的第二步推算求结果：
![](http://img.blog.csdn.net/20140613105646406)
　　　　也是需要用到快速幂，过程也稍微复杂了些。注意 mb 可能会超过int。
　　以下是第二种方法的代码：
```
1 //#include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 #include <cstring>
 5 #include <cmath>
 6 #include <map>
 7 #include <algorithm>
 8 #include <vector>
 9 #include <iostream>
10 #define pii pair<int,int>
11 #define INF 0x3f3f3f3f
12 #define LL unsigned long long
13 using namespace std;
14 const double PI  = acos(-1.0);
15 const int N=10010;
16 const LL mod=9901;
17 bool isPrime[N];
18 LL p[N];       //素数表
19 
20 int get_all_prime(int n)    //筛法求所有[0~n)素数，返回素数表大小
21 {
22     memset(isPrime, 1, sizeof(isPrime));
23     int cnt=0;
24     for(int i=2; i<n; i++)
25     {
26         if(!isPrime[i])    continue;
27         p[cnt++]=i;
28         for(int j=i*i; j<n; j+=i)    isPrime[j]=0;
29     }
30     return cnt;
31 }
32 
33 LL _mul(LL a,LL b,LL mod)   //a*b要用加法形式运算才不会溢出
34 {
35     a%=mod;
36     LL r=0;     //结果
37     while( b )
38     {
39         if( b&1 )     r=(r+a)%mod;
40         a=(a+a)%mod;
41         b>>=1;
42     }
43     return r;
44 }
45 
46 LL pow(LL a,LL b,LL mod)    //快速幂函取模
47 {
48     a%=mod;
49     LL r=1;     //结果
50     while( b )
51     {
52         if( b&1 )     r=_mul(r,a,mod);
53         a=_mul(a,a,mod);
54         b>>=1;
55     }
56     return r;
57 }
58 
59 LL cal(LL A,LL B)
60 {
61     LL ans=1;
62     for(int i=0; p[i]*p[i]<=A; i++ )    //先求A的所有质因子
63     {
64         if(A%p[i]==0)
65         {
66             int cnt=0;
67             while(A%p[i]==0)    //全部取光
68             {
69                 cnt++;
70                 A/=p[i];
71             }
72             LL mb=mod*(p[i]-1);
73             ans*=(pow(p[i], cnt*B+1, mb)+mb-1)%mb/(p[i]-1) ;  //要防止出现负数
74             ans%=mod;
75         }
76     }
77 
78 
79     if(A>1)
80     {
81         //如果没有把A成功分解，那么必定是个质数。
82         //其实也可以写在上面那一步中，只是复杂度就会稍高了。
83         LL mb=mod*(A-1);
84         ans*=(pow(A, B+1, mb)+mb-1)%mb/(A-1) ;  //要防止出现负数
85         ans%=mod;
86     }
87 
88     return ans;
89 }
90 
91 int main()
92 {
93     //freopen("input.txt", "r", stdin);
94     get_all_prime(N);
95     int A, B;
96     while(~scanf("%d%d",&A,&B))
97         printf("%lld\n", cal(A,B) );
98     return 0;
99 }
AC代码
```

