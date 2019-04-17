# [LeetCode] Valid Square 验证正方形 - Grandyang - 博客园







# [[LeetCode] Valid Square 验证正方形](https://www.cnblogs.com/grandyang/p/6914746.html)









Given the coordinates of four points in 2D space, return whether the four points could construct a square.

The coordinate (x,y) of a point is represented by an integer array with two integers.

Example:
Input: p1 = [0,0], p2 = [1,1], p3 = [1,0], p4 = [0,1]
Output: True



Note:
- All the input integers are in the range [-10000, 10000].
- A valid square has four equal sides with positive length and four equal angles (90-degree angles).
- Input points have no order.



这道题给了我们四个点，让我们验证这四个点是否能组成一个正方形，刚开始博主考虑的方法是想判断四个角是否是直角，但即便四个角都是直角，也不能说明一定就是正方形，还有可能是矩形。还得判断各边是否相等。其实我们可以仅通过边的关系的来判断是否是正方形，根据初中几何的知识我们知道正方形的四条边相等，两条对角线相等，满足这两个条件的四边形一定是正方形。那么这样就好办了，我们只需要对四个点，两两之间算距离，如果计算出某两个点之间距离为0，说明两点重合了，直接返回false，如果不为0，那么我们就建立距离和其出现次数之间的映射，最后如果我们只得到了两个不同的距离长度，那么就说明是正方形了，参见代码如下：



解法一：

```
class Solution {
public:
    bool validSquare(vector<int>& p1, vector<int>& p2, vector<int>& p3, vector<int>& p4) {
        unordered_map<int, int> m;
        vector<vector<int>> v{p1, p2, p3, p4};
        for (int i = 0; i < 4; ++i) {
            for (int j = i + 1; j < 4; ++j) {
                int x1 = v[i][0], y1 = v[i][1], x2 = v[j][0], y2 = v[j][1];
                int dist = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
                if (dist == 0) return false;
                ++m[dist];
            }
        }
        return m.size() == 2;
    }
};
```



我们其实不用建立映射，直接用个集合set来放距离就行了，如果最后集合中不存在0，且里面只有两个数的时候，说明是正方形，参见代码如下：



解法二：

```
class Solution {
public:
    bool validSquare(vector<int>& p1, vector<int>& p2, vector<int>& p3, vector<int>& p4) {
        unordered_set<int> s{d(p1, p2), d(p1, p3), d(p1, p4), d(p2, p3), d(p2, p4), d(p3, p4)};
        return !s.count(0) && s.size() == 2;
    }
    int d(vector<int>& p1, vector<int>& p2) {
        return (p1[0] - p2[0]) * (p1[0] - p2[0]) + (p1[1] - p2[1]) * (p1[1] - p2[1]);
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/89985/c-3-lines-unordered_set](https://discuss.leetcode.com/topic/89985/c-3-lines-unordered_set)





[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












