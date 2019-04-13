
# 274. H-Index - OraYang的博客 - CSDN博客

2018年01月13日 13:57:06[OraYang](https://me.csdn.net/u010665216)阅读数：191标签：[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[medium																](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[hashtable																](https://so.csdn.net/so/search/s.do?q=hashtable&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)[
							](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



# 题目
Given an array of citations (each citation is a non-negative integer) of a researcher, write a function to compute the researcher’s h-index.
According to the[definition of h-index on Wikipedia](https://en.wikipedia.org/wiki/H-index): “A scientist has index*h*if*h*of his/her*N*papers have**at least***h*citations each, and the other*N − h*papers have**no more than***h*citations each.”
For example, given`citations = [3, 0, 6, 1, 5]`, which means the researcher has`5`papers in total and each of them had received`3, 0, 6, 1, 5`citations respectively. Since the researcher has`3`papers with**at least**`3`citations each and the remaining two with**no more than**`3`citations each, his h-index is`3`.
**Note**: If there are several possible values for`h`, the maximum one is taken as the h-index.

# 思路
本题思路比较简单，但是容易理解错误题意，这题目的意思是给定一个数组，数组大小为N，表示一位研究人员一共发了N篇文章，其中数组中的数值代表每篇文章被引用的次数。我们所要求解的h大小需要满足条件：
> A scientist has index h if h of his/her N papers have at least h citations each, and the other N − h papers have no more than h citations each.
本题采用一个hashtable来存储每个hash表中数字出现的次数，其中数字大于N的统一处理（见代码）。下面给一个特殊例子，利于大家理解：
> [6,7,8,9,10]，这里h=5：当数组里的数值均大于N=5时，h=N

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
hIndex(
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
& citations) {
```
```python
int
```
```python
size = citations.size(),res=
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
(size<=
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
item: citations){
```
```python
if
```
```python
(item>size)
                mp[size]++;
```
```python
else
```
```python
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
int
```
```python
i=size;i>=
```
```python
0
```
```python
;i--)
        {
            res+=mp[i];
```
```python
if
```
```python
(res>=i)
```
```python
return
```
```python
i;
        }
    }
};
```

