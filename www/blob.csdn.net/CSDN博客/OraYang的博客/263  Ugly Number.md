
# 263. Ugly Number - OraYang的博客 - CSDN博客

2017年08月03日 10:09:32[OraYang](https://me.csdn.net/u010665216)阅读数：104标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Write a program to check whether a given number is an ugly number.
Ugly numbers are positive numbers whose prime factors only include`2, 3, 5`. For example,`6,
 8`are ugly while`14`is not ugly since it includes another prime factor`7`.
Note that`1`is typically treated as an ugly number.
思路：本题判断一个数的质数因子是否只有2，3，5；最直接的方式就是不断除以2，3，5，如果最后结果是1则返回true，否则返回false
代码：

```python
class Solution {
public:
    bool isUgly(int num) {
    if(num==1) return true;
    if(num==0) return false;
	while(num%2==0) num=num/2;
	while(num%3==0) num=num/3;
	while(num%5==0) num=num/5;
    return num==1;
        
    }  
};
```


