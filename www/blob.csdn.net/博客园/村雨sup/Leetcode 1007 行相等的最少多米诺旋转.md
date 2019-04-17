# Leetcode 1007. 行相等的最少多米诺旋转 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






### 1007. 行相等的最少多米诺旋转

 显示英文描述
[我的提交](https://leetcode-cn.com/contest/weekly-contest-127/problems/minimum-domino-rotations-for-equal-row/submissions/)[返回竞赛](https://leetcode-cn.com/contest/weekly-contest-127/)







- 用户通过次数203
- 用户尝试次数238
- 通过次数207
- 提交次数517
- 题目难度Medium



在一排多米诺骨牌中，`A[i]` 和 `B[i]` 分别代表第 i 个多米诺骨牌的上半部分和下半部分。（一个多米诺是两个从 1 到 6 的数字同列平铺形成的 —— 该平铺的每一半上都有一个数字。）

我们可以旋转第 `i` 张多米诺，使得 `A[i]` 和 `B[i]` 的值交换。

返回能使 `A` 中所有值或者 `B` 中所有值都相同的最小旋转次数。

如果无法做到，返回 `-1`.



示例 1：

![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2019/03/08/domino.png)
输入：A = [2,1,2,4,2,2], B = [5,2,6,2,3,2]
输出：2
解释：
图一表示：在我们旋转之前， A 和 B 给出的多米诺牌。
如果我们旋转第二个和第四个多米诺骨牌，我们可以使上面一行中的每个值都等于 2，如图二所示。

示例 2：
输入：A = [3,5,1,2,3], B = [3,6,3,3,4]
输出：-1
解释：
在这种情况下，不可能旋转多米诺牌使一行的值相等。



提示：
- `1 <= A[i], B[i] <= 6`
- `2 <= A.length == B.length <= 20000`

```
class Solution {
public:
    static const int INF = INT_MAX;
    
    int caozuo(vector<int> a,vector<int> b,int x){
        int res = 0;
        for(int i=0;i < a.size();i++){
            if(a[i] != x){
                if(b[i] == x){
                    res++;
                }
                else return INF;
            }
        }
        return res;
    }
    
    int minDominoRotations(vector<int>& A, vector<int>& B) {
        int res = INF;
        for(int i=1;i <= 6;i++){
            res = min(res,caozuo(A,B,i));
            res = min(res,caozuo(B,A,i));
        }
        
        return res == INF ? -1:res;
    }
};
```

__注意，ab范围在1~6之间，直接每个都测试过去














