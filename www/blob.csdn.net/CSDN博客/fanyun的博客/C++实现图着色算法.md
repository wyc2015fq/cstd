# C++实现图着色算法 - fanyun的博客 - CSDN博客
2018年12月31日 17:53:38[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：288
1.实现图着色算法
2.基本思路
   颜色数不合法，直接不满足；当颜色数合法，对于每一个方案，判断图中每一条边是否满足要求即可。
3.代码实现
```cpp
#include <bits/stdc++.h>
using namespace std;
int uu[250010],vv[250010];
int color[505];
int main(){
	int v,e,k;
	int x,y;
	scanf("%d%d%d",&v,&e,&k);
	for(int i = 0;i < e;i++){
		scanf("%d%d",&x,&y);
		uu[i] = x;
		vv[i] = y;
	}
	int q;
	cin >> q;
	while(q--){
		int flag = true;
		set<int> s;
		s.clear();
		for(int i = 1;i <= v;i++){
			cin >> color[i];
			s.insert(color[i]);
		}
		if(s.size() < k || s.size() > k){
			flag = false;
		}
		for(int i = 0;i < e && flag == true;i++){
			if(color[uu[i]] == color[vv[i]]){
				flag = false;
			}
		}
		puts(flag?"Yes":"No");
	} 
	return 0;
}
```
