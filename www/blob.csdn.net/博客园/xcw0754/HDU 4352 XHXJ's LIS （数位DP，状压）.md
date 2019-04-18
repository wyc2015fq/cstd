# HDU 4352 XHXJ's LIS （数位DP，状压） - xcw0754 - 博客园
# [HDU 4352 XHXJ's LIS （数位DP，状压）](https://www.cnblogs.com/xcw0754/p/4855767.html)
题意：
　　前面3/4的英文都是废话。将一个正整数看成字符串，给定一个k，问区间[L,R]中严格的LIS=k的数有多少个？
思路：
　　实在没有想到字符0~9最多才10种，况且也符合O(nlogn)求LIS的特点，所以用状态压缩可以解决。
　　看到状态压缩的字眼基本就会做了，增加一维来保存当前LIS的状态。由于求LIS时的辅助数组d[i]表示长度为i的LIS最后一个元素，d数组是严格递增的，所以好好利用d数组的特性来设计状态压缩才是关键。压缩的状态0101可以表示：仅有0和2在数组d中，即d[1]=0，d[2]=2的意思。状态的设计方法有多种。
 　　此题在考虑前导零问题时，逐个枚举位数，可以这样做是因为如果位数超过了1，则最后一个数位若为0是不会对结果构成影响的，因为最后的0都不会被考虑在LIS中。而对于那些个位数为0（或者说后缀0）会对结果产生影响的，最好是不要这样用了（例如spoj Balanced Numbers就不可以）。
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
16 const int N=20;
17 
18 LL f[N][1<<11][11], bit[N];
19 //[位数][状态][k]
20 
21 int insert(int s,bool flag,int pos) //修改状态
22 {
23     for(int i=pos; i<=9&&flag; i++)   //找到第一位>=pos的，抹去
24         if(s&(1<<i))
25         {
26             s^=(1<<i);
27             break;
28         }
29     return s|(1<<pos);
30 }
31 
32 int gethigh(int s)  //获取LIS最大元素，即d[len]。
33 {
34     for(int i=9; i>=0; i--)    if(s&(1<<i))    return i;
35     return -1;
36 }
37 
38 LL dfs(int i,int up,int s,int k,bool e)
39 {
40     //up为总位数，s为状态，k为仍需一段len=k的串来组成LIS=K的
41     if(i==0)               return k==0;
42     if(i<k)                return 0;    //剩下的位数已不够k个，不能组成LIS=k
43     if(!e && ~f[i][s][k] ) return f[i][s][k];
44 
45     LL ans=0;
46     int d= i==up? 1: 0;     //为了解决前缀0的情况,起始不为0
47     int u= e? bit[i]: 9;
48 
49     int h=gethigh(s);       //LIS的最大元素
50     for( ; d<=u; d++)
51     {
52         if(d>h) ans+=dfs(i-1,up,insert(s,0,d),k-1,e&&d==u);
53         else    ans+=dfs(i-1,up,insert(s,1,d),k,e&&d==u);   //LIS长度不变
54     }
55     return e? ans: f[i][s][k]=ans;
56 }
57 
58 LL cal(LL n,int k)
59 {
60     if(n==0)    return 0;
61     int len=0;
62     while(n)    //拆数
63     {
64         bit[++len]=n%10;
65         n/=10;
66     }
67     LL ans=0;
68     for(int i=k; i<len; i++)    //为了解决前导0问题，逐个枚举
69         ans+=dfs(i,i,0,k,false);
70     if(len>=k)
71         ans+=dfs(len,len,0,k,true);
72     return ans;
73 }
74 
75 int main()
76 {
77     //freopen("input.txt","r",stdin);
78     memset(f, -1, sizeof(f));
79     LL L, R;int t, K, Case=0;
80     cin>>t;
81     while( t-- )
82     {
83         scanf("%lld%lld%d",&L,&R,&K);
84         printf("Case #%d: %lld\n", ++Case, cal(R,K)-cal(L-1,K));
85     }
86     return 0;
87 }
AC代码
```

