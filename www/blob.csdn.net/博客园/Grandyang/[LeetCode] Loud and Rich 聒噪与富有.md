# [LeetCode] Loud and Rich 聒噪与富有 - Grandyang - 博客园







# [[LeetCode] Loud and Rich 聒噪与富有](https://www.cnblogs.com/grandyang/p/10513992.html)








In a group of N people (labelled `0, 1, 2, ..., N-1`), each person has different amounts of money, and different levels of quietness.

For convenience, we'll call the person with label `x`, simply "person `x`".

We'll say that `richer[i] = [x, y]` if person `x` definitely has more money than person `y`.  Note that `richer` may only be a subset of valid observations.

Also, we'll say `quiet[x] = q` if person x has quietness `q`.

Now, return `answer`, where `answer[x] = y` if `y` is the least quiet person (that is, the person `y` with the smallest value of `quiet[y]`), among all people who definitely have equal to or more money than person `x`.

Example 1:

```
Input: richer = [[1,0],[2,1],[3,1],[3,7],[4,3],[5,3],[6,3]], quiet = [3,2,5,4,6,1,7,0]
Output: [5,5,2,5,4,5,6,7]
Explanation:
answer[0] = 5.
Person 5 has more money than 3, which has more money than 1, which has more money than 0.
The only person who is quieter (has lower quiet[x]) is person 7, but
it isn't clear if they have more money than person 0.

answer[7] = 7.
Among all people that definitely have equal to or more money than person 7
(which could be persons 3, 4, 5, 6, or 7), the person who is the quietest (has lower quiet[x])
is person 7.

The other answers can be filled out with similar reasoning.
```

Note:
- `1 <= quiet.length = N <= 500`
- `0 <= quiet[i] < N`, all `quiet[i]` are different.
- `0 <= richer.length <= N * (N-1) / 2`
- `0 <= richer[i][j] < N`
- `richer[i][0] != richer[i][1]`
- `richer[i]`'s are all different.
- The observations in `richer` are all logically consistent.




这道题说是有N个人，给了我们一个二维数组 richer，告诉了一些人之间的贫富关系，还有一个 quiet 数组，表示每个人的安静程度，对于每一个人，让我们找出最安静，且不比其贫穷的人，注意这里可以包括自己。说实话，博主光开始没有看懂这道题的例子，因为博主以为返回的应该安静值，其实返回的应该是人的编号才对，这样题目中的例子才能讲得通，就比如说对于编号为2的那人，richer 数组中只说明了其比编号为1的人富有，但没有显示任何人比其富有，所有返回的人应该是其本身，所以 answer[2] = 2，还有就是编号为7的人，这里编号为3，4，5，6的人都比起富有，但是其本身的安静值最低，为0，所以还是返回其本身的编号，所以answer[7] = 7。

理解了题意之后就可以开始做题了，这道题的本质其实就是有向图的遍历，LeetCode 中还是有一些类似的题目的，比如 [Course Schedule](https://www.cnblogs.com/grandyang/p/4484571.html)，[Course Schedule II](https://www.cnblogs.com/grandyang/p/4504793.html)，[Minimum Height Trees](https://www.cnblogs.com/grandyang/p/5000291.html)，和 [Reconstruct Itinerary](https://www.cnblogs.com/grandyang/p/5183210.html) 等。对于这类的题，其实解法都差不多，首先都是需要建立图的结构，一般都是用邻接链表来表示，在博主之前的那篇博客 [Possible Bipartition](https://www.cnblogs.com/grandyang/p/10317141.html)，分别使用了二维数组和 HashMap 来建立邻接链表，一般来说，使用 HashMap 能节省一些空间，且更加灵活一些，所以这里还是用 HashMap 来建立。由于要找的人必须等于或者富于自己，所以我们可以建立每个人和其所有富于自己的人的集合，因为这里除了自己，不可能有人和你一样富的人。建立好图的结构后，我们可以对每个人进行分别查找了，首先每个人的目标安静值可以初始化为自身，因为就算没有比你富有的人，你自己也可以算满足条件的人，从 HashMap 中可以直接查找到比你富有的人，他们的安静值是可以用来更新的，但是还可能有人比他们还富有，那些更富有的人的安静值也得查一遍，所以就需要用递归来做，遍历到每个比你富有的人，对他再调用递归，这样返回的就是比他富有或相等，且安静值最小的人，用这个安静值来更新当前人的安静值即可，注意我们在递归的开始首先要查一下，若某人的安静值已经更新了，直接返回即可，不用再重复计算了，参见代码如下：




解法一：

```
class Solution {
public:
    vector<int> loudAndRich\(vector<vector<int>>& richer, vector<int>& quiet) {
        vector<int> res(quiet.size(), -1);
        unordered_map<int, vector<int>> findRicher;
        for (auto a : richer) findRicher[a[1]].push_back(a[0]);
        for (int i = 0; i < quiet.size(); ++i) {
            helper(findRicher, quiet, i, res);
        }
        return res;
    }
    int helper(unordered_map<int, vector<int>>& findRicher, vector<int>& quiet, int i, vector<int>& res) {
        if (res[i] > 0) return res[i];
        res[i] = i;
        for (int j : findRicher[i]) {
            if (quiet[res[i]] > quiet[helper(findRicher, quiet, j, res)]) {
                res[i] = res[js];
            }
        }
        return res[i];
    }
};
```




我们也可以使用迭代的写法，这里还是要用邻接链表来建立图的结构，但是有所不同的是，这里我们需要建立的映射是每个人跟所有比他穷的人的集合。然后还有建立每个人的入度，我们将所有入度为0的人将入队列 queue，先开始遍历，入度为0，表示是已经条件中没有人比他更富，那么我们就可以通过 HashMap 来遍历所有比他穷的人，若当前的人的安静值小于比他穷的人的安静值，那么更新比他穷的人的安静值，可以看到这里我们每次更新的都是别人的安静值，而上面递归的解法更新的都是当前人的安静值。然后将比他穷的人的入度减1，当减到0时，加入到队列queue中继续遍历，参见代码如下：




解法二：

```
class Solution {
public:
    vector<int> loudAndRich(vector<vector<int>>& richer, vector<int>& quiet) {
        int n = quiet.size();
        vector<int> res(n, -1), inDegree(n);
        unordered_map<int, vector<int>> findPoorer;
        queue<int> q;
        for (auto a : richer) {
            findPoorer[a[0]].push_back(a[1]);
            ++inDegree[a[1]];
        }
        for (int i = 0; i < quiet.size(); ++i) {
            if (inDegree[i] == 0) q.push(i);
            res[i] = i;
        }
        while (!q.empty()) {
            int cur = q.front(); q.pop();
            for (int next : findPoorer[cur]) {
                if (quiet[res[next]] > quiet[res[cur]]) res[next] = res[cur];
                if (--inDegree[next] == 0) q.push(next);
            }
        }
        return res;
    }
};
```




类似题目：

[Course Schedule](https://www.cnblogs.com/grandyang/p/4484571.html)

[Course Schedule II](https://www.cnblogs.com/grandyang/p/4504793.html)

[Minimum Height Trees](https://www.cnblogs.com/grandyang/p/5000291.html)

[Reconstruct Itinerary](http://www.cnblogs.com/grandyang/p/5183210.html)




参考资料：

[https://leetcode.com/problems/loud-and-rich/](https://leetcode.com/problems/loud-and-rich/)

[https://leetcode.com/problems/loud-and-rich/discuss/137918/C%2B%2BJavaPython-Concise-DFS](https://leetcode.com/problems/loud-and-rich/discuss/137918/C%2B%2BJavaPython-Concise-DFS)

[https://leetcode.com/problems/loud-and-rich/discuss/138088/C%2B%2B-with-topological-sorting](https://leetcode.com/problems/loud-and-rich/discuss/138088/C%2B%2B-with-topological-sorting)




[LeetCode All in One 题目讲解汇总(持续更新中...)](https://www.cnblogs.com/grandyang/p/4606334.html)












