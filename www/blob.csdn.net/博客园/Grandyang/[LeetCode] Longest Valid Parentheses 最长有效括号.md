# [LeetCode] Longest Valid Parentheses 最长有效括号 - Grandyang - 博客园







# [[LeetCode] Longest Valid Parentheses 最长有效括号](https://www.cnblogs.com/grandyang/p/4424731.html)







Given a string containing just the characters `'('` and `')'`, find the length of the longest valid (well-formed) parentheses substring.

Example 1:
Input: "(()"
Output: 2
Explanation: The longest valid parentheses substring is `"()"`
Example 2:
Input: "`)()())`"
Output: 4
Explanation: The longest valid parentheses substring is `"()()"`


这道求最长有效括号比之前那道 [Valid Parentheses](http://www.cnblogs.com/grandyang/p/4424587.html) 难度要大一些，这里我们还是借助栈来求解，需要定义个start变量来记录合法括号串的起始位置，我们遍历字符串，如果遇到左括号，则将当前下标压入栈，如果遇到右括号，如果当前栈为空，则将下一个坐标位置记录到start，如果栈不为空，则将栈顶元素取出，此时若栈为空，则更新结果和i - start + 1中的较大值，否则更新结果和i - 栈顶元素中的较大值，代码如下：



```
class Solution {
public:
    int longestValidParentheses(string s) {
        int res = 0, start = 0;
        stack<int> m;
        for (int i = 0; i < s.size(); ++i) {
            if (s[i] == '(') m.push(i);
            else if (s[i] == ')') {
                if (m.empty()) start = i + 1;
                else {
                    m.pop();
                    res = m.empty() ? max(res, i - start + 1) : max(res, i - m.top());
                }
            }
        }
        return res;
    }
};
```



还有一种利用动态规划Dynamic Programming的解法，可参见网友[喜刷刷的博客](http://bangbingsyb.blogspot.com/2014/11/leetcode-longest-valid-parentheses.html)。



类似题目：

[Remove Invalid Parentheses](http://www.cnblogs.com/grandyang/p/4944875.html)

[Different Ways to Add Parentheses](http://www.cnblogs.com/grandyang/p/4682458.html)

[Generate Parentheses](http://www.cnblogs.com/grandyang/p/4444160.html)

[Valid Parentheses](http://www.cnblogs.com/grandyang/p/4424587.html)



参考资料：

[https://leetcode.com/problems/longest-valid-parentheses/](https://leetcode.com/problems/longest-valid-parentheses/)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












