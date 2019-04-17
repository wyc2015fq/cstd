# leetcode-208. Implement Trie (Prefix Tree) 实现 Trie (前缀树) - 别说话写代码的博客 - CSDN博客





2019年04月09日 23:06:30[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：5标签：[实现 Trie (前缀树)																[Implement Trie (Prefix Tree)																[leetcode																[leetcode 208](https://so.csdn.net/so/search/s.do?q=leetcode 208&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



|Implement a trie with `insert`, `search`, and `startsWith` methods.**Example:**Trie trie = new Trie();trie.insert("apple");trie.search("apple");   // returns truetrie.search("app");     // returns falsetrie.startsWith("app"); // returns truetrie.insert("app");   trie.search("app");     // returns true**Note:**- You may assume that all inputs are consist of lowercase letters `a-z`.- All inputs are guaranteed to be non-empty strings.|实现一个 Trie (前缀树)，包含 `insert`, `search`, 和 `startsWith` 这三个操作。**示例:**Trie trie = new Trie();trie.insert("apple");trie.search("apple");   // 返回 truetrie.search("app");     // 返回 falsetrie.startsWith("app"); // 返回 truetrie.insert("app");   trie.search("app");     // 返回 true**说明:**- 你可以假设所有的输入都是由小写字母 `a-z` 构成的。- 保证所有输入均为非空字符串。|
|----|----|

前缀树：Trie树，又称单词查找树或键树，是一种树形结构，是一种哈希树的变种。 

典型应用是 

1. 用于统计和排序大量的字符串（但不仅限于字符串），所以经常被搜索引擎系统用于文本词频统计。 

2. 用于前缀匹配，比如我们在搜索引擎中输入待搜索的字词时，搜索引擎会给予提示有哪些前缀。 

它的优点是：最大限度地减少无谓的字符串比较，查询效率比哈希表高。缺点就是空间开销大。

例如，一个保存了8个键的trie结构，"A", "to", "tea", "ted", "ten", "i", "in", and "inn".如下图所示：



![](http://upload.wikimedia.org/wikipedia/commons/thumb/b/be/Trie_example.svg/375px-Trie_example.svg.png)

前缀树有如下特点： 

1. 根节点不包含字符，除根节点外每一个节点都只包含一个字符。 

2. 从根节点到某一节点，路径上经过的字符连接起来，为该节点对应的字符串。 

3. 每个节点的所有子节点包含的字符都不相同。 

4. 如果字符的种数为n，则每个结点的出度为n，这也是空间换时间的体现，浪费了很多的空间。 

5. 插入查找的复杂度为O(n)，n为字符串长度。


```
class TrieNode{   //前缀树节点
public:
    bool isword;    //根节点到当前节点的字符串是否是单词
    TrieNode *child[26];   //每个节点有26个子节点
    TrieNode():isword(false){
        for(auto &a:child)
            a=NULL;
    }
};
class Trie {
public:
    /** Initialize your data structure here. */
    TrieNode *root;   //定义个根节点
public:
    Trie() {
        root = new TrieNode();   
    }
    
    /** Inserts a word into the trie. */
    void insert(string word) {
        TrieNode *cur = root;
        for(int i=0;i<word.size();++i)
        {
            if(cur->child[word[i]-'a']==NULL)   //没有第i个节点
                cur->child[word[i]-'a']= new TrieNode();   //新建
            cur = cur->child[word[i]-'a'];   //遍历
        }
        cur->isword=true;   //插入之后标记是个单词
    }
    
    /** Returns if the word is in the trie. */
    bool search(string word) {
        TrieNode *cur = root;
        for(int i=0;i<word.size();++i)
        {
            if(cur->child[word[i]-'a']==NULL)
                return false;
            cur = cur->child[word[i]-'a'];
        }
        return cur->isword;     //返回是否是一个单词
    }
    
    /** Returns if there is any word in the trie that starts with the given prefix. */
    bool startsWith(string prefix) {
        TrieNode *cur = root;
        for(int i=0;i<prefix.size();++i)
        {
            if(cur->child[prefix[i]-'a']==NULL)
                return false;
            cur = cur->child[prefix[i]-'a'];
        }
        return true;   //查找到前缀了就返回true
    }
};

/**
 * Your Trie object will be instantiated and called as such:
 * Trie* obj = new Trie();
 * obj->insert(word);
 * bool param_2 = obj->search(word);
 * bool param_3 = obj->startsWith(prefix);
 */
```](https://so.csdn.net/so/search/s.do?q=Implement Trie (Prefix Tree)&t=blog)](https://so.csdn.net/so/search/s.do?q=实现 Trie (前缀树)&t=blog)




