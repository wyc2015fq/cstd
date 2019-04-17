# [LeetCode] Falling Squares 下落的方块 - Grandyang - 博客园







# [[LeetCode] Falling Squares 下落的方块](https://www.cnblogs.com/grandyang/p/8486414.html)







On an infinite number line (x-axis), we drop given squares in the order they are given.

The `i`-th square dropped (`positions[i] = (left, side_length)`) is a square with the left-most point being `positions[i][0]` and sidelength `positions[i][1]`.

The square is dropped with the bottom edge parallel to the number line, and from a higher height than all currently landed squares. We wait for each square to stick before dropping the next.

The squares are infinitely sticky on their bottom edge, and will remain fixed to any positive length surface they touch (either the number line or another square). Squares dropped adjacent to each other will not stick together prematurely.



Return a list `ans` of heights. Each height `ans[i]` represents the current highest height of any square we have dropped, after dropping squares represented by `positions[0], positions[1], ..., positions[i]`.

Example 1:
Input: [[1, 2], [2, 3], [6, 1]]
Output: [2, 5, 5]
Explanation:
After the first drop of `positions[0] = [1, 2]: _aa _aa ------- `The maximum height of any square is 2.

After the second drop of `positions[1] = [2, 3]: __aaa __aaa __aaa _aa__ _aa__ -------------- `The maximum height of any square is 5. The larger square stays on top of the smaller square despite where its center of gravity is, because squares are infinitely sticky on their bottom edge.

After the third drop of `positions[1] = [6, 1]: __aaa __aaa __aaa _aa _aa___a -------------- `The maximum height of any square is still 5. Thus, we return an answer of `[2, 5, 5]`.





Example 2:
Input: [[100, 100], [200, 100]]
Output: [100, 100]
Explanation: Adjacent squares don't get stuck prematurely - only their bottom edge can stick to surfaces.



Note:
- `1 <= positions.length <= 1000`.
- `1 <= positions[i][0] <= 10^8`.
- `1 <= positions[i][1] <= 10^6`.



这道题不就是经典的俄罗斯方块么？！只不过是简化版的，我们只有方块下落，没有其他那些奇形怪状的东西，这样简化了难度，不过方块的大小不是固定的，有可能很大，但是不管方块再大，只要有一点点部分搭在其他方块上面，整个方块都会在上面，并不会掉下来，让我们求每落下一个方块后的最大高度。我们知道返回的是每落下一个方块后当前场景中的最大高度，那么返回的数组的长度就应该和落下方块的个数相同。所以我们可以建立一个heights数组，其中heights[i]表示第i块方块落下后所在的高度，那么第i块方块落下后场景的最大高度就是[0, i]区间内的最大值。那么我们在求出heights数组后，只要不停返回[0, i]区间内的最大值即可。继续来看，这道题的难点就是方块重叠的情况，我们先来想，如果各个方块不重叠，那么heights[i]的高度就是每个方块自身的高度。一旦重叠了，就得在已有的基础上再加上自身的高度。那么我们可以采用brute force的思想，对于每个一个下落的方块，我们都去看和后面将要落下的方块有没有重叠，有的话，和后面将要落下的方块的位置相比较，取二者中较大值为后面要落下的方块位置高度heights[j]。判读两个方块是否重叠的方法是如果方块2的左边界小于方块1的右边界，并且方块2点右边界大于方块1点左边界。就拿题目中的例子1来举例吧，第一个下落的方块的范围是[1, 3]，长度为2，则heights[0]=2，然后我们看其和第二个方块[2, 5]是否重叠，发现是重叠的，则heights[1]更新为2，再看第三个方块[6, 7]，不重叠，不更新。然后第二个方块落下，此时累加高度，则heights[1]=5，再看第三个方块，不重叠，不更新。然后第三个方块落下, heights[2]=1。此时我们heights数组更新好了，然后我们开始从头遍历，维护一个当前最大值curMax，每次将[0, i]中最大值加入结果res即可，参见代码如下：



解法一：

```
class Solution {
public:
    vector<int> fallingSquares(vector<pair<int, int>>& positions) {
        int n = positions.size(), cur = 0;
        vector<int> heights(n), res;
        for (int i = 0; i < n; ++i) {
            int len = positions[i].second, left = positions[i].first, right = left + len;
            heights[i] += len;
            for (int j = i + 1; j < n; ++j) {
                int l = positions[j].first, r = l + positions[j].second;
                if (l < right && r > left) {
                    heights[j] = max(heights[j], heights[i]);
                }
            }
        }
        for (int h : heights) {
            cur = max(cur, h);
            res.push_back(cur);
        }
        return res;
    }
};
```



我们来看一种时间复杂度为O(nlogn)的解法，这种解法将每一个不同高度的区间都存到了一个HashMap中，然后每当有新的方块落下的时候，可以使用二分法来快速定位到可能发生重叠的区间的位置，如果有重叠的话，将原区间再根据高度拆成多个小区间，并且一直维护一个当前出现过的最高值，并加入结果res中。我们的HashMap的映射是建立pair和int之间的映射，由于HashMap是有自动排序的功能，默认的是使用pair中第一个元素，正好就是每个方块的起始位置。然后我们遍历每个下落的方块，建立一个临时的二维数组t，用来保存拆分后的小区间。然后我们取出当前方块的起始终止位置start和end，然后我们希望在HashMap中找第一个不大于当前方块起始位置的区间，在Java中我们可以使用floorKey()函数，但是在C++中，我们只有lower_bound()和upper_bound()可以用，分别表示找第一个不大于目标值，和第一个大于目标值的区间，那么我们为了找到第一个不大于当前起始位置的区间，可以先用upper_bound()来找到第一个大于起始位置的区间，然后向前移动一个，就是第一个不大于的了。注意向前移动操作有前提条件，就是upper_bound()返回的位置不能是首位置，否则无法前移，还有就是如果前一个区间的结束为止小于等于当前区间的起始位置，说明两个区间没有重叠，我们再移回来。

下面就要进行拆分区间的核心操作了，我们用一个while循环，循环条件是it存在，并且it指向区间的起始位置小于当前区间的结束位置。由于之前的操作确定了这两个区间一定会有重叠，那么重叠的方式就有一下四种（上方为当前区间，下方为it区间）：

```
———
      | |
      ———
————————
|      |
————————
```

如上图所示，如果当前区间（上方）的起始位置大于it指向区间（下方）的起始位置，说明红色那段区间需要被拆分出来，我们将其拆分出来并存入数组t中。

```
———
  | |
  ———
    ————————
    |      |
    ————————
```

如上图所示，如果当前区间（上方）的结束位置小于it指向区间（下方）的结束位置，说明洋红色那段区间需要被拆分出来，我们将其拆分出来并存入数组t中。

```
———
  | |
  ———
————————
|      |
————————
```

如上图所示，红色区间和洋红色区间都需要拆分出来，我们将其拆分出来并存入数组t中。

```
————————
|      |
————————
  ———
  | |
  ———
```

如上图所示，底层方块被完全覆盖了，没有区间需要被拆分出来。

我们用底层it指向的区间的高度来更新h，这里的h表示当前方块下落后的基础高度，然后我们将底层it指向的区间删除，因为我们已经将没有被覆盖的区间拆分出来并存入数组t中了。注意erase()函数返回的是被删除区间的下一个位置，这样使得我们的while函数能继续判断，直到it区间和当前区间不再重叠位置。退出while循环后，我们需要将当前下落方块的区间加入HashMap中，高度为基础高度h加上自身高度len。接下来就把数组t中拆分出来的小区间都加入到HashMap中，然后用当前用h+len来更新curMax，表示当前场景最大高度，加入结果res中，参见代码如下：



解法二：

```
class Solution {
public:
    vector<int> fallingSquares(vector<pair<int, int>>& positions) {
        vector<int> res;
        map<pair<int, int>, int> m;
        int curMax = 0;
        for (auto &pos : positions) {
            vector<vector<int>> t;
            int len = pos.second, start = pos.first, end = start + len, h = 0;
            auto it = m.upper_bound({start, start});
            if (it != m.begin() && (--it)->first.second <= start) ++it;
            while (it != m.end() && it->first.first < end) {
                if (start > it->first.first) t.push_back({it->first.first, start, it->second});
                if (end < it->first.second) t.push_back({end, it->first.second, it->second});
                h = max(h, it->second);
                it = m.erase(it);
            }
            m[{start, end}] = h + len;
            for (auto &a : t) m[{a[0], a[1]}] = a[2];
            curMax = max(curMax, h + len);
            res.push_back(curMax);
        }
        return res;
    }
};
```



类似题目：

[The Skyline Problem](http://www.cnblogs.com/grandyang/p/4534586.html)



参考资料：

[https://leetcode.com/problems/falling-squares/solution/](https://leetcode.com/problems/falling-squares/solution/)

[https://leetcode.com/problems/falling-squares/discuss/108769/C++-O(nlogn)](https://leetcode.com/problems/falling-squares/discuss/108769/C++-O(nlogn))



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












