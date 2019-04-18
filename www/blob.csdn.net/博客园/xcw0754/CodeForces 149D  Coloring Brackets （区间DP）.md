# CodeForces  149D   Coloring Brackets （区间DP） - xcw0754 - 博客园
# [CodeForces  149D   Coloring Brackets （区间DP）](https://www.cnblogs.com/xcw0754/p/4842021.html)
题意：
　　给一个合法的括号序列，仅含()这两种。现在要为每对括号中的**其中一个**括号上色，有两种可选：蓝or红。要求不能有两个同颜色的括号相邻，问有多少种染色的方法？
思路：
　　这题的模拟成分比较多吧？两种颜色还有无色，用2个bit就可以表示了。然后就是各种转移，注意结果可能非常大，要取模后输出。转移主要是不让同颜色的括号在一起。处理时可以用DFS，在区间[L,R]中找到距离最远的所有合法括号，递归往下处理子问题，直到剩下一对括号直接处理就行了。
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
15 const int N=1000;
16 const int mod=1e9+7;
17 char s[N];
18 
19 LL dp[N][N][3][3];  //0无色，1红色，2蓝色
20 
21 void cal(int ll,int mid,int rr) //区间合并
22 {
23     if(ll==mid) return ;
24     for(int i=0; i<3; i++)
25     for(int j=0; j<3; j++)
26     for(int k=0; k<3; k++)
27     for(int u=0; u<3; u++)
28     if(k!=u||k==0)
29     dp[ll][rr][i][j]+=(dp[ll][mid-1][i][k] * dp[mid][rr][u][j])%mod;
30 }
31 void cal2(int ll,int rr)    //区间嵌套的
32 {
33     for(int k=0; k<3; k++)      //枚举内区间
34     for(int u=0; u<3; u++)
35     {
36         if( u!=1 )    dp[ll][rr][0][1]=(dp[ll][rr][0][1]+dp[ll+1][rr-1][k][u])%mod;
37         if( u!=2 )    dp[ll][rr][0][2]=(dp[ll][rr][0][2]+dp[ll+1][rr-1][k][u])%mod;
38         if( k!=1 )    dp[ll][rr][1][0]=(dp[ll][rr][1][0]+dp[ll+1][rr-1][k][u])%mod;
39         if( k!=2 )    dp[ll][rr][2][0]=(dp[ll][rr][2][0]+dp[ll+1][rr-1][k][u])%mod;
40     }
41 }
42 
43 void DFS(int L,int R)
44 {
45     if(L+1==R)
46     {
47         dp[L][R][1][0]=dp[L][R][0][1]=1;
48         dp[L][R][2][0]=dp[L][R][0][2]=1;
49         return ;
50     }
51     int q=L+1;
52     while( q<R )   //将[L->R]分成多个子区间
53     {
54         int p=q, cnt=0;
55         while( 1 )  //找对应的另一半括号)的位置
56         {
57             if( s[q]=='(' ) cnt++;
58             if( s[q]==')' ) cnt--;
59             if(cnt==0)      break;
60             q++;
61         }
62         DFS(p, q);      //计算区间[p->q]
63         cal(L+1, p, q); //两个区间合并
64         q++;
65     }
66     cal2(L,R);          //子区间并到大区间
67 }
68 
69 
70 int main()
71 {
72     //freopen("input.txt", "r", stdin);
73     memset(dp,0,sizeof(dp));
74     scanf("%s",s+1);
75     int n=strlen(s+1);
76     DFS(0, n+1);    //将s[0]和s[n+1]视为一对括号比较好处理
77     LL ans=0;
78 
79     for(int i=0; i<3; i++)
80         for(int j=0; j<3; j++)
81             ans=(ans+dp[1][n][i][j])%mod;
82 
83     cout<<ans<<endl;
84     return 0;
85 }
AC代码
```

