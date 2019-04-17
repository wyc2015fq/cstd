# [LeetCode] Special Binary String 特殊的二进制字符串 - Grandyang - 博客园







# [[LeetCode] Special Binary String 特殊的二进制字符串](https://www.cnblogs.com/grandyang/p/8606024.html)







*Special* binary strings are binary strings with the following two properties:


- The number of 0's is equal to the number of 1's.
- Every prefix of the binary string has at least as many 1's as 0's.



Given a special string `S`, a *move* consists of choosing two consecutive, non-empty, special substrings of `S`, and swapping them. *(Two strings are consecutive if the last character of the first string is exactly one index before the first character of the second string.)*

At the end of any number of moves, what is the lexicographically largest resulting string possible?

Example 1:
Input: S = "11011000"
Output: "11100100"
Explanation:
The strings "10" [occuring at S[1]] and "1100" [at S[3]] are swapped.
This is the lexicographically largest string possible after some number of swaps.



Note:
- `S` has length at most `50`.
- `S` is guaranteed to be a *special* binary string as defined above.



这道题给了我们一个特殊的二进制字符串，说是需要满足两个要求，一是0和1的个数要相等，二是任何一个前缀中的1的个数都要大于等于0的个数。根据[压力山大大神的帖子](https://leetcode.com/problems/special-binary-string/discuss/113212/Think-of-it-as-Valid-Parentheses)，其实就是一个括号字符串啊。这里的1表示左括号，0表示右括号，那么题目中的两个限制条件其实就是限定这个括号字符串必须合法，即左右括号的个数必须相同，且左括号的个数随时都要大于等于右括号的个数，可以参见类似的题目[Valid Parenthesis String](http://www.cnblogs.com/grandyang/p/7617017.html)。那么这道题让我们通过交换子字符串，生成字母顺序最大的特殊字符串，注意这里交换的子字符串也必须是特殊字符串，满足题目中给定的两个条件，换作括号来说就是交换的子括号字符串也必须是合法的。那么我们来想什么样的字符串是字母顺序最大的呢，根据题目中的例子可以分析得出，应该是1靠前的越多越好，那么换作括号来说就是括号嵌套多的应该放在前面。比如我们分析题目中的例子:

11011000    ->    (()(()))

11100100    ->    ((())())

我们发现，题目中的例子中的交换操作其实是将上面的红色部分和蓝色部分交换了，因为蓝色的部分嵌套的括号多，那么左括号就多，在前面的1就多，所以字母顺序大。所以我们要做的就是将中间的子串分别提取出来，然后排序，再放回即可。上面的这个例子相对简单一些，实际上上面的红色和蓝色部分完全可以更复杂，所以再给它们排序之前，其自身的顺序应该已经按字母顺序排好了才行，这种特点天然适合递归的思路，先递归到最里层，然后一层一层向外扩展，直至完成所有的排序。

好，下面我们来看递归函数的具体写法，由于我们移动的子字符串也必须是合法的，那么我们利用检测括号字符串合法性的一个最常用的方法，就是遇到左括号加1，遇到右括号-1，这样得到0的时候，就是一个合法的子字符串了。我们用变量i来统计这个合法子字符串的起始位置，字符串数组v来保存这些合法的子字符串。好了，我们开始遍历字符串S，遇到1，cnt自增1，否则自减1。当cnt为0时，我们将这个字串加入v，注意前面说过，我们需要给这个字串自身也排序，所以我们要对自身调用递归函数，我们不用对整个子串调用递归，因为字串的起始位置和结束位置是确定的，一定是1和0，我们只需对中间的调用递归即可，然后更新i为j+1。当我们将所有排序后的合法字串存入v中后，我们对v进行排序，将字母顺序大的放前面，最后将其连为一个字符串即可，参见代码如下：



```
class Solution {
public:
    string makeLargestSpecial(string S) {
        int cnt = 0, i = 0;
        vector<string> v;
        string res = "";
        for (int j = 0; j < S.size(); ++j) {
            cnt += (S[j] == '1') ? 1 : -1;
            if (cnt == 0) {
                v.push_back('1' + makeLargestSpecial(S.substr(i + 1, j - i - 1)) + '0');
                i = j + 1;
            }
        }
        sort(v.begin(), v.end(), greater<string>());
        for (int i = 0; i < v.size(); ++i) res += v[i];
        return res;
    }
};
```



类似题目：

[Valid Parenthesis String](http://www.cnblogs.com/grandyang/p/7617017.html)



参考资料：

[https://leetcode.com/problems/special-binary-string/discuss/113681/7-line-c++](https://leetcode.com/problems/special-binary-string/discuss/113681/7-line-c++)

[https://leetcode.com/problems/special-binary-string/discuss/113212/Think-of-it-as-Valid-Parentheses](https://leetcode.com/problems/special-binary-string/discuss/113212/Think-of-it-as-Valid-Parentheses)

[https://leetcode.com/problems/special-binary-string/discuss/113211/Easy-and-Concise-Solution-with-Explanation-C++JavaPython](https://leetcode.com/problems/special-binary-string/discuss/113211/Easy-and-Concise-Solution-with-Explanation-C++JavaPython)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












