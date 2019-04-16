# 540 Team Queue （queue模拟队伍数据记录操作） - PeterBishop - CSDN博客





2018年07月11日 11:18:02[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：16








```cpp
#include<iostream>
#include<queue>
#include<map>
#include<string>
//类似于二维数组，但是需要模仿queue必须要两个queue
using namespace std;
const int maxt= 1010;
int main()
{
	int t, kcase = 0;
	while (cin >> t && t)
	{
		cout << "Scenario #" << ++kcase << endl;
		map<int, int> team;//map记录队员对应的队序
		for (int i = 0; i < t; i++)
		{
			int n, x;
			cin >> n;
			while (n--)
			{
				cin >> x;
				team[x] = i;
			}
		}
		queue<int> q, q2[maxt];//queue模拟 q存储队序，q2数组存储各个队的队员
		while (true)
		{
			string cmd;
			int x;
			cin >> cmd;
			if (cmd[0] == 'S')
				break;
			else 
				if (cmd[0] == 'D')
				{
					int t = q.front();
					cout << q2[t].front() << endl;
					q2[t].pop();
					if (q2[t].empty())
						q.pop();
				}
				else
				{
					cin >> x;
					int t = team[x];
					if (q2[t].empty())
						q.push(t);
					q2[t].push(x);
				}
		}
		cout << endl;
	}
}
```




