# 使用递归方法逆序一个Stack （空间复杂度O(1)） - Likes的博客 - CSDN博客
2019年02月23日 17:48:22[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：27
```cpp
#include <iostream>
#include <stack>
using namespace std;
int GetSackBotton(stack<int>& stk)//递归，抽出栈低元素
{
	int n = stk.top();
	stk.pop();
	if (stk.size() == 0)
		return n;
	int result= GetSackBotton(stk);
	stk.push(n);
	return result;
}
void ReverseStack(stack<int>& stk)
{
	int n = GetSackBotton(stk);
	if (stk.empty())
	{
		stk.push(n);
		return;
	}
	ReverseStack(stk);
	stk.push(n);
}
int main()
{
	stack<int> stk;
	for (int i = 1; i < 6; i++)
		stk.push(i);
	ReverseStack(stk);
	
	return 0;
}
```
