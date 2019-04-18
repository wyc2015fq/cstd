# UVALive 4043 Ants 蚂蚁（二分图最佳完美匹配，KM算法） - xcw0754 - 博客园
# [UVALive 4043 Ants 蚂蚁（二分图最佳完美匹配，KM算法）](https://www.cnblogs.com/xcw0754/p/4719582.html)
题意：
　　有n个蚂蚁n棵树，蚂蚁与树要配对，在配对成功的一对之间连一条线段，要求所有线段不能相交。按顺序输出蚂蚁所匹配的树。
思路：
　　这个题目真是技巧啊，不能用贪心来为每个蚂蚁选择最近的树，这样很可能是相交了的。
　　整体最优能让每条线段不相交，证明：
　　假设a1-b1与a2-b2相交。则dis(a1,b1)+dis(a2,b2)>=dis(a1,b2)+dis(a2,b1)。如果我们所决定的最优匹配是按照整体距离最短来匹配的，那么dis(a1,b1)+dis(a2,b2)必定小于dis(a1,b2)+dis(a2,b1)，否则，与最优矛盾。推广到整个图就是匹配图中任意两个点都是最优的，否则我们一定可以用更优的方式来替代他们。而整体最优靠的是KM算法。注意到，本题是完全二分图。
　　Tips：要选的是整体权值最小，只需要将边权置为距离相反数再跑KM算法即可。
```
1 #include <bits/stdc++.h>
  2 using namespace std;
  3 const int N=110;
  4 int antx[N], anty[N], treex[N], treey[N];
  5 double g[N][N];     //距离
  6 
  7 inline double dis(int a,int b)
  8 {
  9     return sqrt((treex[a]-antx[b])*(treex[a]-antx[b])+(treey[a]-anty[b])*(treey[a]-anty[b]));
 10 }
 11 
 12 int n;
 13 double Lx[N], Ly[N], slack[N];
 14 int girl[N];
 15 int S[N], T[N];
 16 
 17 bool DFS(int x)
 18 {
 19     S[x]=true;
 20     for(int i=1; i<=n; i++)
 21     {
 22         if(T[i])    continue;
 23         double tmp=Lx[x]+Ly[i]-g[x][i];
 24         if(tmp<1e-6)
 25         {
 26             T[i]=true;
 27             if(girl[i]==0 || DFS(girl[i]))
 28             {
 29                 girl[i]=x;
 30                 return true;
 31             }
 32         }
 33         else if(slack[i]>tmp)
 34             slack[i]=tmp;
 35     }
 36     return false;
 37 }
 38 
 39 void KM(int n)
 40 {
 41     for(int i=1; i<=n; i++) //初始化工作
 42     {
 43         girl[i]=0;
 44         Lx[i]=-1e19;
 45         Ly[i]=0.0;
 46         for(int j=1; j<=n; j++)
 47             Lx[i]=max(Lx[i], g[i][j]);
 48     }
 49     for(int i=1; i<=n; i++) //对于每个树
 50     {
 51         for(int j=1; j<=n; j++) slack[j]=1e19;
 52         while(1)
 53         {
 54             memset(S, 0, sizeof(S));
 55             memset(T, 0, sizeof(T));
 56             if( DFS(i) )    break;      //找到匹配的蚂蚁
 57 
 58 
 59             double d=1e19;
 60             for(int j=1; j<=n; j++) //找最小D
 61             {
 62                 if(!T[j] && d>slack[j])
 63                     d=slack[j];
 64             }
 65 
 66             for(int j=1; j<=n; j++) //更新树
 67             {
 68                 if(S[j])
 69                     Lx[j]-=d;
 70             }
 71 
 72             for(int j=1; j<=n; j++) //更新蚂蚁
 73             {
 74                 if(T[j])    Ly[j]+=d;
 75                 else        slack[j]-=d;
 76             }
 77         }
 78     }
 79 }
 80 
 81 int main()
 82 {
 83     freopen("input.txt", "r", stdin);
 84     int k=0;
 85     while(~scanf("%d",&n))
 86     {
 87         if(k)   printf("\n");
 88         k++;
 89         for(int i=1; i<=n; i++)
 90             scanf("%d%d", &antx[i], &anty[i]);      //ant
 91         for(int i=1; i<=n; i++)
 92             scanf("%d%d", &treex[i], &treey[i]);    //apple tree
 93 
 94 
 95         for(int i=1; i<=n; i++)
 96             for(int j=1; j<=n; j++)
 97                 g[i][j]=-dis(i,j);
 98 
 99         KM(n);
100         for(int i=1; i<=n; i++)
101             printf("%d\n", girl[i]);    //ans为girl
102     }
103     return 0;
104 }
AC代码
```

