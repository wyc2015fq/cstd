# leetcode 167.Two SumII（双指针遍历） - HJ - CSDN博客
2018年03月04日 15:14:57[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：92
题目链接：[https://leetcode.com/problems/two-sum-ii-input-array-is-sorted/description/](https://leetcode.com/problems/two-sum-ii-input-array-is-sorted/description/)
解题思路：
要求在一个已经按升序排列好的数组里找出两个数之和等于target，小的数在前面，大的数在后面；
开两个指针，一个指向最小数，一个最大数。每次计算两个指针指向的数之和，讨论三种情况：
①和与target相等则直接退出；
②和小于target则左指针往右移一位；
③和大于target则右指针往左移一位；
```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {
        int left = 0,right = numbers.size()-1;
        while(true) {
            int ans = numbers[left] + numbers[right];
            if(ans == target) {
                break;
            }else if(ans < target) {
                left++;
            }else {
                right--;
            }
        }
        vector<int> s;
        s.push_back(left + 1);
        s.push_back(right + 1);
        return s;
    }
};
```
[](https://leetcode.com/problems/two-sum-ii-input-array-is-sorted/description/)
