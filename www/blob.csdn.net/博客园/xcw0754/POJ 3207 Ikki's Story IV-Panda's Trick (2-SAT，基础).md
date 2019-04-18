# POJ  3207  Ikki's Story IV - Panda's Trick  (2-SAT，基础) - xcw0754 - 博客园
# [POJ  3207  Ikki's Story IV - Panda's Trick  (2-SAT，基础)](https://www.cnblogs.com/xcw0754/p/4632977.html)
题意：
　　有一个环，环上n个点，现在在m个点对之间连一条线，线可以往圆外面绕，也可以往里面绕，问是否必定会相交？
思路：
　　根据所给的m条边可知，假设给的是a-b，那么a-b要么得绕环外，要么只能在环内，除非a和b是连续的点才不会影响到任何弧，否则一定会多少影响其他弧的走势。比如样例所举出的 0-2和 1-3，就必须有一条弧选择外环，一个选择内环。
　　如何使他们有序不冲突呢？其实这题跟环上的点没多大关系，而且给的点数n也没什么用，而倒是边才重要。我们要做的是让边不冲突。那么肯定是要先选出可能冲突的边来处理，让他们有序而不冲突。
　　那么什么时候会冲突？上面所讲的那样就会冲突了，所以我们要将有可能冲突的边全部找出来，那些连边都没有的点是没有用的，连边都没有，谈什么冲突。
　　根据冲突边来建图？若有两条边x和y冲突了，那么一条必须在内，另一条相反。则按照2-SAT的模型，选择了x在内，y则必须在外；选择了y在内，则x必须在外。将x放在环外和环内作为两个点，y也同理，将他们连上对应的有向边 “内x->外y”（可以用2*x和2*x+1这种方式代替环内环外的）。这里是以边作为点的。即每条边有两种可能，但是不能同时选，这和2-SAT模型一样。根据这个模型可以建图了。
　　求解方式？建图后用tarjan算法求强连通分量，根据边不能同时在环外和环内的规则，如果发现同时在环外/内就判其实无解的。
```
1 #include <iostream>
 2 #include <stdio.h>
 3 #include <string.h>
 4 #include <vector>
 5 #include <stack>
 6 #include <algorithm>
 7 #define LL long long
 8 #define INF 0x7f7f7f7f
 9 using namespace std;
10 const int N=500+5;
11 vector<int> vect[N*2];
12 int e[N][2];
13 int lowlink[N*2], dfn[N*2], scc_no[N*2], dfn_clock, scc_cnt;
14 stack<int> stac;
15 
16 void DFS(int x)
17 {
18     stac.push(x);
19     dfn[x]=lowlink[x]=++dfn_clock;
20     for(int i=0; i<vect[x].size(); i++)
21     {
22         int t=vect[x][i];
23         if(!dfn[t])
24         {
25             DFS(t);
26             lowlink[x]=min(lowlink[x],lowlink[t]);
27         }
28         else if(!scc_no[t])
29             lowlink[x]=min(lowlink[x],dfn[t]);
30     }
31     if(lowlink[x]==dfn[x])
32     {
33         scc_cnt++;
34         while(true)
35         {
36             int t=stac.top();stac.pop();
37             scc_no[t]=scc_cnt;
38             if(t==x)    break;
39         }
40     }
41 }
42 
43 int cal(int up)
44 {
45     scc_cnt=dfn_clock=0;
46     memset(scc_no,0,sizeof(scc_no));
47     memset(lowlink,0,sizeof(lowlink));
48     memset(dfn,0,sizeof(dfn));
49 
50     for(int i=0; i<up; i++)    if(!dfn[i])    DFS(i);   //求强连通分量
51 
52     for(int i=0; i<up; i+=2)    if(scc_no[i]==scc_no[i+1])    return false; //同在一个强连通分量内
53 
54     return true;
55 }
56 
57 
58 int main()
59 {
60     //freopen("input.txt", "r", stdin);
61     //仅有1个测试例子
62     int n, m, a, b;
63 
64     scanf("%d%d",&n,&m);
65     for(int i=0; i<m; i++)
66     {
67         scanf("%d%d",&e[i][0],&e[i][1]);
68         if(e[i][0]>e[i][1]) swap(e[i][0],e[i][1]);  //保证小在前
69     }
70 
71     for(int i=0; i<m; i++)  //穷举每条弧与其他弧的关系：相当于以点建图
72     {
73         a=e[i][0];
74         b=e[i][1];
75         for(int j=i+1; j<m; j++)
76         {
77             int c=e[j][0];
78             int d=e[j][1];
79             if( a<c && c<b && b<d || c<a && a<d && d<b ) //弧冲突了  a<c<b<d || c<a<d<b
80             {
81                 vect[i*2].push_back(j*2+1);    //只有错开才能无冲突
82                 vect[j*2].push_back(i*2+1);
83                 vect[i*2+1].push_back(j*2);
84                 vect[j*2+1].push_back(i*2);
85             }
86         }
87     }
88     if(cal(m*2))    puts("panda is telling the truth...");
89     else         puts("the evil panda is lying again");
90     return 0;
91 }
AC代码
```

