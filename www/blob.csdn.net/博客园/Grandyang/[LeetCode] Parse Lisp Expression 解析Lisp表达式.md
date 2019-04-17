# [LeetCode] Parse Lisp Expression 解析Lisp表达式 - Grandyang - 博客园







# [[LeetCode] Parse Lisp Expression 解析Lisp表达式](https://www.cnblogs.com/grandyang/p/8045255.html)







You are given a string `expression` representing a Lisp-like expression to return the integer value of.

The syntax for these expressions is given as follows. 
- An expression is either an integer, a let-expression, an add-expression, a mult-expression, or an assigned variable. Expressions always evaluate to a single integer.
- (An integer could be positive or negative.)
- A let-expression takes the form `(let v1 e1 v2 e2 ... vn en expr)`, where `let` is always the string `"let"`, then there are 1 or more pairs of alternating variables and expressions, meaning that the first variable `v1` is assigned the value of the expression `e1`, the second variable `v2` is assigned the value of the expression `e2`, and so on sequentially; and then the value of this let-expression is the value of the expression `expr`.
- An add-expression takes the form `(add e1 e2)` where `add` is always the string `"add"`, there are always two expressions `e1, e2`, and this expression evaluates to the addition of the evaluation of `e1` and the evaluation of `e2`.
- A mult-expression takes the form `(mult e1 e2)` where `mult` is always the string `"mult"`, there are always two expressions `e1, e2`, and this expression evaluates to the multiplication of the evaluation of `e1` and the evaluation of `e2`.
- For the purposes of this question, we will use a smaller subset of variable names. A variable starts with a lowercase letter, then zero or more lowercase letters or digits. Additionally for your convenience, the names "add", "let", or "mult" are protected and will never be used as variable names.
- Finally, there is the concept of scope. When an expression of a variable name is evaluated, within the context of that evaluation, the innermost scope (in terms of parentheses) is checked first for the value of that variable, and then outer scopes are checked sequentially. It is guaranteed that every expression is legal. Please see the examples for more details on scope.

Evaluation Examples:
Input: (add 1 2)
Output: 3

Input: (mult 3 (add 2 3))
Output: 15

Input: (let x 2 (mult x 5))
Output: 10

Input: (let x 2 (mult x (let x 3 y 4 (add x y))))
Output: 14
Explanation: In the expression (add x y), when checking for the value of the variable x,
we check from the innermost scope to the outermost in the context of the variable we are trying to evaluate.
Since x = 3 is found first, the value of x is 3.

Input: (let x 3 x 2 x)
Output: 2
Explanation: Assignment in let statements is processed sequentially.

Input: (let x 1 y 2 x (add x y) (add x y))
Output: 5
Explanation: The first (add x y) evaluates as 3, and is assigned to x.
The second (add x y) evaluates as 3+2 = 5.

Input: (let x 2 (add (let x 3 (let x 4 x)) x))
Output: 6
Explanation: Even though (let x 4 x) has a deeper scope, it is outside the context
of the final x in the add-expression.  That final x will equal 2.

Input: (let a1 3 b2 (add a1 1) b2) 
Output 4
Explanation: Variable names can contain digits after the first character.


Note:
- The given string `expression` is well formatted: There are no leading or trailing spaces, there is only a single space separating different components of the string, and no space between adjacent parentheses. The expression is guaranteed to be legal and evaluate to an integer.
- The length of `expression` is at most 2000. (It is also non-empty, as that would not be a legal expression.)
- The answer and all intermediate calculations of that answer are guaranteed to fit in a 32-bit integer.



这道题让我们解析Lisp语言的表达式，以前听说过Lisp语言，但是完全没有接触过，看了题目中的描述和给的例子，感觉很叼。估计题目只让我们处理一些简单的情况，毕竟不可能让我们写一个编译器出来。题目中说了给定的表达式都是合法的，这样也降低了难度。还有一个好的地方是题目给了充足的例子，让我们去更好的理解这门新的语言。我们通过分析例子发现，所有的命令都是用括号来包裹的，而且里面还可以嵌套小括号即子命令。让我们处理的命令只有三种，add，mult，和let。其中add和mult比较简单就是加法和乘法，就把后面两个数字或者子表达式的值加起来或成起来即可。let命令稍稍麻烦一些，后面可以跟好多变量或表达式，最简单的是三个，一般第一个是个变量，比如x，后面会跟一个数字或子表达式，就是把后面的数字或子表达式的值赋值给前面的变量，第三个位置是个表达式，其值是当前let命令的返回值。还有一个比较重要的特性是外层的变量值不会随着里层的变量值改变，比如对于下面这个例子：

```
(let x 2 (add (let x 3 (let x 4 x)) x))
```

刚开始x被赋值为2了，然后在返回值表达式中，又有一个add操作，add操作的第一个变量又是一个子表达式，在这个子表达式中又定义了一个变量x，并复制为3，再其返回值表达式又定义了一个变量x，赋值为4，并返回这个x，那么最内层的表达式的返回值是4，那么x被赋值为3的那层的返回值也是4，此时add的第一个数就是4，那么其第二个x是多少，其实这个x并没有被里层的x的影响，仍然是刚开始赋值的2，那么我们就看出特点了，外层的变量是能影响里层变量的，而里层变量无法影响外层变量。那么我们只要在递归的时候不加引用就行了，这样值就不会在递归函数中被更改了。

对于这种长度不定且每个可能包含子表达式的题，递归是一个很好的选择，由于需要给变量赋值，所以需要建立一个变量和其值之间的映射，然后我们就要来写递归函数了，最开始我们给定的表达式肯定是有括号的，所以我们先处理这种情况，括号对于我们的解析没有用，所以要去掉首尾的括号，然后我们用一个变量cur表示当前指向字符的位置，初始化为0，下面要做的就是先解析出命令单词，我们调用一个子函数parse，在parse函数中，简单的情况就是解析出add，mult，或let这三个命令单词，我们用一个指针来遍历字符，当越界或遇到空格就停止，但是如果我们需要解析的是个子表达式，而且里面可能还有多个子表达式，那么我们就需要找出最外面这个左括号对应的右括号，因为中间可能还会有别的左右括号，里面的内容就再之后再次调用递归函数时处理。判断的方法就是利用匹配括号的方法，用变量cnt来表示左括号的的个数，初始化为1，当要parse的表达式第一个字符是左括号时，进入循环，循环条件是cnt不为0，当遇到左括号时cnt自增1，反之当遇到右括号时cnt自减1，每次指针end都向右移动一个，最后我们根据end的位置减去初始时cur的值（保存在变量t中），可以得到表达式。如果解析出的是命令let，那么进行while循环，然后继续解析后面的内容，如果此时cur大于s的长度了，说明此时是let命令的最后一个部分，也就是返回值部分，直接调用递归函数返回即可。否则就再解析下一个部分，说明此时是变量和其对应值，我们要建立映射关系。如果之前解析出来的是add命令，那么比较简单，就直接解析出后面的两个部分的表达式，并分别调用递归函数，将递归函数的返回值累加并返回即可。对于mult命令同样的处理方式，只不过是将两个递归函数的返回值乘起来并返回。然后我们再来看如果表达式不是以左括号开头的，说明只能是数字或者变量，那么先来检测数字，如果第一个字符是负号或者0到9之间的数字，那么直接将表达式转为int型即可；否则的话就是变量，我们直接从哈希map中取值即可。最后需要注意的就是递归函数的参数哈希map一定不能加引用，具体可以参见上面那个例子的分析，加了引用后外层的变量值就会受内层的影响，这是不符合题意的，参见代码如下：



```
class Solution {
public:
    int evaluate(string expression) {
        unordered_map<string, int> m;
        return helper(expression, m);
    }
    int helper(string str, unordered_map<string, int> m) {
        if (str[0] == '-' || (str[0] >= '0' && str[0] <= '9')) return stoi(str);
        else if (str[0] != '(') return m[str];
        string s = str.substr(1, str.size() - 2);
        int cur = 0;
        string cmd = parse(s, cur);
        if (cmd == "let") {
            while (true) {
                string var = parse(s, cur);
                if (cur > s.size()) return helper(var, m);
                string t = parse(s, cur);
                m[var] = helper(t, m);
            }
        } else if (cmd == "add") {
            return helper(parse(s, cur), m) + helper(parse(s, cur), m);
        } else if (cmd == "mult") {
            return helper(parse(s, cur), m) * helper(parse(s, cur), m);
        }
    }
    string parse(string& s, int& cur) {
        int end = cur + 1, t = cur, cnt = 1;
        if (s[cur] == '(') {
            while (cnt != 0) {
                if (s[end] == '(') ++cnt;
                else if (s[end] == ')') --cnt;
                ++end;
            }
        } else {
            while (end < s.size() && s[end] != ' ') ++end;
        }
        cur = end + 1;
        return s.substr(t, end - t);
    }
};
```



类似题目：

[Ternary Expression Parser](http://www.cnblogs.com/grandyang/p/6022498.html)



参考资料：

[https://discuss.leetcode.com/topic/112079/c-recursion-solution-with-explaination/2](https://discuss.leetcode.com/topic/112079/c-recursion-solution-with-explaination/2)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












