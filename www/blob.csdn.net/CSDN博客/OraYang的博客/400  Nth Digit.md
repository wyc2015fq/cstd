
# 400. Nth Digit - OraYang的博客 - CSDN博客

2017年08月09日 09:49:38[OraYang](https://me.csdn.net/u010665216)阅读数：98标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Find thenthdigit of the infinite integer sequence 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, ...
Note:
nis positive and will fit within the range of a 32-bit signed integer (n< 231).
Example 1:
Input:3Output:3
Example 2:
Input:11Output:0Explanation:The 11th digit of the sequence 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, ... is a 0, which is part of the number 10.思路：本题题意比较难以理解，看了官方的solution才发现了题目的含义：寻找n是整数序列中的第几个数字（digit),是数字所以1-10，一共有11个数字，这样理解了题目的目的后，解题思路就有了，参考主流方法，分为三步：
step1:找到n落在哪个位数区间1-9；10-99；100-999；.....
step2:找到n落在哪个数值上
step3:找到n落在哪个位上
代码：

```python
class Solution {
public:
    int findNthDigit(int n) {
        long digit = 1, ith = 1, base = 9;  
        while(n > base*digit)  
        {  
            n -= base*(digit++);  
            ith += base;  
            base *= 10;  
        }  
        return to_string(ith+(n-1)/digit)[(n-1)%digit]-'0';  
        
    }
};
```


