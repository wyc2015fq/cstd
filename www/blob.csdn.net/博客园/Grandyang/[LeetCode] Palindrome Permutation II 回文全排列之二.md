# [LeetCode] Palindrome Permutation II 回文全排列之二 - Grandyang - 博客园







# [[LeetCode] Palindrome Permutation II 回文全排列之二](https://www.cnblogs.com/grandyang/p/5315227.html)







Given a string `s`, return all the palindromic permutations (without duplicates) of it. Return an empty list if no palindromic permutation could be form.

For example:

Given `s = "aabb"`, return `["abba", "baab"]`.

Given `s = "abc"`, return `[]`.

Hint:
- If a palindromic permutation exists, we just need to generate the first half of the string.
- To generate all distinct permutations of a (half of) string, use a similar approach from: [Permutations II](https://leetcode.com/problems/permutations-ii) or [Next Permutation](https://leetcode.com/problems/next-permutation).



这道题是之前那道[Palindrome Permutation](http://www.cnblogs.com/grandyang/p/5223238.html)的拓展，那道题只是让判断存不存在回文全排列，而这题让我们返回所有的回文全排列，此题给了我们充分的提示：如果回文全排列存在，我们只需要生成前半段字符串即可，后面的直接根据前半段得到。那么我们再进一步思考，由于回文字符串有奇偶两种情况，偶数回文串例如abba，可以平均分成前后半段，而奇数回文串例如abcba，需要分成前中后三段，需要注意的是中间部分只能是一个字符，那么我们可以分析得出，如果一个字符串的回文字符串要存在，那么奇数个的字符只能有0个或1个，其余的必须是偶数个，所以我们可以用哈希表来记录所有字符的出现个数，然后我们找出出现奇数次数的字符加入mid中，如果有两个或两个以上的奇数个数的字符，那么返回空集，我们对于每个字符，不管其奇偶，都将其个数除以2的个数的字符加入t中，这样做的原因是如果是偶数个，那么将其一般加入t中，如果是奇数，如果有1个，那么除以2是0，不会有字符加入t，如果是3个，那么除以2是1，取一个加入t。等我们获得了t之后，t是就是前半段字符，我们对其做全排列，每得到一个全排列，我们加上mid和该全排列的逆序列就是一种所求的回文字符串，这样我们就可以得到所有的回文全排列了。在全排序的子函数中有一点需要注意的是，如果我们直接用数组来保存结果时，并且t中如果有重复字符的话可能会出现重复项，比如 t = "baa" 的话，那么最终生成的结果会有重复项，不信可以自己尝试一下。这里简单的说明一下，当 start=0，i=1 时，我们交换后得到 aba，在之后当 start=1，i=2 时，交换后可以得到 aab。但是在之后回到第一层当baa后，当 start=0，i=2 时，交换后又得到了 aab，重复就产生了。那么其实最简单当去重复的方法就是将结果res定义成HashSet，利用其去重复的特性，可以保证我们得到的是没有重复的，参见代码如下：



解法一：

```
class Solution {
public:
    vector<string> generatePalindromes(string s) {
        unordered_set<string> res;
        unordered_map<char, int> m;
        string t = "", mid = "";
        for (auto a : s) ++m[a];
        for (auto it : m) {
            if (it.second % 2 == 1) mid += it.first;
            t += string(it.second / 2, it.first);
            if (mid.size() > 1) return {};
        }
        permute(t, 0, mid, res);
        return vector<string>(res.begin(), res.end());
    }
    void permute(string &t, int start, string mid, unordered_set<string> &res) {
        if (start >= t.size()) {
            res.insert(t + mid + string(t.rbegin(), t.rend()));
        } 
        for (int i = start; i < t.size(); ++i) {
            if (i != start && t[i] == t[start]) continue;
            swap(t[i], t[start]);
            permute(t, start + 1, mid, res);
            swap(t[i], t[start]);
        }
    }
};
```



下面这种方法和上面的方法很相似，不同之处来于求全排列的方法略有不同，上面那种方法是通过交换字符的位置来生成不同的字符串，而下面这种方法是通过加不同的字符来生成全排列字符串，参见代码如下：



解法二：

```
class Solution {
public:
    vector<string> generatePalindromes(string s) {
        vector<string> res;
        unordered_map<char, int> m;
        string t = "", mid = "";
        for (auto a : s) ++m[a];
        for (auto &it : m) {
            if (it.second % 2 == 1) mid += it.first;
            it.second /= 2;
            t += string(it.second, it.first);
            if (mid.size() > 1) return {};
        }
        permute(t, m, mid, "", res);
        return res;
    }
    void permute(string &t, unordered_map<char, int> &m, string mid, string out, vector<string> &res) {
        if (out.size() >= t.size()) {
            res.push_back(out + mid + string(out.rbegin(), out.rend()));
            return;
        } 
        for (auto &it : m) {
            if (it.second > 0) {
                --it.second;
                permute(t, m, mid, out + it.first, res);
                ++it.second;
            }
        }
    }
};
```



在来看一种利用了std提供的next_permutation函数来实现的方法，这样就大大减轻了我们的工作量，但是这种方法个人感觉算是有些投机取巧了，不知道面试的时候面试官允不允许这样做，贴上来拓宽一下思路也是好的：



解法三：

```
class Solution {
public:
    vector<string> generatePalindromes(string s) {
        vector<string> res;
        unordered_map<char, int> m;
        string t = "", mid = "";
        for (auto a : s) ++m[a];
        for (auto it : m) {
            if (it.second % 2 == 1) mid += it.first;
            t += string(it.second / 2, it.first);
            if (mid.size() > 1) return {};
        }
        sort(t.begin(), t.end());
        do {
            res.push_back(t + mid + string(t.rbegin(), t.rend()));
        } while (next_permutation(t.begin(), t.end()));
        return res;
    }
};
```



类似题目：

[Next Permutation](http://www.cnblogs.com/grandyang/p/4428207.html)

[Palindrome Permutation](http://www.cnblogs.com/grandyang/p/5223238.html)

[Permutations II](http://www.cnblogs.com/grandyang/p/4359825.html)

[Permutations](http://www.cnblogs.com/grandyang/p/4358848.html)



参考资料：

[https://leetcode.com/problems/palindrome-permutation-ii/](https://leetcode.com/problems/palindrome-permutation-ii/)

[https://leetcode.com/problems/palindrome-permutation-ii/discuss/69696/AC-Java-solution-with-explanation](https://leetcode.com/problems/palindrome-permutation-ii/discuss/69696/AC-Java-solution-with-explanation)

[https://leetcode.com/problems/palindrome-permutation-ii/discuss/69698/Short-backtracking-solution-in-Java-(3-ms)](https://leetcode.com/problems/palindrome-permutation-ii/discuss/69698/Short-backtracking-solution-in-Java-(3-ms))

[https://leetcode.com/problems/palindrome-permutation-ii/discuss/69767/22-lines-0ms-C%2B%2B-easy-to-understand](https://leetcode.com/problems/palindrome-permutation-ii/discuss/69767/22-lines-0ms-C%2B%2B-easy-to-understand)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












