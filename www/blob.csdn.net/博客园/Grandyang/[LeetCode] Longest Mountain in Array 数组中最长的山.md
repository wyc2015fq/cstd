# [LeetCode] Longest Mountain in Array 数组中最长的山 - Grandyang - 博客园







# [[LeetCode] Longest Mountain in Array 数组中最长的山](https://www.cnblogs.com/grandyang/p/10459400.html)







Let's call any (contiguous) subarray B (of A) a *mountain* if the following properties hold:
- `B.length >= 3`
- There exists some `0 < i < B.length - 1` such that `B[0] < B[1] < ... B[i-1] < B[i] > B[i+1] > ... > B[B.length - 1]`

(Note that B could be any subarray of A, including the entire array A.)

Given an array `A` of integers, return the length of the longest *mountain*. 

Return `0` if there is no mountain.

Example 1:
Input: [2,1,4,7,3,2,5]
Output: 5
Explanation: The largest mountain is [1,4,7,3,2] which has length 5.

Example 2:
Input: [2,2,2]
Output: 0
Explanation: There is no mountain.

Note:
- `0 <= A.length <= 10000`
- `0 <= A[i] <= 10000`

Follow up:
- Can you solve it using only one pass?
- Can you solve it in `O(1)` space?



这道题给了我们一个数组，然后定义了一种像山一样的子数组，就是先递增再递减的子数组，注意这里是强行递增或者递减的，并不存在相等的情况。那么实际上这道题就是让我们在数组中寻找一个位置，使得以此位置为终点的递增数组和以此位置为起点的递减数组的长度最大。而以某个位置为起点的递减数组，如果反个方向来看，其实就是就该位置为终点的递增数列，那么既然都是求最长的递增数列，我们可以分别用两个dp数组up和down，其中 up[i] 表示以 i 位置为终点的最长递增数列的个数，down[i] 表示以 i 位置为起点的最长递减数列的个数，这样我们正向更新up数组，反向更新down数组即可。先反向更新好了down之后，在正向更新up数组的同时，也可以更新结果res，当某个位置的 up[i] 和 down[i] 均大于0的时候，那么就可以用 up[i] + down[i] + 1 来更新结果res了，参见代码如下：



解法一：

```
class Solution {
public:
    int longestMountain(vector<int>& A) {
        int res = 0, n = A.size();
        vector<int> up(n), down(n);
        for (int i = n - 2; i >= 0; --i) {
            if (A[i] > A[i + 1]) down[i] = down[i + 1] + 1;
        }
        for (int i = 1; i < n; ++i) {
            if (A[i] > A[i - 1]) up[i] = up[i - 1] + 1;
            if (up[i] > 0 && down[i] > 0) res = max(res, up[i] + down[i] + 1);
        }
        return res;
    }
};
```



我们可以对空间进行优化，不必使用两个数组来记录所有位置的信息，而是只用两个变量up和down来分别记录以当前位置为终点的最长递增数列的长度，和以当前位置为终点的最长递减数列的长度。 我们从 i=1 的位置开始遍历，因为山必须要有上坡和下坡，所以 i=0 的位置永远不可能成为peak。此时我们再看，如果当前位置跟前面的位置相等了，那么当前位置的up和down都要重置为0，从当前位置开始找新的山，和之前的应该断开。或者是当down不为0，说明此时是在下坡，那么如果当前位置大于之前的了，突然变上坡了，那么之前的累计也需要重置为0。然后当前位置再进行判断，若大于前一个位置，则是上坡，up自增1，若小于前一个位置，是下坡，down自增1。当up和down同时为正数，则用 up+down+1 来更新结果res即可，参见代码如下：



解法二：

```
class Solution {
public:
    int longestMountain(vector<int>& A) {
        int res = 0, up = 0, down = 0, n = A.size();
        for (int i = 1; i < n; ++i) {
            if ((down && A[i - 1] < A[i]) || (A[i - 1] == A[i])) {
                up = down = 0;
            }
            if (A[i - 1] < A[i]) ++up;
            if (A[i - 1] > A[i]) ++down;
            if (up > 0 && down > 0) res = max(res, up + down + 1);
        }
        return res;
    }
};
```



我们可以换一种思路，还是一次遍历就行，进行while循环，条件是 i < n-1，然后判断，当前数字大于等于下一个数字则跳过，因为我们希望首先上坡，当找到递增的起点i后，则再开始循环，找山顶peak，找到了之后，再进行下坡，找到山脚j，这样如果i，peak，和j都不相同的话，说明找到了一个完整的山，用 j-i+1 来更新结果res即可，然后i从j开始继续遍历，参见代码如下：



解法三：

```
class Solution {
public:
    int longestMountain(vector<int>& A) {
        int res = 0, i = 0, n = A.size();
        while (i < n - 1) {
            while (i < n - 1 && A[i] >= A[i + 1]) ++i;
            int peak = i;
            while (peak < n - 1 && A[peak] < A[peak + 1]) ++peak;
            int j = peak;
            while (j < n - 1 && A[j] > A[j + 1]) ++j;
            if (i < peak && peak < j) res = max(res, j - i + 1);
            i = j;
        }
        return res;
    }
};
```



也可以再换种思路，首先来找山峰，peak的范围是 [1, n-1]，因为首尾两个数字都不能做山峰，能做山峰的位置上的数必须大于其左右两边的数字，然后分别向左右两个方向遍历，这样就可以找到完整的山，用 right-left+1 来更新结果res，参见代码如下：



解法四：

```
class Solution {
public:
    int longestMountain(vector<int>& A) {
        int res = 0, n = A.size();
        for (int i = 1; i < n - 1; ++i) {
            if (A[i - 1] < A[i] && A[i + 1] < A[i]) {
                int left = i - 1, right = i + 1;
                while (left > 0 && A[left - 1] < A[left]) --left;
                while (right < n - 1 && A[right] > A[right + 1]) ++right;
                res = max(res, right - left + 1);
            }
        }
        return res;
    }
};
```



参考资料：

[https://leetcode.com/problems/longest-mountain-in-array/](https://leetcode.com/problems/longest-mountain-in-array/)

[https://leetcode.com/problems/longest-mountain-in-array/discuss/176952/Java-1-pass-and-O(1)-space-beats-100](https://leetcode.com/problems/longest-mountain-in-array/discuss/176952/Java-1-pass-and-O(1)-space-beats-100)

[https://leetcode.com/problems/longest-mountain-in-array/discuss/135593/C%2B%2BJavaPython-1-pass-and-O(1)-space](https://leetcode.com/problems/longest-mountain-in-array/discuss/135593/C%2B%2BJavaPython-1-pass-and-O(1)-space)

[https://leetcode.com/problems/longest-mountain-in-array/discuss/150136/Simple-O(n)-one-pass-O(1)-space-Java-AC-solution-beats-99.05](https://leetcode.com/problems/longest-mountain-in-array/discuss/150136/Simple-O(n)-one-pass-O(1)-space-Java-AC-solution-beats-99.05)



[LeetCode All in One 题目讲解汇总(持续更新中...)](https://www.cnblogs.com/grandyang/p/4606334.html)












