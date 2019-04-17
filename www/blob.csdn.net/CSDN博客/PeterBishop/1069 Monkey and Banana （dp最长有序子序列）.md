# 1069 Monkey and Banana （dp最长有序子序列） - PeterBishop - CSDN博客





2018年12月04日 15:15:58[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：26








```cpp
/* ***********************************************
Author        :PeterBishop
Created Time  :Wed 06 Mar 2019 20:01:57 CST
File Name     :test.cpp
Origin        :H D O J  1069
************************************************ */

#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <string>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <queue>
#include <set>
#include <map>

using namespace std;
const int MAXN=100000;
struct cube{
	int x,y,z;
}c[MAXN];
bool cmp(cube a,cube b){
	if(a.x==b.x){
		return a.y<b.y;
	}
	return a.x<b.x;
}
int main()
{
	//freopen("out.txt","w",stdout);
	freopen("in.txt","r",stdin);
	int n,t_num=1;
	int z1,z2,z3;
	while(cin>>n){
		if(n==0)
			break;
		int len=0;
		for(int i=0;i<n;++i)
		{
			cin>>z1>>z2>>z3;
			c[len].x=z1,c[len].y=z2,c[len++].z=z3;
			c[len].x=z1,c[len].y=z3,c[len++].z=z2;
			c[len].x=z2,c[len].y=z1,c[len++].z=z3;
			c[len].x=z2,c[len].y=z3,c[len++].z=z1;
			c[len].x=z3,c[len].y=z1,c[len++].z=z2;
			c[len].x=z3,c[len].y=z2,c[len++].z=z1;
		}


		sort(c,c+6*n,cmp);
		int dp[MAXN];
		dp[0]=c[0].z;
		int m=0;
		//先是对长宽升序，dp[i]代表的是第i块方块为基础，可以到多高
		for(int i=1;i<len;i++){
			m=0;
			for(int j=0;j<i;j++){
				if(c[j].x<c[i].x&&c[j].y<c[i].y)
					m=max(m,dp[j]);//这里决定了是不是要用第j块作为前一块的地基
			} 
			dp[i]=c[i].z+m;//更新加上i的长度
		}
		m=0;
		for(int i=0;i<len;i++)
			m=max(m,dp[i]);
		cout<<"Case "<<t_num++<<": maximum height = "<<m<<endl;

	}
    return 0;
}
```





