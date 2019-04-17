# [LeetCode] Decode Ways 解码方法 - Grandyang - 博客园







# [[LeetCode] Decode Ways 解码方法](https://www.cnblogs.com/grandyang/p/4313384.html)







A message containing letters from `A-Z` is being encoded to numbers using the following mapping:
'A' -> 1
'B' -> 2
...
'Z' -> 26

Given an encoded message containing digits, determine the total number of ways to decode it.

For example,
Given encoded message `"12"`, it could be decoded as `"AB"` (1 2) or `"L"` (12).

The number of ways decoding `"12"` is 2.



这道题要求解码方法，跟之前那道[Climbing Stairs 爬梯子问题](http://www.cnblogs.com/grandyang/p/4079165.html) 非常的相似，但是还有一些其他的限制条件，比如说一位数时不能为0，两位数不能大于26，其十位上的数也不能为0，出去这些限制条件，根爬梯子基本没啥区别，也勉强算特殊的斐波那契数列，当然需要用动态规划Dynamci Programming来解。建立一位dp数组，长度比输入数组长多多2，全部初始化为1，因为斐波那契数列的前两项也为1，然后从第三个数开始更新，对应数组的第一个数。对每个数组首先判断其是否为0，若是将改为dp赋0，若不是，赋上一个dp值，此时相当如加上了dp[i - 1], 然后看数组前一位是否存在，如果存在且满足前一位不是0，且和当前为一起组成的两位数不大于26，则当前dp值加上dp[i - 2], 至此可以看出来跟斐波那契数组的递推式一样，代码如下：



C++ 解法一：

```
class Solution {
public:
    int numDecodings(string s) {
        if (s.empty() || (s.size() > 1 && s[0] == '0')) return 0;
        vector<int> dp(s.size() + 1, 0);
        dp[0] = 1;
        for (int i = 1; i < dp.size(); ++i) {
            dp[i] = (s[i - 1] == '0') ? 0 : dp[i - 1];
            if (i > 1 && (s[i - 2] == '1' || (s[i - 2] == '2' && s[i - 1] <= '6'))) {
                dp[i] += dp[i - 2];
            }
        }
        return dp.back();
    }
};
```



Java 解法一：

```
public class Solution {
    public int numDecodings(String s) {
        if (s.isEmpty() || (s.length() > 1 && s.charAt(0) == '0')) return 0;
        int[] dp = new int[s.length() + 1];
        dp[0] = 1;
        for (int i = 1; i < dp.length; ++i) {
            dp[i] = (s.charAt(i - 1) == '0') ? 0 : dp[i - 1];
            if (i > 1 && (s.charAt(i - 2) == '1' || (s.charAt(i - 2) == '2' && s.charAt(i - 1) <= '6'))) {
                dp[i] += dp[i - 2];
            }
        }
        return dp[s.length()];
    }
}
```



下面这种方法跟上面的方法的思路一样，只是写法略有不同：



C++ 解法二：

```
class Solution {
public:
    int numDecodings(string s) {
        if (s.empty()) return 0;
        vector<int> dp(s.size() + 1, 0);
        dp[0] = 1;
        for (int i = 1; i < dp.size(); ++i) {
            if (s[i - 1] != '0') dp[i] += dp[i - 1];
            if (i >= 2 && s.substr(i - 2, 2) <= "26" && s.substr(i - 2, 2) >= "10") {
                dp[i] += dp[i - 2];
            }
        }
        return dp.back();
    }
};
```



我们再来看一种空间复杂度为O(1)的解法，我们用两个变量c1, c2来分别表示s[i-1]和s[i-2]的解码方法，然后我们从i=1开始遍历，也就是字符串的第二个字符，我们判断如果当前字符为'0'，说明当前字符不能单独拆分出来，只能和前一个字符一起，我们先将c1赋为0，然后我们看前面的字符，如果前面的字符是1或者2时，我们就可以更新c1 = c1 + c2，然后c2 = c1 - c2，其实c2赋值为之前的c1，如果不满足这些条件的话，那么c2 = c1，参见代码如下：



C++ 解法三：

```
class Solution {
public:
    int numDecodings(string s) {
        if (s.empty() || s.front() == '0') return 0;
        int c1 = 1, c2 = 1;
        for (int i = 1; i < s.size(); ++i) {
            if (s[i] == '0') c1 = 0;
            if (s[i - 1] == '1' || (s[i - 1] == '2' && s[i] <= '6')) {
                c1 = c1 + c2;
                c2 = c1 - c2;
            } else {
                c2 = c1;
            }
        }
        return c1;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/7025/a-concise-dp-solution](https://discuss.leetcode.com/topic/7025/a-concise-dp-solution)

[https://discuss.leetcode.com/topic/6647/accepted-solution-to-decode-ways-no-need-to-take-care-of-0-case](https://discuss.leetcode.com/topic/6647/accepted-solution-to-decode-ways-no-need-to-take-care-of-0-case)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












