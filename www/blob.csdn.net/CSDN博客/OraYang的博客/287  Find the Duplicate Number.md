
# 287. Find the Duplicate Number - OraYang的博客 - CSDN博客

2017年12月07日 11:14:10[OraYang](https://me.csdn.net/u010665216)阅读数：286所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Given an array nums containing n + 1 integers where each integer is between 1 and n (inclusive), prove that at least one duplicate number must exist. Assume that there is only one duplicate number, find the duplicate one.
Note:
> You must not modify the array (assume the array is read only).

> You must use only constant, O(1) extra space.

> Your runtime complexity should be less than O(n2).

> There is only one duplicate number in the array, but it could be repeated more than once.

## 思路
本题给定一个大小为$n+1$的数组，数组里面的值的值域为$[1，n]$；那么请找出数组里面的重复项。这道题目如果仔细分析的话，本质上就是个快慢指针的问题。首先什么是快慢指针呢？
### 快慢指针
快慢指针其实是计算机系统原理里的知识点，快慢指针中的快慢指的是移动的步长，即每次向前移动速度的快慢。例如可以让快指针每次沿链表向前移动2，慢指针每次向前移动1次。
通过结合应用，我们发现快慢指针有两大总结：
> 判断循环链表，假如快指针步长为2，慢指针步长为1，那么当快慢指针同时从首结点出发时，如果在某个时刻两个指针相等，那么链表必定是循环链表。

> 判断循环链表入口点，当两个指针相等时，此时快指针从首结点出发，步长为1，慢指针继续出发，两个指针必定在循环链表入口点相遇。
有了前面快慢指针的理解那么我们怎么应用到这道题目中来呢，我们首先举个例子说明下情况：
|1|3|4|2|2|
|---|---|---|---|---|
|0|1|2|3|4|
上面表格第一行代表数组nums里面的数值，第二行代表数组里面数值的索引。
很明显，假如我们通过以下方式来获取数值：
定义一个变量num，第一次，num = nums[0]；第二次num = nums[num]；第三次num = nums[num]，那么获取的数值变化如下所示：
|1|3|2|4|2|4|2|
|---|---|---|---|---|---|---|
|0|1|3|2|4|2|4|
很明显，通过每次nums[num]的赋值，数值最后会进入一个循环，说明只要这个数组里存在重复项，那么按照刚刚的赋值方法必定会进入循环。而且我们发现首次进入循环时的数字就是我们的重复项。因此我们可以通过建立快慢指针，找到循环数组的入口点，这个入口点的数值就是本题需要求解的重复项！
## 代码
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
findDuplicate(
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
if
```
```python
(nums.size()>
```
```python
1
```
```python
)
        {
```
```python
int
```
```python
slow = nums[
```
```python
0
```
```python
],fast=nums[nums[
```
```python
0
```
```python
]];
```
```python
while
```
```python
(slow!=fast)
            {
                slow = nums[slow];
                fast = nums[nums[fast]];
            }
            fast =
```
```python
0
```
```python
;
```
```python
while
```
```python
(slow!=fast)
            {
                fast = nums[fast];
                slow = nums[slow];
            }
```
```python
return
```
```python
slow;
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

