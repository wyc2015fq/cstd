
# 459. Repeated Substring Pattern - OraYang的博客 - CSDN博客

2017年08月19日 21:50:22[OraYang](https://me.csdn.net/u010665216)阅读数：107标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a non-empty string check if it can be constructed by taking a substring of it and appending multiple copies of the substring together. You may assume the given string consists of lowercase English letters only and its length will not exceed 10000.
Example 1:
Input:"abab"Output:TrueExplanation:It's the substring "ab" twice.
Example 2:
Input:"aba"Output:False
Example 3:
Input:"abcabcabcabc"Output:TrueExplanation:It's the substring "abc" four times. (And the substring "abcabc" twice.)思路：求重复字符串，本题采用遍历比较的思想，重复项小于等于n/2，大于等于1
代码:

```python
class Solution {
public:
    bool repeatedSubstringPattern(string s) {
        int n = s.size();
        for (int i = n / 2; i >= 1; --i) {
            if (n % i == 0) {
                int c = n / i;
                string t = "";
                for (int j = 0; j < c; ++j) {
                    t += s.substr(0, i); 
                }
                if (t == s) return true;
            }
        }
        return false;
    }
};
```



