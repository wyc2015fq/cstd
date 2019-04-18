# HDU 5265 pog loves szh II (技巧) - xcw0754 - 博客园
# [HDU 5265 pog loves szh II (技巧)](https://www.cnblogs.com/xcw0754/p/4572458.html)
题意：给一个数字序列，要求再其中找到两个数，其和再模p的结果是最大的，求此和。
思路：先将输入的元素模p，排序。结果可能有两种情况：
(1)a+b大于p：肯定由两个最大的数之和来产生。
(2)a+b小于p：设b为最大且a+b小于p，那么结果在这两个数的位置之间产生。用两个指针找出来，再与(1)中的ans比较，谁大就取谁。
若有a+b=p-1肯定是答案。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 using namespace std;
 4 const int N=100005;
 5 int n;
 6 LL a[N], p;
 7 
 8 int main()
 9 {
10    // freopen("e://input.txt", "r", stdin);
11     while(~scanf("%d %lld",&n,&p))
12     {
13         for(int i=0; i<n; i++)
14         {
15             scanf("%d",&a[i]);
16             a[i]%=p;
17         }
18         sort(a,a+n);
19 
20         int q1=0, q2=n-1;
21         LL tmp, ans=(a[q2]+a[q2-1])%p;
22 
23         while(q1<q2)
24         {
25             tmp=a[q1]+a[q2];
26             if( tmp>=p )     q2--;
27             else
28             {
29                 if(tmp>ans)    ans=tmp;
30                 q1++;
31             }
32         }
33         printf("%lld\n",ans);
34     }
35     return 0;
36 }
AC代码
```

