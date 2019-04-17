# [LeetCode] Basic Calculator 基本计算器 - Grandyang - 博客园







# [[LeetCode] Basic Calculator 基本计算器](https://www.cnblogs.com/grandyang/p/4570699.html)







Implement a basic calculator to evaluate a simple expression string.

The expression string may contain open `(` and closing parentheses `)`, the plus `+` or minus sign `-`, non-negativeintegers and empty spaces ``.

Example 1:
Input: "1 + 1"
Output: 2

Example 2:
Input: " 2-1 + 2 "
Output: 3
Example 3:
Input: "(1+(4+5+2)-3)+(6+8)"
Output: 23
Note:
- You may assume that the given expression is always valid.
- Do not use the `eval` built-in library function.



这道题让我们实现一个基本的计算器来计算简单的算数表达式，而且题目限制了表达式中只有加减号，数字，括号和空格，没有乘除，那么就没啥计算的优先级之分了。于是这道题就变的没有那么复杂了。我们需要一个栈来辅助计算，用个变量sign来表示当前的符号，我们遍历给定的字符串s，如果遇到了数字，由于可能是个多位数，所以我们要用while循环把之后的数字都读进来，然后用sign*num来更新结果res；如果遇到了加号，则sign赋为1，如果遇到了符号，则赋为-1；如果遇到了左括号，则把当前结果res和符号sign压入栈，res重置为0，sign重置为1；如果遇到了右括号，结果res乘以栈顶的符号，栈顶元素出栈，结果res加上栈顶的数字，栈顶元素出栈。代码如下：



解法一：

```
class Solution {
public:
    int calculate(string s) {
        int res = 0, sign = 1, n = s.size();
        stack<int> st;
        for (int i = 0; i < n; ++i) {
            char c = s[i];
            if (c >= '0') {
                int num = 0;
                while (i < n && s[i] >= '0') {
                    num = 10 * num + (s[i++] - '0');
                }
                res += sign * num;
                --i;
            } else if (c == '+') {
                sign = 1;
            } else if (c == '-') {
                sign = -1;
            } else if (c == '(') {
                st.push(res);
                st.push(sign);
                res = 0;
                sign = 1;
            } else if (c == ')') {
                res *= st.top(); st.pop();
                res += st.top(); st.pop();
            }
        }
        return res;
    }
};
```



下面这种方法和上面的基本一样，只不过对于数字的处理略微不同，上面的方法是连续读入数字，而这种方法是使用了一个变量来保存读入的num，所以在遇到其他字符的时候，都要用sign*num来更新结果res，参见代码如下：



解法二：

```
class Solution {
public:
    int calculate(string s) {
        int res = 0, num = 0, sign = 1, n = s.size();
        stack<int> st;
        for (int i = 0; i < n; ++i) {
            char c = s[i];
            if (c >= '0') {
                num = 10 * num + (c - '0');
            } else if (c == '+' || c == '-') {
                res += sign * num;
                num = 0;
                sign = (c == '+') ? 1 : -1;
             } else if (c == '(') {
                st.push(res);
                st.push(sign);
                res = 0;
                sign = 1;
            } else if (c == ')') {
                res += sign * num;
                num = 0;
                res *= st.top(); st.pop();
                res += st.top(); st.pop();
            }
        }
        res += sign * num;
        return res;
    }
};
```



在做了[Basic Calculator III](http://www.cnblogs.com/grandyang/p/8873471.html)之后，再反过头来看这道题，发现递归处理括号的方法在这道题也同样适用，我们用一个变量cnt，遇到左括号自增1，遇到右括号自减1，当cnt为0的时候，说明括号正好完全匹配，这个trick在验证括号是否valid的时候经常使用到。然后我们就是根据左右括号的位置提取出中间的子字符串调用递归函数，返回值赋给num，参见代码如下：



解法三：

```
class Solution {
public:
    int calculate(string s) {
        int res = 0, num = 0, sign = 1, n = s.size();
        for (int i = 0; i < n; ++i) {
            char c = s[i];
            if (c >= '0' && c <= '9') {
                num = 10 * num + (c - '0');
            } else if (c == '(') {
                int j = i, cnt = 0;
                for (; i < n; ++i) {
                    if (s[i] == '(') ++cnt;
                    if (s[i] == ')') --cnt;
                    if (cnt == 0) break;
                }
                num = calculate(s.substr(j + 1, i - j - 1));
            }
            if (c == '+' || c == '-' || i == n - 1) {
                res += sign * num;
                num = 0;
                sign = (c == '+') ? 1 : -1;
             } 
        }
        return res;
    }
};
```



类似题目：

[Basic Calculator IV](http://www.cnblogs.com/grandyang/p/8934427.html)

[Basic Calculator III](http://www.cnblogs.com/grandyang/p/8873471.html)

[Basic Calculator II](http://www.cnblogs.com/grandyang/p/4601208.html)

[Evaluate Reverse Polish Notation](http://www.cnblogs.com/grandyang/p/4247718.html)

[Different Ways to Add Parentheses](http://www.cnblogs.com/grandyang/p/4682458.html)

[Expression Add Operators](http://www.cnblogs.com/grandyang/p/4814506.html)



参考资料：

[https://leetcode.com/problems/basic-calculator/](https://leetcode.com/problems/basic-calculator/)

[https://leetcode.com/problems/basic-calculator/discuss/62361/Iterative-Java-solution-with-stack](https://leetcode.com/problems/basic-calculator/discuss/62361/Iterative-Java-solution-with-stack)

[https://leetcode.com/problems/basic-calculator/discuss/62362/JAVA-Easy-Version-To-Understand!!!!!](https://leetcode.com/problems/basic-calculator/discuss/62362/JAVA-Easy-Version-To-Understand!!!!!)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












