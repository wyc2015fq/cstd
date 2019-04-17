# 47. Permutations II - 李鑫o_O - CSDN博客





2016年03月28日 17:03:27[hustlx](https://me.csdn.net/HUSTLX)阅读数：235








```cpp
vector<vector<int>> permuteUnique(vector<int>& nums) {
    int n = nums.size();
    vector<vector<int>> pre = { {nums[0]} };
    for (int i = 1; i < n; i++) {
        vector<vector<int>> cur;
        for (auto temp : pre) {
            for (int j = 0; j <= temp.size(); j++) {
                auto temp1 = temp;
                temp1.insert(temp1.begin()+j, nums[i]);
                cur.push_back(temp1);
            }
        }
        pre = cur;
        sort(pre.begin(), pre.end());
        auto it = unique(pre.begin(), pre.end());
        pre.erase(it, pre.end());
    }
    return pre;
}
```




