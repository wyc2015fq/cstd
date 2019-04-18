# UVA 11346  Probability 概率 （连续概率） - xcw0754 - 博客园
# [UVA 11346  Probability 概率 （连续概率）](https://www.cnblogs.com/xcw0754/p/4754307.html)
题意：给出a和b，表示在直角坐标系上的x=[-a,a] 和 y=[-b,b]的这样一块矩形区域。给出一个数s，问在矩形内随机选择一个点p=(x,y)，则(0.0)和p点组成的矩形面积大于s的概率是多少？
思路：
　　由于4个象限上的区域是一样的，所以只需要在第一象限上求概率即可。可以根据面积的大小来求概率。
　　s可能很小，那么p点在任意地方都是满足要求的，所以概率1。如果a*b<=s，那么p点怎么选都不可能大于s，所以概率0。
　　求出x*y<=s的部分，这部分是不满足要求的，1减去这部分面积占a*b的比例就是答案了。x*y=s是一条曲线y=s/x。画出曲线，求出在举行a*b内的面积大小，用积分来求就行了。主要在求积分问题上，知识忘光了。
```
1 #include <bits/stdc++.h>
 2 #define pii pair<int,int>
 3 #define INF 0x3f3f3f3f
 4 #define LL long long
 5 using namespace std;
 6 const int N=33;
 7 
 8 int main()
 9 {
10     freopen("input.txt", "r", stdin);
11     int t;  double a, b, s;
12     cin>>t;
13     while(t--)
14     {
15         scanf("%lf%lf%lf", &a, &b, &s);
16         if(a*b<s)//没有可能出现大于s的情况
17             puts("0.000000%");
18         else if(s<1e-6)//s太小，视为0
19             puts("100.000000%");
20         else
21         {
22             double m=a*b;
23             double ans=(m-s-s*log(m/s))/m;
24             printf( "%.6f%%\n", ans*100);
25         }
26     }
27     return 0;
28 }
AC代码
```

