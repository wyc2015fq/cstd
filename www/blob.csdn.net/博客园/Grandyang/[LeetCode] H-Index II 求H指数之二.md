# [LeetCode] H-Index II 求H指数之二 - Grandyang - 博客园







# [[LeetCode] H-Index II 求H指数之二](https://www.cnblogs.com/grandyang/p/4782695.html)







Given an array of citations sorted in ascending order (each citation is a non-negative integer) of a researcher, write a function to compute the researcher's h-index.

According to the [definition of h-index on Wikipedia](https://en.wikipedia.org/wiki/H-index): "A scientist has index *h* if *h* of his/her *N* papers have at least *h* citations each, and the other *N − h* papers have no more than *h *citations each."

Example:
Input: `citations = [0,1,3,5,6]`
Output: 3 
Explanation: `[0,1,3,5,6] `means the researcher has `5` papers in total and each of them had 
             received 0`, 1, 3, 5, 6` citations respectively. 
             Since the researcher has `3` papers with at least `3` citations each and the remaining 
             two with no more than `3` citations each, her h-index is `3`.
Note:

If there are several possible values for *h*, the maximum one is taken as the h-index.

Follow up:
- This is a follow up problem to [H-Index](https://leetcode.com/problems/h-index/description/), where `citations` is now guaranteed to be sorted in ascending order.
- Could you solve it in logarithmic time complexity?



这题是之前那道 [H-Index](http://www.cnblogs.com/grandyang/p/4781203.html) 的拓展，输入数组是有序的，让我们在 O(log n) 的时间内完成计算，看到这个时间复杂度，而且数组又是有序的，应该有很敏锐的意识应该用二分查找法，属于博主之前的总结帖 [LeetCode Binary Search Summary 二分搜索法小结](https://www.cnblogs.com/grandyang/p/6854825.html) 中的第五类，目标值 target 会随着 mid 值的变化而变化，这里的 right 的初始值和 while 循环条件是否加等号是需要注意的问题，一般来说，博主的习惯是把 right 初始化为数组的长度，然后循环条件中不加等号，但是这种 right 的初始化对于这种目标值不固定的情况下不好使，需要初始化为长度减1（目前博主还没有遇到反例，有的话请务必告知博主）。那么此时循环条件中是否要加等号，这个其实很玄学，在 [Find Peak Element](https://www.cnblogs.com/grandyang/p/4217175.html#4111463) 中，right 也是初始化为数组长度减1，但是循环条件却不能加等号。这道题却一定需要加等号，否则会跪在 [0] 这个 test case，有些时候固有的规律并不好使，可能只能代一些 corner case 来进行检验，比如 [], [0], [1,2] 这种最简便的例子。

基于上面的分析，我们最先初始化 left 和 right 为0和 len-1，然后取中间值 mid，比较 citations[mid] 和 len-mid 做比较，如果前者大，则 right 移到 mid 之前，反之 right 移到 mid 之后，循环条件是 left<=right，最后返回 len-left 即可，参见代码如下：



```
class Solution {
public:
    int hIndex(vector<int>& citations) {
        int len = citations.size(), left = 0, right = len - 1;
        while (left <= right) {
            int mid = 0.5 * (left + right);
            if (citations[mid] == len - mid) return len - mid;
            else if (citations[mid] > len - mid) right = mid - 1;
            else left = mid + 1;
        }
        return len - left;
    }
};
```



类似题目：

[H-Index](http://www.cnblogs.com/grandyang/p/4781203.html)



参考资料：

[https://leetcode.com/problems/h-index-ii/](https://leetcode.com/problems/h-index-ii/)

[https://leetcode.com/problems/h-index-ii/discuss/71063/Standard-binary-search](https://leetcode.com/problems/h-index-ii/discuss/71063/Standard-binary-search)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












