# 77. Combinations 回朔法例子 - 李鑫o_O - CSDN博客





2016年04月10日 13:30:12[hustlx](https://me.csdn.net/HUSTLX)阅读数：423
个人分类：[算法](https://blog.csdn.net/HUSTLX/article/category/6145896)












Given two integers n and k, return all possible combinations of k numbers out of 1 ... n.


For example,

If n = 4 and k = 2, a solution is:
[
  [2,4],
  [3,4],
  [2,3],
  [1,2],
  [1,3],
  [1,4],
]
```cpp
void help(vector<vector<int>> &res, vector<int> &re, int n,int k, int start) {
	if (k==0) {
		res.push_back(re);
		return;
	}
	for (int i = start;i <= n;i++) {
		re.push_back(i);
		help(res, re, n,k-1,i+1);
		re.pop_back();
	}
	return;
}
vector<vector<int>> combine(int n, int k) {
	vector<vector<int>> res;
	vector<int> re;
	help(res, re, n, k, 1);
	return res;
}
```








