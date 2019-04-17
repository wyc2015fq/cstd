# Leetcode 1029. 可被 5 整除的二进制前缀 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






### 1029. 可被 5 整除的二进制前缀

 显示英文描述
[我的提交](https://leetcode-cn.com/contest/weekly-contest-130/problems/binary-prefix-divisible-by-5/submissions/)[返回竞赛](https://leetcode-cn.com/contest/weekly-contest-130/)







- 用户通过次数467
- 用户尝试次数662
- 通过次数477
- 提交次数1964
- 题目难度Easy



给定由若干 `0` 和 `1` 组成的数组 `A`。我们定义 `N_i`：从 `A[0]` 到 `A[i]` 的第 `i` 个子数组被解释为一个二进制数（从最高有效位到最低有效位）。

返回布尔值列表 `answer`，只有当 `N_i` 可以被 `5` 整除时，答案 `answer[i]` 为 `true`，否则为 `false`。



示例 1：
输入：[0,1,1]
输出：[true,false,false]
解释：
输入数字为 0, 01, 011；也就是十进制中的 0, 1, 3 。只有第一个数可以被 5 整除，因此 answer[0] 为真。

示例 2：
输入：[1,1,1]
输出：[false,false,false]

示例 3：
输入：[0,1,1,1,1,1]
输出：[true,false,false,false,true,false]

示例 4：
输入：[1,1,1,0,1]
输出：[false,false,false,false,false]



提示：
- `1 <= A.length <= 30000`
- `A[i]` 为 `0` 或 `1`

```
class Solution {
public:
    vector<bool> prefixesDivBy5(vector<int>& A) {
        vector<bool> res;
        long long num = 0;
        for(int i=0;i < A.size();i++){
            num = ((num << 1)|A[i])%5;
            // cout << num << " ";
            if(num%5 == 0) res.push_back(true);
            else res.push_back(false);
        }
        return res;
    }
};
```

看到长度3000就想到数据长度应该会超，事实证明确实，只要每次取个膜就好了。














