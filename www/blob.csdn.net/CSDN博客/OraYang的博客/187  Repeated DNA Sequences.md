
# 187. Repeated DNA Sequences - OraYang的博客 - CSDN博客

2018年01月13日 11:45:45[OraYang](https://me.csdn.net/u010665216)阅读数：137标签：[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[medium																](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[hashtable																](https://so.csdn.net/so/search/s.do?q=hashtable&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)[
							](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



# 题目
All DNA is composed of a series of nucleotides abbreviated as A, C, G, and T, for example: “ACGAATTCCG”. When studying DNA, it is sometimes useful to identify repeated sequences within the DNA.
Write a function to find all the 10-letter-long sequences (substrings) that occur more than once in a DNA molecule.
For example,
Given s = "AAAAACCCCCAAAAACCCCCCAAAAAGGGTTT",
Return:
["AAAAACCCCC", "CCCCCAAAAA"].

# 思路
本题最直接的思路就是，建立一个hashtable，存储string及对应个数的map，再loop一次，找出出现次数大于1次的string并将其存储到数组里。
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
string
```
```python
>
```
```python
findRepeatedDnaSequences(
```
```python
string
```
```python
s) {
```
```python
vector
```
```python
<
```
```python
string
```
```python
>
```
```python
res;
```
```python
if
```
```python
(s.size()<=
```
```python
10
```
```python
)
```
```python
return
```
```python
res;
```
```python
unordered_map
```
```python
<
```
```python
string
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
i=
```
```python
0
```
```python
;i<=s.size()-
```
```python
10
```
```python
;i++)
        {
            mp[s.substr(i,
```
```python
10
```
```python
)]++;
        }
```
```python
unordered_map
```
```python
<
```
```python
string
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
::iterator it = mp.begin();
```
```python
while
```
```python
(it!=mp.end())
        {
```
```python
if
```
```python
(it->second>
```
```python
1
```
```python
)
                res.push_back(it->first);
            it++;
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

