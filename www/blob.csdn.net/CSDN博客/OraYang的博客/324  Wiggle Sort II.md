
# 324. Wiggle Sort II - OraYang的博客 - CSDN博客

2018年03月01日 14:17:30[OraYang](https://me.csdn.net/u010665216)阅读数：192所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 题目
Given an unsorted array`nums`, reorder it such that`nums[0] < nums[1] > nums[2] < nums[3]...`.
**Example:**
(1) Given`nums = [1, 5, 1, 1, 6, 4]`, one possible answer is`[1, 4, 1, 5, 1, 6]`.
(2) Given`nums = [1, 3, 2, 2, 3, 1]`, one possible answer is`[2, 3, 1, 3, 1, 2]`.
**Note:**
You may assume all input has valid answer.
**Follow Up:**
Can you do it in O(n) time and/or in-place with O(1) extra space?

# 思路
本题是个典型的荷兰国旗问题（DNF）。通过观察题目我们发现要想满足题目中大小关系，必须找到一个中位数M,将大于等于M的数放在奇数索引位，将小于等于M的数放在偶数索引位。按照上述规则我们就能解决该题目了。
根据集体思路我们将方案分成两部份：
> 找到中位数（C++里面有现成的库函数nth_element()）

> 荷兰国旗问题
这里我们着重强调下荷兰国旗问题在本题中的应用。因为奇偶索引的关系，导致数组被分成了三部分，我们维护三个指针：
> 起始指针（奇数位）

> 末尾指针（偶数位）

> 当前指针（扫描指针）
我们利用扫描指针扫描数组，大于中位数的我们将其与奇数位互换，小于中位数的我们将其与偶数位互换，等于中位数的，我们将扫描指针自增1。
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
void
```
```python
wiggleSort(
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
n = nums.size();
```
```python
// Find a median.
```
```python
auto
```
```python
midptr = nums.begin() + n /
```
```python
2
```
```python
;
        nth_element(nums.begin(),midptr,nums.end());
```
```python
int
```
```python
midvalue = *midptr;
```
```python
//define
```
```python
#define A(i) nums[(1+2*i)%(nums.size()|1)]
```
```python
//DNF
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
,j=
```
```python
0
```
```python
,k=n-
```
```python
1
```
```python
;
```
```python
//i begin,k end,j scan
```
```python
while
```
```python
(j<=k)
        {
```
```python
if
```
```python
(A(j)>midvalue)
            {
                swap(A(i++),A(j++));   
            }
```
```python
else
```
```python
if
```
```python
(A(j)<midvalue)
            {
                swap(A(j),A(k--));
            }
```
```python
else
```
```python
j++;
        }
    }
};
```

