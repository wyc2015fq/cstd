# [LeetCode] Employee Free Time 职员的空闲时间 - Grandyang - 博客园







# [[LeetCode] Employee Free Time 职员的空闲时间](https://www.cnblogs.com/grandyang/p/8552586.html)








We are given a list `schedule` of employees, which represents the working time for each employee.

Each employee has a list of non-overlapping `Intervals`, and these intervals are in sorted order.

Return the list of finite intervals representing common, positive-length free time for *all* employees, also in sorted order.

Example 1:
Input: schedule = [[[1,2],[5,6]],[[1,3]],[[4,10]]]
Output: [[3,4]]
Explanation:
There are a total of three employees, and all common
free time intervals would be [-inf, 1], [3, 4], [10, inf].
We discard any intervals that contain inf as they aren't finite.



Example 2:
Input: schedule = [[[1,3],[6,7]],[[2,4]],[[2,5],[9,12]]]
Output: [[5,6],[7,9]]



(Even though we are representing `Intervals` in the form `[x, y]`, the objects inside are `Intervals`, not lists or arrays. For example, `schedule[0][0].start = 1, schedule[0][0].end = 2`, and `schedule[0][0][0]` is not defined.)

Also, we wouldn't include intervals like [5, 5] in our answer, as they have zero length.

Note:
- `schedule` and `schedule[i]` are lists with lengths in range `[1, 50]`.
- `0 <= schedule[i].start < schedule[i].end <= 10^8`.




这道题和之前那道[Merge Intervals](http://www.cnblogs.com/grandyang/p/4370601.html)基本没有太大的区别，那道题是求合并后的区间，这道题求合并后区间中间不相连的区间。那么只要我们合并好了区间，就很容易做了。那么我么首先应该给所有的区间排个序，按照起始位置从小到大来排。因为我们总不可能一会处理前面的，一会处理后面的区间。排好序以后，我们先取出第一个区间赋给t，然后开始遍历所有的区间内所有的区间，如果t的结束位置小于当前遍历到的区间i的起始位置，说明二者没有交集，那么把不相交的部分加入结果res中，然后把当前区间i赋值给t；否则如果区间t和区间i有交集，那么我们更新t的结束位置为二者中的较大值，因为按顺序遍历区间的时候，区间t的结束位置是比较的基准，越大越容易和后面的区间进行合并，参见代码如下：



解法一：

```
class Solution {
public:
    vector<Interval> employeeFreeTime(vector<vector<Interval>>& schedule) {
        vector<Interval> res, v;
        for (auto a : schedule) {
            v.insert(v.end(), a.begin(), a.end());
        }
        sort(v.begin(), v.end(), [](Interval &a, Interval &b) {return a.start < b.start;});
        Interval t = v[0];
        for (Interval i : v) {
            if (t.end < i.start) {
                res.push_back(Interval(t.end, i.start));
                t = i;
            } else {
                t = (t.end < i.end) ? i : t;
            }
        }
        return res;
    }
};
```



我们再来看一种解法，这种解法挺巧妙的，我们使用TreeMap建立一个位置和其出现次数之间的映射，对于起始位置，进行正累加，对于结束位置，进行负累加。由于TreeMap具有自动排序的功能，所以我们进行遍历的时候，就是从小到大进行遍历的。定义一个变量cnt，初始化为0，我们对于每个遍历到的数，都加上其在TreeMap中的映射值，即该数字出现的次数，起始位置的话就会加正数，结束位置就是加负数。开始的时候，第一个数字一定是个起始位置，那么cnt就是正数，那么接下来cnt就有可能加上正数，或者减去一个负数，我们想，如果第一个区间和第二个区间没有交集的话，那么接下来遇到的数字就是第一个区间的结束位置，所以会减去1，这样此时cnt就为0了，这说明一定会有中间区域存在，所以我们首先把第一个区间当前起始位置，结束位置暂时放上0，组成一个区间放到结果res中，这样我们在遍历到下一个区间的时候更新结果res中最后一个区间的结束位置。语言描述难免太干巴巴的，我们拿题目中的例1来说明，建立好的TreeMap如下所示：

1 -> 2
2 -> -1
3 -> -1
4 -> 1
5 -> 1
6 -> -1
10 -> -1

那么开始遍历这所有的映射对，cnt首先为2，然后往后遍历下一个映射对2 -> -1，此时cnt为1了，不进行其他操作，再往下遍历，下一个映射对3 -> -1，此时cnt为0了，说明后面将会出现断层了，我们将(3, 0)先存入结果res中。然后遍历到4 -> 1时，cnt为1，此时将结果res中的(3, 0)更新为 (3, 4)。然后到5 -> 1，此时cnt为2，不进行其他操作，然后到6 -> -1，此时cnt为1，不进行其他操作，然后到10 -> -1，此时cnt为0，将(10, 0)加入结果res中。由于后面再没有任何区间了，所以res最后一个区间不会再被更新了，我们应该将其移出结果res，因为题目中限定了区间不能为无穷，参见代码如下：



解法二：

```
class Solution {
public:
    vector<Interval> employeeFreeTime(vector<vector<Interval>>& schedule) {
        vector<Interval> res;
        map<int, int> m;
        int cnt = 0;
        for (auto employee : schedule) {
            for (Interval i : employee) {
                ++m[i.start];
                --m[i.end];
            }
        }
        for (auto a : m) {
            cnt += a.second;
            if (!cnt) res.push_back(Interval(a.first, 0));
            if (cnt && !res.empty() && !res.back().end) res.back().end = a.first;
        }
        if (!res.empty()) res.pop_back();
        return res;
    }
};
```



类似题目：

[Merge Intervals](http://www.cnblogs.com/grandyang/p/4370601.html)



参考资料：

[https://leetcode.com/problems/employee-free-time/discuss/113127/C++-Clean-Code](https://leetcode.com/problems/employee-free-time/discuss/113127/C++-Clean-Code)

[https://leetcode.com/problems/employee-free-time/discuss/113134/Simple-Java-Sort-Solution-Using-(Priority-Queue)-or-Just-ArrayList](https://leetcode.com/problems/employee-free-time/discuss/113134/Simple-Java-Sort-Solution-Using-(Priority-Queue)-or-Just-ArrayList)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












