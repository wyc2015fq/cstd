
# 674. Longest Continuous Increasing Subsequence - OraYang的博客 - CSDN博客

2018年03月20日 09:01:38[OraYang](https://me.csdn.net/u010665216)阅读数：237



# 题目
Given an unsorted array of integers, find the length of longest`continuous`increasing subsequence (subarray).
**Example 1:**
**Input:**[1,3,5,4,7]**Output:**3**Explanation:**The longest continuous increasing subsequence is [1,3,5], its length is 3. 
Even though [1,3,5,7] is also an increasing subsequence, it's not a continuous one where 5 and 7 are separated by 4.
**Example 2:**
**Input:**[2,2,2,2,2]**Output:**1**Explanation:**The longest continuous increasing subsequence is [2], its length is 1.
**Note:**Length of the array will not exceed 10,000.

# 思路
本题是个典型的动态规划求解;要求递增子序列的最大长度，那么每次求解以索引index为结尾的递增序列的长度maxendinghere。再维护一个maxsofar来存储到目前为止最大的长度。本题是编程珠玑第八章算法设计中提到的扫描算法，时间复杂度为O(N)
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
findLengthOfLCIS(
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
int
```
```python
maxendinghere =
```
```python
1
```
```python
;
```
```python
int
```
```python
maxsofar =
```
```python
1
```
```python
;
```
```python
if
```
```python
(nums.size()==
```
```python
0
```
```python
)
```
```python
return
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
int
```
```python
i=
```
```python
1
```
```python
;i<nums.size();i++)
        {
```
```python
if
```
```python
(nums[i]>nums[i-
```
```python
1
```
```python
])
                maxendinghere++;
```
```python
else
```
```python
maxendinghere =
```
```python
1
```
```python
;
            maxsofar = max(maxsofar,maxendinghere);
        }
```
```python
return
```
```python
maxsofar;
    }
};
```

