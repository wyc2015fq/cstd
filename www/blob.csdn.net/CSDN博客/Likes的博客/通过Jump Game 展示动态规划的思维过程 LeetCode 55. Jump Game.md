# 通过Jump Game 展示动态规划的思维过程 LeetCode 55. Jump Game - Likes的博客 - CSDN博客
2019年03月09日 20:17:03[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：8标签：[leetcode																[55																[jump game																[dp																[动态规划](https://so.csdn.net/so/search/s.do?q=动态规划&t=blog)](https://so.csdn.net/so/search/s.do?q=dp&t=blog)](https://so.csdn.net/so/search/s.do?q=jump game&t=blog)](https://so.csdn.net/so/search/s.do?q=55&t=blog)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[算法](https://blog.csdn.net/songchuwang1868/article/category/7908138)
[https://leetcode.com/problems/jump-game/](https://leetcode.com/problems/jump-game/)
方法一：递归（回溯、DFS） 超时
在递归树的叶子节点判断，如果能够达到将成员变量置为true
```cpp
class Solution {
public:
	bool result;
	void fun(vector<int>& nums, int start)
	{
		if (start>=nums.size()-1)
		{
			result = true;
			return;
		}
		if (nums[start] == 0)
			return;
		for (int i = 1; i <= nums[start]; i++)
		{
			fun(nums, start + i);
		}
	}
	bool canJump(vector<int>& nums) {
		int start = 0;
		result = false;
		fun(nums, 0);
		return result;
	}
};
```
方法二：递归
递归树上任意一条路径上可达就可达，在for循环中若有一个fun返回值为true，即可结束当前递归，返回true。显然反复计算了fun
(nums,i)，为此我们可以向动态规划改进。但是时间复杂度没有本质变化 ，依然超时
```cpp
class Solution {
public:
	
	bool fun(vector<int>& nums, int start)
	{
		if (start>=nums.size()-1)
			return true;
		if (nums[start] == 0)
			return false;
		for (int i = 1; i <= nums[start]; i++)
		{
			bool r = fun(nums, start + i);
			if (r == true)
				return true;
		}
		return false;
	}
	bool canJump(vector<int>& nums) {
		return fun(nums, 0);
	}
};
```
三、动态规划--自上而下（从递归树上面往下面走）
通过减枝，时间复杂度降低，但是依然无法过完所有case，当数据量很大时，栈溢出。
```cpp
class Solution {
public:
	unordered_map<int, bool> table;
	bool fun(vector<int>& nums, int start)
	{
		if (start>=nums.size()-1)
			return true;
		if (nums[start] == 0)
			return false;
		for (int i = 1; i <= nums[start]; i++)
		{
			/*bool r = fun(nums, start + i);
			if (r == true)
				return true;*/
			//先查表，没有再计算
			bool r;
			if (table.find(start + i) == table.end())
			{
				r = fun(nums, start + i);
				table.insert(make_pair(start + i, r));
			}
			else
			{
				r = table[start + i];
			}
			if (r == true)
				return true;
		}
		return false;
	}
	bool canJump(vector<int>& nums) {
		return fun(nums, 0);
	}
};
```
四、动态规划--自底向上
```cpp
class Solution {
public:
	bool canJump(vector<int>& nums) {
		vector<bool> table(nums.size(), true);
		
		for (int k = nums.size() - 2; k >= 0; k--)
		{
			if (nums[k] == 0)
			{
				table[k] = false;
				continue;
			}
			bool r = false;
			for (int i = 1; i <= nums[k]; i++)
			{
				if (k + i >= nums.size() - 1||table[k + i] == true)
				{
					r = true;
					break;
				}
			}
			table[k] = r;
		}
		return table[0];
	}
};
```
五、贪心算法
...
