# UVA 658 It's not a Bug, it's a Feature! (最短路，经典) - xcw0754 - 博客园
# [UVA 658 It's not a Bug, it's a Feature! (最短路，经典)](https://www.cnblogs.com/xcw0754/p/4623121.html)
题意：有n个bug，有m个补丁，每个补丁有一定的要求（比如某个bug必须存在，某个必须不存在，某些无所谓等等），打完出来后bug还可能变多了呢。但是打补丁是需要时间的，每个补丁耗时不同，那么问题来了：要打多久才能无bug？（同1补丁可重复打）
分析：
n<=20，那么用位来表示bug的话有220=100万多一点。不用建图了，图实在太大了，用位图又不好玩。那么直接用隐式图搜索（在任意点，只要满足转移条件，任何状态都能转）。
　　但是有没有可能每个状态都要搜1次啊？那可能是100万*100万啊，这样出题还有解么？大部分状态是怎么打补丁都到不了的，但是具体要说是多少个也不能这么说吧，这个分析可能也太麻烦了，设计100个补丁能将n个1打成所有状态都覆盖就差不多超时了。（出题人应该不会这么精明，忽略这个问题）
　　那么用位表示状态要怎么办？类似于状态压缩的感觉，每个bug有3种可能（必存在，必不存在，无所谓），只要40个位搞定，但是两个位表示1个bug状态也太麻烦了吧？那就开两个数组咯，配合表示就行了，丝毫没有省空间！！但是容易看了。但是这么设计使得在匹配的时候神速啊。
思路：
　　一开始所有bug是存在的，所以有sta=末尾n个1。接着我们要将其打成sta=0，任务艰巨，赶紧开始。
　　（1）首先求状态的上限up。将1左移n个位置再减少1。
　　（2）dijkstra来穷举每个状态，即0~up。用优先队列来优化它，一旦距离dist有更新，就进队(这么像SPFA!其实用了优先队列，一点不像，只要vis[该点]=1，那就忽略它，而spfa不忽略)。
　　（3）当队列不空一直循环，直到循环了up次，或者可达状态全部遍历过了，就退出了，直接检查dist[0]是否为无穷大。
难点：
　　其实这题考的不是SSSP，而是位运算的设计。
　　（1）如何检查m个bug种哪些是可以转移的？
　　　　数组pat_r1[i]表示第i个补丁需要哪些bug必须存在，存在则该位为1；
　　　　数组pat_r2[i]表示第i个补丁对那些bug无所谓，无所谓的位为0。
　　　　假设本状态为flag，判断第j个补丁是否可以打，应该这样：
 　　　　　　int res=(flag^pat_r1[j]); 　　//异或完，满足要求的(即相同的)位会为0（无所谓的先不管）。
    　　　　　　res&=pat_r2[j]; 　　　　　　        //将无所谓的位全部置为0。
    　　　　　　if(res==0)    return true;　　　//满足要求
　　（2）如何获取转移后的状态？
　　　　数组pat_o1[i]表示第i个补丁打完后输出的哪些bug依然存在，存在则该位为1；
　　　　数组pat_o2[i]表示第i个补丁打完后输出的哪些bug依然不变，不会变的位为1。
　　　　假设本状态为flag，打完补丁j 输出的结果应该是：
 　　　　　　int tmp=(flag&pat_o2[j]); 　　 //将不变的取出来
    　　　　　　tmp+=pat_o1[j]; 　　 　　　　 //将不变的补回去
    　　　　　　return tmp;　　　　　　　　　　//打完了
```
1 #include <bits/stdc++.h>
  2 #define LL long long
  3 #define pii pair<int,int>
  4 using namespace std;
  5 const int N=100+5;
  6 const int INF=0x7f7f7f7f;
  7 int pat_r1[N];   //补丁所需要的要求1
  8 int pat_r2[N];   //补丁所需要的要求2，记录无所谓
  9 int val[N];      //权值
 10 int pat_o1[N];   //打完补丁后的产出1
 11 int pat_o2[N];   //打完补丁后的产出2,记录不变
 12 int dist[1200000];   //表示状态
 13 bool vis[1200000];  
 14 int bugs, patches, v;
 15 int res, tmp;    //临时变量
 16 
 17 /*
 18 int cal()   //超时！！未优化的dijkstra。
 19 {
 20     memset(dist,0x7f,sizeof(dist));
 21     memset(vis,0,sizeof(vis));
 22 
 23     int t=bugs, up=1;
 24     while(t--)  up+=up;
 25     dist[--up]=0;
 26     for(int i=up; i>=0; i--)
 27     {
 28         int small=INF, flag=INF;
 29         for(int j=0; j<=up; j++)  //在状态j中找dist最短
 30         {
 31             if(!vis[j]&&dist[j]<small)
 32             {
 33                 flag=j;         //标记该状态
 34                 small=dist[j];  //snall保存所耗时间
 35             }
 36         }
 37         if(flag==INF)   return -1;  //找不到
 38 
 39         vis[flag]=1;
 40 
 41 
 42         for(int j=1; j<=patches; j++)   //扫描每个补丁
 43         {
 44             //检查是否满足条件
 45             res=(flag^pat_r1[j]);   //异或完，满足要求的会为0（除了无所谓的）。
 46             res&=pat_r2[j];         //将无所谓的位全部置为0。
 47 
 48             if( !res  )    //只要为0就是满足条件
 49             {
 50                 tmp=(flag&pat_o2[j]);   //将不变的取出来
 51                 tmp+=pat_o1[j];         //将不变的补回去
 52                 if( dist[tmp]>dist[flag]+val[j] )    //时间比较短的
 53                 {
 54                     dist[tmp]=dist[flag]+val[j];
 55                     if(!tmp)   return dist[tmp];  //bug全部修复
 56                 }
 57             }
 58         }
 59     }
 60     return -1;  //修不了
 61 
 62 
 63 }
 64 
 65 */
 66 
 67 
 68 int can_use(int flag, int j)
 69 {
 70     //检查是否满足条件
 71     int res=(flag^pat_r1[j]);   //异或完，满足要求的会为0（除了无所谓的）。
 72     res&=pat_r2[j];         //将无所谓的位全部置为0。
 73     if(res==0)    return true;
 74     return false;
 75 }
 76 
 77 int get_sta(int flag, int j)
 78 {
 79     //获取将要更新的状态
 80     int tmp=(flag&pat_o2[j]);   //将不变的取出来
 81     tmp+=pat_o1[j];             //将不变的补回去
 82     return tmp;
 83 }
 84 
 85 int cal()
 86 {
 87     priority_queue<pii, vector<pii>, greater<pii> > que;
 88 
 89     memset(vis, 0, sizeof(vis));
 90     memset(dist, 0x7f, sizeof(dist));
 91 
 92     int t=bugs, up=1;
 93     while(t--)  up+=up; //获取上限
 94     dist[--up]=0;
 95 
 96     que.push(make_pair(0,up));
 97     while(!que.empty())
 98     {
 99         int flag=que.top().second; que.pop();
100         if(vis[flag]) continue;     //重复
101         vis[flag]=1;
102 
103         for(int j=1; j<=patches; j++)   //扫描每个补丁
104         {
105             int tmp=get_sta(flag, j);
106             if( can_use(flag, j) && dist[tmp]>dist[flag]+ val[j] )    //只要为0就是满足条件
107             {
108                 dist[tmp]=dist[flag]+ val[j];
109                 que.push(make_pair(dist[tmp],tmp));
110             }
111         }
112     }
113     return dist[0];
114 }
115 
116 int main()
117 {
118     freopen("input.txt", "r", stdin);
119     char s[50], s2[50];
120     int Case=0;
121     while(scanf("%d %d", &bugs, &patches ), bugs||patches)
122     {
123         for(int i=1; i<=patches; i++)
124         {
125             scanf("%d %s %s", &v, s, s2);
126             val[i]=v;
127             pat_r1[i]= pat_r2[i]= 0;
128             for(int j=1; j<=bugs; j++)  //要求
129             {
130                 pat_r1[i]<<=1;
131                 pat_r2[i]<<=1;
132 
133                 if(s[j-1]=='+')       //'+'用1表示，'-'用0表示(直接移位，不用处理)
134                     pat_r1[i]++;
135                 if(s[j-1]!='0')       //无所谓用0表示
136                     pat_r2[i]++;
137             }
138 
139             pat_o1[i]= pat_o2[i]= 0;
140             for(int j=1; j<=bugs; j++)  //输出
141             {
142                 pat_o1[i]<<=1;
143                 pat_o2[i]<<=1;
144                 if(s2[j-1]=='+')  //有bug就1，无bug为0
145                     pat_o1[i]++;
146                 if(s2[j-1]=='0')  //不变为1
147                     pat_o2[i]++;
148             }
149         }
150         printf("Product %d\n",++Case);
151         int ans=cal();
152         if(ans<INF) printf("Fastest sequence takes %d seconds.\n\n", ans );
153         else     printf("Bugs cannot be fixed.\n\n");
154     }
155     return 0;
156 }
AC代码177ms
```
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 #define pii pair<int,int>
 4 using namespace std;
 5 const int N=100+5;
 6 const int INF=0x7f7f7f7f;
 7 int pat_r1[N], pat_r2[N], pat_o1[N], pat_o2[N], val[N], dist[1200000], vis[1200000];
 8 int bugs, patches, v;
 9 
10 int can_use(int flag, int j)
11 {
12     int res=(flag^pat_r1[j]);   //异或完，满足要求的会为0（除了无所谓的）。
13     res&=pat_r2[j];         //将无所谓的位全部置为0。
14     if(res==0)    return true;
15     return false;
16 }
17 
18 int get_sta(int flag, int j)
19 {
20     int tmp=(flag&pat_o2[j]);   //将不变的取出来
21     tmp+=pat_o1[j];             //将不变的补回去
22     return tmp;
23 }
24 
25 
26 
27 int cal()
28 {
29     priority_queue<pii, vector<pii>, greater<pii> > que;
30     memset(vis, 0, sizeof(vis));
31     memset(dist, 0x7f, sizeof(dist));
32     int up=1;
33     while(bugs--)  up+=up;
34     dist[--up]=0;
35 
36     que.push(make_pair(0,up));
37     while(!que.empty())
38     {
39         int flag=que.top().second; que.pop();
40         vis[flag]=1;
41         for(int j=1; j<=patches; j++)   //扫描每个补丁
42         {
43             int tmp=get_sta(flag, j);
44             if( can_use(flag, j) && dist[tmp]>dist[flag]+ val[j] )    //只要为0就是满足条件
45             {
46                 dist[tmp]=dist[flag]+ val[j];
47                 if(!vis[tmp])    que.push(make_pair(dist[tmp],tmp));
48                 //if(!tmp)   return dist[0];  //不能这么做，可能还有更新，这只是更新第一次而已，至少要n次
49             }
50         }
51     }
52     return dist[0];
53 }
54 
55 int main()
56 {
57     freopen("input.txt", "r", stdin);
58     char s[50], s2[50];
59     int Case=0;
60     while(scanf("%d %d", &bugs, &patches ), bugs||patches)
61     {
62         for(int i=1; i<=patches; i++)
63         {
64             scanf("%d %s %s", &v, s, s2);
65             val[i]=v;
66             pat_r1[i]= pat_r2[i]= pat_o1[i]= pat_o2[i]= 0;
67             for(int j=1; j<=bugs; j++)
68             {
69                 pat_r1[i]<<=1,pat_r2[i]<<=1,pat_o1[i]<<=1,pat_o2[i]<<=1;
70                 if(s[j-1]=='+')     pat_r1[i]++;
71                 if(s[j-1]!='0')     pat_r2[i]++;
72                 if(s2[j-1]=='+')    pat_o1[i]++;
73                 if(s2[j-1]=='0')    pat_o2[i]++;
74             }
75         }
76         int ans=cal();
77         if(ans<INF) printf("Product %d\nFastest sequence takes %d seconds.\n\n",++Case, ans );
78         else     printf("Product %d\nBugs cannot be fixed.\n\n",++Case);
79     }
80     return 0;
81 }
短码AC版本
```

