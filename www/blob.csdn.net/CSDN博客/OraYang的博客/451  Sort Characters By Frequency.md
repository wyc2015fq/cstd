
# 451. Sort Characters By Frequency - OraYang的博客 - CSDN博客

2018年01月21日 19:05:18[OraYang](https://me.csdn.net/u010665216)阅读数：173标签：[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[medium																](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[hashtmap																](https://so.csdn.net/so/search/s.do?q=hashtmap&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)[
							](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



# 题目
Given a string, sort it in decreasing order based on the frequency of characters.
**Example 1:**
**Input:**"tree"**Output:**"eert"**Explanation:**'e' appears twice while 'r' and 't' both appear once.
So 'e' must appear before both 'r' and 't'. Therefore "eetr" is also a valid answer.
**Example 2:**
**Input:**"cccaaa"**Output:**"cccaaa"**Explanation:**Both 'c' and 'a' appear three times, so "aaaccc" is also a valid answer.
Note that "cacaca" is incorrect, as the same characters must be together.
**Example 3:**
**Input:**"Aabb"**Output:**"bbAa"**Explanation:**"bbaA" is also a valid answer, but "Aabb" is incorrect.
Note that 'A' and 'a' are treated as two different characters.

# 思路
本题思路很简单，是个典型的hashmap+优先队列的思想，总共只需要遍历三遍；分为以下三步：
> step1:遍历一遍字符串，同时存储字符并统计出现的次数

> step2:遍历哈希表，利用优先队列排序

> step3:遍历优先队列，输出结果
具体流程见代码：
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
string
```
```python
frequencySort(
```
```python
string
```
```python
s) {
```
```python
unordered_map
```
```python
<
```
```python
char
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
        priority_queue<pair<
```
```python
int
```
```python
,
```
```python
char
```
```python
>> pq;
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
item:s)
        {
            mp[item]++;
        }
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
it = mp.begin();it!=mp.end();it++)
        {
            pq.push(make_pair(it->second,it->first));
        }
```
```python
string
```
```python
res =
```
```python
""
```
```python
;
```
```python
while
```
```python
(!pq.empty())
        {
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
;i<pq.top().first;i++)
            {
                res+=pq.top().second;
            }
            pq.pop();
        }
```
```python
return
```
```python
res;
    }
};
```

