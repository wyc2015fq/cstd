# 筛选法求n以内的素数 - wjheha的博客 - CSDN博客
2018年11月22日 19:23:38[wjheha](https://me.csdn.net/wjheha)阅读数：94
题目：找出100以内的素数？
```
方法一：空间换时间
#include <iostream>
#include <cmath>
using namespace std;
int main() {
	//统计100以内的素数，筛选法
	int sum = 0, a[100] = { 0 };
	for (int i = 2; i < sqrt(100.0); i++) {  //只用考虑根号n以内的数的倍数就可以了
		sum = i;
		if (a[sum] == 0) {   //已经被标记过了，就没必要再次标记了
			while (sum < 100) {
				sum = sum + i;
				if (sum < 100) {
					a[sum] = 1;    //不是素数，进行标记
				}
			}
		}
	}
	for (int i = 2; i < 100; i++) {
		if (a[i] == 0) {
			cout << i << " ";
		}
	}
	return 0;
}
```
```
方法二：逐一判断法
#include <iostream>
using namespace std;
int main() {
    //统计n以内的素数
    int n;
    cin >> n;
    cout << 2 << endl;
    for(int i = 3; i <= n; i+=2) {
        int k;
        for(k = 3; k < i; k+=2) {
            if(i % k == 0) {
                break;
            }
            if(k * k > i) {
                break;
            }
        }
        if(k * k > i) {
            cout << i << endl;
        }
    }
}
```
