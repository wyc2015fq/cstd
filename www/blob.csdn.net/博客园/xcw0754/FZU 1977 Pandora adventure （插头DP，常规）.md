# FZU  1977 Pandora adventure （插头DP，常规） - xcw0754 - 博客园
# [FZU  1977 Pandora adventure （插头DP，常规）](https://www.cnblogs.com/xcw0754/p/4789842.html)
题意：有一个n*m矩阵，其中有些格子必走，有些格子不可走，其他格子是可走也可不走，问有多少条哈密顿回路？
思路：
　　本来是一道很简单的题，代码写多了连白痴bug都查不出了，竟然用i>=ex&&j>=ey来判定最后一个必走点后面的点！明显是错的。
　　其实主要在选走的格子，那么有两种选择，“走”or“不走”，不走的话上一个格子的状态照搬过来。这样就没有了固定的终点了，因为终点可以是很多种情况（比如是选走点/必走点），那么只要是在ex和ey后面的格子（指的是(ex,ey)之后遍历的所有非障碍格子），都是可以作为终点的，只有在这些格子才可以将左右括号连起来（只统计但不插入）。其他的基本和常规的一样了，本次仍然用括号表示法，再过滤掉很多无效的状态，所以代码略长，但容易看。
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
 10 const int mod=12357;
 11 const int NN=100010;
 12 char g[N][N];
 13 LL ans;
 14 int cur, n, m, ex, ey, tx, ty;
 15 struct Hash_Map
 16 {
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
 32         for(int i=head[h]; ~i; i=next[i])
 33             if(status[i] == st) //这个状态已经存在，累加进去。
 34             {
 35                 value[i] += val;
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
 56 int Fr(LL s,int pos,int bit)   //寻找状态s的第pos个插头对应的右括号。
 57 {
 58     int cnt=0;
 59     for(pos+=2; pos<m; pos++)
 60     {
 61         if(getbit(s, pos)==3-bit)   cnt++;
 62         if(getbit(s, pos)==bit)     cnt--;
 63         if(cnt==-1)         return setbit(s, pos, 3-bit);
 64     }
 65 }
 66 int Fl(LL s,int pos,int bit)   //寻找状态s的第pos个插头对应的左括号。
 67 {
 68     int cnt=0;
 69     for(pos--; pos>=0; pos--)
 70     {
 71         if(getbit(s, pos)==3-bit)  cnt++;
 72         if(getbit(s, pos)==bit)    cnt--;
 73         if( cnt==-1)    return setbit(s, pos, 3-bit);
 74     }
 75 }
 76 
 77 void DP(int i,int j)    //非障碍空格
 78 {
 79     LL t;
 80     for(int k=0; k<hashmap[cur^1].size; k++)
 81     {
 82         LL s=hashmap[cur^1].status[k];
 83         LL v=hashmap[cur^1].value[k];
 84         int R=getbit(s,j), D=getbit(s,j+1);
 85         if(g[i][j]=='X')    //障碍格子
 86         {
 87             if( R==0 && D==0 )    hashmap[cur].insert(s, v);
 88             continue ;
 89         }
 90         if(R && D)  //两个括号
 91         {
 92             t=(setbit(s,j,0)&setbit(s,j+1,0));
 93             if(R==D)    //同个方向的括号
 94             {
 95                 if(R==1)    t=Fr(t, j, 2);  //要改
 96                 else        t=Fl(t, j, 1);
 97                 hashmap[cur].insert(t, v);
 98             }
 99             else if( R==2 && D==1 )        //不同的连通分量
100                 hashmap[cur].insert(t, v);
101             else if(t==0 && ( i>ex || (i==ex && j>=ey)))    //注意点！
102                 ans+=v;
103         }
104         else if(R || D)     //仅1个括号
105         {
106             if(R)
107             {
108                 if(i+1<n && g[i+1][j]!='X')   hashmap[cur].insert(s, v);
109                 if(j+1<m && g[i][j+1]!='x')   hashmap[cur].insert(setbit(setbit(s,j,0), j+1, R), v);
110             }
111             else
112             {
113                 if(j+1<m && g[i][j+1]!='X')   hashmap[cur].insert(s, v);
114                 if(i+1<n && g[i+1][j]!='X')   hashmap[cur].insert(setbit(setbit(s,j+1,0), j, D), v);
115             }
116         }
117         else                //无括号
118         {
119             if( g[i][j+1]!='X' && g[i+1][j]!='X' && j+1<m && i+1<n )    //新括号
120                 hashmap[cur].insert( setbit(s,j,1)|setbit(s,j+1,2), v);
121             if( g[i][j]=='*' )      //此点可不走
122                 hashmap[cur].insert( s, v);
123         }
124     }
125 }
126 
127 void cal()
128 {
129     for(int i=0; i<n; i++)
130     {
131         cur^=1;
132         hashmap[cur].clear();
133         for(int j=0; j<hashmap[cur^1].size; j++)    //新行，需要左移一下状态。
134             hashmap[cur].insert( hashmap[cur^1].status[j]<<2, hashmap[cur^1].value[j] );
135         for(int j=0; j<m; j++)
136         {
137             cur^=1;
138             hashmap[cur].clear();
139             DP(i,j);
140             if(i==tx && j==ty)    return ;  //最后的有效点
141         }
142     }
143 }
144 
145 
146 int main()
147 {
148     //freopen("input.txt", "r", stdin);
149     int t, Case=0;
150     cin>>t;
151     while(t--)
152     {
153         tx=ty=ex=ey=-1;
154         ans=cur=0;
155         memset(g, 'X', sizeof(g));
156         scanf("%d%d",&n,&m);
157         for(int i=0; i<n; i++)      //输入矩阵
158         {
159             for(int j=0; j<m; j++)
160             {
161                 char c=getchar();
162                 if(c=='X'||c=='*'||c=='O')
163                 {
164                     g[i][j]=c;
165                     if( c=='O' )    ex=i,ey=j;  //必走格
166                     if( c!='X' )    tx=i,ty=j;  //终点格
167                 }
168                 else    j--;
169             }
170         }
171 
172         hashmap[cur].clear();
173         hashmap[cur].insert(0, 1);  //初始状态
174         cal();
175         cout<<"Case "<<++Case<<": "<<ans<<endl;
176     }
177     return 0;
178 }
AC代码
```

