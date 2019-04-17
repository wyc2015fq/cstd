# [LeetCode] Word Search II 词语搜索之二 - Grandyang - 博客园







# [[LeetCode] Word Search II 词语搜索之二](https://www.cnblogs.com/grandyang/p/4516013.html)







Given a 2D board and a list of words from the dictionary, find all words in the board.

Each word must be constructed from letters of sequentially adjacent cell, where "adjacent" cells are those horizontally or vertically neighboring. The same letter cell may not be used more than once in a word.

 For example,

Given **words** = `["oath","pea","eat","rain"]` and **board** = 

[
  ['o','a','a','n'],
  ['e','t','a','e'],
  ['i','h','k','r'],
  ['i','f','l','v']
]

Return `["eat","oath"]`.

**Note:**

You may assume that all inputs are consist of lowercase letters `a-z`.


[click to show hint.](https://leetcode.com/problems/word-search-ii/)


You would need to optimize your backtracking to pass the larger test. Could you stop backtracking earlier?

If the current candidate does not exist in all words' prefix, you 
could stop backtracking immediately. What kind of data structure could 
answer such query efficiently? Does a hash table work? Why or why not? 
How about a Trie? If you would like to learn how to implement a basic 
trie, please work on this problem: [Implement Trie (Prefix Tree)](https://leetcode.com/problems/implement-trie-prefix-tree/) first.




这道题是在之前那道[Word Search 词语搜索](http://www.cnblogs.com/grandyang/p/4332313.html)的基础上做了些拓展，之前是给一个单词让判断是否存在，现在是给了一堆单词，让返回所有存在的单词，在这道题最开始更新的几个小时内，用brute force是可以通过OJ的，就是在之前那题的基础上多加一个for循环而已，但是后来出题者其实是想考察字典树的应用，所以加了一个超大的test case，以至于brute force无法通过，强制我们必须要用字典树来求解。LeetCode中有关字典树的题还有[Implement Trie (Prefix Tree) 实现字典树(前缀树)](http://www.cnblogs.com/grandyang/p/4491665.html)和[Add and Search Word - Data structure design 添加和查找单词-数据结构设计](http://www.cnblogs.com/grandyang/p/4507286.html)，那么我们在这题中只要实现字典树中的insert功能就行了，查找单词和前缀就没有必要了，然后DFS的思路跟之前那道[Word Search 词语搜索](http://www.cnblogs.com/grandyang/p/4332313.html)基本相同，请参见代码如下：



```
class Solution {
public:
    struct TrieNode {
        TrieNode *child[26];
        string str;
        TrieNode() : str("") {
            for (auto &a : child) a = NULL;
        }
    };
    struct Trie {
        TrieNode *root;
        Trie() : root(new TrieNode()) {}
        void insert(string s) {
            TrieNode *p = root;
            for (auto &a : s) {
                int i = a - 'a';
                if (!p->child[i]) p->child[i] = new TrieNode();
                p = p->child[i];
            }
            p->str = s;
        }
    };
    vector<string> findWords(vector<vector<char> >& board, vector<string>& words) {
        vector<string> res;
        if (words.empty() || board.empty() || board[0].empty()) return res;
        vector<vector<bool> > visit(board.size(), vector<bool>(board[0].size(), false));
        Trie T;
        for (auto &a : words) T.insert(a);
        for (int i = 0; i < board.size(); ++i) {
            for (int j = 0; j < board[i].size(); ++j) {
                if (T.root->child[board[i][j] - 'a']) {
                    search(board, T.root->child[board[i][j] - 'a'], i, j, visit, res);
                }
            }
        }
        return res;
    }
    void search(vector<vector<char> > &board, TrieNode *p, int i, int j, vector<vector<bool> > &visit, vector<string> &res) { 
        if (!p->str.empty()) {
            res.push_back(p->str);
            p->str.clear();
        }
        int d[][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        visit[i][j] = true;
        for (auto &a : d) {
            int nx = a[0] + i, ny = a[1] + j;
            if (nx >= 0 && nx < board.size() && ny >= 0 && ny < board[0].size() && !visit[nx][ny] && p->child[board[nx][ny] - 'a']) {
                search(board, p->child[board[nx][ny] - 'a'], nx, ny, visit, res);
            }
        }
        visit[i][j] = false;
    }
};
```



参考资料：

[https://leetcode.com/discuss/36367/simple-c-solution-using-trie](https://leetcode.com/discuss/36367/simple-c-solution-using-trie)

[http://blog.csdn.net/ljiabin/article/details/45846527](http://blog.csdn.net/ljiabin/article/details/45846527)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












