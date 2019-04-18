# HDU  3709  Balanced Number  （数位DP） - xcw0754 - 博客园
# [HDU  3709  Balanced Number  （数位DP）](https://www.cnblogs.com/xcw0754/p/4855122.html)
题意：
　　找出区间内平衡数的个数，所谓的平衡数，就是以这个数字的某一位为支点，另外两边的数字大小乘以力矩之和相等，即为平衡数。
思路：
　　一开始以为需要枚举位数，枚举前缀和，枚举后缀和，一旦枚举起来就会MLE。
　　其实只需要3维 [第几位][和][轴位置]，对于轴的位置是需要枚举的，每个位都是有可能的，比如900和7都是一个平衡数。注意这道题的区间下限可能为0，而0也是平衡数，这在拆十进制的时候len=0的，最好将0特处理。
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
16 const int N=19;
17 
18 LL f[N][325][N], bit[N];   
19 //[第几位][和][轴]
20 
21 LL dfs(int i,int sum,int mid,bool e)
22 {
23     if(i==0)    return sum==0;
24     if(sum<0 || sum>=325)    return 0;    
25     if(!e && ~f[i][sum][mid])    return f[i][sum][mid];
26     
27     LL ans=0;
28     int u= e? bit[i]: 9;
29     for(int d=0; d<=u; d++)
30     {
31         if(sum==0 && i==mid && d==0)    continue;    //首位是mid，必须不为0
32         ans+=dfs(i-1, sum+(i-mid)*d, mid, e&&d==u);    //注意不能为负
33     }
34     return e? ans: f[i][sum][mid]=ans;
35 }
36 
37 LL cal(LL n)
38 {
39     if(n<0)        return 0;
40     int len=0;
41     while(n)    //拆数
42     {
43         bit[++len]=n%10;
44         n/=10;
45     }
46     LL ans=1;    //dfs是没有统计0的，因为len=0是不会执行dfs的
47     for(int i=1; i<=len; i++)
48         ans+=dfs(len, 0, i, true);
49     return ans;
50 }
51 
52 int main()
53 {
54     //freopen("input.txt","r",stdin);
55     memset(f,-1,sizeof(f));
56     LL L,R;int t;cin>>t;
57     while( t-- )
58     {
59         cin>>L>>R;
60         cout<<cal(R)-cal(L-1)<<endl;
61     }
62     return 0;
63 }
AC代码
```

