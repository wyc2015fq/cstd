# HDU 5305 Friends (DFS，穷举+剪枝) - xcw0754 - 博客园
# [HDU 5305 Friends (DFS，穷举+剪枝)](https://www.cnblogs.com/xcw0754/p/4671478.html)
题意：
　　给定n个人，m对朋友关系，如果对于每个人，只能刚好选择其所有朋友中的一半的人进行聊天（只是我和我的朋友，不是我的朋友和我的朋友），那么有多少种情况？只要一个选择不同，视为不同情况。
思路：
　　比如我在14个朋友中选择了7个跟我聊天，那么另外7人已经完全与我没干系，而和我聊天的7个朋友，也已经和我聊天了，即我们配对了，共7对，他所选择的那一半的人中也必须有我。
　　其实只考虑所给的m条边就行了。如果是奇数对关系，必定有人是奇数个朋友，那么也就0种情况。如果是偶数条边，还得判断每个人是否都是偶数个朋友，若不是也是0种。
　　满足了情况之后再对m个关系选取其中的m/2条即可。但是所选的关系也必须是满足要求的，那么对于所选的m/2条关系进行判断即可知道是否满足要求，穷举所有可能进行判断。DFS就可以了，每条边要么选，要么不选。但是必须剪枝才能过。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 #define pii pair<int,int>
 4 #define INF 0x7f7f7f7f
 5 using namespace std;
 6 const int N=10;
 7 int n, m, s[65], e[65], num, times[N], du[N];
 8 int DFS(int x)  //x是第几条边
 9 {
10     if( num*2>=m )      //已经够一半了，判断是否满足要求
11     {
12         for(int i=1; i<=n; i++)    if( 2*du[i]!= times[i] ) return 0;   //每个人的度有一半即可。
13         return 1;
14     }
15 
16     int ans=0;
17     if(  du[ s[x] ]*2<times[ s[x] ] && du[ e[x] ]*2 <times[e[x]] )  //剪枝：这条边两个端点都已经满度，就不能再选了。
18     {
19         du[s[x]]++,du[e[x]]++;
20         num++;          //所选边的数量
21         ans+=DFS(x+1);
22         du[s[x]]--,du[e[x]]--;
23         num--;
24     }
25 
26     if( m/2-num < m-x  )     //还没有决定是否选的边数必须不小于ｍ的一半
27         ans+=DFS(x+1);
28     return ans;
29 }
30 
31 int cal(int n )
32 {
33     //先检查是否满足奇数度的要求
34     if(m&1)     return 0;
35     for(int i=1; i<=n; i++)    if( times[i]&1 )   return 0;
36 
37     num=0;
38     memset(du,0,sizeof(du));
39     return DFS(0);
40 }
41 
42 int main()
43 {
44     //freopen("e://input.txt", "r", stdin);
45     int t;
46     cin>>t;
47     while(t--)
48     {
49         memset(times, 0, sizeof(times));
50         scanf( "%d%d",&n,&m );
51         for(int i=0; i<m; i++)
52         {
53             scanf("%d%d",&s[i],&e[i]);
54             times[s[i] ]++; //记录朋友个数
55             times[e[i] ]++;
56         }
57         printf("%d\n",cal(n));
58     }
59     return 0;
60 }
AC代码
```

