# leetcode-10:Regular Expression Matching正则表达式匹配 - 别说话写代码的博客 - CSDN博客





2018年12月18日 10:06:26[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：31标签：[正则表达式匹配																[leetcode 10																[Regular Expression Matching](https://so.csdn.net/so/search/s.do?q=Regular Expression Matching&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=leetcode 10&t=blog)




题目：
|Given an input string (`s`) and a pattern (`p`), implement regular expression matching with support for `'.'` and `'*'`.'.' Matches any single character.'*' Matches zero or more of the preceding element.The matching should cover the **entire** input string (not partial).**Note:**- `s` could be empty and contains only lowercase letters `a-z`.- `p` could be empty and contains only lowercase letters `a-z`, and characters like `.` or `*`.**Example 1:****Input:**s = "aa"p = "a"**Output:** false**Explanation:** "a" does not match the entire string "aa".**Example 2:****Input:**s = "aa"p = "a*"**Output:** true**Explanation:** '*' means zero or more of the precedeng element, 'a'. Therefore, by repeating 'a' once, it becomes "aa".**Example 3:****Input:**s = "ab"p = ".*"**Output:** true**Explanation:** ".*" means "zero or more (*) of any character (.)".**Example 4:****Input:**s = "aab"p = "c*a*b"**Output:** true**Explanation:** c can be repeated 0 times, a can be repeated 1 time. Therefore it matches "aab".**Example 5:****Input:**s = "mississippi"p = "mis*is*p*."**Output:** false|给定一个字符串 (`s`) 和一个字符模式 (`p`)。实现支持 `'.'` 和 `'*'` 的正则表达式匹配。'.' 匹配任意单个字符。'*' 匹配零个或多个前面的元素。匹配应该覆盖**整个**字符串 (`s`) ，而不是部分字符串。**说明:**- `s` 可能为空，且只包含从 `a-z` 的小写字母。- `p` 可能为空，且只包含从 `a-z` 的小写字母，以及字符 `.` 和 `*`。**示例 1:****输入:**s = "aa"p = "a"**输出:** false**解释:** "a" 无法匹配 "aa" 整个字符串。**示例 2:****输入:**s = "aa"p = "a*"**输出:** true**解释:** '*' 代表可匹配零个或多个前面的元素, 即可以匹配 'a' 。因此, 重复 'a' 一次, 字符串可变为 "aa"。**示例 3:****输入:**s = "ab"p = ".*"**输出:** true**解释:** ".*" 表示可匹配零个或多个('*')任意字符('.')。**示例 4:****输入:**s = "aab"p = "c*a*b"**输出:** true**解释:** 'c' 可以不被重复, 'a' 可以被重复一次。因此可以匹配字符串 "aab"。**示例 5:****输入:**s = "mississippi"p = "mis*is*p*."**输出:** false|
|----|----|

思路：用递归思路，但是如果写  这种递归，时间不能通过。分为3种情况，1.当模板p为空时，若s也空返回true,否则flase,2.当p的串大于1且p[1]=='*"时， 需要匹配'*',方法有当前s匹配p[2],或者当前元素相等匹配s[1]和p，3.在以上条件不满足，也就是要么p==1或p[1]!='*'，则只需要匹配当前的s和p

```cpp
class Solution {
public:
    bool isMatch(string s, string p) {
        if (p.empty()) return s.empty();
        if (p.size() > 1 && p[1] == '*') 
            return isMatch(s,p.substr(2)) || (!s.empty() && (s[0]==p[0] || p[0]=='.')
                && isMatch(s.substr(1),p));
        else return !s.empty() && (s[0]==p[0] || p[0]=='.') &&  
            isMatch(s.substr(1),p.substr(1));
    }
};
```](https://so.csdn.net/so/search/s.do?q=正则表达式匹配&t=blog)




