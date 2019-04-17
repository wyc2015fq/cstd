# Codeforces Round #497 (Div. 2)1007A 1008A 1008B 总结 - PeterBishop - CSDN博客





2018年07月14日 12:45:33[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：69








A题没啥好说的，题目读完再动手！！！

B题真的难受了，一开始想复杂了，其实就三种情况讨论

```cpp
#include<iostream>
#include<algorithm>
using namespace std;
int main() {
	int n, ma = 0x7fffffff;
	cin >> n;
	while (n--) {
		int a, b;
		scanf("%d%d", &a, &b);
		if (a<b)
			swap(a, b);
		if (b>ma) 
		{
			printf("NO"); return 0;
		}
		else if (a>ma) 
			ma = b;
		else 
			ma = a;
	}
	printf("YES");
	return 0;
}
```

C题我一直觉得是求全排列然后去比较就好了，但是stl那个并不适用，所以卡了挺长时间，但其实是理解错了，有更好的方法

既然是与原来的比较，那么就是从原来的基础上进行排列，既然是相对的，那么为什么不找一个简单的基准呢？对，直接排序就好了，然后要做到最大值，例如123456，最大的肯定十234561，值就是5，用上一个大的去压住下一个大的，但是如果出现重复的话，就需要统计重复的数字的个数的最大值，程序中的最大值肯定是没办法被压住的，那么我们先把重复最多的放在一边，把剩下的放一边，那么一定可以保证的是多出来一个或多个（取决于最大值的个数）比重复最多的数字大的数字，并且需要重复最多的数字的一个或者多个去覆盖最大值的位置，这么一加一减，答案就是重复最多的数字的个数是无法被别的数字压住的，所以答案总数减去它。例：12223445667，把222放一边，13445667放一边，可以看出右边这个可以44566271，3没有用到，借了左边一个2

```cpp
#include<iostream>
#include<algorithm>
#include<map>
using namespace std;
int main() {
	int n;
	cin >> n;
	int x;
	map<int, int> mp;
	int mx = -10;
	for (int i = 0; i<n; i++) 
	{
		cin >> x;
		mp[x]++;
		mx = max(mx, mp[x]);
	}
	cout << n - mx << endl;
}
```
D和E水平不够暂不考虑。。。。            


