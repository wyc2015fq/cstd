# ZOJ  3627  Treasure Hunt II （贪心，模拟） - xcw0754 - 博客园
# [ZOJ  3627  Treasure Hunt II （贪心，模拟）](https://www.cnblogs.com/xcw0754/p/4833963.html)
题意：有n个城市并排着，每个城市有些珠宝，有两个人站在第s个城市准备收集珠宝，两人可以各自行动，但两人之间的距离不能超过dis，而且每经过一个城市就需要消耗1天，他们仅有t天时间收集珠宝，问最多能收集多少珠宝？
思路：
　　其实就是类似一个滑动窗口在收集一个序列上的权值。首先两个人可以同时往两边散开，直到极限距离dis（这样省时），然后他们可能往左/右走，也可能往左走一会儿再往右走，也可能往右走一会儿再往左走。可以看出其实这些考虑都是对称的，那么我们主要考虑1边就行了。可以尝试枚举往左边走k天，其他t-k天往右走（利用前缀和）。注意有可能dis是个奇数的，那么在t充足的情况下，应该两边扩展到dis-1就停下呢，还是往左/右一点呢？还得靠枚举，左边试一下，右边试一下。dis也可能比t还大，那么最多也就是同时往两边扩展t天。
　　情况不算多，但是写起来还是得很小心的。还有，数据必须用longlong了。对称情况只需要反转一下序列，改变一下起点s。时间复杂度O(n)。
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
12 #define LL  long long
13 using namespace std;
14 const int N=100010;
15 const double PI  = acos(-1.0);
16 LL w[N], sum[N];
17 int n, dis, s;
18 LL get_ans(int p1,int p2,int t)
19 {
20     LL big=sum[p2]-sum[p1-1];
21     LL ans=big;
22     for(int k=0; k<p1&&t>=k; k++)   //枚举给左边k天,其他给右边
23     {
24         LL left=sum[p1-1]-sum[p1-k-1], right=0;
25         int r=t-2*k;                    //若大于0，则可继续往右走
26         if( r>0 )   right=sum[min(n,p2+r)]-sum[p2];    //右边还能获得珠宝
27         ans=max(ans, big+left+right);
28     }
29     return ans;
30 }
31 
32 
33 LL cal(int t)    //初始定点p1,p2必须拉到最长
34 {
35     int p1=max(1, s-min(dis/2,t));
36     int p2=min(n, s+min(dis/2,t));
37     LL ans=0;
38     t-=dis/2;
39     if(dis%2==1)   //奇数
40     {
41         if( t<1 || p1==1&&p2==n )   return get_ans(p1,p2, t); //t已无剩
42         if( p1>1 )  ans=max(ans, get_ans(p1-1,p2, t-1));    //左坑
43         if( p2<n )  ans=max(ans, get_ans(p1,p2+1, t-1));    //右坑
44     }
45     else    //偶数，不需要占坑
46     {
47         ans=max(ans, get_ans(p1,p2, t));
48     }
49     return ans;
50 }
51 
52 
53 int main()
54 {
55     freopen("input.txt", "r", stdin);
56     int t;LL ans;
57     while(~scanf("%d%d",&n,&s))
58     {
59         memset(sum, 0, sizeof(sum));
60         for(int i=1; i<=n; i++)
61         {
62             scanf("%d",&w[i]);
63             sum[i]+=sum[i-1]+w[i];
64         }
65         scanf("%d%d",&dis,&t);    //dis是两人的距离上限，t是所有的时间
66         ans=cal(t);
67 
68         reverse(w+1,w+n+1);     //反过来继续求。
69         memset(sum, 0, sizeof(sum));
70         for(int i=1; i<=n; i++)    sum[i]=sum[i-1]+w[i];
71         s=n+1-s;
72         ans=max(ans, cal(t));
73         cout<<ans<<endl;
74     }
75     return 0;
76 }
AC代码
```

