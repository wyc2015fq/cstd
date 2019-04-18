# HDU 5273 Dylans loves numbers（水题） - xcw0754 - 博客园
# [HDU 5273 Dylans loves numbers（水题）](https://www.cnblogs.com/xcw0754/p/4592538.html)
题意:给出一个
思路：一个个数，当遇到第一个1时就将flag置为1；当遇到0就将flag置为0。当遇到1时，flag=0就统计，当flag=1时就不统计。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 using namespace std;
 4 
 5 int main()
 6 {
 7     int t;
 8     LL n;
 9     cin>>t;
10     while(t--)
11     {
12         cin>>n;
13         int cnt=0,flag=0;
14         while(n)
15         {
16             if((n&1)==1)
17             {
18                 if(!flag)
19                 {
20                     flag=1;
21                     cnt++;
22                 }
23             }
24             else
25                 flag=0;
26             n>>=1;
27         }
28         printf("%d\n",cnt);
29     }
30     return 0;
31 }
AC代码
```

