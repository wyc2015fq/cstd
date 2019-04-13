
# 344. Reverse String - OraYang的博客 - CSDN博客

2017年08月06日 09:32:21[OraYang](https://me.csdn.net/u010665216)阅读数：127标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Write a function that takes a string as input and returns the string reversed.
Example:
Given s = "hello", return "olleh".
思路：
本题使用C++ 标准库函数 reverse即可，详情请戳：[点击打开链接](http://www.cplusplus.com/reference/algorithm/reverse/)
代码：

```python
class Solution {
public:
    string reverseString(string s) {
        reverse(s.begin(),s.end());
        return s;
        
    }
};
```


