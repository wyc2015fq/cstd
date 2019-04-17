# [LeetCode] Race Car 赛车 - Grandyang - 博客园







# [[LeetCode] Race Car 赛车](https://www.cnblogs.com/grandyang/p/10360655.html)







Your car starts at position 0 and speed +1 on an infinite number line.  (Your car can go into negative positions.)

Your car drives automatically according to a sequence of instructions A (accelerate) and R (reverse).

When you get an instruction "A", your car does the following: `position += speed, speed *= 2`.

When you get an instruction "R", your car does the following: if your speed is positive then `speed = -1` , otherwise `speed = 1`.  (Your position stays the same.)

For example, after commands "AAR", your car goes to positions 0->1->3->3, and your speed goes to 1->2->4->-1.

Now for some target position, say the length of the shortest sequence of instructions to get there.
Example 1:
Input: 
target = 3
Output: 2
Explanation: 
The shortest instruction sequence is "AA".
Your position goes from 0->1->3.
Example 2:
Input: 
target = 6
Output: 5
Explanation: 
The shortest instruction sequence is "AAARA".
Your position goes from 0->1->3->7->7->6.



Note:
- `1 <= target <= 10000`.



这道题是关于赛车的题（估计韩寒会比较感兴趣吧，从《后会无期》，到《乘风破浪》，再到《飞驰人生》，貌似每一部都跟车有关，正所谓不会拍电影的赛车手不是好作家，哈哈～）。好，不多扯了，来做题吧，以下讲解主要参考了[fun4LeetCode大神的帖子](https://leetcode.com/problems/race-car/discuss/124326/Summary-of-the-BFS-and-DP-solutions-with-intuitive-explanation)。说是起始时有个小车在位置0，速度为1，有个目标位置target，是小车要到达的地方。而小车只有两种操作，第一种是加速操作，首先当前位置加上小车速度，然后小车速度乘以2。第二种是反向操作，小车位置不变，小车速度重置为单位长度，并且反向。问我们最少需要多少个操作才能到达target。我们首先来看下若小车一直加速的话，都能经过哪些位置，从起点开始，若小车连加五次速，位置的变化为：

0 -> 1 -> 3 -> 7 -> 15 -> 31

有没有发现这些数字很眼熟，没有的话，就每个数字都加上个1，那么就应该眼熟了吧，对于信仰1024的程序猿来说，不眼熟不行啊，这就变成了2的指数数列啊，那么我们得出了结论，当小车从0开始连加n个速的话，其将会到达位置 2^n - 1。我们可以看出，小车越往后，位置跨度越大，那么当target不在这些位置上，很有可能一脚油门就开过了，比如，target = 6 的话，小车在3的位置上，一脚油门，就到7了，这时候就要回头，回头后，速度变为-1，此时正好就到达6了，那么小车的操作如下：

Initial:    pos -> 0,    speed -> 1

A:    　　pos -> 1,    speed -> 2

A:    　　pos -> 3,    speed -> 4

A:    　　pos -> 7,    speed -> 8

R:    　　pos -> 7,    speed -> -1

A:    　　**pos -> 6**,    speed -> -2

所以，我们只需要5步就可以了。但是还有个问题，假如回头了以后，一脚油门之后，又过站了怎么办？比如 target = 5 的时候，之前小车回头之后到达了6的位置，此时速度已经是-2了，再加个速，就直接干到了位置4，就得再回头，那么这种方式小车的操作如下：

Initial:    pos -> 0,    speed -> 1

A:    　　pos -> 1,    speed -> 2

A:    　　pos -> 3,    speed -> 4

A:    　　pos -> 7,    speed -> 8

R:    　　pos -> 7,    speed -> -1

A:    　　pos -> 6,    speed -> -2

A:    　　pos -> 4,    speed -> -4

R:    　　pos -> 4,    speed -> 1

A:    　　**pos ->**** 5**,    speed -> 2

那么此时我们就用了8步，但这是最优的方法么，我们一定要在过了目标才回头么，不撞南墙不回头么？其实不必，我们可以在到达target之前提前调头，然后往回走走，再调回来，使得之后能恰好到达target，比如下面这种走法：

Initial:    pos -> 0,    speed -> 1

A:    　　pos -> 1,    speed -> 2

A:    　　pos -> 3,    speed -> 4

R:    　　pos -> 3,    speed -> -1

A:    　　pos -> 2,    speed -> -2

R:    　　pos -> 2,    speed -> 1

A:    　　pos -> 3,    speed -> 2

A:    　　**pos ->**** 5**,    speed -> 4

我们在未到达target的位置3时就直接掉头了，往后退到2，再调回来，往前走，到达5，此时总共只用了7步，是最优解。那么我们怎么知道啥时候要掉头？问得好，答案是不知道，我们得遍历每种情况。但是为了避免计算一些无用的情况，比如小车反向过了起点，或者是超过target好远都不回头，我们需要限定一些边界，比如小车不能去小于0的位置，以及小车在超过了target时，就必须回头了，不能继续往前开了。还有就是小车当前的位置不能超过target*2，不过这个限制条件博主还没有想出合理的解释，各位看官大神们知道的话可以给博主讲讲～

对于求极值的题目，根据博主多年与LeetCode抗争的经验，就是BFS，带剪枝的DFS解法，贪婪算法，或者动态规划Dynamic Programming这几种解法（带记忆数组的DFS解法也可以归到DP一类中去）。一般来说，贪婪算法比较naive，大概率会跪。BFS有时候可以，带剪枝的DFS解法中的剪枝比较难想，而DP绝对是神器，基本没有解决不了的问题，但是代价就是得抓破头皮想状态转移方程，并且一般DP只能用来求极值，而想求极值对应的具体情况（比如这道题如果让求最少个数的指令是什么），有时候可能还得用带剪枝的DFS解法。不过这道题BFS也可以，那么我们就先用BFS来解吧。

这里的BFS解法，跟迷宫遍历中的找最短路径很类似，可以想像成水波，一圈一圈的往外扩散，当碰到target时候，当前的半径就是最短距离。用队列queue来辅助遍历，里面放的是位置和速度的pair对儿，将初始状态位置0速度1先放进queue，然后需要一个HashSet来记录处理过的状态，为了节省空间和加快速度，我们将位置和速度变为字符串，并在中间加逗号隔开，这样HashSet中只要保存字符串即可。之后开始while循环，此时采用的是层序遍历的写法，当前queue中所有元素遍历完了之后，结果res才自增1。在for循环中，首先取出队首pair对儿的位置和速度，如果位置和target相等，直接返回结果res。否则就要去新的地方了，首先尝试的是加速操作，此时新的位置newPos为之前的位置加速度，新的速度newSpeed为之前速度的2倍，然后将newPos和newSpeed加码成字符串，若新的状态没有处理过，且新位置大于0，小于target*2的话，则将新状态加入visited，并排入队列中。接下来就是转向的情况，newPos和原位置保持不变，newSpeed根据之前speed的正负变成-1或1，然后将newPos和newSpeed加码成字符串，若新的状态没有处理过，且新位置大于0，小于target*2的话，则将新状态加入visited，并排入队列中。for循环结束后，结果res自增1即可，参见代码如下：



解法一：

```
class Solution {
public:
    int racecar(int target) {
        int res = 0;
        queue<pair<int, int>> q{{{0, 1}}};
        unordered_set<string> visited{{"0,1"}};
        while (!q.empty()) {
            for (int i = q.size(); i > 0; --i) {
                int pos = q.front().first, speed = q.front().second; q.pop();
                if (pos == target) return res;
                int newPos = pos + speed, newSpeed = speed * 2;
                string key = to_string(newPos) + "," + to_string(newSpeed);
                if (!visited.count(key) && newPos > 0 && newPos < (target * 2)) {
                    visited.insert(key);
                    q.push({newPos, newSpeed});
                }
                newPos = pos; 
                newSpeed = (speed > 0) ? -1 : 1;
                key = to_string(newPos) + "," + to_string(newSpeed);
                if (!visited.count(key) && newPos > 0 && newPos < (target * 2)) {
                    visited.insert(key);
                    q.push({newPos, newSpeed});
                }
            }
            ++res;
        }
        return -1;
    }
};
```



好，既然说了DP是神器，那么就来用用这传说中的神器吧。首先来定义dp数组吧，就用一个一维的dp数组，长度为target+1，其中dp[i]表示到达位置i，所需要的最少的指令个数。接下来就是推导最难的状态转移方程了，这里我们不能像BFS解法一样对每个状态都无脑尝试加速和反向操作，因为状态转移方程是要跟之前的状态建立联系的。根据之前的分析，对于某个位置i，我们有两种操作，一种是在到达该位置之前，回头两次，另一种是超过该位置后再回头，我们就要模拟这两种情况。

首先来模拟位置i之前回头两次的情况，那么这里我们就有正向加速，和反向加速两种可能。我们假设正向加速能到达的位置为j，正向加速次数为cnt1，反向加速能到达的位置为k，反向加速的次数为cnt2。那么正向加速位置j从1开始遍历，不能超过i，且根据之前的规律，j每次的更新应该是 2^cnt1 - 1，然后对于每个j位置，我们都要反向跑一次，此时反向加速位置k从0开始遍历，不能超过j，k每次更新应该是 2^cnt2 - 1，那么到达此时的位置时，我们正向走了j，反向走了k，即可表示为正向走了(j - k)，此时的指令数为 cnt1 + 1 + cnt2 + 1，加的2个‘1’分贝是反向操作的两次计数，当我们第二次反向后，此时的方向就是朝着i的方向了，此时跟i之间的距离可以直接用差值在dp数组中取，为dp[i - (j - k)]，以此来更新dp[i]。

接下来模拟超过i位置后才回头的情况，此时cnt1是刚好能超过或到达i位置的加速次数，我们可以直接使用，此时我们比较i和j，若相等，则直接用cnt1来更新dp[i]，否则就反向操作一次，然后距离差为j-i，从dp数组中直接调用 dp[j-i]，然后加上反向操作1次，用来更新dp[i]，最终返回 dp[target] 即为所求，参见代码如下：



解法二：

```
class Solution {
public:
    int racecar(int target) {
        vector<int> dp(target + 1);
        for (int i = 1; i <= target; ++i) {
            dp[i] = INT_MAX;
            int j = 1, cnt1 = 1;
            for (; j < i; j = (1 << ++cnt1) - 1) {
                for (int k = 0, cnt2 = 0; k < j; k = (1 << ++cnt2) - 1) {
                    dp[i] = min(dp[i], cnt1 + 1 + cnt2 + 1 + dp[i - (j - k)]);
                }
            }
            dp[i] = min(dp[i], cnt1 + (i == j ? 0 : 1 + dp[j - i]));
        }
        return dp[target];
    }
};
```



下面是DP的递归写法，跟上面的迭代写法并没有太大的区别，只是将直接从dp数组中取值的过程变成了调用递归函数，参见代码如下：



解法三：

```
class Solution {
public:
    int racecar(int target) {
        vector<int> dp(target + 1, -1);
        dp[0] = 0;
        return helper(target, dp);
    }
    int helper(int i, vector<int>& dp) {
        if (dp[i] >= 0) return dp[i];
        dp[i] = INT_MAX;
        int j = 1, cnt1 = 1;
        for (; j < i; j = (1 << ++cnt1) - 1) {
            for (int k = 0, cnt2 = 0; k < j; k = (1 << ++cnt2) - 1) {
                dp[i] = min(dp[i], cnt1 + 1 + cnt2 + 1 + helper(i - (j - k), dp));
            }
        }
        dp[i] = min(dp[i], cnt1 + (i == j ? 0 : 1 + helper(j - i, dp)));
        return dp[i];
    }
};
```



参考资料：

[https://leetcode.com/problems/race-car/](https://leetcode.com/problems/race-car/)

[https://leetcode.com/problems/race-car/discuss/123834/C%2B%2BJavaPython-DP-solution](https://leetcode.com/problems/race-car/discuss/123834/C%2B%2BJavaPython-DP-solution)

[https://leetcode.com/problems/race-car/discuss/124326/Summary-of-the-BFS-and-DP-solutions-with-intuitive-explanation](https://leetcode.com/problems/race-car/discuss/124326/Summary-of-the-BFS-and-DP-solutions-with-intuitive-explanation)




[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













