# leetcode-20：Valid Parentheses有效的括号（括号匹配） - 别说话写代码的博客 - CSDN博客





2018年12月20日 11:02:54[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：38
所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)









题目：
|Given a string containing just the characters `'('`, `')'`, `'{'`, `'}'`, `'['` and `']'`, determine if the input string is valid.An input string is valid if:- Open brackets must be closed by the same type of brackets.- Open brackets must be closed in the correct order.Note that an empty string is also considered valid.**Example 1:****Input:** "()"**Output:** true**Example 2:****Input:** "()[]{}"**Output:** true**Example 3:****Input:** "(]"**Output:** false**Example 4:****Input:** "([)]"**Output:** false**Example 5:****Input:** "{[]}"**Output:** true|给定一个只包括 `'('`，`')'`，`'{'`，`'}'`，`'['`，`']'` 的字符串，判断字符串是否有效。有效字符串需满足：- 左括号必须用相同类型的右括号闭合。- 左括号必须以正确的顺序闭合。注意空字符串可被认为是有效字符串。**示例 1:****输入:** "()"**输出:** true**示例 2:****输入:** "()[]{}"**输出:** true**示例 3:****输入:** "(]"**输出:** false**示例 4:****输入:** "([)]"**输出:** false**示例 5:****输入:** "{[]}"**输出:** true|
|----|----|

 思路：借用栈。若为({[则入栈，若当前元素为})],看当前栈是否为空并与对应括号匹配， 不匹配返回false，匹配就出栈。知道最终栈为空。

```cpp
class Solution {
public:
    bool isValid(string s) {
        stack<char> stk;
        for(int i=0;i<s.size();++i)
        {
            if(s[i]=='('||s[i]=='{'||s[i]=='[') stk.push(s[i]);
            else
            {
                if(stk.empty() ||(s[i]==')'&&stk.top()!='(') ||
                  (s[i]=='}'&&stk.top()!='{') || (s[i]==']'&&stk.top()!='[') )
                    return false;
                stk.pop();
            }
        }
        return stk.empty();
    }
};
```





