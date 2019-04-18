# HDU 5489 Removed Interval （LIS，变形） - xcw0754 - 博客园
# [HDU 5489 Removed Interval （LIS，变形）](https://www.cnblogs.com/xcw0754/p/4845765.html)
题意：
　　给出一个n个元素的序列，要求从中删除任一段长度为L的连续子序列，问删除后的LIS是多少？（n<=10w, L<=n ,元素可能为负）
思路：　　
　　如果会O(nlogn)求普通LIS的算法，这道题将变得很简单。
　　首先按照求LIS的思路，当扫到元素a[i]并完成操作后，a[1->i]就是一段已经处理完成的序列，假设a[i+1]->a[i+L]这一段是将要删去的，那么将分成两段：a[0]->a[i]和a[i+L+1]->a[n]。
　　假设后一段以a[i+L+1]开头，而前段以一个小于a[i+L+1]的元素结尾，那么LIS应该为bac[前段]+len[后段]。
　　如何求bac[前段]？根据O(nlogn)求普通LIS的算法，当扫到a[i]且处理完的时候，就可以在当前d[]数组中找到a[i+L+1]应处的位置，用bac[i+L+1]标记起来其位置，知位置也就知其长度了。
　　如何求后段？当前段的bac[]数组全部求完时，按照O(nlogn)求普通LIS的算法，逆向再求一次此序列的LIS，当扫到a[j]时，求一下以a[j]开头的len[j]就行了。
　　其实就是枚举一个位置pos=i，然后删除其前面一段长度为L的子序列，分别求前段以a[i]结尾，求后段以a[i]开头的LIS。但是这样为什么是正确的？假设枚举到seq[i]，后段中出现有LIS更长的，但是不是以seq[i]开头的怎么办？那么LIS更长的开头必定是一个seq[j]<seq[i]，而你是从后面往前枚举的，以seq[j]开头的早就被你考虑过了。
```
1 //#include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 #include <cstring>
 5 #include <cmath>
 6 #include <map>
 7 #include <algorithm>
 8 #include <vector>
 9 #include <iostream>
10 #define pii pair<int,int>
11 #define INF 2147483647
12 #define LL unsigned long long
13 using namespace std;
14 const double PI  = acos(-1.0);
15 const int N=100100;
16 const int mod=9901;
17 
18 int a[N], p[N], bac[N];
19 
20 int cal(int n,int L)
21 {
22     if(L==n)    return 0;
23 
24     int len, ans;
25     p[len=1]=a[1];
26     for(int i=2; i+L<=n; i++)
27     {
28         if(a[i+L]>p[len]) bac[i+L]=len;
29         else    bac[i+L]=lower_bound(p+1,p+len+1,a[i+L])-p-1;
30 
31         if(a[i]>p[len])     p[++len]=a[i];
32         else        *lower_bound(p+1,p+len+1,a[i])=a[i];
33     }
34     ans=max(len, bac[n]+1);
35     memset(p,0x7f,sizeof(p));   //注意初始化
36 
37     p[len=n]=a[n];
38     for(int i=n-1; i>L; i--)
39     {
40         int big;
41         if(a[i]<p[len])
42         {
43             p[--len]=a[i];
44             big=n+1-len + bac[i];
45         }
46         else
47         {
48             int pos=upper_bound(p+len,p+n+1,a[i])-p-1;    //找到应插入的下标
49             p[pos]=a[i];
50             big=n+1-pos + bac[i];
51         }
52         ans=max(ans, big);
53     }
54     return ans;
55 }
56 
57 int main()
58 {
59     //freopen("input.txt", "r", stdin);
60     int t, n, L, Case=0;
61     cin>>t;
62     while(t--)
63     {
64         memset(p,-1,sizeof(p));     //注意初始化
65         memset(bac,0,sizeof(bac));
66 
67         scanf("%d%d",&n,&L);
68         for(int i=1; i<=n; i++) scanf("%d",&a[i]);
69         printf("Case #%d: %d\n", ++Case, cal(n,L));
70     }
71 }
AC代码
```

