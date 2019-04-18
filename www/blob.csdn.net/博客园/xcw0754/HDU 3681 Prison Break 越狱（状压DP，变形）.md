# HDU 3681 Prison Break 越狱（状压DP，变形） - xcw0754 - 博客园
# [HDU 3681 Prison Break 越狱（状压DP，变形）](https://www.cnblogs.com/xcw0754/p/4861537.html)
题意：
　　给一个n*m的矩阵，每个格子中有一个大写字母，一个机器人从‘F’出发，拾取所有的开关‘Y’时便能够越狱，但是每走一格需要花费1点能量，部分格子为充电站‘G’，每个电站只能充1次电。而且部分格子为障碍'D'或者空格‘S’。机器人在开始时电池是满能量的，问电池容量至少为多少可以越狱？（1<=n,m<=14，充电站+开关的总个数<=15）
思路：
　　看错题了，以为充电站和开关的个数分别至多为14，其实是两种加起来14。
　　既然只有15个关键的格子，那么状压就有用了。假设每个点关键格必须走1次，那么就像可重复走的TSP了。但是充电站只能充1次，而且路过了也可以不充。所以，其实关键格只有那些“开关”，只要保证所有开关都收集全了，其他都无所谓。
　　具体做法，求出这15个格子（包括起点）的两两之间的距离（BFS就够了），相当于一个完全连通图。再二分答案，判断是否能够收集所有开关。判断能否收集开关，就相当于求所有点只能走1次的欧拉路径了，直接状态压缩来求，注意：两点间不一定可达，遇到充电站必须充满电。这样子对于一个格子具体走过了多少格已经没有关系了。
```
1 //#include <bits/stdc++.h>
  2 #include <iostream>
  3 #include <cstdio>
  4 #include <cstring>
  5 #include <cmath>
  6 #include <deque>
  7 #include <map>
  8 #include <algorithm>
  9 #include <vector>
 10 #include <iostream>
 11 #define pii pair<int,int>
 12 #define INF 0x3f3f3f3f
 13 #define LL long long
 14 #define ULL unsigned long long
 15 using namespace std;
 16 const double PI  = acos(-1.0);
 17 const int N=16;
 18 int dp[1<<15][N], tag[N][N], vis[N][N], dis[N][N], n, m, egy;
 19 char g[N][N];
 20 
 21 struct node
 22 {
 23     int x,y,d;
 24     node(){};
 25     node(int x,int y,int d):x(x),y(y),d(d){};
 26 };
 27 inline bool istar(int x,int y){return tag[x][y]>0;}
 28 inline bool isok(int x,int y){ return (x>0&&x<=n)&&(y>0&&y<=m)&&g[x][y]!='D';}
 29 inline int cmp(node a,node b){ return  g[a.x][a.y]<g[b.x][b.y];}
 30 bool binary(int cap)
 31 {
 32     memset(dp,-1,sizeof(dp));
 33     dp[1][1]=cap;
 34     for(int s=1; s<(1<<n); s++)
 35     {
 36         for(int i=1; i<=n; i++)
 37         {
 38             if( (s&(1<<i-1))==0 )   continue;   //未走
 39             for(int j=2; j<=n; j++)
 40             {
 41                 if( s&(1<<j-1) )    continue;   //只能走1次
 42                 int add= j<=egy?cap:-1;
 43                 if( dis[i][j]>0 && dp[s][i]>=dis[i][j] )   //前提要走得到那个位置
 44                     dp[s|(1<<j-1)][j]=max(dp[s|(1<<j-1)][j], max(dp[s][i]-dis[i][j],add));
 45             }
 46         }
 47     }
 48     int mod=(1<<n)-(1<<egy)+1, ans=-INF;
 49     for(int s=1; s<(1<<n); s++)
 50     {
 51         if( (s&mod)==mod )
 52         {
 53             for(int i=2; i<=n; i++)
 54                 ans=max(ans, dp[s][i]);
 55         }
 56     }
 57     return ans>=0;
 58 }
 59 
 60 deque<node>  que;
 61 void BFS(int x,int y)   //求最短路
 62 {
 63     memset(vis,0,sizeof(vis));
 64     que.clear();
 65     que.push_back(node(x,y,0));
 66     vis[x][y]=true;
 67     while(!que.empty())
 68     {
 69         node t=que.front();que.pop_front();
 70         if( istar(t.x,t.y) )    //记录最短路
 71             dis[tag[x][y]][tag[t.x][t.y]]=t.d;
 72         if(isok(t.x-1,t.y)&&!vis[t.x-1][t.y])
 73         {
 74             que.push_back(node(t.x-1,t.y,t.d+1) );
 75             vis[t.x-1][t.y]=true;
 76         }
 77         if(isok(t.x+1,t.y)&&!vis[t.x+1][t.y])
 78         {
 79             que.push_back(node(t.x+1,t.y,t.d+1) );
 80             vis[t.x+1][t.y]=true;
 81         }
 82         if(isok(t.x,t.y-1)&&!vis[t.x][t.y-1])
 83         {
 84             que.push_back(node(t.x,t.y-1,t.d+1) );
 85             vis[t.x][t.y-1]=true;
 86         }
 87         if(isok(t.x,t.y+1)&&!vis[t.x][t.y+1])
 88         {
 89             que.push_back(node(t.x,t.y+1,t.d+1) );
 90             vis[t.x][t.y+1]=true;
 91         }
 92     }
 93 }
 94 
 95 
 96 int cal()
 97 {
 98     vector<node> vect;   //关键点
 99     for(int i=1; i<=n; i++)    //编号
100         for(int j=1; j<=m; j++)
101             if(g[i][j]!='S'&&g[i][j]!='D')
102                 vect.push_back(node(i,j,0));
103 
104     sort(vect.begin(), vect.end(), cmp);    //按字典序排序
105     for(int i=0; i<vect.size(); i++)      //编号
106         tag[vect[i].x][vect[i].y]=i+1;
107 
108     egy=1;
109     for(int i=0; i<vect.size(); i++)
110     {
111         if(g[vect[i].x][vect[i].y]=='G')    egy++;
112         BFS(vect[i].x,vect[i].y);       //计算最短路
113     }
114     n=vect.size();
115     if(egy==n)    return 0;   //无开关？
116     if(!binary(1000))       return -1;   //不可达
117 
118     int L=0, R=300;
119     while(L<R)      //二分答案
120     {
121         int mid=R-(R-L+1)/2;
122         if( binary(mid) )   R=mid;
123         else                L=mid+1;
124     }
125     return R;
126 }
127 
128 int main()
129 {
130     //freopen("input.txt","r",stdin);
131     while(scanf("%d%d",&n,&m), n+m)
132     {
133         memset(tag,0,sizeof(tag));
134         memset(dis,-1,sizeof(dis));
135 
136         for(int i=1; i<=n; i++)    scanf("%s",g[i]+1);
137         printf("%d\n",cal());
138     }
139     return 0;
140 }
AC代码
```

