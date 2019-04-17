# [LeetCode] Exam Room 考试房间 - Grandyang - 博客园







# [[LeetCode] Exam Room 考试房间](https://www.cnblogs.com/grandyang/p/10618437.html)








In an exam room, there are `N` seats in a single row, numbered `0, 1, 2, ..., N-1`.

When a student enters the room, they must sit in the seat that maximizes the distance to the closest person.  If there are multiple such seats, they sit in the seat with the lowest number.  (Also, if no one is in the room, then the student sits at seat number 0.)

Return a class `ExamRoom(int N)` that exposes two functions: `ExamRoom.seat()` returning an `int` representing what seat the student sat in, and `ExamRoom.leave(int p)` representing that the student in seat number `p` now leaves the room.  It is guaranteed that any calls to `ExamRoom.leave(p)` have a student sitting in seat `p`.

Example 1:

```
Input: ["ExamRoom","seat","seat","seat","seat","leave","seat"], [[10],[],[],[],[],[4],[]]
Output: [null,0,9,4,2,null,5]
Explanation:
ExamRoom(10) -> null
seat() -> 0, no one is in the room, then the student sits at seat number 0.
seat() -> 9, the student sits at the last seat number 9.
seat() -> 4, the student sits at the last seat number 4.
seat() -> 2, the student sits at the last seat number 2.
leave(4) -> null
seat() -> 5, the student sits at the last seat number 5.
```

​​​​​​​

Note:
- `1 <= N <= 10^9`
- `ExamRoom.seat()` and `ExamRoom.leave()` will be called at most `10^4` times across all test cases.
- Calls to `ExamRoom.leave(p)` are guaranteed to have a student currently sitting in seat number `p`.




这道题是之前那道 [Maximize Distance to Closest Person](https://www.cnblogs.com/grandyang/p/10503789.html) 的拓展，说是有个考场，每个考生入座的时候都要尽可能的跟左右两边的人距离保持最大，当最大距离相同的时候，考生坐在座位编号较小的那个位置。对于墙的处理跟之前那道是一样的，能靠墙就尽量靠墙，这样肯定离别人最远。其实在之前那道题 [Maximize Distance to Closest Person](https://www.cnblogs.com/grandyang/p/10503789.html) 最后的讨论部分博主就预言了这道题，当时博主还没有看这道题，果然是要我们返回座位的具体位置。

博主最先想的方法是用一个大小为N的数组来表示所有的座位，初始化为0，表示没有一个人，若有人入座了，则将该位置变为1，离开则变为0，那么对于 leave() 函数就十分简单了，直接将对应位置改为0即可。重点就是 seat() 函数了，这个可以借鉴之前那道 [Maximize Distance to Closest Person](https://www.cnblogs.com/grandyang/p/10503789.html) 的思路，采用双指针来做，主要就是找连续的0进行处理，还是要分 start 是否为0的情况，因为空位从墙的位置开始，跟空位在两人之间的处理情况是不同的，若空位从墙开始，肯定是坐墙边，而若是在两人之间，则需要坐在最中间，还要记得更新 start 为下一个空座位。最后在处理末尾空位连到墙的时候，跟之前稍有些不同，因为题目要求当最大距离相同的时候，需要选择座位号小的位置，而当此时 start 为0的时候，说明所有的位置都是空的，那么我们不需要再更新 idx 了，就用其初始值0，表示就坐在第一个位置，是符合题意的。最后别忘了将 idx 位置赋值为1，表示有人坐了。

看到这里，你一定以为大功告成了吧，but，OJ总是不断给人惊喜，Time Limit Exceeded，貌似不让我们这么轻易过关啊。那么只能进行优化了，首先分析上面的思路哪块比较耗时，其实就是遍历，当有大量的空座位的时候，中间的0还得一个个遍历，不是很高效，那么比较直接的改进方法就是去掉那些0，我们只保存有人坐的位置，即所有1的位置。这样省去了遍历0的时间，大大提高了效率，此时我们就可以使用 TreeSet 来保存1的位置，其余部分并不怎么需要改变，在确定了座位 idx 时，将其加入 TreeSet 中。在 leave() 中，直接移除离开人的座位位置即可，参见代码如下：




```
class ExamRoom {
public:
    ExamRoom(int N) {
        n = N;
    }
    
    int seat() {
        int start = 0, mx = 0, idx = 0;
        for (int i : spots) {
            if (start == 0) {
                if (mx < i - start) {
                    mx = i - start;
                    idx = 0;
                }
            } else {
                if (mx < (i - start + 1) / 2) {
                    mx = (i - start + 1) / 2;
                    idx = start + mx - 1;
                }
            }
            start = i + 1;
        }
        if (start > 0 && mx < n - start) {
            mx = n - start;
            idx = n - 1;
        }
        spots.insert(idx);
        return idx;
    }
    
    void leave(int p) {
        spots.erase(p);
    }

private:
    int n;
    set<int> spots;
};
```




讨论：若这道题还有 follow up 的话，那么博主能想到的就是变成二维的数组，这样才更像一个真正的考场啊，毕竟大多数考场都不是只有一排座位的。变成了二维的话，那么周围四面八方的人的距离都要考虑呢，想想觉得还挺难的，大家有什么想法的话，欢迎留言讨论哈～




类似题目：

[Maximize Distance to Closest Person](https://www.cnblogs.com/grandyang/p/10503789.html)




参考资料：

[https://leetcode.com/problems/exam-room/](https://leetcode.com/problems/exam-room/)

[https://leetcode.com/problems/exam-room/discuss/139862/C%2B%2BJavaPython-Straight-Forward](https://leetcode.com/problems/exam-room/discuss/139862/C%2B%2BJavaPython-Straight-Forward)

[https://leetcode.com/problems/exam-room/discuss/148595/Java-PriorityQueue-with-customized-object.-seat%3A-O(logn)-leave-O(n)-with-explanation](https://leetcode.com/problems/exam-room/discuss/148595/Java-PriorityQueue-with-customized-object.-seat%3A-O(logn)-leave-O(n)-with-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](https://www.cnblogs.com/grandyang/p/4606334.html)












