# HDU 1561  The more, The Better （树形DP，常规） - xcw0754 - 博客园
# [HDU 1561  The more, The Better （树形DP，常规）](https://www.cnblogs.com/xcw0754/p/4818842.html)
题意：给一个森林，n个节点，每个点有点权，问若从中刚好选择m个点（选择某点之前必须先选择了其父亲），使得这m个点权之和最大为多少？
思路：
　　比较常规。就是DFS一次，枚举在子树中可能选择的k个点（注意上限为min（子树节点数，到此子树最多可选节点数）），需要注意的是dp[t][1]必须是点t自己，枚举的时候必须先选择t才能选择t的孩子。但是本题是森林，那么可以建1个虚拟根编号为0（根输入一模一样），然后虚拟根的权为0即可，而所要选的数就变成m+1了。
```
1 #include <bits/stdc++.h>
 2 #define pii pair<int,int>
 3 #define max(x,y) (x>y?x:y)
 4 #define min(x,y) (x<y?x:y)
 5 #define INF 0x3f3f3f3f
 6 #define LL long long
 7 using namespace std;
 8 const int N=210;
 9 
10 struct node
11 {
12     int from,to,val,next;
13     node(){};
14     node(int from,int to,int val,int next):from(from),to(to),val(val),next(next){};
15 }edge[N];
16 int head[N], n, edge_cnt;
17 void add_node(int from,int to,int val)
18 {
19     edge[edge_cnt]=node(from, to, val, head[from]);
20     head[from]=edge_cnt++;
21 }
22 
23 int dp[N][N];
24 int DFS(int t,int m,int val)
25 {
26     if(m==0)    return 0;   //点数上限了。
27     dp[t][1]=val;           //只能挑1个点时，必须挑自己
28     node e;
29     int sum=1;
30     for(int i=head[t]; i!=-1&&m>1; i=e.next)
31     {
32         e=edge[i];
33         int tmp=DFS(e.to, m-1, e.val);  //最多可以在e.to子树中选多少个点
34         sum+=tmp;
35 
36         for(int j=sum; j>1; j--)
37             for(int k=1; k<=tmp&& k<j; k++) //保证j-k>=1，因为t是必选的
38                 if(dp[t][j-k]>=0)
39                     dp[t][j]=max(dp[t][j], dp[t][j-k]+dp[e.to][k]);
40     }
41     return sum; //返回在本子树中可以选的点数上限
42 }
43 
44 int main()
45 {
46     //freopen("input.txt", "r", stdin);
47     int a,b,m;
48     while(scanf("%d%d",&n,&m),n+m)
49     {
50         memset(head, -1, sizeof(head));
51         memset(dp, -1, sizeof(dp));
52         edge_cnt=0;
53 
54         for(int i=1; i<=n; i++)
55         {
56             scanf("%d%d",&a,&b);
57             add_node(a,i,b);
58         }
59         DFS(0, m+1, 0); //0是虚拟根
60         printf("%d\n", dp[0][m+1]);
61     }
62     return 0;
63 }
AC代码
```

