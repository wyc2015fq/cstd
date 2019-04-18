# UVA 10288 Coupons 彩票 （数学期望） - xcw0754 - 博客园
# [UVA 10288 Coupons 彩票 （数学期望）](https://www.cnblogs.com/xcw0754/p/4754015.html)
题意：一种刮刮卡一共有n种图案，每张可刮出一个图案，收集n种就有奖，问平均情况下买多少张才能中奖？用最简的分数形式表示答案。n<=33。
思路：这题实在好人，n<=33。用longlong就可以表示分数了，不用去写大数。
　　假设现在已刮到k个图案了，刮到新图案的概率是(n-k)/n，即若要再收集一个新图案平均要刮s=n/(n-k)次。所以只需要穷举k=1 to n，累加s的和就行了。注意式子可以将分子n提取出来。
```
1 #include <bits/stdc++.h>
 2 #define pii pair<int,int>
 3 #define INF 0x3f3f3f3f
 4 #define LL long long
 5 using namespace std;
 6 const int N=33;
 7 LL lef[N+1], up[N+1], down[N+1];
 8 void pre_cal()
 9 {
10     memset(lef, 0, sizeof(lef));
11     memset(up, 0, sizeof(up));
12     memset(down, 0, sizeof(down));
13     lef[1]=1;
14     for(int i=2; i<=N; i++)
15     {
16         LL d1=1, d2=1;  //d1为分子，d2为分母
17         for(int j=2; j<=i; j++)
18         {
19             d1=d1*j+d2;
20             d2*=j;
21             LL p=__gcd(d1, d2);
22             d1/=p;
23             d2/=p;
24         }
25 
26         d1*=i;
27         LL p=__gcd(d1, d2);
28         d1/=p;d2/=p;    //约分
29 
30         lef[i]=d1/d2;
31         up[i]=d1%d2;
32         down[i]=d2;
33     }
34 
35 }
36 
37 
38 int main()
39 {
40     //freopen("input.txt", "r", stdin);
41     pre_cal();
42     int n;
43     while(~scanf("%d", &n))
44     {
45         if(up[n]==0)    printf("%lld\n", lef[n]);
46         else
47         {
48             for(LL i=lef[n]; i>0; i/=10 )    printf(" ");printf(" %lld\n", up[n]);   //****分子
49 
50             printf("%lld ", lef[n]);    //****左部
51             for(LL i=down[n]; i>0; i/=10)      printf("-");printf("\n");
52 
53             for(LL i=lef[n]; i>0; i/=10 )    printf(" ");
54             printf(" %lld\n", down[n]); //****分母
55         }
56     }
57     return 0;
58 }
AC代码
```

