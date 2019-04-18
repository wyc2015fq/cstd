# Valid Parentheses——解题报告 - bigfacesafdasgfewgf - CSDN博客





2015年05月08日 10:22:37[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：546











    【题目】

    Given a string containing just the characters '(', ')', '{', '}', '[' and ']', determine if the input string is valid.


The brackets must close in the correct order, "()" and "()[]{}" are all valid but "(]" and "([)]" are not.




    【分析】

    栈的使用，比较简单。




    【代码】



```cpp
class Solution {
public:
    bool isValid(string s) {
        if(s.length() == 0)
            return true;
        
        stack<int> sta;
        for(int i = 0; i < s.length(); i++)
        {
            if(s[i] == '(' || s[i] == '[' || s[i] == '{')
                sta.push(s[i]);
            else if(s[i] == ')')
            {
                if(sta.empty() || sta.top() != '(')
                    return false;
                else
                    sta.pop();
            }
            else if(s[i] == ']')
            {
                if(sta.empty() || sta.top() != '[')
                    return false;
                else
                    sta.pop();
            }
            else
            {
                if(sta.empty() || sta.top() != '{')                
                    return false;
                else
                    sta.pop();
            }
        }
        
        if(sta.empty())
            return true;
        else
            return false;
    }
};
```





