# [LeetCode] Word Break 拆分词句 - Grandyang - 博客园







# [[LeetCode] Word Break 拆分词句](https://www.cnblogs.com/grandyang/p/4257740.html)







Given a non-empty string *s* and a dictionary *wordDict* containing a list of non-empty words, determine if *s* can be segmented into a space-separated sequence of one or more dictionary words.

Note:
- The same word in the dictionary may be reused multiple times in the segmentation.
- You may assume the dictionary does not contain duplicate words.

Example 1:
Input: s = "leetcode", wordDict = ["leet", "code"]
Output: true
Explanation: Return true because `"leetcode"` can be segmented as `"leet code"`.

Example 2:
Input: s = "applepenapple", wordDict = ["apple", "pen"]
Output: true
Explanation: Return true because `"`applepenapple`"` can be segmented as `"`apple pen apple`"`.
             Note that you are allowed to reuse a dictionary word.

Example 3:
Input: s = "catsandog", wordDict = ["cats", "dog", "sand", "and", "cat"]
Output: false


这道拆分词句问题是看给定的词句能分被拆分成字典里面的内容，这是一道很经典的题目，解法不止一种，考察的范围很广，属于我们必须要熟练掌握的题目。那么先来想brute force的解法，就拿例子1来分析，如果字典中只有两个单词，我们怎么去判断，是不是可以将原字符串s分成任意两段，然后再看分成的单词是否在字典中。注意这道题说是单词可以重复使用，所以可以分成任意段，而且字典中的单词可以有很多个，这就增加了题目的难度，很多童鞋就在这里迷失了，毫无头绪。那么，就由博主来给各位指点迷津吧（此处应有掌声👏）。

既然要分段，看子字符串是否在字典中，由于给定的字典是数组（之前还是HashSet呢），那么我们肯定不希望每次查找都需要遍历一遍数组，费劲！还是把字典中的所有单词都存入HashSet中吧，这样我们就有了常数时间级的查找速度，perfect！好，我们得开始给字符串分段了，怎么分，只能一个一个分了，先看第一个字母是否在字典中，如果不在的话，好办，说明这种分法肯定是错的。问题是在的话，后面的那部分怎么处理，难道还用for循环？你也不知道还要分多少段，怎么用for循环。对于这种不知道怎么处理的情况，一个万能的做法是丢给递归函数，让其去递归求解，这里我们suppose递归函数会返回我们一个正确的值，如果返回的是true的话，表明我们现在分成的两段都在字典中，我们直接返回true即可，因为只要找出一种情况就行了。这种调用递归函数的方法就是brute force的解法，我们遍历了所有的情况，优点是写法简洁，思路清晰，缺点是存在大量的重复计算，被OJ啪啪打脸。所以我们需要进行优化，使用记忆数组memo来保存所有已经计算过的结果，再下次遇到的时候，直接从cache中取，而不是再次计算一遍。这种使用记忆数组memo的递归写法，和使用dp数组的迭代写法，乃解题的两大神器，凡事能用dp解的题，一般也有用记忆数组的递归解法，好似一对形影不离的好基友～关于dp解法，博主会在下文中讲解。这里我们的记忆数组memo[i]定义为范围为[0, i)的子字符串是否可以拆分，初始化为-1，表示没有计算过，如果可以拆分，则赋值为1，反之为0。在之前讲brute force解法时，博主提到的是讲分成两段的后半段的调用递归函数，我们也可以不取出子字符串，而是用一个start变量，来标记分段的位置，这样递归函数中只需要从start的位置往后遍历即可，在递归函数更新记忆数组memo即可，参见代码如下：



解法一：

```
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> wordSet(wordDict.begin(), wordDict.end());
        vector<int> memo(s.size(), -1);
        return check(s, wordSet, 0, memo);
    }
    bool check(string s, unordered_set<string>& wordSet, int start, vector<int>& memo) {
        if (start >= s.size()) return true;
        if (memo[start] != -1) return memo[start];
        for (int i = start + 1; i <= s.size(); ++i) {
            if (wordSet.count(s.substr(start, i - start)) && check(s, wordSet, i, memo)) {
                return memo[start] = 1;
            }
        }
        return memo[start] = 0;
    }
};
```



这道题其实还是一道经典的DP题目，也就是动态规划Dynamic Programming。博主曾经说玩子数组或者子字符串且求极值的题，基本就是DP没差了，虽然这道题没有求极值，但是玩子字符串也符合DP的状态转移的特点。把一个人的温暖转移到另一个人的胸膛... 咳咳，跑错片场了，那是爱情转移～ 强行拉回，DP解法的两大难点，定义dp数组跟找出状态转移方程，先来看dp数组的定义，这里我们就用一个一维的dp数组，其中dp[i]表示范围[0, i)内的子串是否可以拆分，注意这里dp数组的长度比s串的长度大1，是因为我们要handle空串的情况，我们初始化dp[0]为true，然后开始遍历。注意这里我们需要两个for循环来遍历，因为此时已经没有递归函数了，所以我们必须要遍历所有的子串，我们用j把[0, i)范围内的子串分为了两部分，[0, j) 和 [j, i)，其中范围 [0, j) 就是dp[j]，范围 [j, i) 就是s.substr(j, i-j)，其中dp[j]是之前的状态，我们已经算出来了，可以直接取，只需要在字典中查找s.substr(j, i-j)是否存在了，如果二者均为true，将dp[i]赋为true，并且break掉，此时就不需要再用j去分[0, i)范围了，因为[0, i)范围已经可以拆分了。最终我们返回dp数组的最后一个值，就是整个数组是否可以拆分的布尔值了，代码如下：



解法二：

```
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> wordSet(wordDict.begin(), wordDict.end());
        vector<bool> dp(s.size() + 1);
        dp[0] = true;
        for (int i = 0; i < dp.size(); ++i) {
            for (int j = 0; j < i; ++j) {
                if (dp[j] && wordSet.count(s.substr(j, i - j))) {
                    dp[i] = true;
                    break;
                }
            }
        }
        return dp.back();
    }
};
```



下面我们从题目中给的例子来分析：

l 

le e 

lee ee e 

**leet**

leetc eetc etc tc c 

leetco eetco etco tco co o 

leetcod eetcod etcod tcod cod od d 

leetcode eetcode etcode tcode **code **

T F F F T F F F T 

我们知道算法的核心思想是逐行扫描，每一行再逐个字符扫描，每次都在组合出一个新的字符串都要到字典里去找，如果有的话，则跳过此行，继续扫描下一行。



既然DFS都可以解题，那么BFS也就坐不住了，也要出来蹦跶一下。其实本质跟递归的解法没有太大的区别，递归解法在调用递归的时候，原先的状态被存入了栈中，这里BFS是存入了队列中，使用visited数组来标记已经算过的位置，作用跟memo数组一样，从队列中取出一个位置进行遍历，把可以拆分的新位置存入队列中，遍历完成后标记当前位置，然后再到队列中去取即可，参见代码如下：



解法三：

```
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> wordSet(wordDict.begin(), wordDict.end());
        vector<bool> visited(s.size());
        queue<int> q{{0}};
        while (!q.empty()) {
            int start = q.front(); q.pop();
            if (!visited[start]) {
                for (int i = start + 1; i <= s.size(); ++i) {
                    if (wordSet.count(s.substr(start, i - start))) {
                        q.push(i);
                        if (i == s.size()) return true;
                    }
                }
                visited[start] = true;
            }
        }
        return false;
    }
};
```



参考资料：

[https://leetcode.com/problems/word-break/description/](https://leetcode.com/problems/word-break/description/)

[https://leetcode.com/problems/word-break/solution/](https://leetcode.com/problems/word-break/solution/)

[https://leetcode.com/problems/word-break/discuss/43797/A-solution-using-BFS](https://leetcode.com/problems/word-break/discuss/43797/A-solution-using-BFS)

[https://leetcode.com/problems/word-break/discuss/43886/Evolve-from-brute-force-to-optimal-a-review-of-all-solutions](https://leetcode.com/problems/word-break/discuss/43886/Evolve-from-brute-force-to-optimal-a-review-of-all-solutions)



类似题目：

[Word Break II](http://www.cnblogs.com/grandyang/p/4576240.html)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












