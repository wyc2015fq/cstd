# HDU 1711 Number Sequence (数字KMP，变形) - xcw0754 - 博客园
# [HDU 1711 Number Sequence (数字KMP，变形)](https://www.cnblogs.com/xcw0754/p/4550384.html)
题意：
　　在一个序列中找到一个连续的子序列，返回其开始位置。
思路：
　　每个数字当成1个字符，长的序列是原串，短的序列是模式串，求next数组后进行匹配。
```
1 #include <iostream>
 2 #include <cmath>
 3 #include <cstdio>
 4 using namespace std;
 5 const int N=1000010;
 6 int a[N];
 7 int nextt[10010];
 8 int b[10010];
 9 int n, m;
10 
11 void get_next()
12 {
13     nextt[0]=-1;
14     int i=0, j=-1;
15     while(i<m)
16     {
17         if(j==-1 || b[i]==b[j])
18         {
19             nextt[i+1]=++j;
20             ++i;
21         }
22         else
23             j=nextt[j];
24     }
25 }
26 
27 int cal()
28 {
29 
30     get_next();
31     /*
32     for(int i=0; i<=m; i++)
33         cout<<next[i]<<" ";
34     cout<<endl;*/
35     if(m>n) return -1;
36     int i=0, j=0, cnt=0;
37     while(i<n)
38     {
39         if(j==-1||a[i]==b[j])
40             i++,j++;
41         else
42             j=nextt[j];
43         if(j==m) return i-m+1;
44     }
45     return -1;
46 }
47 
48 
49 int main()
50 {
51     //freopen("input.txt", "r", stdin);
52     int t;
53     cin>>t;
54     while(t--)
55     {
56         scanf("%d%d",&n,&m);
57         for(int i=0; i<n; i++)
58             scanf("%d",&a[i]);
59         for(int i=0; i<m; i++)
60             scanf("%d",&b[i]);
61         printf("%d\n",cal());
62     }
63 
64     return 0;
65 }
AC代码
```

