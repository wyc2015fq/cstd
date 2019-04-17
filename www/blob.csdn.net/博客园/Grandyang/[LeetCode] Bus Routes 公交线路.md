# [LeetCode] Bus Routes 公交线路 - Grandyang - 博客园







# [[LeetCode] Bus Routes 公交线路](https://www.cnblogs.com/grandyang/p/10293947.html)









We have a list of bus routes. Each `routes[i]` is a bus route that the i-th bus repeats forever. For example if `routes[0] = [1, 5, 7]`, this means that the first bus (0-th indexed) travels in the sequence 1->5->7->1->5->7->1->... forever.

We start at bus stop `S` (initially not on a bus), and we want to go to bus stop `T`. Travelling by buses only, what is the least number of buses we must take to reach our destination? Return -1 if it is not possible.
Example:
Input: 
routes = [[1, 2, 7], [3, 6, 7]]
S = 1
T = 6
Output: 2
Explanation: 
The best strategy is take the first bus to the bus stop 7, then take the second bus to the bus stop 6.

Note:
- `1 <= routes.length <= 500`.
- `1 <= routes[i].length <= 500`.
- `0 <= routes[i][j] < 10 ^ 6`.





这道题给了我们一堆公交线路表，然后给了我们起点和终点，问我们最少要换乘几辆公交可以从起点到达终点。这种原本我们只需要使用谷歌地图或者百度地图轻松实现的事，现在需要自己来实现。但这毕竟是简化版，真实情况一定要复杂得多。 这题容易进的一个误区就是把routes直接当作邻接链表来进行图的遍历，其实是不对的，因为routes数组的含义是，某个公交所能到达的站点，而不是某个站点所能到达的其他站点。这里出现了两种不同的结点，分别是站点和公交。而routes数组建立的是公交和其站点之间的关系，那么我们应该将反向关系数组也建立出来，即要知道每个站点有哪些公交可以到达。由于这里站点的标号不一定是连续的，所以我们可以使用HashMap，建立每个站点和其属于的公交数组之间的映射。由于一个站点可以被多个公交使用，所以要用个数组来保存公交。既然这里求的是最少使用公交的数量，那么就类似迷宫遍历求最短路径的问题，BFS应该是首先被考虑的解法。那么就用队列queue来辅助，首先将起点S排入队列中，然后我们还需要一个HashSet来保存已经遍历过的公交（注意这里思考一下，为啥放的是公交而不是站点，因为我们统计的是最少需要坐的公交个数，这里一层就相当于一辆公交，最小的层数就是公交数），这些都是BFS的标配，应当已经很熟练了。我们应该在最开头先判断一下，若起点和终点相同，那么直接返回0，因为根本不用坐公交。否则我们开始while循环，先将结果res自增1，因为既然已经上了公交，那么公交个数至少为1，初始化的时候是0。这里我们使用BFS的层序遍历的写法，就是当前所有的结点都当作深度相同的一层，至于为何采用这种倒序遍历的for循环写法，是因为之后队列的大小可能变化，放在判断条件中可能会出错。在for循环中，先取出队首站点，然后我们要去HashMap中去遍历经过该站点的所有公交，若某个公交已经遍历过了，直接跳过，否则就加入visited中。然后我们去routes数组中取出该公交的所有站点，如果有终点，则直接返回结果res，否则就将站点排入队列中继续遍历，参见代码如下：



解法一：

```
class Solution {
public:
    int numBusesToDestination(vector<vector<int>>& routes, int S, int T) {
        if (S == T) return 0;
        int res = 0;
        unordered_map<int, vector<int>> stop2bus;
        queue<int> q{{S}};
        unordered_set<int> visited;
        for (int i = 0; i < routes.size(); ++i) {
            for (int j : routes[i]) {
                stop2bus[j].push_back(i);
            }
        }
        while (!q.empty()) {
            ++res;
            for (int i = q.size(); i > 0; --i) {
                int t = q.front(); q.pop();
                for (int bus : stop2bus[t]) {
                    if (visited.count(bus)) continue;
                    visited.insert(bus);
                    for (int stop : routes[bus]) {
                        if (stop == T) return res;
                        q.push(stop);
                    }
                }
            }
        }
        return -1;
    }
};
```



下面这种方法也是BFS解法，思路上跟上面的解法没有啥大的区别，就是数据结构的写法上略有不同。这里的队列queue放的是一个由站点和公交个数组成的pair对儿，这样我们就不用维护一个全局的最小公交数变量了。当然反向关系数组还是要建立出来的，即要知道每个站点有哪些公交可以到达。和上面稍有不同的是，我们使用了HashSet来保存经过某个站点的所有公交，但其实和用数组并没啥区别，因为这里没有查询需求，无法发挥HashSet的优势。由于对于每个站点，我们都保存了当达该站点所需的最少公交数，那么我们就不需要使用层序遍历的BFS的写法，直接用最一般的写法即可。还有一个不同之处在于，这里我们的visited保存的是遍历过的站点，而不在是公交了。在while循环中，首先将队首元素取出来，这里就取出来了当前站点cur，和最少公交数cnt，若当前站点就是终点，那就直接返回cnt。否则遍历经过当前站点的所有公交，对每辆公交，再去遍历去所有站点，若站点已经被遍历过了，直接跳过，否则就加入visited中，并和cnt+1一起组成个pair对儿排入队列中继续遍历，参见代码如下：



解法二：

```
class Solution {
public:
    int numBusesToDestination(vector<vector<int>>& routes, int S, int T) {
        if (S == T) return 0;
        unordered_map<int, unordered_set<int>> stop2bus;
        queue<pair<int, int>> q{{{S, 0}}};
        unordered_set<int> visited;
        for (int i = 0; i < routes.size(); ++i) {
            for (int j : routes[i]) {
                stop2bus[j].insert(i);
            }
        }
        while (!q.empty()) {
            int cur = q.front().first, cnt = q.front().second; q.pop();
            if (cur == T) return cnt;
            for (int bus : stop2bus[cur]) {
                for (int stop : routes[bus]) {
                    if (visited.count(stop)) continue;
                    visited.insert(stop);
                    q.push({stop, cnt + 1});
                }
            }
        }
        return -1;
    }
};
```



参考资料：

[https://leetcode.com/problems/bus-routes/](https://leetcode.com/problems/bus-routes/)

[https://leetcode.com/problems/bus-routes/discuss/122712/Simple-Java-Solution-using-BFS](https://leetcode.com/problems/bus-routes/discuss/122712/Simple-Java-Solution-using-BFS)

[https://leetcode.com/problems/bus-routes/discuss/122771/C%2B%2BJavaPython-BFS-Solution](https://leetcode.com/problems/bus-routes/discuss/122771/C%2B%2BJavaPython-BFS-Solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












