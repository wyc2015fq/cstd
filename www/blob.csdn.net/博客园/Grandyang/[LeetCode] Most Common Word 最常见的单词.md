# [LeetCode] Most Common Word 最常见的单词 - Grandyang - 博客园







# [[LeetCode] Most Common Word 最常见的单词](https://www.cnblogs.com/grandyang/p/10061930.html)







Given a paragraph and a list of banned words, return the most frequent word that is not in the list of banned words.  It is guaranteed there is at least one word that isn't banned, and that the answer is unique.

Words in the list of banned words are given in lowercase, and free of punctuation.  Words in the paragraph are not case sensitive.  The answer is in lowercase.



Example:
Input: 
paragraph = "Bob hit a ball, the hit BALL flew far after it was hit."
banned = ["hit"]
Output: "ball"
Explanation: 
"hit" occurs 3 times, but it is a banned word.
"ball" occurs twice (and no other word does), so it is the most frequent non-banned word in the paragraph. 
Note that words in the paragraph are not case sensitive,
that punctuation is ignored (even if adjacent to words, such as "ball,"), 
and that "hit" isn't the answer even though it occurs more because it is banned.



Note:
- `1 <= paragraph.length <= 1000`.
- `1 <= banned.length <= 100`.
- `1 <= banned[i].length <= 10`.
- The answer is unique, and written in lowercase (even if its occurrences in `paragraph` may have uppercase symbols, and even if it is a proper noun.)
- `paragraph` only consists of letters, spaces, or the punctuation symbols `!?',;.`
- There are no hyphens or hyphenated words.
- Words only consist of letters, never apostrophes or other punctuation symbols.



这道题给了我们一个字符串，是一个句子，里面有很多单词，并且还有标点符号，然后又给了我们一个类似黑名单功能的一个字符串数组，让我们在返回句子中出现的频率最高的一个单词。要求非常简单明了，那么思路也就简单粗暴一些吧。因为我们返回的单词不能是黑名单中的，所以我们对于每一个统计的单词肯定都需要去黑名单中检查，为了提高效率，我们可以把黑名单中所有的单词放到一个HashSet中，这样就可以常数级时间内查询了。然后要做的就是处理一下字符串数组，因为字符串中可能有标点符号，所以我们先过滤一遍字符串，这里我们使用了系统自带的两个函数isalpha()和tolower()函数，其实自己写也可以，就放到一个子函数处理一下也不难，这里就偷懒了，遍历每个字符，如果不是字母，就换成空格符号，如果是大写字母，就换成小写的。然后我们又使用一个C++中的读取字符串流的类，Java中可以直接调用split函数，叼的飞起。但谁让博主固执的写C++呢，也无所谓啦，习惯就好，这里我们也是按照空格拆分，将每个单词读出来，这里要使用一个mx变量，统计当前最大的频率，还需要一个HashMap来建立单词和其出现频率之间的映射。然后我们看读取出的单词，如果不在黑名单中内，并且映射值加1后大于mx的话，我们更新mx，并且更新结果res即可，参见代码如下：



```
class Solution {
public:
    string mostCommonWord(string paragraph, vector<string>& banned) {
        unordered_set<string> ban(banned.begin(), banned.end());
        unordered_map<string, int> strCnt;
        int mx = 0;
        for (auto &c : paragraph) c = isalpha(c) ? tolower(c) : ' ';
        istringstream iss(paragraph);
        string t = "", res = "";
        while (iss >> t) {
            if (!ban.count(t) && ++strCnt[t] > mx) {
                mx = strCnt[t];
                res = t;
            }
        }
        return res;
    }
};
```



参考资料：

[https://leetcode.com/problems/most-common-word/](https://leetcode.com/problems/most-common-word/)

[https://leetcode.com/problems/most-common-word/discuss/124286/Clean-6ms-C%2B%2B-solution](https://leetcode.com/problems/most-common-word/discuss/124286/Clean-6ms-C%2B%2B-solution)

[https://leetcode.com/problems/most-common-word/discuss/123854/C%2B%2BJavaPython-Easy-Solution-with-Explanation](https://leetcode.com/problems/most-common-word/discuss/123854/C%2B%2BJavaPython-Easy-Solution-with-Explanation)




[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













