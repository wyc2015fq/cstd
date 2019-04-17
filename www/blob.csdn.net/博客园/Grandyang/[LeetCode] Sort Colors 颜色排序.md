# [LeetCode] Sort Colors 颜色排序 - Grandyang - 博客园







# [[LeetCode] Sort Colors 颜色排序](https://www.cnblogs.com/grandyang/p/4341243.html)







Given an array with *n* objects colored red, white or blue, sort them [in-place](https://en.wikipedia.org/wiki/In-place_algorithm) so that objects of the same color are adjacent, with the colors in the order red, white and blue.

Here, we will use the integers 0, 1, and 2 to represent the color red, white, and blue respectively.

Note: You are not suppose to use the library's sort function for this problem.

Example:
Input: [2,0,2,1,1,0]
Output: [0,0,1,1,2,2]
Follow up:
- A rather straight forward solution is a two-pass algorithm using counting sort.
First, iterate the array counting number of 0's, 1's, and 2's, then overwrite array with total number of 0's, then 1's and followed by 2's.
- Could you come up with a one-pass algorithm using only constant space?





这道题的本质还是一道排序的题，题目中给出提示说可以用计数排序，需要遍历数组两遍，那么先来看这种方法，因为数组中只有三个不同的元素，所以实现起来很容易。

- 首先遍历一遍原数组，分别记录0,1,2的个数
- 然后更新原数组，按个数分别赋上0，1，2



解法一：

```
class Solution {
public:
    void sortColors(int A[], int n) {
        int count[3] = {0}, idx = 0;
        for (int i = 0; i < n; ++i) ++count[A[i]];
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < count[i]; ++j) {
                A[idx++] = i;
            }
        }
    }
};
```



题目中还要让只遍历一次数组来求解，那么我需要用双指针来做，分别从原数组的首尾往中心移动。

- 定义red指针指向开头位置，blue指针指向末尾位置

- 从头开始遍历原数组，如果遇到0，则交换该值和red指针指向的值，并将red指针后移一位。若遇到2，则交换该值和blue指针指向的值，并将blue指针前移一位。若遇到1，则继续遍历。



解法二：

```
class Solution {
public:
    void sortColors(int A[], int n) {int red = 0, blue = n - 1;
        for (int i = 0; i <= blue; ++i) {
            if (A[i] == 0) {
                swap(A[i], A[red++]);
            } else if (A[i] == 2) {
                swap(A[i--], A[blue--]);
            } 
        }
    }
};
```



类似题目：

[Sort List](http://www.cnblogs.com/grandyang/p/4249905.html)

[Wiggle Sort II](http://www.cnblogs.com/grandyang/p/5139057.html)

[Wiggle Sort](http://www.cnblogs.com/grandyang/p/5177285.html)



参考资料：

[https://leetcode.com/problems/sort-colors/](https://leetcode.com/problems/sort-colors/)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












