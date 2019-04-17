# [LeetCode] Largest Number 最大组合数 - Grandyang - 博客园







# [[LeetCode] Largest Number 最大组合数](https://www.cnblogs.com/grandyang/p/4225047.html)







Given a list of non negative integers, arrange them such that they form the largest number.

For example, given `[3, 30, 34, 5, 9]`, the largest formed number is `9534330`.

Note: The result may be very large, so you need to return a string instead of an integer.

Credits:
Special thanks to [@ts](https://oj.leetcode.com/discuss/user/ts) for adding this problem and creating all test cases.



这道题给了我们一个数组，让我们将其拼接成最大的数，那么根据题目中给的例子来看，主要就是要给给定数组进行排序，但是排序方法不是普通的升序或者降序，因为9要排在最前面，而9既不是数组中最大的也不是最小的，所以我们要自定义排序方法。如果不参考网友的解法，我估计是无法想出来的。这种解法对于两个数字a和b来说，如果将其都转为字符串，如果ab > ba，则a排在前面，比如9和34，由于934>349，所以9排在前面，再比如说30和3，由于303<330，所以3排在30的前面。按照这种规则对原数组进行排序后，将每个数字转化为字符串再连接起来就是最终结果。代码如下：



```
class Solution {
public:
    string largestNumber(vector<int>& nums) {
        string res;
        sort(nums.begin(), nums.end(), [](int a, int b) {
           return to_string(a) + to_string(b) > to_string(b) + to_string(a); 
        });
        for (int i = 0; i < nums.size(); ++i) {
            res += to_string(nums[i]);
        }
        return res[0] == '0' ? "0" : res;
    }
};
```



参考资料：

[http://leetcodesolution.blogspot.com/2015/01/leetcode-largest-number.html](http://leetcodesolution.blogspot.com/2015/01/leetcode-largest-number.html)
[https://leetcode.com/discuss/32431/simple-10-line-c-solution](https://leetcode.com/discuss/32431/simple-10-line-c-solution)
[https://leetcode.com/discuss/29699/share-a-short-code-in-c](https://leetcode.com/discuss/29699/share-a-short-code-in-c)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












