# UVA 10572 Black & White （状压DP） - xcw0754 - 博客园
# [UVA 10572 Black & White （状压DP）](https://www.cnblogs.com/xcw0754/p/4803144.html)
题意：有一个n*m的矩阵，其中部分格子已经涂黑，部分涂白，要求为其他格子也上黑/白色，问有多少种涂法可以满足一下要求：
　　（1）任意2*2的子矩阵不可以同色。
　　（2）所有格子必须上色。
　　（3）只能有两个连通分量（即1黑1白）。
　　注：1<n,m<9。若能满足，顺便输出任一种涂色方法。
思路：　　
　　本来题也不难，只是刚开始写最小表示法，加上这题的难度，所以搞非常久。注：本题用最小表示法比较好。
　　大概逻辑如下：
　　　　if g[i][j]非首列且2*2子矩阵同色　　then　　非法状态;
　　　　if 上格连通分量不会丢失.　　then　　合格状态;
　　然后对矩阵最后的两个格子特判如下（合法状态指的是对当前格子合法）：
　　1)倒数第2格子的判断：
　　　　（1）if g[i][j]与上格同色　　then　　合法状态；
　　　　（2） if 上格的连通分量编号在轮廓线上并不是唯一　　then　　合法状态；
　　　　（3） if 轮廓线上所有格子颜色相同　　then　　合法状态；
　　2)倒数第1格子的判断：
　　　　（1）if 先尝试上面3种情况，若合法，再继续。
　　　　（2）if n=2 or 最后一个格子与上/左格（任一个以上都行）连通　　then　　合法状态；
　　当扫完所有格子后，将所有合法中连通分量最多为2个的都是符合要求的。路径记录只需要用一个64位整型来记录即可，较简单。
```
1 #include <bits/stdc++.h>
  2 #include <iostream>
  3 #include <cstdio>
  4 #include <cstring>
  5 #define pii pair<int,int>
  6 #define INF 0x3f3f3f3f
  7 #define LL long long
  8 #define ULL unsigned long long
  9 using namespace std;
 10 const int N=10;
 11 int g[N][N], code[N],color[N], cur, n, m;
 12 int num[N];         //用于最小表示法
 13 char str[N][N];     //用于路径输出
 14 struct Hash_Map
 15 {
 16     static const int mod=12357;
 17     static const int NN=20010;
 18     int head[mod];       //桶指针
 19     int next[NN];        //记录链的信息
 20     LL  status[NN];      //状态
 21     LL  value[NN];       //状态对应的DP值
 22     ULL  path[NN];       //路径的状态
 23     int size;
 24 
 25     void clear()
 26     {
 27         memset(head, -1, sizeof(head));
 28         size = 0;
 29     }
 30 
 31     void insert(LL st, LL val,ULL p)
 32     {
 33         int h = st%mod;
 34         for(int i=head[h]; i!=-1; i=next[i])
 35         {
 36             if(status[i] == st)
 37             {
 38                 value[i] += val;
 39                 path[i] = p;    //任一路径
 40                 return ;
 41             }
 42         }
 43         status[size]= st;
 44         value[size] = val;
 45         path[size] = p;
 46         next[size] = head[h] ;
 47         head[h] = size++;
 48     }
 49 }hashmap[2];
 50 
 51 LL encode()         //编码
 52 {
 53     memset(num,-1,sizeof(num));
 54     LL s=0;
 55     for(int i=m-1,cnt=0; i>=0; i--)
 56     {
 57         if(num[code[i]]<0)  num[code[i]]=cnt++;
 58         s<<=4;
 59         s+=(num[code[i]]&7)+color[i]*8; //最高位为颜色，低3位为轮廓线
 60     }
 61     s<<=1;
 62     s+=color[m];  //color[m]放在最低位,方便处理。
 63     return s;
 64 }
 65 
 66 void decode(LL s)      //解码：1个颜色位+3个状态位表示一个格子状态
 67 {
 68     color[m+1]=(s&1);s>>=1;
 69     for(int i=1; i<=m; i++)  //解码到code[1~m]上面,就不用移动。
 70     {
 71         code[i]=(s&7);    s>>=3;
 72         color[i]=(s&1);   s>>=1;
 73     }
 74 }
 75 
 76 inline bool isunique()  //判断上格子的连通分量是否唯一。
 77 {
 78     for(int i=1; i<m; i++)    if( code[i]==code[m] )    return false;
 79     return true;
 80 }
 81 inline bool samecolor() //轮廓线上除了color[m]外，所有格子是否同色
 82 {
 83     for(int i=1; i<m; i++)    if(color[i]!=color[i-1])    return false;
 84     return true;
 85 }
 86 
 87 void comb(int c,int j)  //合并连通分量
 88 {
 89     if( c==color[m] )    code[0]=code[m]; //与上同色
 90     else                code[0]=7;
 91     if(j>0 && c==color[1] )              //与左同色
 92     {
 93         int t=code[0];
 94         for(int i=0; i<m; i++)    if( code[i]==t )    code[i]=code[1];
 95     }
 96 }
 97 
 98 void trycover(int c,LL v,ULL p,int i,int j) //尝试对g[i,j]涂c色
 99 {
100     if( j>0 && color[1]==c && color[m+1]==c && color[m]==c )   return ;    //2*2同色
101     color[0]=c&1;
102     if(i+1==n && j+2==m)    //最后2格特处理
103     {
104         if( c==color[m] || !isunique() || samecolor() )
105         {
106             comb(c,j);
107             hashmap[cur].insert(encode(), v, p);
108         }
109     }
110     else if(i+1==n && j+1==m)//最后1格特处理
111     {
112         if( c==color[m] || !isunique() || samecolor() )
113         {
114             if(i<2|| c==color[m]||c==color[1])  //如果是m=2的矩阵，需要特别注意最后两个格子。
115             {
116                 comb(c,j);
117                 hashmap[cur].insert(encode(), v, p);
118             }
119         }
120     }
121     else
122     {
123         if( c!=color[m] && isunique() ) return ;        //连通分量消失
124         comb(c,j);
125         hashmap[cur].insert(encode(), v, p);
126     }
127 }
128 
129 void cal()
130 {
131     hashmap[cur=0].clear();
132     for(int i=0; i<(1<<m); i++ )   //产生第1行的所有情况。
133     {
134         color[m]=0;
135         bool flag=1;
136         for(int j=0,t=i; j<m; j++,t>>=1)  //判断是否冲突。
137         {
138             color[j]=(t&1);
139             if(g[0][m-1-j]<2 && g[0][m-1-j]!=color[j])   flag=0;    //注意点
140         }
141         if(flag)
142         {
143             code[m-1]=0;
144             for(int j=m-2,up=0; j>=0; j--)
145                 if(color[j]==color[j+1])    code[j]=code[j+1];
146                 else    code[j]=++up;
147             hashmap[0].insert(encode(), 1, i);
148         }
149     }
150 
151     for(int i=1; i<n; i++)  //穷举剩下所有格子。
152     {
153         for(int j=0; j<m; j++)
154         {
155             cur^=1;
156             hashmap[cur].clear();
157             for(int k=0; k<hashmap[cur^1].size; k++)
158             {
159                 LL s=hashmap[cur^1].status[k];
160                 LL v=hashmap[cur^1].value[k];
161                 ULL p=hashmap[cur^1].path[k];
162                 decode(s);
163                 p<<=1;
164                 if( g[i][j]==2 )
165                 {
166                     trycover(0,v,p+0,i,j);
167                     trycover(1,v,p+1,i,j);
168                 }
169                 else    trycover(g[i][j],v,p+g[i][j],i,j);
170             }
171         }
172     }
173 }
174 
175 void print()    //答案及路径输出。
176 {
177     int ans=0;ULL mapp=0;
178     for(int i=0; i<hashmap[cur].size; i++)
179     {
180         int big=0;
181         decode(hashmap[cur].status[i]);
182         for(int y=1; y<=m; y++)    big=max(big, code[y]);   //求最大编号
183         if(big<=1)
184         {
185             mapp=hashmap[cur].path[i];
186             ans+=hashmap[cur].value[i];
187         }
188     }
189     printf("%d\n", ans);
190     if(ans)
191     {
192         memset(str,'\0',sizeof(str));
193         for(int i=n-1; i>=0; i--)
194         {
195             for(int j=m-1; j>=0; j--)
196             {
197                 if(mapp&1)  str[i][j]='#';
198                 else        str[i][j]='o';
199                 mapp>>=1;
200             }
201         }
202         for(int i=0; i<n; i++)    printf("%s\n",str[i]);
203     }
204 }
205 
206 int main()
207 {
208     freopen("input.txt", "r", stdin);
209     int t;cin>>t;
210     while(t--)
211     {
212         memset(g,0,sizeof(g));
213         scanf("%d%d", &n,&m);
214         for(int i=0; i<n; i++)
215         {
216             for(int j=0; j<m; j++)
217             {
218                 char c=getchar();
219                 if(c=='#')          g[i][j]=1;          //黑色
220                 else if(c=='o')     g[i][j]=0;
221                 else if(c=='.')     g[i][j]=2;
222                 else j--;
223             }
224         }
225         cal();print();printf("\n");
226     }
227     return 0;
228 }
AC代码
```

