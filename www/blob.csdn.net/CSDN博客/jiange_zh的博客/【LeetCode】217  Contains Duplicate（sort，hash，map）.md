
# 【LeetCode】217. Contains Duplicate（sort，hash，map） - jiange_zh的博客 - CSDN博客


2016年01月08日 10:24:12[jiange_zh](https://me.csdn.net/jiange_zh)阅读数：369标签：[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[hash																](https://so.csdn.net/so/search/s.do?q=hash&t=blog)[map																](https://so.csdn.net/so/search/s.do?q=map&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=hash&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)个人分类：[算法与数据结构																](https://blog.csdn.net/jiange_zh/article/category/5787591)
[
																								](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



## Question
Given an array of integers, find if the array contains any duplicates. Your function should return true if any value appears at least twice in the array, and it should return false if every element is distinct.
## Code
使用map，用了C++11的新特性auto，十分智能方便哈哈~
```python
bool
```
```python
containsDuplicate(
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
map
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
cnt;
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
num : nums)
            cnt[num]++;
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
num : cnt)
```
```python
if
```
```python
(num.second >
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
true
```
```python
;
```
```python
return
```
```python
false
```
```python
;
    }
```
不过时间方面有点差，用了96ms
如果使用hash，减到48ms
map使用红黑树实现，查找速度是log(n)级别；
hash查找速度基本和数据量大小无关。
不过并非任何时候hash都比map快，因为hash需要hash函数，构造会比较慢。
另外，本题可以对数组进行排序，之后判断相邻元素是否有相同的即可。

