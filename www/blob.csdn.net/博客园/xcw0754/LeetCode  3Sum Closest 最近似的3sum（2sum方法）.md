# LeetCode   3Sum Closest 最近似的3sum（2sum方法） - xcw0754 - 博客园
# [LeetCode   3Sum Closest 最近似的3sum（2sum方法）](https://www.cnblogs.com/xcw0754/p/4504719.html)
![](https://images0.cnblogs.com/blog2015/641737/201505/142247262208332.png)
题意：找到最接近target的3个元素之和，并返回该和。
思路：用2个指针，时间复杂度O（n^2）。
```
1 int threeSumClosest(vector<int>& nums, int target) {
 2     int sum=nums[0]+nums[1]+nums[2];
 3     sort(nums.begin(), nums.end());
 4     for(int i=0; i<nums.size()-2; i++)
 5     {
 6         int p=i+1, q=nums.size()-1;
 7         while( p!=q )
 8         {
 9             int tmp=target-(nums[i]+nums[p]+nums[q]);
10             if(abs(tmp)<abs(sum-target) )    sum=nums[i]+nums[p]+nums[q];
11             if(!tmp)    return target;
12             if(tmp>0)    p++;
13             else        q--;
14         }
15     }
16     return sum;
17 }
AC代码
```

