
# 342. Power of Four - OraYang的博客 - CSDN博客

2017年08月06日 09:21:32[OraYang](https://me.csdn.net/u010665216)阅读数：149标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
given an integer (signed 32 bits), write a function to check whether it is a power of 4.
Example:
Given num = 16, return true. Given num = 5, return false.
Follow up: Could you solve it without loops/recursion?
思路：本题与[power of two](http://blog.csdn.net/u010665216/article/details/76522236),[power of
 three](http://blog.csdn.net/u010665216/article/details/76738039)略有不同，不能简单的套用power of three的方法。可以分析2、4的幂都有哪些特征，可以从2进制角度考虑，
1：1
4：100
8：1000
16：10000
32：100000
64：1000000
总结规律发现满足2或4的幂必须num&(num-1) == 0,其次每4位用0101判断是否为4的幂
代码：

```python
class Solution {
public:
    bool isPowerOfFour(int num) {
        return num > 0 && (num&(num-1)) == 0 && (num & 0x55555555) != 0;
        //0x55555555 is to get rid of those power of 2 but not power of 4
        //so that the single 1 bit always appears at the odd position 
        
    }
};
```



