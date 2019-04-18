# HDU 5469 Antonidas （树形DP，暴力） - xcw0754 - 博客园
# [HDU 5469 Antonidas （树形DP，暴力）](https://www.cnblogs.com/xcw0754/p/4841135.html)
题意：
　　给一棵n节点的树图，每个点都是一个小写字母，要求找到两个点(a,b)，从a->b的路径上形成了一个字符串为s。给出s，问是否存在这样的点对。
思路：
　　考虑一个点，要么从该点出发，要么在该点结束，要么它作为一个中间点将左右两个串连起来成为s。叶子只能是起点或者终点。在每个点中需要保存两个队列，表示有点可以从正or反向走到这个点的长度（即前缀与后缀，但只需记录当前点是排在第几）。对于在本节点连接的情况，枚举一下哪些孩子可能在本节点连接就行了。
　　2s多
```
1 #include <bits/stdc++.h>
  2 #define pii pair<int,int>
  3 #define max(x,y) ((x)>(y)?(x):(y))
  4 #define min(x,y) ((x)<(y)?(x):(y))
  5 #define abs(x) ((x)<0?-(x):(x))
  6 #define INF 0x3f3f3f3f
  7 #define LL  long long
  8 using namespace std;
  9 const double PI  = acos(-1.0);
 10 const int N=10010;
 11 struct node
 12 {
 13     int from, to, next;
 14     node(){};
 15     node(int from,int to,int next):from(from),to(to),next(next){};
 16 }edge[N*2];
 17 
 18 int head[N], edge_cnt, n, m;
 19 char c[N], s[N];
 20 void add_node(int from,int to)
 21 {
 22     edge[edge_cnt]=node(from,to,head[from]);
 23     head[from]=edge_cnt++;
 24 }
 25 
 26 bitset<10005> mapp[N], mapp2[N];
 27 deque<int>    que1[N], que2[N];
 28 bool ans;
 29 void DFS(int t,int far)
 30 {
 31     node e;
 32     for(int i=head[t]; i!=-1 && ans==false; i=e.next)
 33     {
 34         e=edge[i];
 35         if(e.to==far)   continue;
 36         DFS(e.to, t);
 37 
 38         int siz=que1[e.to].size();
 39         for(int j=0; j<siz; j++)    //将que1装进来先
 40         {
 41             int r=que1[e.to].front();
 42             que1[e.to].pop_front();
 43             que1[e.to].push_back(r);    //que1[e.to]并没有删除
 44             if( c[t]==s[r+1] )
 45             {
 46                 if(mapp[t][r+1])    mapp2[t][r+1]=1;    //增加1个位表示是否有两个孩子能连到此点
 47                 else if(!mapp[t][r+1])   mapp[t][r+1]=1,que1[t].push_back(r+1);
 48                 if(r+1==m)
 49                 {
 50                     ans=true;
 51                     return ;
 52                 }
 53             }
 54         }
 55     }
 56 
 57     for(int i=head[t]; i!=-1 && ans==false; i=e.next)   //考虑每个孩子
 58     {
 59         e=edge[i];if(e.to==far)   continue;
 60 
 61         int siz=que1[e.to].size();      //先把此孩子的左，从mapp中全部去掉
 62         for(int j=0; j<siz; j++)
 63         {
 64             int r=que1[e.to].front();
 65             que1[e.to].pop_front();
 66             que1[e.to].push_back(r);
 67             if( c[t]==s[r+1] && !mapp2[t][r+1] )    mapp[t][r+1]=0;
 68         }
 69 
 70         siz=que2[e.to].size();             //判断是否在此点链接
 71         for(int i=0; i<siz; i++)
 72         {
 73             int r=que2[e.to].front();
 74             que2[e.to].pop_front();
 75             que2[e.to].push_back(r);        //que2还没有删
 76             if( mapp[t][r-1]==1 ){ ans=true; return ;} //找到了另一半
 77         }
 78 
 79         while( !que2[e.to].empty() )   //找不到时，再将que2装进去
 80         {
 81             int r=que2[e.to].front();que2[e.to].pop_front();
 82             if( c[t]==s[r-1] )      //刚好相同
 83             {
 84                 que2[t].push_back( r-1 );
 85                 if(r-1==1)          //以此点为终点
 86                 {
 87                     ans=true;
 88                     return ;
 89                 }
 90             }
 91         }
 92 
 93         while( !que1[e.to].empty() )    //将此孩子的que1装回去
 94         {
 95             int r=que1[e.to].front();
 96             que1[e.to].pop_front();
 97             if( c[t]==s[r+1] )
 98             {
 99                 if( !mapp[t][r+1] ) mapp[t][r+1]=1;
100             }
101         }
102     }
103     if(c[t]==s[1])    que1[t].push_back(1);    //起点或终点
104     if(c[t]==s[m])    que2[t].push_back(m);
105 }
106 
107 
108 
109 
110 bool test()     //s的长度为1的情况
111 {
112     for(int i=1; i<=n; i++)
113         if(c[i]==s[1])    return true;
114     return false;
115 }
116 
117 void init()
118 {
119     edge_cnt=0;
120     ans=false;
121     memset(head,-1,sizeof(head));
122     for(int i=0; i<=n; i++)
123         mapp[i].reset(),
124         mapp2[i].reset(),
125         que1[i].clear(),
126         que2[i].clear();
127 }
128 
129 
130 int main()
131 {
132     //freopen("input.txt", "r", stdin);
133     int t, a, b, Case=0;
134     cin>>t;
135     while(t--)
136     {
137         scanf("%d",&n);
138         init();
139         for(int i=1; i<n; i++)
140         {
141             scanf("%d%d",&a,&b);
142             add_node(a,b);
143             add_node(b,a);
144         }
145         scanf("%s", c+1);
146         scanf("%s", s+1);
147         m=strlen(s+1);
148 
149         DFS(1,-1);
150         if(m==1)
151         {
152             if( test() )        printf("Case #%d: Find\n", ++Case);
153             else                printf("Case #%d: Impossible\n", ++Case);
154         }
155         else if(m<=n&&ans==true)printf("Case #%d: Find\n", ++Case);
156         else                    printf("Case #%d: Impossible\n", ++Case);
157     }
158     return 0;
159 }
AC代码
```

