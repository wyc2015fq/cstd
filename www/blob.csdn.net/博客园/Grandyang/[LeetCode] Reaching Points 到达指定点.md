# [LeetCode] Reaching Points 到达指定点 - Grandyang - 博客园







# [[LeetCode] Reaching Points 到达指定点](https://www.cnblogs.com/grandyang/p/9033955.html)







A move consists of taking a point `(x, y)` and transforming it to either `(x, x+y)` or `(x+y, y)`.

Given a starting point `(sx, sy)` and a target point `(tx, ty)`, return `True` if and only if a sequence of moves exists to transform the point `(sx, sy)` to `(tx, ty)`. Otherwise, return `False`.
Examples:
Input: sx = 1, sy = 1, tx = 3, ty = 5
Output: True
Explanation:
One series of moves that transforms the starting point to the target is:
(1, 1) -> (1, 2)
(1, 2) -> (3, 2)
(3, 2) -> (3, 5)

Input: sx = 1, sy = 1, tx = 2, ty = 2
Output: False

Input: sx = 1, sy = 1, tx = 1, ty = 1
Output: True


Note:
- `sx, sy, tx, ty` will all be integers in the range `[1, 10^9]`.



这道题说有一个点的变换方式，就是可以将 (x, y) 变成 (x + y, y) 或 (x, x + y)，然后给了我们两个坐标点，一个是起始点 (sx, sy)，一个是目标点 (tx, ty)，问我们利用这种转换方式能否将起始点转换为目标点。这题给了一个很大的限定条件，就是所有坐标的数字都是正数，即所有的点都在第一象限里。这是个很大的限定条件，因为这样的话，坐标数字变换的过程中总是在不断的变大，因为没有负数的存在，就不会缩小。这样的话，只要我们发现只要起始点中的横纵坐标有任意一个大于了目标点的横纵坐标，那么直接返回false就可以了。首先，我们可以先来想想brute force怎么做，非常的直截了当，先判断之前说的返回false的两种情况，然后判断如果起始点等于了目标点，返回true，然后就是调用两个递归函数了，毫无悬念的TLE了，可以参见[官方解法贴](https://leetcode.com/problems/reaching-points/solution/)中的代码。令博主感到意外的是，这道题的时间要求很苛刻啊，官方解题贴中的前三种解法全都TLE了，但是经过分析后发现，前三种方法TLE是必然的，因为没用使用到此题想考察的核心要点。这道题的标签是Math，对于博主来说，标记为Math的题跟brainteaser没啥区别，因为都很难想出来。我们再想想为啥brute force的解法会超时，如果起始点的数字很小的话，而目标点的数字特别的巨大，那么仅仅通过加法来慢慢的累加到的一个巨大的数，怎么可能不超时。快速累加的高效方法是乘法，但我们要知道需要累加的个数，就需要用除法来计算，其实我们对累加的个数也不那么感兴趣，我们其实对余数感兴趣，那么求余运算就是很高效的方法。求余运算是将数字变小的操作，那么其实我们可以将目标数字缩小，看能能否缩小到起始位置，也是符合题意的，只不过此时的变换方式由加法变为了减法而已。

我们的目标是将tx和ty分别缩小到sx和sy，不可能一步就缩小到位，那么这肯定是一个循环，终止条件是tx和ty中任意一个小于了sx和sy，那么在循环内部，我们想要缩小tx或ty，先缩小两者中较大的那个，若tx大于ty，我们可以尝试缩小tx，但是如果此时ty等于sy了话，我们可以迅速判断出结果，通过计算此时tx和sx的差值是否是ty的倍数，因为此时ty不能改变了，只能缩小tx，若能通过减去整数倍数个ty得到sx的，就表示可以到达。如果ty不等于sy的话，那么直接tx对ty取余即可。同理，若ty大于tx，我们可以尝试缩小ty，但是如果此时tx等于sx了话，我们可以迅速判断出结果，通过计算此时ty和sy的差值是否是tx的倍数，如果tx不等于sx的话，那么直接ty对tx取余即可。循环退出后检测起始点和目标点是否相等，参见代码如下：



解法一：

```
class Solution {
public:
    bool reachingPoints(int sx, int sy, int tx, int ty) {
        while (tx >= sx && ty >= sy) {
            if (tx > ty) {
                if (ty == sy) return (tx - sx) % ty == 0;
                tx %= ty;
            } else {
                if (tx == sx) return (ty - sy) % tx == 0;
                else ty %= tx;
            }
        }
        return tx == sx && ty == sy;
    }
};
```



下面这种解法将没有在循环内部处理相等的情况，而是无脑缩小tx和ty，最后循环退出后，再来判断tx和ty的关系，然后快速的判断，由于取余运算缩小的太快了，所以博主不认为二者的运行效率能差多少，参见代码如下：



解法二：

```
class Solution {
public:
    bool reachingPoints(int sx, int sy, int tx, int ty) {
        while (tx >= sx && ty >= sy) {
            if (tx > ty) tx %= ty;
            else ty %= tx;
        }
        if (tx > ty) return tx == sx && ty == sy % sx;
        return ty == sy && tx == sx % sy;
    }
};
```



下面这种写法跟上面的没有太大的区别，就看起来更加整齐一些，博主有强迫症吗？？？



解法三：

```
class Solution {
public:
    bool reachingPoints(int sx, int sy, int tx, int ty) {
        while (tx >= sx && ty >= sy) {
            if (tx > ty) tx %= ty;
            else ty %= tx;
        }
        if (tx == sx) return (ty - sy) % sx == 0;
        if (ty == sy) return (tx - sx) % sy == 0;
        return false;
    }
};
```



再来看一种递归的写法，就四行啊，其实就是用取余运算代替加法优化了brute force的解法，参见代码如下：



解法四：

```
class Solution {
public:
    bool reachingPoints(int sx, int sy, int tx, int ty) {
        if (tx < sx || ty < sy) return false;
        if (tx == sx && (ty - sy) % sx == 0) return true;
        if (ty == sy && (tx - sx) % sy == 0) return true;
        return reachingPoints(sx, sy, tx % ty, ty % tx);
    }
};
```



参考资料：

[https://leetcode.com/problems/reaching-points/solution/](https://leetcode.com/problems/reaching-points/solution/)

[https://leetcode.com/problems/reaching-points/discuss/114726/C++-Simple-iterative.](https://leetcode.com/problems/reaching-points/discuss/114726/C++-Simple-iterative.)

[https://leetcode.com/problems/reaching-points/discuss/122916/concise-c++-solution](https://leetcode.com/problems/reaching-points/discuss/122916/concise-c++-solution)

[https://leetcode.com/problems/reaching-points/discuss/116110/C++-iterative-solution](https://leetcode.com/problems/reaching-points/discuss/116110/C++-iterative-solution)

[https://leetcode.com/problems/reaching-points/discuss/114732/Java-Simple-solution-with-explanation](https://leetcode.com/problems/reaching-points/discuss/114732/Java-Simple-solution-with-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












