# UVALive 4128 Steam Roller 蒸汽式压路机（最短路，变形）       WA中。。。。。 - xcw0754 - 博客园
# [UVALive 4128 Steam Roller 蒸汽式压路机（最短路，变形）       WA中。。。。。](https://www.cnblogs.com/xcw0754/p/4705868.html)
题意：
　　给一个由n*m个正方形格子组成的矩形，其中每个格子的边都是可以走的，长度给定，规定：如果在进入该路前需要拐弯，或者走完该路需要拐弯，都是需要付出双倍距离的（每条路最多算2倍）。问从起点到终点的最短路经长。
思路：
　　这个题目超级难搞，思路很简单，就是很麻烦！！！我将1个点4个方向的路长都记录下来，然后进行最短路，只要一个点的某个方向更新了，就进队。所有都考虑得差不多了，就是WA。。。
　　步骤 :
　　（1）起点不进队，因为起点出发都需要双倍，况且，如果起点路长为0的话，其他点就不能路过起点了，所以将起点能更新到的点进队，并更新他们的路长，而起点路长仍然是INF。
　　（2）对于用一个点来更新别人，可以更新相邻4个方向的点，而更新某一个方向的点又可以用当前点的4个方向来更新它。
　　（3）一旦被更新的点是终点，就更新答案（出来也要双倍）。因为路径可能经过终点多次再折返从终点出来会更近。
　　（4）所以边注意不能算2倍以上。
```
1 #include <bits/stdc++.h>
  2 #define INF 0x7f7f7f7f
  3 #define pii pair<int,int>
  4 #define LL unsigned long long
  5 #define up 0
  6 #define right 1
  7 #define down 2
  8 #define left 3
  9 using namespace std;
 10 const int N=110;
 11 int n, m, a, sx, sy, ex, ey;
 12 int g[N][N][4];     //0从12点钟开始，顺时针走
 13 int dist[N][N][4];  //记录来自4个方向的距离
 14 int tag[N][N][4];  //标记4个值是否已经翻倍了
 15 int inq[N][N];
 16 int ans;
 17 bool ok(int x, int y)
 18 {
 19     if(x>0&&x<=n && y>0&&y<=m)    return true;
 20     else    return false;
 21 }
 22 void isinq(deque<pii> &que, int x,int y)
 23 {
 24     if(!inq[x][y])    que.push_back(make_pair(x,y));
 25     inq[x][y]=1;
 26 }
 27 
 28 void isend(int x,int y)
 29 {
 30     if(x==ex && y==ey)
 31     {
 32         if(dist[x][y][up]<INF)
 33             ans=min(ans, dist[x][y][up]+(1-tag[x][y][up])*g[x][y][down]);
 34         if(dist[x][y][down]<INF)
 35             ans=min(ans, dist[x][y][down]+(1-tag[x][y][down])*g[x][y][up]);
 36         if(dist[x][y][left]<INF)
 37             ans=min(ans, dist[x][y][left]+(1-tag[x][y][left])*g[x][y][right]);
 38         if(dist[x][y][right]<INF)
 39             ans=min(ans, dist[x][y][right]+(1-tag[x][y][right])*g[x][y][left]);
 40     }
 41 }
 42 
 43 
 44 void init_start(deque<pii> &que)
 45 {
 46     int x=sx;
 47     int y=sy;
 48     if(ok(x-1,y) && g[x][y][up] )  //上0
 49     {
 50         dist[x-1][y][up] = 2*g[x][y][up] ;
 51         tag[x-1][y][up]=1;           //拐弯了，已经翻倍
 52         isinq(que, x-1, y );
 53         isend(x-1,y);
 54     }
 55     if(ok(x+1,y) && g[x][y][down] )  //下2
 56     {
 57         dist[x+1][y][down] = 2*g[x][y][down];
 58         tag[x+1][y][down]=1;
 59         isinq(que, x+1, y );
 60         isend(x+1,y);
 61     }
 62     if(ok(x,y-1) && g[x][y][left] )  //左3
 63     {
 64         dist[x][y-1][left] = 2*g[x][y][left];
 65         tag[x][y-1][left]=1;
 66         isinq(que, x, y-1);
 67         isend(x,y-1);
 68     }
 69     if(ok(x,y+1) && g[x][y][right] )  //右1
 70     {
 71         dist[x][y+1][right] = 2*g[x][y][right];
 72         tag[x][y+1][right]=1;
 73         isinq(que, x, y+1);
 74         isend(x,y+1);
 75     }
 76 
 77 }
 78 
 79 
 80 void spfa()
 81 {
 82     deque<pii> que;
 83     init_start(que);
 84 
 85 
 86     while(!que.empty())
 87     {
 88         int x=que.front().first;
 89         int y=que.front().second;
 90         que.pop_front();
 91         inq[x][y]=0;
 92         //每个点有4个方向
 93         if(ok(x-1,y) && g[x][y][up] )  //上0
 94         {
 95             if(dist[x][y][up]<INF && dist[x-1][y][up]> dist[x][y][up] + g[x][y][up])  //上->上
 96             {
 97                 dist[x-1][y][up]= dist[x][y][up] + g[x][y][up];
 98                 tag[x-1][y][up]=0;   //直走的，不计
 99                 isinq(que, x-1, y );
100             }
101             if(dist[x][y][right]<INF && dist[x-1][y][up]> dist[x][y][right] + (1-tag[x][y][right])*g[x][y][left] + 2*g[x][y][up]  )  //右->上
102             {
103                 dist[x-1][y][up]= dist[x][y][right] + (1-tag[x][y][right])*g[x][y][left] + 2*g[x][y][up];
104                 tag[x-1][y][up]=1;  //拐弯了，已经翻倍
105                 isinq(que, x-1, y );
106             }
107             if(dist[x][y][left]<INF && dist[x-1][y][up]> dist[x][y][left] + (1-tag[x][y][left])*g[x][y][right] + 2*g[x][y][up]  )  //左->上
108             {
109                 dist[x-1][y][up]= dist[x][y][left] + (1-tag[x][y][left])*g[x][y][right] + 2*g[x][y][up];
110                 tag[x-1][y][up]=1;  //同上
111                 isinq(que, x-1, y );
112             }
113             if(dist[x][y][down]<INF && dist[x-1][y][up]> dist[x][y][down] + (1-tag[x][y][down])*g[x][y][up] + 2*g[x][y][up]  )    //下-上
114             {
115                 dist[x-1][y][up]= dist[x][y][down] + (1-tag[x][y][down])*g[x][y][up] + 2*g[x][y][up];
116                 tag[x-1][y][up]=1;
117                 isinq(que, x-1, y);
118             }
119             isend(x-1,y);
120         }
121         if(ok(x+1,y) && g[x][y][down] )  //下2
122         {
123             if(dist[x][y][down]<INF && dist[x+1][y][down] > dist[x][y][down] + g[x][y][down] )  //下->下
124             {
125                 dist[x+1][y][down] = dist[x][y][down] + g[x][y][down];
126                 tag[x+1][y][down]=0;
127                 isinq(que, x+1, y );
128             }
129             if(dist[x][y][right]<INF && dist[x+1][y][down] > dist[x][y][right] + (1-tag[x][y][right])*g[x][y][left] + 2*g[x][y][down] )  //右->下
130             {
131                 dist[x+1][y][down] = dist[x][y][right] + (1-tag[x][y][right])*g[x][y][left] + 2*g[x][y][down];
132                 tag[x+1][y][down]=1;
133                 isinq(que, x+1, y );
134             }
135             if(dist[x][y][left]<INF && dist[x+1][y][down] > dist[x][y][left] + (1-tag[x][y][left])*g[x][y][right] + 2*g[x][y][down] )  //左->下
136             {
137                 dist[x+1][y][down] = dist[x][y][left] + (1-tag[x][y][left])*g[x][y][right] + 2*g[x][y][down];
138                 tag[x+1][y][down]=1;
139                 isinq(que, x+1, y);
140             }
141             if(dist[x][y][up]<INF && dist[x+1][y][down] > dist[x][y][up] + (1-tag[x][y][up])*g[x][y][down] + 2*g[x][y][down]  )   //上-下
142             {
143                 dist[x+1][y][down] = dist[x][y][up] + (1-tag[x][y][up])*g[x][y][down] + 2*g[x][y][down];
144                 tag[x+1][y][down]=1;
145                 isinq(que, x+1, y);
146             }
147             isend(x+1,y);
148         }
149         if(ok(x,y-1) && g[x][y][left] )  //左3
150         {
151             if(dist[x][y][left]<INF && dist[x][y-1][left] > dist[x][y][left] + g[x][y][left]  )  //左->左
152             {
153                 dist[x][y-1][left] = dist[x][y][left] + g[x][y][left];
154                 tag[x][y-1][left]=0;
155                 isinq(que, x, y-1);
156             }
157             if(dist[x][y][down]<INF && dist[x][y-1][left] > dist[x][y][down] + (1-tag[x][y][down])*g[x][y][up] + 2*g[x][y][left] )  //下->左
158             {
159                 dist[x][y-1][left] = dist[x][y][down] + (1-tag[x][y][down])*g[x][y][up] + 2*g[x][y][left];
160                 tag[x][y-1][left]=1;
161                 isinq(que, x, y-1);
162             }
163             if(dist[x][y][up]<INF && dist[x][y-1][left] > dist[x][y][up] + (1-tag[x][y][up])*g[x][y][down] + 2*g[x][y][left] )  //上->左
164             {
165                 dist[x][y-1][left] = dist[x][y][up] + (1-tag[x][y][up])*g[x][y][down] + 2*g[x][y][left];
166                 tag[x][y-1][left]=1;
167                 isinq(que, x, y-1);
168             }
169             if(dist[x][y][right]<INF && dist[x][y-1][left] > dist[x][y][right] + (1-tag[x][y][right])*g[x][y][left] + 2*g[x][y][left]   )   //右-左
170             {
171                 dist[x][y-1][left] = dist[x][y][right] + (1-tag[x][y][right])*g[x][y][left] + 2*g[x][y][left];
172                 tag[x][y-1][left]=1;
173                 isinq(que, x, y-1);
174             }
175             isend(x,y-1);
176         }
177         if(ok(x,y+1) && g[x][y][right] )  //右1
178         {
179             if(dist[x][y][right]<INF && dist[x][y+1][right] > dist[x][y][right] + g[x][y][right]  )  //右->右
180             {
181                 dist[x][y+1][right] = dist[x][y][right] + g[x][y][right];
182                 tag[x][y+1][right]=0;
183                 isinq(que, x, y+1);
184             }
185             if(dist[x][y][down]<INF && dist[x][y+1][right] > dist[x][y][down] + (1-tag[x][y][down])*g[x][y][up] + 2*g[x][y][right] )  //下->右
186             {
187                 dist[x][y+1][right] = dist[x][y][down] + (1-tag[x][y][down])*g[x][y][up] + 2*g[x][y][right];
188                 tag[x][y+1][right]=1;
189                 isinq(que, x, y+1);
190             }
191             if(dist[x][y][up]<INF && dist[x][y+1][right] > dist[x][y][up] + (1-tag[x][y][up])*g[x][y][down] + 2*g[x][y][right] )  //上->右
192             {
193                 dist[x][y+1][right] = dist[x][y][up] + (1-tag[x][y][up])*g[x][y][down] + 2*g[x][y][right];
194                 tag[x][y+1][right]=1;
195                 isinq(que, x, y+1);
196             }
197 
198             if(dist[x][y][left]<INF && dist[x][y+1][right] > dist[x][y][left] + (1-tag[x][y][left])*g[x][y][right] + 2*g[x][y][right] ) //左-右
199             {
200                 dist[x][y+1][right] = dist[x][y][left] + (1-tag[x][y][left])*g[x][y][right] + 2*g[x][y][right];
201                 tag[x][y+1][right]=1;
202                 isinq(que, x, y+1);
203             }
204             isend(x,y+1);
205         }
206     }
207 }
208 
209 
210 int cal()
211 {
212     memset(dist, 0x7f, sizeof(dist));
213     memset(tag, 0, sizeof(tag));
214     memset(inq, 0, sizeof(inq));
215     ans=INF;
216     spfa();
217     return ans==INF? 0: ans;
218 }
219 
220 
221 int main()
222 {
223     //freopen("input.txt", "r", stdin);
224     int Case=0;
225     while(scanf("%d %d %d %d %d %d", &n, &m, &sx, &sy, &ex, &ey), n+m+sx+sy+ex+ey)
226     {
227         memset(g, 0, sizeof(g));
228         for(int x=1; x<n; x++)  //行号
229         {
230             for(int y=1; y<m; y++)  //列号
231             {
232                 scanf("%d", &a);
233                 g[x][y][right]=a;   //右边1
234                 g[x][y+1][left]=a; //左边3
235             }
236             for(int y=1; y<=m; y++)
237             {
238                 scanf("%d", &a);
239                 g[x][y][down]=a;   //下2
240                 g[x+1][y][up]=a; //上0
241             }
242         }
243         for(int y=1; y<m; y++)
244         {
245             scanf("%d", &a);
246             g[n][y][right]=a;   //右
247             g[n][y+1][left]=a; //左
248         }
249 
250         int aa=cal();
251         if(aa) printf("Case %d: %d\n",++Case, aa);
252         else    printf("Case %d: Impossible\n", ++Case);
253     }
254     return 0;
255 }
WA代码
```

