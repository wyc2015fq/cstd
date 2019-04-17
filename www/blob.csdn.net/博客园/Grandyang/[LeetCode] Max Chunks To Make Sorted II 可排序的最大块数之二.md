# [LeetCode] Max Chunks To Make Sorted II 可排序的最大块数之二 - Grandyang - 博客园







# [[LeetCode] Max Chunks To Make Sorted II 可排序的最大块数之二](https://www.cnblogs.com/grandyang/p/8850299.html)







*This question is the same as "Max Chunks to Make Sorted" except the integers of the given array are not necessarily distinct, the input array could be up to length `2000`, and the elements could be up to `10**8`.*

Given an array `arr` of integers (not necessarily distinct), we split the array into some number of "chunks" (partitions), and individually sort each chunk.  After concatenating them, the result equals the sorted array.

What is the most number of chunks we could have made?

Example 1:
Input: arr = [5,4,3,2,1]
Output: 1
Explanation:
Splitting into two or more chunks will not return the required result.
For example, splitting into [5, 4], [3, 2, 1] will result in [4, 5, 1, 2, 3], which isn't sorted.

Example 2:
Input: arr = [2,1,3,4,4]
Output: 4
Explanation:
We can split into two chunks, such as [2, 1], [3, 4, 4].
However, splitting into [2, 1], [3], [4], [4] is the highest number of chunks possible.

Note:
- `arr` will have length in range `[1, 2000]`.
- `arr[i]` will be an integer in range `[0, 10**8]`.



这道题是之前那道 [Max Chunks To Make Sorted](http://www.cnblogs.com/grandyang/p/8823944.html) 的拓展，那道题说了数组是[0, n-1]中所有数字的一种全排列，n为数组的长度。而这道题的数字就没有这种限制，可以是大于n的数字，也可以有重复的数字。由于数字和坐标不再有太多的关联，所以之前那题中比较数字和坐标的大小的解法肯定行不通了。我们来看一种十分巧妙的解法，首先我们需要明确的一点是，拆分后的块儿排序后拼在一起会跟原数组相同，我们用一个例子来说明：

2  1  4  3  4

1  2344

1  2  3  4  4

我们看到第一行是原数组，第二行是排序后并拼接在了一起的块儿，不同的颜色代表不同的块儿，而第三行是直接对原数组排序后的结果。仔细观察可以发现，能形成块儿的数字之和跟排序后的数组的相同长度的子数组的数字之和是相同的。比如第一个块儿是数字2和1，和为3，而排序后的前两个数字为1和2，和也是3，那么我们就知道原数组的前两个数字可以拆成一个块儿。同理，原数组中的第三个和第四个数字分别为4和3，和为7，而排序后的数组对应位置的数字之和也是7，说明可以拆分出块儿。需要注意的是，在累加数组和的时候有可能会超过整型最大值，所以我们用长整型来保存就可以了。就是这么简单而暴力的思路，时间复杂度为O(nlgn)，主要花在给数组排序上了。由于本题是 [Max Chunks To Make Sorted](http://www.cnblogs.com/grandyang/p/8823944.html) 的generalized的情况，所以这种解法自然也可以解决之前那道题了，不过就是时间复杂度稍高了一些，参见代码如下：



解法一：

```
class Solution {
public:
    int maxChunksToSorted(vector<int>& arr) {
        long res = 0, sum1 = 0, sum2 = 0;
        vector<int> expect = arr;
        sort(expect.begin(), expect.end());
        for (int i = 0; i < arr.size(); ++i) {
            sum1 += arr[i];
            sum2 += expect[i];
            if (sum1 == sum2) ++res;
        }
        return res;
    }
};
```



这道题的时间复杂度可以优化到线性，不过就是需要花点空间。下面这种解法也相当的巧妙，我们需要两个数组forward和backward，其中 foward[i] 表示 [0, i] 范围内最大的数字，而 backward[i] 表示 [i, n-1] 范围内最小的数字，实际上就是要知道已经遍历过的最大值，和还未遍历的到的最小值。为啥我们对这两个值感兴趣呢？这是本解法的精髓所在，我们知道可以拆分为块儿的前提是之后的数字不能比当前块儿中的任何数字小，不然那个较小的数字就无法排到前面。就像例子1，为啥不能拆出新块儿，就因为最小的数字在末尾。那么这里我们能拆出新块儿的条件就是，当前位置出现过的最大值小于等于之后还未遍历到的最小值时，就能拆出新块儿。比如例子2中，当 i=1 时，此时出现过的最大数字为2，未遍历到的数字中最小值为3，所以可以拆出新块儿，参见代码如下：



解法二：

```
class Solution {
public:
    int maxChunksToSorted(vector<int>& arr) {
        int res = 1, n = arr.size();
        vector<int> f = arr, b = arr;   
        for (int i = 1; i < n; ++i) f[i] = max(arr[i], f[i - 1]);   
        for (int i = n - 2; i >= 0; --i) b[i] = min(arr[i], b[i + 1]);
        for (int i = 0; i < n - 1; ++i) {
            if (f[i] <= b[i + 1]) ++res;
        }
        return res;
    }
};
```



我们可以优化一下空间复杂度，因为我们可以在遍历的过程中维护一个当前最大值curMax，所以就不用一个专门的forward数组了，但是backward数组还是要的，参见代码如下： 



解法三：

```
class Solution {
public:
    int maxChunksToSorted(vector<int>& arr) {
        int res = 1, n = arr.size(), curMax = INT_MIN;
        vector<int> b = arr;    
        for (int i = n - 2; i >= 0; --i) b[i] = min(arr[i], b[i + 1]);
        for (int i = 0; i < n - 1; ++i) {
            curMax = max(curMax, arr[i]);
            if (curMax <= b[i + 1]) ++res;
        }
        return res;
    }
};
```



下面这种使用单调栈Monotonous Stack的解法的题也十分的巧妙，我们维护一个单调递增的栈，遇到大于等于栈顶元素的数字就压入栈，当遇到小于栈顶元素的数字后，处理的方法很是巧妙啊：首先取出栈顶元素，这个是当前最大值，因为我们维护的就是单调递增栈啊，然后我们再进行循环，如果栈不为空，且新的栈顶元素大于当前数字，则移除栈顶元素。这步简直绝了，这里我们单调栈的元素个数实际上是遍历到当前数字之前可以拆分成的块儿的个数，我们遇到一个大于栈顶的元素，就将其压入栈，suppose其是一个新块儿的开头，但是一旦后面遇到小的数字了，我们就要反过来检查前面的数字，有可能我们之前认为的可以拆分成块儿的地方，现在就不能拆了，举个栗子来说吧：

比如数组为 [1 0 3 3 2]，我们先把第一个数字1压入栈，此时栈为：

stack：1

然后遍历到第二个数字0，发现小于栈顶元素，将栈顶元素1取出存入curMax，此时栈为空了，不做任何操作，将curMax压回栈，此时栈为：

stack：1

然后遍历到第三个数字3，大于栈顶元素，压入栈，此时栈为：

stack：1，3

然后遍历到第四个数字3，等于栈顶元素，压入栈，此时栈为：

stack：1，3，3

然后遍历到第五个数字2，小于栈顶元素，将栈顶元素3取出存入curMax，此时新的栈顶元素3，大于当前数字2，移除此栈顶元素3，然后新的栈顶元素1，小于当前数字2，循环结束，将curMax压回栈，此时栈为：

stack：1，3

所以最终能拆为两个块儿，即stack中数字的个数，参见代码如下：



解法四：

```
class Solution {
public:
    int maxChunksToSorted(vector<int>& arr) {
        stack<int> st;
        for (int i = 0; i < arr.size(); ++i) {
            if (st.empty() || st.top() <= arr[i]) {
                st.push(arr[i]);
            } else {
                int curMax = st.top(); st.pop();
                while (!st.empty() && st.top() > arr[i]) st.pop();
                st.push(curMax);
            }
        }
        return st.size();
    }
};
```



类似题目：

[Max Chunks To Make Sorted](http://www.cnblogs.com/grandyang/p/8823944.html)



参考资料：

[https://leetcode.com/problems/max-chunks-to-make-sorted-ii/](https://leetcode.com/problems/max-chunks-to-make-sorted-ii/)

[https://leetcode.com/problems/max-chunks-to-make-sorted-ii/discuss/113464/C++-9-lines-15ms](https://leetcode.com/problems/max-chunks-to-make-sorted-ii/discuss/113464/C++-9-lines-15ms)

[https://leetcode.com/problems/max-chunks-to-make-sorted-ii/discuss/120336/Stack-Solution-in-Java](https://leetcode.com/problems/max-chunks-to-make-sorted-ii/discuss/120336/Stack-Solution-in-Java)

[https://leetcode.com/problems/max-chunks-to-make-sorted-ii/discuss/113462/Java-solution-left-max-and-right-min.](https://leetcode.com/problems/max-chunks-to-make-sorted-ii/discuss/113462/Java-solution-left-max-and-right-min.)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












