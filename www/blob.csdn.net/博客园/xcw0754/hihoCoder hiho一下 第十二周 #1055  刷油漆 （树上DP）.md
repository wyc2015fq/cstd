# hihoCoder  hiho一下 第十二周  #1055 : 刷油漆  （树上DP） - xcw0754 - 博客园
# [hihoCoder  hiho一下 第十二周  #1055 : 刷油漆  （树上DP）](https://www.cnblogs.com/xcw0754/p/4441313.html)
思路：
　　只能刷部分节点数m，总节点数n。如果m>=n那么就可以全刷了，那就不用任何算法了。如果m<n那么就要有取舍了。用DP思路，记录下每个节点如果获得到1~m个选择所能获得的最大权值。这里的树不仅仅是二叉，可能是多叉。所以一个节点怎么合理分配给每个孩子的空间大小很关键，当第一个孩子获得了1空间，那么后面的孩子所能获得的空间就要少一个了。这样穷举每个孩子能得到的空间，来求当前节点的最大权值，会有很多种可能，m！。不用这么做。
递归步骤：
　　假设状态记录为 dp[节点][该节点所获得的空间大小]　　　　注：该节点所获得的空间大小可能从1~m，m指该节点在树的第几层。
（1）如果当前节点最多只能获得1的空间，那么就不用计算大于1的可能了，仅需计算他自己，也就是dp[自己][1]。这个节点就是根节点的广度搜索的第m层（m是根节点所能获得的空间大小）。
（2）对于当前节点node，所能获得的最大空间m，那么需要先计算出所有孩子的数据先，不然怎么计算node的最大权值。要知道，node的最大权值都是靠其孩子来支撑的。所以这一步先将m减1，然后m-1全部给孩子，孩子就能计算出从1~m-1的可能了。
（3）要更新当前节点node自己的所有数据了，也就是dp[node][1~m]。考虑到穷举会有m!种可能。得找另外的办法解决：考虑到当前节点的所有数据全靠孩子，那么对所有孩子逐个考虑，假设孩子为i。接着要考虑更新node的第j个数据，j=m...1，就是从大到小来更新（有点像01背包），必须从大到小，因为每当我们更新第j个数据dp[node][j]时，需要用到前面dp[node][<j]的值，假如从小到大，那么当考虑到dp[node][j]时，前面dp[node][<j]中已经包含了第i个孩子的数据了，这样就造成了重复，比如dp[node][2]=10，这里面取第1个孩子和node节点，接着dp[node][4]=max(dp[node][4], dp[node][4-2]+dp[第1个孩子][2])= max(dp[node][4], dp[node][2]+dp[第1个孩子][2])  表示给第一个1孩子2个空间所能获得大小，而dp[node][2]已经取了第1个孩子，dp[第1个孩子][2]又取该孩子一次，那么node的第1个孩子就被收藏了两次。重复了。正确的循环如下代码
```
1 #include <iostream>
 2 #include <cstring>
 3 #include <vector>
 4 #include <stdio.h>
 5 using namespace std;
 6 
 7 const int N2=110;   //平方
 8 const int N3=1100;  //立方
 9 int value[N3];        //权值
10 vector< vector<int> > edge(N2);   //边
11 int dp[N2][N3];         //dp值
12 
13 int tree_dp( int node ,int m )
14 {
15     if(m==1)        //如果分给node为1个空间，那只能收获他自己而已
16     {
17         dp[node][m]=value[node];
18         return 0;
19     }
20 
21     //递归计算出所有孩子
22     for(int i=0; i<edge[node].size(); i++)  //对于每个孩子
23         tree_dp(edge[node][i], m-1 );
24 
25     //更新本节点的所有可能性
26     dp[node][1]=value[node];
27     for(int i=0; i<edge[node].size(); i++)//考虑每个孩子，也是为了防止重复
28         for(int j=m; j>1; j--)      //必须从大到小，防止重复选取。
29             for(int k=1; k<j; k++)  //记得要留一个给自己
30                 dp[node][j] =max( dp[node][j], dp[node][j-k] + dp[edge[node][i]][k]);//状态方程
31     return 0;
32 }
33 int main()
34 {
35     //freopen("input.txt", "r", stdin);
36     int n, m, ai, bi;
37     while(cin>>n>>m)
38     {
39         memset(dp, 0, sizeof(dp));
40         for(int i=0; i<N2; i++)     //清除边
41             edge[i].clear();
42 
43         for(int i=1; i<=n; i++)     //n个权值
44             cin>>value[i];
45 
46         for(int i=1; i<n; i++)      //n-1条边
47         {
48             cin>>ai>>bi;
49             edge[ai].push_back(bi);
50         }
51         tree_dp(1,m);
52         cout<<dp[1][m]<<endl;
53     }
54     return 0;
55 }
刷油漆
```

