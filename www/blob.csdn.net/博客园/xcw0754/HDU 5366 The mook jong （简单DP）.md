# HDU 5366 The mook jong （简单DP） - xcw0754 - 博客园
# [HDU 5366 The mook jong （简单DP）](https://www.cnblogs.com/xcw0754/p/4714628.html)
题意：ZJiaQ希望把木人桩摆在自家的那个由1*1的地砖铺成的1*n的院子里。由于ZJiaQ是个强迫症，所以他要把一个木人桩正好摆在一个地砖上，由于木人桩手比较长，所以两个木人桩之间地砖必须大于等于两个，现在ZJiaQ想知道在至少摆放一个木人桩的情况下，有多少种摆法。
思路：题意类似于“要求找到所有满足这样要求的二进制的数：（1）不能出现101   （2）不能出现11  （3）不能为0”。
　　问题是可以降低规模的，也就是也可以递推，假设cnt[i][0]表示第i位放0的所有可能数，cnt[i][1]表示第i位放1的所有可能数，递推式如下：
 　　（1）cnt[i][0]=cnt[i-1][0]+cnt[i-1][1];
        　　（2）cnt[i][1]=cnt[i-2][0]+1;
　　而答案就是cnt[n][0]+cnt[n][1]。
```
1 //#include <bits/stdc++.h>
 2 #include <cstdio>
 3 #include <vector>
 4 #include <deque>
 5 #include <cmath>
 6 #include <cstring>
 7 #include <iostream>
 8 #include <algorithm>
 9 #define INF 0x7f7f7f7f
10 #define pii pair<int,int>
11 #define LL long long
12 using namespace std;
13 const int N=65;
14 LL cnt[N][3];
15 LL ans[N];
16 
17 void pre_cal()
18 {
19     memset(cnt, 0, sizeof(cnt));
20     cnt[1][0]=0;
21     cnt[1][1]=1;
22 
23     cnt[2][0]=1;
24     cnt[2][1]=1;
25 
26     ans[1]=1;
27     ans[2]=2;
28 
29     for(int i=3; i<N; i++)
30     {
31         cnt[i][0]=cnt[i-1][0]+cnt[i-1][1];
32         cnt[i][1]=cnt[i-2][0]+1;
33         ans[i]=cnt[i][0]+cnt[i][1];
34     }
35 }
36 
37 int main()
38 {
39     freopen("input.txt", "r", stdin);
40     pre_cal();
41     int n;
42     while(cin>>n)
43     {
44         cout<<ans[n]<<endl;
45     }
46     return 0;
47 }
AC代码
```
另一种方法是，穷举一下所有可能，打出60个答案，直接用数组保存，速度杠杠的。

