# [LeetCode] Basic Calculator II 基本计算器之二 - Grandyang - 博客园







# [[LeetCode] Basic Calculator II 基本计算器之二](https://www.cnblogs.com/grandyang/p/4601208.html)







Implement a basic calculator to evaluate a simple expression string.

The expression string contains only non-negativeintegers, `+`, `-`, `*`, `/` operators and empty spaces ``. The integer division should truncate toward zero.

Example 1:
Input: "3+2*2"
Output: 7

Example 2:
Input: " 3/2 "
Output: 1
Example 3:
Input: " 3+5 / 2 "
Output: 5

Note:
- You may assume that the given expression is always valid.
- Do not use the `eval` built-in library function.

**Credits:**
Special thanks to [@ts](https://leetcode.com/discuss/user/ts) for adding this problem and creating all test cases.



这道题是之前那道 [Basic Calculator](http://www.cnblogs.com/grandyang/p/4570699.html) 的拓展，不同之处在于那道题的计算符号只有加和减，而这题加上了乘除，那么就牵扯到了运算优先级的问题，好在这道题去掉了括号，还适当的降低了难度，估计再出一道的话就该加上括号了。不管那么多，这道题先按木有有括号来处理，由于存在运算优先级，我们采取的措施是使用一个栈保存数字，如果该数字之前的符号是加或减，那么把当前数字压入栈中，注意如果是减号，则加入当前数字的相反数，因为减法相当于加上一个相反数。如果之前的符号是乘或除，那么从栈顶取出一个数字和当前数字进行乘或除的运算，再把结果压入栈中，那么完成一遍遍历后，所有的乘或除都运算完了，再把栈中所有的数字都加起来就是最终结果了。代码如下：



解法一：

```
class Solution {
public:
    int calculate(string s) {
        long res = 0, num = 0, n = s.size();
        char op = '+';
        stack<int> st;
        for (int i = 0; i < n; ++i) {
            if (s[i] >= '0') {
                num = num * 10 + s[i] - '0';
            }
            if ((s[i] < '0' && s[i] != ' ') || i == n - 1) {
                if (op == '+') st.push(num);
                if (op == '-') st.push(-num);
                if (op == '*' || op == '/') {
                    int tmp = (op == '*') ? st.top() * num : st.top() / num;
                    st.pop();
                    st.push(tmp);
                }
                op = s[i];
                num = 0;
            } 
        }
        while (!st.empty()) {
            res += st.top();
            st.pop();
        }
        return res;
    }
};
```



在做了 [Basic Calculator III](http://www.cnblogs.com/grandyang/p/8873471.html) 之后，再反过头来看这道题，发现只要将处理括号的部分去掉直接就可以在这道题上使用，参见代码如下：



解法二：

```
class Solution {
public:
    int calculate(string s) {
        long res = 0, curRes = 0, num = 0, n = s.size();
        char op = '+';
        for (int i = 0; i < n; ++i) {
            char c = s[i];
            if (c >= '0' && c <= '9') {
                num = num * 10 + c - '0';
            }
            if (c == '+' || c == '-' || c == '*' || c == '/' || i == n - 1) {
                switch (op) {
                    case '+': curRes += num; break;
                    case '-': curRes -= num; break;
                    case '*': curRes *= num; break;
                    case '/': curRes /= num; break;
                }
                if (c == '+' || c == '-' || i == n - 1) {
                    res += curRes;
                    curRes = 0;
                }
                op = c;
                num = 0;
            } 
        }
        return res;
    }
};
```



类似题目：

[Basic Calculator III](http://www.cnblogs.com/grandyang/p/8873471.html)

[Basic Calculator](http://www.cnblogs.com/grandyang/p/4570699.html)

[Expression Add Operators](http://www.cnblogs.com/grandyang/p/4814506.html)



参考资料：

[https://leetcode.com/problems/basic-calculator-ii/](https://leetcode.com/problems/basic-calculator-ii/)

[https://leetcode.com/problems/basic-calculator-ii/discuss/63003/Share-my-java-solution](https://leetcode.com/problems/basic-calculator-ii/discuss/63003/Share-my-java-solution)

[https://leetcode.com/problems/basic-calculator-ii/discuss/63004/17-lines-C++-easy-20-ms](https://leetcode.com/problems/basic-calculator-ii/discuss/63004/17-lines-C++-easy-20-ms)

[https://leetcode.com/problems/basic-calculator-ii/discuss/63031/Simple-C++-solution-beats-85-submissions-with-detailed-explanations](https://leetcode.com/problems/basic-calculator-ii/discuss/63031/Simple-C++-solution-beats-85-submissions-with-detailed-explanations)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












