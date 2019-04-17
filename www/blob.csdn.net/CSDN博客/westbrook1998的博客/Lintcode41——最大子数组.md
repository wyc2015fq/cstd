# Lintcode41——最大子数组 - westbrook1998的博客 - CSDN博客





2018年02月19日 19:17:33[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：70








## 题目：

> 
一样的套路，动态规划问题 

  给定一个整数数组，找到一个具有最大和的子数组，返回其最大和。  

注意事项 

  子数组最少包含一个数  

## 题解：

```
public class Solution {
    /*
     * @param nums: A list of integers
     * @return: A integer indicate the sum of max subarray
     */
    public int maxSubArray(int[] nums) {
        // write your code here
        int n=nums.length;
        int max=nums[0];
        for(int i=1;i<n;i++){
            if(nums[i-1]>=0)
                nums[i]=nums[i-1]+nums[i];
            if(nums[i]>max)
                max=nums[i];
        }
        return max;
    }
}
从第二个数开始判断，如果前面的和大于等于0，就加上，否则舍弃。
```




