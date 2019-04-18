# UVA 11865 Stream My Contest 组网 （朱刘算法，有向生成树，树形图） - xcw0754 - 博客园
# [UVA 11865 Stream My Contest 组网 （朱刘算法，有向生成树，树形图）](https://www.cnblogs.com/xcw0754/p/4716571.html)
题意：
　　给n个点编号为0~n-1，0号点为根，给m条边（含自环，重边），每条边有个代价，也有带宽。给定c，问代价不超过c，树形图的最小带宽的最大值能达到多少？
思路：
　　点数才60，而带宽范围也不大，可以进行二分穷举最小带宽，将小于穷举值的边“禁用”，进行求树形图！只要能求得树形图，说明带宽还能继续往上提。
　　注：如果只有根，即n=1，那么输出其中最小的一条边的带宽。而求树形图比较简单，用朱刘算法O(n*m)，网上有模板。
　　朱刘算法的精髓在于：缩点就缩得彻底，将原来的图都给改掉了。如果产生了环，那就缩点，缩点后，属于原来环上的边u-v就会变成一条自环（没用的）。而找环的技术也经典，只是复杂度看起来不止O(n)了，随着环越来越少，复杂度越高。
```
1 #include <bits/stdc++.h>
  2 #define INF 0x7f7f7f7f
  3 #define pii pair<int,int>
  4 #define LL long long
  5 using namespace std;
  6 const int M=10010;
  7 const int N=100;
  8 
  9 struct node
 10 {
 11     int from,to;
 12     int bps,cost;
 13     node(){};
 14     node(int from,int to,int bps,int cost):from(from),to(to),bps(bps),cost(cost){};
 15 }edge[M], cpy[M];
 16 int edge_cnt, n, m, c;
 17 
 18 void add_node(int from,int to,int bps,int cost)
 19 {
 20     edge[edge_cnt]=node(from, to, bps, cost);
 21     cpy[edge_cnt]=node(from, to, bps, cost);//拷贝
 22     edge_cnt++;
 23 }
 24 
 25 int far[N], In[N], ID[N], vis[N];
 26 
 27 bool MST(int bps,int cost,int n,int root)
 28 {
 29     LL ans=0;
 30     while(true)
 31     {
 32         //找最小边
 33         for(int i=0; i<n; i++)  In[i]=INF;
 34         for(int i=0; i<edge_cnt; i++)
 35         {
 36             int u=edge[i].from;
 37             int v=edge[i].to;
 38             if(edge[i].cost<In[v] && u!=v && edge[i].bps>=bps)
 39             {
 40                 far[v]=u;
 41                 In[v]=edge[i].cost;
 42             }
 43         }
 44 
 45         //判断是否与生成树
 46         for(int i=0; i<n; i++)
 47         {
 48             if(i==root)     continue;
 49             if(In[i]==INF)  return false;    //缩点后root可能不为0
 50         }
 51 
 52         //找环
 53         int cntnode=0;
 54         memset(ID,  -1, sizeof(ID));     //ID保存的是环的编号
 55         memset(vis, -1, sizeof(vis));
 56         In[root]=0;
 57         for(int i=0; i<n; i++)
 58         {
 59             ans+=In[i];
 60             int v=i;
 61             while( vis[v]!=i && ID[v]==-1 && v!=root )  //用vis记录本次寻根所走过的点
 62             {
 63                 vis[v]=i;
 64                 v=far[v];
 65             }
 66             if(ID[v]==-1 && v!=root)
 67             {
 68                 for(int u=far[v]; u!=v; u=far[u])
 69                 {
 70                     ID[u]=cntnode;
 71                 }
 72                 ID[v]=cntnode++;
 73             }
 74         }
 75 
 76         if(cntnode==0)  break;
 77 
 78         for(int i=0;i<n;i++)   //不成环的点，单独作为1个环
 79             if(ID[i]==-1)    ID[i]=cntnode++;
 80 
 81         //有环
 82         for(int i=0; i<edge_cnt; i++)
 83         {
 84             int v=edge[i].to;
 85             edge[i].from =ID[edge[i].from];  //真的缩点,只不过边数仍然一样多
 86             edge[i].to =ID[edge[i].to];
 87             if(edge[i].from!=edge[i].to)
 88             {
 89                 edge[i].cost-=In[v];    //如果u和v不是同个环，那么v的入边的大小得改了。
 90             }
 91         }
 92         n=cntnode;
 93         root=ID[root];
 94     }
 95     if(ans<=cost)    return  true;
 96     else    return false;
 97 }
 98 
 99 
100 int cal(int L,int R,int root)
101 {
102     while(L<R)    //二分答案
103     {
104         for(int i=0; i<edge_cnt; i++)    edge[i]=cpy[i];    //注意要复制，因为原来的已经改变了
105 
106         int mid=L+(R-L+1)/2;
107         if( MST(mid, c, n, root) )  L=mid;
108         else       R=mid-1;
109     }
110     return L;
111 }
112 
113 int main()
114 {
115     //freopen("input.txt", "r", stdin);
116     int t, u, v, a, b;
117     cin>>t;
118     while(t--)
119     {
120         scanf("%d%d%d", &n, &m, &c);
121         edge_cnt=0;
122         int down=INF, up=0;     //找最大和最小进行二分
123 
124         for(int i=0; i<m; i++)
125         {
126             scanf("%d%d%d%d", &u, &v, &a, &b);
127             down=min(down, a);
128             up=max(up, a);
129             add_node(u, v, a, b);
130         }
131 
132         if(!MST(down, c, n, 0)) printf("streaming not possible.\n");
133         else      printf("%d kbps\n",  cal(down, up, 0));   //只有根的情况，输出down
134     }
135     return 0;
136 }
AC代码
```

