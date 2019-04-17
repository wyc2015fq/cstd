# Leetcode 1012. 十进制整数的反码 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






### 1012. 十进制整数的反码

 显示英文描述
[我的提交](https://leetcode-cn.com/contest/weekly-contest-128/problems/complement-of-base-10-integer/submissions/)[返回竞赛](https://leetcode-cn.com/contest/weekly-contest-128/)







- 用户通过次数571
- 用户尝试次数602
- 通过次数582
- 提交次数1191
- 题目难度Easy



每个非负整数 `N` 都有其二进制表示。例如， `5` 可以被表示为二进制 `"101"`，`11` 可以用二进制 `"1011"` 表示，依此类推。注意，除 `N = 0` 外，任何二进制表示中都不含前导零。

二进制的反码表示是将每个 `1` 改为 `0` 且每个 `0` 变为 `1`。例如，二进制数 `"101"` 的二进制反码为 `"010"`。

给定十进制数 `N`，返回其二进制表示的反码所对应的十进制整数。



示例 1：
输入：5
输出：2
解释：5 的二进制表示为 "101"，其二进制反码为 "010"，也就是十进制中的 2 。

示例 2：
输入：7
输出：0
解释：7 的二进制表示为 "111"，其二进制反码为 "000"，也就是十进制中的 0 。

示例 3：
输入：10
输出：5
解释：10 的二进制表示为 "1010"，其二进制反码为 "0101"，也就是十进制中的 5 。



提示：
- `0 <= N < 10^9`

```
class Solution {
public:
    int bitwiseComplement(int N) {
        if(N == 0) return 1;
        int res = 0;
        vector<int> vec;
        while(N){
            vec.push_back(1-(N&1));
            N = N>>1;
        }
        for(int i=vec.size()-1;i >= 0;i--){
            res = (res<<1)|vec[i];
        }
        return res;
    }
};
```

_














