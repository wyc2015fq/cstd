# HDU 1213 How Many Tables (并查集，常规) - xcw0754 - 博客园
# [HDU 1213 How Many Tables (并查集，常规)](https://www.cnblogs.com/xcw0754/p/4607813.html)
并查集基本知识看：http://blog.csdn.net/dellaserss/article/details/7724401
题意：假设一张桌子可坐无限多人，小明准备邀请一些朋友来，所有有关系的朋友都可以坐同一张桌，没有关系的则要另开一桌，问需要多少张桌子（小明不坐，不考虑小明与其他人的关系）？
思路：常规的并查集。要求出所有人的老大，有几个老大就要几张桌子。那么有关系的都归为同一个老大。用数组实现，再顺便压缩路径。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 using namespace std;
 4 const int N=1005;
 5 int pre[N]; //结点i的上级
 6 inline void init(int n){for(int i=1; i<=n; i++) pre[i]=i;}  //将上级初始化为自己
 7 
 8 int find(int x) //找x的上级
 9 {
10     int k=x;
11     while(pre[x]!=x)    x=pre[x];   //找上级
12     int tmp;
13     while(pre[k]!=x) //优化:路径压缩
14     {
15         tmp=pre[k];
16         pre[k]=x;
17         k=tmp;
18     }
19     return x;
20 }
21 
22 void joint(int a,int b)  //将a和b合并为一个集合
23 {
24     a=find(a);
25     b=find(b);
26     if(a!=b)    pre[a]=b;
27 }
28 
29 int check(int n)    //查查到底需要几张桌子
30 {
31     set<int> sett;
32     for(int i=1; i<=n; i++)    find(i); //防漏网之鱼，将所有人的老大推到最顶。
33     for(int i=1; i<=n; i++)    sett.insert(pre[i]); 
34     return sett.size();
35 }
36 int main()
37 {
38     //freopen("e://input.txt", "r", stdin);
39     int t, n, m, a, b;
40     cin>>t;
41     while(t--)
42     {
43         scanf("%d%d", &n, &m);
44         init(n);
45         for(int i=0; i<m; i++)
46         {
47             scanf("%d%d",&a,&b);
48             joint(a,b);
49         }
50         printf("%d\n",check(n));
51     }
52     return 0;
53 }
AC代码
```

