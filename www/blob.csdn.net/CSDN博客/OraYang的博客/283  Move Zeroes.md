
# 283. Move Zeroes - OraYang的博客 - CSDN博客

2017年08月04日 10:50:55[OraYang](https://me.csdn.net/u010665216)阅读数：117标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given an array`nums`, write a function to move all`0`'s
 to the end of it while maintaining the relative order of the non-zero elements.
For example, given`nums = [0, 1, 0, 3, 12]`, after calling your function,`nums`should
 be`[1, 3, 12, 0, 0]`.
Note:
You must do thisin-placewithout making a copy of the array.
Minimize the total number of operations.
思路：本题有个简单明了的思路，维护一个变量count ，遍历一次数组，在遍历的同时判断数值是否为0，为0，count++，并将0删除，最后在数组末尾添上对应个数的0即可
代码：

```python
class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        vector<int>::iterator itr=nums.begin();
        int count =0;
        while(1)
        {
            if(itr==nums.end())
                break;
            if(*itr==0)
            {
                count++;
                nums.erase(itr); 
                continue;
            }
            itr++;
        }
        for(int j=0;j<count;j++)
            nums.push_back(0);
        
    }
};
```


