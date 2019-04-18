# hihoCoder #1162 : 骨牌覆盖问题·三 （矩阵快速幂，DP） - xcw0754 - 博客园
# [hihoCoder #1162 : 骨牌覆盖问题·三 （矩阵快速幂，DP）](https://www.cnblogs.com/xcw0754/p/4780744.html)
题意：有一个k*n的棋盘，要求用1*2的骨牌来铺满，有多少种方案？（k<8，n<100000001）
思路：
　　由于k是比较小，但是又不那么小，可以专门构造这样的一个矩阵M，使得只要我们有一个初始矩阵R，求得ans矩阵，然后答案就在ans中了。ans=R*Mn。
　　M的大小应该是2k*2k，所以当k稍微大一些就不合适存储这个矩阵了，而且里面大部分都是0，很浪费。由于k<8，所以M的大小为128*128是可以接受的。复杂度是O(23*k*logn)，大概是千万级别的。
```
1 #include <bits/stdc++.h>
 2 #define pii pair<int,int>
 3 #define INF 0x3f3f3f3f
 4 #define LL long long
 5 using namespace std;
 6 const int N=21000;
 7 const int mod=12357;
 8 int k, n;
 9 int M[130][130], grid[130][130], tot[130][130], cur[130][130];
10 
11 void DFS(int x,int y,int col)   //构造矩阵M。
12 {
13     if(col==k)
14     {
15         M[y][x]=1;              //表示y可以转移到x
16         return ;
17     }
18     DFS(x<<1, (y<<1)+1, col+1);                      //不放
19     DFS((x<<1)+1, y<<1, col+1);                      //放竖
20     if(col+2<=k)    DFS((x<<2)+3, (y<<2)+3, col+2);  //放横
21 }
22 
23 void mul(int A[][130],int B[][130])
24 {
25     for(int i=0; i<(1<<k); i++)
26     {
27         for(int j=0; j<(1<<k); j++)
28         {
29             int tmp=0;
30             for(int q=0; q<(1<<k); q++)
31             {
32                 tmp+=A[i][q]*B[q][j];
33                 tmp%=mod;
34             }
35             grid[i][j]=tmp;
36         }
37     }
38     memcpy(A, grid, sizeof(grid));
39 }
40 
41 
42 int cal(int t)  //注意t=n-1
43 {
44     memset(M, 0, sizeof(M));
45     memset(tot, 0, sizeof(tot) );
46     memset(cur, 0, sizeof(cur) );
47     DFS( 0, 0, 0);   //求矩阵。
48     memcpy(cur, M, sizeof(M));
49     while(t)
50     {
51         if(t&1) mul(M, cur);    //该位为1
52         mul(cur, cur);  //矩阵自乘
53         t>>=1;
54     }
55     return M[(1<<k)-1][(1<<k)-1];   //矩阵很特殊，只需要这一项。
56 }
57 
58 int main()
59 {
60     //freopen("input.txt", "r", stdin);
61     while(~scanf("%d%d", &k, &n))    printf("%d\n", cal(n-1));
62     return 0;
63 }
AC代码
```

