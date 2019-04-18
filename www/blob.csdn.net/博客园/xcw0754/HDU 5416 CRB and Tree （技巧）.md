# HDU 5416 CRB and Tree （技巧） - xcw0754 - 博客园
# [HDU 5416 CRB and Tree （技巧）](https://www.cnblogs.com/xcw0754/p/4746367.html)
题意：给一棵n个节点的树（无向边），有q个询问，每个询问有一个值s，问有多少点对(u,v)的xor和为s？ 注意：(u,v)和(v,u)只算一次。而且u=v也是合法的。
思路：任意点对之间的路径肯定经过LCA的，但是如果如果知道某个点t到根的路径xor和为e，那么就能够得知 x^e=s中的x应该是多少，那就算有多少点到根的xor和为x即可。e是表示t到根的，所以而x也是其他点到根的路径xor和，两个点他们的LCA到根这段会被算2次，那么xor就为0了。
　　（1）DFS算出每个点到根的路径xor和，相同的用一个桶装起来，复杂度O(n)。
　　（2）对于每个询问s，穷举树上n个点，找到另一个点到root的路径xor和，在对应桶里的都是可以组成s的。（注意要去重）
　　特别要注意的是，s可能为0，那么就有x^x=s的情况，另一个点也会在同一个桶里。还有，要用long long保存答案。
```
1 #include <bits/stdc++.h>
 2 #define pii pair<int,int>
 3 #define INF 0x7f7f7f7f
 4 #define LL long long
 5 using namespace std;
 6 const int N=100050;
 7 vector<int> vect[N];
 8 struct node
 9 {
10     int from,to,val;
11     node(){};
12     node(int from,int to,int val):from(from),to(to),val(val){};
13 }edge[N*2];
14 int edge_cnt, n, buck[N*2], self[N], vis[N], q;
15 
16 
17 void add_node(int from,int to,int val)
18 {
19     edge[edge_cnt]=node(from, to, val);
20     vect[from].push_back(edge_cnt++);
21 }
22 
23 void DFS(int x,int sum)
24 {
25     vis[x]=1;
26     for(int i=0; i<vect[x].size(); i++)
27     {
28         node &e=edge[vect[x][i]];
29         if( !vis[e.to])
30         {
31             buck[ self[e.to]=sum^e.val ]++;
32             DFS(e.to, sum^e.val);
33         }
34     }
35 }
36 
37 LL get_ans(int a)
38 {
39     LL ans=0;
40     if(a==0)
41     {
42         for(int j=1; j<=n; j++)
43         {
44             int t=self[j];          //a为0，这个桶肯定是自己的那个
45             ans+=buck[ t ]-1;       //先扣掉自己。但是仍然会算重了。它到别人，别人也会到他。
46         }
47         ans+=n*2; //每个点到自己都算。
48     }
49     else        //a!=0，那么不可能有两个相同数的异或和为a的。
50     {
51         for(int j=1; j<=n; j++)
52         {
53             int t=a^self[j];    //这个肯定不跟j同个桶。
54             ans+=buck[t];
55         }
56     }
57     return ans/2;   //去重
58 }
59 
60 void cal(int n)
61 {
62     memset(vis, 0, sizeof(vis));
63     memset(buck, 0, sizeof(buck));
64     memset(self, 0, sizeof(self));
65     DFS(0, 0);
66     cin>>q;
67     for(int i=0,a=0; i<q; i++)
68     {
69         scanf("%d", &a);
70         printf("%lld\n", get_ans(a));
71     }
72 }
73 
74 int main()
75 {
76     freopen("input.txt", "r", stdin);
77     int t, a, b, c;
78     cin>>t;
79     while(t--)
80     {
81         cin>>n;
82         edge_cnt=0;
83         for(int i=0; i<=n; i++) vect[i].clear();
84         for(int i=1; i<n; i++)
85         {
86             scanf("%d%d%d", &a, &b, &c);
87             add_node(a,b,c);
88             add_node(b,a,c);
89         }
90         add_node(0, 1, 0);  //虚拟的根节点，0号节点
91         cal(n);
92     }
93     return 0;
94 }
AC代码
```
数据
2
3
1 2 1
2 3 2
3
2
3
4
4
1 2 1
2 3 2
2 4 1
4
0
1
2
3
答案：
1
1
0
5
2
1
2

