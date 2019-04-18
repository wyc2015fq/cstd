# UVALive 3211 Now or later(2-SAT，二分，Kosaraju) - xcw0754 - 博客园
# [UVALive 3211 Now or later(2-SAT，二分，Kosaraju)](https://www.cnblogs.com/xcw0754/p/4635464.html)
题意：
　　有n个飞机要降落，每机都可以在两个时间点上选择降落。但是两机的降落时间间隔太小会影响安全性，所以，要求两机的降落时间应该达到最大，当然也不能冲突了。问最大的时间间隔是多少？（其实问的是max(每种方案中两机间的最小间隔) ）
思路：
　　二分穷举每个时间间隔，对于每个间隔，建反向图，对图进行DFS着色看是否有冲突，无冲突的话证明此间隔是可以实现的。要找一个可以实现的，且间隔最大的。
```
1 #include <iostream>
 2 #include <stdio.h>
 3 #include <string.h>
 4 #include <vector>
 5 #include <stack>
 6 #include <algorithm>
 7 #include <bits/stdc++.h>
 8 #define LL long long
 9 #define pii pair<int,int>
10 #define INF 0x7f7f7f7f
11 using namespace std;
12 const int N=2000+5;
13 int n, s[N*2], c=0, col[N*2], early[N], late[N];
14 vector<int> vect[N*2];
15 
16 
17 bool color(int x)
18 {
19     if(col[x^1])    return false;   //(x,x^1)这一对已经选好了x^1，你还想选x，肯定矛盾。
20     if(col[x])      return true;    //你要选的，刚好已选。
21     col[x]=true;
22     s[c++]=x;
23     for(int i=0; i<vect[x].size(); i++)
24     {
25         int t=vect[x][i];
26         if(!color(t))   return false;  //染色不成功
27     }
28     return true;
29 }
30 
31 
32 int cal(int dis)
33 {
34     for(int i=0; i<n*2; i++)    vect[i].clear();
35     for(int i=0; i<n; i++)  //建图
36     {
37         for(int j=0; j<n; j++)
38         {
39             if(i==j)    continue;
40             if( abs(early[i]-early[j])<dis )    vect[j*2+1 ].push_back(i*2 );
41             if( abs(early[i]-late[j]) <dis )    vect[j*2 ].push_back(i*2 );
42             if( abs(late[i] -early[j])<dis )    vect[j*2+1].push_back(i*2+1);
43             if( abs(late[i] -late[j]) <dis )    vect[j*2].push_back(i*2+1);
44         }
45     }
46     memset(col,0,sizeof(col));  //所有着色的都是要的
47     memset(s,0,sizeof(s));      //作为栈记录此次失败所标记过的，回头只需将栈中的点去掉标记
48     for(int i=0; i<n*2; i+=2)
49     {
50         if(!col[i] && !col[i+1]) //都还未着色，如果有1个已经着色，那这对就已经选好了
51         {
52             c=0;
53             if( !color(i))
54             {
55                 while(c)    col[s[--c]]=0;    //选i是不行的，要清除刚才的着色。
56                 if(!color(i+1)) return false;   //i不行，就从i+1开始着色试试。
57             }
58         }
59     }
60     return true;
61 }
62 
63 int main()
64 {
65     freopen("input.txt", "r", stdin);
66     int a, b, up;
67     while(~scanf("%d",&n))
68     {
69         memset(early,0,sizeof(early));
70         memset(late,0,sizeof(late));
71         up=0;
72         for(int i=0; i<n; i++)
73         {
74             scanf("%d%d",&early[i],&late[i]);
75             up=max(late[i],up);
76         }
77 
78         int l=0, r=up;
79         while(l<=r)
80         {
81             int mid=(l+r)/2;
82             if(cal(mid))   l=mid+1;
83             else    r=mid-1;
84         }
85         printf("%d\n",l-1);
86     }
87 
88     return 0;
89 }
AC代码
```

