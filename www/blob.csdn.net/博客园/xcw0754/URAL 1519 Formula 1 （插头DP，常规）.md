# URAL 1519 Formula 1 （插头DP，常规） - xcw0754 - 博客园
# [URAL 1519 Formula 1 （插头DP，常规）](https://www.cnblogs.com/xcw0754/p/4788453.html)
题意：给一个n*m的矩阵，格子中是'*'则是障碍格子，不允许进入，其他格子都是必走的格子，所走格子形成一条哈密顿回路，问有多少种走法？
思路：
　　本来是很基础的题，顿时不知道进入了哪个坑。这篇插头DP的[文章](http://blog.sina.com.cn/s/blog_51cea4040100gmky.html)够详细，推荐一看（最好不要照抄代码）。
　　细节要小心，比如输出用I64d，必须用long long，判断是否无哈密顿回路，位操作等等。
　　这次仍然用括号表示法，1表示(，2表示)。
```
1 #include <bits/stdc++.h>
  2 #define pii pair<int,int>
  3 #define INF 0x3f3f3f3f
  4 #define LL long long
  5 using namespace std;
  6 const int N=20;
  7 const int mod=12357;
  8 const int NN=1000010;
  9 char g[N][N];
 10 int cur, n, m, ex, ey;
 11 struct Hash_Map
 12 {
 13     int head[mod];      //桶指针
 14     int next[NN];        //记录链的信息
 15     int status[NN];      //状态
 16     LL  value[NN];       //状态对应的DP值。
 17     int size;
 18 
 19     void clear()    //清除哈希表中的状态
 20     {
 21         memset(head, -1, sizeof(head));
 22         size = 0;
 23     }
 24 
 25     void insert(int st, LL val)  //插入状态st的值为val
 26     {
 27         int h = st%mod;
 28         for(int i=head[h]; i!=-1; i=next[i])
 29             if(status[i] == st) //这个状态已经存在，累加进去。
 30             {
 31                 value[i] += val;
 32                 return ;
 33             }
 34         status[size]= st;           //找不到状态st，则插入st。
 35         value[size] = val;
 36         next[size] = head[h] ;      //新插入的元素在队头
 37         head[h] = size++;
 38     }
 39 }hashmap[2];
 40 
 41 int getbit(int s,int pos)   //取出状态s的第pos个插头
 42 {
 43     int bit=0;
 44     if(s&(1<<(2*pos+1)))  bit+=2;    //高位对应高位
 45     if(s&(1<<2*pos))    bit+=1;
 46     return bit;
 47 }
 48 int setbit(int s,int pos,int bit)   //将状态s的第pos个插头设置为bit
 49 {
 50     if(s&(1<<2*pos ))     s^=1<<(2*pos);
 51     if(s&(1<<(2*pos+1)))  s^=1<<(2*pos+1);
 52     return (s|(bit<<2*pos));
 53 }
 54 
 55 int Fr(int s,int pos,int bit)   //寻找状态s的第pos个插头对应的右括号。
 56 {
 57     int cnt=0;
 58     for(pos+=2; pos<m; pos++)
 59     {
 60         if(getbit(s, pos)==3-bit)   cnt++;
 61         if(getbit(s, pos)==bit)     cnt--;
 62         if(cnt==-1)         return setbit(s, pos, 3-bit);
 63     }
 64 }
 65 int Fl(int s,int pos,int bit)   //寻找状态s的第pos个插头对应的左括号。
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
 76 void DP(int i,int j)    //非障碍空格
 77 {
 78     for(int k=0,t; k<hashmap[cur^1].size; k++)
 79     {
 80         int s=hashmap[cur^1].status[k];
 81         LL v=hashmap[cur^1].value[k];
 82         int R=getbit(s,j), D=getbit(s,j+1);
 83         if(g[i][j]=='*')    //障碍格子
 84         {
 85             if( R==0 && D==0 )    hashmap[cur].insert(s, v);
 86             continue ;
 87         }
 88         if(R && D)  //两个括号
 89         {
 90             t=(setbit(s,j,0)&setbit(s,j+1,0));
 91             if(R==D)    //同个方向的括号
 92             {
 93                 if(R==1)    t=Fr(t, j, 2);  //要改
 94                 else        t=Fl(t, j, 1);
 95                 hashmap[cur].insert(t, v);
 96             }
 97             if( R==2 && D==1 )        //不同的连通分量
 98                 hashmap[cur].insert(t, v);
 99             if(i==ex && j==ey && R==1 && D==2 )  //终点时'('和')'才可以合并。
100                 hashmap[cur].insert(t, v);
101         }
102         else if(R || D)     //仅1个括号
103         {
104             hashmap[cur].insert(s,v);
105             if(R)   t=setbit(setbit(s,j,0),j+1,R);
106             else    t=setbit(setbit(s,j+1,0),j,D);
107             hashmap[cur].insert(t,v);
108         }
109         else                //无括号
110             hashmap[cur].insert( setbit(s,j,1)|setbit(s,j+1,2), v);
111     }
112 }
113 
114 void cal()
115 {
116     if(ex==-1)  return ;  //无空格
117     for(int i=0; i<n; i++)
118     {
119         cur^=1;
120         hashmap[cur].clear();
121         for(int j=0; j<hashmap[cur^1].size; j++)    //新行，需要左移一下状态。
122             if( getbit( hashmap[cur^1].status[j], m)==0 )   //多余的状态需要去除
123                 hashmap[cur].insert( hashmap[cur^1].status[j]<<2, hashmap[cur^1].value[j] );
124         for(int j=0; j<m; j++)
125         {
126             cur^=1;
127             hashmap[cur].clear();
128             DP(i,j);
129             if(i==ex && j==ey)    return ;  //终点
130         }
131     }
132 }
133 
134 LL print()
135 {
136     for(int i=0; i<hashmap[cur].size; i++)  //寻找轮廓线状态为0的值。
137         if(  hashmap[cur].status[i]==0 )
138             return hashmap[cur].value[i];
139     return 0;
140 }
141 int main()
142 {
143     //freopen("input.txt", "r", stdin);
144     while(~scanf("%d%d",&n,&m))
145     {
146         ex=ey=-1;
147         cur=0;
148         for(int i=0; i<n; i++)      //输入矩阵
149             for(int j=0; j<m; j++)
150             {
151                 char c=getchar();
152                 if(c=='.'||c=='*')
153                 {
154                     g[i][j]=c;
155                     if( c=='.' )    ex=i,ey=j;//终点空格
156                 }
157                 else    j--;
158             }
159 
160         hashmap[cur].clear();
161         hashmap[cur].insert(0, 1);  //初始状态
162         cal();
163         cout<<print()<<endl;
164     }
165     return 0;
166 }
AC代码
```

