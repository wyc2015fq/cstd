# HDU 3377 Plan （插头DP，变形） - xcw0754 - 博客园
# [HDU 3377 Plan （插头DP，变形）](https://www.cnblogs.com/xcw0754/p/4792911.html)
题意：有一个n*m的矩阵，每个格子中有一个值（可能负值），要从左上角走到右下角，求路径的最大花费。
思路：
　　除了起点和终点外，其他的点可以走，也可以不走。
　　（2）我用的是括号表示法，所以起始状态为')'，即仅有一个右括号，那么到右下角也应该是只有一个右括号。因为，如果碰到()**)**,加粗表示起点的那个右括号,那么合并后变成**)**##，仍然是右括号，如果是**)**()，那么合并后变成##)，仍然是右括号，相当于延续了。插头每到达一个格子就先将其值给加上，如果要合并的时候，再减掉（因为多算了一次），因此，新括号的出现，就需要多加上3个格子的值了。
 　　（2）还有一个更直观的办法，就是添加半个圈，从起点到终点，设他们都为必走的格子，那就跟[FZU 1977 PANDORA ADVENTURE （插头DP，常规）](http://www.cnblogs.com/xcw0754/p/4789842.html)差不多了，只是没有了障碍格子而已。
　　比如矩阵：
　　000
　　000
　　000
　　可以建图为（其中b为必走的格子）：
　　bbbb
　　b00b
　　000b
　　00bb
　　由于不用这样建图也是很容易解决的，所以下面代码就不这样建图了。
```
1 #include <bits/stdc++.h>
  2 #include <iostream>
  3 #include <cstdio>
  4 #include <cstring>
  5 #define pii pair<int,int>
  6 #define INF 0x3f3f3f3f
  7 #define LL long long
  8 using namespace std;
  9 const int N=15;
 10 int g[N][N], cur, n, m;
 11 struct Hash_Map
 12 {
 13     static const int mod=12357;
 14     static const int NN=100010;
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
 34                 value[i] = max(value[i], val);
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
 46 inline int getbit(LL s,int pos)   //取出状态s的第pos个插头
 47 {
 48     return (s>>2*pos)&3;
 49 }
 50 inline int setbit(LL s,int pos,int bit)   //将状态s的第pos个插头设置为bit
 51 {
 52     if(s&(1<<2*pos ))     s^=1<<(2*pos);
 53     if(s&(1<<(2*pos+1)))  s^=1<<(2*pos+1);
 54     return (s|(bit<<2*pos));
 55 }
 56 
 57 int Fr(LL s,int pos,int bit)   //寻找状态s的第pos个插头对应的右括号。
 58 {
 59     int cnt=0;
 60     for(pos+=2; pos<m; pos++)
 61     {
 62         if(getbit(s, pos)==3-bit)   cnt++;
 63         if(getbit(s, pos)==bit)     cnt--;
 64         if(cnt==-1)         return setbit(s, pos, 3-bit);
 65     }
 66 }
 67 int Fl(LL s,int pos,int bit)   //寻找状态s的第pos个插头对应的左括号。
 68 {
 69     int cnt=0;
 70     for(pos--; pos>=0; pos--)
 71     {
 72         if(getbit(s, pos)==3-bit)  cnt++;
 73         if(getbit(s, pos)==bit)    cnt--;
 74         if( cnt==-1)    return setbit(s, pos, 3-bit);
 75     }
 76 }
 77 LL ans;
 78 void DP(int i,int j)
 79 {
 80     for(int k=0; k<hashmap[cur^1].size; k++)
 81     {
 82         LL s=hashmap[cur^1].status[k];
 83         LL v=hashmap[cur^1].value[k];
 84         int R=getbit(s, j), D=getbit(s, j+1);
 85         LL t=(setbit(s,j,0)&setbit(s,j+1,0));
 86         if(R && D)  //两个括号
 87         {
 88 
 89             if(R==D)    //同个方向的括号
 90             {
 91                 if(R==1)    t=Fr(t, j, 2);  //要改
 92                 else        t=Fl(t, j, 1);
 93                 hashmap[cur].insert(t, v-g[i][j]);
 94             }
 95             else if( R==2 && D==1 )        //不同的连通分量
 96                 hashmap[cur].insert(t, v-g[i][j]);
 97         }
 98         else if(R || D)     //仅1个括号
 99         {
100             if( i+1==n && j+1==m && t==0 && ( R==2 || D==2 ) )        //终点才能闭合
101                 ans=max(ans, v);
102             if(R)   //右插头
103             {
104                 if(i+1<n )   hashmap[cur].insert(s, v+g[i+1][j]);//往下
105                 if(j+1<m )   hashmap[cur].insert(setbit(t,j+1,R), v+g[i][j+1]);//往右
106             }
107             else    //下插头
108             {
109                 if(j+1<m )   hashmap[cur].insert(s, v+g[i][j+1]); //往右
110                 if(i+1<n )   hashmap[cur].insert(setbit(t,j,D), v+g[i+1][j]);//往下
111             }
112         }
113         else
114         {
115             if( i+j ) hashmap[cur].insert(s, v);            //不装括号
116             if( j+1<m && i+1<n && i+j )    //新括号
117              hashmap[cur].insert( setbit(s,j,1)|setbit(s,j+1,2), v+g[i][j]+g[i+1][j]+g[i][j+1]);
118         }
119     }
120 }
121 
122 void cal()
123 {
124     for(int i=0; i<n; i++)
125     {
126         cur^=1;
127         hashmap[cur].clear();
128         for(int j=0; j<hashmap[cur^1].size; j++)    //新行，需要左移一下状态。
129             hashmap[cur].insert( hashmap[cur^1].status[j]<<2, hashmap[cur^1].value[j] );
130         for(int j=0; j<m; j++)
131         {
132             cur^=1;
133             hashmap[cur].clear();
134             DP(i,j);
135         }
136     }
137 }
138 
139 
140 
141 int main()
142 {
143     //freopen("input.txt", "r", stdin);
144     int Case=0;
145     while(~scanf("%d%d",&n,&m))
146     {
147         memset(g, 0, sizeof(g));
148         ans=-INF;       //注意
149         cur=0;
150         for(int i=0; i<n; i++)      //输入
151             for(int j=0; j<m; j++)
152                 scanf("%d",&g[i][j]);
153 
154         hashmap[cur].clear();
155         hashmap[cur].insert(2, g[0][0]);
156         cal();
157         printf("Case %d: %lld\n", ++Case, ans);
158     }
159     return 0;
160 }
AC代码
```

