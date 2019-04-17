# 剑指offer 面试题20：表示数值的字符串 - 别说话写代码的博客 - CSDN博客





2018年11月15日 16:42:47[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：19
所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)









请实现一个函数用来判断字符串是否表示数值（包括整数和小数）。例如，字符串"+100","5e2","-123","3.1416"和"-1E-16"都表示数值。 但是"12e","1a3.14","1.2.3","+-5"和"12e+4.3"都不是。

可以用A[.[B]][e|EC]或者.B[e|EC]表示，其中A和C都是整数（可以有正负号也可以没有），B是一个无符号整数

```cpp
class Solution {
public:
    bool scanUnsignedInteger(char ** str) //扫描无符号整数
    {
        char * before =*str;
        while(**str!='\0' && **str>='0'&&**str<='9') ++(*str);
        return *str > before;  //看是否真正有数字
    }
    bool scanInteger(char ** str)   //扫描可能带符号的整数
    {
        if(**str =='+' || **str=='-') ++(*str);   //看是否有正负号
        return scanUnsignedInteger(str);
    }
    bool isNumeric(char* string)
    {
        if(string==nullptr) return false;
        bool num = scanInteger(&string);  //传string的地址,可以记录当前操作到原始数据的哪个位置
        if(*string == '.')  //遇到小数点
        {
            ++string;
            //.123  0.123  223.   223.0   223.5
            //小数点后有数字，小数点前有数字，小数点前后都有数字
            num = scanUnsignedInteger(&string) ||num ;//满足前面是数字小数点或者后面是无符号数字
        }
        if(*string == 'e' ||*string == 'E')   //遇到指数
        {
            ++string;
            //e前面没数字,整个字符不能代表数字,.e1  e1
            //e后面没数字，整个字符不能代表数字，如12e   1e5.2
            num = scanInteger(&string) && num;  //前面满足数字条件并且e后面是有符号数字
        }
        return num && *string=='\0';
    }
};
```





