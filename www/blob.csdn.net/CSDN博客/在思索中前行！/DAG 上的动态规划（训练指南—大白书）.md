# DAG 上的动态规划（训练指南—大白书） - 在思索中前行！ - CSDN博客





2015年11月04日 16:42:48[_Tham](https://me.csdn.net/txl16211)阅读数：2644标签：[DAG																[动态规划																[DAG上动态规划](https://so.csdn.net/so/search/s.do?q=DAG上动态规划&t=blog)
个人分类：[NOIP 动态规划](https://blog.csdn.net/txl16211/article/category/2239693)








有向无环图（DAG,Directed Acyclic Graph）上的动态规划是学习动态规划的基础。很多问题都可以转化为DAG上的最长路、最短路或路径计数问题。


一、矩形嵌套

题目描述：

       有n个矩形，每个矩形可以用两个整数a,b描述，表示它的长和宽。矩形X(a,b)可以嵌套在矩形Y(c,d)中当且仅当a<c,b<d,或者b<c,a<d(相当于把矩形X旋转90°)。例如(1,5)可以嵌套在(6,2)内，但不能嵌套在(3,4)内。你的任务是选出尽可能多的矩形排成一行。使得除了最后一个之外，每个矩形都可以嵌套在下一个矩形内。


解题思路：

       如何求DAG中不固定起点的最长路径呢？仿照数字三角形的做法，设d(i)表示从节点i出发的最长路长度，应该如何写状态方程呢？第一步只能走到它的相邻点，因此：
d(i) = max { d(j) + 1 | (i,j)-> E }

其中E为边集，最终答案为d(i).那如果要求输出字典序最小的最长路径呢？那么必须找到第一个最长的路径的值然后递归输出。

代码：



```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<algorithm>
#include<iostream>
using namespace std ;
const int MX = 1000 + 10 ;
int n ;
int G[MX][MX],dp[MX] ;
struct node
{
    int x,y ;
}T[MX] ;
void buildGraph() // 建图
{
    memset(G,0,sizeof(G)) ;
    for(int i=0 ;i<n ;i++)
      for(int j=0 ;j<n ;j++)
        if(T[i].x>T[j].x&&T[i].y>T[j].y)
             G[i][j]=1 ;
}
int DAG(int x) // 记忆化求解
{
    int& ans = dp[x] ;
    if(ans > 0) return ans ;
    ans=1 ;
    for(int i=0 ;i<n ;i++)
      if(G[x][i])
      {
          int mx=DAG(i)+1 ;
          ans = ans > mx ? ans : mx ;
      }
    return ans ;
}
void print(int x) // 打印路径
{
    printf("%d ",x) ;
    for(int i=0 ;i<n ;i++)
      if(G[x][i]&&dp[x]==dp[i]+1)
      {
          print(i) ;
          break ;
      }
}
int main()
{
     int Tx ;
     scanf("%d",&Tx) ;
     while(Tx--)
     {
         scanf("%d",&n) ;
         for(int i=0 ;i<n ;i++)
         {
             scanf("%d%d",&T[i].x,&T[i].y) ;
             if(T[i].x>T[i].y)
                 swap(T[i].x,T[i].y) ;
         }
         int ans=1 ;
         buildGraph() ;
         memset(dp,-1,sizeof(dp)) ;
         for(int i=0 ;i<n ;i++)
         {
             int mx=DAG(i) ;
             ans= mx > ans ? mx : ans ;
         }
         for(int i=0 ;i<n ;i++)// 寻找第一个点
          if(dp[i]==ans)
          {
              printf("%d\n",ans) ;
              print(i) ;
             break ;
          }
     }
     return 0 ;
}
```


二、硬币问题


题目描述：

        有n种硬币，面值分别为V1,V2...,Vn,每种都有无限多。给定非负整数S，可以选用多少个硬币，使得面值之和恰好为S？输出硬币数目的最小值和最大值。0 <= n <= 100, 0 <= S <= 10000, 1 <= Vi <= S。

解题思路：

        本题的本质还是DAG上的路径问题。我们把每种面值看作一个点，表示"还需要凑足的面值"，则初始状态为S，目标状态为0。若当前的状态i，每使用一个硬币j，状态便转移到i-Vj。这个模型和嵌套矩形一题类似，但也有些明显的不同之处：上题并没有确定路径的起点和终点（可以把任意矩形放在第一个和最后一个），而本题的起点必须是S，终点必须是0。把终点固定之后"最短路"才是有意义的。在嵌套矩形中，最短序列显然是空（如果不允许空的话，就是单个矩形，不管怎样都是平凡的），而本题的最短路径却不是那么容易确定的。  
           

       接下来考虑"硬币问题"。注意到最长路和最短路的求法是类似的，下面只考虑最长路。由于终点固定，d(i)的确切含义变为"从节点i出发到节点0的最长路径长度"。

代码：



```cpp
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#define INF 1<<30
#define maxn 100+10
using namespace std ;
int V[maxn],n;
int min[maxn],max[maxn];

inline int Min(int a,int b){return a<b?a:b;}
inline int Max(int a,int b){return a>b?a:b;}

//打印可行的方案 
void print_ans(int* d,int S)
{
  for(int i=1;i<=n;i++)
  {
    if(S>=V[i] && d[S]==d[S-V[i]]+1)
    {
       printf("%d ",V[i]);
       print_ans(d,S-V[i]);
       break;
    }
  }
}
int main()
{
  int S;
  while(~scanf("%d%d",&S,&n)) //输入面值S和面值的种数n 
  {		
    for(int i=1;i<=n;i++)
      scanf("%d",&V[i]);
      max[0]=0; min[0]=0;
    for(int i=1;i<=S;i++)
      min[i]=INF,max[i]=-INF;
    //递推实现 
    for(int i=1;i<=S;i++)
      for(int j=1;j<=n;j++)
        if(i>=V[j])
        {
          min[i]=Min(min[i],min[i-V[j]]+1);
          max[i]=Max(max[i],max[i-V[j]]+1);
        }
     print_ans(min,S);	
     printf("    min\n");
     print_ans(max,S);	
     printf("    max\n");
     printf("min:%d max:%d\n",min[S],max[S]);	
  }
  return 0;
}
```


分析：本质上市一个DAG上的路径问题，我们把每种面值看做一个点，表示还需凑足的面值，则初始状态为0，目标状态为0，若当前在i,则每使用一枚硬币j，状态转移到i-vj。

代码：



```cpp
#include<stdio.h>
#define N 1100
int v[N],min[N],max[N],min_coins[N],max_coins[N];

void print_ans(int *d,int s, int n) {
	while(s){
		printf("%d ",v[d[s]]);
		s-=v[d[s]];
	}
	printf("\n");
}
int main() {
	int T,i,j,n,s;
	scanf("%d",&T);
	while(T--) {
		scanf("%d %d",&n,&s);
		for(i=0;i<n;i++)
			scanf("%d",&v[i]);
		min[0]=max[0]=0;
		for(i=1;i<=s;i++){
			min[i]=0x7FFFFFFF;
			max[i]=-0x7FFFFFFF;
		}
		for(i=1;i<=s;i++){
			for(j=0;j<n;j++){
				if(i>=v[j]){
					//min[i]=min[i]<(min[i-v[j]]+1)?min[i]:(min[i-v[j]]+1);
					if(min[i]>min[i-v[j]]+1){
						min[i]=min[i-v[j]]+1;
						min_coins[i]=j;
					}
					if(max[i]<max[i-v[j]]+1){
						max[i]=max[i-v[j]]+1;
						max_coins[i]=j;
					}
					//max[i]=max[i]>(max[i-v[j]]+1)?max[i]:(max[i-v[j]]+1);
				}
			}
		}
		printf("%d %d\n",min[s],max[s]);
		print_ans(min_coins,s,n);
		print_ans(max_coins,s,n);
	}
	return 0;
}
```


上面的代码中，如果不需要输出路径的话，则可以不要max_coins和min_coins数组，下面在写一个递归的。



```cpp
#include<stdio.h>
#include<string.h>
#define N 1100

int v[N],d[N],vis[N];

int dp(int s, int n) {
    int i;
    if(vis[s]) 
        return d[s];
    vis[s]=1;
    
    d[s]=-1<<30;//没有算过的我们假定很小 
    for(i=0;i<n;i++)
        if(s>=v[i]&&d[s]<dp(s-v[i],n)+1)
            d[s]=dp(s-v[i],n)+1;
    return d[s];
}
void print_ans(int s,int n){
    int i;
    for(i=0;i<n;i++){
        if(s>=v[i]&&d[s]==d[s-v[i]]+1){
            printf("%d ",v[i]);//输出所选的银币面值 
            print_ans(s-v[i],n);
            break;
        }
    }
}
int main() {
    int T,i,n,s,ans;
    scanf("%d",&T);
    while(T--) {
        scanf("%d %d",&n,&s);
        for(i=0;i<n;i++) {
            scanf("%d",&v[i]);
        } 
        memset(vis,0,sizeof(vis));
        vis[0]=1;
        d[0]=0;//终点状态药初始化为0，访问过 
        ans=dp(s,n);
        printf("%d\n",ans);
        print_ans(s,n); 
        printf("\n");
    }
    return 0;
}
```


只写了求最长路径的，如果要求最短路径，则和最长路径类似，在此省略。](https://so.csdn.net/so/search/s.do?q=动态规划&t=blog)](https://so.csdn.net/so/search/s.do?q=DAG&t=blog)




