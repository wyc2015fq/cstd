# 1509  Windows Message Queue （priority_queue的自定义类型+重载比较函数） - PeterBishop - CSDN博客





2018年07月12日 10:36:19[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：39
个人分类：[HDOJ](https://blog.csdn.net/qq_40061421/article/category/7502192)









```cpp
#include<cstdio>
#include<queue>

using namespace std;

struct node {
	int sign, vip, num;//顺序，优先值，变量值
	char mbr[400];           //变量名开大点
	bool friend operator < (node a, node b) {//重载一下比较函数
		if (a.vip == b.vip)
			return a.sign > b.sign;
		return a.vip > b.vip;     //越大优先级越小
	}
}ans;
int main() {
	char get[4];
	priority_queue<node> q;
	int k = 1;
	while (~scanf("%s", get)) {
		if (get[0] == 'G') {
			if (q.empty()) {
				printf("EMPTY QUEUE!\n");
			}
			else {
				ans = q.top();
				printf("%s %d\n", ans.mbr, ans.num);
				q.pop();
			}
		}
		else {
			scanf("%s %d %d", ans.mbr, &ans.num, &ans.vip);
			ans.sign = k;
			k++;
			q.push(ans);
		}
	}
	return 0;
}
```




