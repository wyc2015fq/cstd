# Leetcode  1013. 总持续时间可被 60 整除的歌曲 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






### 1013. 总持续时间可被 60 整除的歌曲

 显示英文描述
[我的提交](https://leetcode-cn.com/contest/weekly-contest-128/problems/pairs-of-songs-with-total-durations-divisible-by-60/submissions/)[返回竞赛](https://leetcode-cn.com/contest/weekly-contest-128/)







- 用户通过次数450
- 用户尝试次数595
- 通过次数456
- 提交次数1236
- 题目难度Easy



在歌曲列表中，第 `i` 首歌曲的持续时间为 `time[i]` 秒。

返回其总持续时间（以秒为单位）可被 `60` 整除的歌曲对的数量。形式上，我们希望索引的数字  `i < j` 且有 `(time[i] + time[j]) % 60 == 0`。



示例 1：
输入：[30,20,150,100,40]
输出：3
解释：这三对的总持续时间可被 60 整数：
(time[0] = 30, time[2] = 150): 总持续时间 180
(time[1] = 20, time[3] = 100): 总持续时间 120
(time[1] = 20, time[4] = 40): 总持续时间 60

示例 2：
输入：[60,60,60]
输出：3
解释：所有三对的总持续时间都是 120，可以被 60 整数。



提示：
- `1 <= time.length <= 60000`
- `1 <= time[i] <= 500`

```
class Solution {
public:
    int numPairsDivisibleBy60(vector<int>& time) {
        vector<int> vec(60,0);
        int ans = 0;
        for(auto num:time){
            ans += vec[(60-(num%60))%60]; // 最后还要膜一下60，如果num == 60 会出现越界的情况
            vec[num%60]++;
        }
        return ans;
    }
};
```
















