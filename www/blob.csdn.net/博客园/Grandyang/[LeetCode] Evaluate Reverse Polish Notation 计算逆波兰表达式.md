# [LeetCode] Evaluate Reverse Polish Notation 计算逆波兰表达式 - Grandyang - 博客园







# [[LeetCode] Evaluate Reverse Polish Notation 计算逆波兰表达式](https://www.cnblogs.com/grandyang/p/4247718.html)







Evaluate the value of an arithmetic expression in [Reverse Polish Notation](http://en.wikipedia.org/wiki/Reverse_Polish_notation).

Valid operators are `+`, `-`, `*`, `/`. Each operand may be an integer or another expression.

Note:
- Division between two integers should truncate toward zero.
- The given RPN expression is always valid. That means the expression would always evaluate to a result and there won't be any divide by zero operation.

Example 1:
Input: ["2", "1", "+", "3", "*"]
Output: 9
Explanation: ((2 + 1) * 3) = 9

Example 2:
Input: ["4", "13", "5", "/", "+"]
Output: 6
Explanation: (4 + (13 / 5)) = 6

Example 3:
Input: ["10", "6", "9", "3", "+", "-11", "*", "/", "*", "17", "+", "5", "+"]
Output: 22
Explanation: 
  ((10 * (6 / ((9 + 3) * -11))) + 17) + 5
= ((10 * (6 / (12 * -11))) + 17) + 5
= ((10 * (6 / -132)) + 17) + 5
= ((10 * 0) + 17) + 5
= (0 + 17) + 5
= 17 + 5
= 22


[逆波兰表达式](http://zh.wikipedia.org/wiki/%E9%80%86%E6%B3%A2%E5%85%B0%E8%A1%A8%E7%A4%BA%E6%B3%95)就是把操作数放前面，把操作符后置的一种写法，我们通过观察可以发现，第一个出现的运算符，其前面必有两个数字，当这个运算符和之前两个数字完成运算后从原数组中删去，把得到一个新的数字插入到原来的位置，继续做相同运算，直至整个数组变为一个数字。于是按这种思路写了代码如下，但是拿到OJ上测试，发现会有Time Limit Exceeded的错误，无奈只好上网搜答案，发现大家都是用栈做的。仔细想想，这道题果然应该是栈的完美应用啊，从前往后遍历数组，遇到数字则压入栈中，遇到符号，则把栈顶的两个数字拿出来运算，把结果再压入栈中，直到遍历完整个数组，栈顶数字即为最终答案。代码如下:



解法一：

```
class Solution {
public:
    int evalRPN(vector<string>& tokens) {
        if (tokens.size() == 1) return stoi(tokens[0]);
        stack<int> st;
        for (int i = 0; i < tokens.size(); ++i) {
            if (tokens[i] != "+" && tokens[i] != "-" && tokens[i] != "*" && tokens[i] != "/") {
                st.push(stoi(tokens[i]));
            } else {
                int num1 = st.top(); st.pop();
                int num2 = st.top(); st.pop();
                if (tokens[i] == "+") st.push(num2 + num1);
                if (tokens[i] == "-") st.push(num2 - num1);
                if (tokens[i] == "*") st.push(num2 * num1);
                if (tokens[i] == "/") st.push(num2 / num1);
            }
        }
        return st.top();
    }
};
```



我们也可以用递归来做，由于一个有效的逆波兰表达式的末尾必定是操作符，所以我们可以从末尾开始处理，如果遇到操作符，向前两个位置调用递归函数，找出前面两个数字，然后进行操作将结果返回，如果遇到的是数字直接返回即可，参见代码如下：



解法二：

```
class Solution {
public:
    int evalRPN(vector<string>& tokens) {
        int op = (int)tokens.size() - 1;
        return helper(tokens, op);
    }
    int helper(vector<string>& tokens, int& op) {
        string str = tokens[op];
        if (str != "+" && str != "-" && str != "*" && str != "/") return stoi(str);
        int num1 = helper(tokens, --op);
        int num2 = helper(tokens, --op);
        if (str == "+") return num2 + num1;
        if (str == "-") return num2 - num1;
        if (str == "*") return num2 * num1;
        return num2 / num1;
    }
};
```



类似题目：

[Basic Calculator](http://www.cnblogs.com/grandyang/p/4570699.html)

[Expression Add Operators](http://www.cnblogs.com/grandyang/p/4814506.html)



参考资料：

[https://leetcode.com/problemset/algorithms/](https://leetcode.com/problemset/algorithms/)

[https://leetcode.com/problems/evaluate-reverse-polish-notation/discuss/47642/a-recursive-solution-in-cpp](https://leetcode.com/problems/evaluate-reverse-polish-notation/discuss/47642/a-recursive-solution-in-cpp)

[https://leetcode.com/problems/evaluate-reverse-polish-notation/discuss/47544/Challenge-me-neat-C%2B%2B-solution-could-be-simpler](https://leetcode.com/problems/evaluate-reverse-polish-notation/discuss/47544/Challenge-me-neat-C%2B%2B-solution-could-be-simpler)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












