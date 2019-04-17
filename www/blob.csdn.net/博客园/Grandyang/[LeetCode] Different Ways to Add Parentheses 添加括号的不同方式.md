# [LeetCode] Different Ways to Add Parentheses 添加括号的不同方式 - Grandyang - 博客园







# [[LeetCode] Different Ways to Add Parentheses 添加括号的不同方式](https://www.cnblogs.com/grandyang/p/4682458.html)







Given a string of numbers and operators, return all possible results from computing all the different possible ways to group numbers and operators. The valid operators are `+`, `-`and `*`.


Example 1

Input: `"2-1-1"`.
((2-1)-1) = 0
(2-(1-1)) = 2
Output: `[0, 2]`


Example 2

Input: `"2*3-4*5"`
(2*(3-(4*5))) = -34
((2*3)-(4*5)) = -14
((2*(3-4))*5) = -10
(2*((3-4)*5)) = -10
(((2*3)-4)*5) = 10
Output: `[-34, -14, -10, -10, 10]`



这道题让给我们一个可能含有加减乘的表达式，让我们在任意位置添加括号，求出所有可能表达式的不同值。这道题跟之前的那道[Unique Binary Search Trees II 独一无二的二叉搜索树之二](http://www.cnblogs.com/grandyang/p/4301096.html)用的方法一样，用递归来解，划分左右子树，递归构造。



```
class Solution {
public:
    vector<int> diffWaysToCompute(string input) {
        vector<int> res;
        for (int i = 0; i < input.size(); ++i) {
            if (input[i] == '+' || input[i] == '-' || input[i] == '*') {
                vector<int> left = diffWaysToCompute(input.substr(0, i));
                vector<int> right = diffWaysToCompute(input.substr(i + 1));
                for (int j = 0; j < left.size(); ++j) {
                    for (int k = 0; k < right.size(); ++k) {
                        if (input[i] == '+') res.push_back(left[j] + right[k]);
                        else if (input[i] == '-') res.push_back(left[j] - right[k]);
                        else res.push_back(left[j] * right[k]);
                    }
                }
            }
        }
        if (res.empty()) res.push_back(atoi(input.c_str()));
        return res;
    }
};
```



类似题目：

[Remove Invalid Parentheses 移除非法括号](http://www.cnblogs.com/grandyang/p/4944875.html)

[Longest Valid Parentheses](http://www.cnblogs.com/grandyang/p/4424731.html)

[Generate Parentheses](http://www.cnblogs.com/grandyang/p/4444160.html)

[Valid Parentheses](http://www.cnblogs.com/grandyang/p/4424587.html)



参考资料：

[http://www.cnblogs.com/ganganloveu/p/4681439.html](http://www.cnblogs.com/ganganloveu/p/4681439.html)

[https://leetcode.com/discuss/48488/c-4ms-recursive-method](https://leetcode.com/discuss/48488/c-4ms-recursive-method)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












