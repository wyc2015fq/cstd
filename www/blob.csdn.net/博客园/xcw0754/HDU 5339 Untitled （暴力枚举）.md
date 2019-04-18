# HDU 5339 Untitled  （暴力枚举） - xcw0754 - 博客园
# [HDU 5339 Untitled  （暴力枚举）](https://www.cnblogs.com/xcw0754/p/4694843.html)
题意：给定一个序列，要求从这个序列中挑出k个数字，使得n%a1%a2%a3....=0（顺序随你意）。求k的最小值。
思路：排个序，从大的数开始模起，这是因为小的模完还能模大的么？
　　每个元素可以选，也可以不选，两种情况。递归穷举每个可能性，O(2n)。
```
1 //#include <bits/stdc++.h>
 2 #include <cstdio>
 3 #include <cstring>
 4 #include <map>
 5 #include <algorithm>
 6 #include <set>
 7 #include <iostream>
 8 #include <deque>
 9 #include <vector>
10 #define INF 0x7f7f7f7f
11 #define pii pair<int,int>
12 #define LL unsigned long long
13 using namespace std;
14 const int N=110;
15 int a[N];
16 int n, m;
17 int ans;
18 bool tag[N];
19 
20 
21 void DFS(int num, int cnt)
22 {
23     if(num==n)
24     {
25         int tmp=m;
26         for(int i=n-1; i>=0; i--)
27         {
28             if(tag[i])
29             {
30                 tmp%=a[i];
31             }
32         }
33         if(!tmp)
34         {
35             ans=min(cnt, ans);
36         }
37         return;
38     }
39 
40     tag[num]=0;
41     DFS(num+1, cnt );
42 
43     tag[num]=1;
44     DFS(num+1, cnt+1);
45 }
46 
47 
48 int main()
49 {
50     //freopen("input.txt", "r", stdin);
51     int   b, d, L, U, t, q;
52     cin>>t;
53     while(t--)
54     {
55         ans=INF;
56         memset(tag,0,sizeof(tag));
57 
58         scanf("%d%d", &n, &m);
59         for(int i=0; i<n; i++)    scanf("%d", &a[i]);
60 
61         sort(a, a+n);
62         DFS(0, 0);
63         if(ans==INF)
64             cout<<"-1"<<endl;
65         else
66             cout<<ans<<endl;
67     }
68     return 0;
69 }
AC代码
```

