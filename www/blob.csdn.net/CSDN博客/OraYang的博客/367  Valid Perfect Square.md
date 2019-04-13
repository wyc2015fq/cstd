
# 367. Valid Perfect Square - OraYang的博客 - CSDN博客

2017年08月07日 10:20:19[OraYang](https://me.csdn.net/u010665216)阅读数：135标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a positive integernum, write a function which returns True ifnumis a perfect square else False.
Note:Do notuse any built-in library function such as`sqrt`.
Example 1:
Input: 16
Returns: True
Example 2:
Input: 14
Returns: False思路：分析规律：
1x1=1      1
2x2=41+3
3x3=91+3+5
4x4=251+3+5+7
nxn =1+3+5+...(2xn-1)
代码：

```python
class Solution {
public:
    bool isPerfectSquare(int num) {
         int i = 1;
     while (num > 0) {
         num -= i;
         i += 2;
     }
     return num == 0;
        
    }
};
```



