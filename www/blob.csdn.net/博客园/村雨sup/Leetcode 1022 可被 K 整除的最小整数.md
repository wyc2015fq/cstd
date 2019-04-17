# Leetcode 1022. 可被 K 整除的最小整数 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)









### 1022. 可被 K 整除的最小整数

 显示英文描述
[我的提交](https://leetcode-cn.com/contest/weekly-contest-129/problems/smallest-integer-divisible-by-k/submissions/)[返回竞赛](https://leetcode-cn.com/contest/weekly-contest-129/)







- 用户通过次数74
- 用户尝试次数262
- 通过次数75
- 提交次数1115
- 题目难度Medium



给定正整数 `K`，你需要找出可以被 K 整除的、仅包含数字 1 的最小正整数 N。

返回 `N` 的长度。如果不存在这样的 `N`，就返回 `-1`。



示例 1：
输入：1
输出：1
解释：最小的答案是 N = 1，其长度为 1。
示例 2：
输入：2
输出：-1
解释：不存在可被 2 整除的正整数 N 。
示例 3：
输入：3
输出：3
解释：最小的答案是 N = 111，其长度为 3。


提示：
- `1 <= K <= 10^5`

























```
class Solution {
public:
    int smallestRepunitDivByK(int K) {
        set<int> s;
        int c = 1;
        int prod = 1%K;
        while(!s.count(prod)) { //没有碰到过
            if(prod == 0) {
                return c;
            }
            s.insert(prod);
            prod = (prod*10+1)%K; //这也能用mo来算。。
            cout << prod << " ";
            ++c;
        }
        return -1;
    }
};
```

——学到了












