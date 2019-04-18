# UVA 568 Just the Facts （水） - xcw0754 - 博客园
# [UVA 568 Just the Facts （水）](https://www.cnblogs.com/xcw0754/p/4606398.html)
题意：
　　求一个数n的阶乘，其往后数第1个不是0的数字是多少。
思路：
　　[1,n]逐个乘，出现后缀0就过滤掉，比如12300就变成123，继续算下去。为解决爆long long问题，将其余一个数mod，过滤掉前面过大的部分，因为计算出来也没用。这个mod应该是多少？ 10亿就行。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 using namespace std;
 4 const int N=10001;
 5 const int mod=1000000000;
 6 int ans[N];
 7 
 8 void init()
 9 {
10     LL fun=1;
11     ans[1]=1;
12     for(int i=2; i<N; i++)
13     {
14         fun*=i;
15         while(fun%10==0)
16             fun/=10;    //过滤掉后缀0
17         ans[i]=fun%10;          //取最后一位
18         fun%=mod;
19     }
20 }
21 
22 int main()
23 {
24     //freopen("input.txt", "r", stdin);
25     init();
26     int a;
27     while(~scanf("%d",&a))
28         printf("%5d -> %d\n",a,ans[a]);
29     return 0;
30 }
AC代码
```

