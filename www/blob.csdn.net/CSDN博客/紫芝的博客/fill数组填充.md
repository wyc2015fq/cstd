# fill数组填充 - 紫芝的博客 - CSDN博客





2018年03月28日 11:40:44[紫芝](https://me.csdn.net/qq_40507857)阅读数：643








                函数参数 fill(first,last,val)，first,last 对应数组的范围，val为元素的值


fill 顾名思义 这个函数是用来填充的，例如我们想对一个数组赋值

```cpp
#include<iostream>  
#include<bits/stdc++.h>  
using namespace std;  
int main(){  
cout<<"一维数组 "<<endl;
    int a[20];  
    int b[20];  
    char c[20];  
    float d[20];  
    
    
    fill(a,a+11,1);             //赋值1   
    fill(b,b+11,2147483647);    //赋值最大整型 2147483647  
    fill(c,c+11,'*');           //赋值 *   
    fill(d,d+11,0.1);           //赋值 0.1   
    
    
    for(int i=0;i<20;i++) 	cout<<a[i]<<"   ";  cout<<endl;  
    for(int i=0;i<20;i++) 	cout<<b[i]<<"   ";  cout<<endl;  
    for(int i=0;i<20;i++) 	cout<<c[i]<<"   ";  cout<<endl;  
    for(int i=0;i<20;i++) 	cout<<d[i]<<"   ";  cout<<endl;  

cout<<"二维数组 "<<endl;
    int mp[20][20];
	fill(mp[0],mp[0]+20*20,5);
	for(int i=0;i<20;i++)
	{
	for(int j=0;j<20;j++)
	cout<<mp[i][j]<<" "; 
	cout<<endl;	
	}
	
    return 0;  
}
```

注意：填充量必须是常数

```
/*
NYOJ737 石子合并（一）区间动态规划
*/
#include<bits/stdc++.h>
using namespace std;
#define inf 0x3f3f3f3f
//#define inf 1<<20
const int maxn=210;
int n,a[maxn]; 
int  dp[maxn][maxn];//dp[i][j]表示从第i堆到第j堆合并的代价
int  sum[maxn][maxn];//表示石头的数量 
int main()
{
	ios::sync_with_stdio(0);
	while(cin>>n)
	{
		for(int i=1;i<=n;i++)
		cin>>a[i];
		memset(sum,0,sizeof(sum));
		//fill(dp[0],dp[0]+n*n,inf);//错误 
		fill(dp[0],dp[0]+maxn*maxn,inf);//fill填充量必须是常数 
		
		for(int i=1;i<=n;i++)
		for(int j=1;j<=n;j++)
		//dp[i][j]=inf;
		cout<<dp[i][j]<<endl;
		
		for(int i=1;i<=n;i++)
		sum[i][i]=a[i],dp[i][i]=0;
		
		for(int len=1;len<n;len++){//区间长度 
			for(int i=1;i<=n&&i+len<=n;i++){//区间起点 
				int j=i+len;//区间终点
				for(int k=i;k<=j;k++)//用k来表示分割区间 
				{
					sum[i][j]=sum[i][k]+sum[k+1][j];
					dp[i][j]=min(dp[i][j],dp[i][k]+dp[k+1][j]+sum[i][j]);
				 } 
			}
		}
		cout<<dp[1][n]<<endl;
	}
	return 0;
}
```






