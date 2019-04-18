# 3Sum Closest ——解题报告 - bigfacesafdasgfewgf - CSDN博客





2015年05月07日 10:36:15[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：617











    【题目】

    Given an array S of n integers, find three integers in S such that the sum is closest to a given number, target. Return the sum of the three integers. You may assume that each input would have
 exactly one solution.


For example, given array S = {-1 2 1 -4}, and target = 1.


The sum that is closest to the target is 2. (-1 + 2 + 1 = 2).




    【分析】

    这道题目与寻找和为定值的3个数类似。


遍历a，然后需要最接近target-a的两个数b和c。这个子问题可以用two pointers的思路来做。记住，如果a+b+c正好等于target，直接返回target。如果不等，才需要把较大还是较小分情况讨论。




    【代码】

     运行时间：17ms



```cpp
class Solution {
public:
    int threeSumClosest(vector<int>& nums, int target) {
        if(nums.size() < 3)
            return 0;
            
        sort(nums.begin(), nums.end());
        int res = nums[0] + nums[1] + nums[2];
        for(int i = 0; i < nums.size() - 2; i++)
        {
            int lo = i + 1; 
            int hi = nums.size() - 1; 
            while(lo < hi)
            {
                int sum = nums[i] + nums[lo] + nums[hi];
                int diff1 = (sum - target > 0) ? (sum - target) : (target - sum);  // the non-negative difference
                int diff2 = (res - target > 0) ? (res - target) : (target - res);
                res = diff1 < diff2 ? sum : res; // res corresponding to the minimal diff
                if(diff1 == 0)
                    return res;
                else if(sum < target)
                    lo++;
                else
                    hi--;
            }
        }
        return res;
    }
};
```





