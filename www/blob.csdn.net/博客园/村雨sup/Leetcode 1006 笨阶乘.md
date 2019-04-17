# Leetcode 1006. 笨阶乘 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






### 1006. 笨阶乘

 显示英文描述
[我的提交](https://leetcode-cn.com/contest/weekly-contest-127/problems/clumsy-factorial/submissions/)[返回竞赛](https://leetcode-cn.com/contest/weekly-contest-127/)







- 用户通过次数305
- 用户尝试次数347
- 通过次数309
- 提交次数665
- 题目难度Medium



通常，正整数 `n` 的阶乘是所有小于或等于 `n` 的正整数的乘积。例如，`factorial(10) = 10 * 9 * 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1`。

相反，我们设计了一个笨阶乘 `clumsy`：在整数的递减序列中，我们以一个固定顺序的操作符序列来依次替换原有的乘法操作符：乘法(*)，除法(/)，加法(+)和减法(-)。

例如，`clumsy(10) = 10 * 9 / 8 + 7 - 6 * 5 / 4 + 3 - 2 * 1`。然而，这些运算仍然使用通常的算术运算顺序：我们在任何加、减步骤之前执行所有的乘法和除法步骤，并且按从左到右处理乘法和除法步骤。

另外，我们使用的除法是地板除法（*floor division*），所以 `10 * 9 / 8` 等于 `11`。这保证结果是一个整数。

实现上面定义的笨函数：给定一个整数 `N`，它返回 `N` 的笨阶乘。



示例 1：
输入：4
输出：7
解释：7 = 4 * 3 / 2 + 1

示例 2：
输入：10
输出：12
解释：12 = 10 * 9 / 8 + 7 - 6 * 5 / 4 + 3 - 2 * 1



提示：
- `1 <= N <= 10000`
- `-2^31 <= answer <= 2^31 - 1`

```
class Solution {
public:
    int clumsy(int N) {
        if(N == 1) return 1;
        if(N == 2) return 2;
        if(N == 3) return 6;
        if(N == 4) return 7;
        int qianyikuai = INT_MAX;
        int qianyige = N;
        int j = 0;
        for(int i=N-1;i > 0;i--,j++){
            if(j%4 == 0)qianyige = qianyige*i;
            if(j%4 == 1)qianyige = qianyige/i;
            if(j%4 == 2){
                if(j == 2)qianyige += i;
                else qianyige -= i;
            }
            if(j%4 == 3){
                if(qianyikuai == INT_MAX) {qianyikuai = qianyige;qianyige = i;}
                else{
                    qianyikuai -= qianyige;
                    qianyige = i;
                }
            }
        }
        return qianyikuai - qianyige;
    }
};
```

__又是自己写的屎山

```
class Solution {
public:
    int clumsy(int N) {
        int rate = N;
        int ans = 0;
        int op = 0;
        for(int i=N-1;i > 0;i--){
            if(op == 0) rate*=i;
            if(op == 1) rate/=i;
            if(op == 2) {ans += rate+i;rate = 0;} //这里ans加过rate了，就把rate置0了
            if(op == 3) rate = -i;
            op = (op+1)%4;
        }
        ans += rate;
        return ans;
    }
};
```

_大佬写的，哭了














