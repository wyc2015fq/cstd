# HDU 3342 Legal or Not (图是否有环) - xcw0754 - 博客园
# [HDU 3342 Legal or Not (图是否有环)](https://www.cnblogs.com/xcw0754/p/4586873.html)
题意：
　　给出n个人的师徒关系，如有 a是b的师傅，b是c的师傅，c是a的师傅，这样则不合法，输出NO，否则输出YES。
思路：
　　每段关系可以看成一条有向边，从师傅指向徒弟，那么徒弟的徒子徒孙都不可能再指向其师傅或自己，所以不可能构成环。
两种方法 ：
1，用拓扑的方法，每次去掉一个入度为0的点，全部点都去掉就是合法的。（下面代码用此法）
2，用深搜，记录路径，当搜到走过的路径上的点就是有环。每个点都可能重复搜多次，因为这不是树。
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 const int N=105;
 4 int n, m;
 5 bool vis[N];    //遍历
 6 int num[N];      //入度数
 7 vector< vector<int> > vect;
 8 deque<int> que;
 9 
10 int cal()
11 {
12     que.clear();
13     for(int i=0; i<n; i++)    if(!num[i])    que.push_back(i);  //挑出入度为0的
14 
15     while(!que.empty())
16     {
17         int p=que.front();
18         que.pop_front();
19         vis[p]=true;
20         for(int i=0,q; i<vect[p].size(); i++  )
21         {
22             q=vect[p][i];
23             num[q]--;    //这条边删掉
24             if(!num[q])    que.push_back(q);
25         }
26     }
27 
28     for(int i=0; i<n; i++)     if(!vis[i])    return 0;
29     return 1;
30 }
31 
32 int main()
33 {
34     int a, b;
35    // freopen("input.txt", "r", stdin);
36     while(scanf("%d%d",&n,&m),n)
37     {
38         vect.clear();
39         vect.resize(n);
40         memset(num, 0, sizeof(num));
41         memset(vis,0,sizeof(vis));
42         for(int i=0; i<m; i++)
43         {
44             scanf("%d%d",&a,&b);
45             vect[a].push_back(b);   //有向图
46             num[b]++;
47         }
48 
49         if(cal())    printf("YES\n");
50         else    printf("NO\n");
51 
52     }
53     return 0;
54 }
AC代码
```

