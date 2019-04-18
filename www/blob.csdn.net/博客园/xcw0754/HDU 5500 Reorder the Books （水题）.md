# HDU 5500 Reorder the Books （水题） - xcw0754 - 博客园
# [HDU 5500 Reorder the Books （水题）](https://www.cnblogs.com/xcw0754/p/4868613.html)
题意：
　　有n本书，编号为1~n，现在书的顺序乱了，要求恢复成有序的样子，每次只能抽出其中一本并插到最前面，问最少需要多少抽几次？
思路：
　　如果pos[i]放的不是书i的话，则书i的右边所有的书都必须抽出来。抽出来的书必定可以回到它最终的位置上，即每本书最多抽出来1次，可以想象抽出来后直接扔掉，在剩下的书中找到一个有序，且最后一个元素为n，且等差为1的序列{...n-2,n-1,n}。
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
19 const int N=30;
20 
21 int n, a[N];
22 int cal()
23 {
24     for(int i=n; i>0; i--)
25         if(a[i]==n)    n--;
26     return n;
27 }
28 
29 int main()
30 {
31     //freopen("input.txt", "r", stdin);
32     int t;cin>>t;
33     while(t--)
34     {
35         scanf("%d",&n);
36         for(int i=1; i<=n; i++)
37             scanf("%d", &a[i]);
38         cout<<cal()<<endl;
39     }
40     return 0;
41 }
AC代码
```

