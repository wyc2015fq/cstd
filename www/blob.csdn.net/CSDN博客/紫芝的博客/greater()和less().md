# greater()和less() - 紫芝的博客 - CSDN博客





2018年04月25日 18:57:43[紫芝](https://me.csdn.net/qq_40507857)阅读数：73








# greater<T>()

greater是较大意思，用于升序排列

# less<T>()

less是较小的意思，用于降序排列

```cpp
#include<bits/stdc++.h>
using namespace std;

int main()
{
	ios::sync_with_stdio(false);
	int a[]={0,1,4,7,2,5,8,3,6,9};
	int n=sizeof(a)/sizeof(int);
	
	sort(a,a+n);//默认从小到大排序 
	for(int i=0;i<n;i++)
	cout<<a[i]<<" ";
	cout<<endl;
	
	sort(a,a+n,less<int>());//从小到大排序 
	for(int i=0;i<n;i++)
	cout<<a[i]<<" ";
	cout<<endl;
	
	sort(a,a+n,greater<int>());//从大到小排序 
	for(int i=0;i<n;i++)
	cout<<a[i]<<" ";
	cout<<endl;
	return 0;
 }
```




