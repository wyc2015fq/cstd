# POJ 2942 Knights of the Round Table （点双连通分量） - xcw0754 - 博客园
# [POJ 2942 Knights of the Round Table （点双连通分量）](https://www.cnblogs.com/xcw0754/p/4625062.html)
题意：多个骑士要开会，3人及以上才能凑一桌，其中部分人已经互相讨厌，肯定不坐在同一桌的相邻位置，而且一桌只能奇数个人才能开台。给出多个人的互相讨厌图，要求多少人开不成会（注：会议不要求同时进行，一个人开多个会不冲突）？
分析：
　　给的是互相讨厌的图，那么转成互相喜欢的吧，扫一遍，如果不互相讨厌就认为互相喜欢，矩阵转邻接表先。
　　有边相连的两个点代表能坐在一块。那么找出一个圈圈出来，在该圈内的点有奇数个人的话肯定能凑成1桌。圈圈？那就是简单环了，跟点双连通分量的定义好像一样：每个点都能同时处于1个及以上的简单环中。这么说，只要有环，他们就能凑一桌了（每个环开一桌，同1人参加多桌并不冲突）。
　　但是奇数的问题怎么解决？如果是一个点双连通分量是个偶图（即二分图），那么肯定只有偶数环。想想，图都双连通了，那么必有简单环，1个简单环中如果是奇数个了，着色法染色时必定有冲突。那么就用偶图判定来解决这个问题。
实现：
　　（1）互相讨厌图转互相喜欢图。
　　（2）求点双连通分量，并把同个点双连通分量内的点都给归类出来。（注意可能图不连通）
　　（3）黑白着色判定偶图，非偶图的留下，偶图忽略。3个人一下的点双连通分量也忽略。
　　（4）只要1个点能够处于任一非偶图中，标记其为“可以开会”。
　　（5）统计哪些人开不了会。（肯定是那些3人以下的，偶数个人还想坐一块的）
```
1 //#include <bits/stdc++.h>
  2 #include <iostream>
  3 #include <cstdio>
  4 #include <cstring>
  5 #include <stack>
  6 #include <vector>
  7 #define LL long long
  8 #define pii pair<int,int>
  9 using namespace std;
 10 const int N=1000+5;
 11 const int INF=0x7f7f7f7f;
 12 int n, m, bcc_cnt, dfn_clock;    //点双连通分量的个数
 13 int g[N][N], dfn[N], low[N], bcc_no[N], col[N], flag[N];
 14 
 15 stack<pair<int,int> >   stac;
 16 vector<int> bcc[N], vect[N];
 17 
 18 
 19 void DFS(int x,int far)
 20 {
 21     dfn[x]=low[x]=++dfn_clock;
 22     for(int i=0; i<vect[x].size(); i++)
 23     {
 24         int t=vect[x][i];
 25         if(!dfn[t])
 26         {
 27             stac.push(make_pair(x,t));
 28             DFS(t,x);
 29             low[x]=min(low[x],low[t]);
 30             if(low[t]>=dfn[x])
 31             {
 32                 bcc[++bcc_cnt].clear();
 33                 while(1)
 34                 {
 35                     int a=stac.top().first;
 36                     int b=stac.top().second;
 37                     stac.pop();
 38                     if(bcc_no[a]!=bcc_cnt)
 39                     {
 40                         bcc_no[a]=bcc_cnt;
 41                         bcc[bcc_cnt].push_back(a);
 42                     }
 43                     if(bcc_no[b]!=bcc_cnt)
 44                     {
 45                         bcc_no[b]=bcc_cnt;
 46                         bcc[bcc_cnt].push_back(b);
 47                     }
 48                     if(a==x && b==t)  break;
 49                 }
 50             }
 51         }
 52         else if(dfn[t]<dfn[x] && t!=far)    //特别注意，“dfn[t]<dfn[x]”这句是必须的，特别是在求点双连通分量时。否则可能乱。
 53         {
 54             stac.push(make_pair(x,t));
 55             low[x]=min(low[x],dfn[t]);
 56         }
 57     }
 58 }
 59 
 60 void find_bcc()     //找出点双连通分量，放在bcc中
 61 {
 62     bcc_cnt= dfn_clock= 0;
 63     memset(low,   0,sizeof(low));
 64     memset(bcc_no,0,sizeof(bcc_no));
 65     memset(dfn,   0,sizeof(dfn));
 66     for(int i=1; i<=n; i++)
 67         if(!dfn[i])    DFS(i,-1);
 68 }
 69 
 70 int color(int num)  //判断是否偶图,偶图不含奇圈
 71 {
 72     col[bcc[num][0]]=2;
 73     deque<int> que;que.push_back(bcc[num][0]);
 74     while(!que.empty()) //广搜着色
 75     {
 76         int x=que.front();que.pop_front();
 77         for(int i=0; i<bcc[num].size(); i++)
 78         {
 79             int t=bcc[num][i];
 80             if(x!=t&&!g[x][t])  //只要有边
 81             {
 82                 if(col[t]==col[x])    return false; //颜色已经相同，非偶图
 83                 if(!col[t])         //无染过才进
 84                 {
 85                     col[t]=3-col[x];
 86                     que.push_back(t);
 87                 }
 88 
 89             }
 90         }
 91     }
 92     return true;
 93 }
 94 
 95 int color_it()
 96 {
 97     memset(flag, 0, sizeof(flag));
 98     for(int i=1; i<=bcc_cnt; i++)
 99     {
100         memset(col, 0, sizeof(col));        //每次都要置0，因为可能有点属于两个双连通分量
101         if(bcc[i].size()<3)   continue;   //不够人数开会
102         if(color(i)==false)               //不是偶图
103             for(int j=0; j<bcc[i].size(); j++)  //这些人都可以开会，mark一下
104                 flag[bcc[i][j]]=1;
105     }
106 
107     int cnt=0;
108     for(int i=1; i<=n; i++) //统计哪些人不能开会
109         if(!flag[i])    cnt++;
110     return cnt;
111 }
112 
113 int main()
114 {
115     freopen("input.txt", "r", stdin);
116     int a, b;
117     while(scanf("%d%d",&n,&m), n+m)
118     {
119         for(int i=1; i<=n; i++) vect[i].clear();
120         memset(g,0,sizeof(g));
121 
122         for(int i=0; i<m; i++)
123         {
124             scanf("%d%d",&a,&b);
125             g[a][b]=g[b][a]=1;
126         }
127         for(int i=1; i<=n; i++)     //转邻接表
128             for(int j=i+1; j<=n; j++)
129                 if(!g[i][j])    vect[i].push_back(j),vect[j].push_back(i);
130 
131         find_bcc();
132         printf("%d\n",color_it());
133 
134     }
135 
136     return 0;
137 }
AC代码
```

