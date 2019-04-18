# HDU 5501 The Highest Mark （贪心+DP，经典） - xcw0754 - 博客园
# [HDU 5501 The Highest Mark （贪心+DP，经典）](https://www.cnblogs.com/xcw0754/p/4868657.html)
题意：
　　有n道题目，每道题目的初始分数为Ai，分数每分钟减少Bi，完成此题需要Ci分钟，问在t分钟内最多能获得多少分？
思路：
　　好题~
　　如果没有B的话，就是一道裸的01背包的题目了。每道题目的得分为：v=A-B*g  （其中g为完成这道题目的时刻），想要用背包解的话是可以的，但是完成的次序不同的话，得分也可能受到影响。那就排个序得了，问题在于如何排序？假设已经确定要做哪些题了，则如果交换了相邻两道题的位置，对总分数是有益的，那么肯定是换了，而且他们对其他的题目完全无影响，不妨假设只有两道题要完成，现在要决定他们的完成次序。假设先完成题目1，再完成题目2，设t=C1+C2，那么总分为v1=A1-B1*C1 + A2-B2*ｔ；如果交换次序，则总分为v2=A2-B2*C2 + A1-B1*ｔ，那么v1<=v2时，明显有必要交换位置。
　　由v1<=v2得到   A1-B1*C1 + A2-B2*ｔ <= A2-B2*C2 + A1-B1*ｔ
　　化简得到 B1*C1 + B2*t >= B2*C2 + B1*ｔ
　　那么在排序时直接用这条式子就行了。
```
1 //#include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 #include <cstring>
 5 #include <cmath>
 6 #include <set>
 7 #include <deque>
 8 #include <map>
 9 #include <algorithm>
10 #include <vector>
11 #include <iostream>
12 #define pii pair<int,int>
13 #define back que[rear-1]
14 #define INF 0x3f3f3f3f
15 #define LL long long
16 #define ULL unsigned long long
17 using namespace std;
18 const double PI  = acos(-1.0);
19 const int N=1010;
20 struct node
21 {
22     int a, b, c;
23     double rate;
24 }seq[N];
25 int n, time, dp[N*3];
26 int cmp(node a,node b)
27 {
28     int t2=a.c+b.c;
29     return b.b*b.c+a.b*t2>=a.b*a.c+b.b*t2;
30 }
31 int cal()
32 {
33     sort(seq+1,seq+n+1, cmp);
34     memset(dp,-1,sizeof(dp));
35     dp[0]=0;
36     for(int i=1; i<=n; i++)
37         for(int j=time; j>=seq[i].c; j--)
38         {
39             if( dp[j-seq[i].c]<0 )   continue;
40             int v=seq[i].a - j*seq[i].b;
41             dp[j]=max( dp[j], dp[j-seq[i].c]+v );
42         }
43     int ans=0;
44     for(int i=time; i>=0; i--)
45         ans=max(ans, dp[i]);
46     return ans;
47 }
48 
49 int main()
50 {
51     freopen("input.txt", "r", stdin);
52     int t;cin>>t;
53     while(t--)
54     {
55         scanf("%d%d",&n,&time);
56         for(int i=1; i<=n; i++)
57             scanf("%d%d%d", &seq[i].a,&seq[i].b,&seq[i].c);
58         cout<<cal()<<endl;
59     }
60     return 0;
61 }
AC代码
```
用官方题解的代码：
```
1 //#include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 #include <cstring>
 5 #include <cmath>
 6 #include <set>
 7 #include <deque>
 8 #include <map>
 9 #include <algorithm>
10 #include <vector>
11 #include <iostream>
12 #define pii pair<int,int>
13 #define back que[rear-1]
14 #define INF 0x3f3f3f3f
15 #define LL long long
16 #define ULL unsigned long long
17 using namespace std;
18 const double PI  = acos(-1.0);
19 const int N=1010;
20 struct node
21 {
22     int a, b, c;
23     double rate;
24 }seq[N];
25 int n, time, dp[N*3];
26 int cmp(node a,node b)
27 {
28     return a.rate>b.rate;
29 }
30 int cal()
31 {
32     sort(seq+1,seq+n+1, cmp);
33     memset(dp,-1,sizeof(dp));
34     dp[0]=0;
35     for(int i=1; i<=n; i++)
36         for(int j=time; j>=seq[i].c; j--)
37         {
38             if( dp[j-seq[i].c]<0 )   continue;
39             int v=seq[i].a - j*seq[i].b;
40             dp[j]=max( dp[j], dp[j-seq[i].c]+v );
41         }
42     int ans=0;
43     for(int i=time; i>=0; i--)
44         ans=max(ans, dp[i]);
45     return ans;
46 }
47 
48 int main()
49 {
50     //freopen("input.txt", "r", stdin);
51     int t;cin>>t;
52     while(t--)
53     {
54         scanf("%d%d",&n,&time);
55         for(int i=1; i<=n; i++)
56         {
57             scanf("%d%d%d", &seq[i].a,&seq[i].b,&seq[i].c);
58             seq[i].rate=1.0*seq[i].b/seq[i].c;
59         }
60         cout<<cal()<<endl;
61     }
62     return 0;
63 }
AC代码
```

