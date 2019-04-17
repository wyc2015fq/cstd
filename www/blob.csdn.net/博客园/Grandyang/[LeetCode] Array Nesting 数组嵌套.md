# [LeetCode] Array Nesting 数组嵌套 - Grandyang - 博客园







# [[LeetCode] Array Nesting 数组嵌套](https://www.cnblogs.com/grandyang/p/6932727.html)







A zero-indexed array A consisting of N different integers is given. The array contains all integers in the range [0, N - 1].

Sets S[K] for 0 <= K < N are defined as follows:

S[K] = { A[K], A[A[K]], A[A[A[K]]], ... }.

Sets S[K] are finite for each K and should NOT contain duplicates.

Write a function that given an array A consisting of N integers, return the size of the largest set S[K] for this array.

Example 1:
Input: A = [5,4,0,3,1,6,2]
Output: 4
Explanation: 
A[0] = 5, A[1] = 4, A[2] = 0, A[3] = 3, A[4] = 1, A[5] = 6, A[6] = 2.

One of the longest S[K]:
S[0] = {A[0], A[5], A[6], A[2]} = {5, 6, 2, 0}



Note:
- N is an integer within the range [1, 20,000].
- The elements of A are all distinct.
- Each element of array A is an integer within the range [0, N-1].



这道题让我们找嵌套数组的最大个数，给的数组总共有n个数字，范围均在[0, n-1]之间，题目中也把嵌套数组的生成解释的很清楚了，其实就是值变成坐标，得到的数值再变坐标。那么实际上当循环出现的时候，嵌套数组的长度也不能再增加了，而出现的这个相同的数一定是嵌套数组的首元素，博主刚开始没有想清楚这一点，以为出现重复数字的地方可能是嵌套数组中间的某个位置，于是用个set将生成的嵌套数组存入，然后每次查找新生成的数组是否已经存在。而且还以原数组中每个数字当作嵌套数组的起始数字都算一遍，结果当然是TLE了。其实对于遍历过的数字，我们不用再将其当作开头来计算了，而是只对于未遍历过的数字当作嵌套数组的开头数字，不过在进行嵌套运算的时候，并不考虑中间的数字是否已经访问过，而是只要找到和起始位置相同的数字位置，然后更新结果res，参见代码如下：



解法一：

```
class Solution {
public:
    int arrayNesting(vector<int>& nums) {
        int n = nums.size(), res = INT_MIN;
        vector<bool> visited(n, false);
        for (int i = 0; i < nums.size(); ++i) {
            if (visited[nums[i]]) continue;
            res = max(res, helper(nums, i, visited));
        }
        return res;
    }
    int helper(vector<int>& nums, int start, vector<bool>& visited) {
        int i = start, cnt = 0;
        while (cnt == 0 || i != start) {
            visited[i] = true;
            i = nums[i];
            ++cnt;
        }
        return cnt;
    }
};
```



下面这种方法写法上更简洁一些，思路完全一样，参见代码如下：



解法二：

```
class Solution {
public:
    int arrayNesting(vector<int>& nums) {
        int n = nums.size(), res = INT_MIN;
        vector<bool> visited(n, false);
        for (int i = 0; i < n; ++i) {
            if (visited[nums[i]]) continue;
            int cnt = 0, j = i;
            while(cnt == 0 || j != i) {
                visited[j] = true;
                j = nums[j];
                ++cnt;
            }
            res = max(res, cnt);
        }
        return res;
    }
};
```



下面这种解法是网友@edyyy提醒博主的，我们可以优化解法二的空间，我们并不需要专门的数组来记录数组是否被遍历过，而是我们在遍历的过程中，将其交换到其应该出现的位置上，因为如果某个数出现在正确的位置上，那么它一定无法组成嵌套数组，这样就相当于我们标记了其已经访问过了，思路确实很赞啊，参见代码如下：



解法三：

```
class Solution {
public:
    int arrayNesting(vector<int>& nums) {
        int n = nums.size(), res = 0;
        for (int i = 0; i < n; ++i) {
            int cnt = 1;
            while (nums[i] != i && nums[i] != nums[nums[i]]) {
                swap(nums[i], nums[nums[i]]);
                ++cnt;
            }
            res = max(res, cnt);
        }
        return res;
    }
};
```



类似题目：

[Nested List Weight Sum II](http://www.cnblogs.com/grandyang/p/5615583.html)

[Flatten Nested List Iterator](http://www.cnblogs.com/grandyang/p/5358793.html)

[Nested List Weight Sum](http://www.cnblogs.com/grandyang/p/5340305.html)



参考资料：

[https://discuss.leetcode.com/topic/90537/this-is-actually-dfs](https://discuss.leetcode.com/topic/90537/this-is-actually-dfs)

[https://discuss.leetcode.com/topic/90538/c-java-clean-code-o-n](https://discuss.leetcode.com/topic/90538/c-java-clean-code-o-n)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












