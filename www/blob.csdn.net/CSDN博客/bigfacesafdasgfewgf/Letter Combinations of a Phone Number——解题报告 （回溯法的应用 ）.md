# Letter Combinations of a Phone Number——解题报告  （回溯法的应用  ） - bigfacesafdasgfewgf - CSDN博客





2015年05月08日 08:33:59[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：1427











    【题目】

Given a digit string, return all possible letter combinations
 that the number could represent.


A mapping of digit to letters (just like on the telephone buttons) is given below.

![](http://upload.wikimedia.org/wikipedia/commons/thumb/7/73/Telephone-keypad2.svg/200px-Telephone-keypad2.svg.png)
Input:Digit string "23"
Output: ["ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"].

Note:

Although the above answer is in lexicographical order, your answer could be in any order you want.




    【分析】

     如果我们使用for循环来遍历的话，也是可以求解的。但是，这道题目在参考了人家的解法之后，觉得使用基于回溯法的递归方式更加巧妙。

    注意：递归思路在编程的时候，一定要先写跳出递归的条件if语句，然后再写其余的部分。而且，回溯法的思想是遍历完一次之后，退一步，继续遍历，这个继续遍历的过程有时调用一次函数的过程，所以是递归方式。




    【代码】

    注意：vector的初始化；还有string类型作为一个容器，也是有push_back和pop_back函数的；回溯法的核心代码local.pop_back()。

    运行时间：2ms



```cpp
class Solution {
public:
    vector<string> letterCombinations(string digits) {
        vector<string> res;
        if(digits.size() == 0)
            return res;
            
        vector<string> phone(2, "");
        phone.push_back("abc");
        phone.push_back("def");
        phone.push_back("ghi");
        phone.push_back("jkl");
        phone.push_back("mno");
        phone.push_back("pqrs");
        phone.push_back("tuv");
        phone.push_back("wxyz");
        
        string local;
        backTracking(res, phone, digits, 0, local);
        
        return res;
    }
    
    void backTracking(vector<string>& res, vector<string>& phone, string& digits, int index, string& local)
    {
        if(index == digits.length())  // 作为每次遍历结束的出口
        {
            res.push_back(local);
            return;
        }
            
        for(int i = 0; i < phone[digits[index] - '0'].length(); i++)  // 遍历每个按键包含的串长
        {
            local += phone[digits[index] - '0'][i];  // 连接当前字符
            backTracking(res, phone, digits, index + 1, local);  // 遍历当前字符后面的情况
            local.pop_back();  // 回溯法的关键：遍历好一次之后，退一步，继续遍历
        }
        // 全是引用类型，无返回值，简洁
    }
};
```







