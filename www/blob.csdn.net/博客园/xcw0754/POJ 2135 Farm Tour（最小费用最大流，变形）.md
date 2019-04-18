# POJ 2135 Farm Tour（最小费用最大流，变形） - xcw0754 - 博客园
# [POJ 2135 Farm Tour（最小费用最大流，变形）](https://www.cnblogs.com/xcw0754/p/4643231.html)
题意：给一个无向图，FJ要从1号点出发到达n号点，再返回到1号点，但是路一旦走过了就会销毁（即回去不能经过），每条路长度不同，那么完成这趟旅行要走多长的路？（注：会有重边，点号无序，无向图！）
思路：
　　有重边，要用邻接表。所给的每条边都要变成4条有向边！否则可能一开始就到达不了终点了。最后要再加上一个源点和汇点，容量cap（源点，1）=2，指定只能走两次，再规定其他所给的边的容量是1就行了，当边被走过了，就自动增加了流，也就走不了了。
　　解释看代码更清晰。
```
1 //#pragma comment(linker,"/STACK:102400000,102400000")
  2 #include <iostream>
  3 #include <stdio.h>
  4 #include <string.h>
  5 #include <vector>
  6 #include <stack>
  7 #include <algorithm>
  8 #include <map>
  9 //#include <bits/stdc++.h>
 10 #define LL long long
 11 #define pii pair<int,int>
 12 #define INF 0x7f7f7f7f
 13 using namespace std;
 14 const int N=10000+5;
 15 vector<int> vect[N];
 16 struct node
 17 {
 18     int from;
 19     int to;
 20     int cost;
 21     int cap;
 22     int flow;
 23 }edge[N*4];     //边数是4*M的大小
 24 
 25 int edge_num;   //边的上限
 26 int f[N];       //流
 27 int path[N];    //记录路径
 28 int c[N];       //记录费用
 29 bool in[N];     //是否在queue中
 30 
 31 void add_node(int a, int b, int c, int ca, int fl)
 32 {
 33     edge[edge_num].from=a;
 34     edge[edge_num].to=b;
 35     edge[edge_num].cost=c;
 36     edge[edge_num].cap=ca;
 37     edge[edge_num].flow=fl;
 38     vect[a].push_back(edge_num++);
 39 }
 40 
 41 
 42 LL spfa(int s,int e)
 43 {
 44     deque<int> que;
 45     que.push_back(s);
 46     c[s]=0;
 47     f[s]=INF;
 48     in[s]=1;
 49     while(!que.empty())
 50     {
 51         int x=que.front();
 52         que.pop_front();
 53         in[x]=0;
 54         for(int i=0; i<vect[x].size(); i++)
 55         {
 56             node t=edge[vect[x][i]];
 57             if(t.cap>t.flow && c[t.to]>c[t.from]+t.cost)  //能流过,且费用更小即可更新。
 58             {
 59                 path[t.to]=vect[x][i];          //记边号，方便更新flow
 60                 c[t.to]=c[t.from]+t.cost;       //更新cost，相当于距离
 61                 f[t.to]=min(f[t.from], t.cap-t.flow);
 62                 if(!in[t.to])
 63                 {
 64                     que.push_back(t.to);
 65                     in[t.to]=1;
 66                 }
 67             }
 68         }
 69     }
 70     return c[e];
 71 }
 72 
 73 int m;
 74 
 75 int mcmf(int s, int e)
 76 {
 77     LL ans=0;
 78     while(1)
 79     {
 80         memset(f, 0, sizeof(f));
 81         memset(c, 0x7f, sizeof(c));
 82         memset(in, 0, sizeof(in));
 83         memset(path, 0, sizeof(path));
 84 
 85         int tmp=spfa(s, e);
 86         if(tmp==INF)    return ans;
 87         ans+=tmp*f[e];   //这是最小费用。注：每次的流可能不是1。
 88 
 89         int ed=e;
 90         while(ed!=s)
 91         {
 92             int en=path[ed];
 93             edge[en].flow+=f[e];
 94             edge[en^1].flow-=f[e];
 95             ed=edge[en].from;
 96             //cout<<edge[en].from<<"-"<<edge[en].to<<endl;
 97         }
 98     }
 99 }
100 
101 
102 
103 
104 
105 int main()
106 {
107     freopen("input.txt", "r", stdin);
108     int  n, a, b, c;
109     while(~scanf("%d%d", &n, &m))
110     {
111         for(int i=n*2; i>=0; i--)   vect[i].clear();
112         edge_num=0;
113         for(int i=0; i<m; i++)
114         {
115             scanf("%d%d%d",&a,&b,&c);
116             add_node(a,b,c,1,0);    //4条有向边。
117             add_node(b,a,-c,0,0);
118 
119             add_node(b,a,c,1,0);
120             add_node(a,b,-c,0,0);
121         }
122         add_node(0,1,0,2,0);        //加多2条边
123         add_node(1,0,0,0,0);        //加这条无意义，但是更新flow时更方便
124         add_node(n,n+1,0,2,0);
125         add_node(n+1,n,0,0,0);
126 
127         printf("%lld\n",mcmf(0, n+1));
128     }
129 
130 
131     return 0;
132 }
AC代码
```

