
# 128. Longest Consecutive Sequence - OraYang的博客 - CSDN博客

2018年01月26日 16:15:11[OraYang](https://me.csdn.net/u010665216)阅读数：319标签：[union-find																](https://so.csdn.net/so/search/s.do?q=union-find&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=union-find&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 题目
Given an unsorted array of integers, find the length of the longest consecutive elements sequence.
For example,
Given`[100, 4, 200, 1, 3, 2]`,
The longest consecutive elements sequence is`[1, 2, 3, 4]`. Return its length:`4`.
Your algorithm should run in O(*n*) complexity.

# 思路
这道题是个典型的寻找连接的题目（union-find），这里我们的解题思路如下：
> 将数组中的数值存储到哈希表中，这里需要O(n)的时间复杂度

> 然后遍历数组，同时每次判断num-1是否在数组中，如果存在就继续继续遍历下去，如果不存在，就num+1,num+2,判断连续最长的数是多少
讲道理这个思路很巧妙
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
longestConsecutive(
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
//扫描一遍数组，利用哈希表存储数字，如果数字出现两次只存储一个
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
int
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
item: nums)
        {
            mp[item]=
```
```python
1
```
```python
;
        }
```
```python
//再次扫描一遍数组
```
```python
int
```
```python
best=
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
item:nums)
        {
```
```python
if
```
```python
(mp.find(item-
```
```python
1
```
```python
)==mp.end())
            {
```
```python
int
```
```python
end = item +
```
```python
1
```
```python
;
```
```python
while
```
```python
(mp.find(end)!=mp.end())
                {
                    end+=
```
```python
1
```
```python
;
                }
                best = max(end-item,best);
            }
        }
```
```python
return
```
```python
best;
    }
};
```

