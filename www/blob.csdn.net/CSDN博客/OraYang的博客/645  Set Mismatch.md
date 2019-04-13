
# 645. Set Mismatch - OraYang的博客 - CSDN博客

2017年08月31日 09:15:22[OraYang](https://me.csdn.net/u010665216)阅读数：202标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
The set`S`originally contains numbers from 1 to`n`.
 But unfortunately, due to the data error, one of the numbers in the set got duplicated toanothernumber in the set, which results in repetition of one number and loss of another number.
Given an array`nums`representing the data status of this set after the error. Your task
 is to firstly find the number occurs twice and then find the number that is missing. Return them in the form of an array.
Example 1:
Input:nums = [1,2,2,4]Output:[2,3]
Note:
The given array size will in the range [2, 10000].
The given array's numbers won't have any order.
思路：本题，第一种方法：排序法；先将数组数据排序，那么相同的数据连续在一起，缺失数据则是相邻数据差值为2，代码如下：

```python
class Solution {
public:
    vector<int> findErrorNums(vector<int>& nums) {
        sort(nums.begin(),nums.end());
        vector<int> res;
        int missing =1;
        for(int i=1;i<nums.size();i++)
        {
            if(nums[i]==nums[i-1])
                res.insert(res.begin(),nums[i]);
            if(nums[i]-nums[i-1]==2)
            {
                missing = nums[i]-1;
            }
                
        }
        if(nums[nums.size()-1]!=nums.size())
            res.push_back(nums.size());
        else 
            res.push_back(missing);
        return res;
    }
};
```
第二种方法：利用map关联容器，存储数组里面值及出现次数的键值对，出现次数等于2是频繁项，没有出现为缺少项，代码如下：
```python
class Solution {
public:
    vector<int> findErrorNums(vector<int>& nums) {
        vector<int> res;
        map<int,int> mp;
        for(int i=0;i<nums.size();i++)
            mp[nums[i]]++;
        for(int j=1;j<=nums.size();j++)
        {
            if(mp[j]==2)
                res.insert(res.begin(),j);
            if(mp[j]==0)
            // if(mp.find(3)==mp.end())
                res.push_back(j);
        }
        return res;
    }
};
```
第三种方法：XOR,这道题目，出题还是很特殊的，如果对异或的特性比较了解，其实不管从时间复杂度还是空间复杂度，都应该首选异或的解题思路的
```python
class Solution {
public:
    vector<int> findErrorNums(vector<int>& nums) {
        vector<int> res;
        int Xor = 0;
        for(auto num:nums)
            Xor^=num;
        for(int i=1;i<=nums.size();i++)
            Xor^=i;
        int rightbit = Xor & ~(Xor - 1);//判断有无进位
        int dup=0,missing =0;
        for(auto n:nums)
        {
            if((n&rightbit)!=0)
                dup^=n;
            else
                missing^=n;
        }
        for(int i=1;i<=nums.size();i++)
        {
            if ((i & rightbit) != 0)
                dup ^= i;
            else
                missing ^= i;
        }
        for (int i = 0; i < nums.size(); i++) {
            if (nums[i] == missing)
                return {missing,dup}; 
        }
        return {dup,missing};    
    }
};
```



