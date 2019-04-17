# [LeetCode] Cracking the Safe 破解密码 - Grandyang - 博客园







# [[LeetCode] Cracking the Safe 破解密码](https://www.cnblogs.com/grandyang/p/8452361.html)







There is a box protected by a password. The password is `n` digits, where each letter can be one of the first `k` digits `0, 1, ..., k-1`.

You can keep inputting the password, the password will automatically be matched against the last `n` digits entered.

For example, assuming the password is `"345"`, I can open it when I type `"012345"`, but I enter a total of 6 digits.

Please return any string of minimum length that is guaranteed to open the box after the entire string is inputted.

Example 1:
Input: n = 1, k = 2
Output: "01"
Note: "10" will be accepted too.



Example 2:
Input: n = 2, k = 2
Output: "00110"
Note: "01100", "10011", "11001" will be accepted too.



Note:
- `n` will be in the range `[1, 4]`.
- `k` will be in the range `[1, 10]`.
- `k^n` will be at most `4096`.



这道题说的是给了k个数字，值为0到k-1，让我们组成n位密码。让我们找一个万能钥匙串，能破解任意的n位密码组合，这里对于破解的定义为只要密码是钥匙串的子串就可以破解了，然我们求出最短的一个万能钥匙串。来看一个例子，n=2，k=2，那么密码的组合有四种，

00，01，10，11

所以 00110 就是一种钥匙串，因为密码 00 (00110), 01 (00110), 10 (00110), 11 (00110), 分别都包括在钥匙串中。我们可以发现，为了尽可能的使钥匙串变短，所以我们的密码之间尽可能要相互重叠，比如00和01，就共享一个0，如果是3个数，012和120共享两个数"12"，那么我们可以发现，两个长度为n的密码最好能共享n-1个数字，这样累加出来的钥匙串肯定是最短的。

密码共有n位，每一个位可以有k个数字，那么总共不同的密码总数就有k的n次方个。我们的思路是先从n位都是0的密码开始，取出钥匙串的最后n个数字，然后将最后一个数字依次换成其他数字，我们用一个HashSet来记录所有遍历过的密码，这样如果不在集合中，说明是一个新密码，而生成这个新密码也只是多加了一个数字，这样能保证我们的钥匙串最短，这是一种贪婪的解法，相当的巧妙，参见代码如下：



解法一：

```
class Solution {
public:
    string crackSafe(int n, int k) {
        string res = string(n, '0');
        unordered_set<string> visited{{res}};
        for (int i = 0; i < pow(k, n); ++i) {
            string pre = res.substr(res.size() - n + 1, n - 1);
            for (int j = k - 1; j >= 0; --j) {
                string cur = pre + to_string(j);
                if (!visited.count(cur)) {
                    visited.insert(cur);
                    res += to_string(j);
                    break;
                }
            }
        }
        return res;
    }
};
```



来看同一种解法的递归写法，思路和迭代的写法一模一样，写法略有不同而已，参见代码如下：



解法二：

```
class Solution {
public:
    string crackSafe(int n, int k) {
        string res = string(n, '0');
        unordered_set<string> visited{{res}};
        helper(n, k, pow(k, n), visited, res);
        return res;
    }
    void helper(int n, int k, int total, unordered_set<string>& visited, string& res) {
        if (visited.size() == total) return;
        string pre = res.substr(res.size() - n + 1, n - 1);
        for (int i = k - 1; i >= 0; --i) {
            string cur = pre + to_string(i);
            if (visited.count(cur)) continue;
            visited.insert(cur);
            res += to_string(i);
            helper(n, k, total, visited, res);
        }
    }
};
```



参考资料：

[https://leetcode.com/problems/cracking-the-safe/](https://leetcode.com/problems/cracking-the-safe/)

[https://leetcode.com/problems/cracking-the-safe/discuss/110264/Easy-DFS](https://leetcode.com/problems/cracking-the-safe/discuss/110264/Easy-DFS)

[https://leetcode.com/problems/cracking-the-safe/discuss/112966/C++-greedy-loop-from-backward-with-explaination](https://leetcode.com/problems/cracking-the-safe/discuss/112966/C++-greedy-loop-from-backward-with-explaination)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












