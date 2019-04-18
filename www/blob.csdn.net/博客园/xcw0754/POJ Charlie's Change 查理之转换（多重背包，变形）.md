# POJ Charlie's Change 查理之转换（多重背包，变形） - xcw0754 - 博客园
# [POJ Charlie's Change 查理之转换（多重背包，变形）](https://www.cnblogs.com/xcw0754/p/4490781.html)
题意:
　　给定身上的4种硬币，分别是1 ，5 ，10， 25面额各有多张，要求组成面额p的硬币尽可能多。输出组成p的4种硬币各自的数量。
思路：
　　多重背包，300+ms。用01背包+二进制的方法。记录下所有的硬币的个数以及4种硬币分别的个数，注意初始化dp数组的不同之处。
```
1 #include <iostream>
 2 #include <cstdio>
 3 #include <cstring>
 4 #define LL long long
 5 using namespace std;
 6 
 7 const int  N=10010, INF=-1;
 8 int coin[4]={1, 5, 10, 25};
 9 int dp[N][5], num[5];
10 int p;
11 
12 void cal()
13 {
14     memset(dp,0,sizeof(dp));
15     for(int i=1; i<=p; i++) dp[i][0]=INF;
16     for(int i=0; i<4; i++)
17     {
18         int k=1, tmp=num[i];
19         while(1)
20         {
21             if(k>tmp&&tmp)    k=tmp;
22             else if(k>tmp)    break;
23 
24             for(int j=p; j>=k*coin[i]; j--)
25             {
26                 if( dp[j-k*coin[i]][0]!=INF &&dp[j][0]<dp[j-k*coin[i]][0]+k)
27                 {
28                     dp[j][0]=dp[j-k*coin[i]][0]+k;
29                     for(int c=1; c<=i+1; c++)       //复制次数
30                         dp[j][c]=dp[j-k*coin[i]][c];
31                     dp[j][i+1]+=k;  //添加次数
32                 }
33             }
34             tmp-=k;
35             k<<=1;
36         }
37     }
38     if(dp[p][0]==INF)    printf("Charlie cannot buy coffee.\n");
39     else    printf("Throw in %d cents, %d nickels, %d dimes, and %d quarters.\n", dp[p][1], dp[p][2], dp[p][3], dp[p][4] );
40 }
41 
42 
43 int main()
44 {
45 
46     //freopen("input.txt", "r", stdin);
47     while(cin>>p,p)
48     {
49         int cnt=0;
50         for(int i=0; i<4; i++)
51         {
52             scanf("%d", &num[i]);
53             cnt+=num[i]*coin[i];
54         }
55         if(cnt<p)
56         {
57             printf("Charlie cannot buy coffee.\n");
58             continue;
59         }
60         cal();
61     }
62     return 0;
63 }
AC代码
```

