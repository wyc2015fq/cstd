
# 507. Perfect Number - OraYang的博客 - CSDN博客

2017年08月23日 09:52:46[OraYang](https://me.csdn.net/u010665216)阅读数：349标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
We define the Perfect Number is apositiveinteger that is equal to the sum of all itspositivedivisors except itself.
Now, given anintegern,
 write a function that returns true when it is a perfect number and false when it is not.
Example:
Input:28Output:TrueExplanation:28 = 1 + 2 + 4 + 7 + 14
Note:The input numbernwill not exceed 100,000,000. (1e8)
思路：本题思路比较简单，构造从2开始的循环，循环次数小于等于sqrt(num)，期间求num得约数
代码：

```python
class Solution {
public:
    bool checkPerfectNumber(int num) {
        if (num == 1) return false;
        int sum = 0;
        for (int i = 2; i <= sqrt(num); i++) {
            if (num % i == 0) {
                sum += i + num / i;
            }
        }
        sum++;       
        return sum == num;    
    }
};
```


