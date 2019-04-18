# HDU 1964 Pipes （插头DP，变形） - xcw0754 - 博客园
# [HDU 1964 Pipes （插头DP，变形）](https://www.cnblogs.com/xcw0754/p/4790107.html)
题意：给一个n*m的矩阵，每个格子都是必走的，且无障碍格子，每对格子之间都有一个花费，问哈密顿回路的最小花费。
思路：
　　这个和Formula1差不多，只是求得是最小花费，这只需要修改一下DP值为花费就行了，主要是在创建新括号，以及延续一个插头的时候花费，因为可能上一个格子有多个状态都可以转移到本格子的同一状态，那么同一个格子中的同一状态的DP值取最小值即可。
```
1 #include <bits/stdc++.h>
  2 #include <iostream>
  3 #include <cstdio>
  4 #include <cstring>
  5 #define pii pair<int,int>
  6 #define INF 0x3f3f3f3f
  7 #define LL long long
  8 using namespace std;
  9 const int N=105;
 10 
 11 int g[N][N], cur, n, m;
 12 struct Hash_Map
 13 {
 14     static const int mod=12357;
 15     static const int NN=100010;
 16     int head[mod];      //桶指针
 17     int next[NN];        //记录链的信息
 18     LL  status[NN];      //状态
 19     LL  value[NN];       //状态对应的DP值。
 20     int size;
 21 
 22     void clear()    //清除哈希表中的状态
 23     {
 24         memset(head, -1, sizeof(head));
 25         size = 0;
 26     }
 27 
 28     void insert(LL st, LL val)  //插入状态st的值为val
 29     {
 30         int h = st%mod;
 31         for(int i=head[h]; ~i; i=next[i])
 32             if(status[i] == st) //这个状态已经存在，累加进去。
 33             {
 34                 value[i] = min(value[i], val);
 35                 return ;
 36             }
 37         status[size]= st;           //找不到状态st，则插入st。
 38         value[size] = val;
 39         next[size] = head[h] ;      //新插入的元素在队头
 40         head[h] = size++;
 41     }
 42 }hashmap[2];
 43 
 44 inline int getbit(LL s,int pos)   //取出状态s的第pos个插头
 45 {
 46     return (s>>2*pos)&3;
 47 }
 48 inline int setbit(LL s,int pos,int bit)   //将状态s的第pos个插头设置为bit
 49 {
 50     if(s&(1<<2*pos ))     s^=1<<(2*pos);
 51     if(s&(1<<(2*pos+1)))  s^=1<<(2*pos+1);
 52     return (s|(bit<<2*pos));
 53 }
 54 
 55 int Fr(LL s,int pos,int bit)   //寻找状态s的第pos个插头对应的右括号。
 56 {
 57     int cnt=0;
 58     for(pos+=2; pos<m; pos++)
 59     {
 60         if(getbit(s, pos)==3-bit)   cnt++;
 61         if(getbit(s, pos)==bit)     cnt--;
 62         if(cnt==-1)         return setbit(s, pos, 3-bit);
 63     }
 64 }
 65 int Fl(LL s,int pos,int bit)   //寻找状态s的第pos个插头对应的左括号。
 66 {
 67     int cnt=0;
 68     for(pos--; pos>=0; pos--)
 69     {
 70         if(getbit(s, pos)==3-bit)  cnt++;
 71         if(getbit(s, pos)==bit)    cnt--;
 72         if( cnt==-1)    return setbit(s, pos, 3-bit);
 73     }
 74 }
 75 
 76 void DP(int i,int j)
 77 {
 78     LL t;
 79     for(int k=0; k<hashmap[cur^1].size; k++)
 80     {
 81         LL s=hashmap[cur^1].status[k];
 82         LL v=hashmap[cur^1].value[k];
 83         int R=getbit(s, j), D=getbit(s, j+1);
 84         if(R && D)  //两个括号
 85         {
 86             t=(setbit(s,j,0)&setbit(s,j+1,0));
 87             if(R==D)    //同个方向的括号
 88             {
 89                 if(R==1)    t=Fr(t, j, 2);  //要改
 90                 else        t=Fl(t, j, 1);
 91                 hashmap[cur].insert(t, v);
 92             }
 93             else if( R==2 && D==1 )        //不同的连通分量
 94                 hashmap[cur].insert(t, v);
 95             else if( i+1==n && j+1==m && t==0 )        //终点才能闭合
 96                 hashmap[cur].insert(t, v);
 97         }
 98         else if(R || D)     //仅1个括号
 99         {
100             if(R)   //右插头
101             {
102                 if(i+1<n )   hashmap[cur].insert(s, v+g[i*m+j+1][i*m+m+j+1]);//往下
103                 if(j+1<m )   hashmap[cur].insert(setbit(setbit(s,j,0), j+1, R), v+g[i*m+j+1][i*m+j+2]);//往右
104             }
105             else    //下插头
106             {
107                 if(j+1<m )   hashmap[cur].insert(s, v+g[i*m+j+1][i*m+j+2]); //往右
108                 if(i+1<n )   hashmap[cur].insert(setbit(setbit(s,j+1,0), j, D), v+g[i*m+j+1][i*m+m+j+1]);//往下
109             }
110         }
111         else                //无括号
112         {
113             if(  j+1<m && i+1<n )    //新括号
114             {
115                 v+=g[i*m+j+1][i*m+m+j+1];   //下
116                 v+=g[i*m+j+1][i*m+j+2];     //右
117                 hashmap[cur].insert( setbit(s,j,1)|setbit(s,j+1,2), v);
118             }
119         }
120     }
121 }
122 
123 void cal()
124 {
125     for(int i=0; i<n; i++)
126     {
127         cur^=1;
128         hashmap[cur].clear();
129         for(int j=0; j<hashmap[cur^1].size; j++)    //新行，需要左移一下状态。
130             hashmap[cur].insert( hashmap[cur^1].status[j]<<2, hashmap[cur^1].value[j] );
131         for(int j=0; j<m; j++)
132         {
133             cur^=1;
134             hashmap[cur].clear();
135             DP(i,j);
136         }
137     }
138 }
139 LL print()
140 {
141     for(int i=0; i<hashmap[cur].size; i++)
142         if(hashmap[cur].status[i]==0)
143             return hashmap[cur].value[i];
144     return 0;
145 }
146 
147 
148 int main()
149 {
150     //freopen("input.txt", "r", stdin);
151     int t, Case=0;
152     cin>>t;
153     while(t--)
154     {
155         memset(g,0,sizeof(g));
156         n=m=cur=0;
157         scanf("%d%d",&n,&m);
158         for(int i=1; i<n; i++)      //输入
159         {
160             for(int j=1; j<m; j++)  //左右
161             {
162                 char c=getchar();
163                 if(isdigit(c))
164                     g[i*m-m+j][i*m-m+j+1]=g[i*m-m+j+1][i*m-m+j]=c-'0';
165                 else j--;
166             }
167             for(int j=1; j<=m; j++) //上下
168             {
169                 char c=getchar();
170                 if(isdigit(c))
171                     g[i*m-m+j][i*m+j]=g[i*m+j][i*m-m+j]=c-'0';
172                 else j--;
173             }
174         }
175         for(int j=1; j<m; j++)      //最后一行：左右
176         {
177             char c=getchar();
178             if(isdigit(c))
179                 g[n*m-m+j][n*m-m+j+1]=g[n*m-m+j+1][n*m-m+j]=c-'0';
180             else j--;
181         }
182         char s[20];
183         scanf("%s%s",s,s);//多余
184 
185         hashmap[cur].clear();
186         hashmap[cur].insert(0, 0);  //初始状态:花费0
187         cal();
188         cout<<print()<<endl;
189     }
190     return 0;
191 }
AC代码
```

