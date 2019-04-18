# Generate Parentheses——解题报告 - bigfacesafdasgfewgf - CSDN博客





2015年05月09日 09:07:59[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：700标签：[LeetCode																[括号匹配																[递归																[字符串																[遍历](https://so.csdn.net/so/search/s.do?q=遍历&t=blog)
个人分类：[LeetCode](https://blog.csdn.net/puqutogether/article/category/2832605)








    【题目】

    Given n pairs of parentheses, write a function to generate all combinations of well-formed parentheses.


For example, given n = 3, a solution set is:


"((()))", "(()())", "(())()", "()(())", "()()()"







    【分析】

    显然是个遍历的过程，但是如何遍历是个问题。遍历，我们经常可以使用递归的方式解决。递归，套路是先写出跳出条件，然后再写其他递归调用程序部分。**这个题目中跳出条件明显是当左括号（和右括号）都使用完的时候，压入结果，返回。**那么剩下的部分呢？这个时候就要结合具体问题，具体分析了。**括号匹配最核心的是，生成的括号一定要是well-formed。这就需要当前生成的cur字符串中，一定要保证左括号的数量小于右括号数量。所以，如果左括号数量m不等于0，则可以继续放置，而当左括号数量小于右括号数量，并且右括号数量不等于0的时候，才可以放置右括号！**







    【代码】

    运行时间：5ms






```cpp
class Solution {
public:
    vector<string> generateParenthesis(int n) {
        vector<string> res;
        if(n == 0)
            return res;
            
        string cur = "";
        fun(res, cur, n, n);
        
        return res;
    }
    
    void fun(vector<string>& res, string cur, int m, int n)
    {
        if(m == 0 && n == 0)
        {
            res.push_back(cur);
            return;
        }
        
        if(m != 0)
        {
            fun(res, cur + '(', m-1, n);
        }
        if(m < n && n != 0)  // "m < n" ensure the generated parantheses are well-formed
        {
            fun(res, cur + ')', m, n-1);
        }
        
    }
};
```](https://so.csdn.net/so/search/s.do?q=字符串&t=blog)](https://so.csdn.net/so/search/s.do?q=递归&t=blog)](https://so.csdn.net/so/search/s.do?q=括号匹配&t=blog)](https://so.csdn.net/so/search/s.do?q=LeetCode&t=blog)




