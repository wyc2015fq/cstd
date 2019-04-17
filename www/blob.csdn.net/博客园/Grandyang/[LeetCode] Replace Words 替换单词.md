# [LeetCode] Replace Words 替换单词 - Grandyang - 博客园







# [[LeetCode] Replace Words 替换单词](https://www.cnblogs.com/grandyang/p/7423420.html)








In English, we have a concept called `root`, which can be followed by some other words to form another longer word - let's call this word `successor`. For example, the root `an`, followed by `other`, which can form another word `another`.

Now, given a dictionary consisting of many roots and a sentence. You need to replace all the `successor` in the sentence with the `root` forming it. If a `successor` has many `roots` can form it, replace it with the root with the shortest length.

You need to output the sentence after the replacement.

Example 1:
Input: dict = ["cat", "bat", "rat"]
sentence = "the cattle was rattled by the battery"
Output: "the cat was rat by the bat"



Note:
- The input will only have lower-case letters.
- 1 <= dict words number <= 1000
- 1 <= sentence words number <= 1000
- 1 <= root length <= 100
- 1 <= sentence words length <= 1000



这道题给了我们一个前缀字典，又给了一个句子，让我们将句子中较长的单词换成其前缀(如果在前缀字典中存在的话)。我们对于句子中的一个长单词如何找前缀呢，是不是可以根据第一个字母来快速定位呢，比如cattle这个单词的首字母是c，那么我们在前缀字典中找所有开头是c的前缀，为了方便查找，我们将首字母相同的前缀都放到同一个数组中，总共需要26个数组，所以我们可以定义一个二维数组来装这些前缀。还有，我们希望短前缀在长前缀的前面，因为题目中要求用最短的前缀来替换单词，所以我们可以先按单词的长度来给所有的前缀排序，然后再依次加入对应的数组中，这样就可以保证短的前缀在前面。

下面我们就要来遍历句子中的每一个单词了，由于C++中没有split函数，所以我们就采用字符串流来提取每一个单词，对于遍历到的单词，我们根据其首字母查找对应数组中所有以该首字母开始的前缀，然后直接用substr函数来提取单词中和前缀长度相同的子字符串来跟前缀比较，如果二者相等，说明可以用前缀来替换单词，然后break掉for循环。别忘了单词之前还要加上空格，参见代码如下：



解法一：

```
class Solution {
public:
    string replaceWords(vector<string>& dict, string sentence) {
        string res = "", t = "";
        vector<vector<string>> v(26);
        istringstream is(sentence);
        sort(dict.begin(), dict.end(), [](string &a, string &b) {return a.size() < b.size();});
        for (string word : dict) {
            v[word[0] - 'a'].push_back(word);
        }
        while (is >> t) {
            for (string word : v[t[0] - 'a']) {
                if (t.substr(0, word.size()) == word) {
                    t = word;
                    break;
                }
            }
            res += t + " ";
        }
        res.pop_back();
        return res;
    }
};
```



你以为想出了上面的解法，这道题就算做完了？? Naive! ! ! 这道题最好的解法其实是用前缀树(Trie / Prefix Tree)来做，关于前缀树使用之前有一道很好的入门题[Implement Trie (Prefix Tree)](http://www.cnblogs.com/grandyang/p/4491665.html)。了解了前缀树的原理机制，那么我们就可以发现这道题其实很适合前缀树的特点。我们要做的就是把所有的前缀都放到前缀树里面，而且在前缀的最后一个结点的地方将标示isWord设为true，表示从根节点到当前结点是一个前缀，然后我们在遍历单词中的每一个字母，我们都在前缀树查找，如果当前字母对应的结点的表示isWord是true，我们就返回这个前缀，如果当前字母对应的结点在前缀树中不存在，我们就返回原单词，这样就能完美的解决问题了。所以啊，以后遇到了有关前缀或者类似的问题，一定不要忘了前缀树这个神器哟～



解法二：

```
class Solution {
public:
    class TrieNode {
    public:
        bool isWord;
        TrieNode *child[26];
        TrieNode(): isWord(false) {
            for (auto &a : child) a = NULL;
        }
    };
    
    string replaceWords(vector<string>& dict, string sentence) {
        string res = "", t = "";
        istringstream is(sentence);
        TrieNode *root = new TrieNode();
        for (string word : dict) {
            insert(root, word);
        }
        while (is >> t) {
            if (!res.empty()) res += " ";
            res += findPrefix(root, t);
        }
        return res;
    }
    
    void insert(TrieNode* node, string word) {
        for (char c : word) {
            if (!node->child[c - 'a']) node->child[c - 'a'] = new TrieNode();
            node = node->child[c - 'a'];
        }
        node->isWord = true;
    }
    
    string findPrefix(TrieNode* node, string word) {
        string cur = "";
        for (char c : word) {
            if (!node->child[c - 'a']) break;
            cur.push_back(c);
            node = node->child[c - 'a'];
            if (node->isWord) return cur;
        }
        return word;
    }
};
```



类似题目：

[Implement Trie (Prefix Tree)](http://www.cnblogs.com/grandyang/p/4491665.html)



参考资料：

[https://discuss.leetcode.com/topic/97203/trie-tree-concise-java-solution-easy-to-understand](https://discuss.leetcode.com/topic/97203/trie-tree-concise-java-solution-easy-to-understand)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













