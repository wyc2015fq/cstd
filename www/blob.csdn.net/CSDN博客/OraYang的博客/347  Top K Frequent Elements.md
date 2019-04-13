
# 347. Top K Frequent Elements - OraYang的博客 - CSDN博客

2018年01月14日 21:37:47[OraYang](https://me.csdn.net/u010665216)阅读数：168标签：[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[medium																](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[hashtable																](https://so.csdn.net/so/search/s.do?q=hashtable&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)[
							](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



# 题目
Given a non-empty array of integers, return the**k**most frequent elements.
For example,
Given`[1,1,1,2,2,3]`and k = 2, return`[1,2]`.
**Note:**
You may assumekis always valid, 1 ≤k≤ number of unique elements.
Your algorithm’s time complexitymust bebetter than O(nlogn), wherenis the array’s size.


# 思路
本题采用哈希表与优先队列的思想，思路比较简单
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
topKFrequent(
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
& nums,
```
```python
int
```
```python
k) {
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
auto
```
```python
item : nums)
        {
            mp[item]++;
        }
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
res;
        priority_queue<pair<
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
it = mp.begin();it!=mp.end();it++)
        {
            pq.push(make_pair(it->second,it->first));
```
```python
if
```
```python
(pq.size()>mp.size()-k)
            {
                res.push_back(pq.top().second);
                pq.pop();
            }
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

