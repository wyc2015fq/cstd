# c++ map根据value排序以及lamda表达式的应用 - 李鑫o_O - CSDN博客





2016年05月03日 10:20:26[hustlx](https://me.csdn.net/HUSTLX)阅读数：1372








```cpp
vector<int> topKFrequent(vector<int>& nums, int k) {
	map<int, int> m;
	vector<pair<int, int>> temp;
	vector<int> res;
	for (auto n : nums) {
		m[n]++;
	}
	for (auto i : m) {
		temp.push_back(i);
	}
	std::sort(temp.begin(), temp.end(), [](pair<int, int>& a, pair<int, int>& b) {return a.second > b.second;});
	for (auto i:temp) {
		res.push_back(i.first);
		k--;
		if (k == 0) return res;
	}
}
```




