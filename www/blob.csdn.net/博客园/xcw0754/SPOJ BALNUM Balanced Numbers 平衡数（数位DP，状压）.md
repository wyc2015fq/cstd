# SPOJ  BALNUM  Balanced Numbers 平衡数（数位DP，状压） - xcw0754 - 博客园
# [SPOJ  BALNUM  Balanced Numbers 平衡数（数位DP，状压）](https://www.cnblogs.com/xcw0754/p/4856108.html)
题意：
　　平衡树定义为“一个整数的某个数位若是奇数，则该奇数必定出现偶数次；偶数位则必须出现奇数次”，比如 222，数位为偶数2，共出现3次，是奇数次，所以合法。给一个区间[L,R]，问有多少个平衡数？
思路：
　　这题比较好解决，只有前导零问题需要解决。如果枚举到011，那么其前导零（偶数）出现了1次而已，而此数11却是平衡数，所以不允许前导零的出现！
　　由于dfs时必定会枚举到前导零，否则位数较少的那些统计不到。状态需要3维or2维也行，3维的比较容易处理，用一维表示数位出现次数，另一维表示数位是否已经出现过了，而剩下一维自然就是位数了。乱搞一下就行了。
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
18 LL f[N][1<<10][1<<10], bit[N];
19 //[位数][状态][是否出现过]
20 
21 int isok(int s,int w)
22 {
23     if(!w)   return 0;
24     for(int i=0; i<10; i++) 
25     {
26         if( i%2==0 && (w&(1<<i)) && (s&(1<<i))==0 ) return 0;    //偶数
27         if( i%2!=0 && (w&(1<<i)) && (s&(1<<i))!=0 ) return 0;
28     }
29     return 1;
30 }
31 
32 LL dfs(int i,int s,int w,int sum,bool e)
33 {
34     if(i==0)            return isok(s,w);
35     if(!e&&~f[i][s][w]) return f[i][s][w];
36 
37     LL ans=0;
38     int u= e? bit[i]: 9;
39     for(int d=0; d<=u; d++)
40     {
41         int ww=w, ss=s;
42         if( sum+d!=0 ) ww|=1<<d,ss^=1<<d;
43         ans+=dfs(i-1, ss, ww, sum+d, e&&d==u);
44     }
45     return e? ans: f[i][s][w]=ans;
46 }
47 
48 LL cal(LL n)
49 {
50     if(n==0)    return 0;
51     int len=0, s=0;
52     while(n)    //拆数
53     {
54         bit[++len]=n%10;
55         n/=10;
56     }
57     return dfs(len,0,0,0,true);
58 }
59 
60 int main()
61 {
62     //freopen("input.txt","r",stdin);
63     memset(f, -1, sizeof(f));
64     LL L, R;int t;
65     cin>>t;
66     while( t-- )
67     {
68         cin>>L>>R;
69         cout<<cal(R)-cal(L-1)<<endl;
70     }
71     return 0;
72 }
73 
74 AC代码
AC代码
```
 　　为了省空间，以为只是标记一下偶数位就行了，奇数位若是出现偶数次，相当于没有出现（抵消）。注意要考虑0的情况，举例，如果出现数字11，抵消后状态为0，那么和出现数字0没有什么两样。然而过了样例却WA。
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
18 LL f[N][1<<10][1<<5], bit[N];
19 //[位数][状态][是否出现过]
20 
21 int isok(int s,int w)
22 {
23     for(int i=1; i<10; i+=2)    //奇数
24         if( (s&(1<<i)) )
25             return 0;
26     for(int i=0; i<5&&w; i++)    //偶数
27         if( (w&(1<<i)) && (s&(1<<2*i))==0 )
28             return 0;
29     //cout<<"123"<<endl;
30     return 1;
31 }
32 
33 LL dfs(int i,int s,int w,int sum,bool e)
34 {
35     if(i==0)            return isok(s,w);
36     if(!e&&~f[i][s][w]) return f[i][s][w];
37 
38     LL ans=0;
39     int u= e? bit[i]: 9;
40     for(int d=0; d<=u; d++)
41     {
42         if(!sum&&!d)    ans+=dfs(i-1, s, w, 0, e&&d==u);
43         else
44         {
45             int ww=w;
46             if( d%2==0 ) ww|=1<<d/2;
47             ans+=dfs(i-1, s^(1<<d), ww, sum+d, e&&d==u);
48         }
49     }
50     return e? ans: f[i][s][w]=ans;
51 }
52 
53 LL cal(LL n)
54 {
55     if(n==0)    return 0;
56     int len=0;
57     while(n)    //拆数
58     {
59         bit[++len]=n%10;
60         n/=10;
61     }
62     return dfs(len,0,0,0,true)-1;
63 }
64 
65 int main()
66 {
67     //freopen("input.txt","r",stdin);
68     memset(f, -1, sizeof(f));
69     LL L, R;int t;
70     cin>>t;
71     while( t-- )
72     {
73         cin>>L>>R;
74         cout<<cal(R)-cal(L-1)<<endl;
75     }
76     return 0;
77 }
WA代码
```
　　进行一番YY之后AC了，因为像只出现偶数个奇数位的情况，而被抵消了，相当于没有出现过，而由于记录的时候并没有记录到底是否是前导零还是被抵消的那种，两种的结果是不一样的，只需要加多一维来区分开这两种就可以了。
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
18 LL f[N][1<<10][1<<5][2], bit[N];
19 //[位数][状态][是否出现过]
20 
21 int isok(int s,int w)
22 {
23     for(int i=1; i<10; i+=2)    //奇数
24         if( (s&(1<<i)) )
25             return 0;
26     for(int i=0; i<5&&w; i++)    //偶数
27         if( (w&(1<<i)) && (s&(1<<2*i))==0 )
28             return 0;
29     return 1;
30 }
31 
32 LL dfs(int i,int s,int w,bool sum,bool e)
33 {
34     if(i==0)            return isok(s,w);
35     if(!e&&~f[i][s][w][sum]) return f[i][s][w][sum];
36 
37     LL ans=0;
38     int u= e? bit[i]: 9;
39     for(int d=0; d<=u; d++)
40     {
41         if(0==sum+d)    ans+=dfs(i-1, 0, 0, 0, e&&d==u);
42         else
43         {
44             int ww=w;
45             if( d%2==0 ) ww|=1<<d/2;
46             ans+=dfs(i-1, s^(1<<d), ww, sum+d, e&&d==u);
47         }
48     }
49     return e? ans: f[i][s][w][sum]=ans;
50 }
51 
52 LL cal(LL n)
53 {
54     if(n==0)    return 0;
55     int len=0;
56     while(n)    //拆数
57     {
58         bit[++len]=n%10;
59         n/=10;
60     }
61     return dfs(len,0,0,0,true)-1;
62 }
63 
64 int main()
65 {
66     //freopen("input.txt","r",stdin);
67     memset(f, -1, sizeof(f));
68     LL L, R;int t;
69     cin>>t;
70     while( t-- )
71     {
72         cin>>L>>R;
73         cout<<cal(R)-cal(L-1)<<endl;
74     }
75     return 0;
76 }
AC代码
```

