# HDU 4274 Spy's Work （树形DP，模拟） - xcw0754 - 博客园
# [HDU 4274 Spy's Work （树形DP，模拟）](https://www.cnblogs.com/xcw0754/p/4836380.html)
题意：　
　　给定一棵树，每个节点代表一个员工，节点编号小的级别就小，那么点1就是boss了。接下来给出对m个点的限制，有3种符号分别是op=“大于/小于/等于”，表示以第i个点为根的子树所有人的工资之和 大于/小于/等于 x，要求判断m个限制是否冲突了。注意每个员工的工资下限是1，而无上限。ps：可能出现对同个点多个限制，注意取交集。
思路：
　　很水的题嘛，想复杂了。注意限制是针对整棵子树的！所以我们只需要算出这棵子树的范围，再判断是否和所给的限制有冲突，如果没有冲突的话还得取“所给限制”与“计算出的范围”的交集。在输入m个限制的时候注意可能已经冲突了，需先提前判断一下。注意可能需要用longlong，初始化时就将每个人的范围限制在[1,INF)，这样就完事了~
```
1 //#include <bits/stdc++.h>
  2 #include <iostream>
  3 #include <cstdio>
  4 #include <cstring>
  5 #include <cmath>
  6 #include <map>
  7 #include <deque>
  8 #include <algorithm>
  9 #include <vector>
 10 #include <iostream>
 11 #define pii pair<int,int>
 12 #define max(x,y) ((x)>(y)?(x):(y))
 13 #define min(x,y) ((x)<(y)?(x):(y))
 14 #define INF 0x7f7f7f7f
 15 #define LL  long long
 16 using namespace std;
 17 const double PI  = acos(-1.0);
 18 const int N=10100;
 19 bool isWA;
 20 int n, edge_cnt, head[N];
 21 LL down[N], up[N];
 22 struct node
 23 {
 24     int from, to, next;
 25     node(){};
 26     node(int from,int to,int next):from(from),to(to),next(next){};
 27 }edge[N];
 28 
 29 void add_node(int from,int to)
 30 {
 31     edge[edge_cnt]=node(from,to,head[from]);
 32     head[from]=edge_cnt++;
 33 }
 34 
 35 bool DFS(int t)
 36 {
 37     node e;
 38     LL L=1, R=INF;        //计算此子树的范围
 39     for(int i=head[t]; i!=-1; i=e.next)
 40     {
 41         e=edge[i];
 42         if(DFS(e.to)==false)    return false;
 43         L+=down[e.to];
 44     }
 45 
 46     if(head[t]==-1)    //叶子，只要在[1,INF)都是合法的
 47     {
 48         if(up[t]>0)  return true;
 49         else         return false;
 50     }
 51     else
 52     {
 53         if( R<down[t] || L>up[t]  )    return false;    //无交集
 54         down[t]=max(down[t], L);        //更新本子树的范围
 55         up[t]=min(up[t], R);
 56         return true;
 57     }
 58 }
 59 
 60 
 61 void init(int n)
 62 {
 63     for(int i=0; i<=n; i++) //注意初始化问题
 64         down[i]=1,
 65         up[i]=INF,
 66         head[i]=-1;
 67     isWA=false;
 68     edge_cnt=0;
 69 }
 70 
 71 int main()
 72 {
 73     //freopen("input.txt", "r", stdin);
 74     int n, m, a, d;char b,c;
 75     while(~scanf("%d",&n))
 76     {
 77         init(n);
 78         for(int i=2; i<=n; i++)
 79         {
 80             scanf("%d",&a);
 81             add_node(a,i);
 82         }
 83         scanf("%d",&m);
 84 
 85         for(int i=0,L,R; i<m; i++)
 86         {
 87             scanf("%d%c%c%d",&a,&b,&c,&d);
 88             L=1, R=INF;
 89             if(c=='<')         R=d-1;
 90             else if(c=='>')    L=d+1;
 91             else               L=R=d;
 92             if(L>up[a] || R<down[a])  isWA=true;//所给条件可能已经冲突
 93             down[a]=L, up[a]=R;
 94         }
 95         if(!isWA && DFS(1))  puts("True");
 96         else                 puts("Lie");
 97     }
 98 
 99     return 0;
100 }
AC代码
```

