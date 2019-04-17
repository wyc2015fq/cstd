# Leetcode 868. 二进制间距 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






### 868. 二进制间距

 显示英文描述
[我的提交](https://leetcode-cn.com/contest/weekly-contest-93/problems/binary-gap/submissions/)[返回竞赛](https://leetcode-cn.com/contest/weekly-contest-93/)







- 用户通过次数201
- 用户尝试次数220
- 通过次数207
- 提交次数396
- 题目难度Easy



给定一个正整数 `N`，找到并返回 `N` 的二进制表示中两个连续的 1 之间的最长距离。 

如果没有两个连续的 1，返回 `0` 。



示例 1：
输入：22
输出：2
解释：
22 的二进制是 0b10110 。
在 22 的二进制表示中，有三个 1，组成两对连续的 1 。
第一对连续的 1 中，两个 1 之间的距离为 2 。
第二对连续的 1 中，两个 1 之间的距离为 1 。
答案取两个距离之中最大的，也就是 2 。

示例 2：
输入：5
输出：2
解释：
5 的二进制是 0b101 。

示例 3：
输入：6
输出：1
解释：
6 的二进制是 0b110 。

示例 4：
输入：8
输出：0
解释：
8 的二进制是 0b1000 。
在 8 的二进制表示中没有连续的 1，所以返回 0 。



提示：
- `1 <= N <= 10^9`

```
class Solution {
public:
    int binaryGap(int N) {
        int maxnum = 0;
        int cnt = 0;
        while(N%2 == 0)N = N>>1;
        while(N>0){
            if(N&1){
                maxnum = max(maxnum,cnt);
                cnt = 1;
            }
            else cnt++;
            N = N>>1;
        }
        return maxnum;
    }
};
```

_














