# [LeetCode] Reconstruct Itinerary 重建行程单 - Grandyang - 博客园







# [[LeetCode] Reconstruct Itinerary 重建行程单](https://www.cnblogs.com/grandyang/p/5183210.html)







Given a list of airline tickets represented by pairs of departure and arrival airports `[from, to]`, reconstruct the itinerary in order. All of the tickets belong to a man who departs from `JFK`. Thus, the itinerary must begin with `JFK`.

Note:
- If there are multiple valid itineraries, you should return the itinerary that has the smallest lexical order when read as a single string. For example, the itinerary `["JFK", "LGA"]` has a smaller lexical order than `["JFK", "LGB"]`.
- All airports are represented by three capital letters (IATA code).
- You may assume all tickets may form at least one valid itinerary.



Example 1:
`tickets` = `[["MUC", "LHR"], ["JFK", "MUC"], ["SFO", "SJC"], ["LHR", "SFO"]]`
Return `["JFK", "MUC", "LHR", "SFO", "SJC"]`.

Example 2:
`tickets` = `[["JFK","SFO"],["JFK","ATL"],["SFO","ATL"],["ATL","JFK"],["ATL","SFO"]]`
Return `["JFK","ATL","JFK","SFO","ATL","SFO"]`.
Another possible reconstruction is `["JFK","SFO","ATL","JFK","ATL","SFO"]`. But it is larger in lexical order.

Credits:
Special thanks to [@dietpepsi](https://leetcode.com/discuss/user/dietpepsi) for adding this problem and creating all test cases.



这道题给我们一堆飞机票，让我们建立一个行程单，如果有多种方法，取其中字母顺序小的那种方法。这道题的本质是有向图的遍历问题，那么LeetCode关于有向图的题只有两道[Course Schedule](http://www.cnblogs.com/grandyang/p/4484571.html)和[Course Schedule II](http://www.cnblogs.com/grandyang/p/4504793.html)，而那两道是关于有向图的顶点的遍历的，而本题是关于有向图的边的遍历。每张机票都是有向图的一条边，我们需要找出一条经过所有边的路径，那么DFS不是我们的不二选择。先来看递归的结果，我们首先把图建立起来，通过邻接链表来建立。由于题目要求解法按字母顺序小的，那么我们考虑用multiset，可以自动排序。等我们图建立好了以后，从节点JFK开始遍历，只要当前节点映射的multiset里有节点，我们取出这个节点，将其在multiset里删掉，然后继续递归遍历这个节点，由于题目中限定了一定会有解，那么等图中所有的multiset中都没有节点的时候，我们把当前节点存入结果中，然后再一层层回溯回去，将当前节点都存入结果，那么最后我们结果中存的顺序和我们需要的相反的，我们最后再翻转一下即可，参见代码如下：



解法一：

```
class Solution {
public:
    vector<string> findItinerary(vector<pair<string, string>> tickets) {
        vector<string> res;
        unordered_map<string, multiset<string>> m;
        for (auto a : tickets) {
            m[a.first].insert(a.second);
        }
        dfs(m, "JFK", res);
        return vector<string> (res.rbegin(), res.rend());
    }
    void dfs(unordered_map<string, multiset<string>>& m, string s, vector<string>& res) {
        while (m[s].size()) {
            string t = *m[s].begin();
            m[s].erase(m[s].begin());
            dfs(m, t, res);
        }
        res.push_back(s);
    }
};
```



下面我们来看迭代的解法，需要借助栈来实现，来实现回溯功能。比如对下面这个例子：

tickets = [["JFK", "KUL"], ["JFK", "NRT"], ["MRT", "JFK"]]

那么建立的图如下：

JFK -> KUL, NRT

NRT -> JFK

由于multiset是按顺序存的，所有KUL会在NRT之前，那么我们起始从JFK开始遍历，先到KUL，但是KUL没有下家了，这时候图中的边并没有遍历完，此时我们需要将KUL存入栈中，然后继续往下遍历，最后再把栈里的节点存回结果即可，参见代码如下：



解法二：

```
class Solution {
public:
    vector<string> findItinerary(vector<pair<string, string>> tickets) {
        vector<string> res;
        stack<string> st{{"JFK"}};
        unordered_map<string, multiset<string>> m;
        for (auto t : tickets) {
            m[t.first].insert(t.second);
        }
        while (!st.empty()) {
            string t = st.top(); 
            if (m[t].empty()) {
                res.insert(res.begin(), t);
                st.pop();
            } else {
                st.push(*m[t].begin());
                m[t].erase(m[t].begin());
            }
        }
        return res;
    }
};
```



类似题目：

[Course Schedule](http://www.cnblogs.com/grandyang/p/4484571.html)

[Course Schedule II](http://www.cnblogs.com/grandyang/p/4504793.html)



参考资料：

[https://leetcode.com/problems/reconstruct-itinerary/](https://leetcode.com/problems/reconstruct-itinerary/)

[https://discuss.leetcode.com/topic/36370/short-ruby-python-java-c](https://discuss.leetcode.com/topic/36370/short-ruby-python-java-c)

[https://discuss.leetcode.com/topic/36721/short-c-dfs-iterative-44ms-solution-with-explanation-no-recursive-calls-no-backtracking](https://discuss.leetcode.com/topic/36721/short-c-dfs-iterative-44ms-solution-with-explanation-no-recursive-calls-no-backtracking)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












