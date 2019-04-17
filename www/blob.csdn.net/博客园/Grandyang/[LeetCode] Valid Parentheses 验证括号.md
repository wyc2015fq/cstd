# [LeetCode] Valid Parentheses 验证括号 - Grandyang - 博客园







# [[LeetCode] Valid Parentheses 验证括号](https://www.cnblogs.com/grandyang/p/4424587.html)







Given a string containing just the characters `'('`, `')'`, `'{'`, `'}'`, `'['` and `']'`, determine if the input string is valid.

An input string is valid if:
- Open brackets must be closed by the same type of brackets.
- Open brackets must be closed in the correct order.

Note that an empty string is also considered valid.

Example 1:
Input: "()"
Output: true

Example 2:
Input: "()[]{}"
Output: true

Example 3:
Input: "(]"
Output: false

Example 4:
Input: "([)]"
Output: false

Example 5:
Input: "{[]}"
Output: true


这道题让我们验证输入的字符串是否为括号字符串，包括大括号，中括号和小括号。这里我们需要用一个栈，我们开始遍历输入字符串，如果当前字符为左半边括号时，则将其压入栈中，如果遇到右半边括号时，若此时栈为空，则直接返回false，如不为空，则取出栈顶元素，若为对应的左半边括号，则继续循环，反之返回false，代码如下：



```
class Solution {
public:
    bool isValid(string s) {
        stack<char> parentheses;
        for (int i = 0; i < s.size(); ++i) {
            if (s[i] == '(' || s[i] == '[' || s[i] == '{') parentheses.push(s[i]);
            else {
                if (parentheses.empty()) return false;
                if (s[i] == ')' && parentheses.top() != '(') return false;
                if (s[i] == ']' && parentheses.top() != '[') return false;
                if (s[i] == '}' && parentheses.top() != '{') return false;
                parentheses.pop();
            }
        }
        return parentheses.empty();
    }
};
```



类似题目：

[Remove Invalid Parentheses](http://www.cnblogs.com/grandyang/p/4944875.html)

[Different Ways to Add Parentheses](http://www.cnblogs.com/grandyang/p/4682458.html)

[Longest Valid Parentheses](http://www.cnblogs.com/grandyang/p/4424731.html)

[Generate Parentheses](http://www.cnblogs.com/grandyang/p/4444160.html)



参考资料：

[https://leetcode.com/problems/valid-parentheses/](https://leetcode.com/problems/valid-parentheses/)

[https://discuss.leetcode.com/topic/27768/short-easy-to-follow-8ms-java-solution](https://discuss.leetcode.com/topic/27768/short-easy-to-follow-8ms-java-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












