# 搜索+剪枝——POJ 1011 Sticks - 在思索中前行！ - CSDN博客





2014年08月05日 09:51:34[_Tham](https://me.csdn.net/txl16211)阅读数：725









### [搜索+剪枝——POJ 1011 Sticks](http://ren.iteye.com/blog/344095)
**博客分类：**[算法](http://ren.iteye.com/category/66075)


非常经典的搜索题目，第一次做还是暑假集训的时候，前天又把它翻了出来



本来是想找点手感的，不想在原先思路的基础上，竟把它做出来了而且还是0ms过得。仔细想想，对搜索又有了一点点认识。

    题目要求将一系列的sticks重新组合，形成若干相等相等长度的木棒，且尽量使木棒长度最小，如果数据量比较小的话，就纯粹是搜索了，但题目要求的 sticks可能达到64根，如果纯粹的搜索则显然是会远远超过1000ms的，因而也就把剪枝放在了很重要的位置。从第一根stick开始，寻找下一根 stick使两者的长度小于等于木棒的长度，然后再寻找下一根stick，直到和为一根木棒的长度；然后又从一根没有被使用的stick开始进行下一根木棒的组合。概括一点说，就是一个深度优先搜索。

    做完这个题目之后，仔细想了想，觉得之前没有做出来最主要的原因就出在回溯上，因为对于dfs最初使用的的返回类型是void，当某种情况不行，需要回溯到上一层时，我就会很急地使用return语句，这个时候并没有回溯到上一层，而是退出了函数；另外，也要告诫自己要控制好变量，就这个题目而言，开始也有地方的那个used值并没有做好处理。致使最后有时候会出现一些莫名其妙的结果，与产生错误的提示信息。也走了那条从TLE到WA，再到AC的路，但还是学到了很多东西。




```cpp
#include <iostream>
#include <algorithm>
using namespace std;

int sticks[64], n, len, num;
bool used[64];

bool compare(int a, int b)
{
	return a > b;	
}

bool dfs(int cur, int left, int level) 
{	//cur: 当前已经计算的木棒编号，left:该段还剩的长度，level:已经成功的木棒数
	if(left == 0) {//匹配一根木棒成功
		if(level == num-2)
			return true;
		for(cur = 0; used[cur]; cur++)
			;
		used[cur] = true;
		if(dfs(cur+1, len-sticks[cur], level+1))
			return true;
		used[cur] = false;
		return false;
	} else {
		if(cur >= n-1)
			return false;
		for(int i = cur; i < n; i++) {
			if(used[i])
				continue;
			if((sticks[i] == sticks[i-1]) && !used[i-1])
				continue;	
			if(sticks[i] > left)
				continue;
			used[i] = true;
			if(dfs(i, left-sticks[i], level))
				return true;
			used[i] = false;
		}	
		return false;
	}
}

int main()
{
	while(cin>>n) {
		if(n == 0)
			break;
		int sum = 0;
		for(int i = 0; i < n; i++) {
		 	scanf("%d", &sticks[i]);
		 	sum += sticks[i];
		}
		sort(sticks, sticks+n, compare); //由大到小排序	
		bool end = false;
		for(len = sticks[0]; len <= sum/2; len++) {
			if(sum%len == 0) {
				used[0] = true;
				num = sum/len;
				if(dfs(0, len-sticks[0], 0)) {
					end = true;
					printf("%d\n", len);
					break;	
				}	
				used[0] = false;
			}	
		}
		if(!end)
			printf("%d\n", sum);
		memset(used, 0, sizeof(used));
	}
	//system("pause");
	return 0;
}
```





