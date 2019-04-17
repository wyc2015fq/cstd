# [LeetCode] Multiply Strings 字符串相乘 - Grandyang - 博客园







# [[LeetCode] Multiply Strings 字符串相乘](https://www.cnblogs.com/grandyang/p/4395356.html)







Given two non-negative integers `num1` and `num2`represented as strings, return the product of `num1` and `num2`, also represented as a string.

Example 1:
Input: num1 = "2", num2 = "3"
Output: "6"
Example 2:
Input: num1 = "123", num2 = "456"
Output: "56088"

Note:
- The length of both `num1` and `num2` is < 110.
- Both `num1` and `num2` contain only digits `0-9`.
- Both `num1` and `num2` do not contain any leading zero, except the number 0 itself.
- You must not use any built-in BigInteger libraryor convert the inputs to integer directly.



这道题让我们求两个字符串数字的相乘，输入的两个数和返回的数都是以字符串格式储存的，这样做的原因可能是这样可以计算超大数相乘，可以不受int或long的数值范围的约束，那么我们该如何来计算乘法呢，我们小时候都学过多位数的乘法过程，都是每位相乘然后错位相加，那么这里就是用到这种方法，举个例子，比如 89 x 76，那么根据小学的算术知识，我们不难写出计算过程如下：



```
8 9  <- num2
    7 6  <- num1
-------
    5 4
  4 8
  6 3
5 6
-------
6 7 6 4
```



如果自己在写些例子出来，不难发现，两数相乘得到的乘积的长度其实正好是两个数字的长度之和，若 num1 长度为m，num2 长度为n，则 num1 x num2 的长度为 m+n，还有就是要明白乘的时候为什么要错位，比如6乘8得到的48为啥要跟6乘9得到的54错位相加，因为8是十位上的数字，其本身相当于80，所以错开的一位实际上末尾需要补的0。还有一点需要观察出来的就是，num1 和 num2 中任意位置的两个数字相乘，得到的两位数在最终结果中的位置是确定的，比如 num1 中位置为i的数字乘以 num2 中位置为j的数字，那么得到的两位数字的位置为 i+j 和 i+j+1，明白了这些后，我们就可以进行错位相加了，累加出最终的结果。

由于我们要从个位上开始相乘，所以从 num1 和 num2 字符串的尾部开始往前遍历，分别提取出对应位置上的字符，将其转为整型后相乘。然后确定相乘后的两位数所在的位置 p1 和 p2，由于 p2 相较于 p1 是地位，所以我们将得到的两位数 mul 先加到 p2 位置上去，这样可能会导致 p2 位上的数字大于9，所以我们将十位上的数字要加到高位 p1 上去，只将余数留在 p2位置，这样每个位上的数字都变成一位。然后要做的是从高位开始，将数字存入结果res中，记住 leading zeros 要跳过，最后处理下 corner case，即若结果 res 为空，则返回 "0"，否则返回结果 res，代码如下：



```
class Solution {
public:
    string multiply(string num1, string num2) {
        string res = "";
        int m = num1.size(), n = num2.size();
        vector<int> vals(m + n);
        for (int i = m - 1; i >= 0; --i) {
            for (int j = n - 1; j >= 0; --j) {
                int mul = (num1[i] - '0') * (num2[j] - '0');
                int p1 = i + j, p2 = i + j + 1, sum = mul + vals[p2];
                vals[p1] += sum / 10;
                vals[p2] = sum % 10;
            }
        }
        for (int val : vals) {
            if (!res.empty() || val != 0) res.push_back(val + '0');
        }
        return res.empty() ? "0" : res;
    }
};
```



类似题目：

[Add Two Numbers](http://www.cnblogs.com/grandyang/p/4129891.html)

[Plus One](http://www.cnblogs.com/grandyang/p/4079357.html)

[Add Binary](http://www.cnblogs.com/grandyang/p/4084971.html)

[Add Strings](http://www.cnblogs.com/grandyang/p/5944311.html)



参考资料：

[https://leetcode.com/problems/multiply-strings/](https://leetcode.com/problems/multiply-strings/)

[https://leetcode.com/problems/multiply-strings/discuss/17605/Easiest-JAVA-Solution-with-Graph-Explanation](https://leetcode.com/problems/multiply-strings/discuss/17605/Easiest-JAVA-Solution-with-Graph-Explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












