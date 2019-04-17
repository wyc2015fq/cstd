# [LeetCode] Smallest Rotation with Highest Score 得到最高分的最小旋转 - Grandyang - 博客园







# [[LeetCode] Smallest Rotation with Highest Score 得到最高分的最小旋转](https://www.cnblogs.com/grandyang/p/9272921.html)







Given an array `A`, we may rotate it by a non-negative integer `K` so that the array becomes `A[K], A[K+1], A{K+2], ... A[A.length - 1], A[0], A[1], ..., A[K-1]`.  Afterward, any entries that are less than or equal to their index are worth 1 point. 

For example, if we have `[2, 4, 1, 3, 0]`, and we rotate by `K = 2`, it becomes `[1, 3, 0, 2, 4]`.  This is worth 3 points because 1 > 0 [no points], 3 > 1 [no points], 0 <= 2 [one point], 2 <= 3 [one point], 4 <= 4 [one point].

Over all possible rotations, return the rotation index K that corresponds to the highest score we could receive.  If there are multiple answers, return the smallest such index K.
Example 1:
Input: [2, 3, 1, 4, 0]
Output: 3
Explanation:  
Scores for each K are listed below: 
K = 0,  A = [2,3,1,4,0],    score 2
K = 1,  A = [3,1,4,0,2],    score 3
K = 2,  A = [1,4,0,2,3],    score 3
K = 3,  A = [4,0,2,3,1],    score 4
K = 4,  A = [0,2,3,1,4],    score 3

So we should choose K = 3, which has the highest score.


Example 2:
Input: [1, 3, 0, 2, 4]
Output: 0
Explanation:  A will always have 3 points no matter how it shifts.
So we will choose the smallest K, which is 0.

Note:
- `A` will have length at most `20000`.
- `A[i]` will be in the range `[0, A.length]`.



这道题给了我们一个长度为N的数组，说是数组中的数字的范围都在[0, N]之间，然后定义了一个旋转操作，比如在位置K进行旋转，数组在K位置断开，新数组以A[k]为开头数字，断开的前半段数组直接拼到末尾即可。然后又定义了一种积分规则，说是如果某个坐标位置大于等于其数字的话，得1分，让我们求出某个旋转位置K，使得其积分最大，如果积分相同的话，取位置小的K。通过分析题目中的例子，发现题目并不难理解。博主首先尝试了暴力搜索的方法，就是遍历每个K值，生成旋转后的数组，然后再统计得分，不幸挂掉了。那么我们必须要想出更好的解法才行。首先我们想，如果数组中的每个数字都跟其坐标值相同的话，比如[0, 1, 2, 3, 4]，那么肯定得分最高，即K=0。但实际上并不会是有序的，而且并不是每个数字都会出现，题目中只给了数字的范围，有可能会有重复数字哦。说实话这道题博主研究[大神lee215的帖子](https://leetcode.com/problems/smallest-rotation-with-highest-score/discuss/118725/Easy-and-Concise-5-lines-Solution-C++JavaPython?page=2)，都研究了好久，最后终于有些明白了，就大概讲讲吧，如果有不对的地方欢迎大家指正。

这道题博主感觉还是很有难度的，而且答案的思路也十分巧妙，并没有采用brute force那种直接求每一个K值的得分，而是反其道而行之，对于每个数字，探究其跟K值之间的联系。首先我们要讨论一下边界情况，那么就是当A[i] = 0 或 N 的情况，首先如果A[i] = 0的话，那么0这个数字在任何位置都会小于等于坐标值，所以在任何位置都会得分的，那么其实可以忽略之，因为其不会对最大值产生任何影响，同理，如果A[i] = N的时候，由于长度为N的数组的坐标值范围是[0, N-1]，所以数字N在任何位置都不得分，同样也不会对最大值产生任何影响，可以忽略之。那么我们关心的数字的范围其实是[1, N-1]。在这个范围内的数字在旋转数组的过程中，从位置0变到N-1位置的时候，一定会得分，因为此范围的数字最大就是N-1。这个一定得的分我们在最后统一加上，基于上面的发现，我们再来分析下题目中的例子 [2, 3, 1, 4, 0]，其中红色数字表示不得分的位置：

A:    23   1   4   0   (K = 0)

A:    3   1   4   0   2   (K = 1)

A:    14   0   2   3   (K = 2)

A:    4   0   2   3   1   (K = 3)

A:    0   23   1   4   (K = 4)

idx:  0   1   2   3   4

对于某个数字A[i]，我们想知道其什么时候能旋转到坐标位置为A[i]的地方，这样就可以得分了。比如上面博主标记了红色的数字3，最开始时的位置为1，此时是不得分的，我们想知道其什么时候能到位置3，答案是当K=3的时候，其刚好旋转到位置3，K再增加的时候，其又开始不得分了。所以这个最后能得分的临界位置是通过 (i - A[i] + N) % N 得到，那么此时如果K再增加1的话，A[i]就开始不得分了（如果我们suppose每个位置都可以得分，那么不得分的地方就可以当作是失分了），所以我们可以在这个刚好开始不得分的地方标记一下，通过-1进行标记，这个位置就是 (i - A[i] + 1 + N) % N。我们用一个长度为N的change数组，对于每个数字，我们都找到其刚好不得分的地方，进行-1操作，那么此时change[i]就表示数组中的数字在i位置会不得分的个数，如果我们仔细观察上面红色的数字，可以发现，由于是左移，坐标在不断减小，所以原先失分的地方，在K+1的时候还是失分，除非你从开头位置跑到末尾去了，那会得分，所以我们要累加change数组，并且K每增加1的时候，要加上额外的1，最后change数组中最大数字的位置就是要求的K值了，参见代码如下：



解法一：

```
class Solution {
public:
    int bestRotation(vector<int>& A) {
        int n = A.size();
        vector<int> change(n, 0);
        for (int i = 0; i < n; ++i) change[(i - A[i] + 1 + n) % n] -= 1;
        for (int i = 1; i < n; ++i) change[i] += change[i - 1] + 1;
        return max_element(change.begin(), change.end()) - change.begin();
    }
};
```



我们也可以不用STL自带的max_element函数，而是在遍历的过程中同时找最大值即可，参见代码如下： 



解法二：

```
class Solution {
public:
    int bestRotation(vector<int>& A) {
        int n = A.size(), res = 0;
        vector<int> change(n, 0);
        for (int i = 0; i < n; ++i) change[(i - A[i] + 1 + n) % n] -= 1;
        for (int i = 1; i < n; ++i) {
            change[i] += change[i - 1] + 1;
            res = (change[i] > change[res]) ? i : res;
        }
        return res;
    }
};
```



参考资料：

[https://leetcode.com/problems/smallest-rotation-with-highest-score/discuss/121296/7-lines-C++-solution](https://leetcode.com/problems/smallest-rotation-with-highest-score/discuss/121296/7-lines-C++-solution)

[https://leetcode.com/problems/smallest-rotation-with-highest-score/discuss/118725/Easy-and-Concise-5-lines-Solution-C++JavaPython?page=2](https://leetcode.com/problems/smallest-rotation-with-highest-score/discuss/118725/Easy-and-Concise-5-lines-Solution-C++JavaPython?page=2)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












