# [LeetCode] Solve the Equation 解方程 - Grandyang - 博客园







# [[LeetCode] Solve the Equation 解方程](https://www.cnblogs.com/grandyang/p/7350578.html)








Solve a given equation and return the value of `x` in the form of string "x=#value". The equation contains only '+', '-' operation, the variable `x` and its coefficient.

If there is no solution for the equation, return "No solution".

If there are infinite solutions for the equation, return "Infinite solutions".

If there is exactly one solution for the equation, we ensure that the value of `x` is an integer.

Example 1:
Input: "x+5-3+x=6+x-2"
Output: "x=2"



Example 2:
Input: "x=x"
Output: "Infinite solutions"



Example 3:
Input: "2x=x"
Output: "x=0"



Example 4:
Input: "2x+3x-6x=x+2"
Output: "x=-1"



Example 5:
Input: "x=x+2"
Output: "No solution"



这道题给了我们一个用字符串表示的方程式，让我们求出x的解，根据例子可知，还包括x有无穷多个解和x没有解的情况。解一元一次方程没什么难度，难点在于处理字符串，如何将x的系数合并起来，将常数合并起来，化简成ax=b的形式来求解。博主最开始的思路是先找到等号，然后左右两部分分开处理。由于要化成ax=b的格式，所以左半部分对于x的系数都是加，右半部分对于x的系数都是减。同理，左半部分对于常数是减，右半部分对于常数是加。

那么我们就开始处理字符串了，我们定义一个符号变量sign，初始化为1，数字变量num，初始化为-1，后面会提到为啥不能初始化为0。我们遍历每一个字符，如果遇到了符号位，我们看num的值，如果num是-1的话，说明是初始值，没有更新过，我们将其赋值为0；反之，如果不是-1，说明num已经更新过了，我们乘上当前的正负符号值sign。这是为了区分"-3"和"3+3"这种两种情况，遇到-3种的符号时，我们还不需要加到b中，所以num此时必须为0，而遇到3+3中的加号时，此时num已经为3了，我们要把前面的3加到b中。

遇到数字的时候，我们还是要看num的值，如果是初始值，那么就将其赋值为0，然后计算数字的时候要先给num乘10，再加上当前的数字。这样做的原因是常数不一定都是个位数字，有可能是两位数或者三位数，这样做才能正确的读入数字。我们在遇到数字的时候并不更新a或者b，我们只在遇到符号位或者x的时候才更新。这样如果最后一位是数字的话就会产生问题，所以我们要在字符串的末尾加上一个+号，这样确保了末尾数字会被处理。

遇到x的时候比较tricky，因为可能是x, 0x, -x这几种情况，我们还是首先要看num的值是否为初始值-1，如果是的话，那么就可能是x或-x这种情况，我们此时将num赋值为sign；如果num不是-1，说明num已经被更新了，可能是0x, -3x等等，所以我们要将num赋值为num*sign。这里应该就明白了为啥不能将num初始化为0了，因为一旦初始化为0了，就没法区分x和0x这两种情况了。

那么我们算完了a和b，得到了ax=b的等式，下面的步骤就很简单了，只要分情况讨论得出正确的返回结果即可，参见代码如下：



解法一：

```
class Solution {
public:
    string solveEquation(string equation) {
        int a = 0, b = 0;
        auto found = equation.find("=");
        helper(equation.substr(0, found), true, a, b);
        helper(equation.substr(found + 1), false, a, b);
        if (a == 0 && a == b) return "Infinite solutions";
        if (a == 0 && a != b) return "No solution";
        return "x=" + to_string(b / a);
    }
    void helper(string e, bool isLeft, int& a, int& b) {
        int sign = 1, num = -1;
        e += "+";
        for (int i = 0; i < e.size(); ++i) {
            if (e[i] == '-' || e[i] == '+') {
                num = (num == -1) ? 0 : (num * sign);
                b += isLeft ? -num : num;
                num = -1;
                sign = (e[i] == '+') ? 1 : -1;
            } else if (e[i] >= '0' && e[i] <= '9') {
                if (num == -1) num = 0;
                num = num * 10 + e[i] - '0';
            } else if (e[i] == 'x') {
                num = (num == -1) ? sign : (num * sign);
                a += isLeft ? num : -num;
                num = -1;
            }
        }
    }
};
```



下面这种解法也很不错，也是求ax=b等式中的a和b，但是没有根据等号拆分为左右两部分，而是用一个变量sign来控制是对a和b加还是减，这跟上面解法中的的sign不一样。这里没有专门管正负的变量，而是通过双指针，指向数字的范围，这个数字可以是x的系数，也可以是常量，可以带着正负号，然后通过stoi函数将字符串直接转为整型数，然后乘以sign加到a或b中。变量j会指向数字或者符号，当i大于j时，我们就提取出范围内的数字。当我们遇到x的时候，跟上面一样，要处理+x, -x, 0x的情况，我们看前一位的字符，如果是符号，那么我们直接给a加上符号值；如果是数字，就用上面的办法提取出数字乘以sign加到a中。如果遇到了等号，那么先处理前面的数字(如果有的话)，然后flip sign。最后循环结束后，还要考虑最后一位是数字的情况，要加到b中。后面分情况讨论就不多说了，参见代码如下：



解法二：

```
class Solution {
public:
    string solveEquation(string equation) {
        int n = equation.size(), a = 0, b = 0, sign = 1, j = 0;
        for (int i = 0; i < n; ++i) {
            if (equation[i] == '+' || equation[i] == '-') {
                if (i > j) b += stoi(equation.substr(j, i - j)) * sign;
                j = i;
            } else if (equation[i] == 'x') {
                if (i == j || equation[i - 1] == '+') {
                    a += sign;                 
                } else if (equation[i - 1] == '-') {
                    a -= sign;
                } else {
                    a += stoi(equation.substr(j, i - j)) * sign;
                }
                j = i + 1;
            } else if (equation[i] == '=') {
                if (i > j) b += stoi(equation.substr(j, i - j)) * sign;
                sign = -1;
                j = i + 1;
            }
        }
        if (j < n) b += stoi(equation.substr(j)) * sign;
        if (a == 0 && a == b) return "Infinite solutions";
        if (a == 0 && a != b) return "No solution";
        return "x=" + to_string(-b / a);
    }
};
```



类似题目：

[Fraction Addition and Subtraction](http://www.cnblogs.com/grandyang/p/6954197.html)



参考资料：

[https://discuss.leetcode.com/topic/95203/concise-java-solution](https://discuss.leetcode.com/topic/95203/concise-java-solution)

[https://discuss.leetcode.com/topic/95279/c-two-pointers-concise-solution](https://discuss.leetcode.com/topic/95279/c-two-pointers-concise-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













