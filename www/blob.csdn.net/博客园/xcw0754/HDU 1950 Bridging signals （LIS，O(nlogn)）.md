# HDU 1950 Bridging signals （LIS，O(nlogn)） - xcw0754 - 博客园
# [HDU 1950 Bridging signals （LIS，O(nlogn)）](https://www.cnblogs.com/xcw0754/p/4842731.html)
题意：
　　给一个数字序列，要求找到LIS，输出其长度。
思路：
　　扫一遍+二分，复杂度O(nlogn)，空间复杂度O(n)。
　　具体方法：增加一个数组，用d[i]表示长度为 i 的递增子序列的最后一个元素，且该元素总是保持当前最小。初始化d[1]=A[i]，当前LIS的长度len=1。从 2 to n，若A[i]>d[len]，则d[++len]=A[i]，否则，在数组d中找到A[i]应该插入的位置，代替掉那个第一个比它大的数字，比如d[k]<A[i]<=d[k+1]，直接将A[i]代替掉d[k+1]。完成后len就是LIS的长度了。
```
1 #include <bits/stdc++.h>
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
15 const int N=40100;
16 const int mod=1e9+7;
17 int a[N], d[N];
18 int* lower_( int *s,int *e,int val ) //二分找值，返回下标
19 {
20     int L=0, R=e-s-1, mid;
21     while(L<R)
22     {
23         mid=R-(R-L+1)/2;            //保证至少减少1
24         if( s[mid]<val )    L=mid+1;//至少增加1
25         else                R=mid;
26     }
27     return &s[R];
28 }
29 
30 int main()
31 {
32     freopen("input.txt", "r", stdin);
33     int t, n, len;
34     cin>>t;
35     while(t--)
36     {
37         scanf("%d",&n);
38         for(int i=1; i<=n; i++) scanf("%d",&a[i]);
39         len=1;
40         d[len]=a[1];
41 
42         for( int i=2; i<=n; i++ )
43         {
44             if( a[i]>d[len] )    d[++len]=a[i];
45             else                 *lower_(d+1,d+len+1,a[i])=a[i];
46             //else               *lower_bound(d+1,d+len+1,a[i])=a[i]; 上一行代码可换成此行
47         }
48         printf("%d\n",len);
49     }
50     return 0;
51 }
AC代码
```

