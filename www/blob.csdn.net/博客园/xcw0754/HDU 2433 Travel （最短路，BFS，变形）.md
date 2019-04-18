# HDU 2433 Travel （最短路，BFS，变形） - xcw0754 - 博客园
# [HDU 2433 Travel （最短路，BFS，变形）](https://www.cnblogs.com/xcw0754/p/4611692.html)
题意：
　　给出一个图的所有边，每次从图中删除一条边，求任意点对的路径总和（求完了就将边给补回去）。（有重边）
思路：
```
1 #include <bits/stdc++.h>
  2 using namespace std;
  3 const int N=105, INF=0x7f7f7f7f;
  4 int mapp[N][N];
  5 bool vis[N];     //是否存在队列中
  6 int dest[N];
  7 vector<pair<int,int> >  vect;
  8 int n, m;
  9 int num[N];
 10 
 11 
 12 void spfa(int x)
 13 {
 14     memset(vis,0,sizeof(vis));
 15     memset(dest,0x7f,sizeof(dest));
 16     deque<int> que;
 17     que.push_back(x);
 18     vis[x]=1;
 19     dest[x]=0;
 20     while(!que.empty())
 21     {
 22         int tmp=que.front();
 23         que.pop_front();
 24         vis[tmp]=0;
 25         for(int i=1; i<=n; i++)
 26         {
 27             if(mapp[tmp][i]>0 && dest[tmp]+1<dest[i] )
 28             {
 29                 dest[i]=dest[tmp]+1;
 30                 if(!vis[i]) //没有在队列中
 31                 {
 32                     vis[i]=1;
 33                     //que.push_back(i);
 34                     if(!que.empty() && dest[i]<dest[que.front()])    que.push_front(i);
 35                     else    que.push_back(i);
 36                 }
 37             }
 38         }
 39     }
 40 }
 41 
 42 
 43 
 44 
 45 int cal()
 46 {
 47     int sum=0;
 48     for(int i=1; i<=n; i++)
 49     {
 50 
 51         spfa(i);
 52         for(int j=1; j<=n; j++)
 53         {
 54             if(dest[j]==INF)    return 0;
 55             sum+=dest[j];
 56         }
 57     }
 58     return sum;
 59 }
 60 
 61 
 62 
 63 
 64 
 65 int main()
 66 {
 67     freopen("input.txt", "r", stdin);
 68     int a, b, c;
 69     while(~scanf("%d%d", &n, &m))
 70     {
 71         vect.clear();
 72         memset(mapp,0,sizeof(mapp));
 73         memset(num,0,sizeof(num));
 74         for(int i=0; i<m; i++)
 75         {
 76             scanf("%d%d",&a,&b);
 77             mapp[a][b]++;
 78             mapp[b][a]++;
 79             num[a]++;
 80             num[b]++;
 81             vect.push_back(make_pair(a,b));
 82         }
 83 
 84         for(int i=0; i<vect.size(); i++)
 85         {
 86             a=vect[i].first;
 87             b=vect[i].second;
 88             if(num[a]==1||num[b]==1)
 89             {
 90                 printf("INF\n");
 91                 continue;
 92             }
 93             mapp[a][b]--;    //先断开
 94             mapp[b][a]--;
 95 
 96             int ans=cal();
 97             if(ans==0)  printf("INF\n");
 98             else        printf("%d\n",ans);
 99 
100             mapp[a][b]++;     //再连上
101             mapp[b][a]++;
102         }
103 
104 
105     }
106 
107     return 0;
108 }
正确而TLE代码，spfa+暴力
```
```
1 #include <bits/stdc++.h>
  2 using namespace std;
  3 const int N=105, INF=0x7f7f7f7f;
  4 int mapp[N][N];//图矩阵
  5 bool mapp2[N][N][N];   //标记从某点出发所需关键路径
  6 int tem_sum[N];
  7 int num[N];     //每个点的度
  8 int vis[N];     //用于BFS
  9 vector<pair<int,int> >  vect;   //m条边
 10 int n, m;
 11 
 12 int bfs(int x)
 13 {
 14     memset(vis,0,sizeof(vis));
 15     deque<int> que(1,x);
 16     int cnt=0, sum=0;
 17     vis[x]=1;
 18     while(!que.empty())
 19     {
 20         cnt++;
 21         int siz=que.size();
 22         for(int i=0; i<siz; i++)
 23         {
 24             int tmp=que.front();
 25             que.pop_front();
 26             for(int j=1; j<=n; j++)
 27             {
 28                 if(!vis[j]&&mapp[tmp][j])
 29                 {
 30                     vis[j]=1;
 31                     sum+=cnt;
 32                     mapp2[x][tmp][j]= mapp2[x][j][tmp]=1;//主要在这，记录每个点出发所需要的关键路径
 33                     que.push_back(j);
 34                 }
 35             }
 36         }
 37     }
 38     return sum;
 39 }
 40 
 41 int cal(int a,int b)    //a-b是删除的边
 42 {
 43     int sum=0;
 44     for(int i=1; i<=n; i++)
 45     {
 46         if(!a || mapp2[i][a][b] )
 47         {
 48             int tmp=bfs(i);
 49             if(!tmp)    return 0;
 50             if(!a)  tem_sum[i]=tmp; //第一次求还得更新tem_sum
 51             sum+=tmp;
 52         }
 53         else    sum+=tem_sum[i];
 54     }
 55     return sum;
 56 }
 57 
 58 
 59 
 60 int main()
 61 {
 62     //freopen("input.txt", "r", stdin);
 63     int a, b, c;
 64     while(cin>>n>>m)
 65     {
 66         vect.clear();
 67         memset(mapp,0,sizeof(mapp));
 68         memset(mapp2,0,sizeof(mapp2));
 69         memset(num,0,sizeof(num));
 70         memset(tem_sum,0,sizeof(tem_sum));  //一旦更新，不再改
 71 
 72         for(int i=0; i<m; i++)
 73         {
 74             scanf("%d%d",&a,&b);
 75             mapp[a][b]++, mapp[b][a]++;
 76             num[a]++, num[b]++;
 77             vect.push_back(make_pair(a,b));
 78         }
 79 
 80         int sum=cal(0,0);    //先求sum，看能否每点互通，不通以下都不必计算了。
 81         for(int i=0; i<m; i++)
 82         {
 83             a=vect[i].first,  b=vect[i].second;
 84             if(num[a]==1 || num[b]==1 || m<n || sum==0 )   //度为1的点，边不够，不连通
 85             {
 86                 printf("INF\n");
 87                 continue;
 88             }
 89             if(mapp[a][b]==1)                   //仅有那么1条边，才需要更新
 90             {
 91                 mapp[a][b]--, mapp[b][a]--;     //先断开
 92                 int ans=cal(a,b);//只求需要该边的SSSP
 93                 if(ans==0)  printf("INF\n");
 94                 else        printf("%d\n",ans);
 95                 mapp[a][b]++, mapp[b][a]++;     //再连上
 96             }
 97             else    printf("%d\n",sum);         //非关键，直接输出
 98         }
 99 
100     }
101     return 0;
102 }
AC代码(BFS)
```

