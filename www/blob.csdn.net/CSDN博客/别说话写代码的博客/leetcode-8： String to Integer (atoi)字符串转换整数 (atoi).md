# leetcode-8： String to Integer (atoi)字符串转换整数 (atoi) - 别说话写代码的博客 - CSDN博客





2018年12月14日 14:32:57[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：26标签：[leetcode-8																[leetcode atoi																[字符串转换整数																[String to Integer](https://so.csdn.net/so/search/s.do?q=String to Integer&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=字符串转换整数&t=blog)




题目：
|Implement `atoi` which converts a string to an integer.The function first discards as many whitespace characters as necessary until the first non-whitespace character is found. Then, starting from this character, takes an optional initial plus or minus sign followed by as many numerical digits as possible, and interprets them as a numerical value.The string can contain additional characters after those that form the integral number, which are ignored and have no effect on the behavior of this function.If the first sequence of non-whitespace characters in str is not a valid integral number, or if no such sequence exists because either str is empty or it contains only whitespace characters, no conversion is performed.If no valid conversion could be performed, a zero value is returned.**Note:**- Only the space character `' '` is considered as whitespace character.- Assume we are dealing with an environment which could only store integers within the 32-bit signed integer range: [−231,  231 − 1]. If the numerical value is out of the range of representable values, INT_MAX (231 − 1) or INT_MIN (−231) is returned.**Example 1:**```Input: "42"Output: 42```**Example 2:**```Input: "   -42"Output: -42Explanation: The first non-whitespace character is '-', which is the minus sign.             Then take as many numerical digits as possible, which gets 42.```**Example 3:**```Input: "4193 with words"Output: 4193Explanation: Conversion stops at digit '3' as the next character is not a numerical digit.```**Example 4:**```Input: "words and 987"Output: 0Explanation: The first non-whitespace character is 'w', which is not a numerical              digit or a +/- sign. Therefore no valid conversion could be performed.```**Example 5:**```Input: "-91283472332"Output: -2147483648Explanation: The number "-91283472332" is out of the range of a 32-bit signed integer.             Thefore INT_MIN (−231) is returned.```|请你来实现一个 `atoi` 函数，使其能将字符串转换成整数。首先，该函数会根据需要丢弃无用的开头空格字符，直到寻找到第一个非空格的字符为止。当我们寻找到的第一个非空字符为正或者负号时，则将该符号与之后面尽可能多的连续数字组合起来，作为该整数的正负号；假如第一个非空字符是数字，则直接将其与之后连续的数字字符组合起来，形成整数。该字符串除了有效的整数部分之后也可能会存在多余的字符，这些字符可以被忽略，它们对于函数不应该造成影响。注意：假如该字符串中的第一个非空格字符不是一个有效整数字符、字符串为空或字符串仅包含空白字符时，则你的函数不需要进行转换。在任何情况下，若函数不能进行有效的转换时，请返回 0。**说明：**假设我们的环境只能存储 32 位大小的有符号整数，那么其数值范围为 [−231,  231 − 1]。如果数值超过这个范围，qing返回  INT_MAX (231 − 1) 或 INT_MIN (−231) 。**示例 1:**```输入: "42"输出: 42```**示例 2:**```输入: "   -42"输出: -42解释: 第一个非空白字符为 '-', 它是一个负号。     我们尽可能将负号与后面所有连续出现的数字组合起来，最后得到 -42 。```**示例 3:**```输入: "4193 with words"输出: 4193解释: 转换截止于数字 '3' ，因为它的下一个字符不为数字。```**示例 4:**```输入: "words and 987"输出: 0解释: 第一个非空字符是 'w', 但它不是数字或正、负号。     因此无法执行有效的转换。```**示例 5:**```输入: "-91283472332"输出: -2147483648解释: 数字 "-91283472332" 超过 32 位有符号整数范围。      因此返回 INT_MIN (−231) 。```|
|----|----|

思路：从前往后顺序做，需要注意很多点。

1. 若字符串开头是空格，则跳过所有空格，到第一个非空格字符，如果没有，则返回0.

2. 若第一个非空格字符是符号+/-，则标记flag的真假，这道题还有个局限性，那就是在c++里面，+-1和-+1都是认可的，都是-1，而在此题里，则会返回0.

3. 若下一个字符不是数字，则返回0. 完全不考虑小数点和自然数的情况，不过这样也好，起码省事了不少。

4. 如果下一个字符是数字，则转为整形存下来，若接下来再有非数字出现，则返回目前的结果。

5. 还需要考虑边界问题，如果超过了整形数的范围，则用边界值替代当前值。

```cpp
class Solution1 {
public:
    int myAtoi(string str) {
        if(str.empty()) return 0;
        int len = str.length(),flag=1,ret=0,i=0;
        while(i<len || str[i]==' ') ++i;
        if(str[i]=='+' || str[i]=='-')
            flag = (str[i++]=='+')?1:-1;
        while(i<len && str[i]<='9'&&str[i]>='0')
        {
            if(ret>INT_MAX/10 || (ret==INT_MAX/10 && str[i]-'0'>7))
                return (flag==1)?INT_MAX:INT_MIN;
            ret = ret*10 + (str[i++]-'0');
        }
        return flag*ret;
        
    }
};
```

非常好的一道题目！](https://so.csdn.net/so/search/s.do?q=leetcode atoi&t=blog)](https://so.csdn.net/so/search/s.do?q=leetcode-8&t=blog)




