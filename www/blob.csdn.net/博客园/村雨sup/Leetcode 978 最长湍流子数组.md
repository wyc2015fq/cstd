# Leetcode 978. 最长湍流子数组 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






### 978. 最长湍流子数组

 显示英文描述
[我的提交](https://leetcode-cn.com/contest/weekly-contest-120/problems/longest-turbulent-subarray/submissions/)[返回竞赛](https://leetcode-cn.com/contest/weekly-contest-120/)







- 用户通过次数196
- 用户尝试次数229
- 通过次数200
- 提交次数567
- 题目难度Medium



当 `A` 的子数组 `A[i], A[i+1], ..., A[j]` 满足下列条件时，我们称其为*湍流子数组*：
- 若 `i <= k < j`，当 `k` 为奇数时， `A[k] > A[k+1]`，且当 `k` 为偶数时，`A[k] < A[k+1]`；
- 或 若 `i <= k < j`，当 `k` 为偶数时，`A[k] > A[k+1]` ，且当 `k` 为奇数时， `A[k] < A[k+1]`。

也就是说，如果比较符号在子数组中的每个相邻元素对之间翻转，则该子数组是湍流子数组。

返回 `A` 的最大湍流子数组的长度。



示例 1：
输入：[9,4,2,10,7,8,8,1,9]
输出：5
解释：(A[1] > A[2] < A[3] > A[4] < A[5])

示例 2：
输入：[4,8,12,16]
输出：2

示例 3：
输入：[100]
输出：1



提示：
- `1 <= A.length <= 40000`
- `0 <= A[i] <= 10^9`

```
class Solution {
public:
    int maxTurbulenceSize(vector<int>& A) {
        int cnt = 1;
        int maxnum = 1;
        for(int i=0;i < A.size()-1;i++){
            if(i&1){
                if(A[i] > A[i+1]) {
                    cnt++; maxnum = max(maxnum,cnt);
                }
                else cnt = 1;
            }
            else{
                if(A[i] < A[i+1]) {
                    cnt++; maxnum = max(maxnum,cnt);
                }
                else cnt = 1;
            }
        }
        cnt = 1;
        for(int i=0;i < A.size()-1;i++){
            if(i&1){
                if(A[i] < A[i+1]) {
                    cnt++; maxnum = max(maxnum,cnt);
                }
                else cnt = 1;
            }
            else{
                if(A[i] > A[i+1]) {
                    cnt++; maxnum = max(maxnum,cnt);
                }
                else cnt = 1;
            }
        }
        return maxnum;
    }
};
```

_














