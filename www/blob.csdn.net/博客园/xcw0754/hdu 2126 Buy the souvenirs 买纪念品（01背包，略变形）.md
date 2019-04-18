# hdu 2126 Buy the souvenirs 买纪念品（01背包，略变形） - xcw0754 - 博客园
# [hdu 2126 Buy the souvenirs 买纪念品（01背包，略变形）](https://www.cnblogs.com/xcw0754/p/4488239.html)
题意：
　　给出一些纪念品的价格，先算出手上的钱最多能买多少种东西k，然后求手上的钱能买k种东西的方案数。也就是你想要买最多种东西，而最多种又有多少种组合可选择。
思路：
　　01背包。显然要先算出手上的钱m最多能买多少种东西k，可以从价格最少的纪念品买起，看最多能买多少种，置为k。接下来按照常规01背包计算，需要记录下方案数和组成的物品数，看代码就会懂的。
```
1 #include <iostream>
 2 #include <stdio.h>
 3 #include <cstring>
 4 #include <algorithm>
 5 using namespace std;
 6 const int N=35;
 7 int n, m, k, sum;
 8 int p[N],cbn[510][N];
 9 int cal()
10 {
11     memset(cbn,0,sizeof(cbn));
12     cbn[0][0]=1;    //必要的初始化
13     for(int i=0; i<n ;i++)    //对于每件物品
14     {
15         for(int j=m; j>=p[i]; j--)    //对于每种价格
16         {
17             for(int e=0; e<=n; e++)     //组成j价格的方案数是cbn[j]的总和，其中下标e表示由e种纪念品组成的。
18                 cbn[j][e+1]+=cbn[j-p[i]][e];
19         }
20     }
21     int cnt=0;
22     for(int i=m; i>=sum; i--)    //价格在sum～m，其组合物品数为k的方案数，统计起来就是答案。
23         cnt+=cbn[i][k];
24     return cnt;
25 }
26 int main()
27 {
28     //freopen("input.txt","r",stdin);
29     int t;
30     cin>>t;
31     while(t--)
32     {
33         cin>>n>>m;
34         for(int i=0; i<n; i++)    scanf("%d", &p[i] );
35         sort(p, p+n);
36         for(sum=k=0; k<n; k++)
37         {
38             sum+=p[k];
39             if(sum>m)
40             {
41                 sum-=p[k];    //这个sum有用的，是买最多件物品的最少价格。
42                 break;
43             }        
44         }
45         
46         if(k==0)    //没钱
47         {
48             printf("Sorry, you can't buy anything.\n");
49             continue;
50         }
51         printf("You have %d selection(s) to buy with %d kind(s) of souvenirs.\n", cal(), k );
52     }
53     return 0;
54 }
AC代码
```

