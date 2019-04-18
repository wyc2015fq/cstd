# HDU 2444 The Accomodation of Students (偶图判定，匈牙利算法) - xcw0754 - 博客园
# [HDU 2444 The Accomodation of Students (偶图判定，匈牙利算法)](https://www.cnblogs.com/xcw0754/p/4613719.html)
题意：
　　有一堆的学生关系，要将他们先分成两个组，同组的人都不互不认识，如果不能分2组，输出No。若能，则继续。在两组中挑两个认识的人（每组各1人）到一个双人房。输出需要多少个双人房？
思路:
　　先判定是否为二分图，可以用黑白着色法（DFS或BFS都行）。若是二分图，再进行匹配，用匈牙利算法，注：给的是整个图，没有区分男女，用邻接表比较好。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 using namespace std;
 4 const int N=205;
 5 vector< vector<int> > vect;
 6 int col[N];
 7 int girl[N];
 8 int vis[N];
 9 int n, m;
10 
11 bool color(const int x)   //x可达的点着色
12 {
13     for(int i=0; i<vect[x].size(); i++)
14     {
15         int t=vect[x][i];
16         if(col[x]==col[t])    return false;    //出错了，同色
17         if(!col[t])
18         {
19             col[t]=3-col[x];//性别标为1和2
20             if(color(t)==false)    return false;    //只有刚着色的才需要深搜
21         }
22     }
23     return true;   //只有单个点才会到这
24 }
25 
26 bool is2()//二分图判断，注意其可能并不是个连通图
27 {
28     memset(col,0,sizeof(col));
29     for(int i=1; i<=n; i++) //黑白着色
30     {
31         if(!col[i]) //可能有多个连通分量
32         {
33             col[i]=1;
34             if(!color(i))
35             {
36                 //cout<<i<<endl;
37                 return false;
38             }
39         }
40     }
41     return true;
42 }
43 
44 
45 int find(int x)
46 {
47     for(int i=0; i<vect[x].size(); i++)
48     {
49         int t=vect[x][i];
50         if(!vis[t])
51         {
52             vis[t]=1;
53             if(!girl[t] || find(girl[t]))   //未匹配，或能为其对象另匹配
54             {
55                 girl[t]=x;
56                 return true;
57             }
58         }
59     }
60     return false;
61 }
62 
63 int hungary()
64 {
65     int cnt=0;
66     memset(girl,0,sizeof(girl));
67     for(int i=1; i<=n; i++)
68     {
69         memset(vis,0,sizeof(vis));
70         if(find(i))    cnt++;
71     }
72     return cnt;
73 }
74 
75 int main()
76 {
77     //freopen("input.txt", "r", stdin);
78     int a, b, c;
79     while(cin>>n>>m)
80     {
81         vect.clear();
82         vector<int> tmp;
83         for(int i=0; i<=n; i++) vect.push_back(tmp);//切忌用resize，会惨死在这
84 
85         for(int i=0; i<m; i++)
86         {
87             scanf("%d%d",&a,&b);
88             vect[a].push_back(b);
89             vect[b].push_back(a);
90         }
91         if(!is2()&& (puts("No"),1) )    continue; //仅仅为了少一行代码
92         printf("%d\n",hungary()>>1);    //二分图匹配，匈牙利
93     }
94     return 0;
95 }
AC代码
```

