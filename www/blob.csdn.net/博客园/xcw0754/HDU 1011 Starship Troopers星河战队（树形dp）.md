# HDU 1011 Starship Troopers星河战队（树形dp） - xcw0754 - 博客园
# [HDU 1011 Starship Troopers星河战队（树形dp）](https://www.cnblogs.com/xcw0754/p/4241459.html)
## 题意
　　有n个洞穴编号为1～n，洞穴间有通道，形成了一个n-1条边的树, 洞穴的入口即根节点是1。
　　每个洞穴有x只bugs，并有价值y的金子，全部消灭完一个洞穴的虫子，就可以获得这个洞穴的y个金子.
　　现在要派m个战士去找金子，从入口进入。每次只有消灭完当前洞穴的所有虫子，才可以选择进入下一个洞穴。
　　一个战士可以消灭20只虫子，如果要杀死x只虫子，那么要x/20向上取整即(x+19)/20个战士。
　　如果要获得某个洞穴的金子，必须留下足够杀死所有虫子的战士数量， 即(x+19)/20个战士，然后这些留下战士就不能再去其它洞穴
　　其他战士可以继续走去其它洞穴，可以选择分组去不同的洞穴。
　　战士只能往洞穴深处走，不能走回头路。
　　问最多能获得多少金子？
 思路：
　　基础的树形DP。
　　要特别注意的是，如果某个节点的bug数为0，而金子不为0，那么仍然需要派1个人以上去捡，而不是花0个士兵就能获得该点的金子。
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 const int N=110;
 4 vector<int> vect[N];
 5 int bug[N], num[N], dp[N][N], n, m;
 6 
 7 void DFS(int t,int far,int peo)
 8 {
 9     int need=(bug[t]+19)/20;
10     if(peo<need || peo==0)    return ;
11 
12     for(int i=need; i<=peo; i++)    dp[t][i]=num[t];    //若有士兵i>=need个，那么起码拿到本节点的金子
13     for(int i=0; i<vect[t].size(); i++)
14     {
15         int to=vect[t][i];
16         if(to^far)
17         {
18             DFS(to, t, peo-need);
19             for(int j=peo; j>need; j--)         //到达本节点可能的人数。
20                 for(int k=1; k<=j-need; k++)    //给孩子to分配k个士兵。
21                     dp[t][j]=max(dp[t][j],dp[t][j-k]+dp[to][k]);
22         }
23     }
24 }
25 
26 
27 int main()
28 {
29     freopen("input.txt","r",stdin);
30     int u,v;
31     while(scanf("%d%d",&n,&m), n+m>0)
32     {
33         for(int i=0; i<=n; i++)    vect[i].clear();
34         memset(dp,0,sizeof(dp));
35 
36         for(int i=1; i<=n; i++)      //房间里的bug数、金子
37             scanf("%d%d",&bug[i],&num[i]);
38         for(int i=1; i<n; i++)            //连通情况
39         {
40             scanf("%d%d",&u,&v);
41             vect[u].push_back(v);
42             vect[v].push_back(u);
43         }
44         DFS(1, -1, m);
45         printf("%d\n", dp[1][m]);
46     }
47     return 0;
48 }
AC代码
```

