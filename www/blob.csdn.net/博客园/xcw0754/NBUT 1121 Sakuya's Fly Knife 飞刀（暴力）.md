# NBUT 1121 Sakuya's Fly Knife 飞刀（暴力） - xcw0754 - 博客园
# [NBUT 1121 Sakuya's Fly Knife 飞刀（暴力）](https://www.cnblogs.com/xcw0754/p/4544698.html)
题意：给出一个带有n*m个格子的矩阵，部分格子中有靶子target，现在要从一个没有靶子的格子中射出飞刀数把，飞刀是可穿透靶子的，同一直线上都可以一刀全射掉。现在问在哪个格子射出飞刀，可以在全部射中的情况下用刀次数最少？有target的格子不可选。
思路：穷举所有没有靶子的格子，计算每个靶子的角度，若角度相同就是可以一刀解决的。这里角度可能用的是实数的，用map解决。需要注意的是，在判断角度时，可能有重复，比如： 。t。。x。t。。 这里的x代表飞刀的出发点，‘t’代表靶子，那么两边的 t 计算出来的角度是相同的（按照靶子坐标的x/y来算角度即可）。需要做的只是对上下两边进行区分即可。
```
1 #include<bits/stdc++.h>
 2 using namespace std;
 3 bool tar[105][105];//标出每个靶子
 4 int x[105],y[105];//记录每个靶子的坐标
 5 map<double,int> mapp1,mapp2;
 6 
 7 void cal(int n,int m,int t)
 8 {
 9     int ans[3]={1000,1000,1000};//答案
10     for(int i=0; i<n; i++)
11     {
12         for(int j=0; j<m; j++)
13         {
14             if(!tar[i][j])  //对于每个非有刀格子
15             {
16                 mapp1.clear();
17                 mapp2.clear();
18                 int cnt_l=0,cnt_r=0;
19                 for(int k=0; k<t; k++)//扫描所有的target
20                 {
21                     if(y[k]>j)  //上边的
22                     {
23                         double q=(double)(y[k]-j)/(x[k]-i);
24                         if(mapp1[q]<=0)
25                             mapp1[q]=1;
26                     }
27                     else if(y[k]<j) //下边的
28                     {
29                         double q=(double)(y[k]-j)/(x[k]-i);
30                         if(mapp2[q]<=0)
31                             mapp2[q]=1;
32                     }
33                     else    //同一行的
34                     {
35                         if(x[k]>i)  //同行左边
36                             cnt_l=1;
37                         else    //同行右边
38                             cnt_r=1;
39                     }
40                 }
41                 if(ans[0]>cnt_l + cnt_r + mapp1.size()+mapp2.size())//有更少的，则更新答案
42                 {
43                     ans[0]=cnt_l + cnt_r + mapp1.size() + mapp2.size();
44                     ans[1]=i;
45                     ans[2]=j;
46                 }
47             }
48         }
49     }
50     printf("(%d, %d) %d\n",ans[1],ans[2],ans[0]);
51 }
52 
53 
54 int main()
55 {
56     //freopen("e://input.txt","r",stdin);
57     int n, m, t, a, b;
58     while(cin>>n>>m)
59     {
60         memset(tar,0,sizeof(tar));
61         scanf("%d",&t);
62         for(int i=0; i<t; i++)
63         {
64             scanf("%d%d",&x[i],&y[i]);
65             tar[x[i]][y[i]]=true;
66         }
67         cal(n,m,t);
68     }
69 
70     return 0;
71 }
AC代码
```

