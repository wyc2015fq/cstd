# [LeetCode] Alien Dictionary 另类字典 - Grandyang - 博客园







# [[LeetCode] Alien Dictionary 另类字典](https://www.cnblogs.com/grandyang/p/5250200.html)












There is a new alien language which uses the latin alphabet. However, the order among letters are unknown to you. You receive a list of non-empty words from the dictionary, where words are sorted lexicographically by the rules of this new language. Derive the order of letters in this language.

Example 1:
Input:
[
  "wrt",
  "wrf",
  "er",
  "ett",
  "rftt"
]

Output: `"wertf"`
Example 2:
Input:
[
  "z",
  "x"
]

Output: `"zx"`
Example 3:
Input:
[
  "z",
  "x",
  "z"
] 

Output: `""` 

Explanation: The order is invalid, so return `""`.

Note:
- You may assume all letters are in lowercase.
- You may assume that if a is a prefix of b, then a must appear before b in the given dictionary.
- If the order is invalid, return an empty string.
- There may be multiple valid order of letters, return any one of them is fine.



这道题让给了我们一些按“字母顺序”排列的单词，但是这个字母顺序不是我们熟知的顺序，而是另类的顺序，让我们根据这些“有序”的单词来找出新的字母顺序，这实际上是一道有向图遍历的问题，跟之前的那两道 [Course Schedule II](http://www.cnblogs.com/grandyang/p/4504793.html) 和 [Course Schedule](http://www.cnblogs.com/grandyang/p/4484571.html) 的解法很类似，我们先来看BFS的解法，需要一个TreeSet来保存可以推测出来的顺序关系，比如题目中给的例子1，我们可以推出的顺序关系有：

```
t->f
w->e
r->t
e->r
```

这些就是有向图的边，对于有向图中的每个结点，计算其入度，然后从入度为0的结点开始BFS遍历这个有向图，然后将遍历路径保存下来返回即可。下面来看具体的做法：

根据之前讲解，需用TreeSet来保存这些pair，我们还需要一个HashSet来保存所有出现过的字母，需要一个一维数组in来保存每个字母的入度，另外还要一个queue来辅助拓扑遍历，我们先遍历单词集，把所有字母先存入HashSet，然后我们每两个相邻的单词比较，找出顺序pair，然后我们根据这些pair来赋度，我们把HashSet中入度为0的字母都排入queue中，然后开始遍历，如果字母在TreeSet中存在，则将其pair中对应的字母的入度减1，若此时入度减为0了，则将对应的字母排入queue中并且加入结果res中，直到遍历完成，我们看结果res和ch中的元素个数是否相同，若不相同则说明可能有环存在，返回空字符串，参见代码如下：



解法一：

```
class Solution {
public:
    string alienOrder(vector<string>& words) {
        set<pair<char, char>> st;
        unordered_set<char> ch;
        vector<int> in(256, 0);
        queue<char> q;
        string res = "";
        for (auto a : words) ch.insert(a.begin(), a.end());
        for (int i = 0; i < (int)words.size() - 1; ++i) {
            int mn = min(words[i].size(), words[i + 1].size()), j = 0;
            for (; j < min(words[i].size(), words[i + 1].size()); ++j) {
                if (words[i][j] != words[i + 1][j]) {
                    st.insert(make_pair(words[i][j], words[i + 1][j]));
                    break;
                }
            }
            if (j == mn && words[i].size() > words[i + 1].size()) return "";
        }
        for (auto a : st) ++in[a.second];
        for (auto a : ch) {
            if (in[a] == 0) {
                q.push(a);
                res += a;
            } 
        }
        while (!q.empty()) {
            char c = q.front(); q.pop();
            for (auto a : st) {
                if (a.first == c) {
                    --in[a.second];
                    if (in[a.second] == 0) {
                        q.push(a.second);
                        res += a.second;
                    }
                }
            }
        }
        return res.size() == ch.size() ? res : "";
    }
};
```



下面来看一种DFS的解法，思路和BFS的很类似，我们需要建立一个二维的bool数组g，为了节省空间，不必像上面的解法中一样使用一个HashSet来记录所有出现过的字母，我们可以直接用这个二维数组来保存这个信息，只要g[i][i] = true，即表示位置为i的字母存在。同时，这个二维数组还可以保存顺序对儿的信息，只要 g[i][j] = true，就知道位置为i的字母顺序在位置为j的字母前面。找顺序对儿的方法跟上面的解法完全相同，之后我们就可以进行DFS遍历了。由于DFS遍历需要标记遍历结点，那么就用一个visited数组，由于是深度优先的遍历，我们并不需要一定要从入度为0的结点开始遍历，而是从任意一个结点开始都可以，DFS会遍历到出度为0的结点为止，加入结果res，然后回溯加上整条路径到结果res，最后把结果res翻转一下即可，参见代码如下：



解法二：

```
class Solution {
public:
    string alienOrder(vector<string>& words) {
        vector<vector<bool>> g(26, vector<bool>(26, false));
        vector<bool> visited(26, false);
        string res = "";
        for (string word : words) {
            for (char c : word) {
                g[c - 'a'][c - 'a'] = true;
            }
        }
        for (int i = 0; i < (int)words.size() - 1; ++i) {
            int mn = min(words[i].size(), words[i + 1].size()), j = 0;
            for (; j < mn; ++j) {
                if (words[i][j] != words[i + 1][j]) {
                    g[words[i][j] - 'a'][words[i + 1][j] - 'a'] = true;
                    break;
                }
            }
            if (j == mn && words[i].size() > words[i + 1].size()) return "";
        }
        for (int i = 0; i < 26; ++i) {
            if (!dfs(g, i, visited, res)) return "";
        }
        return string(res.rbegin(), res.rend());
    }
    bool dfs(vector<vector<bool>>& g, int idx, vector<bool>& visited, string& res) {
        if (!g[idx][idx]) return true;
        visited[idx] = true;
        for (int i = 0; i < 26; ++i) {
            if (i == idx || !g[idx][i]) continue;
            if (visited[i]) return false;
            if (!dfs(g, i, visited, res)) return false;
        }
        visited[idx] = false;
        g[idx][idx] = false;
        res += 'a' + idx;
        return true;
    }
};
```








类似题目：


[Minimum Height Trees](http://www.cnblogs.com/grandyang/p/5000291.html)

[Course Schedule II](http://www.cnblogs.com/grandyang/p/4504793.html)
[Course Schedule](http://www.cnblogs.com/grandyang/p/4484571.html)




参考资料：

[https://leetcode.com/problems/alien-dictionary/](https://leetcode.com/problems/alien-dictionary/)

[https://leetcode.com/problems/alien-dictionary/discuss/70169/my-concise-java-solution-based-on-topological-sorting](https://leetcode.com/problems/alien-dictionary/discuss/70169/my-concise-java-solution-based-on-topological-sorting)




[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












