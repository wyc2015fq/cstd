# [LeetCode] Champagne Tower 香槟塔 - Grandyang - 博客园







# [[LeetCode] Champagne Tower 香槟塔](https://www.cnblogs.com/grandyang/p/9286537.html)







We stack glasses in a pyramid, where the first row has 1 glass, the second row has 2 glasses, and so on until the 100th row.  Each glass holds one cup (250ml) of champagne.

Then, some champagne is poured in the first glass at the top.  When the top most glass is full, any excess liquid poured will fall equally to the glass immediately to the left and right of it.  When those glasses become full, any excess champagne will fall equally to the left and right of those glasses, and so on.  (A glass at the bottom row has it's excess champagne fall on the floor.)

For example, after one cup of champagne is poured, the top most glass is full.  After two cups of champagne are poured, the two glasses on the second row are half full.  After three cups of champagne are poured, those two cups become full - there are 3 full glasses total now.  After four cups of champagne are poured, the third row has the middle glass half full, and the two outside glasses are a quarter full, as pictured below.

![](https://s3-lc-upload.s3.amazonaws.com/uploads/2018/03/09/tower.png)

Now after pouring some non-negative integer cups of champagne, return how full the j-th glass in the i-th row is (both i and j are 0 indexed.)


Example 1:
Input: poured = 1, query_glass = 1, query_row = 1
Output: 0.0
Explanation: We poured 1 cup of champange to the top glass of the tower (which is indexed as (0, 0)). There will be no excess liquid so all the glasses under the top glass will remain empty.

Example 2:
Input: poured = 2, query_glass = 1, query_row = 1
Output: 0.5
Explanation: We poured 2 cups of champange to the top glass of the tower (which is indexed as (0, 0)). There is one cup of excess liquid. The glass indexed as (1, 0) and the glass indexed as (1, 1) will share the excess liquid equally, and each will get half cup of champange.



Note:
- `poured` will be in the range of `[0, 10 ^ 9]`.
- `query_glass` and `query_row` will be in the range of `[0, 99]`.



这道题用高脚杯摆了个金字塔，貌似在电影里见过这种酷炫的效果，不过好像还是3D的，组了个立体的酒杯金字塔。这道题中的金字塔是2D的，降低了一些难度。在我们最开始没有什么思路的时候，我们就从最简单的开始分析吧：

当只倒一杯酒的时候，只有最顶端的酒杯被填满。

当倒二杯酒的时候，最顶端的酒杯被填满，且第二层的两个酒杯各自装了一半。

当倒三杯酒的时候，最顶端的酒杯被填满，且第二层的两个酒杯也被填满。

当倒四杯酒的时候，最顶端的酒杯被填满，且第二层的两个酒杯也被填满，第三层的三个酒杯分别被填了四分之一，二分之一，和四分之一。

当倒五杯酒的时候，最顶端的酒杯被填满，且第二层的两个酒杯也被填满，第三层的三个酒杯分别被填了二分之一，填满，和二分之一。

...

如果酒是无限的，那么最终每个酒杯就会被填满，所以难点就是怎么知道在倒K杯酒后，当前的酒杯还剩多少。不管酒量又多大，当前酒杯最多只能装一杯，多余的酒都会流到下一行的两个酒杯。那么比如我们总共倒了五杯酒，那么最顶端的酒杯只能留住一杯，剩下的四杯全部均分到下行的酒杯中了，而离其最近的下一行的两个酒杯会平均分到其多出来的酒量。那么第二层的酒杯分别会得到(5-1)/2=2杯。而第二层的两个酒杯也分别只能留住一杯，各自多余的一杯还要往第三层流，那么第三层的第一个杯子接住了第二层的第一个杯子流下的半杯，而第三层的第二个杯子接住了第二层的两个杯子各自流下的半杯，于是填满了。第三层的第三个杯子接住了第二层的第二个杯子流下的半杯。那么我们的思路应该就是处理每一个杯子，将多余的酒量均分到其下一层对应的两个酒杯中，我们只需要处理到query_row那一行即可，如果地query_glass中的酒量超过一杯了，那么我们返回1就行了，因为多余的还会往下流，但我们不需要再考虑了。

我们建立一个二维的dp数组，其中dp[i][j]表示第i行第j列的杯子将要接住的酒量（可能大于1，因为此时还没有进行多余往下流的处理），那么我们就逐个遍历即可，将多余的酒量均分加入下一行的两个酒杯中即可，参见代码如下：



解法一：

```
class Solution {
public:
    double champagneTower(int poured, int query_row, int query_glass) {
        vector<vector<double>> dp(101, vector<double>(101, 0));
        dp[0][0] = poured;
        for (int i = 0; i <= query_row; ++i) {
            for (int j = 0; j <= i; ++j) {
                if (dp[i][j] >= 1) {
                    dp[i + 1][j] += (dp[i][j] - 1) / 2.0;
                    dp[i + 1][j + 1] += (dp[i][j] - 1) / 2.0;
                }
            }
        }
        return min(1.0, dp[query_row][query_glass]);
    }
};
```



我们可以对上面的代码进行空间上的优化，只用一个一维数组即可，参见代码如下：



解法二：

```
class Solution {
public:
    double champagneTower(int poured, int query_row, int query_glass) {
        vector<double> dp(101, 0);
        dp[0] = poured;
        for (int i = 1; i <= query_row; ++i) {
            for (int j = i; j >= 0; --j) {
                dp[j + 1] += dp[j] = max(0.0, (dp[j] - 1) / 2.0);
            }
        }
        return min(1.0, dp[query_glass]);
    }
};
```



参考资料：

[https://leetcode.com/problems/champagne-tower/solution/](https://leetcode.com/problems/champagne-tower/solution/)

[https://leetcode.com/problems/champagne-tower/discuss/118692/9ms-5-Lines-Code-C++Java](https://leetcode.com/problems/champagne-tower/discuss/118692/9ms-5-Lines-Code-C++Java)

[https://leetcode.com/problems/champagne-tower/discuss/118660/20ms-C++-Easy-understand-solution](https://leetcode.com/problems/champagne-tower/discuss/118660/20ms-C++-Easy-understand-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












