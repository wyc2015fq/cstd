# Leetcode 969. 煎饼排序 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






### 969. 煎饼排序

 显示英文描述
[我的提交](https://leetcode-cn.com/contest/weekly-contest-118/problems/pancake-sorting/submissions/)[返回竞赛](https://leetcode-cn.com/contest/weekly-contest-118/)







- 用户通过次数134
- 用户尝试次数158
- 通过次数135
- 提交次数256
- 题目难度Medium



给定数组 `A`，我们可以对其进行*煎饼翻转*：我们选择一些正整数 `k <= A.length`，然后反转 `A` 的前 k 个元素的顺序。我们要执行零次或多次煎饼翻转（按顺序一次接一次地进行）以完成对数组 `A` 的排序。

返回能使 `A` 排序的煎饼翻转操作所对应的 k 值序列。任何将数组排序且翻转次数在 `10 * A.length` 范围内的有效答案都将被判断为正确。



示例 1：
输入：[3,2,4,1]
输出：[4,2,4,3]
解释：
我们执行 4 次煎饼翻转，k 值分别为 4，2，4，和 3。
初始状态 A = [3, 2, 4, 1]
第一次翻转后 (k=4): A = [1, 4, 2, 3]
第二次翻转后 (k=2): A = [4, 1, 2, 3]
第三次翻转后 (k=4): A = [3, 2, 1, 4]
第四次翻转后 (k=3): A = [1, 2, 3, 4]，此时已完成排序。 

示例 2：
输入：[1,2,3]
输出：[]
解释：
输入已经排序，因此不需要翻转任何内容。
请注意，其他可能的答案，如[3，3]，也将被接受。



提示：
- `1 <= A.length <= 100`
- `A[i]` 是 `[1, 2, ..., A.length]` 的排列

```
class Solution {
public:
    int findmax(vector<int> a,int len){
        int maxnum = INT_MIN;
        int xiabiao = -1;
        for(int i=0;i < len;i++){
            if(a[i] > maxnum){
                maxnum = a[i];
                xiabiao = i;
            }
        }
        return xiabiao;
    }
    void fanzhuan(vector<int>& a,int n){
        for(int i=0;i < n/2;i++){
            int t = a[i];
            a[i] = a[n-i-1];
            a[n-i-1] = t;
        }
    }
    
    vector<int> pancakeSort(vector<int>& A) {
        int lena = A.size();
        vector<int> res;
        for(int i=lena;i > 0;i--){
            int pos = findmax(A,i);
            fanzhuan(A,pos+1);
            res.push_back(pos+1);
            fanzhuan(A,i);
            res.push_back(i);
        }
        return res;
    }
};
```

这是一个非常有名的问题。原始问题的难点在于怎么用最少的次数排好序，但是此题放宽了要求（翻转次数在 10 * A.length 范围内的有效答案都将被判断为正确），所以我们可以很容易想到的一个方法是：首先找出数组中的最大数，然后将最大数及其之前的全部数进行翻转，例如





















