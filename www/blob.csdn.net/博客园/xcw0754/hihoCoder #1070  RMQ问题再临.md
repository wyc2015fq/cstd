# hihoCoder #1070 : RMQ问题再临 - xcw0754 - 博客园
# [hihoCoder #1070 : RMQ问题再临](https://www.cnblogs.com/xcw0754/p/4506542.html)
|G++|77ms|0MB|
思路：这题用暴力是最快的，甚至比线段树还佳。 按全部都是查询的来算，是O(n*q)。
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 const int N=10010;
 4 int w[N], n, q, L, R, tmp;
 5 
 6 int main()
 7 {
 8     //freopen("input.txt", "r", stdin);
 9     cin>>n;
10     for(int i=0; i<n; i++)    scanf("%d",&w[i]);
11 
12     cin>>q;
13     for(int i=0; i<q; i++)
14     {
15         scanf("%d%d%d",&tmp,&L,&R);
16         if(tmp==0)
17         {
18             int m=999999999;
19             for(int i=L-1; i<R; i++)
20                 if(w[i]<m)    m=w[i];
21             cout<<m<<endl;
22         }
23         else    w[L-1]=R;
24     }
25     return 0;
26 }
AC代码
```

