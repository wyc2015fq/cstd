# 剑指offer 面试题：字符串转整数 c++ - 别说话写代码的博客 - CSDN博客





2018年11月27日 09:47:49[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：23
所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)









题目：将一个字符串转换成一个整数(实现Integer.valueOf(string)的功能，但是string不符合数字要求时返回0)，要求不能使用字符串转换整数的库函数。 数值为0或者字符串不是一个合法的数值则返回0。

思路：如果要写bugfree，就要判断空指针，空字符串，正负号，溢出等。这里在线编程只需要通过编译即可

```cpp
class Solution {
public:
    int StrToInt(string str) {
        int k=0,carry=1;
        int len=str.size();
        for(int i=len-1;i>=0;i--)
        {
            if(str[i]<='9'&&str[i]>='0')
            {
                k+=(str[i]-'0')*carry;
                carry*=10;
            }
            else if(i==0&&str[i]=='+')continue;
            else if(i==0&&str[i]=='-')k=-k;
            else return 0;
        }
        return k;
    }
};
```





