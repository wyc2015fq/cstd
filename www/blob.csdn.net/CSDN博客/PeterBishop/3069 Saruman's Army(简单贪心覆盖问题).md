# 3069 Saruman's Army(简单贪心覆盖问题) - PeterBishop - CSDN博客





2018年12月04日 15:10:48[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：37








### 题意：在一条直线上，有n个点。从这n个点中选择若干个，给他们加上标记。对于每一个点，其距离为R以内的区域里必须有一个被标记的点。问至少要有多少点被加上标记。

### 题解：我们从最左边的开始考虑。对于这个点，到距其R以内的区域必须要有带有标记的点。带有标记的点一定在其右侧（包含这个点本身）。给从最左边开始，距离为R以内的最远的点加上标记，尽可能的覆盖更靠右边的点。对于添加了标记的点右侧相距超过R的下一个点，采用同样的方法找到最右侧R距离以内最远的点添加标记。在所有点都被覆盖之前不断重复这一过程。

```cpp
#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;
int a[1010];
int main()
{
	int n,R,i,ans;
	while(scanf("%d%d",&R,&n)&&R!=-1||n!=-1)
	{
		for(i=0;i<n;++i)
			scanf("%d",&a[i]);
		sort(a,a+n);
		i=0;ans=0;
		while(i<n)
		{
			int s=a[i++];//s表示没有被覆盖的最左边的点 
			while(i<n&&a[i]<=s+R)//一直向右前进直到距s的距离大于R的点 
				i++;
			int p=a[i-1];//被标记的点 
			while(i<n&&a[i]<=p+R)//一直向右前进直到距p的距离大于R的点 
				i++;
			ans++;
		}
		printf("%d\n",ans);
	}
	return 0;
}
```





