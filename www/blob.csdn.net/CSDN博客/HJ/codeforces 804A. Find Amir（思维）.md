# codeforces 804A. Find Amir（思维） - HJ - CSDN博客
2018年03月13日 13:05:49[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：106
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
题目链接：  [http://codeforces.com/problemset/problem/804/A](http://codeforces.com/problemset/problem/804/A)
题目大意：有编号为1~n的n个学校，且从编号为i的学校到编号为j的学校需要花费(i+j)%(n+1)，问不规定起点和终点，遍历所有学校所需要的最少花费为多少？
解题思路：
对于编号为1,2,3,...,n的学校：
①从1出发，去向n    花费为0
②从n继续，去向2    花费为1
③从2继续，去向n-1 花费为0
....
最后可以得到花费公式 (n+1)/2+1
```cpp
#include<iostream>
using namespace std;
int main()
{
	int n;
	while(cin>>n)
	{
		int sum=(n+1)/2-1;
		cout<<sum<<endl;
	}
	return 0;
}
```
