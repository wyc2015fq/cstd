# HDU 4734 F(x) （数位DP，基础） - xcw0754 - 博客园
# [HDU 4734 F(x) （数位DP，基础）](https://www.cnblogs.com/xcw0754/p/4855453.html)
题意：
　　 一个非负整数的十进制位是这样的 (AnAn-1An-2 ... A2A1)，定义F(x) = An * 2n-1 + An-1 * 2n-2 + ... + A2 * 2 + A1 * 1。给出A和B，问[0,B]中有几个整数x的F(x)值<=F(A)？
思路：
　　算一下就知道F(x)值不会超过512*9，而B仅仅有8位十进制数，那么8*512*9就可以算出所有的统计了。对于每个询问，先计算F(A)的值，然后统计小于此值有几个就行了。统计的复杂度也是很低的。若是以前缀和来统计后面的个数的话，要么TLE，要么WA。
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
16 const int N=10;
17 
18 int f[N][5120], bit[N], limit;
19 //[第几位][上界]
20 
21 int dfs(int i,int sum,bool e)
22 {
23     if(i==0)            return sum>=0;
24     if(sum<0)        return 0;
25     if(!e && ~f[i][sum] )   return f[i][sum];
26 
27     int ans=0;
28     int u= e? bit[i]: 9;
29     for(int d=0; d<=u; d++)
30         ans+=dfs(i-1, sum-d*(1<<i-1), e&&d==u);
31 
32     return e? ans: f[i][sum]=ans;
33 }
34 
35 int cal(int n)
36 {
37     if(n<0)        return 0;
38     if(n==0)    return 1;
39 
40     int len=0;
41     while(n)    //拆数
42     {
43         bit[++len]=n%10;
44         n/=10;
45     }
46     return dfs(len, limit, true);
47 
48 }
49 void pre_cal(int n)
50 {
51     int len=0;
52     while(n)    //拆数
53     {
54         bit[++len]=n%10;
55         n/=10;
56     }
57     limit=0;
58     for(int i=1; i<=len; i++)    limit+=bit[i]*(1<<i-1);
59 }
60 
61 int main()
62 {
63     //freopen("input.txt","r",stdin);
64     memset(f, -1, sizeof(f));
65     int t, n, Case=0;
66     cin>>t;
67     while( t-- )
68     {
69         scanf("%d%d",&limit,&n);
70         pre_cal(limit);
71         printf("Case #%d: %d\n", ++Case, cal(n));
72     }
73     return 0;
74 }
AC代码
```

