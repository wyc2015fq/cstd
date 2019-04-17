# lintcode - 有效的括号序列 - guoziqing506的博客 - CSDN博客





2016年09月09日 15:24:46[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：1748
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：给定一个字符串所表示的括号序列，包含以下字符： '(', ')', '{', '}', '[' and ']'， 判定是否是有效的括号序列。

样例：括号必须依照 "()" 顺序表示， "()[]{}" 是有效的括号，但 "([)]"则是无效的括号。




其实题目的意思没有表达的很清楚，所谓“有效的”括号序列，指的是“非交错”的括号序列，也就是说，只有两种可能性是合法的：

1. 对应的括号之间互不相交，且相邻二者一定能对应上，比如“()[]{}”

2. 有相交，但是是很标准的嵌套，比如“([])”这种情况




所以，此题可以用哈希表以及栈解决。具体思路如下：

1. 先建立一个哈希表，表中的键为所有的左括号，这些键所对应的值为左括号对应的右括号，也就是这样：



```python
table = {"(": ")", "[": "]", "{": "}"}
```



2. 建立空栈，按字符串顺序，依次遍历括号字符。倘若此时栈非空，且进栈的元素是当前栈顶元素的在哈希表中的值，则将栈顶元素清除。否则将该元素插入栈，作为新的栈顶元素。这样，只需要判断最后栈为空与否就知此字符串是否是合法的括号序列。




代码如下：



```python
class Solution:
    # @param {string} s A string
    # @return {boolean} whether the string is a valid parentheses
    def isValidParentheses(self, s):
        table = {"(": ")", "[": "]", "{": "}"}
        n = len(s)
        stack = []
        for i in s:
            if len(stack) > 0 and stack[-1] in table and table[stack[-1]] == i:
                stack.pop()
            else:
                stack.append(i)
        return len(stack) == 0
        # Write your code here
```







