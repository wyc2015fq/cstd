# Educational Codeforces Round 52 [Rated for Div. 2] 1065A 1065B 1065C - PeterBishop - CSDN博客





2018年10月13日 10:42:15[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：42








 A题良心秒切，没啥好说的

```cpp
#include<iostream>
using namespace std;
int main() {
	long long t;
	while (cin >> t) {
		while (t--){
			long long s, a, b, c;
			cin >> s >> a >> b >> c;
			long long ans;
			ans = s / c + s / c / a*b;
			cout << ans << endl;
		}
	}
}
```

B题我真的佛了，自己菜的不行居然没想起来完全图……其实就是照这个性质去看（完全图消耗的边数最多的）

```cpp
#include<iostream>
#include<algorithm>
using namespace std;

long long N, M;

int main() {
	cin >> N >> M;
	long long x = 0;
	while (M > x*(x - 1) / 2) {
		x++;
	}
	cout << max((long long)0, N - M * 2) << " " << N - x << endl;
	return 0;
}
```

C题贪心，但是题目实在没懂这期暂时不说了。。。。大家不要打我。。。。



