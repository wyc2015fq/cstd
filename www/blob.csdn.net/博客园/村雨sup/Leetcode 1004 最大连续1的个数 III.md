# Leetcode 1004. 最大连续1的个数 III - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






### 1004. 最大连续1的个数 III

 显示英文描述
[我的提交](https://leetcode-cn.com/contest/weekly-contest-126/problems/max-consecutive-ones-iii/submissions/)[返回竞赛](https://leetcode-cn.com/contest/weekly-contest-126/)







- 用户通过次数97
- 用户尝试次数143
- 通过次数102
- 提交次数299
- 题目难度Medium



给定一个由若干 `0` 和 `1` 组成的数组 `A`，我们最多可以将 `K` 个值从 0 变成 1 。

返回仅包含 1 的最长（连续）子数组的长度。



示例 1：
输入：A = [1,1,1,0,0,0,1,1,1,1,0], K = 2
输出：6
解释： 
[1,1,1,0,0,1,1,1,1,1,1]
粗体数字从 0 翻转到 1，最长的子数组长度为 6。
示例 2：
输入：A = [0,0,1,1,0,0,1,1,1,0,1,1,0,0,0,1,1,1,1], K = 3
输出：10
解释：
[0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1]
粗体数字从 0 翻转到 1，最长的子数组长度为 10。


提示：
- `1 <= A.length <= 20000`
- `0 <= K <= A.length`
- `A[i]` 为 `0` 或 `1`

```
class Solution {
public:
    int longestOnes(vector<int>& A, int K) {
        queue<int> que;
        int left = 0;
        int res = 0;
        for(int right=0;right < A.size();right++){
            if(A[right] == 0){que.push(right);}
            if(que.size() > K) {//0的个数大于反转次数了
                left = que.front()+1;que.pop();
            }
            res = max(res,right-left+1);
        }
        return res;
    }
};
```

这种叫滑动窗口法，第一次见














