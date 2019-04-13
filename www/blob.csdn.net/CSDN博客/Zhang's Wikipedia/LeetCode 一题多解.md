
# LeetCode 一题多解 - Zhang's Wikipedia - CSDN博客


2018年11月05日 21:54:16[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：331



## 1. 括号匹配
[20. Valid Parentheses](https://leetcode.com/problems/valid-parentheses/description/)
堆栈版：
class Solution(object):
    def isValid(self, s):
        """
        :type s: str
        :rtype: bool
        """
        stack = []
        para_map = {')':'(', ']':'[', '}':'{'}
        for c in s:
            if c not in para_map:
                stack.append(c)
            elif not stack or stack.pop() != para_map.get(c):
                return False
        return not stack
字符串处理版：
class Solution(object):
    def isValid(self, s):
        """
        :type s: str
        :rtype: bool
        """
        length = len(s)
        s = s.replace('()', '').replace('[]', '').replace('{}', '')
        while len(s) != length:
            length = len(s)
            s = s.replace('()', '').replace('[]', '').replace('{}', '')
        return length == 0使用 Java 的 do while 语句，形式上将更为简洁：
class Solution {
    public boolean isValid(String s) {
        int length;
        do {
            length = s.length();
            s = s.replace("()", "").replace("[]", "").replace("{}", "");
        } while (s.length() != length);
        return length == 0;
    }
}

