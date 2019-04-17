# [LeetCode] Random Pick with Blacklist 带黑名单的随机选取 - Grandyang - 博客园







# [[LeetCode] Random Pick with Blacklist 带黑名单的随机选取](https://www.cnblogs.com/grandyang/p/10029772.html)







Given a blacklist `B` containing unique integers from `[0, N)`, write a function to return a uniform random integer from `[0, N)` which is NOT in `B`.

Optimize it such that it minimizes the call to system’s `Math.random()`.

Note:
- `1 <= N <= 1000000000`
- `0 <= B.length < min(100000, N)`
- `[0, N)` does NOT include N. See [interval notation](https://en.wikipedia.org/wiki/Interval_(mathematics)).

Example 1:
Input: 
["Solution","pick","pick","pick"]
[[1,[]],[],[],[]]
Output: [null,0,0,0]

Example 2:
Input: 
["Solution","pick","pick","pick"]
[[2,[]],[],[],[]]
Output: [null,1,1,1]

Example 3:
Input: 
["Solution","pick","pick","pick"]
[[3,[1]],[],[],[]]
Output: [null,0,0,2]

Example 4:
Input: 
["Solution","pick","pick","pick"]
[[4,[2]],[],[],[]]
Output: [null,1,3,1]

Explanation of Input Syntax:

The input is two lists: the subroutines called and their arguments. `Solution`'s constructor has two arguments, `N` and the blacklist `B`. `pick` has no arguments. Arguments are always wrapped with a list, even if there aren't any.



这道题让我们生成一个N以内的随机数，但是还给了一个黑名单，意思是黑名单里面的数字不能被选到。于是博主最先想到的方法就是用[拒绝采样Rejection Sampling](https://en.wikipedia.org/wiki/Rejection_sampling)来做，因为之前做过使用该方法的两道题 [Implement Rand10() Using Rand7()](https://www.cnblogs.com/grandyang/p/9727206.html) 和 [Generate Random Point in a Circle](https://www.cnblogs.com/grandyang/p/9741220.html)，所以可以立马想到。思路其实很简单，就是随机一个数，如果是黑名单里的，那么就重新随机。为了提高在黑名单中查找数字的速度，我们将所有黑名单的数字放到一个HashSet中，这样我们就拥有了常数级查找的速度，看似一切水到渠成，燃鹅被OJ强行打脸，TLE！那么换一种思路吧，既然你有黑名单，那么林北就有白名单，把所有没被block的数字都放到一个新数组中，然后随机生成数组坐标不就完了。燃鹅x2，又被OJ放倒了，MLE！不准用这么多内存。岂可修，真的没别的办法了嘛？！还好方法解答贴中给了一种使用HashMap的方法来做，博主仔细研读了一番，发现确实秒啊！既然数字总共有N个，那么减去黑名单中数字的个数，就是最多能随机出来的个数。比如N=5，黑名单中有两个数{2, 4}，那么我们最多只能随机出三个，但是我们如果直接rand()%3，会得到0，1，2，我们发现有两个问题，一是黑名单中的2可以随机到，二是数字3没法随机到。那么我们想，能不能随机到0或1则返回其本身，而当随机到2到时候，我们返回的是3，我们需要建立这样的映射，这就是使用HashMap的动机啦。我们首先将超过N - blacklist.size()的数字放入一个HashSet，这里就把{3, 4}放进去了，然后我们遍历blacklist中的数字，如果在HashSet中的话，就将其删除，这样HashSet中就只有{3}了，这个需要建立映射的数字，而用什么数字建立，当然是用黑名单中的数字了，遍历黑名单中的数字，如果小于N - blacklist.size()的话，说明是有可能随机到的，我们和HashSet中的第一个数字建立映射，然后我们可以用个iterator，指向HashSet中的下一个数组，然后继续建立映射。从而实现在pick函数中的移魂换影大法了，先随机个数字，如果有映射，则返回映射值，否则返回原数字，参见代码如下：



```
class Solution {
public:
    Solution(int N, vector<int> blacklist) {
        unordered_set<int> st;
        len = N - blacklist.size();
        for (int i = len; i < N; ++i) st.insert(i);
        for (int num : blacklist) st.erase(num);
        auto it = st.begin();
        for (int num : blacklist) {
            if (num < len) m[num] = *it++;
        }
    }
    
    int pick() {
        int k = rand() % len;
        return m.count(k) ? m[k] : k;
    }

private:
    unordered_map<int, int> m;
    int len;
};
```



类似题目：

[Random Pick with Weight](https://www.cnblogs.com/grandyang/p/9784690.html)

[Random Pick Index](http://www.cnblogs.com/grandyang/p/5875509.html)



参考资料：

[https://leetcode.com/problems/random-pick-with-blacklist/](https://leetcode.com/problems/random-pick-with-blacklist/)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












