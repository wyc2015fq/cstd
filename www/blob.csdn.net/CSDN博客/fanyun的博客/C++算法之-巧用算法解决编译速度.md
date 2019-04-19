# C++算法之-巧用算法解决编译速度 - fanyun的博客 - CSDN博客
2018年07月29日 21:32:46[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：92
      C++ 是一门常用的语言。面对众多的 C++ 代码，等待源文件编译的漫长时间是个令人糟心的时刻,一直以来大家对此怨声载道。      
      经过一番调查和研究，你发现一些源代码之间是有依赖关系的。例如，某个源文件 a.cpp 编译链接生成了动态链接库 a.dll，而 b.cpp 编译链接生成的 b.dll 依赖于 a.dll。这个时候，必须等待 a.dll 生成之后才能生成 b.dll。为了表达简单，我们这个时候称 b.cpp 依赖于 a.cpp。
       大长经常能够使用一个分布式并行的编译平台，可以同时编译多个不互相依赖的文件，大大提高了源代码的编译速度。然而当某些依赖链很长的时候，这个编译平台也无能为力，只能按照依赖顺序一个一个完成编译，从而造成了很长的编译时间。
       为了验证这个想法，你决定着手通过代码分析这些文件之间的编译顺序。已知这些文件的文件名，以及这些文件所依赖的其他文件，你需要编写一个程序，输出一个可行的编译所有源文件的编译顺序。如果有多种可行的序列，请输出所有文件名序列中字典序最小的那一个（序列 (a1, a2, ..., an) 字典序小于序列 (b1, b2, ..., bn)，当且仅当存在某个 i ，使得 ai 的字典序小于 bi，并且对于任意 j < i ，都有 aj = bj）。
**       算法输入：**
        输入包含多组测试数据。输入的第一行包含一个整数 T(T ≤ 100)，表示输入中一共包含有 T 组测试数据。每组测试数据第一行是一个整数 N(N ≤ 1000)，表示一共有 N 个源代码文件。随后一共有 N 行数据，其中第 i(0 ≤ i < N) 行数据包含序号为 i 的源代码文件的依赖信息。每一行开头是一个字符串，表示这一个文件的文件名，随后一个整数 m(0 ≤ m ≤ N)，表示编译这个源文件之前需要先编译 m 个依赖文件。之后是 m 个整数 j0 ... jm-1，表示这 m 个依赖文件的序号(0 ≤ j < N) 。所有的文件名仅由小写字母、数字或“.”组成，并且不会超过 10 个字符。保证 n 个源代码文件的文件名互不相同。
**       算法输出：**
        对于每一组输入，按照编译先后顺序输出一组可行的编译顺序，一行一个文件名。如果有多种可行的序列，请输出所有文件名序列中字典序最小的那一个。如果不存在可行的编译顺序，输出一行 ERROR。每组测试数据末尾输出一个空行。
样例输入：
```cpp
3
2
a.cpp 0
b.cpp 1 0
2
cb 0
c 0
2
a.cpp 1 1
b.cpp 1 0
```
样例输出：
```cpp
a.cpp
b.cpp
c
cb
ERROR
```
```cpp
#include "stdafx.h"
 
#include <vector>
#include <iostream>
#include <random>
#include <map>
#include <string>
using namespace std;
bool isEmpty(vector<vector<int>> vec)
{
	for (auto i : vec)
	{
		for (auto j : i)
		{
			if (j != 0)
				return false;
		}
	}
	return true;
}
int main()
{
	int s;
	cin >> s;
	while (s--)
	{
		int n;
		cin >> n;
		vector<vector<int>> vec(n,vector<int>(n,0));
		map<int, string> m;
		for (int i = 0;i < n;i++)
		{
			string s;
			int num;
			cin >> s >> num;
			m.insert(make_pair(i,s));
			if (num != 0)
			{
				for (int j = 0;j < num;j++)
				{
					int pre;
					cin >> pre;
					vec[pre][i] = 1;
				}
			}
		}
		vector<string> vstr;
		vector<int> used(n,0);
		bool change = false;
		while (!isEmpty(vec))
		{
			
			for (int i = 0;i < n;i++)
			{
				if (!used[i])
				{
					int flag = 0;
					for (int j = 0;j < n;j++)
					{
						if (vec[j][i])
						{
							flag = 1;
							break;
						}
					}
					if (flag == 0)
					{
						change = true;
						vstr.push_back(m[i]);
						int count = 0;
						for (auto i : used)
							count += i;
						for (int h = 0;h < n;h++)
						{
							if (vec[i][h] == 1 && count == n - 2)
								vstr.push_back(m[h]);
							vec[i][h] = 0;
 
 
						}
						used[i] = 1;
						
					}
				}
				
			}
			if (!change)
			{
				vstr.clear();
				vstr.push_back("ERROR");
				break;
			}
		}
		for (auto i : vstr) 
		{
			cout << i << endl;
		}
		cout << endl;
	}
	int a;
	cin >> a;
	return 0;
}
```
