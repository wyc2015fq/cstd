# hdu 3232 Crossing Rivers   过河（数学期望） - xcw0754 - 博客园
# [hdu 3232 Crossing Rivers   过河（数学期望）](https://www.cnblogs.com/xcw0754/p/4752784.html)
题意：你在点A，目的地是点B，A和B的距离为D。中间隔了好多条河（所有河不会重叠），每条河有3个参数（P，L，V），其中P表示距离A点的长度，L表示河的长度，V表示河里的船的速度。假设每条河中仅有1条自动船，以速度V左右匀速运动，碰到河的端点就反向走。那么从A点到B点的期望是多沙？
　　注意：A点到B点都是在x轴上，而任意河只是[A,B]中的一个子区间而已，只是所有子区间都不会重叠。
思路：刚开始以为会有河可能是重叠的，想了N久。
　　由于船的任意时刻的位置都是随机的，那就是概率相等的，那么当走到该河的左端点时，船的位置也是任意的，最快渡河就是刚好船在左端点，渡河时间L/V，最慢就是船刚好从左端点走了，那么它会走到右端点，再回来，时间是2L/V，再车我过去+L/V，总共是3L/V。那么平均下来我渡一条河的时间是2L/V啦。有n条河，那么将渡每条河的期望之和为： ∑ 2L/V 。再加上在陆地上的时间，就是D减去所有河的长度。
```
1 #include <bits/stdc++.h>
 2 #define pii pair<int,int>
 3 #define INF 0x3f3f3f3f
 4 #define LL long long
 5 using namespace std;
 6 const int N=10086;
 7 
 8 int main()
 9 {
10     freopen("input.txt", "r", stdin);
11     int Case=0, n, d, p, L, v ;
12 
13     while(scanf("%d %d", &n, &d), n+d)
14     {
15         double ans=0;
16         for(int i=0; i<n; i++)  //每条河
17         {
18             scanf("%d%d%d",&p, &L, &v );    //跟p完全没有干系
19             ans+=(double)2*L/v; //每条河的期望。
20             d-=L;
21         }
22 
23         printf("Case %d: %.3f\n\n", ++Case, ans+d);
24     }
25     return 0;
26 }
AC代码
```

