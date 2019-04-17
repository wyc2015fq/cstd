# [LeetCode] Sentence Similarity II 句子相似度之二 - Grandyang - 博客园







# [[LeetCode] Sentence Similarity II 句子相似度之二](https://www.cnblogs.com/grandyang/p/8053934.html)







Given two sentences `words1, words2` (each represented as an array of strings), and a list of similar word pairs `pairs`, determine if two sentences are similar.

For example, `words1 = ["great", "acting", "skills"]` and `words2 = ["fine", "drama", "talent"]` are similar, if the similar word pairs are `pairs = [["great", "good"], ["fine", "good"], ["acting","drama"], ["skills","talent"]]`.

Note that the similarity relation is transitive. For example, if "great" and "good" are similar, and "fine" and "good" are similar, then "great" and "fine" are similar.

Similarity is also symmetric. For example, "great" and "fine" being similar is the same as "fine" and "great" being similar.

Also, a word is always similar with itself. For example, the sentences `words1 = ["great"], words2 = ["great"], pairs = []` are similar, even though there are no specified similar word pairs.

Finally, sentences can only be similar if they have the same number of words. So a sentence like `words1 = ["great"]` can never be similar to `words2 = ["doubleplus","good"]`.

Note:
- The length of `words1` and `words2` will not exceed `1000`.
- The length of `pairs` will not exceed `2000`.
- The length of each `pairs[i]` will be `2`.
- The length of each `words[i]` and `pairs[i][j]` will be in the range `[1, 20]`.



这道题是之前那道[Sentence Similarity](http://www.cnblogs.com/grandyang/p/8016251.html)的拓展，那道题说单词之间不可传递，于是乎这道题就变成可以传递了，那么难度就增加了。不过没有关系，还是用我们的经典老三样来解，BFS，DFS，和Union Find。我们先来看BFS的解法，其实这道题的本质是无向连通图的问题，那么首先要做的就是建立这个连通图的数据结构，对于每个结点来说，我们要记录所有和其相连的结点，所以我们建立每个结点和其所有相连结点集合之间的映射，比如对于这三个相似对(a, b), (b, c)，和(c, d)，我们有如下的映射关系：

a -> {b}

b -> {a, c}

c -> {b, d}

d -> {c}

那么如果我们要验证a和d是否相似，就需要用到传递关系，a只能找到b，b可以找到a，c，为了不陷入死循环，我们将访问过的结点加入一个集合visited，那么此时b只能去，c只能去d，那么说明a和d是相似的了。那么我们用for循环来比较对应位置上的两个单词，如果二者相同，那么直接跳过去比较接下来的。否则就建一个访问即可visited，建一个队列queue，然后把words1中的单词放入queue，建一个布尔型变量succ，标记是否找到，然后就是传统的BFS遍历的写法了，从队列中取元素，如果和其相连的结点中有words2中的对应单词，标记succ为true，并break掉。否则就将取出的结点加入队列queue，并且遍历其所有相连结点，将其中未访问过的结点加入队列queue继续循环，参见代码如下：



解法一：

```
class Solution {
public:
    bool areSentencesSimilarTwo(vector<string>& words1, vector<string>& words2, vector<pair<string, string>> pairs) {
        if (words1.size() != words2.size()) return false;
        unordered_map<string, unordered_set<string>> m;
        for (auto pair : pairs) {
            m[pair.first].insert(pair.second);
            m[pair.second].insert(pair.first);
        }    
        for (int i = 0; i < words1.size(); ++i) {
            if (words1[i] == words2[i]) continue;
            unordered_set<string> visited;
            queue<string> q{{words1[i]}};
            bool succ = false;
            while (!q.empty()) {
                auto t = q.front(); q.pop();
                if (m[t].count(words2[i])) {
                    succ = true; break;
                }
                visited.insert(t);
                for (auto a : m[t]) {
                    if (!visited.count(a)) q.push(a);
                }
            }
            if (!succ) return false;
        }    
        return true;
    }
};
```



下面来看递归的写法，解题思路跟上面的完全一样，把主要操作都放到了一个递归函数中来写，参见代码如下：



解法二：

```
class Solution {
public:
    bool areSentencesSimilarTwo(vector<string>& words1, vector<string>& words2, vector<pair<string, string>> pairs) {
        if (words1.size() != words2.size()) return false;
        unordered_map<string, unordered_set<string>> m;
        for (auto pair : pairs) {
            m[pair.first].insert(pair.second);
            m[pair.second].insert(pair.first);
        }
        for (int i = 0; i < words1.size(); ++i) {
            unordered_set<string> visited;
            if (!helper(m, words1[i], words2[i], visited)) return false;
        }
        return true;
    }
    bool helper(unordered_map<string, unordered_set<string>>& m, string& cur, string& target, unordered_set<string>& visited) {
        if (cur == target) return true;
        visited.insert(cur);
        for (string word : m[cur]) {
            if (!visited.count(word) && helper(m, word, target, visited)) return true;
        }
        return false;
    }
};
```



下面这种解法就是碉堡了的联合查找Union Find了，这种解法的核心是一个getRoot函数，如果两个元素属于同一个群组的话，调用getRoot函数会返回相同的值。主要分为两部，第一步是建立群组关系，suppose开始时每一个元素都是独立的个体，各自属于不同的群组。然后对于每一个给定的关系对，我们对两个单词分别调用getRoot函数，找到二者的祖先结点，如果从未建立过联系的话，那么二者的祖先结点时不同的，此时就要建立二者的关系。等所有的关系都建立好了以后，第二步就是验证两个任意的元素是否属于同一个群组，就只需要比较二者的祖先结点都否相同啦。是不是有点深度学习的赶脚，先建立模型training，然后再test。哈哈，博主乱扯的，二者并没有什么联系。我们保存群组关系的数据结构，有时用数组，有时用哈希map，看输入的数据类型吧，如果输入元素的整型数的话，用root数组就可以了，如果是像本题这种的字符串的话，需要用哈希表来建立映射，建立每一个结点和其祖先结点的映射。注意这里的祖先结点不一定是最终祖先结点，而最终祖先结点的映射一定是最重祖先结点，所以我们的getRoot函数的设计思路就是要找到最终祖先结点，那么就是当结点和其映射结点相同时返回，否则继续循环，可以递归写，也可以迭代写，这无所谓。注意这里第一行判空是相当于初始化，这个操作可以在外面写，就是要让初始时每个元素属于不同的群组，参见代码如下：



解法三：

```
class Solution {
public:
    bool areSentencesSimilarTwo(vector<string>& words1, vector<string>& words2, vector<pair<string, string>> pairs) {
        if (words1.size() != words2.size()) return false;
        unordered_map<string, string> m;       
        for (auto pair : pairs) {
            string x = getRoot(pair.first, m), y = getRoot(pair.second, m);
            if (x != y) m[x] = y;
        }
        for (int i = 0; i < words1.size(); ++i) {
            if (getRoot(words1[i], m) != getRoot(words2[i], m)) return false;
        }
        return true;
    }
    string getRoot(string word, unordered_map<string, string>& m) {
        if (!m.count(word)) m[word] = word;
        return word == m[word] ? word : getRoot(m[word], m);
    }
};
```



类似题目：

[Friend Circles](http://www.cnblogs.com/grandyang/p/6686983.html)

[Accounts Merge](http://www.cnblogs.com/grandyang/p/7829169.html)

[Sentence Similarity](http://www.cnblogs.com/grandyang/p/8016251.html)



参考资料：

[https://discuss.leetcode.com/topic/112146/c-dfs-solution](https://discuss.leetcode.com/topic/112146/c-dfs-solution)

[https://discuss.leetcode.com/topic/112183/java-c-clean-code-with-explanation](https://discuss.leetcode.com/topic/112183/java-c-clean-code-with-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












