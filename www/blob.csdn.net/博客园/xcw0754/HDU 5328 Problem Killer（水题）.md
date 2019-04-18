# HDU 5328 Problem Killer（水题） - xcw0754 - 博客园
# [HDU 5328 Problem Killer（水题）](https://www.cnblogs.com/xcw0754/p/4691501.html)
题意：
　　给一个序列，要找一个等差或等比的连续子序列，求其最长的长度。
思路：
　　扫两遍，判断等差或等比即可。从左往右扫，维护一个滑动窗口，考虑新加进来的数，如果满足了要求，则更新长度，否则只留最后两个数字，其他删掉，接着继续考虑下一个数字。等比也是如此，只是要注意精度的问题。
　　别人的代码：
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 const int MAX = 1e6+2;
 4 int arr[MAX];
 5 
 6 int main(void)
 7 {
 8     //freopen("in.txt", "r", stdin);
 9     int t = 0;
10     scanf("%d", &t);
11     while(t--)
12     {
13         int n = 0;
14         scanf("%d", &n);
15 
16         for(int i=1; i<=n; ++i)
17         {
18             scanf("%d", &arr[i]);
19         }
20 
21         if (n==1 || n==2)
22         {
23             printf("%d\n", n);
24             continue;
25         }
26 
27         int l, r;
28         l = 1;
29         int diff = arr[2]-arr[1];
30         int ans1 = 2;
31         for(r=3; r<=n; ++r)
32         {
33             if (arr[r] - arr[r-1] != diff)
34             {
35                 diff = arr[r]-arr[r-1];
36                 l = r-1;
37             }
38             ans1 = max(ans1, r-l+1);
39         }
40 
41         l=1;
42         double ddiff = 1.0*arr[2]/arr[1];
43         int ans2 = 2;
44         for(r=3; r<=n; ++r)
45         {
46             if ( fabs( 1.0*arr[r]/arr[r-1] - ddiff) > 0.000001)
47             {
48                 ddiff = 1.0*arr[r]/arr[r-1];
49                 l = r-1;
50             }
51             ans2 = max(ans2, r-l+1);
52         }
53         printf("%d\n", max(ans1, ans2));
54     }
55     return 0;
56 }
AC代码
```

