# [LeetCode] Reverse Integer 翻转整数 - Grandyang - 博客园







# [[LeetCode] Reverse Integer 翻转整数](https://www.cnblogs.com/grandyang/p/4125588.html)







Given a 32-bit signed integer, reverse digits of an integer.

Example 1:
Input: 123
Output: 321

Example 2:
Input: -123
Output: -321

Example 3:
Input: 120
Output: 21

Note:
Assume we are dealing with an environment which could only store integers within the 32-bit signed integer range: [−231,  231 − 1]. For the purpose of this problem, assume that your function returns 0 when the reversed integer overflows.





翻转数字问题需要注意的就是溢出问题，看了许多网上的解法，由于之前的OJ没有对溢出进行测试，所以网上很多人的解法没有处理溢出问题也能通过OJ。现在OJ更新了溢出测试，所以还是要考虑到。为什么会存在溢出问题呢，我们知道int型的数值范围是 -2147483648～2147483647， 那么如果我们要翻转 1000000009 这个在范围内的数得到 9000000001，而翻转后的数就超过了范围。我最开始的想法是，用 long 型数据，其数值范围为 -9223372036854775808~9223372036854775807， 远大于int型这样就不会出现溢出问题。但实际上 OJ 给出的官方解答并不需要使用 long，一看比自己的写的更精简一些，它没有特意处理正负号，仔细一想，果然正负号不影响计算，而且没有用 long 型数据，感觉写的更好一些，那么就贴出来吧：




解法一：

```
class Solution {
public:
    int reverse(int x) {
        int res = 0;
        while (x != 0) {
            if (abs(res) > INT_MAX / 10) return 0;
            res = res * 10 + x % 10;
            x /= 10;
        }
        return res;
    }
};
```



在贴出答案的同时，OJ还提了一个问题 To check for overflow/underflow, we could check if ret > 214748364 or ret < –214748364 before multiplying by 10. On the other hand, we do not need to check if ret == 214748364, why? （214748364 即为 INT_MAX / 10）

为什么不用check是否等于214748364呢，因为输入的x也是一个整型数，所以x的范围也应该在 -2147483648～2147483647 之间，那么x的第一位只能是1或者2，翻转之后res的最后一位只能是1或2，所以res只能是 2147483641 或 2147483642 都在int的范围内。但是它们对应的x为 1463847412 和 2463847412，后者超出了数值范围。所以当过程中res等于 214748364 时， 输入的x只能为 1463847412， 翻转后的结果为 2147483641，都在正确的范围内，所以不用check。

我们也可以用 long 型变量保存计算结果，最后返回的时候判断是否在int返回内，但其实题目中说了只能存整型的变量，所以这种方法就只能当个思路扩展了，参见代码如下：



解法二：

```
class Solution {
public:
    int reverse(int x) {
        long res = 0;
        while (x != 0) {
            res = 10 * res + x % 10;
            x /= 10;
        }
        return (res > INT_MAX || res < INT_MIN) ? 0 : res;
    }
};
```



类似题目：

[String to Integer (atoi)](http://www.cnblogs.com/grandyang/p/4125537.html)

[Reverse Bits](http://www.cnblogs.com/grandyang/p/4321355.html)



参考资料：

[https://leetcode.com/problems/reverse-integer/](https://leetcode.com/problems/reverse-integer/)

[https://leetcode.com/problems/reverse-integer/discuss/4060/My-accepted-15-lines-of-code-for-Java](https://leetcode.com/problems/reverse-integer/discuss/4060/My-accepted-15-lines-of-code-for-Java)

[https://leetcode.com/problems/reverse-integer/discuss/4056/Very-Short-(7-lines)-and-Elegant-Solution](https://leetcode.com/problems/reverse-integer/discuss/4056/Very-Short-(7-lines)-and-Elegant-Solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













