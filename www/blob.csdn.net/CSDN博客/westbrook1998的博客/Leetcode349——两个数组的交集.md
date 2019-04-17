# Leetcode349——两个数组的交集 - westbrook1998的博客 - CSDN博客





2018年04月30日 20:35:35[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：563








> 
给定两个数组，写一个函数来计算它们的交集。


三种方法，从暴力到优化 

1. O(n²) 

两个for循环直接遍历
```cpp
class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        int s1=nums1.size();
        int s2=nums2.size();
        vector<int> res;
        for(int i=0;i<s1;i++){
            for(int j=0;j<s2;j++){
                if(nums1[i]==nums2[j] && count(res.begin(),res.end(),nums1[i])==0 ){
                    res.push_back(nums1[i]);
                }
            }
        }
        return res;
    }
};
```
- O(nlogn) 

先对数组进行排序，然后用两个指针移动比较

```cpp
class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        int s1=nums1.size();
        int s2=nums2.size();
        sort(nums1.begin(),nums1.end());
        sort(nums2.begin(),nums2.end());
        int i=0,j=0;
        vector<int> res;
        while(i<s1 && j<s2){
            if(nums1[i]>nums2[j]){
                j++;
            }
            else if(nums1[i]<nums2[j]){
                i++;
            }
            else{
                if(count(res.begin(),res.end(),nums1[i])==0){
                    res.push_back(nums1[i]);
                }
                i++;
                j++;
            }
        }
        return res;
    }
};
```
- O(n)
``



