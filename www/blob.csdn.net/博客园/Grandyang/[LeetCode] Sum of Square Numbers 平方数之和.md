# [LeetCode] Sum of Square Numbers 平方数之和 - Grandyang - 博客园







# [[LeetCode] Sum of Square Numbers 平方数之和](https://www.cnblogs.com/grandyang/p/7190506.html)







Given a non-negative integer `c`, your task is to decide whether there're two integers `a` and `b` such that a2 + b2 = c.

Example 1:
Input: 5
Output: True
Explanation: 1 * 1 + 2 * 2 = 5



Example 2:
Input: 3
Output: False


这道题让我们求一个数是否能由平方数之和组成，刚开始博主没仔细看题，没有看到必须要是两个平方数之和，博主以为任意一个就可以。所以写了个带优化的递归解法，楼主已经不是上来就无脑暴力破解的辣个青葱骚年了，直接带优化。可是居然对14返回false，难道14不等于1+4+9吗，结果仔细一看，必须要两个平方数之和。好吧，那么递归都省了，直接判断两次就行了。我们可以从c的平方根，注意即使c不是平方数，也会返回一个整型数。然后我们判断如果i*i等于c，说明c就是个平方数，只要再凑个0，就是两个平方数之和，返回true；如果不等于的话，那么算出差值c - i*i，如果这个差值也是平方数的话，返回true。遍历结束后返回false，参见代码如下：



解法一：

```
class Solution {
public:
    bool judgeSquareSum(int c) {
        for (int i = sqrt(c); i >= 0; --i) {
            if (i * i == c) return true;
            int d = c - i * i, t = sqrt(d);
            if (t * t == d) return true;
        }
        return false;
    }
};
```



下面这种方法用到了集合set，从0遍历到c的平方根，对于每个i*i，都加入集合set中，然后计算c - i*i，如果这个差值也在集合set中，返回true，遍历结束返回false，参见代码如下：



解法二：

```
class Solution {
public:
    bool judgeSquareSum(int c) {
        unordered_set<int> s;
        for (int i = 0; i <= sqrt(c); ++i) {
            s.insert(i * i);
            if (s.count(c - i * i)) return true;
        }
        return false;
    }
};
```



上面两种方法都不是很高效，来看下面这种高效的解法。论坛上有人称之为二分解法，但是博主怎么觉得不是呢，虽然样子很像，但是并没有折半的操作啊。这里用a和b代表了左右两个范围，分别为0和c的平方根，然后while循环遍历，如果a*a + b*b刚好等于c，那么返回true；如果小于c，则a增大1；反之如果大于c，则b自减1，参见代码如下：



解法三：

```
class Solution {
public:
    bool judgeSquareSum(int c) {
        int a = 0, b = sqrt(c);
        while (a <= b) {
            if (a * a + b * b == c) return true;
            else if (a * a + b * b < c) ++a;
            else --b;
        }
        return false;
    }
};
```



类似题目：

[Valid Perfect Square](http://www.cnblogs.com/grandyang/p/5619296.html)



参考资料：

[https://discuss.leetcode.com/topic/94568/simple-c-solution](https://discuss.leetcode.com/topic/94568/simple-c-solution)

[https://discuss.leetcode.com/topic/94435/java-two-pointers-solution](https://discuss.leetcode.com/topic/94435/java-two-pointers-solution)

[https://discuss.leetcode.com/topic/94453/hashset-java-quick-solution-one-for-loop](https://discuss.leetcode.com/topic/94453/hashset-java-quick-solution-one-for-loop)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












