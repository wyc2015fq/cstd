# hihoCoder  #1176 : 欧拉路·一 （简单） - xcw0754 - 博客园
# [hihoCoder  #1176 : 欧拉路·一 （简单）](https://www.cnblogs.com/xcw0754/p/4584523.html)
题意：给出n个岛，每个岛都有桥到达其他岛，且桥数可多可少（即使两岛有多桥），判断是否是欧拉路（即每条桥只能走一次，所有桥都能遍历1遍）。
思路：
满足如下条件之一者即为欧拉路：
1、连通图，每个岛的度数为偶数。
2、连通图，其中仅两个岛的度数为奇数，其他都是偶数。
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 const int N=10005;
 4 vector< vector<int> >   vect;
 5 bool vis[N];
 6 int n,m,a,b;
 7 
 8 
 9 void DFS(int t) //遍历一遍所有点
10 {
11     vis[t]=true;
12     for(int i=0; i<vect[t].size(); i++)
13     {
14         if(!vis[vect[t][i]])
15             DFS(vect[t][i]);
16     }
17 }
18 
19 
20 
21 bool check()    //判断是否为连通图
22 {
23     for(int i=1; i<=n; i++)
24         if(!vis[i])
25             return false;
26     return true;
27 }
28 
29 
30 bool conj() //判断是否为两个奇数/全偶数情况。
31 {
32     int odd=0, even=0;
33     for(int i=1; i<=n; i++)
34         if((vect[i].size()&1)==1)   //奇数
35             odd++;
36         else    even++;
37     if(odd==2||odd==0)    return  true;
38     else    return false;
39 }
40 
41 int main()
42 {
43     //freopen("input.txt","r",stdin);
44     vector<int> tmp;
45     cin>>n>>m;
46     for(int i=0; i<=n; i++)    vect.push_back(tmp);
47     for(int i=0; i<m; i++)
48     {
49         scanf("%d%d",&a,&b);
50         vect[a].push_back(b);
51         vect[b].push_back(a);
52     }
53     DFS(1);
54     if(check()==false)    printf("Part\n");
55     else if(conj())    printf("Full\n");
56     else    printf("Part\n");
57 
58     return 0;
59 }
AC代码
```

