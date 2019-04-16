# Nyoj   一笔画问题(图论) - Vmetrio - 博客园







# [Nyoj   一笔画问题(图论)](https://www.cnblogs.com/wangmengmeng/p/5295941.html)







描述

zyc从小就比较喜欢玩一些小游戏，其中就包括画一笔画，他想请你帮他写一个程序，判断一个图是否能够用一笔画下来。

规定，所有的边都只能画一次，不能重复画。




- 输入
第一行只有一个正整数N(N<=10)表示测试数据的组数。
每组测试数据的第一行有两个正整数P,Q(P<=1000,Q<=2000)，分别表示这个画中有多少个顶点和多少条连线。（点的编号从1到P）
随后的Q行，每行有两个正整数A,B(0<A,B<P)，表示编号为A和B的两点之间有连线。- 输出
如果存在符合条件的连线，则输出"Yes",
如果不存在符合条件的连线，输出"No"。- 样例输入
2
4 3
1 2
1 3
1 4
4 5
1 2
2 3
1 3
1 4
3 4- 样例输出
No
Yes


每个点对应的度  再用到欧拉回路


代码:

```
1 #include <vector>
 2 #include <map>
 3 #include <set>
 4 #include <algorithm>
 5 #include <iostream>
 6 #include <cstdio>
 7 #include <cmath>
 8 #include <cstdlib>
 9 #include <string>
10 #include <cstring>
11 #include <queue>
12 using namespace std;
13 
14 #define N 1010
15 int g[N][N];
16 bool vis[N];
17 
18 int bfs(int n)
19 {
20     queue<int> q;
21     int que,du,odd,i;  
22     memset(vis,0,sizeof(vis));  
23     q.push(1);  
24     vis[1] = 1;  
25     que = 0; //队列中点的个数   
26     odd = 0; //度为奇数的点的个数  
27     while(!q.empty()){
28         int top=q.front();
29         q.pop();  que++;
30         du=0;
31         for(int i=1; i<=n; i++){
32             if(g[top][i]){
33                 if(!vis[i]){
34                     q.push(i);
35                     vis[i]=1;
36                 }
37                 du++;
38             }
39         }
40         if(du&1)  odd++;
41     }
42     if((odd==0 || odd==2)&& que==n)  printf("Yes\n");
43     else    printf("No\n");
44 }
45 
46 int main()
47 {
48     int n;
49     int P,Q,p,q;
50     scanf("%d",&n);
51     while(n--){
52         memset(g,0,sizeof(g));
53         scanf("%d %d",&P,&Q);
54         while(Q--){
55             scanf("%d %d",&p,&q);
56             g[p][q]=g[q][p]=1;
57         }
58         bfs(P);
59     }
60 }
```














