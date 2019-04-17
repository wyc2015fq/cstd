# [LeetCode] Maximum Vacation Days 最大化休假日 - Grandyang - 博客园







# [[LeetCode] Maximum Vacation Days 最大化休假日](https://www.cnblogs.com/grandyang/p/6919389.html)







LeetCode wants to give one of its best employees the option to travel among N cities to collect algorithm problems. But all work and no play makes Jack a dull boy, you could take vacations in some particular cities and weeks. Your job is to schedule the traveling to maximize the number of vacation days you could take, but there are certain rules and restrictions you need to follow.

Rules and restrictions:
- You can only travel among N cities, represented by indexes from 0 to N-1. Initially, you are in the city indexed 0 on Monday.
- The cities are connected by flights. The flights are represented as a N*N matrix (not necessary symmetrical), called flights representing the airline status from the city i to the city j. If there is no flight from the city i to the city j, flights[i][j] = 0; Otherwise, flights[i][j] = 1. Also, flights[i][i] = 0 for all i.
- You totally have K weeks (each week has 7 days) to travel. You can only take flights at most once per day and can only take flights on each week's Monday morning. Since flight time is so short, we don't consider the impact of flight time.
- For each city, you can only have restricted vacation days in different weeks, given an N*K matrix called days representing this relationship. For the value of days[i][j], it represents the maximum days you could take vacation in the city i in the week j.



You're given the flights matrix and days matrix, and you need to output the maximum vacation days you could take during K weeks.

Example 1:
Input:flights = [[0,1,1],[1,0,1],[1,1,0]], days = [[1,3,1],[6,0,3],[3,3,3]]
Output: 12
Explanation: 
Ans = 6 + 3 + 3 = 12. 

One of the best strategies is:
1st week : fly from city 0 to city 1 on Monday, and play 6 days and work 1 day. 
(Although you start at city 0, we could also fly to and start at other cities since it is Monday.) 
2nd week : fly from city 1 to city 2 on Monday, and play 3 days and work 4 days.
3rd week : stay at city 2, and play 3 days and work 4 days.



Example 2:
Input:flights = [[0,0,0],[0,0,0],[0,0,0]], days = [[1,1,1],[7,7,7],[7,7,7]]
Output: 3
Explanation: 
Ans = 1 + 1 + 1 = 3. 

Since there is no flights enable you to move to another city, you have to stay at city 0 for the whole 3 weeks. 
For each week, you only have one day to play and six days to work. 
So the maximum number of vacation days is 3.



Example 3:
Input:flights = [[0,1,1],[1,0,1],[1,1,0]], days = [[7,0,0],[0,7,0],[0,0,7]]
Output: 21
Explanation:
Ans = 7 + 7 + 7 = 21

One of the best strategies is:
1st week : stay at city 0, and play 7 days. 
2nd week : fly from city 0 to city 1 on Monday, and play 7 days.
3rd week : fly from city 1 to city 2 on Monday, and play 7 days.



Note:
- N and K are positive integers, which are in the range of [1, 100].
- In the matrix flights, all the values are integers in the range of [0, 1].
- In the matrix days, all the values are integers in the range [0, 7].
- You could stay at a city beyond the number of vacation days, but you should work on the extra days, which won't be counted as vacation days.
- If you fly from the city A to the city B and take the vacation on that day, the deduction towards vacation days will count towards the vacation days of city B in that week.
- We don't consider the impact of flight hours towards the calculation of vacation days.



这道题给了我们一个NxN的数组，表示城市i是否有飞机直达城市j，又给了我们一个NxK的数组days，表示在第j周能在城市i休假的天数，让我们找出一个行程能使我们休假的天数最大化。开始尝试写了个递归的暴力破解法，结果TLE了。其实这道题比较适合用DP来解，我们建立一个二维DP数组，其中dp[i][j]表示目前是第j周，并且在此时在城市i，总共已经获得休假的总日子数。我们采取从后往前更新的方式(不要问我为什么，因为从前往后更新的写法要复杂一些)，我们从第k周开始往第一周遍历，那么最后结果都累加在了dp[i][0]中，i的范围是[0, n-1]，找出其中的最大值就是我们能休息的最大假期数了。难点就在于找递推式了，我们想dp[i][j]表示的是当前是第j周并在城市i已经获得的休假总日子数，那么上一个状态，也就是j+1周(因为我们是从后往前更新)，跟当前状态有何联系，上一周我们可能还在城市i，也可能在其他城市p，那么在其他城市p的条件是，城市p有直飞城市i的飞机，那么我们可以用上一个状态的值dp[p][j+1]来更新当前值dp[i][j]，还要注意的是我们要从倒数第二周开始更新，因为倒数第一周没有上一个状态，还有就是每个状态dp[i][j]都初始化赋为days[i][j]来更新，这样一旦没有任何城市可以直飞当前城市，起码我们还可以享受当前城市的假期，最后要做的就是想上面所说在dp[i][0]中找最大值，下面的代码是把这一步融合到了for循环中，所以加上了一堆判断条件，我们也可以在dp数组整个更新结束之后再来找最大值，参见代码如下：



解法一：

```
class Solution {
public:
    int maxVacationDays(vector<vector<int>>& flights, vector<vector<int>>& days) {
        int n = flights.size(), k = days[0].size(), res = 0;
        vector<vector<int>> dp(n, vector<int>(k, 0));
        for (int j = k - 1; j >= 0; --j) {
            for (int i = 0; i < n; ++i) {
                dp[i][j] = days[i][j];
                for (int p = 0; p < n; ++p) {
                    if ((i == p || flights[i][p]) && j < k - 1) {
                        dp[i][j] = max(dp[i][j], dp[p][j + 1] + days[i][j]);
                    }
                    if (j == 0 && (i == 0 || flights[0][i])) res = max(res, dp[i][0]);
                }
            }
        }
        return res;
    }
};
```



下面这种方法优化了空间复杂度，只用了一个一维的DP数组，其中dp[i]表示在当前周，在城市i时已经获得的最大假期数，并且除了第一个数初始化为0，其余均初始化为整型最小值，然后我们从第一周往后遍历，我们新建一个临时数组t，初始化为整型最小值，然后遍历每一个城市，对于每一个城市，我们遍历其他所有城市，看是否有飞机能直达当前城市，或者就是当前的城市，我们用dp[p] + days[i][j]来更更新dp[i]，当每个城市都遍历完了之后，我们将t整个赋值给dp，然后进行下一周的更新，最后只要在dp数组中找出最大值返回即可，这种写法不但省空间，而且也相对简洁一些，很赞啊～



解法二：

```
class Solution {
public:
    int maxVacationDays(vector<vector<int>>& flights, vector<vector<int>>& days) {
        int n = flights.size(), k = days[0].size();
        vector<int> dp(n, INT_MIN);
        dp[0] = 0;
        for (int j = 0; j < k; ++j) {
            vector<int> t(n, INT_MIN);
            for (int i = 0; i < n; ++i) {
                for (int p = 0; p < n; ++p) {
                    if (i == p || flights[p][i]) {
                        t[i] = max(t[i], dp[p] + days[i][j]);
                    }
                }
            }
            dp = t;
        }
        return *max_element(dp.begin(), dp.end());
    }
};
```



之前提到了递归的DFS会TLE，但是如果我们使用一个memo数组来保存中间计算结果，就能省去大量的重复计算，并且能够通过OJ，解题思想跟解法一非常的类似，参见代码如下：



解法三：

```
class Solution {
public:
    int maxVacationDays(vector<vector<int>>& flights, vector<vector<int>>& days) {
        int n = flights.size(), k = days[0].size();
        vector<vector<int>> memo(n, vector<int>(k, 0));
        return helper(flights, days, 0, 0, memo);
    }
    int helper(vector<vector<int>>& flights, vector<vector<int>>& days, int city, int day, vector<vector<int>>& memo) {
        int n = flights.size(), k = days[0].size(), res = 0;
        if (day == k) return 0;
        if (memo[city][day] > 0) return memo[city][day];
        for (int i = 0; i < n; ++i) {
            if (i == city || flights[city][i] == 1) {
                res = max(res, days[i][day] + helper(flights, days, i, day + 1, memo));
            }
        }
        return memo[city][day] = res;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/87865/java-dfs-tle-and-dp-solutions/2](https://discuss.leetcode.com/topic/87865/java-dfs-tle-and-dp-solutions/2)

[https://discuss.leetcode.com/topic/87869/c-clean-code-graphic-explanation](https://discuss.leetcode.com/topic/87869/c-clean-code-graphic-explanation)

[https://discuss.leetcode.com/topic/89353/short-java-recursion-dfs-memoization](https://discuss.leetcode.com/topic/89353/short-java-recursion-dfs-memoization)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












