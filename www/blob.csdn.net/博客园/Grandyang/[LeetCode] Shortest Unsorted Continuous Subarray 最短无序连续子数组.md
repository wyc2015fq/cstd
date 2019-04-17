# [LeetCode] Shortest Unsorted Continuous Subarray 最短无序连续子数组 - Grandyang - 博客园







# [[LeetCode] Shortest Unsorted Continuous Subarray 最短无序连续子数组](https://www.cnblogs.com/grandyang/p/6876457.html)







Given an integer array, you need to find one continuous subarray that if you only sort this subarray in ascending order, then the whole array will be sorted in ascending order, too. 

You need to find the shortest such subarray and output its length.

Example 1:
Input: [2, 6, 4, 8, 10, 9, 15]
Output: 5
Explanation: You need to sort [6, 4, 8, 10, 9] in ascending order to make the whole array sorted in ascending order.



Note:
- Then length of the input array is in range [1, 10,000].
- The input array may contain duplicates, so ascending order here means <=.



这道题给了我们一个数组，让我们求最短的无序连续子数组，根据题目中的例子也不难分析出来是让我们找出数组中的无序的部分。那么我最开始的想法就是要确定无序子数组的起始和结束位置，这样就能知道子数组的长度了。所以我们用一个变量start来记录起始位置，然后我们开始遍历数组，当我们发现某个数字比其前面的数字要小的时候，说明此时数组不再有序，所以我们要将此数字向前移动，移到其应该在的地方，我们用另一个变量j来记录移动到的位置，然后我们考虑要不要用这个位置来更新start的值，当start还是初始值-1时，肯定要更新，因为这是出现的第一个无序的地方，还有就是如果当前位置小于start也要更新，这说明此时的无序数组比之前的更长了。我们举个例子来说明，比如数组[1,3,5,4,2]，第一个无序的地方是数字4，它移动到的正确位置是坐标2，此时start更新为2，然后下一个无序的地方是数字2，它的正确位置是坐标1，所以此时start应更新为1，这样每次用i - start + 1来更新结果res时才能得到正确的结果，参见代码如下：



解法一：

```
class Solution {
public:
    int findUnsortedSubarray(vector<int>& nums) {
        int res = 0, start = -1, n = nums.size();
        for (int i = 1; i < n; ++i) {
            if (nums[i] < nums[i - 1]) {
                int j = i;
                while (j > 0 && nums[j] < nums[j - 1]) {
                    swap(nums[j], nums[j - 1]);
                    --j;
                }
                if (start == -1 || start > j) start = j;
                res = i - start + 1;
            }
        }
        return res;
    }
};
```



下面这种方法是用了一个辅助数组，我们新一个跟原数组一摸一样的数组，然后排序。从数组起始位置开始，两个数组相互比较，当对应位置数字不同的时候停止，同理再从末尾开始，对应位置上比较，也是遇到不同的数字时停止，这样中间一段就是最短无序连续子数组了，参见代码如下：



解法二：

```
class Solution {
public:
    int findUnsortedSubarray(vector<int>& nums) {
        int n = nums.size(), i = 0, j = n - 1;
        vector<int> t = nums;
        sort(t.begin(), t.end());
        while (i < n && nums[i] == t[i]) ++i;
        while (j > i && nums[j] == t[j]) --j;
        return j - i + 1;
    }
};
```



下面这种方法很叼啊，是O(n)的时间复杂度加上O(1)的空间复杂度，博主觉得这实际上是对上面的那种方法进行空间上的优化的结果，用两个变量mx和mn来代替上面的有序数组，我们仔细来分析发现，最小值mn初始化为数组的最后一个数字，最大值mx初始化为了第一个数字，然后我们从第二个数字开始遍历，mx和nums[i]之间取较大值赋值给mx，然后比较此时mx和nums[i]之间的大小关系，如果mx大于nums[i]，就把i赋值给end，那么我们想如果第一个数字小于第二个，mx就会赋值为第二个数字，这时候mx和nums[i]就相等了，不进行任何操作，这make sense，因为说明此时是有序的。mn和nums[n-1-i]之间取较小值赋给mn，然后比较此时mn和nums[n-1-i]之间的大小关系，如果mn小于nums[n-1-i]，就把n-1-i赋值给start，那么什么时候会进行赋值呢，是当倒数第二个数字大于最后一个数字，这样mn还是最后一个数字，而nums[n-1-i]就会大于mn，这样我们更新start。我们可以看出start是不断往前走的，end是不断往后走的，整个遍历完成后，start和end就分别指向了最短无序连续子数组的起始和结束位置，参见代码如下：



解法三：

```
class Solution {
public:
    int findUnsortedSubarray(vector<int>& nums) {
        int n = nums.size(), start = -1, end = -2;
        int mn = nums[n - 1], mx = nums[0];
        for (int i = 1; i < n; ++i) {
            mx = max(mx, nums[i]);
            mn = min(mn, nums[n - 1 - i]);
            if (mx > nums[i]) end = i;
            if (mn < nums[n - 1 - i]) start = n - 1 - i;
        }
        return end - start + 1;
    }
};
```



参考资料：

[https://leetcode.com/problems/shortest-unsorted-continuous-subarray/](https://leetcode.com/problems/shortest-unsorted-continuous-subarray/)

[https://leetcode.com/problems/shortest-unsorted-continuous-subarray/discuss/103057/java-on-time-o1-space](https://leetcode.com/problems/shortest-unsorted-continuous-subarray/discuss/103057/java-on-time-o1-space)

[https://leetcode.com/problems/shortest-unsorted-continuous-subarray/discuss/103062/c-clean-code-2-solution-sort-onlgn-max-min-vectors-on](https://leetcode.com/problems/shortest-unsorted-continuous-subarray/discuss/103062/c-clean-code-2-solution-sort-onlgn-max-min-vectors-on)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












