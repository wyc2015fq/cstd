# Leetcode 1020. 将数组分成和相等的三个部分 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






### 1020. 将数组分成和相等的三个部分

 显示英文描述
[我的提交](https://leetcode-cn.com/contest/weekly-contest-129/problems/partition-array-into-three-parts-with-equal-sum/submissions/)[返回竞赛](https://leetcode-cn.com/contest/weekly-contest-129/)







- 用户通过次数321
- 用户尝试次数401
- 通过次数324
- 提交次数883
- 题目难度Easy



给定一个整数数组 `A`，只有我们可以将其划分为三个和相等的非空部分时才返回 `true`，否则返回 `false`。

形式上，如果我们可以找出索引 `i+1 < j` 且满足 `(A[0] + A[1] + ... + A[i] == A[i+1] + A[i+2] + ... + A[j-1] == A[j] + A[j-1] + ... + A[A.length - 1])` 就可以将数组三等分。



示例 1：
输出：[0,2,1,-6,6,-7,9,1,2,0,1]
输出：true
解释：0 + 2 + 1 = -6 + 6 - 7 + 9 + 1 = 2 + 0 + 1

示例 2：
输入：[0,2,1,-6,6,7,9,-1,2,0,1]
输出：false

示例 3：
输入：[3,3,6,5,-2,2,5,1,-9,4]
输出：true
解释：3 + 3 = 6 = 5 - 2 + 2 + 5 + 1 - 9 + 4



提示：
- `3 <= A.length <= 50000`
- `-10000 <= A[i] <= 10000`

```
class Solution {
public:
    bool canThreePartsEqualSum(vector<int>& A) {
        int sum = 0;
        for(auto num:A) sum+=num;
        if(sum%3!=0) return false;
        int k = sum/3;
        int sum1 = 0;
        int pos = 0;
        for(pos=0;pos < A.size();pos++){
            sum1 += A[pos];
            if(sum1 == k){pos++;break;}
        }
        
        int sum2 = 0;
        for(;pos < A.size();pos++){
            sum2 += A[pos];
            if(sum2 == k){pos++;break;}
        }
        int sum3 = 0;
        for(;pos < A.size();pos++){
            sum3 += A[pos];
        }
        if(sum3 == k) return true;
        return false;
    }
};
```

_














