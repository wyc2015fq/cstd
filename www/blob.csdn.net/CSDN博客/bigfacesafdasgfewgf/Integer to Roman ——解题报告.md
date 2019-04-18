# Integer to Roman ——解题报告 - bigfacesafdasgfewgf - CSDN博客





2015年05月06日 19:30:16[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：468











    【题目】

    Given an integer, convert it to a roman numeral.


Input is guaranteed to be within the range from 1 to 3999.




    分析：

    首先，我们需要知道罗马数字的表示方法，可参考链接：http://blog.csdn.net/ljiabin/article/details/39968583

    然后，根据罗马数字的表示方法**，我们可以把输入的num分成每一位，对应位转换成罗马数字即可。**

    具体实现的时候，我们可以**把用到的罗马数字单位做成一个table**，查找即可。




    代码：



```cpp
class Solution {
public:
    string intToRoman(int num) {
        string table[4][10] = {
            {"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"},
            {"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"},
            {"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"}, 
            {"", "M", "MM", "MMM"}
        };
        
        string res = "";
        int digit = 0;
        while(num)
        {
            res = table[digit][num%10] + res;  // 查表，需要指明行列
            digit++;  // 下一位
            num /= 10;
        }
        return res;
    }
};
```





