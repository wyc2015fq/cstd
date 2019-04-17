# [LintCode]  Implement Trie 实现字典树 - Grandyang - 博客园







# [[LintCode]  Implement Trie 实现字典树](https://www.cnblogs.com/grandyang/p/4868908.html)







Implement a trie with insert, search, and startsWith methods.

Have you met this question in a real interview?





**Example**

**Note**

You may assume that all inputs are consist of lowercase letters a-z.



LeetCode上的原题，请参见我之前的博客[Implement Trie (Prefix Tree) 实现字典树(前缀树)](http://www.cnblogs.com/grandyang/p/4491665.html)。



```
/**
 * Your Trie object will be instantiated and called as such:
 * Trie trie;
 * trie.insert("lintcode");
 * trie.search("lint"); will return false
 * trie.startsWith("lint"); will return true
 */
class TrieNode {
public:
    // Initialize your data structure here.
    TrieNode *child[26];
    bool isWord;
    TrieNode(): isWord(false) {
        for (auto & a : child) a = NULL;
    }
};

class Trie {
public:
    Trie() {
        root = new TrieNode();
    }

    // Inserts a word into the trie.
    void insert(string word) {
        TrieNode *p = root;
        for (auto &a : word) {
            int i = a - 'a';
            if (!p->child[i]) p->child[i] = new TrieNode();
            p = p->child[i];
        }
        p->isWord = true;
    }

    // Returns if the word is in the trie.
    bool search(string word) {
        TrieNode *p = root;
        for (auto &a : word) {
            int i = a - 'a';
            if (!p->child[i]) return false;
            p = p->child[i];
        }
        return p->isWord;
    }

    // Returns if there is any word in the trie
    // that starts with the given prefix.
    bool startsWith(string prefix) {
        TrieNode *p = root;
        for (auto &a : prefix) {
            int i = a - 'a';
            if (!p->child[i]) return false;
            p = p->child[i];
        }
        return true;
    }

private:
    TrieNode* root;
};
```
















