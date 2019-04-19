# Map 容器学习 - sinat_31425585的博客 - CSDN博客
2018年01月07日 14:11:28[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：59
Map是STL的一个关联容器，赋值方法一：
```cpp
#include <map>
#include <iostream>
using namespace std;
int main()
{
	map<int, int> pairs;
	int i, j;
	for (i = 0; i < 10; i++)
	{
		pairs[i]++;
	}
	for (j = 0; j < pairs.size(); j++)
	{
		cout <<"pairs " << j << ": " <<  pairs[j] << endl;
	}
	cout << pairs.size() << endl;
	return 0;
}
```
![效果如下](https://img-blog.csdn.net/20180107141050091?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzE0MjU1ODU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
