# HDU 3652 B-number （数位DP，入门） - xcw0754 - 博客园
# [HDU 3652 B-number （数位DP，入门）](https://www.cnblogs.com/xcw0754/p/4854655.html)
题意：
　　如果一个整数能被13整除，且其含有子串13的，称为"B数"，问[1,n]中有多少个B数？
思路：
　　这题不要用那个DFS的模板估计很快秒了。
　　状态设计为dp[位数][前缀][模13][是否含13]，前缀这一维还是有必要的（由于只有前缀1和其他不同，所以也可以用01来表示是否前缀为1）。递归出口是：出现"13"且mod=0。
```
1 #include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 #include <cstring>
 5 #include <cmath>
 6 #include <map>
 7 #include <algorithm>
 8 #include <vector>
 9 #include <iostream>
10 #define pii pair<int,int>
11 #define INF 0x7f3f3f3f
12 #define LL long long
13 #define ULL unsigned long long
14 using namespace std;
15 const double PI  = acos(-1.0);
16 const int N=11;
17 
18 int f[N][N][13][2], bit[N];   
19 //[位数][前缀][模13的余数][是否包含13]
20 
21 int dfs(int i,int pre,int mod,bool B,bool e)
22 {
23     if(i==0)    return !mod&&B;
24     if(!e && ~f[i][pre][mod][B])    return f[i][pre][mod][B];
25 
26     int ans=0, m=0;
27     int u= e? bit[i]: 9;
28     for(int d=0; d<=u; d++)
29     {
30         m=(mod*10+d)%13;
31         if( pre==1&&d==3 )
32             ans+=dfs(i-1, d, m, true, e&&d==u);
33         else
34             ans+=dfs(i-1, d, m, B, e&&d==u);
35     }
36     return e? ans: f[i][pre][mod][B]=ans;
37 }
38 
39 int cal(int n)
40 {
41     int len=0;
42     while(n)    //拆数
43     {
44         bit[++len]=n%10;
45         n/=10;
46     }
47     return dfs(len, 0, 0, false, true);
48 }
49 
50 int main()
51 {
52     //freopen("input.txt","r",stdin);
53     memset(f,-1,sizeof(f));
54     int n;
55     while( ~scanf("%d",&n) )
56         printf("%d\n",cal(n) );
57 
58     return 0;
59 }
AC代码
```

