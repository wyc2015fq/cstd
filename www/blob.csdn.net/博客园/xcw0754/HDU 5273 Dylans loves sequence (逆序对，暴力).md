# HDU 5273 Dylans loves sequence (逆序对，暴力) - xcw0754 - 博客园
# [HDU 5273 Dylans loves sequence (逆序对，暴力)](https://www.cnblogs.com/xcw0754/p/4592531.html)
题意：
　　给定一个序列，对于q个询问：(L,R)之间有几个逆序对？序列元素个数<=1000，q<=100000。只有1个测试例子。
思路：
　　[L,R]的逆序对数量可以这么算，假设L<=K<R，将区间拆成两部分，那么[L,k]中的逆序对要算上， (k,R]中的逆序对也要算上，还有一些逆序对假设为(l,r)，l在左部分，r在右部分。则应该是3部分来构成，设3部分为A,B,C，那么ans=A+B+C 。
　　而如果将k移到最右边，比如k=R-1，那么区间拆成[L,k]和(K,R]，而(K,R]其实就只有R一个元素，并不存在什么逆序对，所以B=0，那么ans=A+C，仅仅由两部分构成，就简单了。
　　对于序列a，假设m[j][i]表示从j到i-1之间有几个大于a[i]（如果知道了j到i-1之间有多少逆序对，那么再加上m[j][i]就是j到i之间的逆序对数了）。复杂度1k*1k。
　　接着求答案了，假设ans[L][R]表示从L到R之间有多少个逆序对，那么ans[j][i]=ans[j][i-1]+m[j][i]。而我们知道ans[j][j]肯定为0，那么就能计算出其他的答案了。复杂度1k*1k。
（也可以对于每个询问才来求ans[L][R]，复杂度10w*1k还是可以过的）
```
1 #include <iostream>
 2 #include <cmath>
 3 #include <cstdio>
 4 #include <cstring>
 5 #include <map>
 6 #include <vector>
 7 #include <set>
 8 #include <map>
 9 #define LL long long
10 using namespace std;
11 const int N=1005;
12 
13 int a[N];
14 int m[N][N];
15 int ans[N][N];
16 int n, q;
17 
18 void cal()
19 {
20     memset(m,0,sizeof(m));
21     memset(ans,0,sizeof(ans));
22 
23     for(int i=1; i<=n; i++)
24     {
25         for(int j=i-1; j>0; j--)
26             if(a[j]>a[i])    m[j][i]=m[j+1][i]+1;   //计算在j~i-1之间比a[i]大的有几个
27             else    m[j][i]=m[j+1][i];
28     }
29 
30     for(int i=1 ;i<=n; i++)
31         for(int j=i+1; j<=n; j++)
32             ans[i][j]=ans[i][j-1]+m[i][j];
33 
34 }
35 
36 
37 int main()
38 {
39     //freopen("input.txt", "r", stdin);
40     int aa,bb;
41     while(~scanf("%d%d",&n,&q))
42     {
43         for(int j=1; j<=n; j++)
44             scanf("%d",&a[j]);
45 
46         cal();
47         while(q--)
48         {
49             scanf("%d%d",&aa,&bb);
50             printf("%d\n",ans[aa][bb]);
51         }
52     }
53 
54     return 0;
55 }
AC代码
```

