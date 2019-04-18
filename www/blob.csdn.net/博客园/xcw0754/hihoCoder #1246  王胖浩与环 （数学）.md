# hihoCoder #1246 : 王胖浩与环 （数学） - xcw0754 - 博客园
# [hihoCoder #1246 : 王胖浩与环 （数学）](https://www.cnblogs.com/xcw0754/p/4930073.html)
题意：
　　有一个环形序列，可以将其切成连续的k段子序列，那么gcd( 每段子序列的和 )就是优美程度。输出n个整数，表示当k=[1, n] 时的最大优美程度。 
思路：
　　观察一下，当切成1段的时候，gcd就是sum[整个序列]，为最大。考虑切成2段，那么最好就是能让这个环切成2段和为sum[整个序列]/2的子序列了。考虑切成3段，那么最好就是能让这个环切成3段和为sum[整个序列]/3的子序列了。继续下去，这不就是求sum[整个序列]的约数吗？
　　假设约数有k个，从大到小分别为factor[1～R]。那么其中有些因数是可能组不成的，得去掉那些组不成的。假设sum[整个序列]最多能切成cnt段和为factor[t]的连续子序列，那么段数i<=cnt的，答案都是factor[t]了，取最大即可。
　　问题在于如何求出切成长为factor[t]的最多段数cnt[t]？当前缀和pre%factor=r出現了m次時应该是这样的： ..|xxxx|xxxx|......|xxx|xxxx|.. （共有m个切口|），观察到除了首尾之外，其他每段都是d的倍数，且首尾之和也是d的倍数（因为sum[整个序列]=k*d）。那么对于factor[t]，只需要枚举r来求出最大的m即可。
```
1 #include<bits/stdc++.h>
 2 #define pii pair<int,int>
 3 #define INF 0x3f3f3f3f
 4 #define LL long long
 5 using namespace std;
 6 const int N=2010;
 7 vector<LL> factor, cnt;
 8 LL a[N];
 9 map<LL,LL> mapp;
10 int main()
11 {
12     freopen("input.txt","r",stdin);
13     int n;
14     while(~scanf("%d",&n))
15     {
16         for(int i=1; i<=n; i++)
17         {
18             scanf("%lld",&a[i]);
19             a[i]+=a[i-1];
20         }
21         for(LL i=1; i*i<=a[n]; i++)
22         {
23             if(a[n]%i==0)        
24             {        
25                 factor.push_back(a[n]/i);
26                 factor.push_back(i);//多一个也不影响结果
27             }
28         }
29         sort(factor.begin(),factor.end());
30         deque<LL> ans;
31         for(int k=factor.size()-1,i=1; k>=0; k--)
32         {
33             LL big=0, c=factor[k];
34             mapp.clear();
35             for(int j=1; j<=n; j++)    
36                 big=max(big,++mapp[a[j]%c]);
37             while(i<=n&&big>=i)
38             {
39                 ans.push_back(c);
40                 i++;
41             }
42         }
43         while(!ans.empty())    
44         {
45             printf("%lld\n",ans.front());
46             ans.pop_front();
47         }
48     }
49     return 0;
50 }
AC代码
```

