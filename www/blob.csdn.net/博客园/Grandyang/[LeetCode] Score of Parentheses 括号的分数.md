# [LeetCode] Score of Parentheses 括号的分数 - Grandyang - 博客园







# [[LeetCode] Score of Parentheses 括号的分数](https://www.cnblogs.com/grandyang/p/10634116.html)








Given a balanced parentheses string `S`, compute the score of the string based on the following rule:
- `()` has score 1
- `AB` has score `A + B`, where A and B are balanced parentheses strings.
- `(A)` has score `2 * A`, where A is a balanced parentheses string.

Example 1:

```
Input: "()"
Output: 1
```

Example 2:

```
Input: "(())"
Output: 2
```

Example 3:

```
Input: "()()"
Output: 2
```

Example 4:

```
Input: "(()(()))"
Output: 6
```

Note:
- `S` is a balanced parentheses string, containing only `(` and `)`.
- `2 <= S.length <= 50`




这道题给了我们一个只有括号的字符串，一个简单的括号值1分，并排的括号是分值是相加的关系，包含的括号是乘的关系，每包含一层，都要乘以个2。题目中给的例子很好的说明了题意，博主最先尝试的方法是递归来做，思路是先找出每一个最外层的括号，再对其中间的整体部分调用递归，比如对于 "()(())" 来说，第一个最外边的括号是 "()"，其中间为空，对空串调用递归返回0，但是结果 res 还是加1，这个特殊的处理后面会讲到。第二个最外边的括号是 "(())" 的外层括号，对其中间的 "()" 调用递归，返回1，再乘以2，则得到 "(())" 的值，然后把二者相加，就是最终需要的结果了。找部分合法的括号字符串的方法就是使用跟之前那道题 [Valid Parentheses](http://www.cnblogs.com/grandyang/p/4424587.html) 的相同的方法，使用一个计数器，遇到左括号，计数器自增1，反之右括号计数器自减1，那么当计数器为0的时候，就是一个合法的字符串了，我们对除去最外层的括号的中间内容调用递归，然后把返回值乘以2，并和1比较，取二者间的较大值加到结果 res 中，这是因为假如中间是空串，那么返回值是0，乘以2还是0，但是 "()" 的分值应该是1，所以累加的时候要跟1做比较。之后记得要更新i都正确的位置，参见代码如下：




解法一：

```
class Solution {
public:
    int scoreOfParentheses(string S) {
        int res = 0, n = S.size();
        for (int i = 0; i < n; ++i) {
            if (S[i] == ')') continue;
            int pos = i + 1, cnt = 1;
            while (cnt != 0) {
                (S[pos++] == '(') ? ++cnt : --cnt;
            }
            int cur = scoreOfParentheses(S.substr(i + 1, pos - i - 2));
            res += max(2 * cur, 1);
            i = pos - 1;
        }
        return res;
    }
};
```




我们也可以使用迭代来做，这里就要借助栈 stack 来做，因为递归在调用的时候，其实也是将当前状态压入栈中，等递归退出后再从栈中取出之前的状态。这里我们实现的思路是，遍历字符串S，当遇到左括号时，将当前的分数压入栈中，并把当前得分清0，若遇到的是右括号，说明此时已经形成了一个完整的合法的括号字符串了，而且除去外层的括号，内层的得分已经算出来了，就是当前的结果 res，此时就要乘以2，并且要跟1比较，取二者中的较大值，这样操作的原因已经在上面解法的讲解中解释过了。然后还要加上栈顶的值，因为栈顶的值是之前合法括号子串的值，跟当前的是并列关系，所以是相加的操作，最后不要忘了要将栈顶元素移除即可，参见代码如下：




解法二：

```
class Solution {
public:
    int scoreOfParentheses(string S) {
        int res = 0;
        stack<int> st;
        for (char c : S) {
            if (c == '(') {
                st.push(res);
                res = 0;
            } else {
                res = st.top() + max(res * 2, 1); 
                st.pop();
            }
        }
        return res;
    }
};
```




我们可以对空间复杂度进行进一步的优化，并不需要使用栈去保留所有的中间情况，我们可以只用一个变量 cnt 来记录当前在第几层括号之中，因为本题的括号累加值是有规律的，"()" 是1，因为最中间的括号在0层括号内，2^0 = 1。"(())" 是2，因为最中间的括号在1层括号内，2^1 = 2。"((()))" 是4，因为最中间的括号在2层括号内，2^2 = 4。因此类推，其实我们只需要统计出最中间那个括号外变有几个括号，就可以直接算出整个多重包含的括号字符串的值，参见代码如下：




解法三：

```
class Solution {
public:
    int scoreOfParentheses(string S) {
        int res = 0, cnt = 0, n = S.size();
        for (int i = 0; i < n; ++i) {
            (S[i] == '(') ? ++cnt : --cnt;
            if (S[i] == ')' && S[i - 1] == '(') res += (1 << cnt);
        }
        return res;
    }
};
```




类似题目：

[Valid Parentheses](http://www.cnblogs.com/grandyang/p/4424587.html)




参考资料：

[https://leetcode.com/problems/score-of-parentheses/](https://leetcode.com/problems/score-of-parentheses/)

[https://leetcode.com/problems/score-of-parentheses/discuss/141777/C%2B%2BJavaPython-O(1)-Space](https://leetcode.com/problems/score-of-parentheses/discuss/141777/C%2B%2BJavaPython-O(1)-Space)



[LeetCode All in One 题目讲解汇总(持续更新中...)](https://www.cnblogs.com/grandyang/p/4606334.html)












