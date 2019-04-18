# SGU 258 Almost Lucky Numbers 接近幸运数（数位DP） - xcw0754 - 博客园
# [SGU 258 Almost Lucky Numbers 接近幸运数（数位DP）](https://www.cnblogs.com/xcw0754/p/4853962.html)
题意：
　　定义一个具有2n位的正整数，其前n位之和与后n位之和相等，则为lucky数。给定一个区间，问有多少个正数可以通过修改某一位数从而变成lucky数？注意不能含前导0。
思路：
　　我的想法是记录那些非lucky数，再想办法来统计，后来发现有点行不通，无法知道其前后部之和是否相等。如果记录lucky数，然后通过统计每个位上的数来变成lucky数，这更麻烦，因为会重复统计，比如11和22是lucky数，而21可以通过修改1位来变成lucky数，被统计了两次。
　　学习了前辈的方法，也强迫一下自己别人的模板。据我对此模板的理解，第一次求解时是直接求解的，但是把所有统计过的都记录起来了，下次若还用到就直接返回就行了。复杂度是108吧。
　　这是前辈们的代码，拿来理解一下，顺便适应一下新模板。
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
17 const int M=45;
18 const int mod=1e9+7;
19 
20 int f[N][N][99][N][N], bit[N+2];
21 
22 int dfs(int i,int up,int sum,int more,int less,bool e)    //e表示:是前缀？
23 {
24     if(i==0)
25         return (sum!=M && ( sum+more>=M && sum-less<=M ) );
26 
27     if(!e && ~f[i][up][sum][more][less])   //已经计算好了(非前缀才行)
28         return f[i][up][sum][more][less];
29 
30     int ans=0;
31     int d= i==up? 1: 0;        //起始，注意首位不能为0啊
32     int u= e? bit[i]: 9;       //终止，注意末位不能超啊
33     for( ; d<=u; d++)          //是否为最后一个取决于参数e
34     {
35         int ssum=  i>(up>>1)? sum+d: sum-d; //单峰形的
36         int mmore= i>(up>>1)? max(more, 9-d): max(more, d);   //前部：可加。后部：可减
37         int lless= i>(up>>1)? max(less, i==up?d-1:d): max(less, 9-d);//前部：可减。后部：可加
38         ans+=dfs(i-1,up, ssum, mmore, lless, e&&d==u);
39     }
40     return e? ans: f[i][up][sum][more][less]=ans; //前缀的返回不同
41 }
42 
43 
44 int cal(int n)
45 {
46     if(n<10)    return 0;   //仅个位数不可能是lucky数
47     int len=0, ans=0;
48     while(n)    //拆数
49     {
50         bit[++len]=n%10;
51         n/=10;
52     }
53 
54     for(int i=2; i<=len; i+=2)      //i是数的长度
55         ans+=dfs(i,i,0+M,0,0,i==len);
56     return ans;
57 }
58 
59 int main()
60 {
61     //freopen("input.txt","r",stdin);
62     memset(f,-1,sizeof(f));
63     int L, R;
64     while( ~scanf("%d%d",&L,&R) )
65         printf("%d\n", cal(R)-cal(L-1) );
66     return 0;
67 }
AC代码
```

