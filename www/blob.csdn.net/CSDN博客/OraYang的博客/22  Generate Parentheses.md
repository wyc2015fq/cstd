
# 22. Generate Parentheses - OraYang的博客 - CSDN博客

2017年09月12日 09:30:40[OraYang](https://me.csdn.net/u010665216)阅读数：155个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Given n pairs of parentheses, write a function to generate all combinations of well-formed parentheses.
For example, given n = 3, a solution set is:
[
“((()))”,
“(()())”,
“(())()”,
“()(())”,
“()()()”
]
## 思路
本题是典型的卡特兰数的应用，与之类似的还有出栈次序，详情请戳[卡特兰数](https://baike.baidu.com/item/%E5%8D%A1%E7%89%B9%E5%85%B0%E6%95%B0/6125746?fr=aladdin)。
本题具体的解题思路，是这样的，要求括号的组合数，那么就要找到卡特兰数的性质，总结如下，对于2n(此题测试n=3)个括号，在某个时间点，必定是左括号书大于右括号数，而且初始括号必定是左括号。那么用递归的思想来求解。递归的临界条件是：左括号剩余数与右括号剩余数同时为0。
## 代码
```python
class
```
```python
Solution
```
```python
{
```
```python
public
```
```python
:
```
```python
vector<string> generateParenthesis(int n) {
        string str =
```
```python
"("
```
```python
;
        vector<string> res;
        generate_parentheses(n-
```
```python
1
```
```python
,n,str,res);
```
```python
return
```
```python
res;
    }
    void generate_parentheses(int left,int right,string s,vector<string>& res){
```
```python
if
```
```python
(left==
```
```python
0
```
```python
&&right==
```
```python
0
```
```python
)
            res.push_back(s);
```
```python
if
```
```python
(left>
```
```python
0
```
```python
)
            generate_parentheses(left-
```
```python
1
```
```python
,right,s+
```
```python
'('
```
```python
,res);
```
```python
if
```
```python
(right>
```
```python
0
```
```python
&&right>left)
            generate_parentheses(left,right-
```
```python
1
```
```python
,s+
```
```python
')'
```
```python
,res);
    }
};
```

