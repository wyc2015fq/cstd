# HDU 3033 I love sneakers! 我爱运动鞋 （分组背包+01背包，变形） - xcw0754 - 博客园
# [HDU 3033 I love sneakers! 我爱运动鞋 （分组背包+01背包，变形）](https://www.cnblogs.com/xcw0754/p/4521810.html)
题意：
　　有n<=100双鞋子，分别属于一个牌子，共k<=10个牌子。现有m<=10000钱，问每个牌子至少挑1双，能获得的最大价值是多少？
思路：
　　分组背包的变形，变成了相反的，每组物品至少挑1件（分组背包问题是至多挑1件）。
　　由于每个牌子至少买1双，那么可以先装一件最便宜的进去，如果有好的再更新（注意每次的容量下限）。而且同一双鞋子不能多次购买，这里要用01背包。对于当前容量cap，可能只装了某一牌子的一双鞋子（不一定最便宜），也可能装了多双，也可能只装了那双硬塞进去的最便宜的。
　　注意点：有的店可是不一定有鞋子的；可能某个牌子连一双都买不起；可能买不全所有牌子。
 　　重写了次：
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
19 const int N=10010;
20 int dp[11][N];
21 struct node
22 {
23     int p,v;
24 }r;
25 vector<node> vect[N];
26 inline int cmp(node a,node b){return a.p<b.p;}
27 bool cal(int n,int m,int k)
28 {
29     for(int i=1; i<=n; i++) //排序方便处理
30         sort(vect[i].begin(),vect[i].end(),cmp);
31     int sum=0;  //最便宜的鞋子价钱之和
32     for(int i=1; i<=k; i++)             //枚举组
33     {
34         if(vect[i].empty()) return false;   //无鞋
35 
36         node a=vect[i][0];
37         for(int j=m; j-a.p>=sum; j--)       //先装进去一个最便宜的
38             dp[i][j]=dp[i-1][j-a.p]+a.v;
39 
40         for(int t=1; t<vect[i].size(); t++) //考虑此组其他鞋
41         {
42             a=vect[i][t];
43             for(int j=m; j-a.p>=sum; j--)
44             {
45                 dp[i][j]=max(dp[i][j], dp[i-1][j-a.p]+a.v );    //注意点
46                 dp[i][j]=max(dp[i][j], dp[i][j-a.p]+a.v );
47             }
48         }
49         sum+=vect[i][0].p;
50         if(sum>m)           return false;   //买不起
51     }
52     return true;
53 }
54 
55 int main()
56 {
57     //freopen("input.txt", "r", stdin);
58     int w, n, m, k;
59     while(cin>>n>>m>>k)     //n双鞋子，m钱，k个牌子
60     {
61         memset(dp,0,sizeof(dp));
62         for(int i=0; i<=k; i++)    vect[i].clear();
63         for(int i=1; i<=n; i++)
64         {
65             scanf("%d%d%d", &w, &r.p, &r.v);
66             vect[w].push_back(r);   //分组保存
67         }
68         if(cal(n,m,k))    printf("%d\n",dp[k][m]);
69         else              printf("Impossible\n");
70     }
71     return 0;
72 }
AC代码
```
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 int n, m, k, w, dp[15][11000];
 4 struct node
 5 {
 6     int p,v;
 7 }r;
 8 vector< vector<node> > vect;
 9 inline int cmp(node a,node b){return a.p< b.p? 1: 0;}
10 int cal()
11 {
12     memset(dp,0,sizeof(dp));
13     int up=0;
14     for(int i=1; i<=k; i++)             //每组
15     {
16         r=vect[i][0];
17         for(int j=m; j>=up+r.p; j--)    //先装进去每组中最便宜的一个，有更好的再更新
18             dp[i][j]=dp[i-1][j-r.p]+r.v;
19 
20         for(int t=1; t<vect[i].size(); t++)//同组每种鞋
21         {
22             r=vect[i][t];
23             for(int j=m; j>=up+r.p; j--)  //每种容量。注意下限是前面所有店的最便宜鞋价的总和。最差也能买上前面所有店的最便宜的鞋子，其他都是无效的状态。
24             {
25                 dp[i][j]=max(dp[i][j], dp[i-1][j-r.p]+r.v );    //单独放。
26                 dp[i][j]=max(dp[i][j], dp[i][j-r.p]  +r.v );    //配合同组放。
27             }
28         }
29         up+=vect[i][0].p;   //更新下限
30     }
31     if(dp[k][m]>0)    return 1;
32     else    return 0;
33 }
34 void init()
35 {
36     vect.clear();
37     vector<node> tmp;
38     for(int i=0; i<=k; i++) vect.push_back(tmp);
39 }
40 
41 int main()
42 {
43     freopen("input.txt", "r", stdin);
44     while(cin>>n>>m>>k)
45     {
46         init();
47         for(int i=0; i<n; i++)
48         {
49             scanf("%d%d%d", &w, &r.p, &r.v);
50             vect[w].push_back(r);//分组保存
51         }
52         int big=0;
53         for(int i=1; i<=k; i++)
54         {
55             sort(vect[i].begin(), vect[i].end(), cmp);    //排个序,最低价的排在前面
56             if(!vect[i].empty())    big+=vect[i][0].p;  //坑在这，有的店完全没有鞋子！
57             else    big=0x7fffffff;//既然没有鞋子，置为无穷大，表示买不起。
58         }
59         if(big>m){printf("Impossible\n");continue;}   //每家店最便宜的鞋子都买不起
60         if(cal())    printf("%d\n",dp[k][m]);
61         else    printf("Impossible\n");
62     }
63     return 0;
64 }
AC代码
```

