# 剑指offer 面试题19：正则表达式匹配 - 别说话写代码的博客 - CSDN博客





2018年11月15日 16:53:42[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：24
所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)









请实现一个函数用来匹配包括'.'和'*'的正则表达式。模式中的字符'.'表示任意一个字符，而'*'表示它前面的字符可以出现任意次（包含0次）。 在本题中，匹配是指字符串的所有字符匹配整个模式。例如，字符串"aaa"与模式"a.a"和"ab*ac*a"匹配，但是与"aa.a"和"ab*a"均不匹配

```cpp
class Solution {
public:
    bool matchcore(char *str,char* pattern)
    {
        if(*str == '\0'&&*pattern=='\0') return true;  //两个串都完
        if(*str != '\0' && *pattern == '\0') return false;   //匹配串完但模式串没完
        
        if(*(pattern+1)=='*')   //先要判断模式串下一个字符是否为*
        {
            //当前两个字符匹配   或   模式串为.*且匹配串未匹配结束   
            if(*str == *pattern ||(*pattern=='.'&&*str!='\0')) 
                return matchcore(str+1,pattern)||  //匹配串与模式串一样，匹配串++
                matchcore(str,pattern+2)||   //匹配串与模式串不一样，模式串+2
                matchcore(str+1,pattern+2);   //匹配完一个   要同时满足以上三种情况
            else return matchcore(str,pattern+2);   
        }
        if(*str == *pattern || (*pattern=='.'&&*str!='\0'))   //模式串下一个不为*，当前两串相等或匹配串为.
            return matchcore(str+1,pattern+1);
        return false;
    }
    bool match(char* str, char* pattern)
    {
        if(str==nullptr || pattern == nullptr) return false;
        return matchcore(str,pattern);
    }
};
```





