# HDU 5344 MZL's xor (水题) - xcw0754 - 博客园
# [HDU 5344 MZL's xor (水题)](https://www.cnblogs.com/xcw0754/p/4703505.html)
题意：给一个序列A，设序列B的中的元素有(Ai+Aj)(1≤i,j≤n)，那么求B中所有元素的异或之和。而序列A是这样来的：A1=0,Ai=(Ai−1∗m+z) mod l。
思路：相同的元素异或结果为0，所以可以去掉，也就是剩下A中的元素ai+ai那些而已。　小心乘法会溢出
```
1 #include <bits/stdc++.h>
 2 #define INF 0x7f7f7f7f
 3 #define pii pair<int,int>
 4 #define LL long long
 5 using namespace std;
 6 const int N=5*1e5+100;
 7 
 8 
 9 int vect[N];
10 
11 int main()
12 {
13     //freopen("input.txt", "r", stdin);
14     int t, n, m, z, l;
15     cin>>t;
16     while(t--)
17     {
18         scanf("%d %d %d %d",&n, &m, &z, &l);
19         memset(vect, 0, sizeof(vect));
20         LL pre=0, big=0;
21         for(int i=1; i<n; i++)
22         {
23             vect[ (pre*m+z)%l ]++;
24             pre=(pre*m+z)%l ;
25             big=max(big, pre);
26         }
27         int ans=0;
28         for(int i=1; i<N; i++)
29         {
30             if( vect[i]%2==1)
31             {
32                 ans^=(i+i);
33             }
34         }
35         printf("%d\n",ans);
36 
37     }
38     return 0;
39 }
AC代码
```

