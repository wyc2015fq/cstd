# hihoCoder #1174 : 拓扑排序·一 (判断循环图) - xcw0754 - 博客园
# [hihoCoder #1174 : 拓扑排序·一 (判断循环图)](https://www.cnblogs.com/xcw0754/p/4544347.html)
|G++|261ms|13MB|
题意：
　　给出n门课程的修读所需要的前置课程的关系，按理说应该是个拓扑图，但是因为某些原因导致了混乱，所以有可能不是一个拓扑图。现在的问题是，判断该图是否为一个拓扑图（即无环图）。
思路：
　　每次删除全部入度为0的结点，一直删下去肯定是没有任何点存在的，如果不是拓扑图的话就必有环，那么肯定有点的入度永远不为0。若到删到最后没有点存在，那么就是correct的。
```
1 #include<bits/stdc++.h>
 2 using namespace std;
 3 const int N=100100;
 4 
 5 vector< vector<int> >   vect;
 6 int cnt[N]; //记录每个点的入度
 7 
 8 bool cal(int n)
 9 {
10     vector<int> a;//存放入度为0的点
11     for(int i=1; i<=n; i++)    //先在cnt中找到入度为0的所有点
12         if(!cnt[i])    a.push_back(i);
13     vector<int> b;//临时存放入度为0的点
14     while(!a.empty())
15     {
16         b.clear();
17         for(int i=0; i<a.size(); i++)   //每个入度为0的点x
18         {
19             for(int j=0; j<vect[a[i]].size(); j++)  //每个与x相连的点
20             {
21                 cnt[vect[a[i]][j]]--;
22                 if(!cnt[vect[a[i]][j]])//只有那些有变化的点才可能入度为0。
23                     b.push_back(vect[a[i]][j]);
24             }
25             vect[a[i]].clear();
26         }
27         a.clear();
28         if(!b.empty())    a.insert(a.end(), b.begin(), b.end());
29     }
30     for(int i=1; i<=vect.size(); i++)
31     {
32         if(cnt[i]>0)
33             return false;
34     }
35     return true;
36 }
37 
38 void init(int n)    //初始化用的
39 {
40     memset(cnt,0,sizeof(cnt));
41     vect.clear();
42     vector<int> tmp;
43     for(int i=0; i<=n; i++)
44         vect.push_back(tmp);
45 }
46 int main()
47 {
48     //freopen("e://input.txt","r",stdin);
49     int t, n, m, a, b;
50     cin>>t;
51     while(t--)
52     {
53         scanf("%d%d",&n,&m);
54         init(n);
55         for(int i=0; i<m; i++)
56         {
57             scanf("%d%d",&a,&b);
58             vect[a].push_back(b);
59             cnt[b]++;
60         }
61         if(cal(n))
62             printf("Correct\n");
63         else
64             printf("Wrong\n");
65     }
66     return 0;
67 }
AC代码
```

