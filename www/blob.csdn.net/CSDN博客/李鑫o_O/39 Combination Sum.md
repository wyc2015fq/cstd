# 39. Combination Sum - 李鑫o_O - CSDN博客





2016年03月27日 14:42:50[hustlx](https://me.csdn.net/HUSTLX)阅读数：216








回朔法：



```cpp
void help(vector<int>& candidates, vector<vector<int>>& res, vector<int>& temp, int target, int begin) {
    if (target == 0) {
        res.push_back(temp);
        return;
    }
    for (int i = begin; i < candidates.size()&&candidates[i]<=target; i++) {
        temp.push_back(candidates[i]);
        help(candidates, res, temp, target - candidates[i], i);
        temp.pop_back();
    }
    return;
}
vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
    sort(candidates.begin(), candidates.end());
    vector<vector<int>> res;
    vector<int> temp;
    help(candidates, res, temp, target, 0);
    return res;
}
```





