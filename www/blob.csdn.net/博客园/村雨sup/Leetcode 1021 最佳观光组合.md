# Leetcode 1021. 最佳观光组合 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






### 1021. 最佳观光组合

 显示英文描述
[我的提交](https://leetcode-cn.com/contest/weekly-contest-129/problems/best-sightseeing-pair/submissions/)[返回竞赛](https://leetcode-cn.com/contest/weekly-contest-129/)







- 用户通过次数91
- 用户尝试次数246
- 通过次数92
- 提交次数619
- 题目难度Medium



给定正整数数组 `A`，`A[i]` 表示第 `i` 个观光景点的评分，并且两个景点 `i` 和 `j` 之间的距离为 `j - i`。

一对景点（`i < j`）组成的观光组合的得分为（`A[i] + A[j] + i - j`）：景点的评分之和减去它们两者之间的距离。

返回一对观光景点能取得的最高分。



示例：
输入：[8,1,5,2,6]
输出：11
解释：i = 0, j = 2, `A[i] + A[j] + i - j = 8 + 5 + 0 - 2 = 11`


提示：
- `2 <= A.length <= 50000`
- `1 <= A[i] <= 1000`

```
class Solution {
public:
    //A[i] + i + A[j] - j
    int maxScoreSightseeingPair(vector<int>& A) {
        vector<int> vec(A.size(),0);
        vec[0] = A[0];
        for(int i=1;i < A.size();i++){
            vec[i] = max(vec[i-1],A[i]+i);  //vec[i] 在i以及i之前和的最大值
        }
        int ans = INT_MIN;
        for(int j=1;j < A.size();j++){
            ans = max(ans,vec[j-1]+A[j]-j);
        }
        return ans;
    }
};
```

_














