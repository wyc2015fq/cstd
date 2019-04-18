# POJ 3017  Cut the Sequence （单调队列优化DP） - xcw0754 - 博客园
# [POJ 3017  Cut the Sequence （单调队列优化DP）](https://www.cnblogs.com/xcw0754/p/4864935.html)
题意：
　　给定含有n个元素的数列a，要求将其划分为若干个连续子序列，使得每个序列的元素之和小于等于m，问最小化所有序列中的最大元素之和为多少？（n<=105。例：n=8, m=17，8个数分别为2 2 2 | 8 1 8 |1 2，答案为12。）
思路：
　　想明白一个队列+一个set就能完美解决这个问题？
　　首先DP的转移式子是：dp[i]=min( dp[j] +max[j+1, i]  )，且sum[i]-sum[j]<=m，j为枚举的断开处。暴力寻找一个合适的j的复杂度为O(n2)。那么问题就在于如何寻找这个合适的j。
　　先假设j的范围是(low, i]，而max是随着j的增大单调不减的，则max[k,i]>=max[p,i]且k<p是肯定的。那么如果出现max[k,i]=max[p,i]且k<p的话，j=k明显更佳，因为dp[k]<=dp[p]是肯定的！那么如果出现max[k,i]>max[p,i]且k<p的话，取哪个就难说了，也是因为dp[k]<=dp[p] 。
　　根据转移式子知道，j肯定是不跟i同组的，这是根据dp[j]的定义来决定的。那么j有可能的取值为low，或者为k（a[k]>a[i]），那么单调队列就形成了，队列中保存下标，表示所有a[k]>a[i]且low<=k<=i。
　　但是这队列有什么用？值val可能等于队列中任一元素u，而val=dp[u]+max[u+1,i]是没有什么规律的。直接将所有val装进set中，最小的set.begin()就是我们要找的答案了。如果维护单调队列时需要删除怎么办？直接将算出来的val在set中删除。
　　注意：如果队列为空，即a[i]是a[low,i]中的最大值，那么最多只能取j=low。如果low一旦改变了，队头元素也有可能改变喔~因为队头算出来的val可能是根据更小的low算出来的。由于多个val相同的情况是存在的，所以用multiset。
```
1 //#include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 #include <cstring>
 5 #include <cmath>
 6 #include <set>
 7 #include <deque>
 8 #include <map>
 9 #include <algorithm>
10 #include <vector>
11 #include <iostream>
12 #define pii pair<int,int>
13 #define back que[rear-1]
14 #define INF 0x7f7f7f7f
15 #define LL long long
16 #define ULL unsigned long long
17 using namespace std;
18 const double PI  = acos(-1.0);
19 const int N=100100;
20 LL sum[N], has[N], dp[N];
21 int a[N], que[N], top, rear, low;
22 multiset<LL> sett;
23 LL cal(int n,LL m)
24 {
25     for(int i=1; i<=n; i++)
26     {
27         if(a[i]>m)  return -1;
28         while( sum[i]-sum[low]>m )  low++;
29 
30         //过期的只可能在top处
31         while( top<rear && que[top]<=low )
32             sett.erase( has[top++] );
33 
34         if(top<rear)
35         {
36              //队头的val有可能会改变，因为下限low提高了
37             sett.erase( has[top] );
38             has[top]=dp[low]+a[ que[top] ];
39             sett.insert( has[top] );
40         }
41         //插入队列先
42         while(top<rear && a[i]>a[ que[rear-1] ] )
43             sett.erase( has[--rear] );
44 
45         que[rear]=i;    //只记下标
46         LL val=0;
47         if(top^rear)    val=dp[ que[rear-1] ]+a[i];
48         else            val=dp[low]+a[i]; //top=rear时，a[i]就是最大的
49         sett.insert( val );
50         has[rear++]=val;
51         dp[i]=*sett.begin();    //取最小
52     }
53     return dp[n];
54 }
55 
56 int main()
57 {
58     //freopen("input.txt","r",stdin);
59     int n;LL m;
60     scanf("%d%lld", &n, &m);
61     for(int i=1; i<=n; i++) //机器
62     {
63         scanf("%d",&a[i]);
64         sum[i]=sum[i-1]+a[i];
65     }
66     printf("%lld\n", cal(n,m));
67     return 0;
68 }
AC代码
```

