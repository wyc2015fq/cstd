# [LeetCode] Split Array into Fibonacci Sequence 分割数组成斐波那契序列 - Grandyang - 博客园







# [[LeetCode] Split Array into Fibonacci Sequence 分割数组成斐波那契序列](https://www.cnblogs.com/grandyang/p/10434771.html)







Given a string `S` of digits, such as `S = "123456579"`, we can split it into a *Fibonacci-like sequence*`[123, 456, 579].`

Formally, a Fibonacci-like sequence is a list `F` of non-negative integers such that:
- `0 <= F[i] <= 2^31 - 1`, (that is, each integer fits a 32-bit signed integer type);
- `F.length >= 3`;
- and` F[i] + F[i+1] = F[i+2] `for all `0 <= i < F.length - 2`.

Also, note that when splitting the string into pieces, each piece must not have extra leading zeroes, except if the piece is the number 0 itself.

Return any Fibonacci-like sequence split from `S`, or return `[]` if it cannot be done.

Example 1:
Input: "123456579"
Output: [123,456,579]

Example 2:
Input: "11235813"
Output: [1,1,2,3,5,8,13]

Example 3:
Input: "112358130"
Output: []
Explanation: The task is impossible.

Example 4:
Input: "0123"
Output: []
Explanation: Leading zeroes are not allowed, so "01", "2", "3" is not valid.

Example 5:
Input: "1101111"
Output: [110, 1, 111]
Explanation: The output [11, 0, 11, 11] would also be accepted.

Note:
- `1 <= S.length <= 200`
- `S` contains only digits.



这道题给了我们一个字符串，让我们分割成斐波那契序列，至少要分成三个数，并且满足斐波那契数列的性质。关于其性质，博主有个口诀可以快速记忆，那就是大学食堂里今天的汤是昨天的汤加上前天的汤。题目中给的例子挺多的，便于理解题意，其中例子4还强调了不能有leading zeros。但是关于overflow的test case却只字未提，害的博主fail了N多次，才最终handle了所有的溢出的错误。由例子5我们可以看出，符合题意的数列其实可能不止一种，但是本题就让返回一个就行了。不管返回几个，总之不是求极值，DP在这里就不好使了，只能用递归了，由于不知道如何分割，所以肯定需要遍历所有的情况。我们用一个数组out来记录已经组成的序列，用结果res来保存结果。当out数组的个数大于等于3，并且已经遍历完了字符串S，那么此时就是可以把out数组中的内存赋值给结果res了，那么之后只要检测结果res不为空时，直接返回就可以了，这是个很好的剪枝操作，因为此题只需要一个正确答案即可（返回所有情况将作为follow up在本文的底部讨论）。

现在来考虑递归函数的主体该怎么写，既然不知道要如何分割，那么就要尝试所有的情况，一个数字，两个数字，一直到末尾，那么就可以遍历字符串S，然后取子串即可。但从什么位置开始呢，每次都从头吗，这道题都数字不能重复使用，所以应该用个变量start来记录当前遍历到的位置，那么我们从start位置起，每次取 i-start+1 长度的子串 cur，此时在转为int之前，需要先处理leading zeros的情况，判断若cur长度大于1，且首字符为0，直接break，还就是若cur的长度大于10，也break，为啥呢？因为整型的最大值是 2147483647，只有10位，所以当cur长度大于10时，一定会溢出。当cur长度为10时，也有可能溢出，这个在之后处理。好，现在将cur转为长整型 long，因为长度为10也可能溢出，所以要先转为长整型，然后在判断若大于整型最大值 INT_MAX，直接break。接下来就要考虑是否要加入out数组了，当out数字的个数不到2个的时候，我们可以直接加入当前数字，若大于等于2个，需要考虑是否满足斐波纳切数列的性质，即当前数字是否等于前两个数字之和，满足的话才加入，不然就跳过，注意这里不能直接break，因为之后的数字也许可能满足要求。加入out数组之后，就可以调用递归了，此时起始位置传入 i+1，之后再恢复out的状态即可，参见代码如下：



```
class Solution {
public:
    vector<int> splitIntoFibonacci(string S) {
        vector<int> res, out;
        helper(S, 0, out, res);
        return res;
    }
    void helper(string& S, int start, vector<int>& out, vector<int>& res) {
        if (!res.empty()) return;
        if (start >= S.size() && out.size() >= 3) {
            res = out; return;
        }
        for (int i = start; i < S.size(); ++i) {
            string cur = S.substr(start, i - start + 1);
            if ((cur.size() > 1 && cur[0] == '0') || cur.size() > 10) break;
            long num = stol(cur), len = out.size();
            if (num > INT_MAX) break;
            if (out.size() >= 2 && num != (long)out[len - 1] + out[len - 2]) continue;
            out.push_back(num);
            helper(S, i + 1, out, res);
            out.pop_back();
        }
    }
};
```



讨论：这道题只让我们返回了一个斐波那契数列，一个很好的follow up就是返回所有满足题意的序列，就像例子5一样，把两种符合题意的组合都返回出来。其实改起来相当的容易，只需要将结果res换成一个二维数组来保存所有的情况，然后在递归函数中，首先判断如果已经遍历到了S的末尾，并且out数组中的个数大于等于3了，那么将out数组加入结果res即可，其余部分和上面的解法并没有啥区别，代码参见评论区一楼。



类似题目：

[Additive Number](http://www.cnblogs.com/grandyang/p/4974115.html)

[Fibonacci Number](https://www.cnblogs.com/grandyang/p/10306787.html)



参考资料：

[https://leetcode.com/problems/split-array-into-fibonacci-sequence/](https://leetcode.com/problems/split-array-into-fibonacci-sequence/)

[https://leetcode.com/problems/split-array-into-fibonacci-sequence/discuss/133936/short-and-fast-backtracking-solution](https://leetcode.com/problems/split-array-into-fibonacci-sequence/discuss/133936/short-and-fast-backtracking-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












