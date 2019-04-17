# leetcode-22.Generate Parentheses 括号生成 - 别说话写代码的博客 - CSDN博客





2018年12月21日 11:04:27[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：13标签：[leetcode																[Generate Parentheses																[括号生成](https://so.csdn.net/so/search/s.do?q=括号生成&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=Generate Parentheses&t=blog)




题目：
|Given *n* pairs of parentheses, write a function to generate all combinations of well-formed parentheses.For example, given *n* = 3, a solution set is:[  "((()))",  "(()())",  "(())()",  "()(())",  "()()()"]|给出 *n* 代表生成括号的对数，请你写出一个函数，使其能够生成所有可能的并且**有效的**括号组合。例如，给出 *n *=3，生成结果为：[  "((()))",  "(()())",  "(())()",  "()(())",  "()()()"]|
|----|----|

思路：DFS. 对这种列出所有结果的题目，都可以考虑用递归来解。这里字符串只有左括号和右括号两种，每种3个，我们令left=3为左括号个数，right=3为右括号个数。挡在某次递归时出现left>right,则直接返回，若出现left==right==0,就说明左右 括号都打印完了。否则，先打印左括号，left--,再打印右括号,right--.

```cpp
class Solution {
public:
    vector<string> generateParenthesis(int n) {
        vector<string> res;
        dfs(n,n,"",res);
        return res;
    }
    void dfs(int left,int right,string out,vector<string> &res)
    {
        if(left > right) return;
        if(left==0 && right == 0) res.push_back(out);
        else
        {
            if(left>0) dfs(left-1,right,out+"(",res);
            if(right>0) dfs(left,right-1,out+")",res);
        }
    }
```](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)




