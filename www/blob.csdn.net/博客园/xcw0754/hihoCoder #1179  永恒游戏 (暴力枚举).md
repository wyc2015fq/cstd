# hihoCoder #1179 : 永恒游戏 (暴力枚举) - xcw0754 - 博客园
# [hihoCoder #1179 : 永恒游戏 (暴力枚举)](https://www.cnblogs.com/xcw0754/p/4584511.html)
题意：
　　给出一个有n个点的无向图，每个点上有石头数个，现在的游戏规则是，设置某个点A的度数为d，如果A点的石子数大于等于d，则可以从A点给每个邻接点发一个石子。如果游戏可以玩10万次以上，输出INF，否则输出最多能玩几次。
思路：　　
　　暴力枚举每个可以玩的点，假如可以玩无限次，且当前状态为Z（指所有点的石头数的序列作为一个状态），那么在玩了多次之后，一定会造成循环，也就是说，玩几次之后，每个点的石子数和初始的石子数一模一样，这样子我再重复之前是怎么玩的就可以无限玩了。但是由于有200个点，所以玩一次就去判断是否和初始一样的状态，否则复杂度为 O(100000*结点数*该结点的边数)，数量级上亿。
　　现在只需要枚举10万次就知道结果了，不用去判断其他的。而且每次选择要玩的点都可以任意选，只要满足了条件（并不需要先玩石子数多的，或少的）。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 using namespace std;
 4 const int N=205;
 5 int n, m, a, b, num[N];
 6 vector< vector<int> >   vect;
 7 unordered_set<int> sett;
 8 
 9 int cal()
10 {
11     int cnt=0;
12     for(int i=0; i<n; i++)    if(num[i]>=vect[i].size())    sett.insert(i);    //符合条件
13     while(!sett.empty())
14     {
15         cnt++;              //一次
16         if(cnt>=100000) return cnt;
17         int q=*sett.begin();
18         num[q]-=vect[q].size();
19         if(num[q]<vect[q].size())  sett.erase(sett.begin()); //这个点已经不能再玩了
20 
21         for(int i=0; i<vect[q].size(); i++) //往邻居加石头
22         {
23             int p=vect[q][i];  //栈顶结点所连的点
24             num[p]++;
25             if(num[p]>=vect[p].size() ) sett.insert(p);
26         }
27     }
28     return cnt;
29 }
30 
31 int main()
32 {
33     //freopen("e://input.txt", "r", stdin);
34     scanf("%d%d",&n,&m);
35     for(int i=0; i<n; i++) scanf("%d", &num[i]); //石头数
36     vect.resize(n);
37     for(int i=0; i<m; i++)
38     {
39         scanf("%d%d", &a, &b);    //输入边，无向图
40         vect[a].push_back(b);
41         vect[b].push_back(a);
42     }
43 
44     int ans=cal();
45     if(ans>=100000)     printf("INF\n");
46     else                printf("%d\n",ans);
47 
48     return 0;
49 }
AC代码
```

