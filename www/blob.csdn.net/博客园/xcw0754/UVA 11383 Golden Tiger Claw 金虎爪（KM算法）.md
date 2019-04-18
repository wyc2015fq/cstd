# UVA 11383 Golden Tiger Claw 金虎爪（KM算法） - xcw0754 - 博客园
# [UVA 11383 Golden Tiger Claw 金虎爪（KM算法）](https://www.cnblogs.com/xcw0754/p/4723861.html)
题意：
　　给一个n*n的矩阵，每个格子中有正整数w[i][j]，试为每行和每列分别确定一个数字row[i]和col[i]，使得任意格子w[i][j]<=row[i]+col[j]恒成立。先输row，再输出col，再输出全部总和（总和应尽量小）。
思路：
　　本题与匹配无关，但可以用KM算法解决。
　　KM算法中的顶标就是保持了Lx[i]+ly[j]>=g[i][j]再求最大权和匹配的，但这个最大权和并没有关系。我们可以将row[i]看成一个男的，col[i]看成一个女的，这样男女的总数就相等。
　　一般来说，Lx[i]或Ly[i]仅需要取该行/列中最大的那个数即可保证满足要求，但是这样太大了，可以通过调整来使得总和更小。而KM算法的过程就是一个调整的过程，每一对匹配的男女的那条边的权值就会满足等号 w[i][j]=row[i]+col[j]，至少需要一个来满足等号，这样才能保证row[i]+col[j]是达到最小的，即从j列看，col[j]满足条件且最小，从i行看，row[i]满足条件且最小。这刚好与KM算法求最大权和一样。
```
1 #include <bits/stdc++.h>
  2 #define LL long LONG_LONG_MAX
  3 #define INF 0x7f7f7f7f
  4 #define LL long long
  5 using namespace std;
  6 const int N=510;
  7 
  8 int grid[N][N], girl[N];
  9 int Lx[N], Ly[N], slack[N];
 10 bool S[N], T[N];
 11 int n;
 12 
 13 bool DFS(int x)
 14 {
 15     S[x]=true;
 16     for(int i=1; i<=n; i++)
 17     {
 18         if(T[i])    continue;
 19         int tmp=Lx[x]+Ly[i]-grid[x][i];
 20         if(tmp==0)
 21         {
 22             T[i]=true;
 23             if(girl[i]==0 || DFS(girl[i]))
 24             {
 25                 girl[i]=x;
 26                 return true;
 27             }
 28         }
 29         else if(tmp<slack[i])
 30             slack[i]=tmp;
 31     }
 32     return false;
 33 }
 34 
 35 
 36 
 37 int KM()
 38 {
 39     memset(girl, 0, sizeof(girl));
 40     memset(Lx, 0, sizeof(Lx));
 41     memset(Ly, 0, sizeof(Ly));
 42     for(int i=1; i<=n; i++)
 43         for(int j=1; j<=n; j++)
 44             Lx[i]=max(Lx[i], grid[i][j]);
 45 
 46     for(int i=1; i<=n; i++) //对于每个树
 47     {
 48         for(int j=1; j<=n; j++) slack[j]=INF;
 49         while(1)
 50         {
 51             memset(S, 0, sizeof(S));
 52             memset(T, 0, sizeof(T));
 53             if( DFS(i) )    break;      //找到匹配的蚂蚁
 54 
 55 
 56             int d=INF;
 57             for(int j=1; j<=n; j++) //找最小D
 58             {
 59                 if(!T[j] && d>slack[j])
 60                     d=slack[j];
 61             }
 62 
 63             for(int j=1; j<=n; j++) //更新树
 64             {
 65                 if(S[j])
 66                     Lx[j]-=d;
 67             }
 68 
 69             for(int j=1; j<=n; j++) //更新蚂蚁
 70             {
 71                 if(T[j])    Ly[j]+=d;
 72                 else        slack[j]-=d;
 73             }
 74         }
 75     }
 76     int sum=0;
 77     for(int i=1; i<=n; i++) sum+=Lx[i]+Ly[i];
 78     return sum;
 79 }
 80 
 81 
 82 
 83 
 84 int main()
 85 {
 86     freopen("input.txt", "r", stdin);
 87     while(~scanf("%d",&n))
 88     {
 89         memset(grid, 0, sizeof(grid));
 90         for(int i=1; i<=n; i++)
 91             for(int j=1; j<=n; j++)
 92                 scanf("%d",&grid[i][j]);
 93 
 94         int ans=KM();
 95         printf("%d", Lx[1]);//值得注意的输出格式。
 96         for(int i=2; i<=n; i++) printf(" %d", Lx[i]);
 97         printf("\n");
 98         printf("%d",Ly[1]);
 99         for(int i=2; i<=n; i++) printf(" %d", Ly[i]);
100         printf("\n");
101         printf("%d\n", ans);
102     }
103     return 0;
104 }
AC代码
```

