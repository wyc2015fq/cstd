# HDU 1548 A strange lift  奇怪的电梯(BFS，水) - xcw0754 - 博客园
# [HDU 1548 A strange lift  奇怪的电梯(BFS，水)](https://www.cnblogs.com/xcw0754/p/4550402.html)
题意：
　　有一座电梯，其中楼层从1～n，每层都有一个数字k，当处于某一层时，只能往上走k层，或者下走k层。楼主在a层，问是否能到达第b层？
思路：
　　在起点时只能往上走和往下走两个选择，之后的每层都是这样，那么就类似于二叉树。每个节点就是对应的层，因为有可能碰到循环的层，比如1跳到3,3跳回1,这样使得无限循环，所以加个vis数组标记是否遍历过即可。
```
1 #include <iostream>
 2 #include <cmath>
 3 #include <cstring>
 4 #include <cstdio>
 5 #include <deque>
 6 using namespace std;
 7 const int N=2008;
 8 int n, a, b;
 9 int q[N];
10 deque<int> que;
11 bool vis[N];
12 
13 int cal()
14 {
15     if(a==b)    return 0;
16     que.clear();
17 
18 
19     int cnt=0;
20     que.push_back(a);
21     while(!que.empty())
22     {
23         int siz=que.size();
24         for(int i=0; i<siz; i++)
25         {
26             int cur=que.front();
27             que.pop_front();
28             if(cur+q[cur]==b||cur-q[cur]==b)
29                 return cnt+1;
30 
31             if(cur+q[cur]<=n && !vis[cur+q[cur]] )    //没有遍历过的,小于上限的才考虑
32             {
33                 que.push_back(cur+q[cur]);
34                 vis[cur+q[cur]]=1;
35             }
36             if(cur-q[cur]>0 && !vis[cur-q[cur]] )    //没有遍历过的，大于0的才考虑
37             {
38                 que.push_back(cur-q[cur]);
39                 vis[cur-q[cur]]=1;
40             }
41         }
42         cnt++;
43     }
44     return -1;
45 }
46 
47 
48 int main()
49 {
50     //freopen("input.txt", "r", stdin);
51 
52     while(cin>>n,n)
53     {
54         memset(q,0,sizeof(q));
55         memset(vis,0,sizeof(vis));
56 
57         cin>>a>>b;
58         for(int i=1; i<=n; i++)
59             cin>>q[i];
60 
61         printf("%d\n",cal());
62     }
63 
64     return 0;
65 }
AC代码
```

