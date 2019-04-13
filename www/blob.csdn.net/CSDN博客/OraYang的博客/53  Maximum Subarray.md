
# 53. Maximum Subarray - OraYang的博客 - CSDN博客

2017年07月24日 10:45:14[OraYang](https://me.csdn.net/u010665216)阅读数：178标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Find the contiguous subarray within an array (containing at least one number) which has the largest sum.
For example, given the array`[-2,1,-3,4,-1,2,1,-5,4]`,
the contiguous subarray`[4,-1,2,1]`has the largest sum =`6`.
[click to show more practice.](https://leetcode.com/problems/maximum-subarray/#)
More practice:If you have figured out the O(n) solution, try coding another solution using the divide and conquer approach, which is more subtle.
思路：这道题目是个简单的动态规划问题，可以建立一个数组来存储每次的局部最优结果，然后再用循环比较求出最大的和
代码：

```python
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int num = nums.size();
        int maxSum[num]; 
        int i;
        maxSum[0] = nums[0];
        for(i=1;i<num;i++)
        {
            maxSum[i] =max(maxSum[i-1]+nums[i],nums[i]);
        }
          for(i=1;i<num;i++)
        {
          if(maxSum[0]<maxSum[i])
              maxSum[0] = maxSum[i];
    }
        
        return maxSum[0];    
        
    }
};
/*
time limit   //本质上下面两个算法都是贪心算法
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int num = nums.size();
        int sum[num] ;
        int temp =0;
        int i = num-1;
        if(num==1)
                return nums[0];
        for( i  ; i>= 0;  --i )
        {
            sum[i] = accumulate(nums.begin(),nums.begin()+i+1, 0); 
           // cout<<sum[i]<<endl;
            if(i==num-1)
                continue;  
            temp = sum[i];
            for(int j = 0;j<num-i-1;j++)
            {
                temp = nums[j+i+1]+temp-nums[j];
                
                if(sum[i]<temp)
                sum[i] = temp; 
            }
            
        }
        for (i=0;i<num-1;i++)
        {
            cout<<sum[i]<<endl;
          if(sum[0]<sum[i+1])
              sum[0] = sum[i+1];
    }
        return sum[0];
    }
};
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int num = nums.size();
        int sum[num] ;
        int temp;
        int i = num-1;
        if(num==1)
                return nums[0];
        for( i  ; i>= 0;  --i )
        {
            sum[i] = accumulate(nums.begin(),nums.begin()+i+1, 0); 
           // cout<<sum[i]<<endl;
            if(i==num-1)
                continue;  
            for(int j = 0;j<num-i;j++)
            {
                int temp = accumulate(nums.begin()+j,nums.begin()+j+i+1, 0);
                if(sum[i]< temp )
                sum[i] =temp; 
            }
            
        }
        for (i=0;i<num-1;i++)
        {
            cout<<sum[i]<<endl;
          if(sum[0]<sum[i+1])
              sum[0] = sum[i+1];
    }
        return sum[0];
    }
};
*/
```


