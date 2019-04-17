# [LeetCode] Generate Random Point in a Circle 生成圆中的随机点 - Grandyang - 博客园







# [[LeetCode] Generate Random Point in a Circle 生成圆中的随机点](https://www.cnblogs.com/grandyang/p/9741220.html)







Given the radius and x-y positions of the center of a circle, write a function `randPoint` which generates a uniform random point in the circle.

Note:
- input and output values are in [floating-point](https://www.webopedia.com/TERM/F/floating_point_number.html).
- radius and x-y position of the center of the circle is passed into the class constructor.
- a point on the circumference of the circle is considered to be in the circle.
- `randPoint` returns a size 2 array containing x-position and y-position of the random point, in that order.


Example 1:
Input: 
["Solution","randPoint","randPoint","randPoint"]
[[1,0,0],[],[],[]]
Output: [null,[-0.72939,-0.65505],[-0.78502,-0.28626],[-0.83119,-0.19803]]


Example 2:
Input: 
["Solution","randPoint","randPoint","randPoint"]
[[10,5,-7.5],[],[],[]]
Output: [null,[11.52438,-8.33273],[2.46992,-16.21705],[11.13430,-12.42337]]

Explanation of Input Syntax:

The input is two lists: the subroutines called and their arguments. `Solution`'s constructor has three arguments, the radius, x-position of the center, and y-position of the center of the circle. `randPoint` has no arguments. Arguments are always wrapped with a list, even if there aren't any.




这道题给了我们一个圆，包括中点位置和半径，让我们随机生成圆中的任意一个点。这里说明了圆上也当作是圆中，而且这里的随机意味着要等概率。思绪飘回了高中时代，努力搜索着那一丝丝残留的记忆，终于，我把还给老师的知识又要了回来，圆的方程表示为 (x - a) ^ 2 + (y - b) ^ 2 = r ^ 2，这里的 (a, b) 是圆心位置，r为半径。那么我们想如何生成圆中的任意位置呢，如果用这种方式来生成，那么我们先随机出一个x，那么随机出y的时候还要考虑其是否在圆中间，比较麻烦。继续回到高中时代，模糊的记忆中飘来了三个字，极坐标。是的，圆还可以用极坐标的形式来表示，我们只需随机出一个角度theta，再随机出一个小于半径的长度，这样我们就可以得到圆中的坐标位置了，哦耶～ 那么先来生成theta吧，我们知道一圈是360度，即2pi，所以我们随机出一个 [0, 1] 中的小数，再乘以2pi，就可以了。然后就是随机小于半径的长度，这里有个问题需要注意一下，我们并不是直接随机出一个 [0, 1] 中的小数再乘以半径r，而是要对随机出的[0, 1] 中的小数取个平方根再乘以半径r。这是为啥呢，简单来说，是为了保证等概率。如果不用平方根的话，那么表示圆的时候 (len * cos(theta)) ^ 2 + (len * sin(theta) ^ 2，这里就相当于对随机出的[0, 1] 中的小数平方了，那么其就不是等概率的了，因为两个小于1的小数相乘了，其会更加靠近0，这就是为啥我们要平方一下的原因。最后在求点位置的时候要加上圆心的偏移即可，参见代码如下：



解法一：

```
class Solution {
public:
    Solution(double radius, double x_center, double y_center) {
        r = radius; centerX = x_center; centerY = y_center;
    }
    
    vector<double> randPoint() {
        double theta = 2 * M_PI * ((double)rand() / RAND_MAX);
        double len = sqrt((double)rand() / RAND_MAX) * r;
        return {centerX + len * cos(theta), centerY + len * sin(theta)};
    }

private:
    double r, centerX, centerY;
};
```



我们也可以不用极坐标来做，由于之前刚做过[Implement Rand10() Using Rand7()](https://www.cnblogs.com/grandyang/p/9727206.html)，对其中的拒绝采样Rejection Sampling还有印象，所以我们也可以用其来做。这其实就是拒绝采样的经典应用，在一个正方形中有均匀分布的点，让我们随机出其内切圆中的一个点，那么就是我们随机出x和y之后，然后算其平方和，如果小于等于r平方，说明其在圆内，我们可以返回其坐标，记得加上圆心偏移，否则我们重新进行采样。关于拒绝采样的方法可以参见我之前那篇博客[Implement Rand10() Using Rand7()](https://www.cnblogs.com/grandyang/p/9727206.html)，参见代码如下：



解法二：

```
class Solution {
public:
    Solution(double radius, double x_center, double y_center) {
        r = radius; centerX = x_center; centerY = y_center;
    }
    
    vector<double> randPoint() {
        while (true) {
            double x = (2 * (double)rand() / RAND_MAX - 1.0) * r;
            double y = (2 * (double)rand() / RAND_MAX - 1.0) * r;
            if (x * x + y * y <= r * r) return {centerX + x, centerY + y};
        }
    }

private:
    double r, centerX, centerY;
};
```



类似题目：

[Implement Rand10() Using Rand7()](https://www.cnblogs.com/grandyang/p/9727206.html)



参考资料：

[https://leetcode.com/problems/generate-random-point-in-a-circle/](https://leetcode.com/problems/generate-random-point-in-a-circle/)

[https://leetcode.com/problems/generate-random-point-in-a-circle/discuss/154405/C%2B%2B-solution-polar-coordinates](https://leetcode.com/problems/generate-random-point-in-a-circle/discuss/154405/C%2B%2B-solution-polar-coordinates)

[https://leetcode.com/problems/generate-random-point-in-a-circle/discuss/169518/C%2B%2B-AC-solution-using-rejection-sampling](https://leetcode.com/problems/generate-random-point-in-a-circle/discuss/169518/C%2B%2B-AC-solution-using-rejection-sampling)

[https://leetcode.com/problems/generate-random-point-in-a-circle/discuss/155650/Explanation-with-Graphs-why-using-Math.sqrt()](https://leetcode.com/problems/generate-random-point-in-a-circle/discuss/155650/Explanation-with-Graphs-why-using-Math.sqrt())



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












