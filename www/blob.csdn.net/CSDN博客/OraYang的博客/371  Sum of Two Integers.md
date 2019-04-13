
# 371. Sum of Two Integers - OraYang的博客 - CSDN博客

2017年08月08日 08:58:01[OraYang](https://me.csdn.net/u010665216)阅读数：128标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Calculate the sum of two integersaandb, but you arenot allowedto use the operator`+`and`-`.
Example:
Givena= 1 andb= 2, return 3.
思路：本题求和，条件是不允许使用操作符‘+’，那么一般来说，倾向的思路是使用位操作，梳理位操作的性质会发现：
1.按位异或为求和
2.按位相与为进位
接下来利用这两个性质来逐步分析。举例：2 + 3 = 5，2：10 ，3：11
step1.按位相或：10^11 = 01（求和项）
step2.按位相与：10 & 11 = 10(进位项）
step3. 进位项左移，并判断是否为0，为0跳出循环
step4.返回step1与求和项做异或操作
代码：

```python
class Solution {
public:
    int getSum(int a, int b) {
        int sum;
        while (b != 0)
        {
            sum = a ^ b;//calculate sum of a and b without thinking the carry 
            b = (a & b) << 1;//calculate the carry
            a = sum;//add sum(without carry) and carry
        }        
        return sum;      
    }
};
```


