# HDU 5268 ZYB loves Score (简单模拟，水) - xcw0754 - 博客园
# [HDU 5268 ZYB loves Score (简单模拟，水)](https://www.cnblogs.com/xcw0754/p/4574332.html)
题意：计算Bestcoder四题的得分。
思路：直接模拟，4项分数直接计算后输出。注意不要低于百分之40的分。
```
1 //#include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 #define LL long long
 5 using namespace std;
 6 const int N=10;
 7 int a[N];
 8 int b[N];
 9 
10 int cal()
11 {
12     int ans=0;
13     ans=(1000/250*(250-a[1])-b[1]*50 >1000*0.4?  1000/250*(250-a[1])-b[1]*50:  1000*0.4 );
14     ans+=(1500/250*(250-a[2])-b[2]*50>1500*0.4?  1500/250*(250-a[2])-b[2]*50:  1500*0.4);
15     ans+=(2000/250*(250-a[3])-b[3]*50>2000*0.4?  2000/250*(250-a[3])-b[3]*50:  2000*0.4);
16     ans+=(2500/250*(250-a[4])-b[4]*50>2500*0.4?  2500/250*(250-a[4])-b[4]*50:  2500*0.4);
17     return ans;
18 }
19 int main()
20 {
21    // freopen("input.txt", "r", stdin);
22     int t, j=0;
23     cin>>t;
24     while(t--)
25     {
26         scanf("%d%d",&a[1],&b[1]);
27         scanf("%d%d",&a[2],&b[2]);
28         scanf("%d%d",&a[3],&b[3]);
29         scanf("%d%d",&a[4],&b[4]);
30         printf("Case #%d: %d\n",++j,cal());
31     }
32     return 0;
33 }
AC代码
```

