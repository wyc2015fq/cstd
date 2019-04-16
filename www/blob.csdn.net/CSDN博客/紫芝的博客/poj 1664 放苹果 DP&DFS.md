# poj 1664 放苹果 DP&DFS - 紫芝的博客 - CSDN博客





2018年04月11日 00:38:56[紫芝](https://me.csdn.net/qq_40507857)阅读数：50








[点击打开链接](http://poj.org/problem?id=1664)

把 M个同样的苹果放在 N个同样的盘子里，允许有的盘子空着不放


问共有多少种方法？5，1，1和1，5，1是同一种方法 



解法一：

poj 1664 放苹果 DP

dp[i][j]表示共 i个苹果放入 j个盘子里的方法
状态转移过程：

1.这 j个盘子有空盘子，这时候i个苹果放入j个盘子的方法，
和i个苹果放入j-1个盘子的方法是一样的

2.j个盘子中没有空盘子，那么可以从每个盘子中移除一个，

原问题转换成i-j个苹果放到 j个盘子里 
3.为什么没有空盘子时，苹果只拿走一个？

这个问题我们可以这样想，假设放置最少的盘子中有k个苹果，
那么这个状态可以唯一地，由连续k次每次从每个盘子中拿走一个苹果而得到，
所以就不会出现重复的子状态，也就满足了题目的显示条件！



```cpp
#include<iostream>
#include<cstring>
using namespace std;
const int MAXN=15;
int dp[MAXN][MAXN];
int M,N;
 
int main()
{
	ios::sync_with_stdio(false);
	int t;
	cin>>t;
	while(t--){
		cin>>M>>N;
		memset(dp,-1,sizeof(dp));
		for(int i=1;i<=10;i++)
	dp[0][i]=dp[1][i]=dp[i][1]=1;//边界条件
	
	for(int i=1;i<=10;i++){
		for(int j=1;j<=10;j++){
			if(dp[i][j]==-1){
				if(i<j)//证明有空盘子
				dp[i][j]=dp[i][j-1];
				else
			//没有空盘子，就先给这 j个盘子每个都放一个苹果，
			dp[i][j]=dp[i][j-1]+dp[i-j][j];	 
			}
		}
	}
	cout<<dp[M][N]<<endl; 
	}
	return 0;
 }
```

解法二：

DFS递归搜索

一、递归边界



```cpp
只有一个盘子，无论有多少个苹果都只有一种方法
```
二、递归过程
```cpp
//第一：如果盘子的个数大于苹果的个数，就把m个苹果放到m个盘子的方法
```

```cpp
//第二：
```

```cpp
//1.将至少其中一个盘子不放，那么就是 m个苹果放到n-1个盘子的方法
```

```cpp
//2.每个盘子放一个，然后就是 m-n个苹果放到n个盘子里
```

```cpp
#include<iostream>
using namespace std;

int apple_divide(int m,int n)
{
//只有一个盘子，无论有多少个苹果都只有一种方法 
	if(m==0||n==1)
	return 1;
	if(n>m)//如果盘子的个数大于苹果的个数
	return apple_divide(m,m);//m个苹果放到m个盘子的方法 
	else
	return apple_divide(m,n-1)+apple_divide(m-n,n); 
//1.将至少其中一个盘子不放，那么就是 m个苹果放到n-1个盘子的方法
//2.每个盘子放一个，然后就是 m-n个苹果放到n个盘子里 
}
int main()
{
	ios::sync_with_stdio(false);	
 	int m,n,k;//m个苹果，n个盘子 
 	cin>>k;
 	while(k--){
 		cin>>m>>n;
 	cout<<apple_divide(m,n)<<endl;
	 }
	return 0;
 }
```



