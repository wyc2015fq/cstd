
# 231. Power of Two - OraYang的博客 - CSDN博客

2017年08月01日 13:38:22[OraYang](https://me.csdn.net/u010665216)阅读数：143所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given an integer, write a function to determine if it is a power of two.
思路：
本题思路很简单就是递归的调用
代码：

```python
class Solution {
public:
    bool isPowerOfTwo(int n) {
 
        if(n==1)
           return true;
        else if(n<1)
            return false;
        else
        {
            if ((n/2.0) == (n/2))
                return isPowerOfTwo(n/2);
            else 
                return false;
        }
           
        
    }
};
```


