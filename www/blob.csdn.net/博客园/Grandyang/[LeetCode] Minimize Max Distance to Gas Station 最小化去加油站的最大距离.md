# [LeetCode] Minimize Max Distance to Gas Station 最小化去加油站的最大距离 - Grandyang - 博客园







# [[LeetCode] Minimize Max Distance to Gas Station 最小化去加油站的最大距离](https://www.cnblogs.com/grandyang/p/8970057.html)









On a horizontal number line, we have gas stations at positions `stations[0], stations[1], ..., stations[N-1]`, where `N = stations.length`.

Now, we add `K` more gas stations so that D, the maximum distance between adjacent gas stations, is minimized.

Return the smallest possible value of D.

Example:
Input: stations = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10], K = 9
Output: 0.500000

Note:
- `stations.length` will be an integer in range `[10, 2000]`.
- `stations[i]` will be an integer in range `[0, 10^8]`.
- `K` will be an integer in range `[1, 10^6]`.
- Answers within `10^-6` of the true value will be accepted as correct.





这道题说给了我们n个加油站，两两之间相距不同的距离，然后我们可以在任意地方新加K个加油站，问能使得任意两个加油站之间的最大距离的最小值是多少。乍眼一看，感觉很绕，一会儿最大，一会儿最小的。其实我们可以换个场景，比如n个人站一队，每两个人之间距离不同，有的人之间距离可能很大，有的人可能挨得很近。我们现在需要再加入K个人到队列中，我们希望人与人之间的距离尽可能小，所以新人就应该加入到距离大的地方，然后问我们加入K个人后，求人与人之间的最大距离。这么一说，是不是清晰一点了呢。博主最开始看到这个加油站的题，以为跟之前那道[Gas Station](http://www.cnblogs.com/grandyang/p/4266812.html)有关联，结果发现二者并没有什么关系，只不过公用了加油站这个场景而已。对于这道题，我们还是抽离出本质，就是数组插数问题。博主最先考虑的是用贪婪算法，就是先算出每两个数字之间的距离，然后我们每次往距离最大的那两个数字之间插入一个数字，这种想法看似正确，但是会跪在这样一个test case：

[10, 19, 25, 27, 56, 63, 70, 87, 96, 97]，K = 3

其两两之间的距离为：

9，6，2，29，7，7，17，9，1

如果按照博主前面所说的方法，会先将29分开，变成两个14.5，然后会将17分开，变成两个8.5，还剩一个加油站，会将其中一个14.5分开，变成两个7.25。但是这样弄下来，最大的距离还是14.5，而实际上我们有更好的办法，我们用两个加油站将29三等分，会变成三个9.67，然后用剩下的一个去分17，得到两个8.5，此时最大距离就变成了9.67，这才是最优的解法。这说明了博主那种图样图森破的贪婪算法并不work，缺乏对Hard题目的尊重。

后来看了[官方解答贴](https://leetcode.com/problems/minimize-max-distance-to-gas-station/solution/)中的解法，发现用DP会内存超标MLE，用堆会时间超标TLE。其实这道题的正确解法是用二分搜索法，博主第一反应是，还有这种操作！！？？就是有这种操作！！！这道题使用的二分搜索法是博主归纳总结帖[LeetCode Binary Search Summary 二分搜索法小结](http://www.cnblogs.com/grandyang/p/6854825.html)中的第四种，即二分法的判定条件不是简单的大小关系，而是可以抽离出子函数的情况，下面我们来看具体怎么弄。如果光说要用二分法来做，那么首先就要明确的是二分法用来查找什么，难道是用来查找要插入加油站的位置吗？很显然不是，其实是用来查找那个最小的任意两个加油站间的最大距离。这其实跟之前那道[Kth Smallest Element in a Sorted Matrix](http://www.cnblogs.com/grandyang/p/5727892.html)非常的类似，那道题的二分搜索也是直接去折半定位所求的数，然后再来验证其是否真的符合题意。这道题也是类似的思路，题目中给了数字的范围[0, 10^8]，那么二分查找的左右边界值就有了，又给了误差范围10^-6，那么只要right和left差值大于这个阈值，就继续循环。我们折半计算出来的mid就是一个candidate，我们要去验证个candidate是否符合题意。验证的方法其实也不难，我们计算每两个加油站之间的距离，如果此距离大于candidate，则计数器累加1，如果大于candidate距离的个数小于等于k，则说明我们的candidate偏大了，那么right赋值为mid；反之若大于candidate距离的个数大于k，则说明我们的candidate偏小了，那么left赋值为mid。最后left和right都会收敛为所要求的最小的任意两个加油站间的最大距离，是不是很神奇呀！！Amazing！！参见代码如下：



解法一：

```
class Solution {
public:
    double minmaxGasDist(vector<int>& stations, int K) {
        double left = 0, right = 1e8;
        while (right - left > 1e-6) {
            double mid = left + (right - left) / 2;
            if (helper(stations, K, mid)) right = mid;
            else left = mid;
        }
        return left;
    }
    bool helper(vector<int>& stations, int K, double mid) {
        int cnt = 0, n = stations.size();
        for (int i = 0; i < n - 1; ++i) {
            cnt += (stations[i + 1] - stations[i]) / mid;
        }
        return cnt <= K;
    }
};
```



我们也可以把上面解法中的子函数揉到主函数里面，这样可以是的代码更加的简洁，参见代码如下：



解法二：

```
class Solution {
public:
    double minmaxGasDist(vector<int>& stations, int K) {
        double left = 0, right = 1e8;
        while (right - left > 1e-6) {
            double mid = left + (right - left) / 2;
            int cnt = 0, n = stations.size();
            for (int i = 0; i < n - 1; ++i) {
                cnt += (stations[i + 1] - stations[i]) / mid;
            }
            if (cnt <= K) right = mid;
            else left = mid;
        }
        return left;
    }
};
```



类似题目：

[Find K-th Smallest Pair Distance](http://www.cnblogs.com/grandyang/p/8627783.html)

[Kth Smallest Number in Multiplication Table](http://www.cnblogs.com/grandyang/p/8367505.html)

[Maximum Average Subarray II](http://www.cnblogs.com/grandyang/p/8021421.html)

[Kth Smallest Element in a Sorted Matrix](http://www.cnblogs.com/grandyang/p/5727892.html)



参考资料：

[https://leetcode.com/problems/minimize-max-distance-to-gas-station/solution/](https://leetcode.com/problems/minimize-max-distance-to-gas-station/solution/)

[https://leetcode.com/problems/minimize-max-distance-to-gas-station/discuss/113629/Approach-the-problem-using-the-%22trial-and-error%22-algorithm](https://leetcode.com/problems/minimize-max-distance-to-gas-station/discuss/113629/Approach-the-problem-using-the-%22trial-and-error%22-algorithm)

[https://leetcode.com/problems/minimize-max-distance-to-gas-station/discuss/113633/Easy-and-Concise-Solution-using-Binary-Search-C++JavaPython](https://leetcode.com/problems/minimize-max-distance-to-gas-station/discuss/113633/Easy-and-Concise-Solution-using-Binary-Search-C++JavaPython)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












