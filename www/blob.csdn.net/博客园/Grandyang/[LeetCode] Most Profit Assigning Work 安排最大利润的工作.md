# [LeetCode] Most Profit Assigning Work 安排最大利润的工作 - Grandyang - 博客园







# [[LeetCode] Most Profit Assigning Work 安排最大利润的工作](https://www.cnblogs.com/grandyang/p/10264877.html)







We have jobs: `difficulty[i]` is the difficulty of the `i`th job, and `profit[i]` is the profit of the `i`th job. 

Now we have some workers. `worker[i]` is the ability of the `i`th worker, which means that this worker can only complete a job with difficulty at most `worker[i]`. 

Every worker can be assigned at most one job, but one job can be completed multiple times.

For example, if 3 people attempt the same job that pays $1, then the total profit will be $3.  If a worker cannot complete any job, his profit is $0.

What is the most profit we can make?

Example 1:
Input: difficulty = [2,4,6,8,10], profit = [10,20,30,40,50], worker = [4,5,6,7]
Output: 100 
Explanation: Workers are assigned jobs of difficulty [4,4,6,6] and they get profit of [20,20,30,30] seperately.
Notes:
- `1 <= difficulty.length = profit.length <= 10000`
- `1 <= worker.length <= 10000`
- `difficulty[i], profit[i], worker[i]`  are in range `[1, 10^5]`



这道题给了我们一堆工作，每个工作有不同的难度，且对应着不同的利润。现在还有一些工人，每个人能胜任工作的难度不同，题目说了没人最多只能干一项工作，但是每个工作可以被多个人做。现在问我们这些工人能产生的最大利润。题目中给了一个例子，但是这个例子会给我们一些错觉，实际上difficulty数组不一定是有序的，而且可能有相同难度的工作对应不同的利润。还有就是，难度大的工作不一定利润就大，忽略了这些隐藏条件，很容易做错。为了快速的知道每个工作的难度和其对应的利润，我们可以建立难度和利润的之间映射，由于前面说了，相同的难度可能有不同的利润，所以我们希望难度映射到最高的利润，所以每次都跟自身的映射值比较一下，保留较大值即可。同时，我们还希望工作的难度能排个序，这样就可以根据工人的能力值来快速搜索能做的工作了，所以可以使用TreeMap。但是，还有个问题，前面说了难度大的工作不一定利润就大，所以我们希望难度映射的利润，是不大于其难度的工作的利润中的最大值，所以我们还要遍历一遍所有的映射，维护一个当前最大值cur，然后不断的更新每个工作的利润，同时也更新当前最大值。这些都建立好了后，之后就简单了，我们遍历每个工人，根据其能力值，来二分查第一个难度值大于该能力值的工作，可以用内置的 upper_bound 函数，如果结果第一个数字，那么我们将其前面一个难度的工作的利润加入结果res即可，参见代码如下：



解法一：

```
class Solution {
public:
    int maxProfitAssignment(vector<int>& difficulty, vector<int>& profit, vector<int>& worker) {
        int res = 0, cur = 0;
        map<int, int> m;
        for (int i = 0; i < difficulty.size(); ++i) {
            m[difficulty[i]] = max(m[difficulty[i]], profit[i]);
        }
        for (auto &a : m) {
            a.second = max(a.second, cur);
            cur = a.second;
        }
        for (int i = 0; i < worker.size(); ++i) {
            auto it = m.upper_bound(worker[i]);
            if (it != m.begin()) {
                res += (--it)->second;
            }
        }
        return res;
    }
};
```



我们也可以不用TreeMap，而是将难度和利润组成pair，加入到一个数组中，那么就算相同的难度对应不同的利润，也不会丢失数据。我们还是根据难度给所有的工作排个序，同时，我们按能力也给工人排个序，从能力低的工人开始分配工作，这样我们只需要遍历一次所有的工作，因为当能力低的工人分配了某个工作时，后面能力高的工人不需要再考虑之前的工作，因为工作已经按难度排好序了，只需要从当前位置继续往后面匹配工作，我们可以使用一个全局变量i，当工人的能力值大于等于某个工作的难度时，我们用其来更新curMax，这里的curMax记录当前工人能做的工作的最大利润，这样当工作遍历完了，或者当前工作难度已经大于工人的能力值了时就停止，这样curMax就是该工人能获得的利润，加入结果res，参见代码如下：



解法二：

```
class Solution {
public:
    int maxProfitAssignment(vector<int>& difficulty, vector<int>& profit, vector<int>& worker) {
        int res = 0, n = profit.size(), curMax = 0, i = 0;
        vector<pair<int, int>> jobs;
        for (int j = 0; j < n; ++j) {
            jobs.push_back({difficulty[j], profit[j]});
        }
        sort(jobs.begin(), jobs.end());
        sort(worker.begin(), worker.end());
        for (int ability : worker) {
            while (i < n && ability >= jobs[i].first) {
                curMax = max(curMax, jobs[i++].second);
            }
            res += curMax;
        }
        return res;
    }
};
```



我们还可以使用动态规划Dynamic Programming来做，建立一个一维的dp数组，这里的dp[i]表示能力为i的工人所能获得的最大的利润。dp数组大小初始化为100001，这是题目中给定的范围，然后我们用给定的工作的难度和利润来更新dp数组，跟解法一中的更新TreeMap有些像，要考虑相同的难度可能有不同的利润，所以每次更新的时候要跟本身比较。之后就是从1开始更新dp本身，dp[i]跟前面一个dp值比较，取较大值更新dp[i]，这个操作跟解法一中的那个更新TreeMap的映射值的操作是一样的，都是为了避免难度高的工作利润低。这里的dp数组cover了所有的能力值的范围，这样对于任何一个工人，我们可以根据其能力值马上找出其可以获得的最大利润，而不用像解法一中要使用二分查找，我们牺牲了空间，换来了常数级的查找速度，参见代码如下：



解法三：

```
class Solution {
public:
    int maxProfitAssignment(vector<int>& difficulty, vector<int>& profit, vector<int>& worker) {
        int res = 0, n = profit.size();
        vector<int> dp(100001);
        for (int i = 0; i < n; ++i) {
            dp[difficulty[i]] = max(dp[difficulty[i]], profit[i]);
        }   
        for (int i = 1; i < dp.size(); ++i) {
            dp[i] = max(dp[i], dp[i - 1]);
        }
        for (int ability : worker) {
            res += dp[ability];
        }
        return res;
    }
};
```



参考资料：

[https://leetcode.com/problems/most-profit-assigning-work/](https://leetcode.com/problems/most-profit-assigning-work/)

[https://leetcode.com/problems/most-profit-assigning-work/discuss/127133/Java-Solution-with-TreeMap](https://leetcode.com/problems/most-profit-assigning-work/discuss/127133/Java-Solution-with-TreeMap)

[https://leetcode.com/problems/most-profit-assigning-work/discuss/127031/C%2B%2BJavaPython-Sort-and-Two-pointer](https://leetcode.com/problems/most-profit-assigning-work/discuss/127031/C%2B%2BJavaPython-Sort-and-Two-pointer)

[https://leetcode.com/problems/most-profit-assigning-work/discuss/126988/O(M-%2B-N)-solution-based-on-preprocessing](https://leetcode.com/problems/most-profit-assigning-work/discuss/126988/O(M-%2B-N)-solution-based-on-preprocessing)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












