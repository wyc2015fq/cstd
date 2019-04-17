# Leetcode 869. 重新排序得到 2 的幂 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






### 869. 重新排序得到 2 的幂

 显示英文描述
[我的提交](https://leetcode-cn.com/contest/weekly-contest-93/problems/reordered-power-of-2/submissions/)[返回竞赛](https://leetcode-cn.com/contest/weekly-contest-93/)







- 用户通过次数102
- 用户尝试次数134
- 通过次数103
- 提交次数296
- 题目难度Medium



从正整数 `N` 开始，我们按任何顺序（包括原始顺序）将数字重新排序，注意其前导数字不能为零。

如果我们可以通过上述方式得到 2 的幂，返回 `true`；否则，返回 `false`。



示例 1：
输入：1
输出：true

示例 2：
输入：10
输出：false

示例 3：
输入：16
输出：true

示例 4：
输入：24
输出：false

示例 5：
输入：46
输出：true



提示：
- `1 <= N <= 10^9`

```
class Solution {
public:
    bool reorderedPowerOf2(int N) {
        vector<int> temp1 = getvec(N);
        for(int i=1;i < 1e9;i=i<<1){
            vector<int> temp2 = getvec(i);
            if(temp1 == temp2) return true;
        }
        return false;
    }
    
    vector<int> getvec(int n){
        vector<int> res(10);
        while(n){
            res[n%10]++;
            n = n/10;
        }
        return res;
    }
    
};
```

_tql














