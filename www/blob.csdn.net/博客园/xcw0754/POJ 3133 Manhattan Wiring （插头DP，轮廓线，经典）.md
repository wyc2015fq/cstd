# POJ  3133 Manhattan Wiring （插头DP，轮廓线，经典） - xcw0754 - 博客园
# [POJ  3133 Manhattan Wiring （插头DP，轮廓线，经典）](https://www.cnblogs.com/xcw0754/p/4795983.html)
题意：给一个n*m的矩阵，每个格子中有1个数，可能是0或2或3，出现2的格子数为2个，出现3的格子数为2个，要求将两个2相连，两个3相连，求不交叉的最短路（起终点只算0.5长，其他算1）。
思路：
　　这题与普通插头DP有些区别了，就是要求最短路时，而且还要同时两条不相交的最短路。一开始看还是感觉挺难的，因为每个0格子还得考虑两种线，分别是2线和3线，而且还不能出现回路的，于是就想用之前的模板，括号表示法，再加上1个位来表示2/3线，即共3个位来表示一个插头信息，但是写了才发现，括号表示不了，比如仅让右括号进2/3的格子时，如果2/3的格子在最左边时就不行了，同理左括号也不行，并不是真不行，是有点难搞。
　　于是盯着刘汝佳的书看，仅需3进制就能实现，这么神奇？只需要标记是2/3线和无线，这3种而已，这里有个问题是“如果产生回路了，怎么办”？由于我们求得是最短路，如果有一个状态是出现回路的，那么这个回路肯定是不必要的，由于在g[i][j]==0时，还可以跳过这些格子，所以肯定也有一个状态也是一样的，只是没有了回路，而且路程更短。总之，如果出现回路，在取最小值时，会自动被代替掉了，因为同样的状态总是会有更短的。
```
1 //#include <bits/stdc++.h>
  2 #include <iostream>
  3 #include <cstdio>
  4 #include <cstring>
  5 #define pii pair<int,int>
  6 #define INF 0x3f3f3f3f
  7 #define LL long long
  8 using namespace std;
  9 const int N=20;
 10 LL ans;
 11 char g[N][N];
 12 int cur, n, m, ex, ey;
 13 struct Hash_Map
 14 {
 15     static const int mod=12357;
 16     static const int NN=20010;
 17     int head[mod];      //桶指针
 18     int next[NN];        //记录链的信息
 19     LL  status[NN];      //状态
 20     LL  value[NN];       //状态对应的DP值。
 21     int size;
 22 
 23     void clear()    //清除哈希表中的状态
 24     {
 25         memset(head, -1, sizeof(head));
 26         size = 0;
 27     }
 28 
 29     void insert(LL st, LL val)  //插入状态st的值为val
 30     {
 31         int h = st%mod;
 32         for(int i=head[h]; i!=-1; i=next[i])
 33             if(status[i] == st) //这个状态已经存在，累加进去。
 34             {
 35                 value[i] =min(value[i], val);
 36                 return ;
 37             }
 38         status[size]= st;           //找不到状态st，则插入st。
 39         value[size] = val;
 40         next[size] = head[h] ;      //新插入的元素在队头
 41         head[h] = size++;
 42     }
 43 }hashmap[2];
 44 
 45 inline int getbit(LL s,int pos)   //取出状态s的第pos个插头
 46 {
 47     return (s>>2*pos)&3;
 48 }
 49 inline int setbit(LL s,int pos,int bit)   //将状态s的第pos个插头设置为bit
 50 {
 51     if(s&(1<<2*pos ))     s^=1<<(2*pos);
 52     if(s&(1<<(2*pos+1)))  s^=1<<(2*pos+1);
 53     return (s|(bit<<2*pos));
 54 }
 55 
 56 void DP(int i,int j)
 57 {
 58     for(int k=0; k<hashmap[cur^1].size; k++)
 59     {
 60         LL s=hashmap[cur^1].status[k];
 61         LL v=hashmap[cur^1].value[k];
 62         LL t=(setbit(s,j,0)&setbit(s,j+1,0));
 63         int R=getbit(s,j), D=getbit(s,j+1);
 64 
 65         if(g[i][j]==1)    //障碍格子
 66         {
 67             if( R==0 && D==0 )    hashmap[cur].insert(s, v);
 68             continue ;
 69         }
 70         if(R&&D)    //相同就能合并，具体看思路
 71         {
 72             if( g[i][j]==0 &&  R==D )      hashmap[cur].insert(t,v+1);
 73         }
 74         else if(R||D)   //单括号:要么需要延续，要么是到达2或3
 75         {
 76             int big=R+D;
 77             if(g[i][j]==2 && big==1 )    hashmap[cur].insert(t, v+1);   //到达
 78             if(g[i][j]==3 && big==2 )    hashmap[cur].insert(t, v+1);
 79             if(g[i][j]==0 && i+1<n )     hashmap[cur].insert(setbit(t,j,big),  v+1);    //延续
 80             if(g[i][j]==0 && j+1<m )     hashmap[cur].insert(setbit(t,j+1,big),v+1);
 81         }
 82         else    //无括号
 83         {
 84             if(g[i][j]==2)
 85             {
 86                 if(i+1<n)   hashmap[cur].insert( setbit(s,j,1),   v+1 );
 87                 if(j+1<m)   hashmap[cur].insert( setbit(s,j+1,1), v+1 );
 88             }
 89             if(g[i][j]==3)
 90             {
 91                 if(i+1<n)   hashmap[cur].insert( setbit(s,j,2),   v+1 );
 92                 if(j+1<m)   hashmap[cur].insert( setbit(s,j+1,2), v+1 );
 93             }
 94             if(g[i][j]==0)
 95             {
 96                 hashmap[cur].insert(s,v);   //跳过此格子
 97                 if(i+1<n && j+1<m)    hashmap[cur].insert(setbit(t,j,1)|setbit(t,j+1,1), v+1 );
 98                 if(i+1<n && j+1<m)    hashmap[cur].insert(setbit(t,j,2)|setbit(t,j+1,2), v+1 );
 99 
100             }
101         }
102 
103     }
104 
105 }
106 
107 void cal()
108 {
109     for(int i=0; i<n; i++)
110     {
111         cur^=1;
112         hashmap[cur].clear();
113         for(int j=0; j<hashmap[cur^1].size; j++)    //新行，需要左移一下状态。
114             hashmap[cur].insert( hashmap[cur^1].status[j]<<2, hashmap[cur^1].value[j] );
115         for(int j=0; j<m; j++)
116         {
117             cur^=1;
118             hashmap[cur].clear();
119             DP(i,j);
120         }
121     }
122 }
123 
124 LL print()
125 {
126     for(int i=0; i<hashmap[cur].size; i++)
127         if(hashmap[cur].status[i]==0)
128             return hashmap[cur].value[i];
129     return 0;
130 }
131 
132 int main()
133 {
134     //freopen("input.txt", "r", stdin);
135     while(scanf("%d%d",&n,&m), n+m)
136     {
137         for(int i=0; i<n; i++)
138             for(int j=0; j<m; j++)
139                 scanf("%d",&g[i][j]);
140 
141         cur=0;
142         hashmap[cur].clear();
143         hashmap[cur].insert(0, 0);
144         cal();
145         ans=print();
146         cout<<(ans==0?0:ans-2)<<endl;
147     }
148     return 0;
149 }
AC代码
```
　　自己的想法是这样的：用括号表示法来表示插头，由于有两类线，所以增加一个位来区分2/3线，共3个位表示一个插头信息，所以用1和2表示2线的括号，5和6表示3线的括号，0表示无括号。当遇到2/3格子时，要么只接受单个插头进来，要么只有单方向出去，而且出去的可以是左/右括号，这样才能保证连起来；遇到0空格时，可以不走，也可以产生一对括号，分别是(1,2)和(5,6)这两种。其他的基本一样，实践证明还是可以得，只不过时间复杂度稍微就高了，因为多了很多无用的状态。而且有个注意点是，当同向括号'(('出现时，仍然要合并，而且要改另一个右括号为(，但是这'(('中可能有1个是来自2/3格子的，它仅是单向的线，所以有可能是找不到另一半的，如果找不到另一半就忽略，找到了就改（这说明不是来自2/3格子）。
　　注：本代码在POJ上是TLE，但是在UVA或UVALive都是可以过的。
```
1 //#include <bits/stdc++.h>
  2 #include <iostream>
  3 #include <cstdio>
  4 #include <cstring>
  5 #define pii pair<int,int>
  6 #define INF 0x3f3f3f3f
  7 #define LL long long
  8 using namespace std;
  9 const int N=15;
 10 int n, m, cur, g[N][N];
 11 struct Hash_Map
 12 {
 13     static const int mod=12357;
 14     static const int NN=200100;
 15     int head[mod];       //桶指针
 16     int next[NN];        //记录链的信息
 17     LL  status[NN];      //状态
 18     LL  value[NN];       //状态对应的DP值。
 19     int size;
 20 
 21     void clear()    //清除哈希表中的状态
 22     {
 23         memset(head, -1, sizeof(head));
 24         size = 0;
 25     }
 26 
 27     void insert(LL st, LL val)  //插入状态st的值为val
 28     {
 29         int h = st%mod;
 30         for(int i=head[h]; i!=-1; i=next[i])
 31         {
 32             if(status[i] == st) //这个状态已经存在，累加进去。
 33             {
 34                 value[i] = min(value[i], val);
 35                 return ;
 36             }
 37         }
 38 
 39         status[size]= st;           //找不到状态st，则插入st。
 40         value[size] = val;
 41         next[size] = head[h] ;      //新插入的元素在队头
 42         head[h] = size++;
 43     }
 44 }hashmap[2];
 45 
 46 LL setbit(LL s,int pos,int bit)   //将状态s的第pos个插头设置为bit
 47 {
 48     //if(s&(1<<3*pos ))     s^=(LL)1<<(3*pos);
 49     //if(s&(1<<(3*pos+1)))  s^=(LL)1<<(3*pos+1);
 50     //if(s&(1<<(3*pos+2)))  s^=(LL)1<<(3*pos+2);
 51     s&=~(7<<3*pos);
 52     return (s|((LL)bit<<3*pos));
 53 }
 54 
 55 int getbit(LL s,int pos)
 56 {
 57     return ((s>>3*pos)&7);
 58 }
 59 
 60 LL Fr(LL s,int pos,int bit,int rep)   //寻找状态s的第pos个插头对应的右括号。
 61 {
 62     int cnt=0;
 63     for(pos+=2; pos<m; pos++)
 64     {
 65         if(getbit(s, pos)==rep)   cnt++;
 66         if(getbit(s, pos)==bit)   cnt--;
 67         if(cnt==-1)  return setbit(s, pos, rep);
 68     }
 69     return s;
 70 }
 71 LL Fl(LL s,int pos,int bit,int rep)   //寻找状态s的第pos个插头对应的左括号。
 72 {
 73     int cnt=0;
 74     for(pos--; pos>=0; pos--)
 75     {
 76         if(getbit(s, pos)==rep)   cnt++;
 77         if(getbit(s, pos)==bit)        cnt--;
 78         if( cnt==-1)    return setbit(s, pos, rep);
 79     }
 80     return s;   //注意点，有可能找不到对应的另一半括号,Fr同理。
 81 }
 82 void DP(int i,int j)
 83 {
 84     for(int k=0; k<hashmap[cur^1].size; k++)
 85     {
 86         LL s=hashmap[cur^1].status[k];
 87         LL v=hashmap[cur^1].value[k];
 88         LL t=(setbit(s,j,0)&setbit(s,j+1,0));
 89         int R=getbit(s, j), D=getbit(s, j+1);
 90         if( g[i][j]==1 )    //障碍
 91         {
 92             if(R==0 && D==0)    hashmap[cur].insert(s,v);
 93             continue;
 94         }
 95         if(R&&D)
 96         {
 97             if( g[i][j]==2 || g[i][j]==3 || (R&4)!=(D&4) )  continue;   //只能容许1条线进来
 98             if( R==D )
 99             {
100                 if(R==1)      t=Fr(t,j,2,1);  //左
101                 if(R==2)      t=Fl(t,j,1,2);  //右
102                 if(R==5)      t=Fr(t,j,6,5);
103                 if(R==6)      t=Fl(t,j,5,6);
104                 hashmap[cur].insert(t, v-1); //当前块计算了2次
105             }
106             else if( R==2 && D==1 )   hashmap[cur].insert(t,v-1);
107             else if( R==6 && D==5 )   hashmap[cur].insert(t,v-1);
108         }
109         else if(R||D)   //单括号
110         {
111             R=max(R,D);
112             if( g[i][j]==2 && (R==1||R==2) )      hashmap[cur].insert(t, v);
113             if( g[i][j]==3 && (R==5||R==6) )      hashmap[cur].insert(t, v);
114             if( g[i][j]==0 && j+1<m )    hashmap[cur].insert(setbit(t,j+1,R), v+1); //延续
115             if( g[i][j]==0 && i+1<n )    hashmap[cur].insert(setbit(t,j,R), v+1);
116         }
117         else    //无括号
118         {
119             if(g[i][j]==2)  //起终点格子：只可以单线出。
120             {
121                 if(i+1<n) hashmap[cur].insert(setbit(t,j,1) ,  v+2);     //下
122                 if(i+1<n) hashmap[cur].insert(setbit(t,j,2) ,  v+2);     //下
123                 if(j+1<m) hashmap[cur].insert(setbit(t,j+1,1), v+2);     //右
124                 if(j+1<m) hashmap[cur].insert(setbit(t,j+1,2), v+2);     //右
125             }
126             if(g[i][j]==3)
127             {
128                 if(i+1<n) hashmap[cur].insert(setbit(t,j,5) ,  v+2);   //下
129                 if(i+1<n) hashmap[cur].insert(setbit(t,j,6) ,  v+2);   //下
130                 if(j+1<m) hashmap[cur].insert(setbit(t,j+1,5), v+2);   //右
131                 if(j+1<m) hashmap[cur].insert(setbit(t,j+1,6), v+2);   //右
132             }
133             if(g[i][j]==0)
134             {
135                 hashmap[cur].insert(s, v);  //本格子不走
136                 if(i+1<n && j+1<m)
137                 {
138                     hashmap[cur].insert(setbit(t,j,1)|setbit(t,j+1,2), v+3); //2线角头
139                     hashmap[cur].insert(setbit(t,j,5)|setbit(t,j+1,6), v+3); //3线角头
140                 }
141             }
142         }
143     }
144 }
145 
146 
147 
148 void cal()
149 {
150     for(int i=0; i<n; i++)
151     {
152         cur^=1;
153         hashmap[cur].clear();
154         for(int j=0; j<hashmap[cur^1].size; j++)    //新行，需要左移一下状态。
155             hashmap[cur].insert( hashmap[cur^1].status[j]<<3, hashmap[cur^1].value[j] );
156         for(int j=0; j<m; j++)
157         {
158             cur^=1;
159             hashmap[cur].clear();
160             DP(i,j);
161         }
162     }
163 }
164 LL print()
165 {
166     for(int i=0; i<hashmap[cur].size; i++)
167         if(hashmap[cur].status[i]==0)
168             return hashmap[cur].value[i];
169     return 0;
170 }
171 
172 int main()
173 {
174     freopen("input.txt", "r", stdin);
175     while(scanf("%d%d",&n,&m), n+m)
176     {
177         for(int i=0; i<n; i++)
178             for(int j=0; j<m; j++)
179                 scanf("%d",&g[i][j]);
180 
181         cur=0;
182         hashmap[cur].clear();
183         hashmap[cur].insert(0, 0);
184         cal();
185         LL ans=print();
186         printf("%lld\n",ans==0?0:ans-2);
187     }
188     return 0;
189 }
AC代码
```

