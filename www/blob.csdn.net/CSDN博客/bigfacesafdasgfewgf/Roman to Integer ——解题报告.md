# Roman to Integer ——解题报告 - bigfacesafdasgfewgf - CSDN博客





2015年05月06日 20:17:46[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：500











    【题目】

    Given a roman numeral, convert it to an integer.


Input is guaranteed to be within the range from 1 to 3999.




    【分析】

    这个和上篇博文中把数字转换为罗马数字正好相反，逻辑过程有点儿复杂。

    其实解法来源于对罗马数字（字符串）的观察，在解析这个字符串的时候，我们可以分成两种情况：

1）如果former大于等于current，那么结果可以直接加上current的数值；

2）如果former较小，那么我们需要用current - former*2. 这里为什么需要减去2倍的former，因为former在前面一定是先加上过一次，例如：XIV, 过程是10+1+5-2*1=14,看到了吧，1在前面一定是已经加上了一次了。




   【代码】






```cpp
class Solution {
public:
    int romanToInt(string s) {
        int res = 0; 
        int len = s.length();
        res += convert(s[0]);
        
        for(int i = 1; i < len; i++)
        {
            int current = convert(s[i]); 
            int former = convert(s[i - 1]); 
            if(former >= current)  // the former one is larger than or equalling to the later one
            {
                res += current;
            }
            else
            {
                res += current - 2*former;
            }
        }
        return res;
    }
    
    int convert(char c)
    {
        switch(c)
        {
            case 'I': return 1;
            case 'V': return 5;
            case 'X': return 10;
            case 'L': return 50;
            case 'C': return 100;
            case 'D': return 500;
            case 'M': return 1000;
        }
        return 0;
        
    }
};
```























