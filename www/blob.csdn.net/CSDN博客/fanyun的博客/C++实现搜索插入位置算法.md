# C++实现搜索插入位置算法 - fanyun的博客 - CSDN博客
2018年12月31日 17:53:45[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：77
1.要求，给定一个排序数组和一个目标值，在数组中找到目标值，并返回其索引。如果目标值不存在于数组中，返回它将会被按顺序插入的位置。假设数组中无重复元素。
2.思路
    如输入数组A[1,3,5,6],5,输出2；输入B[1,3,5,6],2，输出1.
3.代码实现
```cpp
#include <iostrem>
using namespace std;
class Solution {
public:
    int searchInsert(vector<int>& nums, int target) 
    {
        int left=0;
        int right=nums.size()-1;
        int mid;
        int res=-1;
        while(left<=right)
        {
            mid = (left+right)/2;
            if(nums[mid]==target)
                return mid;
            else if(nums[mid]>target)
            {
                right = mid-1;
            }
            else
            {
                left = mid+1;
                res = mid;             
            }
                
        }
        return res+1;
    }
};
int main()
{
   
   Solution();
   return 0;   
}
```
