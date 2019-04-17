# [LeetCode] Palindrome Number 验证回文数字 - Grandyang - 博客园







# [[LeetCode] Palindrome Number 验证回文数字](https://www.cnblogs.com/grandyang/p/4125510.html)







Determine whether an integer is a palindrome. An integer is a palindrome when it reads the same backward as forward.

Example 1:
Input: 121
Output: true

Example 2:
Input: -121
Output: false
Explanation: From left to right, it reads -121. From right to left, it becomes 121-. Therefore it is not a palindrome.

Example 3:
Input: 10
Output: false
Explanation: Reads 01 from right to left. Therefore it is not a palindrome.

Follow up:

Coud you solve it without converting the integer to a string?




这道验证回文数字的题如果将数字转为字符串，就变成了验证回文字符串的题，没啥难度了，我们就直接来做follow up吧，不能转为字符串，而是直接对整数进行操作，我们可以利用取整和取余来获得我们想要的数字，比如 1221 这个数字，如果 计算 1221 / 1000， 则可得首位1， 如果 1221 % 10， 则可得到末尾1，进行比较，然后把中间的22取出继续比较。代码如下：



解法一：

```
class Solution {
public:
    bool isPalindrome(int x) {
        if (x < 0) return false;
        int div = 1;
        while (x / div >= 10) div *= 10;
        while (x > 0) {
            int left = x / div;
            int right = x % 10;
            if (left != right) return false;
            x = (x % div) / 10;
            div /= 100;
        }
        return true;
    }
};
```



我们再来看一种很巧妙的解法，还是首先判断x是否为负数，这里我们可以用一个小trick，因为我们知道整数的最高位不能是0，所以回文数的最低位也不能为0，数字0除外，所以如果发现某个正数的末尾是0了，也直接返回false即可。好，下面来看具体解法，要验证回文数，那么就需要看前后半段是否对称，如果把后半段翻转一下，就看和前半段是否相等就行了。所以我们的做法就是取出后半段数字，进行翻转，具体做法是，每次通过对10取余，取出最低位的数字，然后加到取出数的末尾，就是将revertNum乘以10，再加上这个余数，这样我们的翻转也就同时完成了，每取一个最低位数字，x都要自除以10。这样当revertNum大于等于x的时候循环停止。由于回文数的位数可奇可偶，如果是偶数的话，那么revertNum就应该和x相等了；如果是奇数的话，那么最中间的数字就在revertNum的最低位上了，我们除以10以后应该和x是相等的，参见代码如下：



解法二：

```
class Solution {
public:
    bool isPalindrome(int x) {
        if (x < 0 || (x % 10 == 0 && x != 0)) return false;
        int revertNum = 0;
        while (x > revertNum) {
            revertNum = revertNum * 10 + x % 10;
            x /= 10;
        }
        return x == revertNum || x == revertNum / 10;
    }
};
```



下面这种解法由网友zeeng提供，如果是palindrome，反转后仍是原数字，就不可能溢出，只要溢出一定不是palindrome返回false就行。可以参考[Reverse Integer](http://www.cnblogs.com/grandyang/p/4125588.html)这题, 直接调用Reverse()。



解法三：

```
class Solution {
public:
    bool isPalindrome(int x) {
        if (x < 0 || (x % 10 == 0 && x != 0)) return false;
        return reverse(x) == x;
    }
    int reverse(int x) {
        int res = 0;
        while (x != 0) {
            if (res > INT_MAX / 10) return -1;
            res = res * 10 + x % 10;
            x /= 10;
        }
        return res;
    }
};
```



类似题目：

[Palindrome Linked List](http://www.cnblogs.com/grandyang/p/4635425.html)

[Reverse Integer](http://www.cnblogs.com/grandyang/p/4125588.html)



参考资料：

[https://leetcode.com/problems/palindrome-number/solution/](https://leetcode.com/problems/palindrome-number/solution/)

[https://leetcode.com/problems/palindrome-number/discuss/5127/9-line-accepted-Java-code-without-the-need-of-handling-overflow](https://leetcode.com/problems/palindrome-number/discuss/5127/9-line-accepted-Java-code-without-the-need-of-handling-overflow)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













