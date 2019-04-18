# HDU 5280 Senior's Array (暴力，水) - xcw0754 - 博客园
# [HDU 5280 Senior's Array (暴力，水)](https://www.cnblogs.com/xcw0754/p/4639654.html)
题意：给一个数列，再给一个数字p，要求p一定要替换掉数列中的一个元素，然后求最大连续子序列之和。
思路：1000*1000的复杂度，O(n*n) 。就是每个都试，然后求和。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 #define pii pair<int,int>
 4 #define INF 0x7f7f7f7f
 5 using namespace std;
 6 const int N=2000;
 7 int a[N];
 8 
 9 int main()
10 {
11     freopen("e://input.txt", "r", stdin);
12     int n, p, t;
13     cin>>t;
14     while(t--)
15     {
16         scanf("%d %d",&n,&p);
17         for(int i=0; i<n; i++)    scanf("%d",&a[i]);
18 
19         LL ans=p, sum=0;
20         for(int i=0; i<n; i++)
21         {
22             //cout<<a[i]<<endl;
23             int tmp=a[i];
24             a[i]=p;
25             sum=0;
26             for(int j=0; j<n; j++)
27             {
28                 sum+=a[j];
29                 if(sum>ans) ans=sum;
30                 if(sum<0)   sum=0;
31             }
32             a[i]=tmp;
33         }
34         printf("%lld\n",ans);
35     }
36     return 0;
37 }
AC代码
```

