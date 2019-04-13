
# 697. Degree of an Array - OraYang的博客 - CSDN博客

2018年03月19日 19:58:24[OraYang](https://me.csdn.net/u010665216)阅读数：252所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 题目
Given a non-empty array of non-negative integers`nums`, the**degree**of this array is defined as the maximum frequency of any one of its elements.
Your task is to find the smallest possible length of a (contiguous) subarray of`nums`, that has the same degree as`nums`.
**Example 1:**
**Input:**[1, 2, 2, 3, 1]**Output:**2**Explanation:**The input array has a degree of 2 because both elements 1 and 2 appear twice.
Of the subarrays that have the same degree:
[1, 2, 2, 3, 1], [1, 2, 2, 3], [2, 2, 3, 1], [1, 2, 2], [2, 2, 3], [2, 2]
The shortest length is 2. So return 2.
**Example 2:**
**Input:**[1,2,2,3,1,4,2]**Output:**6
**Note:**
- nums.length- will be between 1 and 50,000.
- nums[i]- will be an integer between 0 and 49,999.


# 思路
本题分三步来解决这个问题：
第一步是利用一个map来存储值与对应的索引，
第二步是扫描map找到这个数组的度
第三步是扫描数组，找到最短的子数组
# 代码
```python
class
```
```python
Solution {
```
```python
public
```
```python
:
```
```python
int
```
```python
findShortestSubArray(
```
```python
vector
```
```python
<
```
```python
int
```
```python
>
```
```python
& nums) {
```
```python
unordered_map
```
```python
<
```
```python
int
```
```python
,
```
```python
vector
```
```python
<
```
```python
int
```
```python
>
```
```python
>
```
```python
mp;
```
```python
for
```
```python
(
```
```python
int
```
```python
i=
```
```python
0
```
```python
;i<nums.size();i++)
        {
            mp[nums[i]].push_back(i);
        }
```
```python
int
```
```python
maxfreq =
```
```python
0
```
```python
;
```
```python
for
```
```python
(
```
```python
auto
```
```python
it=mp.begin();it!=mp.end();it++)
        {
            maxfreq = max(maxfreq,
```
```python
int
```
```python
(it->second.size()));
```
```python
//cout<<it->second.size()<<endl;
```
```python
}
```
```python
int
```
```python
minlen = nums.size();
```
```python
for
```
```python
(
```
```python
auto
```
```python
it=mp.begin();it!=mp.end();it++)
        {
```
```python
if
```
```python
(it->second.size()==maxfreq)
            {
                minlen = min(minlen,it->second.back()-it->second[
```
```python
0
```
```python
]+
```
```python
1
```
```python
);
            }
        }
```
```python
return
```
```python
minlen;
    }
};
```

