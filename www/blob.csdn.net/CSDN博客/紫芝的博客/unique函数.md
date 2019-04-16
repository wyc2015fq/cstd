# unique函数 - 紫芝的博客 - CSDN博客





2018年04月13日 23:30:38[紫芝](https://me.csdn.net/qq_40507857)阅读数：136








```cpp
//unique函数可以删除有序数组中的重复元素 
//把重复的元素移到后面，依然保存在原数组
```

```cpp
#include<cstdio>
#include<algorithm>
using namespace std;
int a[50005];
int main()
{
	int n;
	while(scanf("%d",&n)!=-1)
	{
		for(int i=0;i<n;i++)
		scanf("%d",&a[i]);
		sort(a,a+n);
		
		int l=unique(a,a+n)-a;
		for(int i=0;i<l;i++)
		printf("%d ",a[i]);
	}
	return 0;
}
```




