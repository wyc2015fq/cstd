# HDU  pog loves szh II (数的处理) - xcw0754 - 博客园
# [HDU  pog loves szh II (数的处理)](https://www.cnblogs.com/xcw0754/p/4559661.html)
题意：
　　给一个序列，找出两个数字a和b（可以相等但不可相同），要求（a+b）%p的结果最大。
思路：
　　先将所有元素模p，再排序。要找出a和b，分两种情况，a+b>p和a+b<p。第一种，肯定是序列中两个最大的数之和。第二种，用两个指针来扫，要求找到一个小于p的和。两种求最大者。时间复杂度：排序nlogn，扫一遍n，所以nlogn。
```
1 #include <iostream>
 2 #include <cmath>
 3 #include <cstdio>
 4 #include <algorithm>
 5 #include <cstring>
 6 #include <vector>
 7 #define LL long long
 8 using namespace std;
 9 const int N=100005;
10 int n;
11 LL a[N], p;//加法也可能爆int？
12 
13 int main()
14 {
15    // freopen("e://input.txt", "r", stdin);
16     while(~scanf("%d %lld",&n,&p))
17     {
18         for(int i=0; i<n; i++)
19         {
20             scanf("%d",&a[i]);
21             a[i]%=p;//先处理
22         }
23         sort(a,a+n);
24 
25         int q1=0, q2=n-1;
26         LL tmp, ans=(a[q2]+a[q2-1])%p;//如果没更新，答案就在这
27 
28         while(q1<q2)
29         {
30             tmp=a[q1]+a[q2];
31             if( tmp>=p )     q2--;//这样的结果已经不可能超过ans
32             else
33             {
34                 if(tmp>ans)    ans=tmp;//只有这种可能超
35                 q1++;
36             }
37         }
38         printf("%lld\n",ans);
39     }
40     return 0;
41 }
AC代码
```

