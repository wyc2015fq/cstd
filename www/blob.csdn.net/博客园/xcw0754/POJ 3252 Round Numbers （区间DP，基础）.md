# POJ 3252  Round Numbers  （区间DP，基础） - xcw0754 - 博客园
# [POJ 3252  Round Numbers  （区间DP，基础）](https://www.cnblogs.com/xcw0754/p/4854951.html)
题意：
　　统计区间[L,R]有多少个数，其二进制表示法中的0的个数不少于1的个数？（不允许前缀0）
思路：
　　状态表示为 [当前第几位][总位数][1的个数]，最后判断一下1的个数是否满足条件，要注意前导0的问题，可以通过枚举二进制的位数来解决。
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
11 #define INF 0x7f3f3f3f
12 #define LL long long
13 #define ULL unsigned long long
14 using namespace std;
15 const double PI  = acos(-1.0);
16 const int N=32;
17 
18 int f[N][N][N], bit[N];
19 //[当前第几位][总位数][1的个数]
20 int dfs(int i,int up,int cnt,bool e)
21 {
22     if(i==0)    return cnt*2<=up;
23     if(!e && ~f[i][up][cnt])    return f[i][up][cnt];
24     if(cnt*2>up)    return 0;
25 
26     int ans=0;
27     int d= i==up?1:0;
28     int u= e? bit[i]: 1;
29     for( ; d<=u; d++)
30     {
31         ans+=dfs(i-1,up,cnt+d,e&&d==u);
32     }
33     return e? ans: f[i][up][cnt]=ans;
34 }
35 
36 int cal(int n)
37 {
38     if(n<=1)    return 0;
39     int len=0;
40     while(n)    //拆数
41     {
42         bit[++len]=(n&1);
43         n>>=1;
44     }
45     int ans=0;
46     for(int i=2; i<len; i++)    ans+=dfs(i,i,0,false);
47     ans+=dfs(len,len,0,true);
48     return ans;
49 }
50 
51 int main()
52 {
53     //freopen("input.txt","r",stdin);
54     memset(f, -1, sizeof(f));
55     int a, b;
56     scanf("%d%d",&a,&b);
57     printf("%d\n",cal(b)-cal(a-1) );
58 
59     return 0;
60 }
AC代码
```

