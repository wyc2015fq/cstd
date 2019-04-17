# [LeetCode] Palindrome Partitioning 拆分回文串 - Grandyang - 博客园







# [[LeetCode] Palindrome Partitioning 拆分回文串](https://www.cnblogs.com/grandyang/p/4270008.html)







Given a string *s*, partition *s* such that every substring of the partition is a palindrome.

Return all possible palindrome partitioning of *s*.

Example:
Input: "aab"
Output:
[
  ["aa","b"],
  ["a","a","b"]
]


这又是一道需要用DFS来解的题目，既然题目要求找到所有可能拆分成回文数的情况，那么肯定是所有的情况都要遍历到，对于每一个子字符串都要分别判断一次是不是回文数，那么肯定有一个判断回文数的子函数，还需要一个DFS函数用来递归，再加上原本的这个函数，总共需要三个函数来求解。我们将已经检测好的回文子串放到字符串数组out中，当s遍历完了之后，将out加入结果res中。那么在递归函数中我们必须要知道当前遍历到的位置，用变量start来表示，所以在递归函数中，如果start等于字符串s的长度，说明已经遍历完成，将out加入结果res中，并返回。否则就从start处开始遍历，由于不知道该如何切割，所以我们要遍历所有的切割情况，即一个字符，两个字符，三个字符，等等。。首先判断取出的子串是否是回文串，调用一个判定回文串的子函数即可，这个子函数传入了子串的起始和终止的范围，若子串是回文串，那么我们将其加入out，并且调用递归函数，此时start传入 i+1，之后还要恢复out的状态。

那么，对原字符串的所有子字符串的访问顺序是什么呢，如果原字符串是 abcd, 那么访问顺序为: a -> b -> c -> d -> cd -> bc -> bcd-> ab -> abc -> abcd, 这是对于没有两个或两个以上子回文串的情况。那么假如原字符串是 aabc，那么访问顺序为：a -> a -> b -> c -> bc -> ab -> abc -> aa -> b -> c -> bc -> aab -> aabc，中间当检测到aa时候，发现是回文串，那么对于剩下的bc当做一个新串来检测，于是有 b -> c -> bc，这样扫描了所有情况，即可得出最终答案，代码如下：



解法一：

```
class Solution {
public:
    vector<vector<string>> partition(string s) {
        vector<vector<string>> res;
        vector<string> out;
        helper(s, 0, out, res);
        return res;
    }
    void helper(string s, int start, vector<string>& out, vector<vector<string>>& res) {
        if (start == s.size()) { res.push_back(out); return; }
        for (int i = start; i < s.size(); ++i) {
            if (!isPalindrome(s, start, i)) continue;
            out.push_back(s.substr(start, i - start + 1));
            helper(s, i + 1, out, res);
            out.pop_back();
        }
    }
    bool isPalindrome(string s, int start, int end) {
        while (start < end) {
            if (s[start] != s[end]) return false;
            ++start; --end;
        }
        return true;
    }
};
```



我们也可以不单独写递归函数，而是使用原函数本身来递归。首先判空，若字符串s为空，则返回一个包有空字符串数组的数组，注意这里不能直接返回一个空数组，后面会解释原因。然后我们从0开始遍历字符串s，因为是使用原函数当递归，所以无法传入起始位置start，所以只能从默认位置0开始，但是我们的输入字符串s是可以用子串来代替的，这样就相当于起始位置start的作用。首先我们还是判断子串是否为回文串，这里的判断子串还是得用一个子函数，由于起点一直是0，所以只需要传一个终点位置即可。如果子串是回文串，则对后面的整个部分调用递归函数，这样我们会得到一个二维数组，是当前子串之后的整个部分拆分为的回文串的所有情况，那么我们只需将当前的回文子串加入到返回的这些所有情况的集合中。现在解释下之前说的为啥当字符串s为空的时候，要返回一个带有空数组的数组，这是因为当子串就是原字符串s的时候，而是还是个回文串，那么后面部分就为空了，若我们对空串调用递归返回的是一个空数组，那么就无法对其进行遍历，则当前的回文串就无法加入到结果res之中，参见代码如下：



解法二：

```
class Solution {
public:
    vector<vector<string>> partition(string s) {
        vector<vector<string>> res;
        if (s.empty()) return {{}};
        for (int i = 0; i < s.size(); ++i) {
            if (!isPalindrome(s, i + 1)) continue;
            for (auto list : partition(s.substr(i + 1))) {
                list.insert(list.begin(), s.substr(0, i + 1));
                res.push_back(list);
            }
        }
        return res;
    }
    bool isPalindrome(string s, int n) {
        for (int i = 0; i < n / 2; ++i) {
            if (s[i] != s[n - 1 - i]) return false;
        }
        return true;
    }
};
```



下面这种解法是基于解法一的优化，我们可以先建立好字符串s的子串回文的dp数组，光这一部分就可以另出一个道题了 [Palindromic Substrings](http://www.cnblogs.com/grandyang/p/7404777.html)，当我们建立好这样一个二维数组dp，其中 dp[i][j] 表示 [i, j] 范围内的子串是否为回文串，这样就不需要另外的子函数去判断子串是否为回文串了，大大的提高了计算的效率，岂不美哉？！递归函数的写法跟解法一中的没啥区别，可以看之前的讲解，参见代码如下：



解法三：

```
class Solution {
public:
    vector<vector<string>> partition(string s) {
        int n = s.size();
        vector<vector<string>> res;
        vector<string> out;
        vector<vector<bool>> dp(n, vector<bool>(n));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j <= i; ++j) {
                if (s[i] == s[j] && (i - j <= 2 || dp[j + 1][i - 1])) {
                    dp[j][i] = true;
                }
            }
        }
        helper(s, 0, dp, out, res);
        return res;
    }
    void helper(string s, int start, vector<vector<bool>>& dp, vector<string>& out, vector<vector<string>>& res) {
        if (start == s.size()) { res.push_back(out); return; }
        for (int i = start; i < s.size(); ++i) {
            if (!dp[start][i]) continue;
            out.push_back(s.substr(start, i - start + 1));
            helper(s, i + 1, dp, out, res);
            out.pop_back();
        }
    }
};
```



再来看一种迭代的解法，这里还是像上个解法一样建立判断字符串s的子串是否为回文串的dp数组，但建立了一个三维数组的res，这里的res数组其实也可以看作是一个dp数组，其中 res[i] 表示前i个字符组成的子串，即范围 [0, i+1] 内的子串的所有拆分方法，那么最终只要返回 res[n] 极为所求。然后进行for循环，i 从 0 到 n，j 从 0 到 i，这里我们同时更新了两个dp数组，一个是回文串的dp数组，另一个就是结果res数组了，对于区间 [j, i] 的子串，若其是回文串，则 dp[j][i] 更新为 true，并且遍历 res[j] 中的每一种组合，将当前子串加入，并且存入到 res[i+1] 中，参见代码如下：



解法四：

```
class Solution {
public:
    vector<vector<string>> partition(string s) {
        int n = s.size();
        vector<vector<vector<string>>> res(n + 1);
        res[0].push_back({});
        vector<vector<bool>> dp(n, vector<bool>(n));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j <= i; ++j) {
                if (s[i] == s[j] && (i - j <= 2 || dp[j + 1][i - 1])) {
                    dp[j][i] = true;
                    string cur = s.substr(j, i - j + 1);
                    for (auto list : res[j]) {
                        list.push_back(cur);
                        res[i + 1].push_back(list);
                    }
                }
            }
        }
        return res[n];
    }
};
```



类似题目：

[Palindrome Partitioning II](http://www.cnblogs.com/grandyang/p/4271456.html)

[Palindromic Substrings](http://www.cnblogs.com/grandyang/p/7404777.html)



参考资料：

[https://leetcode.com/problems/palindrome-partitioning/](https://leetcode.com/problems/palindrome-partitioning/)

[https://leetcode.com/problems/palindrome-partitioning/discuss/41982/Java-DP-%2B-DFS-solution](https://leetcode.com/problems/palindrome-partitioning/discuss/41982/Java-DP-%2B-DFS-solution)

[https://leetcode.com/problems/palindrome-partitioning/discuss/41963/Java%3A-Backtracking-solution.](https://leetcode.com/problems/palindrome-partitioning/discuss/41963/Java%3A-Backtracking-solution.)

[https://leetcode.com/problems/palindrome-partitioning/discuss/41974/My-Java-DP-only-solution-without-recursion.-O(n2)](https://leetcode.com/problems/palindrome-partitioning/discuss/41974/My-Java-DP-only-solution-without-recursion.-O(n2))

[https://leetcode.com/problems/palindrome-partitioning/discuss/42103/Simple-backtracking-Java-solution-with-95-performance](https://leetcode.com/problems/palindrome-partitioning/discuss/42103/Simple-backtracking-Java-solution-with-95-performance)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












