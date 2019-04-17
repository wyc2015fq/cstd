# [LeetCode] Self Dividing Numbers 自整除数字 - Grandyang - 博客园







# [[LeetCode] Self Dividing Numbers 自整除数字](https://www.cnblogs.com/grandyang/p/7906786.html)







A *self-dividing number* is a number that is divisible by every digit it contains.

For example, 128 is a self-dividing number because `128 % 1 == 0`, `128 % 2 == 0`, and `128 % 8 == 0`.

Also, a self-dividing number is not allowed to contain the digit zero.

Given a lower and upper number bound, output a list of every possible self dividing number, including the bounds if possible.

Example 1:
Input: 
left = 1, right = 22
Output: [1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 15, 22]



Note:
- The boundaries of each input argument are `1 <= left <= right <= 10000`.



这道题让我们找一个给定范围内的所有的自整除数字，所谓的自整除数字就是该数字可以整除其每一个位上的数字。既然这道题是Easy类，那么一般来说不需要用tricky的方法，直接暴力搜索就行了，遍历区间内的所有数字，然后调用子函数判断其是否是自整除数，是的话就加入结果res中。在子函数中，我们先把数字转为字符串，然后遍历每个字符，只要其为0，或者num无法整除该位上的数字，就返回false，循环结束后返回true，参见代码如下：



解法一：

```
class Solution {
public:
    vector<int> selfDividingNumbers(int left, int right) {
        vector<int> res;
        for (int i = left; i <= right; ++i) {
            if (check(i)) res.push_back(i);
        }
        return res;
    }
    bool check(int num) {
        string str = to_string(num);
        for (char c : str) {
            if (c == '0' || num % (c - '0')) return false;
        }
        return true;
    }
};
```



我们可以不用子函数，直接在大的for循环中加上一个for循环进行判断即可，参见代码如下：



解法二：

```
class Solution {
public:
    vector<int> selfDividingNumbers(int left, int right) {
        vector<int> res;
        for (int i = left, n = 0; i <= right; ++i) {
            for (n = i; n > 0; n /= 10) {
                if (n % 10 == 0 || i % (n % 10) != 0) break;
            }
            if (n == 0) res.push_back(i);
        }
        return res;
    }
};
```



类似题目：

[Perfect Number](http://www.cnblogs.com/grandyang/p/6636879.html)



参考资料：

[https://discuss.leetcode.com/topic/111201/java-c-clean-code](https://discuss.leetcode.com/topic/111201/java-c-clean-code)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












