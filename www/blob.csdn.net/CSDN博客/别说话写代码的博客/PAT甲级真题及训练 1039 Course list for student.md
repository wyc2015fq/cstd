# PAT甲级真题及训练 1039 Course list for student - 别说话写代码的博客 - CSDN博客





2018年04月18日 11:12:00[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：25








```cpp
#include<cstring>
#include<vector>
#include<cstdio>
#include<algorithm>
using namespace std;

const int N = 40010;   //总人数
const int M = 26 * 26 * 26 * 10 + 1;   //由姓名散列成的数字上届
vector<int> selectCourse[M];

int getID(char name[])
{
	int id = 0;
	for (int i = 0; i < 3; i++)
	{
		id = id * 26 + (name[i] - 'A');
	}
	id = id * 10 + (name[3] - '0');
	return id;
}

/*
1039 Course list for student

sample input:
11 5
4 7
BOB5 DON2 FRA8 JAY9 KAT3 LOR6 ZOE1
1 4
ANN0 BOB5 JAY9 LOR6
2 7
ANN0 BOB5 FRA8 JAY9 JOE4 KAT3 LOR6
3 1
BOB5
5 9
AMY7 ANN0 BOB5 DON2 FRA8 JAY9 KAT3 LOR6 ZOE1
ZOE1 ANN0 BOB5 JOE4 JAY9 FRA8 DON2 AMY7 KAT3 LOR6 NON9

Sample Output:
ZOE1 2 4 5
ANN0 3 1 2 5
BOB5 5 1 2 3 4 5
JOE4 1 2
JAY9 4 1 2 4 5
FRA8 3 2 4 5
DON2 2 4 5
AMY7 1 5
KAT3 3 2 4 5
LOR6 4 1 2 4 5
NON9 0
*/

int main()
{
	char name[5];  //获取用户姓名
	int n, k;   //人数及课程数
	int course, x;  //课程编号与选课人数
	int id;   //将姓名散列为一个返回值
	int i, j;
	scanf("%d %d", &n, &k);

	for (i = 0; i < k; i++)  //对每门课程
	{
		scanf("%d %d", &course, &x);
		for (j = 0; j < x; j++)
		{
			scanf("%s", name);
			selectCourse[getID(name)].push_back(course);
		}
	}
	for (i = 0; i < n; i++)  //对每个人
	{
		scanf("%s", name);
		id = getID(name);
		sort(selectCourse[id].begin(), selectCourse[id].end());
		printf("%s %d", name, selectCourse[id].size());
		for (j = 0; j < selectCourse[id].size(); j++)
		{
			printf(" %d", selectCourse[id][j]);
		}
		printf("\n");
	}

	return 0;
}
```




