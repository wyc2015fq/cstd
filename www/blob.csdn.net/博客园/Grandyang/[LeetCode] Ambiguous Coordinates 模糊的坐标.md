# [LeetCode] Ambiguous Coordinates 模糊的坐标 - Grandyang - 博客园







# [[LeetCode] Ambiguous Coordinates 模糊的坐标](https://www.cnblogs.com/grandyang/p/9563418.html)







We had some 2-dimensional coordinates, like `"(1, 3)"` or `"(2, 0.5)"`.  Then, we removed all commas, decimal points, and spaces, and ended up with the string `S`.  Return a list of strings representing all possibilities for what our original coordinates could have been.

Our original representation never had extraneous zeroes, so we never started with numbers like "00", "0.0", "0.00", "1.0", "001", "00.01", or any other number that can be represented with less digits.  Also, a decimal point within a number never occurs without at least one digit occuring before it, so we never started with numbers like ".1".

The final answer list can be returned in any order.  Also note that all coordinates in the final answer have exactly one space between them (occurring after the comma.)
Example 1:
Input: "(123)"
Output: ["(1, 23)", "(12, 3)", "(1.2, 3)", "(1, 2.3)"]
Example 2:
Input: "(00011)"
Output:  ["(0.001, 1)", "(0, 0.011)"]
Explanation: 
0.0, 00, 0001 or 00.01 are not allowed.
Example 3:
Input: "(0123)"
Output: ["(0, 123)", "(0, 12.3)", "(0, 1.23)", "(0.1, 23)", "(0.1, 2.3)", "(0.12, 3)"]
Example 4:
Input: "(100)"
Output: [(10, 0)]
Explanation: 
1.0 is not allowed.



Note:
- `4 <= S.length <= 12`.
- `S[0]` = "(", `S[S.length - 1]` = ")", and the other elements in `S` are digits.



这道题给了我们一个模糊坐标，括号里面很只有一个数字字符串，没有逗号也没有小数点，让我们自己添加逗号和小数点，让把所有可能的组合都返回。题目中给了很多例子，理解起题意来很容易。这道题的难点是如何合理的拆分，很多拆分是不合法的，题目举了很多不合法的例子，比如 "00", "0.0", "0.00", "1.0", "001", "00.01"。那么我们需要归纳出所有不合法的corner case，然后剩下一般情况比如123，我们就按位加小数点即可。那么我们再来看一下那些非法的例子，我们发现一眼望去好多0，不错，0就是trouble maker，首先不能有0开头的长度大于1的整数，比如00， 001。其次，不能有0结尾的小数，比如0.0，0.00，1.0等。还有，小数的整数位上也不能有0开头的长度大于1的整数。那么我们来归纳一下吧，首先如果字符串为空，那么直接返回空集合。然后如果字符串长度大于1，且首尾字符都是0的话，那么不可分，比如 0xxx0，因为整数长度大于1的话不能以0开头，中间也没法加小数点，因为小数最后一位不能是0。如果长度大于1，第一位是0，但最后一位不是0，那我们可以在第一个0后面加个小数点返回，这时就必须要加小数点了，因为长度大于1的整数不能以0开头。再之后，如果最后一位是0，说明不能加小数点，直接把当前值返回即可。最后就是一般情况了，我们先把原数加入结果res，然后遍历中间的每个位置，都加个小数点，所有情况归纳如下：

if S == "": return []
if S == "0": return [S]
if S == "0XXX0": return []
if S == "0XXX": return ["0.XXX"]
if S == "XXX0": return [S]
return [S, "X.XXX", "XX.XX", "XXX.X"...]



```
class Solution {
public:
    vector<string> ambiguousCoordinates(string S) {
        vector<string> res;
        int n = S.size();
        for (int i = 1; i < n - 2; ++i) {
            vector<string> A = findAll(S.substr(1, i)), B = findAll(S.substr(i + 1, n - 2 - i));
            for (auto &a : A) {
                for (auto &b : B) {
                    res.push_back("(" + a + ", " + b + ")");
                }
            }
        }
        return res;
    }
    vector<string> findAll(string S) {
        int n = S.size();
        if (n == 0 || (n > 1 && S[0] == '0' && S[n - 1] == '0')) return {};
        if (n > 1 && S[0] == '0') return {"0." + S.substr(1)};
        if (S[n - 1] == '0') return {S};
        vector<string> res{S};
        for (int i = 1; i < n; ++i) res.push_back(S.substr(0, i) + "." + S.substr(i));
        return res;
    }
};
```



参考资料：

[https://leetcode.com/problems/ambiguous-coordinates/](https://leetcode.com/problems/ambiguous-coordinates/)

[https://leetcode.com/problems/ambiguous-coordinates/discuss/123851/C%2B%2BJavaPython-Solution-with-Explanation](https://leetcode.com/problems/ambiguous-coordinates/discuss/123851/C%2B%2BJavaPython-Solution-with-Explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












