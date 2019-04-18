# POJ 1739 Tony's Tour （插头DP，轮廓线DP） - xcw0754 - 博客园
# [POJ 1739 Tony's Tour （插头DP，轮廓线DP）](https://www.cnblogs.com/xcw0754/p/4786762.html)
题意：给一个n*m的矩阵，其中#是障碍格子，其他则是必走的格子，问从左下角的格子走到右下角的格子有多少种方式。
思路：
　　注意有可能答案是0，就是障碍格子阻挡住了去路。
　　插头DP有两种比较常见的表示连通信息的方式：
　　（1）最小表示法
　　（2）括号表示法
　　本文用括号表示法实现。左括号为1，右括号为2，用两个位来表示。轮廓线上最多需要表示9个插头信息，那么就是18个位即可。插头的状态转移有如下几种：
　　（1）右插头和下插头都是同个方向的括号，则合并他们，再将对应的两外两个半括号给改成一对。比如 ((())) 合并完变成##()()，橙色的就是需要改的地方。
　　（2）右插头是')'，下插头是'('，则合并他们，且无需任何修改。
　　（3）右插头是'('，下插头是')'，则不能合并，因为一旦合并，肯定是组成1个圆了，就会有多个连通分量的产生。自己画画就知道了。
　　（4）右插头是'('，下插头是')'，只有在最后一个非障碍格子（按行从左到右遍历的）的时候才可以合并，
　　（5）右/下插头只有1个插头存在，那么可以延续它，可以分别往下和右两个方向。
　　（6）没有插头，那么只能另开一对新括号了，分别对应右和下插头的位置。
　　（7）障碍格子，只有该位置的两个插头都是空的时候才可以转移，且轮廓线无需修改。
　　因为状态本来就不多，用哈希表来存状态会比较快且比较省时间，哈希表实现是摘别人的。每次只需要用上一个格子中的状态来转移到当前格子的状态。本题是不能有连通分量产生的，所以只需要在初始的状态设置起点和终点是一对括号，那就相当于在找哈密顿回路了，和Formula 1就一样了。
```
1 //#include <bits/stdc++.h>
  2 #include <iostream>
  3 #include <map>
  4 #include <vector>
  5 #include <cstdio>
  6 #include <cstring>
  7 #define pii pair<int,int>
  8 #define INF 0x3f3f3f3f
  9 #define LL long long
 10 using namespace std;
 11 const int N=10;
 12 char g[N][N];
 13 int cur, n, m, ex, ey;
 14 struct Hash_Map
 15 {
 16     static const int mod=12357;
 17     static const int N=50000;
 18     int head[mod];      //桶指针
 19     int next[N];        //链
 20     int status[N];      //状态
 21     LL  value[N];       //状态对应的DP值。
 22     int size;
 23     void clear()
 24     {
 25         memset(head, -1, sizeof(head));
 26         size = 0;
 27     }
 28 
 29     void insert(int st, LL val)  //插入状态st的值为val
 30     {
 31         int h = st%mod;
 32         for(int i=head[h]; i!=-1; i=next[i])
 33         {
 34             if(status[i] == st) //这个状态已经存在，累加进去。
 35             {
 36                 value[i] += val;
 37                 return ;
 38             }
 39         }
 40         status[size]= st;           //新的
 41         value[size] = val;
 42         next[size] = head[h] ;      //新插入的元素在队头
 43         head[h] = size++;
 44     }
 45 }hashmap[2];
 46 
 47 
 48 int getbit(int s,int pos)   //取出状态s的第pos个插头
 49 {
 50     int bit=0;
 51     if(s&(1<<2*pos))    bit+=1;
 52     if(s&(1<<2*pos+1))  bit+=2;
 53     return bit;
 54 }
 55 int setbit(int s,int pos,int bit)   //将状态s的第pos个插头设置为bit
 56 {
 57     if(s&(1<<2*pos ))   s^=1<<2*pos;
 58     if(s&(1<<2*pos+1))  s^=1<<2*pos+1;
 59     return s|(bit<<2*pos);
 60 }
 61 
 62 int Fr(int s,int pos,int bit)   //寻找状态s的第pos个插头对应的右括号。
 63 {
 64     int cnt=0;
 65     for(pos+=2; pos<m; pos++)
 66     {
 67         if(getbit(s,pos)==3-bit)   cnt++;
 68         if(getbit(s,pos)==bit)     cnt--;
 69         if(cnt==-1)         return setbit(s, pos, 3-bit);
 70     }
 71 
 72 }
 73 int Fl(int s,int pos,int bit)   //寻找状态s的第pos个插头对应的左括号。
 74 {
 75     int cnt=0;
 76     for(pos--; pos>=0; pos--)
 77     {
 78         if(getbit(s,pos)==3-bit)  cnt++;
 79         if(getbit(s,pos)==bit)    cnt--;
 80         if( cnt==-1)    return setbit(s, pos, 3-bit);
 81     }
 82 
 83 }
 84 
 85 void DP(int i,int j)    //状态转移
 86 {
 87     for(int k=0; k<hashmap[cur^1].size; k++)
 88     {
 89         int s=hashmap[cur^1].status[k];
 90         int v=hashmap[cur^1].value[k];
 91         int R=getbit(s,j), D=getbit(s,j+1);
 92         if(g[i][j]=='.')
 93         {
 94             if(R && D)  //两个括号
 95             {
 96                 int t=setbit(s,j,0)&setbit(s,j+1,0);
 97                 if(R==D)    //同个方向的括号
 98                 {
 99                     if(R==1)    t=Fr(t,j,2);  //要改
100                     else        t=Fl(t,j,1);
101                     hashmap[cur].insert(t,v);
102                 }
103                 else if( R==2 && D==1 )        //不同方向括号
104                     hashmap[cur].insert(t,v);
105                 else if(i==ex&&j==ey)
106                     hashmap[cur].insert(t,v);
107             }
108             else if(R || D)     //仅1个括号
109             {
110                 hashmap[cur].insert(s,v);
111                 int t;
112                 if(R)   t=setbit(setbit(s,j,0),j+1,R);
113                 else    t=setbit(setbit(s,j+1,0),j,D);
114                 hashmap[cur].insert(t,v);
115             }
116             else    //无括号
117                 hashmap[cur].insert( setbit(s,j,1)|setbit(s,j+1,2), v);
118         }
119         else if(R==0&&D==0) //障碍格子
120             hashmap[cur].insert(s, v);
121     }
122 }
123 void cal()
124 {
125     for(int i=0; i<n; i++)
126     {
127         cur^=1;
128         hashmap[cur].clear();
129         for(int j=0; j<hashmap[cur^1].size; j++)    //新行，需要左移一下状态。
130             if( getbit( hashmap[cur^1].status[j], m)==0 )
131                 hashmap[cur].insert( hashmap[cur^1].status[j]<<2, hashmap[cur^1].value[j] );
132         for(int j=0; j<m; j++)
133         {
134             cur^=1;
135             hashmap[cur].clear();
136             DP(i,j);
137             if(i==ex && j==ey)    return ;  //终点
138         }
139     }
140 }
141 
142 bool print()
143 {
144     for(int i=0; i<hashmap[cur].size; i++)  //寻找轮廓线状态为0的值。
145     {
146         int s=hashmap[cur].status[i];
147         if(s==0)
148         {
149             printf("%lld\n", hashmap[cur].value[i]);
150             return true;
151         }
152     }
153     return false;
154 }
155 int main()
156 {
157     freopen("input.txt", "r", stdin);
158     while(scanf("%d%d",&n,&m), n+m)
159     {
160         ex=ey=cur=0;
161         for(int i=n-1; i>=0; i--)   scanf("%s",g[i]);   //反向存
162         for(int i=0; i<n; i++)          //寻找终点格子：ex和ey
163             for(int j=0; j<m; j++)
164                 if( g[i][j]=='.' )
165                     ex=i,ey=j;
166 
167         hashmap[cur].clear();
168         hashmap[cur].insert(setbit(0,0,1)|setbit(0,m-1,2), 1);  //初始状态
169         cal();
170         if(!print())    puts("0");  //无路可达
171     }
172     return 0;
173 }
AC代码
```
附上哈希表实现：
```
1 struct Hash_Map
 2 {
 3     static const int mod=12357;
 4     static const int N=50000;
 5     int head[mod];      //桶指针
 6     int next[N];        //记录链的信息
 7     int status[N];      //状态
 8     LL  value[N];       //状态对应的DP值。
 9     int size;
10     void clear()    //清除哈希表中的状态
11     {
12         memset(head, -1, sizeof(head));
13         size = 0;
14     }
15 
16     void insert(int st, LL val)  //插入状态st的值为val
17     {
18         int h = st%mod;
19         for(int i=head[h]; i!=-1; i=next[i])
20         {
21             if(status[i] == st) //这个状态已经存在，累加进去。
22             {
23                 value[i] += val;
24                 return ;
25             }
26         }
27         status[size]= st;           //找不到状态st，则插入st。
28         value[size] = val;
29         next[size] = head[h] ;      //新插入的元素在队头
30         head[h] = size++;
31     }
32 }hashmap[2];
Hash_Map
```

