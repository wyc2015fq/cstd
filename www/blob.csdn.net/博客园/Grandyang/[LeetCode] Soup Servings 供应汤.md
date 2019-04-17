# [LeetCode] Soup Servings 供应汤 - Grandyang - 博客园







# [[LeetCode] Soup Servings 供应汤](https://www.cnblogs.com/grandyang/p/9406434.html)









There are two types of soup: type A and type B. Initially we have `N` ml of each type of soup. There are four kinds of operations:
- Serve 100 ml of soup A and 0 ml of soup B
- Serve 75 ml of soup A and 25 ml of soup B
- Serve 50 ml of soup A and 50 ml of soup B
- Serve 25 ml of soup A and 75 ml of soup B

When we serve some soup, we give it to someone and we no longer have it.  Each turn, we will choose from the four operations with equal probability 0.25. If the remaining volume of soup is not enough to complete the operation, we will serve as much as we can.  We stop once we no longer have some quantity of both types of soup.

Note that we do not have the operation where all 100 ml's of soup B are used first.  

Return the probability that soup A will be empty first, plus half the probability that A and B become empty at the same time.


Example:
Input: N = 50
Output: 0.625
Explanation: 
If we choose the first two operations, A will become empty first. For the third operation, A and B will become empty at the same time. For the fourth operation, B will become empty first. So the total probability of A becoming empty first plus half the probability that A and B become empty at the same time, is 0.25 * (1 + 1 + 0.5 + 0) = 0.625.


Notes:
- `0 <= N <= 10^9`. 
- Answers within `10^-6` of the true value will be accepted as correct.





这道题给了我们两种汤，A和B，开始时各给了N毫升的。然后说是有下面四种操作：

1. 供应100毫升A汤，0毫升B汤。

2. 供应75毫升A汤，25毫升B汤。

3. 供应50毫升A汤，50毫升B汤。

4. 供应25毫升A汤，75毫升B汤。

我们选择每种操作的概率是一样的，让我们返回A汤先供应完的概率加上A汤和B汤同时供应完的一半概率。又给了一个例子来帮助我们理解。说实话，博主觉得这道题挺让人费解的，反正博主是没有啥思路，是直接研究答案的，现在就照着大神们的帖子来讲一讲吧。

先来看这四种操作，由于概率相同，所以每一种操作都的有，所以这四种操作可以想象成迷宫遍历的周围四个方向，那么我们就可以用递归来做。再看一下题目中给的N的范围，可以到10的9次方，而每次汤的消耗最多不过100毫升，由于纯递归基本就是暴力搜索，所以我们需要加上记忆数组memo，来避免重复运算，提高运行的效率。既然用记忆数组，我们不想占用太多空间，可以对工件进行优化。怎么优化呢，我们发现汤的供应量都是25的倍数，所以我们可以将25毫升当作一份汤的量，所以这四种操作就变成了：

1. 供应4份A汤，0份B汤。

2. 供应3份A汤，1份B汤。

3. 供应2份A汤，2份B汤。

4. 供应1份A汤，3份B汤。

所以我们的汤份数就是可以通过除以25来获得，由于N可能不是25的倍数，会有余数，但是就算不到一份的量，也算是完成了一个操作，所以我们可以直接加上24再除以25就可以得到正确的份数。那么接下来就是调用递归了，其实递归函数很直接了当，首先判断如果两种汤都没了，那么返回0.5，因为题目中说了如果两种汤都供应完了，返回一半的概率；如果A汤没了，返回1；如果B汤没了，返回0；如果上面的情况都没有进入，说明此时A汤和B汤都有剩余，所以我们先查记忆数组memo，如果其大于0，说明当前情况已经被计算过了，我们直接返回该值即可。如果没有的话，我们就要计算这种情况的值，通过对四种情况分别调用递归函数中，将返回的概率值累加后除以4即可。这道题还有一个很大的优化，就是当N大过某一个数值的时候，返回的都是1。这里的4800就是这个阈值返回，这样的话memo数组的大小就可以是200x200了，至于是怎么提前设定的，博主就不知道了，估计是强行试出来的吧，参见代码如下：



解法一：

```
class Solution {
public:
    double memo[200][200];
    double soupServings(int N) {
        return N >= 4800 ? 1.0 : f((N + 24) / 25, (N + 24) / 25);
    }
    double f(int a, int b) {
        if (a <= 0 && b <= 0) return 0.5;
        if (a <= 0) return 1.0;
        if (b <= 0) return 0;
        if (memo[a][b] > 0) return memo[a][b];
        memo[a][b] = 0.25 * (f(a - 4, b) + f(a - 3, b - 1) + f(a - 2, b - 2) + f(a - 1, b - 3));
        return memo[a][b];
    }
};
```



下面这种解法的思路基本一样，就是没有用二维数组，而是用了一个HashMap来保存计算过的值，建立字符串到double到映射，这里的字符串是由A汤和B汤的剩余量拼成的，为了保证唯一性，将二者的值先转为字符串，然后在中间加一个冒号拼在一起。由于是字符串，所以我们也不用将毫升数变成份数，直接就原样保存吧，参见代码如下：



解法二：

```
class Solution {
public:
    unordered_map<string, double> m;
    double soupServings(int N) {
        return N >= 4800 ? 1.0 : f(N, N);
    }
    double f(int a, int b) {
        if (a <= 0 && b <= 0) return 0.5;
        if (a <= 0) return 1.0;
        if (b <= 0) return 0;
        string spoon = to_string(a) + ":" + to_string(b);
        if (!m.count(spoon)) {
            m[spoon] = 0.25 * (f(a - 100, b) + f(a - 75, b - 25) + f(a - 50, b - 50) + f(a - 25, b - 75));
        }
        return m[spoon];
    }
};
```



参考资料：

[https://leetcode.com/problems/soup-servings/discuss/125809/Java-soup-(spoon-included)](https://leetcode.com/problems/soup-servings/discuss/125809/Java-soup-(spoon-included))

[https://leetcode.com/problems/soup-servings/discuss/121711/C++JavaPython-When-N-greater-4800-just-return-1](https://leetcode.com/problems/soup-servings/discuss/121711/C++JavaPython-When-N-greater-4800-just-return-1)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












