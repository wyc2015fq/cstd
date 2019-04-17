# Leetcode 1005. K 次取反后最大化的数组和 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






### 1005. K 次取反后最大化的数组和

 显示英文描述
[我的提交](https://leetcode-cn.com/contest/weekly-contest-127/problems/maximize-sum-of-array-after-k-negations/submissions/)[返回竞赛](https://leetcode-cn.com/contest/weekly-contest-127/)







- 用户通过次数377
- 用户尝试次数413
- 通过次数385
- 提交次数986
- 题目难度Easy



给定一个整数数组 A，我们只能用以下方法修改该数组：我们选择某个个索引 `i` 并将 `A[i]` 替换为 `-A[i]`，然后总共重复这个过程 `K` 次。（我们可以多次选择同一个索引 `i`。）

以这种方式修改数组后，返回数组可能的最大和。



示例 1：
输入：A = [4,2,3], K = 1
输出：5
解释：选择索引 (1,) ，然后 A 变为 [4,-2,3]。

示例 2：
输入：A = [3,-1,0,2], K = 3
输出：6
解释：选择索引 (1, 2, 2) ，然后 A 变为 [3,1,0,2]。

示例 3：
输入：A = [2,-3,-1,5,-4], K = 2
输出：13
解释：选择索引 (1, 4) ，然后 A 变为 [2,3,-1,5,4]。



提示：
- `1 <= A.length <= 10000`
- `1 <= K <= 10000`
- `-100 <= A[i] <= 100`

```
class Solution {
public:
    int largestSumAfterKNegations(vector<int>& A, int K) {
        sort(A.begin(),A.end());
        int fushu = 0;
        for(int i=0;i < A.size();i++){
            if(A[i] < 0) fushu++;
        }
        int res = 0;
        if(fushu >= K){
            for(int i=0;i < K;i++){  //将前K个负数反转
                A[i] = 0-A[i];
            }
            for(int i=0;i < A.size();i++){ //求和
                res += A[i];
            }
            return res;
        }
        else{
            for(int i=0;i < fushu;i++){  //将所有负数反转
                A[i] = 0-A[i];
            }
            K = K - fushu; // 剩余反转次数
            sort(A.begin(),A.end());
            if(K%2 == 1){
                A[0] = 0-A[0];
            }
            for(int i=0;i < A.size();i++){ //求和
                res += A[i];
            }
            return res;
        }
    }
};
```

ac时觉的老子天下第一，看完大佬代码觉得我不适合写代码

```
class Solution {
public:
    int largestSumAfterKNegations(vector<int>& A, int K) {
        sort(A.begin(), A.end());
        int n = A.size();
        for (auto v : A)  //auto 学到了
        {
            if (v < 0 && K > 0)
            {
                v = -v;
                K --;
            }
        }
        sort(A.begin(), A.end());
        if (K&1) A[0] = -A[0];
        int ans = 0;
        for (auto v : A)
            ans += v;
        return ans;
    }
};
```

——














