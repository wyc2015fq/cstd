# PAT甲级真题及训练 1047 Student List for Course - 别说话写代码的博客 - CSDN博客





2018年04月18日 11:13:38[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：26标签：[PAT																[甲级](https://so.csdn.net/so/search/s.do?q=甲级&t=blog)](https://so.csdn.net/so/search/s.do?q=PAT&t=blog)
个人分类：[c/c++](https://blog.csdn.net/qq_21997625/article/category/6988647)








```cpp
#include<cstring>
#include<vector>
#include<cstdio>
#include<algorithm>
using namespace std;

/*
1047 Student List for Course

Sample Input:
10 5
ZOE1 2 4 5
ANN0 3 5 2 1
BOB5 5 3 4 2 1 5
JOE4 1 2
JAY9 4 1 2 5 4
FRA8 3 4 2 5
DON2 2 4 5
AMY7 1 5
KAT3 3 5 4 2
LOR6 4 2 4 1 5

Sample Output:
1 4
ANN0
BOB5
JAY9
LOR6
2 7
ANN0
BOB5
FRA8
JAY9
JOE4
KAT3
LOR6
3 1
BOB5
4 7
BOB5
DON2
FRA8
JAY9
KAT3
LOR6
ZOE1
5 9
AMY7
ANN0
BOB5
DON2
FRA8
JAY9
KAT3
LOR6
ZOE1
*/
const int maxn = 40010;  //最大学生数
const int maxc = 2510;  //最大课程门数
char name[maxn][5];  //maxn个学生
vector<int> course[maxc];//course[i]存放第i门课所有学生编号

bool cmp(int a, int b)
{
	return strcmp(name[a], name[b]) < 0;//按姓名从小到大排序
}
int main()
{
	int n, k,c, courseID;//学生个数，课程总数，选课数，课程编号
	scanf("%d %d", &n, &k);
	int i, j;
	for (i = 0; i < n; i++)
	{
		scanf("%s %d", name[i],&c);
		for (j = 0; j < c; j++)
		{
			scanf("%d", &courseID);
			course[courseID].push_back(i);//将学生i加入第courseID门课中
		}
	}
	for (i = 1; i <= k; i++)
	{
		printf("%d %d\n", i, course[i].size());
		sort(course[i].begin(), course[i].end(),cmp);//排序
		for (j = 0; j < course[i].size(); j++)
		{
			printf("%s\n", name[course[i][j]]);
		}
	}
	system("pause");
	return 0;
}
```





