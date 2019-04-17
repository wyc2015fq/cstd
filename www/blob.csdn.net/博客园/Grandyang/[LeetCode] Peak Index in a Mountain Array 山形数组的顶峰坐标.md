# [LeetCode] Peak Index in a Mountain Array 山形数组的顶峰坐标 - Grandyang - 博客园







# [[LeetCode] Peak Index in a Mountain Array 山形数组的顶峰坐标](https://www.cnblogs.com/grandyang/p/10527596.html)








Let's call an array `A` a *mountain* if the following properties hold:
- `A.length >= 3`
- There exists some `0 < i < A.length - 1` such that `A[0] < A[1] < ... A[i-1] < A[i] > A[i+1] > ... > A[A.length - 1]`

Given an array that is definitely a mountain, return any `i` such that `A[0] < A[1] < ... A[i-1] < A[i] > A[i+1] > ... > A[A.length - 1]`.

Example 1:

```
Input: [0,1,0]
Output: 1
```

Example 2:

```
Input: [0,2,1,0]
Output: 1
```

Note:
- `3 <= A.length <= 10000`
- `0 <= A[i] <= 10^6`
- A is a mountain, as defined above.




这道题定义了一种山形的数组，说是有一个最高点，然后向两边各自降低，让我们找出山峰的位置。其实这道题跟之前那道 [Find Peak Element](http://www.cnblogs.com/grandyang/p/4217175.html) 非常的类似，只不过那道题有很多局部峰值，而这里道题只有一个全局峰值。题目中限定了山峰一定存在，即一定有一个最高点，反应在数组中就是最大值，那么问题就转换为了求数组中最大值的位置，最简单直接的方法就是遍历数组找出最大值的位置即可，这里使用了STL的内置函数 max_element() 来一行解题，参见代码如下：




解法一：

```
class Solution {
public:
    int peakIndexInMountainArray(vector<int>& A) {
        return max_element(A.begin(), A.end()) - A.begin();
    }
};
```




由于题目中限定了山峰一定存在，所以我们也可以直接直接来找第一个下降的位置，即 A[i] > A[i+1] 的地方，那么i位置一定就是山峰了，注意i的遍历范围要去掉首尾两个数字，参见代码如下：




解法二：

```
class Solution {
public:
    int peakIndexInMountainArray(vector<int>& A) {
        for (int i = 1; i < (int)A.size() - 1; ++i) {
            if (A[i] > A[i + 1]) return i;
        }
        return 0;
    }
};
```




上面两种解法都是线性的时间复杂度，能不能更快一点呢？那么就只有使用二分搜索法来加快搜索速度了，其实这是博主的总结帖 [LeetCode Binary Search Summary 二分搜索法小结](http://www.cnblogs.com/grandyang/p/6854825.html) 中的第五类情况，跟前的稍有不同的是 right 的初始化，之前的情况博主基本上都是用数组长度初始化 right 的，但是这里要用数组长度减1来初始化 right，因为要跟紧邻的右边的数字比较，这样初始化 right 的意义在于 mid+1 就不会越界了，参见代码如下：




解法三：

```
class Solution {
public:
    int peakIndexInMountainArray(vector<int>& A) {
        int n = A.size(), left = 0, right = n - 1;
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (A[mid] < A[mid + 1]) left = mid + 1;
            else right = mid;
        }
        return right;
    }
};
```




类似题目：

[Find Peak Element](http://www.cnblogs.com/grandyang/p/4217175.html)




参考资料：

[https://leetcode.com/problems/peak-index-in-a-mountain-array/](https://leetcode.com/problems/peak-index-in-a-mountain-array/)

[https://leetcode.com/problems/peak-index-in-a-mountain-array/discuss/139848/C%2B%2BJavaPython-Better-than-Binary-Search](https://leetcode.com/problems/peak-index-in-a-mountain-array/discuss/139848/C%2B%2BJavaPython-Better-than-Binary-Search)




[LeetCode All in One 题目讲解汇总(持续更新中...)](https://www.cnblogs.com/grandyang/p/4606334.html)












