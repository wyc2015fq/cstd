
# 485. Max Consecutive Ones - OraYang的博客 - CSDN博客

2017年08月21日 15:22:25[OraYang](https://me.csdn.net/u010665216)阅读数：111所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a binary array, find the maximum number of consecutive 1s in this array.
Example 1:
Input:[1,1,0,1,1,1]Output:3Explanation:The first two digits or the last three digits are consecutive 1s.
    The maximum number of consecutive 1s is 3.
Note:
The input array will only contain0and1.
The length of input array is a positive integer and will not exceed 10,000
思路：遍历数组，同时维护两个变量，一个是count，一个是max,count每次遇0就重置为0，max在每次遇0前与count比较
代码：

```python
class Solution {
public:
    int findMaxConsecutiveOnes(vector<int>& nums) {
        int count = 0,max =0;
        for(int i =0;i<nums.size();i++){
            if(nums[i]&1)
                count++;
            else{
                max = count>max?count:max;
                count = 0;                
            }
        }
        return count>max?count:max;       
    }
};
```


