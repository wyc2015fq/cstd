# [LeetCode] Minimum Genetic Mutation 最小基因变化 - Grandyang - 博客园







# [[LeetCode] Minimum Genetic Mutation 最小基因变化](https://www.cnblogs.com/grandyang/p/7653006.html)








A gene string can be represented by an 8-character long string, with choices from `"A"`, `"C"`, `"G"`, `"T"`.

Suppose we need to investigate about a mutation (mutation from "start" to "end"), where ONE mutation is defined as ONE single character changed in the gene string.

For example, `"AACCGGTT"` -> `"AACCGGTA"` is 1 mutation.

Also, there is a given gene "bank", which records all the valid gene mutations. A gene must be in the bank to make it a valid gene string.

Now, given 3 things - start, end, bank, your task is to determine what is the minimum number of mutations needed to mutate from "start" to "end". If there is no such a mutation, return -1.

Note:
- Starting point is assumed to be valid, so it might not be included in the bank.
- If multiple mutations are needed, all mutations during in the sequence must be valid.
- You may assume start and end string is not the same.



Example 1:
start: "AACCGGTT"
end:   "AACCGGTA"
bank: ["AACCGGTA"]

return: 1



Example 2:
start: "AACCGGTT"
end:   "AAACGGTA"
bank: ["AACCGGTA", "AACCGCTA", "AAACGGTA"]

return: 2



Example 3:
start: "AAAAACCC"
end:   "AACCCCCC"
bank: ["AAAACCCC", "AAACCCCC", "AACCCCCC"]

return: 3



这道题跟之前的 [Word Ladder](http://www.cnblogs.com/grandyang/p/4539768.html) 完全是一道题啊，换个故事就直接来啊，越来不走心了啊。不过博主做的时候并没有想起来是之前一样的题，而是先按照脑海里第一个浮现出的思路做的，发现也通过OJ了。博主使用的一种BFS的搜索，先建立bank数组的距离场，这里距离就是两个字符串之间不同字符的个数。然后以start字符串为起点，向周围距离为1的点扩散，采用BFS搜索，每扩散一层，level自加1，当扩散到end字符串时，返回当前level即可。注意我们要把start字符串也加入bank中，而且此时我们也知道start的坐标位置，bank的最后一个位置，然后在建立距离场的时候，调用一个count子函数，用来统计输入的两个字符串之间不同字符的个数，注意dist[i][j]和dist[j][i]是相同，所以我们只用算一次就行了。然后我们进行BFS搜索，用一个visited集合来保存遍历过的字符串，注意检测距离的时候，dist[i][j]和dist[j][i]只要有一个是1，就可以了，参见代码如下：



解法一：

```
class Solution {
public:
    int minMutation(string start, string end, vector<string>& bank) {
        if (bank.empty()) return -1;
        bank.push_back(start);
        int res = 0, n = bank.size();
        queue<int> q{{n - 1}};
        unordered_set<int> visited;
        vector<vector<int>> dist(n, vector<int>(n, 0));
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                dist[i][j] = count(bank[i], bank[j]);
            }
        }
        while (!q.empty()) {
            ++res;
            for (int i = q.size(); i > 0; --i) {
                int t = q.front(); q.pop();
                visited.insert(t);
                for (int j = 0; j < n; ++j) {
                    if ((dist[t][j] != 1 && dist[j][t] != 1) || visited.count(j)) continue;
                    if (bank[j] == end) return res;
                    q.push(j);
                }
            }
        }
        return -1;
    }
    int count(string word1, string word2) {
        int cnt = 0, n = word1.size();
        for (int i = 0; i < n; ++i) {
            if (word1[i] != word2[i]) ++cnt;
        }
        return cnt;
    }
};
```



下面这种解法跟之前的那道 [Word Ladder](http://www.cnblogs.com/grandyang/p/4539768.html) 是一样的，也是用的BFS搜索。跟上面的解法不同之处在于，对于遍历到的字符串，我们不再有距离场，而是对于每个字符，我们都尝试将其换为一个新的字符，每次只换一个，这样会得到一个新的字符串，如果这个字符串在bank中存在，说明这样变换是合法的，加入visited集合和queue中等待下一次遍历，记得在下次置换字符的时候要将之前的还原。我们在queue中取字符串出来遍历的时候，先检测其是否和end相等，相等的话返回level，参见代码如下：



解法二：

```
class Solution {
public:
    int minMutation(string start, string end, vector<string>& bank) {
        if (bank.empty()) return -1;
        vector<char> gens{'A','C','G','T'};
        unordered_set<string> s{bank.begin(), bank.end()};
        unordered_set<string> visited;
        queue<string> q{{start}};
        int level = 0;
        while (!q.empty()) {
            for (int i = q.size(); i > 0; --i) {
                string t = q.front(); q.pop();
                if (t == end) return level;
                for (int j = 0; j < t.size(); ++j) {
                    char old = t[j];
                    for (char c : gens) {
                        t[j] = c;
                        if (s.count(t) && !visited.count(t)) {
                            visited.insert(t);
                            q.push(t);
                        }
                    }
                    t[j] = old;
                }
            }
            ++level;
        }
        return -1;
    }
};
```



再来看一种递归的解法，跟 [Permutations](http://www.cnblogs.com/grandyang/p/4358848.html) 中的解法一有些类似，是遍历bank中的字符串，跟当前的字符串cur相比较，调用isDiffOne()函数判断，若正好跟cur相差一个字符，并且之前没有访问过，那么先在visited数组中标记为true，然后调用递归函数，若返回的不为-1，则用其更新结果res，因为-1代表无法变换成cur。调用完递归后恢复状态，在visited数组中标记为false。循环结束后，看res的值，若还是n+1，表示无法更新，返回-1，否则返回res+1，因为这里的res是变换了一次后到达目标字符串的最小变化次数，所以要加上当前的这次变换。至于isDiffOne()函数就没啥难度了，就是一个一个的比较，不同就累加计数器cnt，参见代码如下：



解法三：

```
class Solution {
public:
    int minMutation(string start, string end, vector<string>& bank) {
        if (bank.empty()) return -1;
        vector<bool> visited(bank.size(), false);
        return helper(start, end, bank, visited);
    }
    int helper(string cur, string end, vector<string>& bank, vector<bool>& visited) {
        if (cur == end) return 0;
        int n = bank.size(), res = n + 1;
        for (int i = 0; i < n; ++i) {
            if (visited[i] || !isDiffOne(bank[i], cur)) continue;
            visited[i] = true;
            int t = helper(bank[i], end, bank, visited);
            if (t != -1) res = min(res, t);
            visited[i] = false;
        }
        return res == n + 1 ? -1 : res + 1;
    }
    bool isDiffOne(string& s1, string& s2) {
        int cnt = 0, n = s1.size();
        for (int i = 0; i < n; ++i) {
            if (s1[i] != s2[i]) ++cnt;
            if (cnt > 1) break;
        }
        return cnt == 1;
    }
};
```



类似题目：

[Word Ladder](http://www.cnblogs.com/grandyang/p/4539768.html)

[Word Ladder II](http://www.cnblogs.com/grandyang/p/4548184.html)

[Permutations](http://www.cnblogs.com/grandyang/p/4358848.html)



参考资料：

[https://leetcode.com/problems/minimum-genetic-mutation/](https://leetcode.com/problems/minimum-genetic-mutation/)

[https://leetcode.com/problems/minimum-genetic-mutation/discuss/91491/dfs-java](https://leetcode.com/problems/minimum-genetic-mutation/discuss/91491/dfs-java)

[https://leetcode.com/problems/minimum-genetic-mutation/discuss/91484/java-solution-using-bfs](https://leetcode.com/problems/minimum-genetic-mutation/discuss/91484/java-solution-using-bfs)




[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












