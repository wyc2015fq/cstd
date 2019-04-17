# [LeetCode] IPO 上市 - Grandyang - 博客园







# [[LeetCode] IPO 上市](https://www.cnblogs.com/grandyang/p/6788811.html)







Suppose LeetCode will start its IPO soon. In order to sell a good price of its shares to Venture Capital, LeetCode would like to work on some projects to increase its capital before the IPO. Since it has limited resources, it can only finish at most k distinct projects before the IPO. Help LeetCode design the best way to maximize its total capital after finishing at most k distinct projects.

You are given several projects. For each project i, it has a pure profit Pi and a minimum capital of Ci is needed to start the corresponding project. Initially, you have W capital. When you finish a project, you will obtain its pure profit and the profit will be added to your total capital.

To sum up, pick a list of at most k distinct projects from given projects to maximize your final capital, and output your final maximized capital.

Example 1:
Input: k=2, W=0, Profits=[1,2,3], Capital=[0,1,1].

Output: 4

Explanation: Since your initial capital is 0, you can only start the project indexed 0.
             After finishing it you will obtain profit 1 and your capital becomes 1.
             With capital 1, you can either start the project indexed 1 or the project indexed 2.
             Since you can choose at most 2 projects, you need to finish the project indexed 2 to get the maximum capital.
             Therefore, output the final maximized capital, which is 0 + 1 + 3 = 4.



Note:
- You may assume all numbers in the input are non-negative integers.
- The length of Profits array and Capital array will not exceed 50,000.
- The answer is guaranteed to fit in a 32-bit signed integer.



这道题上来就让人眼前一亮，剑指上市，每个创业公司都有一个上市的梦想吧，博主认为照现在这种发展趋势，感觉上市并非遥不可及，资瓷一下。这道题说初始时我们的资本为0，可以交易k次，并且给了我们提供了交易所需的资本和所能获得的利润，让我们求怎样选择k次交易，使我们最终的资本最大。虽然题目中给我们的资本数组是有序的，但是OJ里的test case肯定不都是有序的，还有就是不一定需要资本大的交易利润就多，该遍历的时候还得遍历。我们可以用贪婪算法来解，每一次都选择资本范围内最大利润的进行交易，那么我们首先应该建立资本和利润对，然后根据资本的大小进行排序，然后我们根据自己当前的资本，用二分搜索法在有序数组中找第一个大于当前资本的交易的位置，然后往前退一步就是最后一个不大于当前资本的交易，然后向前遍历，找到利润最大的那个的进行交易，把利润加入资本W中，然后将这个交易对删除，这样我们就可以保证在进行k次交易后，我们的总资本最大，参见代码如下：



解法一：

```
class Solution {
public:
    int findMaximizedCapital(int k, int W, vector<int>& Profits, vector<int>& Capital) {
        vector<pair<int, int>> v;
        for (int i = 0; i < Capital.size(); ++i) {
            v.push_back({Capital[i], Profits[i]});
        }
        sort(v.begin(), v.end());
        for (int i = 0; i < k; ++i) {
            int left = 0, right = v.size(), mx = 0, idx = 0;
            while (left < right) {
                int mid = left + (right - left) / 2;
                if (v[mid].first <= W) left = mid + 1;
                else right = mid;
            }
            for (int j = right - 1; j >= 0; --j) {
                if (mx < v[j].second) {
                    mx = v[j].second;
                    idx = j;
                }
            }
            W += mx;
            v.erase(v.begin() + idx);
        }
        return W;
    }
};
```



看论坛上的大神们都比较喜欢用一些可以自动排序的数据结构来做，比如我们可以使用一个最大堆和一个最小堆，把资本利润对放在最小堆中，这样需要资本小的交易就在队首，然后从队首按顺序取出资本小的交易，如果所需资本不大于当前所拥有的资本，那么就把利润资本存入最大堆中，注意这里资本和利润要翻个，因为我们希望把利润最大的交易放在队首，便于取出，这样也能实现我们的目的，参见代码如下：



解法二：

```
class Solution {
public:
    int findMaximizedCapital(int k, int W, vector<int>& Profits, vector<int>& Capital) {
        priority_queue<pair<int, int>> maxH;
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> minH;
        for (int i = 0; i < Capital.size(); ++i) {
            minH.push({Capital[i], Profits[i]});
        }
        for (int i = 0; i < k; ++i) {
            while (!minH.empty() && minH.top().first <= W) {
                auto t = minH.top(); minH.pop();
                maxH.push({t.second, t.first}); 
            }
            if (maxH.empty()) break;
            W += maxH.top().first; maxH.pop();
        }
        return W;
    }
};
```



下面这种方法跟上面的解法思路完全一样，就是数据结构有些变化，我们用multiset来模拟最小堆，然后最大堆还是用优先队列来实现，不过是需要存利润值就行了，不需要存对应的资本了，参见代码如下： 



解法三：

```
class Solution {
public:
    int findMaximizedCapital(int k, int W, vector<int>& Profits, vector<int>& Capital) {
        priority_queue<int> q;
        multiset<pair<int, int>> s;
        for (int i = 0; i < Capital.size(); ++i) {
            s.insert({Capital[i], Profits[i]});
        }
        for (int i = 0; i < k; ++i) {
            for (auto it = s.begin(); it != s.end(); ++it) {
                if (it->first > W) break;
                q.push(it->second);
                s.erase(it);
            }
            if (q.empty()) break;
            W += q.top(); q.pop();
        }
        return W;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/77768/very-simple-greedy-java-solution-using-two-priorityqueues](https://discuss.leetcode.com/topic/77768/very-simple-greedy-java-solution-using-two-priorityqueues)

[https://discuss.leetcode.com/topic/78574/8-liner-c-42ms-beat-98-greedy-algorithm-detailed-explanation](https://discuss.leetcode.com/topic/78574/8-liner-c-42ms-beat-98-greedy-algorithm-detailed-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












