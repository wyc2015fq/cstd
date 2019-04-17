# [LeetCode] Backspace String Compare 退格字符串比较 - Grandyang - 博客园







# [[LeetCode] Backspace String Compare 退格字符串比较](https://www.cnblogs.com/grandyang/p/10447783.html)







Given two strings `S` and `T`, return if they are equal when both are typed into empty text editors. `#` means a backspace character.


Example 1:
Input: S = "ab#c", T = "ad#c"
Output: true
Explanation: Both S and T become "ac".


Example 2:
Input: S = "ab##", T = "c#d#"
Output: true
Explanation: Both S and T become "".


Example 3:
Input: S = "a##c", T = "#a#c"
Output: true
Explanation: Both S and T become "c".


Example 4:
Input: S = "a#c", T = "b"
Output: false
Explanation: S becomes "c" while T becomes "b".

Note:
- `1 <= S.length <= 200`
- `1 <= T.length <= 200`
- `S` and `T` only contain lowercase letters and `'#'` characters.

Follow up:
- Can you solve it in `O(N)` time and `O(1)`space?







这道题给了我们两个字符串，里面可能会有井号符#，这个表示退格符，键盘上的退格键我们应该都很熟悉吧，当字打错了的时候，肯定要点退格键来删除的。当然也可以连续点好几下退格键，这样就可以连续删除了，在例子2和3中，也确实能看到连续的井号符。题目搞懂了之后，就开始解题吧，博主最先想的方法就是对S和T串分别处理完退格操作后再进行比较，那么就可以使用一个子函数来进行字符串的退格处理，在子函数中，我们新建一个结果res的空串，然后遍历输入字符串，当遇到退格符的时候，我们判断若结果res不为空，则将最后一个字母去掉；若遇到的是字母，则直接加入结果res中即可。这样S和T串同时处理完了之后，再进行比较即可，参见代码如下：



解法一：

```
class Solution {
public:
    bool backspaceCompare(string S, string T) {
        return helper(S) == helper(T);      
    }
    string helper(string str) {
        string res = "";
        for (char c : str) {
            if (c == '#') {
                if (!res.empty()) res.pop_back();
            } else {
                res.push_back(c);
            }
        }
        return res;
    }
};
```



我们也可以不使用单独的子函数，而是直接用for循环来处理S和T串，当然原理都是一样的，分别建立s和t的空串，然后进行退格操作，最后比较s和t串是否相等即可，参见代码如下：



解法二：

```
class Solution {
public:
    bool backspaceCompare(string S, string T) {
        string s = "", t = "";
        for (char c : S) c == '#' ? s.size() > 0 ? s.pop_back() : void() : s.push_back(c);
        for (char c : T) c == '#' ? t.size() > 0 ? t.pop_back() : void() : t.push_back(c);
        return s == t;
    }
};
```



这道题的follow up让我们使用常数级的空间复杂度，就是说不能新建空的字符串来保存处理之后的结果，那么我们只能在遍历的过程中同时进行比较，这样只能使用双指针同时遍历S和T串了。我们采用从后往前遍历，因为退格是要删除前面的字符，所以倒序遍历要好一些。用变量i和j分别指向S和T串的最后一个字符的位置，然后还需要两个变量cnt1和cnt2来分别记录S和T串遍历过程中连续出现的井号的个数，因为在连续井号后，要连续删除前面的字母，如何知道当前的字母是否是需要删除，就要知道当前还没处理的退格符的个数。好，现在进行while循环，条件是i和j至少有一个要大于等于0，然后对S串进行另一个while循环，条件是当i大于等于0，且当前字符是井号，或者cnt1大于0，那么若当前字符是退格符，则cnt1自增1，否则cnt1自减1，然后i自减1，这样就相当于跳过了当前的字符，不用进行比较。对T串也是做同样的while循环处理。之后若i和j有一个小于0了，那么可以根据i和j是否相等的情况进行返回。否则再看若S和T串当前的字母不相等，则返回false，因为当前位置的退格符已经处理完了，剩下的字母是需要比较相等的，若不相等就可以直接返回false了。最后当外层的while循环推出后，返回i和j是否相等，参见代码如下：



解法三：

```
class Solution {
public:
    bool backspaceCompare(string S, string T) {
        int i = (int)S.size() - 1, j = (int)T.size() - 1, cnt1 = 0, cnt2 = 0;
        while (i >= 0 || j >= 0) {
            while (i >= 0 && (S[i] == '#' || cnt1 > 0)) S[i--] == '#' ? ++cnt1 : --cnt1;
            while (j >= 0 && (T[j] == '#' || cnt2 > 0)) T[j--] == '#' ? ++cnt2 : --cnt2;
            if (i < 0 || j < 0) return i == j;
            if (S[i--] != T[j--]) return false;
        }
        return i == j;
    }
};
```



参考资料：

[https://leetcode.com/problems/backspace-string-compare/](https://leetcode.com/problems/backspace-string-compare/)

[https://leetcode.com/problems/backspace-string-compare/discuss/135603/C%2B%2BJavaPython-O(N)-time-and-O(1)-space](https://leetcode.com/problems/backspace-string-compare/discuss/135603/C%2B%2BJavaPython-O(N)-time-and-O(1)-space)

[https://leetcode.com/problems/backspace-string-compare/discuss/135873/8-lines-C%2B%2B-O(1)-space-easy-to-understand](https://leetcode.com/problems/backspace-string-compare/discuss/135873/8-lines-C%2B%2B-O(1)-space-easy-to-understand)



[LeetCode All in One 题目讲解汇总(持续更新中...)](https://www.cnblogs.com/grandyang/p/4606334.html)












