# 2021 Relative Relatives - PeterBishop - CSDN博客





2018年10月10日 15:59:44[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：17
个人分类：[POJ](https://blog.csdn.net/qq_40061421/article/category/7589386)









### 这个题目是从正面推进的，比较简单能理解，只要去把结构体设计好，然后数组表示好久可以啦 

```cpp
#include<iostream>
#include<string.h>
#include<stdio.h>
#include<algorithm>

using namespace std;

const int MAX = 100;

struct info
{
    char name[32];
    int age;
};struct info descent[MAX];

bool cmp(info a, info b) /* 降序排列 */
{
	if (a.age > b.age)
        return true;
	if (a.age == b.age && strcmp(a.name, b.name) < 0)
        return true;
	return false;
}


int main()
{

	char father[MAX][32];
	char son[MAX][32];
	int  year[MAX];

	int t, num = 0;
	cin >> t;
	while (t-- > 0)
	{
		int n;
        cin >> n;
		int i;
		for (i = 0; i < n; i++)
		{

			cin >> father[i] >> son[i] >> year[i];//读入数据
		}
		int j, k = 1, cur = 0;
		descent[0].age = 100;
        strcpy(descent[0].name, "Ted");//题目条件
		for (i = 0; i < n; i++, cur++)
		{
			if (cur >= k)
                break;
			for (j = 0; j < n; j++)
			{
				if (!strcmp(descent[cur].name, father[j]))//由一个父亲推儿子年龄
				{
					strcpy(descent[k].name, son[j]);
					descent[k].age = descent[i].age - year[j];
					k++;//指示下一个descent
				}
			}
		}
        sort(descent, descent + k, cmp);
		printf("DATASET %d\n", ++num);
		for (i = 1; i < k; i++)
			printf("%s %d\n", descent[i].name, descent[i].age);
	}
	return 0;
}
```





