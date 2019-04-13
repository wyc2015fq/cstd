
# 89. Gray Code - OraYang的博客 - CSDN博客

2017年10月30日 17:06:03[OraYang](https://me.csdn.net/u010665216)阅读数：489所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
The gray code is a binary numeral system where two successive values differ in only one bit.
Given a non-negative integer n representing the total number of bits in the code, print the sequence of gray code. A gray code sequence must begin with 0.
For example, given n = 2, return [0,1,3,2]. Its gray code sequence is:
00 - 0
01 - 1
11 - 3
10 - 2
Note:
For a given n, a gray code sequence is not uniquely defined.
For example, [0,2,3,1] is also a valid gray code sequence according to the above definition.
For now, the judge is able to judge based on one instance of gray code sequence. Sorry about that.
## 思路
这道题目，答案不唯一，本文给出其中一种解法，采用按位操作，即异或的使用，实现巧妙，理解简单
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
grayCode(
```
```python
int
```
```python
n) {
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
result;
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
i =
```
```python
0
```
```python
; i <
```
```python
1
```
```python
<<n; i++) result.push_back(i ^ i>>
```
```python
1
```
```python
);
```
```python
//异或操作
```
```python
return
```
```python
result;
    }
};
```

