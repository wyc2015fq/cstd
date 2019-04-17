# [LeetCode] Car Fleet 车队 - Grandyang - 博客园







# [[LeetCode] Car Fleet 车队](https://www.cnblogs.com/grandyang/p/10540136.html)







`N` cars are going to the same destination along a one lane road.  The destination is `target` miles away.

Each car `i` has a constant speed `speed[i]` (in miles per hour), and initial position `position[i]` miles towards the target along the road.

A car can never pass another car ahead of it, but it can catch up to it, and drive bumper to bumper at the same speed.

The distance between these two cars is ignored - they are assumed to have the same position.

A *car fleet* is some non-empty set of cars driving at the same position and same speed.  Note that a single car is also a car fleet.

If a car catches up to a car fleet right at the destination point, it will still be considered as one car fleet.
How many car fleets will arrive at the destination?

Example 1:

```
Input: target = 12, position = [10,8,0,5,3], speed = [2,4,1,1,3]
Output: 3
Explanation:
The cars starting at 10 and 8 become a fleet, meeting each other at 12.
The car starting at 0 doesn't catch up to any other car, so it is a fleet by itself.
The cars starting at 5 and 3 become a fleet, meeting each other at 6.
Note that no other cars meet these fleets before the destination, so the answer is 3.\
Note:
```
- `0 <= N <= 10 ^ 4`
- `0 < target <= 10 ^ 6`
- `0 < speed[i] <= 10 ^ 6`
- `0 <= position[i] < target`
- All initial positions are different.




这道题说是路上有一系列的车，车在不同的位置，且分别有着不同的速度，但行驶的方向都相同。如果后方的车在到达终点之前追上前面的车了，那么它就会如痴汉般尾随在其后，且速度降至和前面的车相同，可以看作是一个车队，当然，单独的一辆车也可以看作是一个车队，问我们共有多少个车队到达终点。这道题是小学时候的应用题的感觉，什么狗追人啊，人追狗啊之类的。这道题的正确解法的思路其实不太容易想，因为我们很容易把注意力都集中到每辆车，去计算其每个时刻所在的位置，以及跟前面的车相遇的位置，这其实把这道题想复杂了，我们其实并不需要知道车的相遇位置，只关心是否能组成车队一同经过终点线，那么如何才能知道是否能一起过线呢，最简单的方法就是看时间，假如车B在车A的后面，而车B到终点线的时间小于等于车A，那么就知道车A和B一定会组成车队一起过线。这样的话，我们就可以从离终点最近的一辆车开始，先算出其撞线的时间，然后再一次遍历身后的车，若后面的车撞线的时间小于等于前面的车的时间，则会组成车队。反之，若大于前面的车的时间，则说明无法追上前面的车，于是自己会形成一个新的车队，且是车头，则结果 res 自增1即可。

思路有了，就可以具体实现了，我们使用一个 TreeMap 来建立小车位置和其到达终点时间之间的映射，这里的时间使用 double 型，通过终点位置减去当前位置，并除以速度来获得。我们希望能从 position 大的小车开始处理，而 TreeMap 是把小的数字排在前面，这里使用了个小 trick，就是映射的时候使用的是 position 的负数，这样就能先处理原来 position 大的车，从而统计出正确的车队数量，参见代码如下：




解法一：

```
class Solution {
public:
    int carFleet(int target, vector<int>& position, vector<int>& speed) {
        int res = 0; double cur = 0;
        map<int, double> pos2time;
        for (int i = 0; i < position.size(); ++i) {
            pos2time[-position[i]] = (double)(target - position[i]) / speed[i];
        }
        for (auto a : pos2time) {
            if (a.second <= cur) continue;
            cur = a.second;
            ++res;
        }
        return res;
    }
};
```




我们也可以使用优先队列来做，由于其是按照从大到小的顺序自动排列的，所以不用使用上面的小 trick。还有一点和上面的不同的是，我们的并没有在开始就计算过线时间，而是直接存的是速度，因为存整型肯定比存 double 型的要节省空间。在之后处理的时候，我们再取出位置和速度计算时间，然后再进行跟上面相同的操作即可，参见代码如下：




解法二：

```
class Solution {
public:
    int carFleet(int target, vector<int>& position, vector<int>& speed) {
        int res = 0; double cur = 0;
        priority_queue<pair<int, int>> q;
        for (int i = 0; i < position.size(); ++i) {
            q.push({position[i], speed[i]});
        }   
        while (!q.empty()) {
            auto t = q.top(); q.pop();
            double timeNeeded = (double)(target - t.first) / t.second;
            if (timeNeeded <= cur) continue;
            cur = timeNeeded;
            ++res;
        }
        return res;
    }
};
```




参考资料：

[https://leetcode.com/problems/car-fleet/](https://leetcode.com/problems/car-fleet/)

[https://leetcode.com/problems/car-fleet/discuss/180287/Java-Priority-Queue-Explained](https://leetcode.com/problems/car-fleet/discuss/180287/Java-Priority-Queue-Explained)

[https://leetcode.com/problems/car-fleet/discuss/139850/C%2B%2BJavaPython-Straight-Forward](https://leetcode.com/problems/car-fleet/discuss/139850/C%2B%2BJavaPython-Straight-Forward)



[LeetCode All in One 题目讲解汇总(持续更新中...)](https://www.cnblogs.com/grandyang/p/4606334.html)












