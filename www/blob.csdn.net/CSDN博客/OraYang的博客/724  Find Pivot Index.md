
# 724. Find Pivot Index - OraYang的博客 - CSDN博客

2018年03月21日 09:25:27[OraYang](https://me.csdn.net/u010665216)阅读数：315



# 题目
Given an array of integers`nums`, write a method that returns the “pivot” index of this array.
We define the pivot index as the index where the sum of the numbers to the left of the index is equal to the sum of the numbers to the right of the index.
If no such index exists, we should return -1. If there are multiple pivot indexes, you should return the left-most pivot index.
**Example 1:**
**Input:**nums = [1, 7, 3, 6, 5, 6]**Output:**3**Explanation:**The sum of the numbers to the left of index 3 (nums[3] = 6) is equal to the sum of numbers to the right of index 3.
Also, 3 is the first index where this occurs.
**Example 2:**
**Input:**nums = [1, 2, 3]**Output:**-1**Explanation:**There is no index that satisfies the conditions in the problem statement.
**Note:**
- The length of- nums- will be in the range- [0, 10000]- .
- Each element- nums[i]- will be an integer in the range- [-1000, 1000]- .


# 思路
本题的思路就是寻找数组里数据的规律：如果存在这个index，那么sumfront∗2+nums[i]=sumall$sumfront*2+nums[i] = sumall$其中，sumfront指前半部分的和，sumall指后半部分的和。
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
pivotIndex(
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
sumfront =
```
```python
0
```
```python
;
```
```python
int
```
```python
sumall =
```
```python
0
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
1
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
if
```
```python
(nums.size()<
```
```python
3
```
```python
)
```
```python
return
```
```python
-
```
```python
1
```
```python
;
```
```python
//compute sum
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
            sumall+=nums[i];
        }
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
j=
```
```python
0
```
```python
;j<nums.size();j++)
        {
```
```python
if
```
```python
(
```
```python
2
```
```python
*sumfront+nums[j]==sumall)
```
```python
return
```
```python
j;
```
```python
else
```
```python
{
                sumfront+=nums[j];
            }
        }
```
```python
return
```
```python
-
```
```python
1
```
```python
;
    }
};
```

