# [LeetCode] Valid Parenthesis String 验证括号字符串 - Grandyang - 博客园







# [[LeetCode] Valid Parenthesis String 验证括号字符串](https://www.cnblogs.com/grandyang/p/7617017.html)







Given a string containing only three types of characters: '(', ')' and '*', write a function to check whether this string is valid. We define the validity of a string by these rules:
- Any left parenthesis `'('` must have a corresponding right parenthesis `')'`.
- Any right parenthesis `')'` must have a corresponding left parenthesis `'('`.
- Left parenthesis `'('` must go before the corresponding right parenthesis `')'`.
- `'*'` could be treated as a single right parenthesis `')'` or a single left parenthesis `'('` or an empty string.
- An empty string is also valid.



Example 1:
Input: "()"
Output: True



Example 2:
Input: "(*)"
Output: True



Example 3:
Input: "(*))"
Output: True



Note:
- The string size will be in the range [1, 100].



这道题让我们验证括号字符串，跟之前那道[Valid Parentheses](http://www.cnblogs.com/grandyang/p/4424587.html)有些类似。不同之处在于这道题不只有小括号，还存在星号，星号可以当左括号，右括号，或空来使用，问我们能不能得到一个合法的括号字符串。那么我们想，如果不存在星号，那么这题是不是异常的简单，我们甚至连stack都可以不用，直接用一个变量，遇到左括号，自增1，遇到右括号，如果此时计数器已经为0了，直接返回false，否则自减1，一旦计数器出现了负数，立即返回false，最后还要看变量是否为0即可。但是由于星号的存在，这道题就变的复杂了，由于星号可以当括号用，所以当遇到右括号时，就算此时变量为0，也可以用星号来当左括号使用。那么星号什么时候都能当括号来用吗，我们来看两个例子 *) 和 *( ，在第一种情况下，星号可以当左括号来用，而在第二种情况下，无论星号当左括号，右括号，还是空，*( 都是不对的。当然这种情况只限于星号和左括号之间的位置关系，而只要星号在右括号前面，就一定可以消掉右括号。那么我们使用两个stack，分别存放左括号和星号的位置，遍历字符串，当遇到星号时，压入星号栈star，当遇到左括号时，压入左括号栈left，当遇到右括号时，此时如果left和star均为空时，直接返回false；如果left不为空，则pop一个左括号来抵消当前的右括号；否则从star中取出一个星号当作左括号来抵消右括号。当循环结束后，我们希望left中没有多余的左括号，就算有，我们可以尝试着用星号来抵消，当star和left均不为空时，进行循环，如果left的栈顶左括号的位置在star的栈顶星号的右边，那么就组成了 *( 模式，直接返回false；否则就说明星号可以抵消左括号，各自pop一个元素。最终退出循环后我们看left中是否还有多余的左括号，没有就返回true，否则false，参见代码如下：



解法一：

```
class Solution {
public:
    bool checkValidString(string s) {
        stack<int> left, star;
        for (int i = 0; i < s.size(); ++i) {
            if (s[i] == '*') star.push(i);
            else if (s[i] == '(') left.push(i);
            else {
                if (left.empty() && star.empty()) return false;
                if (!left.empty()) left.pop();
                else star.pop();
            }
        }
        while (!left.empty() && !star.empty()) {
            if (left.top() > star.top()) return false;
            left.pop(); star.pop();
        }
        return left.empty();
    }
};
```



如果你觉得上面的解法逻辑稍稍复杂了一些，我们来看一种逻辑无比简单的解法。既然星号可以当左括号和右括号，那么我们就正反各遍历一次，正向遍历的时候，我们把星号都当成左括号，此时用经典的验证括号的方法，即遇左括号计数器加1，遇右括号则自减1，如果中间某个时刻计数器小于0了，直接返回false。如果最终计数器等于0了，我们直接返回true，因为此时我们把星号都当作了左括号，可以跟所有的右括号抵消。而此时就算计数器大于0了，我们暂时不能返回false，因为有可能多余的左括号是星号变的，星号也可以表示空，所以有可能不多，我们还需要反向q一下，哦不，是反向遍历一下，这是我们将所有的星号当作右括号，遇右括号计数器加1，遇左括号则自减1，如果中间某个时刻计数器小于0了，直接返回false。遍历结束后直接返回true，这是为啥呢？此时计数器有两种情况，要么为0，要么大于0。为0不用说，肯定是true，为啥大于0也是true呢？因为之前正向遍历的时候，我们的左括号多了，我们之前说过了，多余的左括号可能是星号变的，也可能是本身就多的左括号。本身就多的左括号这种情况会在反向遍历时被检测出来，如果没有检测出来，说明多余的左括号一定是星号变的。而这些星号在反向遍历时又变做了右括号，最终导致了右括号有剩余，所以当这些星号都当作空的时候，左右括号都是对应的，即是合法的。你可能会有疑问，右括号本身不会多么，其实不会的，如果多的话，会在正向遍历中被检测出来，参见代码如下：



解法二：

```
class Solution {
public:
    bool checkValidString(string s) {
        int left = 0, right = 0, n = s.size();
        for (int i = 0; i < n; ++i) {
            if (s[i] == '(' || s[i] == '*') ++left;
            else --left;
            if (left < 0) return false;
        }
        if (left == 0) return true;
        for (int i = n - 1; i >= 0; --i) {
            if (s[i] == ')' || s[i] == '*') ++right;
            else --right;
            if (right < 0) return false;
        }
        return true;
    }
};
```



下面这种方法是用递归来写的，思路特别的简单直接，感觉应该属于暴力破解法。使用了变量cnt来记录左括号的个数，变量start表示当前开始遍历的位置，那么在递归函数中，首先判断如果cnt小于0，直接返回false。否则进行从start开始的遍历，如果当前字符为左括号，cnt自增1；如果为右括号，若cnt此时小于等于0，返回false，否则cnt自减1；如果为星号，我们同时递归三种情况，分别是当星号为空，左括号，或右括号，只要有一种情况返回true，那么就是true了。如果循环退出后，若cnt为0，返回true，否则false，参见代码如下：



解法三：

```
class Solution {
public:
    bool checkValidString(string s) {
        return helper(s, 0, 0);
    }
    bool helper(string s, int start, int cnt) {
        if (cnt < 0) return false;
        for (int i = start; i < s.size(); ++i) {
            if (s[i] == '(') {
                ++cnt;
            } else if (s[i] == ')') {
                if (cnt <= 0) return false;
                --cnt;
            } else {
                return helper(s, i + 1, cnt) || helper(s, i + 1, cnt + 1) || helper(s, i + 1, cnt - 1);
            }
        }
        return cnt == 0;
    }
};
```



下面这种解法是论坛上排第一的解法，感觉思路清新脱俗，博主研究了好久，参考了网友的留言才稍稍弄懂了一些，这里维护了两个变量low和high，其中low表示在有左括号的情况下，将星号当作右括号时左括号的个数(这样做的原因是尽量不多增加右括号的个数)，而high表示将星号当作左括号时左括号的个数。是不是很绕，没办法。那么当high小于0时，说明就算把星号都当作左括号了，还是不够抵消右括号，返回false。而当low大于0时，说明左括号的个数太多了，没有足够多的右括号来抵消，返回false。那么开始遍历字符串，当遇到左括号时，low和high都自增1；当遇到右括号时，只有当low大于0时，low才自减1，保证了low不会小于0，而high直接自减1；当遇到星号时，只有当low大于0时，low才自减1，保证了low不会小于0，而high直接自增1，因为high把星号当作左括号。当此时high小于0，说明右括号太多，返回false。当循环退出后，我们看low是否为0，参见代码如下：



解法四：

```
class Solution {
public:
    bool checkValidString(string s) {
        int low = 0, high = 0;
        for (char c : s) {
            if (c == '(') {
                ++low; ++high;
            } else if (c == ')') {
                if (low > 0) --low;
                --high;
            } else {
                if (low > 0) --low;
                ++high;
            }
            if (high < 0) return false;
        }
        return low == 0;
    }
};
```



类似题目：

[Valid Parentheses](http://www.cnblogs.com/grandyang/p/4424587.html)

[Special Binary String](http://www.cnblogs.com/grandyang/p/8606024.html)



参考资料：

[https://leetcode.com/problems/valid-parenthesis-string/](https://leetcode.com/problems/valid-parenthesis-string/)

[https://leetcode.com/problems/valid-parenthesis-string/discuss/107566/Java-12-lines-solution-backtracking](https://leetcode.com/problems/valid-parenthesis-string/discuss/107566/Java-12-lines-solution-backtracking)

[https://leetcode.com/problems/valid-parenthesis-string/discuss/139759/Java-Very-easy-solution.-No-recursion-dp.](https://leetcode.com/problems/valid-parenthesis-string/discuss/139759/Java-Very-easy-solution.-No-recursion-dp.)

[https://leetcode.com/problems/valid-parenthesis-string/discuss/107577/Short-Java-O(n)-time-O(1)-space-one-pass](https://leetcode.com/problems/valid-parenthesis-string/discuss/107577/Short-Java-O(n)-time-O(1)-space-one-pass)

[https://leetcode.com/problems/valid-parenthesis-string/discuss/107572/Java-using-2-stacks.-O(n)-space-and-time-complexity.](https://leetcode.com/problems/valid-parenthesis-string/discuss/107572/Java-using-2-stacks.-O(n)-space-and-time-complexity.)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












