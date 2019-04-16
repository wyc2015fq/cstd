# NYNU_省赛选拔题(8) - Vmetrio - 博客园







# [NYNU_省赛选拔题(8)](https://www.cnblogs.com/wangmengmeng/p/5271883.html)





## 题目描述


一天萌萌哒孟孟学长去博物馆参观，他想看到更多的东西。博物馆可以表示为N × M细胞的一个矩形区域。

“.”表示为路，“*”表示为墙壁，每个墙壁上面都挂有美丽的画卷。孟孟学长可以看到与他所在位置相邻的墙壁上面的画卷。他有很多时间，所以他会沿着路走，只要路是通的每一张图片都可以看到。

提示：墙壁有4个面；


## 输入


第一行输入3个数n m k（3 ≤ *n*, *m* ≤ 1000, 1 ≤ *k* ≤ *min*(*n*·*m*, 100 000)）

然后是n行m列的由“.”和“*”组成的矩阵，行列都从1开始计算；

然后k行x和y（1 ≤ *x* ≤ *n*, 1 ≤ *y* ≤ *m*），用来表示此时所在的位置；




## 输出

输出所能看到的最多的画卷；

## 样例输入
4 4 2
*.**
.*.*
***.
***.
1 2
2 1
## 样例输出
3
3


```
1 #include <cstdio>
 2 #include <cstring>
 3 #define M 1001
 4 int id, ans;
 5 int n, m;
 6 char G[M][M]; int v[M][M], r[M*M]; 
 7 int ac[4][2]={0, 1, 0, -1, -1, 0, 1, 0};
 8 void dfs(int x, int y)
 9 {
10     v[x][y]=id;                 //标记数组； 
11     for(int i=0; i<4; i++)
12     {
13         int nx=x+ac[i][0];
14         int ny=y+ac[i][1];
15         if(nx>0&&nx<=n&&ny>0&&ny<=m&&!v[nx][ny])
16         {
17             if(G[nx][ny]=='*')
18                 ans++;
19             else
20                 dfs(nx, ny);
21         }
22     }
23 }
24 int main()
25 {
26     int k;
27     while(scanf("%d%d%d", &n, &m, &k)!=EOF)
28     {
29         for(int i=1; i<=n; i++)
30             scanf("%s", G[i]+1);
31         memset(v, 0, sizeof(v));
32         id=0;
33         for(int i=1; i<=n; i++)
34         {
35             for(int j=1; j<=m; j++)
36             {
37                 if(G[i][j]=='.'&&!v[i][j])
38                 {
39                     ans=0;
40                     id++;
41                     dfs(i, j);
42                     r[id]=ans;    
43                 //    printf("%d\n", ans);                
44                 }
45             }
46         }
47         while(k--)
48         {
49             int x, y;
50             scanf("%d%d", &x, &y);
51             printf("%d\n", r[v[x][y]]);
52         }
53     }
54     return 0;
55 }
```














