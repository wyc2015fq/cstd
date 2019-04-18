# POJ 1523 SPF (割点，连通分量) - xcw0754 - 博客园
# [POJ 1523 SPF (割点，连通分量)](https://www.cnblogs.com/xcw0754/p/4616270.html)
题意：给出一个网络（不一定连通），求所有的割点，以及割点可以切分出多少个连通分量。
思路：很多种情况。
（1）如果给的图已经不是连通图，直接“  No SPF nodes”。
（2）求所有割点应该不难，就是tarjan发明的算法搞定。但是求连通分量就得小心了，多种情况。看下：
　　1）如果一个割点x，其所有孩子都不是割点，那么x至少可以割出两个连通分量（x之上和之下的各1个）。
　　2）如果一个割点x，其有部分孩子是割点，有部分孩子并不是割点，那么x可以割出x之上的1个连通分量，不是割点的孩子均是同1个连通分量，是割点的孩子各自分别是一个连通分量。
　　3）如果一个割点x，如果有些孩子是叶子节点，且该叶子节点的度为1，即连着x，那么该叶子节点会被割出来，单点也是连通分量。
```
1 //#include <bits/stdc++.h>
  2 #include <iostream>
  3 #include <cmath>
  4 #include <cstdio>
  5 #include <vector>
  6 #include <cstring>
  7 #include <climits>
  8 using namespace std;
  9 const int N=1005;
 10 int mapp[N][N];
 11 int cntr, flag, up;                       //时间戳
 12 int dfn[N], low[N], vis[N], iscut[N], cur[N];     //记录时间戳， 反边最顶点， 访问记录， 是否连通分量
 13 
 14 bool DFS(int x, int far)
 15 {
 16     int chd=0;              //孩子节点个数
 17     int cnt=0, sum=0;
 18     dfn[x]=low[x]=++cntr;   //时间戳
 19     vis[x]=1;               //DFS必备
 20     for(int i=1; i<=up; i++)
 21     {
 22         if(!mapp[i][x]) continue;
 23         if(!vis[i])         //树边
 24         {
 25             chd++;
 26             bool f=DFS(i,x);    //i点只有一条边
 27             low[x]=min(low[x],low[i]);
 28 
 29             //判断连通分量的个数
 30             if(!far && chd>1 || far && low[i]>=dfn[x] )   //能让x成为割点的孩子i
 31             {
 32 
 33 
 34                 sum++;
 35                 iscut[x]=1;
 36                 if(iscut[i]||f)    cnt++;      //这个孩子是割点或者满足单点单边条件
 37             }
 38         }
 39         else if(i!=far)
 40             low[x]=min(low[x], dfn[i]);
 41     }
 42     if(!far && iscut[x])
 43     {
 44         iscut[x]=cnt;  //根节点
 45         if(sum>cnt) iscut[x]++;
 46     }
 47     if( far && iscut[x])    //非根节点
 48     {
 49         if(!cnt )     iscut[x]=2;    //没有“是割点”的孩子，那么只要自己是割点，起码可以割为两个连通分量
 50         else if(sum==cnt)    iscut[x]=cnt+1;     //有“是割点”的孩子，那么每个“割点”孩子是1个连通分量，注：有可能还有非割点孩，他们算1个连通分量
 51         else iscut[x]=cnt+2;
 52     }
 53     if(iscut[x])    flag=1; //无割点的标记'
 54     //if(x==1)    cout<<"**"<<sum<<endl;
 55 
 56     if(!chd && low[x]==dfn[x] )   return true;  //返回值用于判断是否是叶子，且没有反向边到fa之上
 57     return false;
 58 }
 59 
 60 
 61 void cal()  //n为边数
 62 {
 63 
 64     flag=cntr=0;
 65     DFS(up,0);  //深搜求割点数
 66     int i;
 67     for(i=1; i<=up; i++)    //先确保是个连通图
 68         if(cur[i]&&!vis[i])
 69         {
 70             printf("  No SPF nodes\n");
 71             return ;
 72         }
 73 
 74     for(i=1; i<=up; i++)
 75         if(cur[i]&&iscut[i])
 76         {
 77             printf("  SPF node %d leaves %d subnets\n", i, iscut[i]);
 78         }
 79 
 80     if(!flag)   printf("  No SPF nodes\n");
 81 }
 82 void init()
 83 {
 84     up=0;
 85     memset(cur,0,sizeof(cur));
 86     memset(mapp,0,sizeof(mapp));
 87     memset(vis,0,sizeof(vis));
 88     memset(dfn,0,sizeof(dfn));
 89     memset(low,0,sizeof(low));
 90     memset(iscut,0,sizeof(iscut));
 91 
 92 }
 93 
 94 int main()
 95 {
 96     freopen("input.txt", "r", stdin);
 97     int j=0, a, b;
 98     while(1)
 99     {
100         int cnt=0;
101         init();
102         while(~scanf("%d",&a))
103         {
104             if(!a && !cnt)    return 0;   //没边，a又为0，则结束了
105             if(!a)
106             {
107                 printf("Network #%d\n",++j);
108                 cal();
109                 printf("\n");break;
110             }
111             scanf("%d",&b);
112             up=max(max(up,a),b);    //记录顶点号上限
113             cur[a]=cur[b]=1;        //记录出现的顶点号
114             mapp[a][b]=mapp[b][a]=1;    //矩阵
115             cnt++;                  //边数
116         }
117     }
118     return 0;
119 }
AC代码
```

