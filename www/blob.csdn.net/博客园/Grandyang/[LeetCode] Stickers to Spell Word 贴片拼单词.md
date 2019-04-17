# [LeetCode] Stickers to Spell Word 贴片拼单词 - Grandyang - 博客园







# [[LeetCode] Stickers to Spell Word 贴片拼单词](https://www.cnblogs.com/grandyang/p/8468045.html)







We are given N different types of stickers. Each sticker has a lowercase English word on it.

You would like to spell out the given `target` string by cutting individual letters from your collection of stickers and rearranging them.

You can use each sticker more than once if you want, and you have infinite quantities of each sticker.

What is the minimum number of stickers that you need to spell out the `target`? If the task is impossible, return -1.

Example 1:

Input:
["with", "example", "science"], "thehat"



Output:
3



Explanation:
We can use 2 "with" stickers, and 1 "example" sticker.
After cutting and rearrange the letters of those stickers, we can form the target "thehat".
Also, this is the minimum number of stickers necessary to form the target string.



Example 2:

Input:
["notice", "possible"], "basicbasic"



Output:
-1



Explanation:
We can't form the target "basicbasic" from cutting letters from the given stickers.



Note:
- `stickers` has length in the range `[1, 50]`.
- `stickers` consists of lowercase English words (without apostrophes).
- `target` has length in the range `[1, 15]`, and consists of lowercase English letters.
- In all test cases, all words were chosen randomly from the 1000 most common US English words, and the target was chosen as a concatenation of two random words.
- The time limit may be more challenging than usual. It is expected that a 50 sticker test case can be solved within 35ms on average.



这道题给了我们N个贴片，每个贴片上有一个小写字母的单词，给了我们一个目标单词target，让我们通过剪下贴片单词上的字母来拼出目标值，每个贴片都有无数个，问我们最少用几个贴片能拼出目标值target，如果不能拼出来的话，就返回-1。这道题博主最开始尝试用贪婪算法，结果发现不行，有网友留言提示说是多重背包问题，然后去论坛上看大神们的解法，果然都是用DP做的，之前曾有网友推荐过一个“背包九讲”的帖子，大概扫过几眼，真是叼到飞起啊，博主希望有时间也能总结一下。先来看这道题吧，既然是用DP来做，那么就需要用dp数组了，我们使用一个一维的dp数组，其中dp[i]表示组成第i个子集合所需要的最少的sticker的个数，注意这里是子集合，而不是子串。长度为n的字符串共有2的n次方个子集合，比如字符串"ab"，就有4个子集合，分别是 "", "a", "b", "ab"。字符串"abc"就有8个子集合，如果我们用0到7来分别对应其子集合，就有：

```
abc   subset 
0    000     ""
1    001     c
2    010     b
3    011     bc
4    100     a
5    101     ac
6    110     bb
7    111     abc
```

我们可以看到0到7的二进制数的每一位上的0和1就相当于mask，是1的话就选上该位对应的字母，000就表示都不选，就是空集，111就表示都选，就是abc，那么只要我们将所有子集合的dp值都算出来，最后返回dp数组的最后一个位置上的数字，就是和目标子串相等的子集合啦。我们以下面这个简单的例子来讲解：

stickers = ["ab", "bc", "ac"], target = "abc"

之前说了abc的共有8个子集合，所以dp数组的长度为8，除了dp[0]初始化为0之外，其余的都初始化为INT_MAX，然后我们开始逐个更新dp数组的值，我们的目标是从sticker中取出字符，来拼出子集合，所以如果当前遍历到的dp值仍为INT_MAX的话，说明该子集合无法被拼出来，自然我们也无法再其基础上去拼别打子集合了，直接跳过。否则我们就来遍历所有的sticker，让变量cur等于i，说明此时是在第i个子集合的基础上去reach其他的子集合，我们遍历当前sticker的每一个字母，对于sticker的每个字母，我们都扫描一遍target的所有字符，如果target里有这个字符，且该字符未出现在当前cur位置的子集合中，则将该字符加入子集合中。什么意思呢，比如当前我们的cur是3，二进制为011，对应的子集合是"bc"，此时如果我们遍历到的sticker是"ab"，那么遇到"a"时，我们知道target中是有"a"的，然后我们看"bc"中包不包括"a"，判断方法是看 (cur >> k) & 1 是否为0，这可以乍看上去不太好理解，其实不难想，因为我们的子集合是跟二进制对应的，"bc"就对应着011，第一个0就表示"a"缺失，所以我们想看哪个字符，就提取出该字符对应的二进制位，提取方法就是 cur >> k，表示cur向右移动k位，懂位操作Bit Manipulation的童鞋应该不难理解，提出出来的值与上1就知道该位是0还是1了，如果是0，表示缺失，那么把该位变为1，通过 cur |= 1 << k来实现，那么此时我们的cur就变位7，二进制为111，对应的子集合是"abc"，更新此时的dp[cur]为 dp[cur] 和 dp[i] + 1 中的较大值即可，最后循环结束后，如果"abc"对应的dp值还是INT_MAX，就返回-1，否则返回其dp值，参见代码如下：



解法一：

```
class Solution {
public:
    int minStickers(vector<string>& stickers, string target) {
        int n = target.size(), m = 1 << n;
        vector<int> dp(m, INT_MAX);
        dp[0] = 0;
        for (int i = 0; i < m; ++i) {
            if (dp[i] == INT_MAX) continue;
            for (string &sticker : stickers) {
                int cur = i;
                for (char c : sticker) {
                    for (int k = 0; k < n; ++k) {
                        if (target[k] == c && !((cur >> k) & 1)) {
                            cur |= 1 << k;
                            break;
                        }
                    }
                }
                dp[cur] = min(dp[cur], dp[i] + 1);
            }
        }
        return dp[m - 1] == INT_MAX ? -1 : dp[m - 1];
    }
};
```



下面这种解法是带记忆数组memo的递归解法，可以看作是DP解法的递归形式，核心思想都一样。只不过dp数组换成了哈希Map，建立子集合跟最小使用的sticker的个数之间的映射，初始化空集为0，我们首先统计每个sticker的各个字母出现的频率，放到对应的二维数组freq中，然后就是调用递归函数。在递归函数中，首先判断，如果target已经在memo中，直接返回其值。否则我们开始计算，首先统计出此时的target字符串的各个字母出现次数，然后我们遍历统计所有sticker中各个字母出现次数的数组freq，如果target字符串的第一个字母不在当前sticker中，我们直接跳过，注意递归函数中的target字符串不是原始的字符串，我们心间一个临时字符串t，然后我们遍历target字符串中存在的字符，如果target中的某字符存在的个数多于sticker中对应的字符，那么将多余的部分存在字符串t中，表示当前sticker无法拼出的字符，交给下一个递归函数来处理，我们看再次调用递归函数的结果ans，如果不为-1，说明可以拼出剩余的那些字符，那么此时我们的res更新为ans+1，循环退出后，此时我们的res就应该是组成当前递归函数中的target串的最少贴片数，更新dp[target]值，如果res是INT_MAX，说明无法拼出，更新为-1，否则更新为res，参见代码如下：



解法二：

```
class Solution {
public:
    int minStickers(vector<string>& stickers, string target) {
        int N = stickers.size();
        vector<vector<int>> freq(N, vector<int>(26, 0));
        unordered_map<string, int> memo;
        memo[""] = 0;
        for (int i = 0; i < N; ++i) {
            for (char c : stickers[i]) ++freq[i][c - 'a'];
        }
        return helper(freq, target, memo);
    }
    int helper(vector<vector<int>>& freq, string target, unordered_map<string, int>& memo) {
        if (memo.count(target)) return memo[target];
        int res = INT_MAX, N = freq.size();
        vector<int> cnt(26, 0);
        for (char c : target) ++cnt[c - 'a'];
        for (int i = 0; i < N; ++i) {
            if (freq[i][target[0] - 'a'] == 0) continue;
            string t = "";
            for (int j = 0; j < 26; ++j) {
                if (cnt[j] - freq[i][j] > 0) t += string(cnt[j] - freq[i][j], 'a' + j);
            }
            int ans = helper(freq, t, memo);
            if (ans != -1) res = min(res, ans + 1);
        }
        memo[target] = (res == INT_MAX) ? -1 : res;
        return memo[target];
    }
};
```



类似题目：

[Ransom Note](http://www.cnblogs.com/grandyang/p/5764314.html)



参考资料：

[https://leetcode.com/problems/stickers-to-spell-word/discuss/108333/Rewrite-of-contest-winner's-solution](https://leetcode.com/problems/stickers-to-spell-word/discuss/108333/Rewrite-of-contest-winner's-solution)

[https://leetcode.com/problems/stickers-to-spell-word/discuss/108318/C++JavaPython-DP-+-Memoization-with-optimization-29-ms-(C++)](https://leetcode.com/problems/stickers-to-spell-word/discuss/108318/C++JavaPython-DP-+-Memoization-with-optimization-29-ms-(C++))

[https://leetcode.com/problems/stickers-to-spell-word/discuss/108334/Explaining-StefanPochmann's-Rewrite-of-contest-winner's-solution-and-+java](https://leetcode.com/problems/stickers-to-spell-word/discuss/108334/Explaining-StefanPochmann's-Rewrite-of-contest-winner's-solution-and-+java)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












