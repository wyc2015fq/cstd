# [CareerCup] 5.5 Number of Converted Bits 转换数字所需的位数 - Grandyang - 博客园







# [[CareerCup] 5.5 Number of Converted Bits 转换数字所需的位数](https://www.cnblogs.com/grandyang/p/4730247.html)







5.5 Write a function to determine the number of bits required to convert integer A to integer B.
EXAMPLE
Input: 31,14
Output: 2



这道题给了我们两个数字A和B，问如果将A转化为B需要变几个位，那么我们很容易想到要用异或来做，因为相同位异或为0，那么为1的为就是不相同的位，总和就是我们要求的结果。那么此题就转化为求异或结果中位‘1’的个数，我们可以用for循环来做，判断异或数的最低位是否为1，若为1就计数器加1，然后将异或数向右移动一位，以此类推直至异或数为0，参见代码如下：



解法一：

```
class Solution {
public:
    int bitConvertNumber(int a, int b) {
        int res = 0;
        for (int c = a ^ b; c != 0; c >>= 1) {
            res += c & 1;
        }
        return res;
    }
};
```



还有一种方法，这里不是将异或数每次向右移动一位，而是用n&(n-1)来清楚最低有效位Least Significant Bit，每清除一个，计数器增1，直到异或数为0停止，参见代码如下：



解法二：

```
class Solution {
public:
    int bitConvertNumber(int a, int b) {
        int res = 0;
        for (int c = a ^ b; c != 0; c &= c - 1) {
            ++res;
        }
        return res;
    }
};
```



Tips：根据之前那篇[Single Number III 单独的数字之三](http://www.cnblogs.com/grandyang/p/4741122.html)，我们知道得到最低有效位Least Significant Bit的方法是n&(-n)，而这道题又学习了清除最低有效位Least Significant Bit的方法是n&(n-1)，这些小技巧需要我们深刻理解，并在今后的解题中灵活的使用，这才是刷题的精髓所在。。。














