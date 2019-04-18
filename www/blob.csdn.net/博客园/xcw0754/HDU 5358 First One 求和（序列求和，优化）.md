# HDU 5358 First One 求和（序列求和，优化） - xcw0754 - 博客园
# [HDU 5358 First One 求和（序列求和，优化）](https://www.cnblogs.com/xcw0754/p/4712257.html)
题意：给定一个含n个元素的序列，求下式子的结果。S(i,j)表示为seq[i...j]之和。注：对于log20可视为1。数据量n<=105。
![](https://images0.cnblogs.com/blog2015/641737/201508/072253003623441.png)
思路：即使能够在O(1)的时间内求得任意S，也是需要O(n*n)来求和的。
　　对于这种题，一般就是研究式子，看有什么办法可以减少复杂度。
　　看到式子中的**向下取整**符号了吗？很多数的取整结果是相同的，即使给个2147483647取整也只是30多而已（噗，忘了多少）。
　　而对于这个式子，S最大也不会超过longlong，确切计算，小于234。那么取log之后的范围这么小，如果能够知道分别有多少个的话，那就快多了。可以看得出对于同一个i，取log后的结果是呈线性的，从1到34逐步递增的（当然有可能没有那么大/小）。
　　那很好办，对于每个i，只需要将一整段“取log后向下取整的结果加1”为k的给截出来，统计k个（i,j）之和再乘以k不就是这一段的答案了吗？那么对于每个i，最多可能被截成34段啦。相比而言快了许多。复杂度为O(34*n)。
　　但是本题连这样的复杂度还是不行，还能继续优化，设![](https://images0.cnblogs.com/blog2015/641737/201508/072305030809747.png)为k。
　　（1）先穷举k，k=[1,34].
　　（2）再穷举i。对于每个i，假设i-1中(i-1, R)这一段的结果为k，而i中答案为k的对应段必定大于等于这段（为什么？小学老师教的序列求和技术）。所以只需从上次穷举完之处继续往后判断即可，因为i比i-1少了个数字，说不定就变小了许多，再加几个不就补回来啦！
　　再具体的话就很难说清了，看代码注释吧。我看得懂相信你也可以。
```
1 #include <bits/stdc++.h>
 2 #define INF 0x7f7f7f7f
 3 #define pii pair<int,int>
 4 #define LL long long
 5 using namespace std;
 6 const int N=100100;
 7 LL sum[N], up[50];
 8 int cur[N];
 9 void pre_cal()
10 {
11     sum[0]=0;
12     for(int i=0; i<40; i++)    up[i]= (LL)1<<i;
13 }
14 LL cal(int n)
15 {
16     for(int i=0; i<=n; i++) cur[i]=i;   //记录以i为下标的，穷举到那里了。
17 
18     LL ans=0;
19     int L=1, R=1;
20     for(int k=0; k<35; k++)
21     {
22         R=cur[1];
23         for(int i=1; i<=n; i++) //以i为下标的
24         {
25             L=cur[i];
26             R=max(cur[i], cur[i-1]);     //这一步决定了AC或者TLE
27             if(L>n) continue;            //以i为下标的已经计算完毕。
28 
29             while( R<=n &&  sum[R]-sum[i-1]<up[k] )    R++;     //找到（logx+1）为k的一段[L,R)
30             if(L<R)
31             {
32                 cur[i]=R;
33                 if((R-L)&1)          ans+=( (LL)(R-L)*i + (LL)(R-1+L)/2*(R-L) )*max(1,k);   //注意这里千万要转longlong
34                 else                 ans+=( (LL)(R-L)*i + (LL)(R-1+L)*(R-L)/2 )*max(1,k);
35             }
36         }
37     }
38 
39     return ans;
40 }
41 
42 int main()
43 {
44     freopen("input.txt", "r", stdin);
45     pre_cal();
46     int t, n;
47     scanf("%d", &t);
48     while(t--)
49     {
50         scanf("%d", &n);
51         for(int i=1; i<=n; i++)
52         {
53             scanf("%lld", &sum[i]);
54             sum[i] += sum[i-1];
55         }
56         printf("%lld\n", cal(n));
57     }
58     return 0;
59 }
AC代码
```

