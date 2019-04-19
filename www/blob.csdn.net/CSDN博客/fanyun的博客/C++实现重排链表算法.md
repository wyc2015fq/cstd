# C++实现重排链表算法 - fanyun的博客 - CSDN博客
2018年12月31日 17:53:24[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：59
1.实现重排链表算法
2.基本思路
   先根据题目信息找到原始链表，然后根据规则重排
3.实现代码
```cpp
#include <bits/stdc++.h>
using namespace std;
int head;
int data[100010],nex[100010];
int main(){
	int n;
	cin >> head >> n;
	for(int i = 0;i < n;i++){
		int h,d,ne;
		cin >> h >> d >> ne;
		data[h] = d;
		nex[h] = ne;
	}
	vector<int> v,re;
	int now = head;
	while(now != -1){
		v.push_back(now);
		now = nex[now];
	}
	int i = 0,j = v.size() - 1;
	while(i <= j){
		re.push_back(v[j--]);
		if(i > j) continue;
		re.push_back(v[i++]);
	}
	for(int i = 0;i < re.size() - 1;i++){
		printf("%05d %d %05d\n",re[i],data[re[i]],re[i + 1]);
	}
	int in = re.size() - 1;
	printf("%05d %d %d\n",re[in],data[re[in]],-1);
	return 0;
}
```
