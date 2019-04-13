
# 496. Next Greater Element I - OraYang的博客 - CSDN博客

2017年08月21日 18:00:53[OraYang](https://me.csdn.net/u010665216)阅读数：107所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
You are given two arrays(without duplicates)`nums1`and`nums2`where`nums1`’s
 elements are subset of`nums2`. Find all the next greater numbers for`nums1`'s
 elements in the corresponding places of`nums2`.
The Next Greater Number of a numberxin`nums1`is
 the first greater number to its right in`nums2`. If it does not exist, output -1 for this
 number.
Example 1:
Input:nums1= [4,1,2],nums2= [1,3,4,2].Output:[-1,3,-1]Explanation:For number 4 in the first array, you cannot find the next greater number for it in the second array, so output -1.
    For number 1 in the first array, the next greater number for it in the second array is 3.
    For number 2 in the first array, there is no next greater number for it in the second array, so output -1.
Example 2:
Input:nums1= [2,4],nums2= [1,2,3,4].Output:[3,-1]Explanation:For number 2 in the first array, the next greater number for it in the second array is 3.
    For number 4 in the first array, there is no next greater number for it in the second array, so output -1.
Note:
All elements innums1andnums2are unique.
The length of bothnums1andnums2would not exceed 1000.
思路：本题思路上很简单，就是对num1中的每个数值通过find函数找到num2中的位置，再判断是否存在next greater num，如果存在这将值替换成next greater num，否则替换为-1
代码：

```python
class Solution {
public:
    vector<int> nextGreaterElement(vector<int>& findNums, vector<int>& nums) {
        for(int i =0;i<findNums.size();i++)
        {
            int flag =1;
            vector<int>::iterator itr = find(nums.begin(),nums.end(),findNums[i]);
            while(itr!=nums.end()){
                if(*itr>findNums[i]){
                    findNums[i] = *itr;
                    flag = 0;
                    break;
                }
                itr++;
            }
            if(flag)
                findNums[i] = -1;
        }
        return findNums;
    }
};
```


