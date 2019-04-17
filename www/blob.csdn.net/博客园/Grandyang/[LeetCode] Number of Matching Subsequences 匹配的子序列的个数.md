# [LeetCode] Number of Matching Subsequences 匹配的子序列的个数 - Grandyang - 博客园







# [[LeetCode] Number of Matching Subsequences 匹配的子序列的个数](https://www.cnblogs.com/grandyang/p/9201323.html)







Given string `S` and a dictionary of words `words`, find the number of `words[i]` that is a subsequence of `S`.
Example :
Input: 
S = "abcde"
words = ["a", "bb", "acd", "ace"]
Output: 3
Explanation: There are three words in `words` that are a subsequence of `S`: "a", "acd", "ace".

Note:
- All words in `words` and `S` will only consists of lowercase letters.
- The length of `S` will be in the range of `[1, 50000]`.
- The length of `words` will be in the range of `[1, 5000]`.
- The length of `words[i]` will be in the range of `[1, 50]`.



这道题给了我们一个字符串S，又给了一个单词数组，问我们数组有多少个单词是字符串S的子序列。注意这里是子序列，而不是子串，子序列并不需要连续。那么只要我们知道如何验证一个子序列的方法，那么就可以先尝试暴力搜索法，就是对数组中的每个单词都验证一下是否是字符串S的子序列。验证子序列的方法就是用两个指针，对于子序列的每个一个字符，都需要在母字符中找到相同的，在母字符串所有字符串遍历完之后或之前，只要子序列中的每个字符都在母字符串中按顺序找到了，那么就验证成功了。很不幸，这种暴力搜索的方法在C++的解法版本中会TLE，貌似Java版本的可以通过，感觉C++被dis了诶～ However，我们可以进行优化呀，在暴力搜索的基础上稍作些优化，就可以骗过OJ啦。下面这种优化的motivation是由于看了使暴力搜索跪了的那个test case，其实是words数组里有大量相同的单词，而且字符串S巨长无比，那么为了避免相同的单词被不停的重复检验，我们用两个HashSet来记录验证过的单词，为啥要用两个呢？因为验证的结果有两种，要么通过，要么失败，我们要分别存在两个HashSet中，这样再遇到每种情况的单词时，我们就知道要不要结果增1了。如果单词没有验证过的话，那么我们就用双指针的方法进行验证，然后根据结果的不同，存到相应的HashSet中去，参见代码如下：



解法一：

```
class Solution {
public:
    int numMatchingSubseq(string S, vector<string>& words) {
        int res = 0, n = S.size();
        unordered_set<string> pass, out;
        for (string word : words) {
            if (pass.count(word) || out.count(word)) {
                if (pass.count(word)) ++res; 
                continue;
            }
            int i = 0, j = 0, m = word.size();
            while (i < n && j < m) {
                if (word[j] == S[i]) ++j;
                ++i;
            }
            if (j == m) {++res; pass.insert(word);}
            else out.insert(word);
        }
        return res;
    }
};
```



上面的解法已经优化的不错了，但是我们还有更叼的方法。这种解法按照每个单词的首字符进行群组，群组里面保存的是一个pair对，由当前字母和下一个位置组成的。然后在遍历字符串S的时候，根据当前遍历到的字母，进入该字母对应的群组中处理，如果群组中某个pair的下一个位置已经等于单词长度了，说明该单词已经验证完成，是子序列，结果自增1；否则的话就将下一个位置的字母提取出来，然后将pair中的下一个位置自增1后组成的新pair加入之前提取出的字母对应的群组中。是不是读到这里已然懵逼了，没关系，博主会举栗子来说明的，就拿题目中的那个例子来说吧：

S = "abcde"

words = ["a", "bb", "acd", "ace"]

那么首先我们将words数组中的单词按照其首字母的不同放入对应的群组中，得到：

a -> {0, 1}, {2, 1}, {3, 1}

b -> {1, 1}

这里，每个pair的第一个数字是该单词在words中的位置，第二个数字是下一个字母的位置。比如 {0, 1} 表示 "a" 在words数组中位置为0，且下一个位置为1（因为当前位置是首字母）。{2, 1} 表示 "acd" 在words数组中位置为2，且下一个位置为1。{3, 1} 表示 "ace" 在words数组中位置为3，且下一个位置为1。{1, 1} 表示 "bb" 在words数组中位置为1，且下一个位置为1。

好，下面我们来遍历字符串S，第一个遇到的字母是 'a'。

那么我们群组中a对应了三个pair，将其提取出来分别进行操作。首先处理 {0, 1}，此时我们发现下一个位置为1，和单词"a"的长度相同了，说明是子序列，结果res自增1。然后处理 {2, 1}，在"acd"中取下一个位置1的字母为'c'，则将下一位置自增1后的新pair {2, 2} 加入c对应的群组。然后处理 {3, 1}，在"ace"中取下一个位置1的字母为'c'，则将下一位置自增1后的新pair {3, 2} 加入c对应的群组。则此时的群组为：

b -> {1, 1}

c -> {2, 2}, {3, 2}

好，继续来遍历字符串S，第二个遇到的字母是 'b'。

那么我们群组中b对应了一个pair，处理 {1, 1}，在"bb"中取下一个位置1的字母为'b'，则将下一位置自增1后的新pair {1, 2} 加入b对应的群组。则此时的群组为：

b -> {1, 2}

c -> {2, 2}, {3, 2}

好，继续来遍历字符串S，第三个遇到的字母是 'c'。

那么我们群组中c对应了两个pair，将其提取出来分别进行操作。首先处理 {2, 2}，在"ace"中取下一个位置2的字母为'e'，则将下一位置自增1后的新pair {2, 3} 加入e对应的群组。然后处理 {3, 2}，在"acd"中取下一个位置2的字母为'd'，则将下一位置自增1后的新pair {3, 3} 加入d对应的群组。则此时的群组为：

b -> {1, 2}

d -> {3, 3}

e -> {2, 3}

好，继续来遍历字符串S，第四个遇到的字母是 'd'。

那么我们群组中d对应了一个pair，处理 {3, 3}，此时我们发现下一个位置为3，和单词"acd"的长度相同了，说明是子序列，结果res自增1。则此时的群组为：

b -> {1, 2}

e -> {2, 3}

好，继续来遍历字符串S，第五个遇到的字母是 'e'。

那么我们群组中e对应了一个pair，处理 {2, 3}，此时我们发现下一个位置为3，和单词"ace"的长度相同了，说明是子序列，结果res自增1。则此时的群组为：

b -> {1, 2}

此时S已经遍历完了，已经没有b了，说明"bb"不是子序列，这make sense，返回结果res即可，参见代码如下：



解法二：

```
class Solution {
public:
    int numMatchingSubseq(string S, vector<string>& words) {
        vector<pair<int, int>> all[128];
        int res = 0, n = words.size();
        for (int i = 0; i < n; i++) {
            all[words[i][0]].emplace_back(i, 1);
        }
        for (char c : S) {
            auto vect = all[c];
            all[c].clear();
            for (auto it : vect) {
                if (it.second == words[it.first].size()) ++res;
                else all[words[it.first][it.second++]].push_back(it);
            }
        }
        return res;
    }
};
```



类似题目：

[Is Subsequence](http://www.cnblogs.com/grandyang/p/5842033.html)



参考资料：

[https://leetcode.com/problems/number-of-matching-subsequences/solution/](https://leetcode.com/problems/number-of-matching-subsequences/solution/)

[https://leetcode.com/problems/number-of-matching-subsequences/discuss/117634/Efficient-and-simple-go-through-words-in-parallel-with-explanation](https://leetcode.com/problems/number-of-matching-subsequences/discuss/117634/Efficient-and-simple-go-through-words-in-parallel-with-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












