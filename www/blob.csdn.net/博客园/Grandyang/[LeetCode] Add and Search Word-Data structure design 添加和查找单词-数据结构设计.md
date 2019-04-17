# [LeetCode] Add and Search Word - Data structure design 添加和查找单词-数据结构设计 - Grandyang - 博客园







# [[LeetCode] Add and Search Word - Data structure design 添加和查找单词-数据结构设计](https://www.cnblogs.com/grandyang/p/4507286.html)







Design a data structure that supports the following two operations:
void addWord(word)
bool search(word)

search(word) can search a literal word or a regular expression string containing only letters `a-z` or `.`. A `.` means it can represent any one letter.

For example:
addWord("bad")
addWord("dad")
addWord("mad")
search("pad") -> false
search("bad") -> true
search(".ad") -> true
search("b..") -> true

Note:
You may assume that all words are consist of lowercase letters `a-z`.

[click to show hint.](https://leetcode.com/problems/add-and-search-word-data-structure-design/)

You should be familiar with how a Trie works. If not, please work on this problem: [Implement Trie (Prefix Tree)](https://leetcode.com/problems/implement-trie-prefix-tree/) first.



LeetCode出新题的速度越来越快了，有点跟不上节奏的感觉了。这道题如果做过之前的那道[Implement Trie (Prefix Tree) 实现字典树(前缀树)](http://www.cnblogs.com/grandyang/p/4491665.html)的话就没有太大的难度了，还是要用到字典树的结构，唯一不同的地方就是search的函数需要重新写一下，因为这道题里面'.'可以代替任意字符，所以一旦有了'.'，就需要查找所有的子树，只要有一个返回true，整个search函数就返回true，典型的DFS的问题，其他部分跟上一道实现字典树没有太大区别，代码如下：



```
class WordDictionary {
public:
    struct TrieNode {
    public:
        TrieNode *child[26];
        bool isWord;
        TrieNode() : isWord(false) {
            for (auto &a : child) a = NULL;
        }
    };
    
    WordDictionary() {
        root = new TrieNode();
    }
    
    // Adds a word into the data structure.
    void addWord(string word) {
        TrieNode *p = root;
        for (auto &a : word) {
            int i = a - 'a';
            if (!p->child[i]) p->child[i] = new TrieNode();
            p = p->child[i];
        }
        p->isWord = true;
    }

    // Returns if the word is in the data structure. A word could
    // contain the dot character '.' to represent any one letter.
    bool search(string word) {
        return searchWord(word, root, 0);
    }
    
    bool searchWord(string &word, TrieNode *p, int i) {
        if (i == word.size()) return p->isWord;
        if (word[i] == '.') {
            for (auto &a : p->child) {
                if (a && searchWord(word, a, i + 1)) return true;
            }
            return false;
        } else {
            return p->child[word[i] - 'a'] && searchWord(word, p->child[word[i] - 'a'], i + 1);
        }
    }
    
private:
    TrieNode *root;
};

// Your WordDictionary object will be instantiated and called as such:
// WordDictionary wordDictionary;
// wordDictionary.addWord("word");
// wordDictionary.search("pattern");
```



讨论：这道题有个很好的Follow up，就是当搜索的单词中存在星号怎么搞，星号的定义和[Wildcard Matching](http://www.cnblogs.com/grandyang/p/4401196.html)中一样，可以代表任意的字符串，包括空字符串，请参见评论区1楼。



类似题目：

[Implement Trie (Prefix Tree)](http://www.cnblogs.com/grandyang/p/4491665.html)

[Wildcard Matching](http://www.cnblogs.com/grandyang/p/4401196.html)



参考资料：

[https://leetcode.com/discuss/36246/my-java-trie-based-solution](https://leetcode.com/discuss/36246/my-java-trie-based-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












