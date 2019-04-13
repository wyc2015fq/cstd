
# 633. Sum of Square Numbers - OraYang的博客 - CSDN博客

2017年08月30日 08:37:05[OraYang](https://me.csdn.net/u010665216)阅读数：173所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a non-negative integer`c`, your task is to decide whether there're two integers`a`and`b`such
 that a2+ b2= c.
Example 1:
Input:5Output:TrueExplanation:1 * 1 + 2 * 2 = 5
Example 2:
Input:3Output:False思路：本题思路是真的简单，只需要循环遍历0-sqrt(c)的整数，判断是否存在符合要求的两个整数。
代码：

```python
class Solution {
public:
    bool judgeSquareSum(int c) {
        for(int a=0;a<=sqrt(c);a++)
        {
            if(sqrt(c-a*a)-int(sqrt(c-a*a))==0.0)
                return true;
        }
        return false;
    }
};
```


