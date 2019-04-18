# hihoCoder #1181: 欧拉路·二 （输出路径） - xcw0754 - 博客园
# [hihoCoder #1181: 欧拉路·二 （输出路径）](https://www.cnblogs.com/xcw0754/p/4584537.html)
题意：
　　给定一个图，要求打印出任一条欧拉路径（保证图肯定有欧拉路）。
思路：
　　深搜的过程中删除遍历过的边，并在回溯时打印出来。在深搜时会形成多个环路，每个环都有一个或多个结点与其他环相扣，这样就可以产生欧拉路径。
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 const int N=1005;
 4 int n, m, a, b;
 5 vector< vector<int> > vect;//邻接表
 6 void fleury(int t)
 7 {
 8     while(!vect[t].empty())
 9     {
10         int p=vect[t][0];
11         vect[t].erase(vect[t].begin());
12         for(int j=0; j<vect[p].size(); j++)//无向图，两边都要删
13         {
14             if(vect[p][j]==t)
15             {
16                 vect[p].erase(vect[p].begin()+j);
17                 break;  //只删一个
18             }
19         }
20         fleury(p);
21     }
22     printf("%d ", t);//回溯时打印
23 }
24 int main()
25 {
26     //freopen("e://input.txt", "r", stdin);
27     scanf("%d%d", &n, &m);
28     vector<int> tmp;
29     for(int i=0; i<=n; i++) vect.push_back(tmp);
30     for(int i=0; i<m; i++)
31     {
32         scanf("%d%d", &a, &b);
33         vect[a].push_back(b);
34         vect[b].push_back(a);
35     }
36 
37     int i=1;
38     while(i<n && !(vect[i].size()&1)) i++;//找到一个奇数为起点，没奇数则以n为起点
39     fleury(i);
40 
41     return 0;
42 }
AC代码
```

