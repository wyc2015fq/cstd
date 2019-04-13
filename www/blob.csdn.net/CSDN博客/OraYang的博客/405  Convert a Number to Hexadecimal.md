
# 405. Convert a Number to Hexadecimal - OraYang的博客 - CSDN博客

2017年08月10日 11:18:12[OraYang](https://me.csdn.net/u010665216)阅读数：98标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given an integer, write an algorithm to convert it to hexadecimal. For negative integer,[two’s complement](https://en.wikipedia.org/wiki/Two%27s_complement)method
 is used.
Note:
All letters in hexadecimal (a-f) must be in lowercase.
The hexadecimal string must not contain extra leading0s. If the number is zero, it is represented by a single zero character'0'; otherwise, the first character in the hexadecimal string will not be the zero character.
The given number is guaranteed to fit within the range of a 32-bit signed integer.
Youmust not useanymethod provided by the librarywhich converts/formats the number to hex directly.
Example 1:
Input:
26
Output:
"1a"
Example 2:
Input:
-1
Output:
"ffffffff"思路：本题是进制转换，要注意int 在求负数的时候遇到的溢出问题，所以转换成long型，有些方案是每四位求一次
代码：

```python
class Solution {
public:
    string toHex(int num) {
         long n = num;
        if(n==0)
            return "0";
        string res = ""; 
        vector<char> arrary = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
        if(n<0)
            n = 0xffffffff + n +1;
        while(n)  
        {  
            res = arrary[(n)%16] + res;  
            n = (n)/16;  
        }  
        return res; 
         
    } 
};
```


