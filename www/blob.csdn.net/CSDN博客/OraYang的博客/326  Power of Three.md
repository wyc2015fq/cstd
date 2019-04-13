
# 326. Power of Three - OraYang的博客 - CSDN博客

2017年08月06日 08:45:44[OraYang](https://me.csdn.net/u010665216)阅读数：348标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given an integer, write a function to determine if it is a power of three.
Follow up:
Could you do it without using any loop / recursion?
思路：
本题和前面的博客[power
 of two](http://blog.csdn.net/u010665216/article/details/76522236http://blog.csdn.net/u010665216/article/details/76522236)的解题思路一样,但是题目尝试让我们不要使用循环与递归，所以本题给出非循环，非递归的方法。
代码（递归）：

```python
class Solution {
public:
    bool isPowerOfThree(int n) {
        if(n==1)  
           return true;  
        else if(n<1)  
            return false;  
        else  
        {  
            if ((n/3.0) == (n/3))  
                return isPowerOfThree(n/3);  
            else   
                return false;  
        }  
        
    }
};
```
代码（非）：
```python
class Solution {
public:
    bool isPowerOfThree(int n) {
    // 1162261467 is 3^19,  3^20 is bigger than int  
    return ( n>0 &&  1162261467%n==0);
}
};
```



