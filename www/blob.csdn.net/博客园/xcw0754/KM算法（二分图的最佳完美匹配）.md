# KM算法（二分图的最佳完美匹配） - xcw0754 - 博客园
# [KM算法（二分图的最佳完美匹配）](https://www.cnblogs.com/xcw0754/p/4719177.html)
　　KM算法大概过程：
　　（1）初始化Lx数组为该boy的一条权值最大的出边。初始化Ly数组为 0。
　　（2）对于每个boy，用DFS为其找到一个girl对象，顺路记录下S和T集，并更新每个girl的slack值。若不能为其找到对象，则转3。
　　（3）找出非T集合的girl的最小slack值为d，更新S集中的boy和T集中的girl，并且顺路更新非T集中的slack值。对于那个失败的boy继续第2步。
　　简括之，就是在保持当前权和最高的情况下，尽量为每个boy找到权更大的边。找的过程就是DFS过程，标记出S和T集是为了保证权和最大，因为只要帮S中任意一个boy另找一个女对象，为这个boy的此次脱单之路告终。
　　DFS的要完成的任务：
　　（1）标记S和T集。
　　（2）更新每个girl的slack值为最小。
　　模板还是必须的，带满了注释，改自kuangbin的模板。
```
1 /* KM算法：复杂度O(nx*nx*ny)
 2 * 完全二分图求最大权匹配(必须为所有boy找到对象，且boy数量必须<=girl数量)
 3 * 若求最小权匹配，可将权值取相反数，结果取相反数
 4 * 点的编号从1开始。
 5 * 以男女模型出现比较直观。
 6 */
 7 int  nx, ny;                  //两边的点数，x为男，y为女。
 8 int  g[N][N];                 //二分图描述，g[x][y]表示边权。
 9 int  girl[N], Lx[N], Ly[N];   //girl[i]记录i的匹配成功对象，男女的顶标
10 int  slack[N];      //为了优化用的，连接到对应girl的松弛值。
11 bool S[N], T[N];    //匈牙利树的节点集合，S为男，T为女。
12 
13 bool DFS(int x) // x一定是boy
14 {
15     S[x]=true;
16     for(int i=1; i<=ny; i++) //枚举girl
17     {
18         if(T[i]) continue;
19         int tmp=Lx[x]+Ly[i]-g[x][i];
20         if( tmp==0 )
21         {
22             T[i]=true;
23             //为第i个girl的男对象另找女对象
24             if(girl[i]==-1 || DFS(girl[i]))
25             {
26                 girl[i]=x;      //记录匹配的boy
27                 return true;
28             }
29         }
30         else if(slack[i]>tmp)   //顺便更新下slack
31             slack[i]=tmp;
32     }
33     return false;
34 }
35 
36 int KM()
37 {
38     memset(girl, -1, sizeof(girl));
39     memset(Ly, 0, sizeof(Ly));
40     for(int i=1; i<=nx; i++) //初始化两个L数组分别为-INF和0
41     {
42         Lx[i] = -INF;
43         for(int j=1; j<=ny; j++)
44             if(g[i][j]>Lx[i])    Lx[i]=g[i][j];
45     }
46     for(int j=1; j<=nx; j++)     //枚举boy
47     {
48         for(int i=1; i<=ny; i++) //初始slack为无穷。slack只需要记录girl的。
49             slack[i]=INF;
50         while(true)     //无限循环，直到帮boy[j]找到对象
51         {
52             memset(S, 0, sizeof(S));
53             memset(T, 0, sizeof(T));
54             if( DFS(j) )  break;    //直接就找到对象了，搞定。
55             int d=INF;
56             for(int i=1; i<=ny; i++)    //根据不在匈牙利树上的girl的slack值找到最小值d
57                 if(!T[i] && d>slack[i])
58                     d=slack[i];
59             for(int i=1; i<=nx; i++)     //所有匈牙利树上的boy更新lx值
60                 if(S[i])    Lx[i]-=d;
61             for(int i=1; i<=ny; i++)     //树上的girl加d，不在树上的girl的slack减d。
62             {
63                 if(T[i])     Ly[i]+=d;   //这是为了让等式仍然成立
64                 else         slack[i]-=d;
65             }
66         }
67     }
68     int ans=0;
69     for(int i=1; i<=ny; i++) //累计匹配边的权和
70         if(girl[i]>0) ans+=g[girl[i]][i];
71     return ans;
72 }
KM算法
```

