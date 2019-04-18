# LeetCode Implement Trie (Prefix Tree) （实现trie树3个函数：插入，查找，前缀） - xcw0754 - 博客园
# [LeetCode Implement Trie (Prefix Tree) （实现trie树3个函数：插入，查找，前缀）](https://www.cnblogs.com/xcw0754/p/4649860.html)
![](https://images0.cnblogs.com/blog2015/641737/201507/152345467662486.png)
题意：实现trie树的3个功能，只含小写字母的串。
思路：老实做即可！
```
1 class TrieNode {
 2 public:
 3     TrieNode*  chd[26];
 4     bool flag;
 5     // Initialize your data structure here.
 6     TrieNode() {
 7         memset(chd,0,sizeof(chd));
 8         flag=0;
 9     }
10 
11 };
12 
13 class Trie {
14 public:
15     Trie() {
16         root = new TrieNode();
17     }
18 
19     // Inserts a word into the trie.
20     void insert(string word) {
21         int p=0;
22         TrieNode* tmp=root;
23         while(p<word.size())
24         {
25             if(!tmp->chd[word[p]-'a'])
26             {
27                 TrieNode* newnode=new TrieNode();
28                 tmp->chd[word[p]-'a']=newnode;
29             }
30             tmp=tmp->chd[word[p]-'a'];
31             p++;
32         }
33         tmp->flag=1;
34     }
35 
36     // Returns if the word is in the trie.
37     bool search(string word) {
38         int p=0;
39         TrieNode* tmp=root;
40         while(p<word.size())
41         {
42             //cout<<word[p]<<" ";
43             if(tmp->chd[word[p]-'a'])    tmp=tmp->chd[word[p]-'a'];
44             else return false;
45             p++;
46         }
47         if(tmp->flag==1) return true;
48         return false;
49     }
50 
51     // Returns if there is any word in the trie
52     // that starts with the given prefix.
53     bool startsWith(string prefix) {
54         int p=0;
55         TrieNode* tmp=root;
56         while(p<prefix.size())
57         {
58             if(tmp->chd[prefix[p]-'a'])    tmp=tmp->chd[prefix[p]-'a'];
59             else return false;
60             p++;
61         }
62         return true;
63     }
64 
65 private:
66     TrieNode* root;
67 };
AC代码
```

