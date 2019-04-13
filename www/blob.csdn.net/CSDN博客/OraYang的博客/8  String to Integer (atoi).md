
# 8. String to Integer (atoi) - OraYang的博客 - CSDN博客

2017年09月04日 14:30:43[OraYang](https://me.csdn.net/u010665216)阅读数：136所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Implementatoito convert a string to an integer.
Hint:Carefully consider all possible input cases. If you want a challenge, please do not see below and ask yourself what are the possible input cases.
Notes:It is intended for this problem to be specified vaguely (ie, no given input specs). You are responsible to gather all the input requirements up front.
Update (2015-02-10):
The signature of the`C++`function had been updated. If you still see your function signature
 accepts a`const char *`argument, please click the reload buttonto
 reset your code definition.
思路：本题是字符串转整数，本题要注意的是字符串有正负之分，而且当数值溢出且为正数时，直接输出2147483647，当数值溢出且为负数时，直接输出-2147483648
代码：

```python
class Solution {
public:
    int myAtoi(string str) {
    long s = 0;  
    bool flag = false;  
    string::iterator itr=str.begin();  
    while(*itr==' ')  
    {  
        itr++;  
    }  
  
    if(*itr=='-'||*itr=='+')  
    {  
        if(*itr=='-')  
        flag=true;  
        itr++;  
    }  
  
    while(*itr>='0'&&*itr<='9')  
    {  
        s=s*10+*itr-'0';  
        itr++;  
        if(s>=2147483648&&flag==true)  
        {  
            // cout<<s<<endl;
            s=2147483648;  
            break;  
        }
        else if(s>=2147483647&&flag==false)  
        {  
            // cout<<s<<endl;
            s=2147483647;  
            break;  
        }
    }  
    return (int)s*(flag?-1:1);  
    }
};
```

