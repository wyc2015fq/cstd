
# 168. Excel Sheet Column Title - OraYang的博客 - CSDN博客

2017年07月26日 10:36:08[OraYang](https://me.csdn.net/u010665216)阅读数：247标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a positive integer, return its corresponding column title as appear in an Excel sheet.
For example:
1 -> A
    2 -> B
    3 -> C
    ...
    26 -> Z
    27 -> AA
    28 -> AB思路：本题首先要了解Excel的标题性质：A~Z, AA~ZZ, AAA~ZZZ, ……；Excel的标题由A-Z字符组成，本质上是26进制的表示，因此本题就是将十进制转换成26进制
代码：

```python
class Solution {
public:
    string convertToTitle(int n) {
        string res = "";
        while(n)
        {
            res = char((n-1)%26+ 'A') + res;
            n = (n-1)/26;
        }
        return res;
    }
};
```


