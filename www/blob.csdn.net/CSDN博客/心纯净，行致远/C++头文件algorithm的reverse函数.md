# C++头文件algorithm的reverse函数 - 心纯净，行致远 - CSDN博客





2018年11月21日 13:43:48[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：788








reverse(beg,end)

reverse()会将区间[beg,end)内的元素全部逆序； 

```cpp
#include<stdio.h>
#include<iostream>
#include<algorithm>
using namespace std;
int main()
{
	int a[50];
	int b[50]; 
	for(int i=0;i<10;i++)
	{
		a[i]=i;
		if(i!=9)
		cout<<a[i]<<" ";
		else
		cout<<a[i]<<endl;
	}
	reverse(a,a+10);         //第二个参数是数组最后一个元素的下一个地址 
	for(int i=0;i<10;i++)
	{
		if(i!=9)
		cout<<a[i]<<" ";
		else
		cout<<a[i]<<endl;
	}
	reverse_copy(a,a+10,b); //倒序放入b数组中 
	for(int i=0;i<10;i++)
	{
		if(i!=9)
		cout<<b[i]<<" ";
		else
		cout<<b[i]<<endl;
	}
	
}
```

![](https://img-blog.csdn.net/2018062311181245?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwODE2MDc4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





