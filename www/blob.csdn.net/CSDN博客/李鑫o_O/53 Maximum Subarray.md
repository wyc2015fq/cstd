# 53. Maximum Subarray - 李鑫o_O - CSDN博客





2016年03月30日 15:47:53[hustlx](https://me.csdn.net/HUSTLX)阅读数：541









Find the contiguous subarray within an array (containing at least one number) which has the largest sum.


For example, given the array `[−2,1,−3,4,−1,2,1,−5,4]`,


the contiguous subarray `[4,−1,2,1]` has the largest sum = `6`.

动态规划：



```cpp
int maxSubArray(vector<int>& nums) {
    int n = nums.size();
    int cur=0;
    int res = INT_MIN;
    for (int i = 0; i < nums.size(); i++) {
        cur = max(cur + nums[i], nums[i]);
        res = max(res, cur);
    }
    return res;
}
```





