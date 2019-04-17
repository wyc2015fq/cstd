# [LeetCode] Remove Duplicates from Sorted Array II 有序数组中去除重复项之二 - Grandyang - 博客园







# [[LeetCode] Remove Duplicates from Sorted Array II 有序数组中去除重复项之二](https://www.cnblogs.com/grandyang/p/4329295.html)







Follow up for "Remove Duplicates":
What if duplicates are allowed at most *twice*?

For example,
Given sorted array A = `[1,1,1,2,2,3]`,

Your function should return length = `5`, and A is now `[1,1,2,2,3]`.



这道题是之前那道[Remove Duplicates from Sorted Array 有序数组中去除重复项](http://www.cnblogs.com/grandyang/p/4329128.html) 的延续，这里允许最多重复的次数是两次，那么我们就需要用一个变量count来记录还允许有几次重复，count初始化为1，如果出现过一次重复，则count递减1，那么下次再出现重复，快指针直接前进一步，如果这时候不是重复的，则count恢复1，由于整个数组是有序的，所以一旦出现不重复的数，则一定比这个数大，此数之后不会再有重复项。理清了上面的思路，则代码很好写了：



```
class Solution {
public:
    int removeDuplicates(int A[], int n) {
        if (n <= 2) return n;
        int pre = 0, cur = 1, count = 1;
        while (cur < n) {
            if (A[pre] == A[cur] && count == 0) ++cur;
            else {
                if (A[pre] == A[cur]) --count;
                else count = 1;
                A[++pre] = A[cur++];
            }
        }
        return pre + 1;
    }
};
```



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












