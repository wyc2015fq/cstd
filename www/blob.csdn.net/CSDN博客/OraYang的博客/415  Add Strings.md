
# 415. Add Strings - OraYang的博客 - CSDN博客

2017年08月13日 13:44:57[OraYang](https://me.csdn.net/u010665216)阅读数：117标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given two non-negative integers`num1`and`num2`represented
 as string, return the sum of`num1`and`num2`.
Note:
The length of bothnum1andnum2is < 5100.
Bothnum1andnum2contains only digits0-9.
Bothnum1andnum2does not contain any leading zero.
Youmust not use any built-in BigInteger libraryorconvert the inputs to integerdirectly.
思路：
本题本质上十进制加法运算，满十进1，题目无非将数值转换成字符串，可以各位减‘0‘求得大小
代码：

```python
class Solution {
public:
    string addStrings(string num1, string num2) {
        int i =num1.size()-1,j=num2.size()-1;
        int carry = 0;
        string res ="";
        while(i>=0||j>=0||carry)
        {
            int sum = 0;
            sum +=carry;
            if(i>=0)
            {
                sum += (num1[i] - '0');
                i--;
            }
            if(j>=0)
            {
                sum+=(num2[j] - '0');
                j--;
            }           
            carry = sum / 10;
            sum = sum % 10;
            res += to_string(sum);                  
        }
        reverse(res.begin(),res.end());
        return res;
    }
};
```




