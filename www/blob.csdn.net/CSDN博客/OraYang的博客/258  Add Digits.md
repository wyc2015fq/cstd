
# 258. Add Digits - OraYang的博客 - CSDN博客

2017年08月03日 09:44:03[OraYang](https://me.csdn.net/u010665216)阅读数：109标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a non-negative integer`num`, repeatedly add all its digits until the result has only
 one digit.
For example:
Given`num = 38`, the process is like:`3
 + 8 = 11`,`1 + 1 = 2`. Since`2`has
 only one digit, return it.
思路：本题思路简单，递归不断求和
代码：

```python
class Solution {
public:
    int addDigits(int num) {
        if(num<10)
            return num;
        else
            return addDigits(sum(num));
        
        
    }
    int sum(int num){
        int sum = 0;
        while(num)
        {
            sum+=num%10;
            num = num/10;
        }
        return sum;
    }
};
```



