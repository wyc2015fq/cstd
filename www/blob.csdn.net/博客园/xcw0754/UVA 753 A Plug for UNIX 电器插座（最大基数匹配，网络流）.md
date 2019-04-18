# UVA 753 A Plug for UNIX 电器插座（最大基数匹配，网络流） - xcw0754 - 博客园
# [UVA 753 A Plug for UNIX 电器插座（最大基数匹配，网络流）](https://www.cnblogs.com/xcw0754/p/4645420.html)
题意：
　　给n个插座，m个设备（肯定要插电了），k种转换头可无限次使用（注意是单向的），问有多少设备最终是不能够插上插座的？
分析：
　　看起来就是设备匹配插座，所以答案不超过m。这个题适合用网络流来解。
　　假设每种头对应着一个编号（可以用map实现转换string到int），主要在k种转换头的建边，他们之间的转换关系就是编号与编号之间的边，因为可以无限次使用，所以容量无穷。再添加源点和汇点就建完了，汇点连接每个插座，源点连接每个设备，每边容量为1。使用增广路算法就得出解了。注意要空一行。
　　很不愿意用结构体的，但是既然用起来这么方便，就用着吧，有些题是需要用结构体的。
```
1 //#pragma comment(linker,"/STACK:102400000,102400000")
  2 #include <iostream>
  3 #include <stdio.h>
  4 #include <string.h>
  5 #include <vector>
  6 #include <stack>
  7 #include <algorithm>
  8 #include <map>
  9 #include <bits/stdc++.h>
 10 #define LL long long
 11 #define pii pair<int,int>
 12 #define INF 0x7f7f7f7f
 13 using namespace std;
 14 const int N=1000000+100;
 15 unordered_map<string, int>   has;
 16 vector<int> dev;
 17 vector<int> plug;
 18 vector<pii> chg;
 19 vector<int> vect[500];
 20 int path[500];
 21 int flow[500];
 22 int edge_cnt;
 23 
 24 struct node
 25 {
 26     int from;
 27     int to;
 28     int cap;
 29     int flo;
 30 }edge[N];
 31 
 32 void add_node(int fr,int t,int ca,int fl)
 33 {
 34     edge[edge_cnt].from=fr;
 35     edge[edge_cnt].to=t;
 36     edge[edge_cnt].cap=ca;
 37     edge[edge_cnt].flo=fl;
 38     vect[fr].push_back(edge_cnt++);
 39 }
 40 
 41 void build_graph(int n, int m, int k, int hui)
 42 {
 43     //每种型号的插口是个点
 44     for(int i=0; i<chg.size(); i++)    //转换器自身
 45     {
 46         int in=chg[i].first;
 47         int out=chg[i].second;
 48 
 49         add_node(in, out, INF, 0);  //应该是四条边
 50         add_node(out, in, 0, 0);
 51     }
 52 
 53     for(int i=0; i<dev.size(); i++)   //添加源点0号
 54     {
 55         int in=dev[i];
 56         add_node(0, in, 1, 0);
 57         add_node(in, 0, 0, 0);
 58     }
 59 
 60     for(int i=0; i<plug.size(); i++)    //添加汇点
 61     {
 62         int out=plug[i];
 63         add_node(out, hui, 1, 0);
 64         add_node(hui, out, 0, 0);
 65     }
 66 }
 67 
 68 int bfs(int s,int e)
 69 {
 70     deque<int> que;
 71     que.push_back(s);
 72     flow[s]=INF;
 73     while(!que.empty())
 74     {
 75         int x=que.front();
 76         que.pop_front();
 77         for(int i=0; i<vect[x].size(); i++)
 78         {
 79             node e=edge[vect[x][i]];
 80             if(!flow[e.to] && e.cap>e.flo)
 81             {
 82                 path[e.to]=vect[x][i];
 83                 flow[e.to]=min(flow[e.from],e.cap-e.flo);
 84                 que.push_back(e.to);
 85             }
 86         }
 87         if(flow[e]) return flow[e];
 88     }
 89     return flow[e];
 90 }
 91 
 92 int cal(int s, int e)
 93 {
 94     int big_flow=0;
 95     while(true)
 96     {
 97         memset(flow,0,sizeof(flow));
 98         memset(path,0,sizeof(path));
 99 
100         int tmp=bfs(s,e);
101         if(!tmp)    return big_flow;
102         big_flow+=tmp;//统计流
103 
104         int ed=e;
105         while(ed!=s)
106         {
107             int t=path[ed];
108             edge[t].flo+=tmp;
109             edge[t^1].flo-=tmp;
110             ed=edge[t].from;
111         }
112     }
113 }
114 
115 
116 int main()
117 {
118     freopen("input.txt", "r", stdin);
119     int n, m, t, k;
120     char s3[30], s4[30];
121     string s1,s2;
122     cin>>t;
123     while(t--)
124     {
125         has.clear();
126         dev.clear();
127         plug.clear();
128         chg.clear();
129 
130         memset(edge,0,sizeof(edge));
131         edge_cnt=0;
132         for(int i=0; i<500; i++)    vect[i].clear();
133         int num=0;
134 
135         scanf("%d",&n); //插座
136         for(int i=0; i<n; i++)
137         {
138             scanf("%s",s3);
139             s1=s3;
140             if(!has[s1])    has[s1]=++num;
141             plug.push_back(has[s1]);
142         }
143 
144         scanf("%d",&m); //设备
145         for(int i=0; i<m; i++)
146         {
147             scanf("%s%s",s3,s4);
148             s1=s3;
149             s2=s4;
150             if(!has[s2])    has[s2]=++num;
151             dev.push_back(has[s2]);
152         }
153 
154         scanf("%d",&k); //转换头
155         for(int i=0; i<k; i++)
156         {
157             scanf("%s%s",s3,s4);
158             s1=s3;
159             s2=s4;
160             if(!has[s1])    has[s1]=++num;
161             if(!has[s2])    has[s2]=++num;
162             chg.push_back(make_pair(has[s1], has[s2]));
163         }
164         build_graph(n, m, k, num+1);
165         printf("%d\n",m-cal(0, num+1));
166         if(t)   printf("\n");
167     }
168     return 0;
169 }
170 
171 AC代码
AC代码
```
```
1 //#pragma comment(linker,"/STACK:102400000,102400000")
  2 #include <iostream>
  3 #include <stdio.h>
  4 #include <string.h>
  5 #include <vector>
  6 #include <stack>
  7 #include <algorithm>
  8 #include <map>
  9 #include <bits/stdc++.h>
 10 #define LL long long
 11 #define pii pair<int,int>
 12 #define INF 0x7f7f7f7f
 13 using namespace std;
 14 const int N=1000000+100;
 15 unordered_map<string, int>   has;
 16 vector<int> dev;
 17 vector<int> plug;
 18 vector<pii> chg;
 19 vector<int> vect[500];
 20 int path[500];
 21 int flow[500];
 22 int edge_cnt;
 23 
 24 struct node
 25 {
 26     int from;
 27     int to;
 28     int cap;
 29     int flo;
 30 }edge[N];
 31 
 32 void add_node(int fr,int t,int ca,int fl)
 33 {
 34     edge[edge_cnt].from=fr;
 35     edge[edge_cnt].to=t;
 36     edge[edge_cnt].cap=ca;
 37     edge[edge_cnt].flo=fl;
 38     vect[fr].push_back(edge_cnt++);
 39 }
 40 
 41 void build_graph(int n, int m, int k, int hui)
 42 {
 43     //每种型号的插口是个点
 44     for(int i=0; i<chg.size(); i++)    //转换器自身
 45     {
 46         int in=chg[i].first;
 47         int out=chg[i].second;
 48 
 49         add_node(in, out, INF, 0);  //应该是四条边
 50         add_node(out, in, 0, 0);
 51     }
 52 
 53     for(int i=0; i<dev.size(); i++)   //添加源点0号
 54     {
 55         int in=dev[i];
 56         add_node(0, in, 1, 0);
 57         add_node(in, 0, 0, 0);
 58     }
 59 
 60     for(int i=0; i<plug.size(); i++)    //添加汇点
 61     {
 62         int out=plug[i];
 63         add_node(out, hui, 1, 0);
 64         add_node(hui, out, 0, 0);
 65     }
 66 }
 67 
 68 int spfa(int s,int e)
 69 {
 70     deque<int> que;
 71     que.push_back(s);
 72     flow[s]=INF;
 73     while(!que.empty())
 74     {
 75         int x=que.front();
 76         que.pop_front();
 77         for(int i=0; i<vect[x].size(); i++)
 78         {
 79             node e=edge[vect[x][i]];
 80             if(!flow[e.to] && e.cap>e.flo)
 81             {
 82                 path[e.to]=vect[x][i];
 83                 flow[e.to]=min(flow[e.from],e.cap-e.flo);
 84                 que.push_back(e.to);
 85             }
 86         }
 87         if(flow[e]) return flow[e];
 88     }
 89     return flow[e];
 90 }
 91 
 92 int cal(int s, int e)
 93 {
 94     int big_flow=0;
 95     while(true)
 96     {
 97         memset(flow,0,sizeof(flow));
 98         memset(path,0,sizeof(path));
 99 
100         int tmp=spfa(s,e);
101         if(!tmp)    return big_flow;
102         big_flow+=tmp;//统计流
103 
104         int ed=e;
105         while(ed!=s)
106         {
107             int t=path[ed];
108             edge[t].flo+=tmp;
109             edge[t^1].flo-=tmp;
110             ed=edge[t].from;
111         }
112     }
113 }
114 
115 
116 int main()
117 {
118     freopen("input.txt", "r", stdin);
119     int n, m, t, k;
120     char s3[30], s4[30];
121     string s1,s2;
122     cin>>t;
123     while(t--)
124     {
125         has.clear();
126         dev.clear();
127         plug.clear();
128         chg.clear();
129 
130         memset(edge,0,sizeof(edge));
131         edge_cnt=0;
132         for(int i=0; i<500; i++)    vect[i].clear();
133         int num=0;
134 
135         scanf("%d",&n); //插座
136         for(int i=0; i<n; i++)
137         {
138             scanf("%s",s3);
139             s1=s3;
140             if(!has[s1])    has[s1]=++num;
141             plug.push_back(has[s1]);
142         }
143 
144         scanf("%d",&m); //设备
145         for(int i=0; i<m; i++)
146         {
147             scanf("%s%s",s3,s4);
148             s1=s3;
149             s2=s4;
150             if(!has[s2])    has[s2]=++num;
151             dev.push_back(has[s2]);
152         }
153 
154         scanf("%d",&k); //转换头
155         for(int i=0; i<k; i++)
156         {
157             scanf("%s%s",s3,s4);
158             s1=s3;
159             s2=s4;
160             if(!has[s1])    has[s1]=++num;
161             if(!has[s2])    has[s2]=++num;
162             chg.push_back(make_pair(has[s1], has[s2]));
163         }
164         build_graph(n, m, k, num+1);
165         printf("%d\n",m-cal(0, num+1));
166         if(t)   printf("\n");
167     }
168     return 0;
169 }
AC代码
```

