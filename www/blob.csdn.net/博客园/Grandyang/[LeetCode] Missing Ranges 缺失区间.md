# [LeetCode] Missing Ranges 缺失区间 - Grandyang - 博客园







# [[LeetCode] Missing Ranges 缺失区间](https://www.cnblogs.com/grandyang/p/5184890.html)







Given a sorted integer array where the range of elements are [0, 99] inclusive, return its missing ranges.
For example, given [0, 1, 3, 50, 75], return [“2”, “4->49”, “51->74”, “76->99”] 



这道题让我们求缺失区间，跟之前那道[Summary Ranges](http://www.cnblogs.com/grandyang/p/4603555.html)很类似，这道题让我们求缺失的空间，给了一个空间的范围[lower upper]，缺失的空间的范围需要在给定的空间范围内。我们首先将lower赋给l，然后开始遍历nums数组，如果i小于nums长度且当前数字小于等于upper，我们让r等于当前数字，否则如果当i等于nums的长度时或者当前数字大于upper时，将r赋为upper+1。然后判断l和r的值，若相同，l自增1，否则当r大于l时，说明缺失空间存在，我们看l和r是否差1，如果是，说明只缺失了一个数字，若不是，则说明缺失了一个区间，我们分别加上数字或者区间即可，参见代码如下：



```
class Solution {
public:
    vector<string> findMissingRanges(vector<int>& nums, int lower, int upper) {
        vector<string> res;
        int l = lower;
        for (int i = 0; i <= nums.size(); ++i) {
            int r = (i < nums.size() && nums[i] <= upper) ? nums[i] : upper + 1;
            if (l == r) ++l;
            else if (r > l) {
                res.push_back(r - l == 1 ? to_string(l) : to_string(l) + "->" + to_string(r - 1));
                l = r + 1;
            }
        }
        return res;
    }
};
```



类似题目：

[Summary Ranges](http://www.cnblogs.com/grandyang/p/4603555.html)



参考资料：

[https://leetcode.com/discuss/29206/my-concise-java-accepted-solution](https://leetcode.com/discuss/29206/my-concise-java-accepted-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)














