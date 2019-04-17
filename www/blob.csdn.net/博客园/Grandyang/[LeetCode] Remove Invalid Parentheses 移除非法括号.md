# [LeetCode] Remove Invalid Parentheses 移除非法括号 - Grandyang - 博客园







# [[LeetCode] Remove Invalid Parentheses 移除非法括号](https://www.cnblogs.com/grandyang/p/4944875.html)







Remove the minimum number of invalid parentheses in order to make the input string valid. Return all possible results.

Note: The input string may contain letters other than the parentheses `(` and `)`.

Example 1:
Input: "()())()"
Output: ["()()()", "(())()"]

Example 2:
Input: "(a)())()"
Output: ["(a)()()", "(a())()"]

Example 3:
Input: ")("
Output: [""]
**Credits:**
Special thanks to [@hpplayer](https://leetcode.com/discuss/user/hpplayer) for adding this problem and creating all test cases.


[Subscribe](https://leetcode.com/subscribe/) to see which companies asked this question




这道题让我们移除最少的括号使得给定字符串为一个合法的含有括号的字符串，我们从小数学里就有括号，所以应该对合法的含有括号的字符串并不陌生，字符串中的左右括号数应该相同，而且每个右括号左边一定有其对应的左括号，而且题目中给的例子也说明了去除方法不唯一，我们需要找出所有合法的取法。参考了网上大神的解法，这道题首先可以用BFS来解，我们先把给定字符串排入队中，然后取出检测其是否合法，若合法直接返回，不合法的话，我们对其进行遍历，对于遇到的左右括号的字符，我们去掉括号字符生成一个新的字符串，如果这个字符串之前没有遇到过，将其排入队中，我们用哈希集合记录一个字符串是否出现过。我们对队列中的每个元素都进行相同的操作，直到队列为空还没找到合法的字符串的话，那就返回空集，参见代码如下：




解法一：

```
class Solution {
public:
    vector<string> removeInvalidParentheses(string s) {
        vector<string> res;
        unordered_set<string> visited{{s}};
        queue<string> q{{s}};
        bool found = false;
        while (!q.empty()) {
            string t = q.front(); q.pop();
            if (isValid(t)) {
                res.push_back(t);
                found = true;
            }
            if (found) continue;
            for (int i = 0; i < t.size(); ++i) {
                if (t[i] != '(' && t[i] != ')') continue;
                string str = t.substr(0, i) + t.substr(i + 1);
                if (!visited.count(str)) {
                    q.push(str);
                    visited.insert(str);
                }
            }
        }
        return res;
    }
    bool isValid(string t) {
        int cnt = 0;
        for (int i = 0; i < t.size(); ++i) {
            if (t[i] == '(') ++cnt;
            else if (t[i] == ')' && --cnt < 0) return false;
        }
        return cnt == 0;
    }
};
```



下面来看一种递归解法，这种解法首先统计了多余的半括号的数量，用cnt1表示多余的左括号，cnt2表示多余的右括号，因为给定字符串左右括号要么一样多，要么左括号多，要么右括号多，也可能左右括号都多，比如")("。所以cnt1和cnt2要么都为0，要么都大于0，要么一个为0，另一个大于0。好，下面进入我们的递归函数，首先判断，如果当cnt1和cnt2都为0时，说明此时左右括号个数相等了，我们调用isValid子函数来判断是否正确，正确的话加入结果res中并返回即可。否则从start开始遍历，这里的变量start表示当前递归开始的位置，我们不需要每次都从头开始，会有大量重复计算。而且对于多个相同的半括号在一起，我们只删除第一个，比如"())"，这里有两个右括号，我们不管删第一个还是删第二个右括号都会得到"()"，没有区别，所以只用算一次就行了，我们通过和上一个字符比较，如果不相同，说明是第一个右括号，如果相同则直接跳过。此时来看如果cnt1大于0，说明此时左括号多，而如果当前字符正好是左括号的时候，我们可以删掉当前左括号，继续调用递归，此时cnt1的值就应该减1，因为已经删掉了一个左括号。同理，如果cnt2大于0，说明此时右括号多，而如果当前字符正好是右括号的时候，我们可以删掉当前右括号，继续调用递归，此时cnt2的值就应该减1，因为已经删掉了一个右括号。参见代码如下：



解法二：

```
class Solution {
public:
    vector<string> removeInvalidParentheses(string s) {
        vector<string> res;
        int cnt1 = 0, cnt2 = 0;
        for (char c : s) {
            cnt1 += (c == '(');
            if (cnt1 == 0) cnt2 += (c == ')');
            else cnt1 -= (c == ')');
        }
        helper(s, 0, cnt1, cnt2, res);
        return res;
    }
    void helper(string s, int start, int cnt1, int cnt2, vector<string>& res) {
        if (cnt1 == 0 && cnt2 == 0) {
            if (isValid(s)) res.push_back(s);
            return;
        }
        for (int i = start; i < s.size(); ++i) {
            if (i != start && s[i] == s[i - 1]) continue;
            if (cnt1 > 0 && s[i] == '(') {
                helper(s.substr(0, i) + s.substr(i + 1), i, cnt1 - 1, cnt2, res);
            }
            if (cnt2 > 0 && s[i] == ')') {
                helper(s.substr(0, i) + s.substr(i + 1), i, cnt1, cnt2 - 1, res);
            }
        }
    }
    bool isValid(string t) {
        int cnt = 0;
        for (int i = 0; i < t.size(); ++i) {
            if (t[i] == '(') ++cnt;
            else if (t[i] == ')' && --cnt < 0) return false;
        }
        return cnt == 0;
    }
};
```



下面这种解法是论坛上的高票解法，思路确实很巧妙。递归函数的参数中，last_i表示当前遍历到的位置，相当上面解法中的start，last_j表示上一个删除的位置，这样可以避免重复计算。然后有个括号字符数组，初始化时放入左括号和右括号，博主认为这个字符数组是此解法最精髓的地方，因为其顺序可以改变，可以变成反向括号，这个就比较叼了，后面再讲它到底有多叼吧。我们在递归函数中，从last_i开始遍历，在找正向括号的时候，用变量cnt表示括号数组中的左括号出现的次数，遇到左括号自增1，遇到右括号自减1。当左括号大于等于右括号的时候，我们直接跳过。这个循环的目的是要删除多余的右括号，所以当cnt小于0的时候，我们从上一个删除位置last_j开始遍历，如果当前是右括号，且是第一个右括号（关于这块可以参见上面解法中的分析），我们删除当前右括号，并调用递归函数。注意这个for循环结束后要直接返回，因为进这个for循环的都是右括号多的，删到最后最多是删成和左括号一样多，不需要再去翻转删左括号。好，最后来说这个最叼的翻转，当字符串的左括号个数大于等于右括号的时候，不会进入第二个for循环，自然也不会return。那么由于左括号的个数可能会要大于右括号，所以我们还要删除多余的左括号，所以我们将字符串反转一下，比如"(()"，反转变成")(("，此时虽然我们还是要删除多余的左括号，但是反转后就没有合法的括号了，所以变成了找反向括号")("，那么还是可以删除多余的左括号，然后我们判断此时括号数组的状态，如果是正向括号，说明此时正要删除左括号，那么就调用递归函数，last_i和last_j均重置为0，括号数组初始化为反向括号。如果此时已经是反向括号了，说明之前的左括号已经删掉了变成了")("，然后又反转了一下，变回来了"()"，那么就可以直接加入结果res了，参见代码如下：



解法三：

```
class Solution {
public:
    vector<string> removeInvalidParentheses(string s) {
        vector<string> res;
        helper(s, 0, 0, {'(', ')'}, res);
        return res;
    }
    void helper(string s, int last_i, int last_j, vector<char> p, vector<string>& res) {
        int cnt = 0;
        for (int i = last_i; i < s.size(); ++i) {
            if (s[i] == p[0]) ++cnt;
            else if (s[i] == p[1]) --cnt;
            if (cnt >= 0) continue;
            for (int j = last_j; j <= i; ++j) {
                if (s[j] == p[1] && (j == last_j || s[j] != s[j - 1])) {
                    helper(s.substr(0, j) + s.substr(j + 1), i, j, p, res);
                }
            }
            return;
        }
        string rev = string(s.rbegin(), s.rend());
        if (p[0] == '(') helper(rev, 0, 0, {')', '('}, res);
        else res.push_back(rev);
    }
};
```



类似题目：

[Different Ways to Add Parentheses](http://www.cnblogs.com/grandyang/p/4682458.html)

[Longest Valid Parentheses](http://www.cnblogs.com/grandyang/p/4424731.html)

[Generate Parentheses](http://www.cnblogs.com/grandyang/p/4444160.html)

[Valid Parentheses](http://www.cnblogs.com/grandyang/p/4424587.html)



参考资料：

[https://leetcode.com/problems/remove-invalid-parentheses/](https://leetcode.com/problems/remove-invalid-parentheses/)

[https://leetcode.com/problems/remove-invalid-parentheses/discuss/75032/share-my-java-bfs-solution](https://leetcode.com/problems/remove-invalid-parentheses/discuss/75032/share-my-java-bfs-solution)

[https://leetcode.com/problems/remove-invalid-parentheses/discuss/75027/easy-short-concise-and-fast-java-dfs-3-ms-solution](https://leetcode.com/problems/remove-invalid-parentheses/discuss/75027/easy-short-concise-and-fast-java-dfs-3-ms-solution)

[https://leetcode.com/problems/remove-invalid-parentheses/discuss/75046/c-depth-limited-dfs-3ms-eliminate-duplicates-without-hashmap](https://leetcode.com/problems/remove-invalid-parentheses/discuss/75046/c-depth-limited-dfs-3ms-eliminate-duplicates-without-hashmap)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













