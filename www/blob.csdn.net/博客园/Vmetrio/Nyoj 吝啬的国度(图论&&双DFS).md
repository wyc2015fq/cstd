# Nyoj  吝啬的国度(图论&&双DFS) - Vmetrio - 博客园







# [Nyoj  吝啬的国度(图论&&双DFS)](https://www.cnblogs.com/wangmengmeng/p/5297926.html)





描述在一个吝啬的国度里有N个城市，这N个城市间只有N-1条路把这个N个城市连接起来。现在，Tom在第S号城市，他有张该国地图，他想知道如果自己要去参观第T号城市，必须经过的前一个城市是几号城市（假设你不走重复的路）。


- 输入
第一行输入一个整数M表示测试数据共有M(1<=M<=5)组
每组测试数据的第一行输入一个正整数N(1<=N<=100000)和一个正整数S(1<=S<=100000)，N表示城市的总个数，S表示参观者所在城市的编号
随后的N-1行，每行有两个正整数a,b(1<=a,b<=N)，表示第a号城市和第b号城市之间有一条路连通。- 输出
每组测试数据输N个正整数，其中，第i个数表示从S走到i号城市，必须要经过的上一个城市的编号。（其中i=S时，请输出-1）- 样例输入
1
10 1
1 9
1 8
8 10
10 3
8 6
1 2
10 4
9 5
3 7
- 样例输出
-1 1 10 10 9 8 3 1 1 8
- 来源
[经典题目](http://acm.nyist.edu.cn/JudgeOnline/search_result.php?source=%E7%BB%8F%E5%85%B8%E9%A2%98%E7%9B%AE)






代码:

```
1  
 2 #include <vector>
 3 #include <map>
 4 #include <set>
 5 #include <algorithm>
 6 #include <iostream>
 7 #include <cstdio>
 8 #include <cmath>
 9 #include <cstdlib>
10 #include <string>
11 #include <cstring>
12 #include <queue>
13 using namespace std;
14 #define INF 0x3f3f3f3f
15 #define MAX 5055
16 
17 vector<int> mp[100010];
18 int a[100010];
19 
20 void dfs(int n,int m)
21 {
22     for(int i=0; i<mp[n].size(); i++){
23         if(mp[n][i]!=m)
24             dfs(mp[n][i],a[mp[n][i]]=n);
25     }
26     return ;
27 }
28 
29 int main()
30 {
31     int M,n,s,x,y;
32     scanf("%d",&M);
33     while(M--){
34         memset(a,0,sizeof(a));
35         scanf("%d%d",&n,&s);
36         for(int i=0; i<n; i++)
37             mp[i].clear();
38         for(int i=1; i<n; i++){
39             scanf("%d%d",&x,&y);
40             mp[x].push_back(y);
41             mp[y].push_back(x);
42         }
43         a[s]=-1;
44         dfs(s,-1);
45         for(int i=1; i<=n; i++)
46             printf("%d ",a[i]);
47         printf("\n");
48     }
49 }
50
```














