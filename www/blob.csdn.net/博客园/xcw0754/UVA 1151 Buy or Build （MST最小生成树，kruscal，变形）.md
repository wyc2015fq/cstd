# UVA 1151 Buy or Build （MST最小生成树，kruscal，变形） - xcw0754 - 博客园
# [UVA 1151 Buy or Build （MST最小生成树，kruscal，变形）](https://www.cnblogs.com/xcw0754/p/4622533.html)
题意：
　　要使n个点之间能够互通，要使两点直接互通需要耗费它们之间的欧几里得距离的平方大小的花费，这说明每两个点都可以使其互通。接着有q个套餐可以选，一旦选了这些套餐，他们所包含的点自动就连起来了，所需要做的就是连上还未通的即可，q<=8。可以多买。求最小生成树所需的代价。
思路：
　　与普通求MST不同的就是多了套餐，而且还可以多买。每个套餐有买或不买两种可能，那么有28种可能，即256种。
　　如果不买套餐，至少需要求1次MST是确定的，这个复杂度已经是O(n*n)了。还得考虑哪些餐套可以搭配来买更便宜，那么就穷举这256种组合，每种组合来一次MST，但是不再需要O（n*n）了，只需要用第一次生成树时所挑出来的边即可。
　　具体做法是，将套餐内的所有点先连接（并查集），再用MST的边来一次kruscal（记得加上套餐费）。对于每个组合都这样做，就能求出结果了。
　　特别要注意：每两个输出结果之间要1个空行，末尾不需要再空行，否则出错。
```
1 #include <bits/stdc++.h>
  2 #define   LL long long
  3 using namespace std;
  4 const int N=1000+5;
  5 const int INF=0x7f7f7f7f;
  6 vector<int> vect[10];
  7 vector< pair<int,int> > cor, e, tree;
  8 int t, r, n, q, a, b;
  9 int cost[10],  pre[N],  g[N][N];;
 10 
 11 int cmp(pair<int,int> a,pair<int,int> b){return g[a.first][a.second]<g[b.first][b.second]? true: false;}//按照距离来排序
 12 int dis( pair<int,int> a,pair<int,int> b ){return  (a.first-b.first)*(a.first-b.first) +(a.second-b.second)*(a.second-b.second) ;}//不需要开方
 13 
 14 int find(int x){return pre[x]==x? x: pre[x]=find(pre[x]);}         //查
 15 void joint(int a,int b){a=find(a),b=find(b);if(a!=b)    pre[a]=b;} //并
 16 
 17 
 18 
 19 LL kruscal()  //将生成树的树边取出
 20 {
 21     for(int i=1; i<=n; i++)     pre[i]=i;
 22     int cnt=0;
 23     LL sum=0;
 24     for(int i=0; i<e.size(); i++)
 25     {
 26         int a=e[i].first;
 27         int b=e[i].second;
 28         if(find(a)!=find(b))
 29         {
 30             cnt++;
 31             tree.push_back(e[i]);   //收藏边
 32             sum+=g[a][b];           //统计权值
 33             joint(a,b); //a和b是点
 34             if(cnt>=n-1)    return sum;
 35         }
 36     }
 37     return sum;
 38 }
 39 
 40 
 41 LL kruscal_2()  //带套餐的
 42 {
 43     LL sum=0;
 44     for(int i=0; i<tree.size(); i++)
 45     {
 46         int a=tree[i].first;
 47         int b=tree[i].second;
 48         if(find(a)!=find(b))
 49         {
 50             sum+=g[a][b];
 51             joint(a,b);
 52         }
 53     }
 54     return sum;
 55 }
 56 
 57 LL cal()
 58 {
 59     sort(e.begin(), e.end(), cmp);
 60     tree.clear();
 61     LL ans=kruscal();      //第一次生成树,挑出有用边
 62     int choice=1;
 63     while(q--)  choice+=choice;
 64     for(int i=1; i<choice; i++)
 65     {
 66         for(int j=1; j<=n; j++)     pre[j]=j;
 67         int tmp=i, cnt=1;
 68         LL sum=0;
 69         while(tmp)      //先将欲买套餐的pre归类
 70         {
 71             if((tmp&1)==1)  //第cnt个套餐要了
 72             {
 73                 sum+=cost[cnt];
 74                 for(int j=1; j<vect[cnt].size(); j++)    joint(vect[cnt][j-1],vect[cnt][j]);
 75             }
 76             tmp>>=1;
 77             cnt++;
 78         }
 79         ans=min(ans, sum+kruscal_2());  //再生成树
 80     }
 81     return ans;
 82 }
 83 
 84 int main()
 85 {
 86     freopen("input.txt", "r", stdin);
 87     cin>>t;
 88     while(t--)
 89     {
 90         cin>>n>>q;
 91         for(int i=1; i<=q; i++)         //每个套餐
 92         {
 93             scanf("%d%d",&a,&cost[i]);
 94             vect[i].clear();
 95             while(a--)
 96             {
 97                 scanf("%d",&r);
 98                 vect[i].push_back(r);
 99             }
100         }
101         cor.clear();
102         for(int i=0; i<n; i++)
103         {
104             scanf("%d%d",&a,&b);
105             cor.push_back(make_pair(a,b));  //每个点的坐标
106         }
107 
108         memset(g, 0, sizeof(g));
109         e.clear();
110         for(int i=1; i<=n; i++)              //计算出距离
111         {
112             for(int j=i+1; j<=n; j++)
113             {
114                 g[i][j]=g[j][i]= dis(cor[i-1],cor[j-1]);
115                 e.push_back(make_pair(i,j));
116             }
117         }
118         cout<<cal()<<endl;
119         if(t) printf("\n");
120     }
121     return 0;
122 }
AC代码
```

