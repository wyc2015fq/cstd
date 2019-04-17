# [LeetCode] Basic Calculator III 基本计算器之三 - Grandyang - 博客园







# [[LeetCode] Basic Calculator III 基本计算器之三](https://www.cnblogs.com/grandyang/p/8873471.html)







Implement a basic calculator to evaluate a simple expression string.

The expression string may contain open `(` and closing parentheses `)`, the plus `+` or minus sign `-`, non-negative integers and empty spaces ``.

The expression string contains only non-negative integers, `+`, `-`, `*`, `/` operators , open `(` and closing parentheses `)` and empty spaces ``. The integer division should truncate toward zero.

You may assume that the given expression is always valid. All intermediate results will be in the range of `[-2147483648, 2147483647]`.

Some examples:
"1 + 1" = 2
" 6-4 / 2 " = 4
"2*(5+5*2)/3+(6/2+8)" = 21
"(2+6* 3+5- (3*14/7+2)*5)+3"=-12



Note: Do not use the `eval` built-in library function.



这道题是基本计算器系列的第三道，前两道分别为[Basic Calculator](http://www.cnblogs.com/grandyang/p/4570699.html)和[Basic Calculator II](http://www.cnblogs.com/grandyang/p/4601208.html)，区别是，第一道只有加减法跟括号，第二道只有加减乘除法，而这第三道既有加减乘除法又有括号运算。其实做过前两道题的话，那么这道题也就没什么问题，因为把前两道题的解法综合一下就是这道题的解法啦。由于此题既有括号，又有乘除法，我们知道括号是优先级最高的，但是好就好在我们可以将括号里的内容当作一个整体调用递归函数来处理。而其他部分，就跟第二道一模一样了。我们还是分情况来处理遍历，我们需要几个变量，num表示当前的数字，curRes表示当前的结果，res为最终的结果，op为操作符号，初始化为'+'。当遇到数字的时候，我们将num自乘以10并加上这个数字，这是由于可能遇到多位数，所以每次要乘以10。当遇到括号的时候，这里就有一个小trick，由于表示可能会有括号嵌套括号，所以我们如果搜索右括号的话，就有可能使得括号没有正确的匹配上，所以我们用一个变量cnt，遇到左括号自增1，遇到右括号自减1，当cnt为0的时候，说明括号正好完全匹配，这个trick在验证括号是否valid的时候经常使用到。然后我们就是根据左右括号的位置提取出中间的子字符串调用递归函数，返回值赋给num。如果遇到符号，或者是最后一个位置的字符时，我们根据op的值对num进行分别的加减乘除的处理，结果保存到curRes中。然后再次判读如果op是加或减，或者是最后一个位置的字符时，将curRes加到结果res中，并且curRes重置为0。最后将当前字符c赋值给op（注意这里只有当时最后一个位置的字符时，才有可能不是运算符号，不过也不要紧了，因为遍历已经结束了），num也要重置为0，参见代码如下：



```
class Solution {
public:
    int calculate(string s) {
        int n = s.size(), num = 0, curRes = 0, res = 0;
        char op = '+';
        for (int i = 0; i < n; ++i) {
            char c = s[i];
            if (c >= '0' && c <= '9') {
                num = num * 10 + c - '0';
            } else if (c == '(') {
                int j = i, cnt = 0;
                for (; i < n; ++i) {
                    if (s[i] == '(') ++cnt;
                    if (s[i] == ')') --cnt;
                    if (cnt == 0) break;
                }
                num = calculate(s.substr(j + 1, i - j - 1));
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

[Basic Calculator IV](http://www.cnblogs.com/grandyang/p/8934427.html)

[Basic Calculator II](http://www.cnblogs.com/grandyang/p/4601208.html)

[Basic Calculator](http://www.cnblogs.com/grandyang/p/4570699.html)



参考资料：

[https://leetcode.com/problems/basic-calculator-iii/discuss/113597/C++-recursive](https://leetcode.com/problems/basic-calculator-iii/discuss/113597/C++-recursive)

[https://leetcode.com/problems/basic-calculator-iii/discuss/113593/C++-Consise-Solution](https://leetcode.com/problems/basic-calculator-iii/discuss/113593/C++-Consise-Solution)

[https://leetcode.com/problems/basic-calculator-iii/discuss/113592/Development-of-a-generic-solution-for-the-series-of-the-calculator-problems](https://leetcode.com/problems/basic-calculator-iii/discuss/113592/Development-of-a-generic-solution-for-the-series-of-the-calculator-problems)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












