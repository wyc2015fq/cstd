# The 12th Zhejiang Provincial Collegiate Programming Contest - I Earthstone Keeper浙江省赛 - xcw0754 - 博客园
# [The 12th Zhejiang Provincial Collegiate Programming Contest - I Earthstone Keeper浙江省赛](https://www.cnblogs.com/xcw0754/p/4456353.html)
题目:http://acm.zju.edu.cn/onlinejudge/showContestProblem.do?problemId=5501
思路：DFS，用了递归就溢出，所以可能得用非递归的。把所有可能到达终点的可能路径都计算，最后比较找最佳。限制条件很多，要细打细算。很烦，不想改了再试，写了很久了
```
1 #include <iostream>
  2 #include <cstdio>
  3 #include <string>
  4 #include <map>
  5 #include <deque>
  6 #include <stack>
  7 #include <cstring>
  8 #include <algorithm>
  9 using namespace std;
 10 char dun[505][505];
 11 bool vis[505][505];
 12 struct node
 13 {
 14     int x,y;
 15     int peln,path;  //惩罚/ 第几层
 16     int s[55];     //遇到的陷阱,26开始就是A
 17 };
 18 stack<node> que;
 19 deque<node> anslist;
 20 
 21 int stax, stay;
 22 int tarx, tary;
 23 int n, m;
 24 int fat, mins;
 25 
 26 void init(node &tmp)        //初始化怪物标记s
 27 {
 28     for(int i=0; i<55; i++)
 29         tmp.s[i]='0';
 30 }
 31 bool tar(node tmp)      //判断是否是目的地
 32 {
 33     if( tmp.x==tarx && tmp.y==tary )
 34         return true;
 35     else
 36         return false;
 37 }
 38 bool cango(node tmp)      //判断能不能走
 39 {
 40     if(tmp.x>=0&&tmp.x<n&&tmp.y>=0&&tmp.y<m )
 41     {
 42         if( dun[tmp.x][tmp.y]=='#' )
 43             return false;
 44         else
 45             return true;
 46     }
 47     else
 48         return false;
 49 }
 50 void adj_mon(node &tmp)   //计算怪物惩罚
 51 {
 52     char c1='0',c2='0',c3='0',c4='0', c5='0';       //走过可能干掉3只怪物
 53 
 54     if( isupper( dun[tmp.x][tmp.y] )  )  //是怪物
 55     {
 56         c1=dun[tmp.x][tmp.y];
 57     }
 58 
 59     node node1=tmp;
 60     node1.x-=1;
 61     if( cango(node1)&&isupper( dun[tmp.x-1][tmp.y] )  )  //是怪物
 62     {
 63         c2=dun[tmp.x-1][tmp.y];
 64         //cout<<"begin"<<endl;
 65     }
 66 
 67     node1=tmp;
 68     node1.x+=1;
 69     if( cango(node1)&&isupper( dun[tmp.x+1][tmp.y] )  )  //是怪物
 70     {
 71         c3=dun[tmp.x+1][tmp.y];
 72         //cout<<"begin"<<endl;
 73     }
 74 
 75     node1=tmp;
 76     node1.y+=1;
 77     if( cango(node1)&&isupper( dun[tmp.x][tmp.y+1] )  )  //是怪物
 78     {
 79         c3=dun[tmp.x][tmp.y+1];
 80         //cout<<"begin"<<endl;
 81     }
 82 
 83     node1=tmp;
 84     node1.y-=1;
 85     if( cango(node1)&&isupper( dun[tmp.x][tmp.y-1] )  )  //是怪物
 86     {
 87         //cout<<"begin"<<endl;
 88         c5=dun[tmp.x][tmp.y-1];
 89     }
 90 
 91 //*************************************
 92     if(c1!='0'&&tmp.s[ c1-'A'+26 ]=='0')
 93     {
 94         tmp.s[ c1-'A'+26 ]=true;
 95         tmp.peln+=( c1-'A'+1 );
 96         //cout<<"begin"<<endl;
 97     }
 98 
 99     if(c2!='0'&&tmp.s[ c2-'A'+26 ]=='0')
100     {
101         tmp.s[ c2-'A'+26 ]=true;
102         tmp.peln+=( c2-'A'+1 );
103         //cout<<"begin"<<endl;
104     }
105 
106     if(c3!='0'&&tmp.s[ c3-'A'+26 ]=='0')
107     {
108         tmp.s[ c3-'A'+26 ]=true;
109         tmp.peln+=(c3-'A'+1);
110         //cout<<"begin"<<endl;
111     }
112 
113     if(c4!='0'&&tmp.s[ c4-'A'+26 ]=='0')
114     {
115         tmp.s[ c4-'A'+26 ]=true;
116         tmp.peln+=(c4-'A'+1);
117         //cout<<"begin"<<endl;
118     }
119 
120     if(c5!='0'&&tmp.s[ c5-'A'+26 ]=='0')
121     {
122         tmp.s[ c5-'A'+26 ]=true;
123         tmp.peln+= c5-'A'+1;
124         //cout<<"begin"<<endl;
125     }
126 
127 
128 }
129 void trap(node &tmp)        //计算陷阱惩罚
130 {
131     if( islower(dun[tmp.x][tmp.y]) )   //陷阱
132         tmp.peln += (dun[tmp.x][tmp.y] - 'a'+1);
133 }
134 void cal_pel(node &tmp)      //计算惩罚
135 {
136     trap(tmp);
137     adj_mon(tmp);
138 }
139 
140 int bfs(node tmp) 
141 {
142     //判断四周有没有能进盏的
143     node node1=tmp;
144     node1.x-=1;
145     if(cango(node1))    //合法
146     {
147 
148         cal_pel(node1);
149         node1.path++;
150         if(tar(node1))
151             anslist.push_back(node1);
152         else if(vis[node1.x][node1.y]==false)                           //非目的
153         {
154             que.push(node1);
155             vis[node1.x][node1.y]=true;
156             bfs(node1);
157             que.pop();
158             vis[node1.x][node1.y]=false;
159 
160         }
161 
162     }
163 
164     node1=tmp;
165     node1.x+=1;
166     if(cango(node1))    //合法
167     {
168         cal_pel(node1); node1.path++;
169         if(tar(node1))
170             anslist.push_back(node1);
171         else if(vis[node1.x][node1.y]==false)                 //非目的
172         {
173             que.push(node1);
174             vis[node1.x][node1.y]=true;
175             bfs(node1) ;
176             que.pop();
177             vis[node1.x][node1.y]=false;
178         }
179     }
180 
181     node1=tmp;
182     node1.y-=1;
183     if(cango(node1))    //合法
184     {
185         cal_pel(node1); node1.path++;
186         if(tar(node1))
187             anslist.push_back(node1);
188         else if(vis[node1.x][node1.y]==false)                 //非目的
189         {
190             que.push(node1);
191             vis[node1.x][node1.y]=true;
192             bfs(node1) ;
193             que.pop();
194             vis[node1.x][node1.y]=false;
195         }
196     }
197 
198     node1=tmp;
199     node1.y+=1;
200     if(cango(node1))    //合法
201     {
202         cal_pel(node1); node1.path++;
203         if(tar(node1))
204             anslist.push_back(node1);
205         else if(vis[node1.x][node1.y]==false)               //非目的
206         {
207             que.push(node1);
208             vis[node1.x][node1.y]=true;
209             bfs(node1) ;
210             que.pop();
211             vis[node1.x][node1.y]=false;
212         }
213 
214     }
215     return 0;
216 }
217 
218 int main()
219 {
220     //freopen("input.txt", "r", stdin);
221     int t;
222     cin>>t;
223     while(t--)
224     {
225         anslist.clear();
226         memset(dun, 0, sizeof(dun));
227         memset(vis, 0, sizeof(vis));
228 
229         cin>>n>>m;
230         cin>>stax>>stay>>tarx>>tary;
231         stax-=1;stay-=1;tarx-=1;tary-=1;
232 
233 
234         for(int i=0; i<n; i++)
235             for(int j=0; j<m; j++)
236                 cin>>dun[i][j];
237 
238 
239         node tmp;
240         init(tmp);
241         tmp.x=stax; tmp.y=stay; tmp.peln=0; tmp.path=0; //起点
242         vis[tmp.x][tmp.y]=true;
243         que.push(tmp); //先进队
244         bfs(que.top());
245 
246         deque<node>::iterator it=anslist.begin();
247         int min_path=1247483647;
248         int min_pel=1247483647;
249         for( ; it!=anslist.end(); it++)
250         {
251             //cout<<it->peln<<" "<<it->path<<endl;
252 
253             if(it->peln<=min_pel&&it->path<=min_path)
254             {
255                 min_pel=it->peln;
256                 min_path=it->path;
257             }
258 
259 
260         }
261         cout<<min_pel<<" "<<min_path<<endl;
262 
263 
264 
265 
266     }
267 
268 
269     return 0;
270 }
递归式的，seg错
```

