
# 164. Maximum Gap - OraYang的博客 - CSDN博客

2018年03月02日 17:41:15[OraYang](https://me.csdn.net/u010665216)阅读数：185所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 题目
Given an unsorted array, find the maximum difference between the successive elements in its sorted form.
Try to solve it in linear time/space.
Return 0 if the array contains less than 2 elements.
You may assume all elements in the array are non-negative integers and fit in the 32-bit signed integer range.

# 思路
本题给定一个无序数组，要你求有序情况下相邻数值的最大差值。如果没有时间复杂度，那么先排序再求差值是显而易见的方案。但是题目要求必须线性时间与空间。因此我们必须思考出能够解决这个问题的好方法。这道题目我们我们可以推导出一个准确的结论：假如数组的最大值为max，最小值为min，数组大小为N，那么最大的差值gap一定不小于平均gap:
ceil((max−min)N−1)$ceil((max-min)/N-1)$
假如我们按照gap来将数值分段，对应数值放到对应的区域，是不是就能求出最大gap了。上面的思路就是桶排序的思想。我们建立N-1个桶，每个桶的大小为平均gap，那么最大的gap就一定发生在不同桶之间的数之间了。
具体的思路请看代码注释
# 代码
```python
class Solution {
```
```python
public
```
```python
int
```
```python
maximumGap
```
```python
(
```
```python
int
```
```python
[] nums) {
```
```python
//Return 0 if the array contains less than 2 elements.
```
```python
if
```
```python
(nums.length<
```
```python
2
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
//find max and min
```
```python
int
```
```python
max = nums[
```
```python
0
```
```python
];
```
```python
int
```
```python
min = nums[
```
```python
1
```
```python
];
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
i : nums){
            max = Math.max(max,i);
            min = Math.min(min,i);
        }
```
```python
//gap/bucket size
```
```python
int
```
```python
gap = (
```
```python
int
```
```python
)Math.ceil((
```
```python
double
```
```python
)(max-min)/(nums.length-
```
```python
1
```
```python
));
```
```python
//the number of bucket nums.length-1
```
```python
//keep two array to store min and max of the bucket separately
```
```python
int
```
```python
[] bucketmax =
```
```python
new
```
```python
int
```
```python
[nums.length-
```
```python
1
```
```python
];
```
```python
int
```
```python
[] bucketmin =
```
```python
new
```
```python
int
```
```python
[nums.length-
```
```python
1
```
```python
];
        Arrays.fill(bucketmin, Integer.MAX_VALUE);
        Arrays.fill(bucketmax, Integer.MIN_VALUE);
```
```python
//put num into bucket
```
```python
//[min + (k-1)gap, min + k*gap).
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
i: nums){
```
```python
if
```
```python
(i==min||i==max)
```
```python
continue
```
```python
;
```
```python
int
```
```python
idx = (i-min)/gap;
            bucketmax[idx] = Math.max(i,bucketmax[idx]);
            bucketmin[idx] = Math.min(i,bucketmin[idx]);
        }
```
```python
//find max gap;
```
```python
int
```
```python
maxgap = Integer.MIN_VALUE;
```
```python
int
```
```python
previous = min;
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
;i<nums.length-
```
```python
1
```
```python
;i++)
        {
```
```python
if
```
```python
(bucketmin[i]==Integer.MAX_VALUE&&bucketmax[i]==Integer.MIN_VALUE)
```
```python
continue
```
```python
;
            maxgap = Math.max(maxgap,bucketmin[i]-previous);
            previous = bucketmax[i];
        }
        maxgap = Math.max(maxgap,max-previous);
```
```python
return
```
```python
maxgap;
    }
}
```

