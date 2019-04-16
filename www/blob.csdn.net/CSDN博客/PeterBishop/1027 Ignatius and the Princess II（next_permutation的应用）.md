# 1027 Ignatius and the Princess II（next_permutation的应用） - PeterBishop - CSDN博客





2018年07月12日 09:22:59[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：18
个人分类：[HDOJ](https://blog.csdn.net/qq_40061421/article/category/7502192)









```cpp
#include<stdio.h>
#include<algorithm>
using namespace std;
int a[10005];
int main(){
	int n,k,m,i;
	while(~scanf("%d%d",&n,&k)){
		m=1;
		for(i=0;i<=n+1;i++)
			a[i]=i+1;
		while(next_permutation(a,a+n)){
			m++;
			if(m==k){
				break;
			}
			
		}
		for(i=0;i<n;i++){
			if(i!=n-1)
				printf("%d ",a[i]);
			else printf("%d\n",a[n-1]);
		}
	}
	return 0;
}
```
[关于next_permutation，这里有一个原理讲的很清楚的，可以看看何为下一个排列以及实现细节](https://www.cnblogs.com/luruiyuan/p/5914909.html)


