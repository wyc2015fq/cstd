# [LeetCode] The Skyline Problem 天际线问题 - Grandyang - 博客园







# [[LeetCode] The Skyline Problem 天际线问题](https://www.cnblogs.com/grandyang/p/4534586.html)







A city's skyline is the outer contour of the silhouette formed by all the buildings in that city when viewed from a distance. Now suppose you are given the locations and height of all the buildings as shown on a cityscape photo (Figure A), write a program to output the skyline formed by these buildings collectively (Figure B).

![Buildings](http://i62.tinypic.com/2a0e4qu.jpg)![Skyline Contour](http://i61.tinypic.com/2vtdqtf.jpg)

The geometric information of each building is represented by a triplet of integers `[Li, Ri, Hi]`, where `Li` and `Ri` are the x coordinates of the left and right edge of the ith building, respectively, and `Hi` is its height. It is guaranteed that `0 ≤ Li, Ri ≤ INT_MAX`, `0 < Hi ≤ INT_MAX`, and `Ri - Li > 0`. You may assume all buildings are perfect rectangles grounded on an absolutely flat surface at height 0.

For instance, the dimensions of all buildings in Figure A are recorded as: `[ [2 9 10], [3 7 15], [5 12 12], [15 20 10], [19 24 8] ] `.

The output is a list of "key points" (red dots in Figure B) in the format of `[ [x1,y1], [x2, y2], [x3, y3], ... ]` that uniquely defines a skyline. A key point is the left endpoint of a horizontal line segment. Note that the last key point, where the rightmost building ends, is merely used to mark the termination of the skyline, and always has zero height. Also, the ground in between any two adjacent buildings should be considered part of the skyline contour.

For instance, the skyline in Figure B should be represented as:`[ [2 10], [3 15], [7 12], [12 0], [15 10], [20 8], [24, 0] ]`.

Notes:
- The number of buildings in any input list is guaranteed to be in the range `[0, 10000]`.
- The input list is already sorted in ascending order by the left x position `Li`.
- The output list must be sorted by the x position.
- There must be no consecutive horizontal lines of equal height in the output skyline. For instance, `[...[2 3], [4 5], [7 5], [11 5], [12 7]...]` is not acceptable; the three lines of height 5 should be merged into one in the final output as such: `[...[2 3], [4 5], [12 7], ...]`



Credits:
Special thanks to [@stellari](https://oj.leetcode.com/discuss/user/stellari) for adding this problem, creating these two awesome images and all test cases.



这道题一打开又是图又是这么长的题目的，看起来感觉应该是一道相当复杂的题，但是做完之后发现也就那么回事，虽然我不会做，是学习的别人的解法。这道求天际线的题目应该算是比较新颖的题，要是非要在之前的题目中找一道类似的题，也就只有[Merge Intervals 合并区间](http://www.cnblogs.com/grandyang/p/4370601.html)了吧，但是与那题不同的是，这道题不是求被合并成的空间，而是求轮廓线的一些关键的转折点，这就比较复杂了，我们通过仔细观察题目中给的那个例子可以发现，要求的红点都跟每个小区间的左右区间点有密切的关系，而且进一步发现除了每一个封闭区间的最右边的结束点是楼的右边界点，其余的都是左边界点，而且每个红点的纵坐标都是当前重合处的最高楼的高度，但是在右边界的那个楼的就不算了。在网上搜了很多帖子，发现网友[Brian Gordon的帖子](https://briangordon.github.io/2014/08/the-skyline-problem.html)图文并茂，什么动画渐变啊，横向扫描啊，简直叼到没朋友啊，但是叼到极致后就懒的一句一句的去读了，这里博主还是讲解另一位网友[百草园的博客](http://www.cnblogs.com/easonliu/p/4531020.html)吧。这里用到了multiset数据结构，其好处在于其中的元素是按堆排好序的，插入新元素进去还是有序的，而且执行删除元素也可方便的将元素删掉。这里为了区分左右边界，将左边界的高度存为负数，建立左边界和负高度的pair，再建立右边界和高度的pair，存入数组中，都存进去了以后，给数组按照左边界排序，这样我们就可以按顺序来处理那些关键的节点了。我们要在multiset中放入一个0，这样在某个没有和其他建筑重叠的右边界上，我们就可以将封闭点存入结果res中。下面我们按顺序遍历这些关键节点，如果遇到高度为负值的pair，说明是左边界，那么将正高度加入multiset中，然后取出此时集合中最高的高度，即最后一个数字，然后看是否跟pre相同，这里的pre是上一个状态的高度，初始化为0，所以第一个左边界的高度绝对不为0，所以肯定会存入结果res中。接下来如果碰到了一个更高的楼的左边界的话，新高度存入multiset的话会排在最后面，那么此时cur取来也跟pre不同，可以将新的左边界点加入结果res。第三个点遇到绿色建筑的左边界点时，由于其高度低于红色的楼，所以cur取出来还是红色楼的高度，跟pre相同，直接跳过。下面遇到红色楼的右边界，此时我们首先将红色楼的高度从multiset中删除，那么此时cur取出的绿色楼的高度就是最高啦，跟pre不同，则可以将红楼的右边界横坐标和绿楼的高度组成pair加到结果res中，这样就成功的找到我们需要的拐点啦，后面都是这样类似的情况。当某个右边界点没有跟任何楼重叠的话，删掉当前的高度，那么multiset中就只剩0了，所以跟当前的右边界横坐标组成pair就是封闭点啦，具体实现参看代码如下：



```
class Solution {
public:
    vector<pair<int, int>> getSkyline(vector<vector<int>>& buildings) {
        vector<pair<int, int>> h, res;
        multiset<int> m;
        int pre = 0, cur = 0;
        for (auto &a : buildings) {
            h.push_back({a[0], -a[2]});
            h.push_back({a[1], a[2]});
        }
        sort(h.begin(), h.end());
        m.insert(0);
        for (auto &a : h) {
            if (a.second < 0) m.insert(-a.second);
            else m.erase(m.find(a.second));
            cur = *m.rbegin();
            if (cur != pre) {
                res.push_back({a.first, cur});
                pre = cur;
            }
        }
        return res;
    }
};
```



参考资料：

[http://www.cnblogs.com/easonliu/p/4531020.html](http://www.cnblogs.com/easonliu/p/4531020.html)

[https://discuss.leetcode.com/topic/22482/short-java-solution](https://discuss.leetcode.com/topic/22482/short-java-solution)

[https://briangordon.github.io/2014/08/the-skyline-problem.html](https://briangordon.github.io/2014/08/the-skyline-problem.html)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












