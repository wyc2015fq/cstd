# HDU 5313 Bipartite Graph （二分图着色，dp） - xcw0754 - 博客园
# [HDU 5313 Bipartite Graph （二分图着色，dp）](https://www.cnblogs.com/xcw0754/p/4677721.html)
题意：
　　Soda有一个n个点m条边的二分图, 他想要通过加边使得这张图变成一个边数最多的完全二分图. 于是他想要知道他最多能够新加多少条边. 注意重边是不允许的.
思路：
　　先将二分图着色，将每个连通分量区分出左右两边的点，在着色过程中，顺便将每个连通分量两边的点数存起来，注意一个连通分量左右两边的点数是绑定的一对数字。现在的问题就是，需要将这几对数字给分配掉，每一对都必须拆开来丢到不同的桶中，而且尽量使得两桶中各自的数字之和最接近，才能使得边最多。
　　分配的过程可以使用DP解决，主要是数字挺大的，bitset优化的背包很强大，可以解决这个问题。
bitset优化了的代码：
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 #define pii pair<int,int>
 4 #define INF 0x7f7f7f7f
 5 using namespace std;
 6 const int N=10100;
 7 vector<int> vect[N];
 8 int col[N], set1, set2;
 9 
10 void color(int u, int c)
11 {
12     col[u]=c;
13     col[u]==1?set2++:set1++;//统计两边的数量
14     for(int i=0; i<vect[u].size(); i++)
15     {
16         int t=vect[u][i];
17         if(!col[t])    color(t, 3-col[u] );
18     }
19 }
20 
21 
22 bitset<N> dp;
23 int s1[N/2], s2[N/2];
24 int cal(int n, int m)
25 {
26     if(m==n/2*(n-n/2) ) return 0;
27     dp.reset();
28 
29     memset(col, 0, sizeof(col));
30     int ans=0, k=0;
31     for(int i=1; i<=n; i++)
32     {
33         if(!col[i])
34         {
35             set1=set2=0;
36             color(i, 1);
37 
38             s1[k]=set1;//每个连通分量两边的点数
39             s2[k++]=set2;
40         }
41     }
42 
43     dp[0]=1;
44     for(int i=0; i<k; i++)      //类似于背包+bitset优化
45         dp = dp<<s1[i] | dp<<s2[i];  //这么神奇！！
46         
47     for(int i=n/2; i>=0; i--)   //再将那个最接近n/2的找出来即可
48         if( dp[i] )   return i*(n-i)-m;
49 }
50 
51 
52 int main()
53 {
54     freopen("input.txt", "r", stdin);
55     int n, m, t, p, q, a, b;
56     cin>>t;
57 
58     while(t--)
59     {
60         scanf("%d%d",&n,&m);
61         for(int i=0; i<=n; i++) vect[i].clear();
62         for(int i=0; i<m; i++)
63         {
64             scanf("%d%d",&a,&b);
65             vect[a].push_back(b);
66             vect[b].push_back(a);
67         }
68         printf("%d\n",cal(n, m));
69     }
70     return 0;
71 }
AC代码
```
贴一个没有优化的TLE代码：
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 #define pii pair<int,int>
 4 #define INF 0x7f7f7f7f
 5 using namespace std;
 6 const int N=10100;
 7 vector<int> vect[N];
 8 int col[N];
 9 int set1, set2;
10 
11 void color(int u, int c)
12 {
13     col[u]=c;
14     if(col[u]==1) set1++;//统计两边的数量
15     else        set2++;
16 
17     for(int i=0; i<vect[u].size(); i++)
18     {
19         int t=vect[u][i];
20         if(!col[t])   //没色
21             color(t, 3-col[u] );
22     }
23 }
24 
25 
26 bitset<N/2> dp[N];
27 int s1[N/2],s2[N/2];
28 int cal(int n, int m)
29 {
30     if(m==n/2*(n-n/2) ) return 0;
31     for(int i=0; i<=n; i++) vect[i].clear(), dp[i].reset();
32 
33     memset(col, 0, sizeof(col));
34     int ans=0, k=0;
35     for(int i=1; i<=n; i++)
36     {
37         if(!col[i])
38         {
39             set1=set2=0;
40             color(i, 1);
41 
42             s1[k]=set1;//每个连通分量两边的点数
43             s2[k++]=set2;
44         }
45     }
46 
47     if(k==1)    return s1[0] * s2[0] - m;//连通图
48     dp[0][0]=1;
49     for(int i=0; i<k; i++)//在这进行DP，尽量使得两边的点数平衡
50     {
51         ans=0;
52         set1=s1[i];
53         set2=s2[i];
54 
55         for(int j=n/2; j>=set1; j--)
56             if( dp[i][j-set1] )    dp[i+1][j]=1,ans=max(ans,j);
57 
58         for(int j=n/2; j>=set2; j--)
59             if( dp[i][j-set2] )    dp[i+1][j]=1,ans=max(ans,j);
60     }
61     return ans*(n-ans)-m;
62 }
63 
64 
65 int main()
66 {
67     //freopen("input.txt", "r", stdin);
68     int n, m, t, p, q, a, b;
69     cin>>t;
70 
71     while(t--)
72     {
73         scanf("%d%d",&n,&m);
74         for(int i=0; i<m; i++)
75         {
76             scanf("%d%d",&a,&b);
77             vect[a].push_back(b);
78             vect[b].push_back(a);
79         }
80         printf("%d\n",cal(n, m));
81     }
82     return 0;
83 }
TLE代码
```

