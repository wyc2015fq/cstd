# POJ  3683  Priest John's Busiest Day (2-SAT，常规) - xcw0754 - 博客园
# [POJ  3683  Priest John's Busiest Day (2-SAT，常规)](https://www.cnblogs.com/xcw0754/p/4634384.html)
题意：
　　一些人要在同一天进行婚礼，但是牧师只有1个，每一对夫妻都有一个时间范围[s , e]可供牧师选择，且起码要m分钟才主持完毕，但是要么就在 s 就开始，要么就主持到刚好 e 结束。因为人数太多了，这些时间可能会重叠，可能还会完全包含，可能还没有交叉，各种情况。问牧师能否主持完全部人的婚礼，若可以，给出每对夫妻占用牧师的一个时间段（记得按所给的夫妻的顺序哦）。
　　主要步骤如下。
（1）先建原图，不管是否会冲突。
（2）找强连通分量来缩点，如果会冲突，这个时候应该发现。
（3）建个缩点后，原图的反向图。
（4）着色法标记所要输出的时间段。
（5）时间段按照所给每对夫妻的顺序来输出。
实现：
　　（1）婚礼要么在s就开始，要么在e刚好结束，还有可能这对夫妻就要求从s-e都要主持呢。 先把时间给转换成分钟，每对夫妻拥有2个时间段。
　　（2）接下来主要是建图，建图时只考虑“冲突”，即如果i*2和j*2冲突了，那么选i*2就必须选j*2+1。先不用顾着i*2和j*2+1是否冲突，只要将4种可能的组合列出来，建图，其他的交给强连通分量去做。
　　（3）强连通分量承接第3步的重任，在求强连通分量时要顺便判断是否会冲突，当然这比较简单。
　　（4）建反向图只是穷举原图中的每条边，判断是否在同个scc中，如果不是就可以建边了，将边反过来这么简单。
　　（5）着色时还得将另外冲突的“全部“着其他颜色，那么根据已建好的反向图一直DFS下去，如果已经着色，可以退出了，别每次都深搜到底了，没必要。
　　（6）时间段还得按所给夫妻顺序！那得再稍微处理一下。
```
1 #include <iostream>
  2 #include <stdio.h>
  3 #include <string.h>
  4 #include <vector>
  5 #include <stack>
  6 #include <algorithm>
  7 #define LL long long
  8 #define pii pair<int,int>
  9 #define INF 0x7f7f7f7f
 10 using namespace std;
 11 const int N=1000+5;
 12 int t1[N], t2[N], t3[N], t4[N]; //保存时间点
 13 bool vis[N];
 14 vector<int> vect[N*2], rg[N*2], scc[N*2];   //分别是：原图，反向图，
 15 stack<int> stac;    //强连通分量必备
 16 int lowlink[N*2], dfn[N*2], scc_no[N*2], scc_cnt, dfn_clock;    //强连通分量必备
 17 int col[N*2];   //用于着色
 18 bool dele[N*2]; //标记输出点
 19 
 20 inline int to_time(int h, int m){return h*60+m;}
 21 inline bool conflict(int a,int b, int c, int d){if(b<=c || d<=a)    return true;return false;}
 22 
 23 void get_graph(int n)   //难在建图吧？
 24 {
 25     for(int i=0; i<n*2; i++)  vect[i].clear();
 26     for(int i=0; i<n; i++)
 27     {
 28         for(int j=0; j<n; j++)        //考虑放在前面
 29         {
 30             if(i==j) continue;
 31             if(!conflict(t1[i],t2[i], t1[j],t2[j]))  vect[i*2].push_back(j*2+1);
 32             if(!conflict(t1[i],t2[i], t3[j],t4[j]))  vect[i*2].push_back(j*2);
 33 
 34             if(!conflict(t3[i],t4[i], t1[j],t2[j]))  vect[i*2+1].push_back(j*2+1);
 35             if(!conflict(t3[i],t4[i], t3[j],t4[j]))  vect[i*2+1].push_back(j*2);
 36         }
 37     }
 38 }
 39 
 40 void DFS(int x) //模板tarjan
 41 {
 42     stac.push(x);
 43     dfn[x]=lowlink[x]=++dfn_clock;
 44     for(int i=0; i<vect[x].size(); i++)
 45     {
 46         int t=vect[x][i];
 47         if(!dfn[t])
 48         {
 49             DFS(t);
 50             lowlink[x]=min(lowlink[x],lowlink[t]);
 51         }
 52         else if(!scc_no[t]) lowlink[x]=min(lowlink[x],dfn[t]);
 53     }
 54     if(lowlink[x]==dfn[x])
 55     {
 56         scc[++scc_cnt].clear();
 57         while(true)
 58         {
 59             int t=stac.top();stac.pop();
 60             scc_no[t]=scc_cnt;
 61             scc[scc_cnt].push_back(t);
 62             if(t==x)    break;
 63         }
 64     }
 65 }
 66 
 67 int find_scc(int n) //求强连通分量，顺便检查是否满足条件
 68 {
 69     scc_cnt=dfn_clock=0;
 70     memset(lowlink,0,sizeof(lowlink));
 71     memset(dfn,0,sizeof(dfn));
 72     memset(scc_no,0,sizeof(scc_no));
 73     for(int i=0; i<n; i++)  if(!dfn[i]) DFS(i);
 74     for(int i=0; i<n; i+=2) if(scc_no[i]==scc_no[i+1])  return false;   //检查是否冲突
 75     return true;
 76 }
 77 
 78 void build_rg(int n)    //建反向图，为了要反向着色
 79 {
 80     for(int i=0; i<n; i++)  rg[i].clear();
 81 
 82     for(int i=0; i<n; i++)
 83     {
 84         for(int j=0; j<vect[i].size(); j++)
 85         {
 86             int t=vect[i][j];
 87             if(scc_no[i]!=scc_no[t])    //不属于同个强连通分量
 88                 rg[scc_no[t]].push_back(scc_no[i]);
 89         }
 90     }
 91 }
 92 
 93 void del(int s)     //删除的是反向边，之前已建好的rg图
 94 {
 95     while(!col[s])
 96     {
 97         col[s]=3;
 98         for(int i=0; i<rg[s].size(); i++)    del(rg[s][i]); //递归“删除”,按反向边的方向（即着其他颜色）
 99     }
100 }
101 
102 void color()
103 {
104     memset(col,0,sizeof(col));
105     for(int i=1; i<=scc_cnt; i++)   //没有按照拓扑顺序也能AC???
106         if(!col[i])
107         {
108             col[i]=2;
109             del( scc_no[ scc[i][0]%2==0?scc[i][0]+1:scc[i][0]-1]);    //在第i个SCC中任取1点，取其对立点，再取其所在scc编号，进行着色
110         }
111 }
112 
113 void print(int n)
114 {
115     memset(dele,0,sizeof(dele));
116     puts("YES");
117     for(int i=1; i<scc_cnt; i++)
118     {
119         if(col[i]==2)   //col=2的都是要的，当然，选全部col=3也一样，因为对称
120             for(int j=0; j<scc[i].size(); j++)
121                 dele[scc[i][j]  ]=true; //把所有要输出的点先标记出来
122     }
123     for(int i=0; i<n; i++)
124     {
125         if(dele[i])
126             if(i%2==0)  //取前段
127             {
128                 int a=t1[i/2]/60;
129                 int b=t1[i/2]%60;
130                 int c=t2[i/2]/60;
131                 int d=t2[i/2]%60;
132                 printf("%02d:%02d %02d:%02d\n",a,b,c,d);
133             }
134             else        //取后段
135             {
136                 int a=t3[i/2]/60;
137                 int b=t3[i/2]%60;
138                 int c=t4[i/2]/60;
139                 int d=t4[i/2]%60;
140                 printf("%02d:%02d %02d:%02d\n",a,b,c,d);
141             }
142     }
143 }
144 int main()
145 {
146     freopen("input.txt", "r", stdin);
147     int n, a, b, c, d, e, f, g;
148     while(~scanf("%d",&n))
149     {
150         memset(t1,0,sizeof(t1));
151         memset(t2,0,sizeof(t2));
152         memset(t3,0,sizeof(t3));
153         memset(t4,0,sizeof(t4));
154         for(int i=0; i<n; i++)
155         {
156             scanf("%d %c %d %d %c %d %d",&a,&b,&c,  &d,&e,&f,  &g);
157             t1[i]=to_time(a,c); //时间转成分钟，分别有2种，t1t2表示在前的start和end，t3t4表示在后
158             t2[i]=to_time(a,c)+g;
159             t3[i]=to_time(d,f)-g;
160             t4[i]=to_time(d,f);
161         }
162         get_graph(n);
163         if(!find_scc(n<<1))    {puts("NO");continue;}
164         build_rg(n*2);          //按缩点建新的反向图reverse_graph
165         color();                //着色完，所需要的点也就出结果了
166         print(n*2);
167     }
168     return 0;
169 }
AC代码
```

