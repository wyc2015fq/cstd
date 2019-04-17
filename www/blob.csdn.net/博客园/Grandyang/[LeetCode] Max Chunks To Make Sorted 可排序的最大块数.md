# [LeetCode] Max Chunks To Make Sorted 可排序的最大块数 - Grandyang - 博客园







# [[LeetCode] Max Chunks To Make Sorted 可排序的最大块数](https://www.cnblogs.com/grandyang/p/8823944.html)







Given an array `arr` that is a permutation of `[0, 1, ..., arr.length - 1]`, we split the array into some number of "chunks" (partitions), and individually sort each chunk.  After concatenating them, the result equals the sorted array.

What is the most number of chunks we could have made?

Example 1:
Input: arr = [4,3,2,1,0]
Output: 1
Explanation:
Splitting into two or more chunks will not return the required result.
For example, splitting into [4, 3], [2, 1, 0] will result in [3, 4, 0, 1, 2], which isn't sorted.

Example 2:
Input: arr = [1,0,2,3,4]
Output: 4
Explanation:
We can split into two chunks, such as [1, 0], [2, 3, 4].
However, splitting into [1, 0], [2], [3], [4] is the highest number of chunks possible.

Note:
- `arr` will have length in range `[1, 10]`.
- `arr[i]` will be a permutation of `[0, 1, ..., arr.length - 1]`.



这道题给了我们一个长度为n的数组，里面的数字是[0, n-1]范围内的所有数字，无序的。现在让我们分成若干块儿，然后给每一小块儿分别排序，再组合到一起，使原数组变得有序，问我们最多能分多少块，题目中的两个例子很好的解释了题意。我们首先来分析例子1，这是一个倒序的数组，第一个数字是最大的，为4，那么我们想，这个数字4原本是应该位于数组的最后一个位置，所以中间不可能断开成新的块了，要不然数字4就没法跑到末尾去了。分析到这里，我们应该隐约有点感觉了，当前数字所在的块至少要到达坐标为当前数字大小的地方，比如数字4所在的块至少要包括i=4的那个位置。那么带着这个发现，来分析例子2。第一个数字是1，那么当前数字1所在的块至少要到 i=1 的位置，然后我们去 i=1 的位置上看，发现是数字0，并没有超过 i=1 的范围，那么前两个数就可以断开成一个新的块儿。再往后看，i=2 的位置是2，可以单独断开，后面的3和4也可以分别断开。所以其实这道题跟[Jump Game II](http://www.cnblogs.com/grandyang/p/4373533.html)那题很像，我们需要维护一个最远能到达的位置，这里的每个数字相当于那道题中的跳力，只有当我们刚好到达最远点的时候，就可以把之前断成一个新的块儿了。

我们遍历原数组，用cur表示能到达的最远点，然后我们遍历当前位置到cur之间的所有点，遍历的同时如果遇到更大的数字就更新cur，当cur大于等于末尾数字的时候，此时不能再拆分新块儿了，返回结果res加1。否则的话说明到达了最远点，更新第一个for循环的变量i，并且结果res自增1。来看个例子:

[2 0 1 4 3]

当 i=0 时，cur=2，j=1，然后我们发现 j=1 和 j=2 的数字都不会更新cur，且cur也没有大于等于3，所以此时 j=3 的时候退出了内部的for循环，i赋值为2，结果res为1。然后此时 i=3，cur=4，4已经大于末尾的3了，直接返回res加1，即2，参见代码如下：



解法一：

```
class Solution {
public:
    int maxChunksToSorted(vector<int>& arr) {
        int res = 0, n = arr.size();
        for (int i = 0; i < n; ++i) {
            int cur = arr[i], j = i + 1;
            for (; j <= cur; ++j) {
                cur = max(cur, arr[j]);
                if (cur >= arr.back()) return res + 1;
            }
            i = j - 1;
            ++res;
        }
        return res;
    }
};
```



其实这道题有更霸道的解法，我们仔细观察一些例子，可以发现断开为新块儿的地方都是当之前出现的最大值正好和当前位置坐标相等的地方，比如例子2中，当 i=1 时，之前最大的数字是1，所以可以断开。而在例子1中，当 i=4 时，才和之前出现过的最大数字4相等，此时断开也没啥意义了，因为后面已经没有数字了，所以还只是一个块儿，参见代码如下： 



解法二：

```
class Solution {
public:
    int maxChunksToSorted(vector<int>& arr) {
        int res = 0, n = arr.size(), mx = 0;
        for (int i = 0; i < n; ++i) {
            mx = max(mx, arr[i]);
            if (mx == i) ++res;
        }
        return res;
    }
};
```



讨论：由于本题是其拓展题[Max Chunks To Make Sorted II](http://www.cnblogs.com/grandyang/p/8850299.html)的特殊情况，所以其拓展题的四种解法都可以用在本题，这里就不一一列举了，具体的代码和讲解可以参见这个帖子[Max Chunks To Make Sorted II](http://www.cnblogs.com/grandyang/p/8850299.html)。



类似题目：

[Max Chunks To Make Sorted II](http://www.cnblogs.com/grandyang/p/8850299.html)

[Jump Game II](http://www.cnblogs.com/grandyang/p/4373533.html)



参考资料：

[https://leetcode.com/problems/max-chunks-to-make-sorted/solution/](https://leetcode.com/problems/max-chunks-to-make-sorted/solution/)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












