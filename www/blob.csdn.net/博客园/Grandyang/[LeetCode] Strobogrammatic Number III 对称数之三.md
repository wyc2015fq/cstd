# [LeetCode] Strobogrammatic Number III 对称数之三 - Grandyang - 博客园







# [[LeetCode] Strobogrammatic Number III 对称数之三](https://www.cnblogs.com/grandyang/p/5203228.html)














A strobogrammatic number is a number that looks the same when rotated 180 degrees (looked at upside down).

Write a function to count the total strobogrammatic numbers that exist in the range of low <= num <= high.

Example:
Input: low = "50", high = "100"
Output: 3 
Explanation: 69, 88, and 96 are three strobogrammatic numbers.
Note:
Because the range might be a large number, the *low*and *high* numbers are represented as string.










这道题是之前那两道 [Strobogrammatic Number II](http://www.cnblogs.com/grandyang/p/5200919.html) 和 [Strobogrammatic Number](http://www.cnblogs.com/grandyang/p/5196960.html) 的拓展，又增加了难度，让我们找到给定范围内的对称数的个数，我们当然不能一个一个的判断是不是对称数，我们也不能直接每个长度调用第二道中的方法，保存所有的对称数，然后再统计个数，这样OJ会提示内存超过允许的范围，所以我们的解法是基于第二道的基础上，不保存所有的结果，而是在递归中直接计数，根据之前的分析，需要初始化n=0和n=1的情况，然后在其基础上进行递归，递归的长度len从low到high之间遍历，然后我们看当前单词长度有没有达到len，如果达到了，我们首先要去掉开头是0的多位数，然后去掉长度和low相同但小于low的数，和长度和high相同但大于high的数，然后结果自增1，然后分别给当前单词左右加上那五对对称数，继续递归调用，参见代码如下：



解法一：

```
class Solution {
public:
    int strobogrammaticInRange(string low, string high) {
        int res = 0;
        for (int i = low.size(); i <= high.size(); ++i) {
            find(low, high, "", i, res);
            find(low, high, "0", i, res);
            find(low, high, "1", i, res);
            find(low, high, "8", i, res);
        }
        return res;
    }
    void find(string low, string high, string path, int len, int &res) {
        if (path.size() >= len) {
            if (path.size() != len || (len != 1 && path[0] == '0')) return;
            if ((len == low.size() && path.compare(low) < 0) || (len == high.size() && path.compare(high) > 0)) {
                return;
            }
            ++res;
        }
        find(low, high, "0" + path + "0", len, res);
        find(low, high, "1" + path + "1", len, res);
        find(low, high, "6" + path + "9", len, res);
        find(low, high, "8" + path + "8", len, res);
        find(low, high, "9" + path + "6", len, res);
    }
};
```



上述代码可以稍微优化一下，得到如下的代码：



解法二：

```
class Solution {
public:
    int strobogrammaticInRange(string low, string high) {
        int res = 0;
        find(low, high, "", res);
        find(low, high, "0", res);
        find(low, high, "1", res);
        find(low, high, "8", res);
        return res;
    }
    void find(string low, string high, string w, int &res) {
        if (w.size() >= low.size() && w.size() <= high.size()) {
            if (w.size() == high.size() && w.compare(high) > 0) {
                return;
            }
            if (!(w.size() > 1 && w[0] == '0') && !(w.size() == low.size() && w.compare(low) < 0)) {
                ++res;
            }
        }
        if (w.size() + 2 > high.size()) return;
        find(low, high, "0" + w + "0", res);
        find(low, high, "1" + w + "1", res);
        find(low, high, "6" + w + "9", res);
        find(low, high, "8" + w + "8", res);
        find(low, high, "9" + w + "6", res);
    }
};
```



类似题目：

[Strobogrammatic Number II](http://www.cnblogs.com/grandyang/p/5200919.html)

[Strobogrammatic Number](http://www.cnblogs.com/grandyang/p/5196960.html)



参考资料：

[https://leetcode.com/problems/strobogrammatic-number-iii/](https://leetcode.com/problems/strobogrammatic-number-iii/)

[https://leetcode.com/problems/strobogrammatic-number-iii/discuss/67431/My-Java-solution-easy-to-understand](https://leetcode.com/problems/strobogrammatic-number-iii/discuss/67431/My-Java-solution-easy-to-understand)

[https://leetcode.com/problems/strobogrammatic-number-iii/discuss/67406/Clear-Java-AC-solution-using-Strobogrammatic-Number-II-method](https://leetcode.com/problems/strobogrammatic-number-iii/discuss/67406/Clear-Java-AC-solution-using-Strobogrammatic-Number-II-method)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












