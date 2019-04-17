# [LeetCode] Expressive Words 富于表现力的单词 - Grandyang - 博客园







# [[LeetCode] Expressive Words 富于表现力的单词](https://www.cnblogs.com/grandyang/p/9426942.html)







Sometimes people repeat letters to represent extra feeling, such as "hello" -> "heeellooo", "hi" -> "hiiii".  Here, we have groups, of adjacent letters that are all the same character, and adjacent characters to the group are different.  A group is extended if that group is length 3 or more, so "e" and "o" would be extended in the first example, and "i" would be extended in the second example.  As another example, the groups of "abbcccaaaa" would be "a", "bb", "ccc", and "aaaa"; and "ccc" and "aaaa" are the extended groups of that string.

For some given string S, a query word is *stretchy* if it can be made to be equal to S by extending some groups.  Formally, we are allowed to repeatedly choose a group (as defined above) of characters `c`, and add some number of the same character `c` to it so that the length of the group is 3 or more.  Note that we cannot extend a group of size one like "h" to a group of size two like "hh" - all extensions must leave the group extended - ie., at least 3 characters long.

Given a list of query words, return the number of words that are stretchy. 
Example:
Input: 
S = "heeellooo"
words = ["hello", "hi", "helo"]
Output: 1
Explanation: 
We can extend "e" and "o" in the word "hello" to get "heeellooo".
We can't extend "helo" to get "heeellooo" because the group "ll" is not extended.

Notes:
- `0 <= len(S) <= 100`.
- `0 <= len(words) <= 100`.
- `0 <= len(words[i]) <= 100`.
- `S` and all words in `words` consist only of lowercase letters



这道题定义了一种富于表现力的单词，就是说某个字母可以重复三次或以上，那么对于这种重复后的单词，我们称之为可拉伸的（stretchy）。现在给了我们一个拉伸后的单词S，又给了我们一个单词数组，问我们里面有多少个单词可以拉伸成为S。其实这道题的关键就在于看某个字母是否被重复了三次，重复两次是不行的。那么我们就只能遍历单词数组words中的单词，来分别和S比较了。每个遍历到的单词的长度suppose是应该小于等于S的，因为S是拉伸后的单词，当然S也可以和遍历到的单词相等，那么表示没有拉伸。我们需要两个指针i和j来分别指向S和遍历单词word，我们需要逐个比较，由于S的长度要大于等于word，所以我们for循环直接遍历S的字母就好了，首先看如果j没越界，并且此时S[i]和word[j]相等的话，那么j自增1，i在for循环中也会自增1，遍历下一个字母。如果此时不相等或者j已经越界的话，我们再看当前的S[i]是否是3个重复中的中间那个，即S[i-1]和S[i+1]需要等于S[i]，是的话，i自增1，然后加上for循环中的自增1，相当于总共增了2个，正好跳过这个重复三连。否则的话再看是否前两个都和当前的字母相等，即S[i-1]和S[i-2]需要等于S[i]，因为可能重复的个数多于3个，如果这个条件不满足的话，直接break就行了。for循环结束或者跳出后，我们看S和word是否正好遍历完，即i和j是否分别等于S和word的长度，是的话结果res自增1，参见代码如下：



```
class Solution {
public:
    int expressiveWords(string S, vector<string>& words) {
        int res = 0, m = S.size(), n = words.size();
        for (string word : words) {
            int i = 0, j = 0;
            for (; i < m; ++i) {
                if (j < word.size() && S[i] == word[j]) ++j;
                else if (i > 0 && S[i] == S[i - 1] && i + 1 < m && S[i] == S[i + 1]) ++i;
                else if (!(i > 1 && S[i] == S[i - 1] && S[i] == S[i - 2])) break;
            }
            if (i == m && j == word.size()) ++res;
        }
        return res;
    }
};
```



参考资料：

[https://leetcode.com/problems/expressive-words/discuss/122660/C++JavaPython-2-Pointers-and-4-pointers](https://leetcode.com/problems/expressive-words/discuss/122660/C++JavaPython-2-Pointers-and-4-pointers)

[https://leetcode.com/problems/expressive-words/discuss/121741/Short-straight-forward-C++-solution-two-pointers-one-pass-scan](https://leetcode.com/problems/expressive-words/discuss/121741/Short-straight-forward-C++-solution-two-pointers-one-pass-scan)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












