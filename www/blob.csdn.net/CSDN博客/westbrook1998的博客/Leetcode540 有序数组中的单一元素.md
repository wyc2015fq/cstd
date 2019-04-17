# Leetcode540. 有序数组中的单一元素 - westbrook1998的博客 - CSDN博客





2018年05月01日 22:39:28[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：135








算是想了一个自认为巧妙的方法（其实并没有…）

> 
给定一个只包含整数的有序数组，每个元素都会出现两次，唯有一个数只会出现一次，找出这个数。


> 
示例 1: 

  输入: [1,1,2,3,3,4,4,8,8] 

  输出: 2
> 
示例 2: 

  输入: [3,3,7,7,10,11,11] 

  输出: 10 

  注意: 您的方案应该在 O(log n)时间复杂度和 O(1)空间复杂度中运行。
代码：

```cpp
class Solution {
public:
    int singleNonDuplicate(vector<int>& nums) {
        int n=nums.size()/2;
        int i=0;
        for(;i<n;i++){
            if(nums[i*2]!=nums[i*2+1]){
                return nums[i*2];
            }
        }
        return nums[i*2];
    }
};
```

不过我也不知道这样是O(n)还是…







