# [LeetCode] Factorial Trailing Zeroes 求阶乘末尾零的个数 - Grandyang - 博客园







# [[LeetCode] Factorial Trailing Zeroes 求阶乘末尾零的个数](https://www.cnblogs.com/grandyang/p/4219878.html)







Given an integer *n*, return the number of trailing zeroes in *n*!.

Note: Your solution should be in logarithmic time complexity.

Credits:
Special thanks to [@ts](https://oj.leetcode.com/discuss/user/ts) for adding this problem and creating all test cases.



这道题并没有什么难度，是让求一个数的阶乘末尾0的个数，也就是要找乘数中10的个数，而10可分解为2和5，而我们可知2的数量又远大于5的数量，那么此题即便为找出5的个数。仍需注意的一点就是，像25,125，这样的不只含有一个5的数字需要考虑进去。代码如下：



C++ 解法一：

```
class Solution {
public:
    int trailingZeroes(int n) {
        int res = 0;
        while (n) {
            res += n / 5;
            n /= 5;
        }
        return res;
    }
};
```



Java 解法一：

```
public class Solution {
    public int trailingZeroes(int n) {
        int res = 0;
        while (n > 0) {
            res += n / 5;
            n /= 5;
        }
        return res;
    }
}
```



这题还有递归的解法，思路和上面完全一样，写法更简洁了，一行搞定碉堡了。



C++ 解法二：

```
class Solution {
public:
    int trailingZeroes(int n) {
        return n == 0 ? 0 : n / 5 + trailingZeroes(n / 5);
    }
};
```



Java 解法二：

```
public class Solution {
    public int trailingZeroes(int n) {
        return n == 0 ? 0 : n / 5 + trailingZeroes(n / 5);
    }
}
```



参考资料：

[https://leetcode.com/discuss/19855/my-one-line-solutions-in-3-languages](https://leetcode.com/discuss/19855/my-one-line-solutions-in-3-languages)

[https://leetcode.com/discuss/44524/a-4ms-c-solution-using-method-in-wikipedia](https://leetcode.com/discuss/44524/a-4ms-c-solution-using-method-in-wikipedia)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












