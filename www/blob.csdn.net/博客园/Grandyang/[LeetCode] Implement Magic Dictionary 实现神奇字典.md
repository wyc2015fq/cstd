# [LeetCode] Implement Magic Dictionary 实现神奇字典 - Grandyang - 博客园







# [[LeetCode] Implement Magic Dictionary 实现神奇字典](https://www.cnblogs.com/grandyang/p/7612918.html)







Implement a magic directory with `buildDict`, and `search` methods.

For the method `buildDict`, you'll be given a list of non-repetitive words to build a dictionary.

For the method `search`, you'll be given a word, and judge whether if you modify exactly one character into another character in this word, the modified word is in the dictionary you just built.

Example 1:
Input: buildDict(["hello", "leetcode"]), Output: Null
Input: search("hello"), Output: False
Input: search("hhllo"), Output: True
Input: search("hell"), Output: False
Input: search("leetcoded"), Output: False



Note:
- You may assume that all the inputs are consist of lowercase letters `a-z`.
- For contest purpose, the test data is rather small by now. You could think about highly efficient algorithm after the contest.
- Please remember to RESET your class variables declared in class MagicDictionary, as static/class variables are persisted across multiple test cases. Please see [here](https://leetcode.com/faq/#different-output) for more details.



这道题让我们设计一种神奇字典的数据结构，里面有一些单词，实现的功能是当我们搜索一个单词，只有存在和这个单词只有一个位置上的字符不相同的才能返回true，否则就返回false，注意完全相同也是返回false，必须要有一个字符不同。博主首先想到了[One Edit Distance](http://www.cnblogs.com/grandyang/p/5184698.html)那道题，只不过这道题的两个单词之间长度必须相等。所以只需检测和要搜索单词长度一样的单词即可，所以我们用的数据结构就是根据单词的长度来分，把长度相同相同的单词放到一起，这样就可以减少搜索量。那么对于和要搜索单词进行比较的单词，由于已经保证了长度相等，我们直接进行逐个字符比较即可，用cnt表示不同字符的个数，初始化为0。如果当前遍历到的字符相等，则continue；如果当前遍历到的字符不相同，并且此时cnt已经为1了，则break，否则cnt就自增1。退出循环后，我们检测是否所有字符都比较完了且cnt为1，是的话则返回true，否则就是跟下一个词比较。如果所有词都比较完了，则返回false，参见代码如下：



解法一：

```
class MagicDictionary {
public:
    /** Initialize your data structure here. */
    MagicDictionary() {}
    
    /** Build a dictionary through a list of words */
    void buildDict(vector<string> dict) {
        for (string word : dict) {
            m[word.size()].push_back(word);
        }
    }
    
    /** Returns if there is any word in the trie that equals to the given word after modifying exactly one character */
    bool search(string word) {
        for (string str : m[word.size()]) {
            int cnt = 0, i = 0;
            for (; i < word.size(); ++i) {
                if (word[i] == str[i]) continue;
                if (word[i] != str[i] && cnt == 1) break; 
                ++cnt;
            }
            if (i == word.size() && cnt == 1) return true;
        }
        return false;
    }

private:
    unordered_map<int, vector<string>> m;
};
```



下面这种解法实际上是用到了前缀树中的search的思路，但是我们又没有整个用到prefix tree，博主感觉那样写法略复杂，其实我们只需要借鉴一下search方法就行了。我们首先将所有的单词都放到一个集合中，然后在search函数中，我们遍历要搜索的单词的每个字符，然后把每个字符都用a-z中的字符替换一下，形成一个新词，当然遇到本身要跳过。然后在集合中看是否存在，存在的话就返回true。记得换完一圈字符后要换回去，不然就不满足只改变一个字符的条件了，参见代码如下：



解法二：

```
class MagicDictionary {
public:
    /** Initialize your data structure here. */
    MagicDictionary() {}
    
    /** Build a dictionary through a list of words */
    void buildDict(vector<string> dict) {
        for (string word : dict) s.insert(word);
    }
    
    /** Returns if there is any word in the trie that equals to the given word after modifying exactly one character */
    bool search(string word) {
        for (int i = 0; i < word.size(); ++i) {
            char t = word[i];
            for (char c = 'a'; c <= 'z'; ++c) {
                if (c == t) continue;
                word[i] = c;
                if (s.count(word)) return true;
            }
            word[i] = t;
        }
        return false;
    }
    
private:
    unordered_set<string> s;
};
```



类似题目：

[Implement Trie (Prefix Tree)](http://www.cnblogs.com/grandyang/p/4491665.html)



参考资料：

[https://discuss.leetcode.com/topic/103004/c-clean-code](https://discuss.leetcode.com/topic/103004/c-clean-code)

[https://discuss.leetcode.com/topic/102992/easy-14-lines-java-solution-hashmap](https://discuss.leetcode.com/topic/102992/easy-14-lines-java-solution-hashmap)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












