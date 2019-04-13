
# 【LeetCode】326. Power of Three （优雅的数学解法） - jiange_zh的博客 - CSDN博客


2016年01月11日 10:47:33[jiange_zh](https://me.csdn.net/jiange_zh)阅读数：1250



## Question
Given an integer, write a function to determine if it is a power of three.
Follow up:
Could you do it without using any loop / recursion?
## Solution
This can be generalized for any 【prime】 number n.
Lets say we have 2 number m & n.
```python
If
```
```python
m
```
```python
is
```
```python
a power
```
```python
of
```
```python
n
```
```python
then
```
```python
for
```
```python
any number p,
```
```python
1.
```
```python
For
```
```python
all
```
```python
p<=m
```
```python
2.
```
```python
m%p =
```
```python
0
```
```python
if
```
```python
and
```
```python
only
```
```python
if
```
```python
p
```
```python
is
```
```python
also a power
```
```python
of
```
```python
n
```
We can use this concept here also. In this case n=3 and m is largest 32bit signed integer which is power of 3， i.e. 1162261467.
```python
bool
```
```python
isPowerOfThree(
```
```python
int
```
```python
p) {
```
```python
return
```
```python
p>
```
```python
0
```
```python
&&
```
```python
1162261467
```
```python
%p ==
```
```python
0
```
```python
;
}
```
特别注意，上面解法只适用于素数。
另外有一些其他解法：
3的倍数在int范围内是有限的，所以可以打表，用hash_map来查找。
## Another question
231.Power of Two
Given an integer, write a function to determine if it is a power of two.
## Solution
解法1：
跟上面那题同理，2的power也满足该定理：
```python
bool
```
```python
isPowerOfTwo(
```
```python
int
```
```python
n) {
```
```python
return
```
```python
(n >
```
```python
0
```
```python
) && (!(
```
```python
2147483648
```
```python
%n));
```
```python
//2147483648 = 2^31
```
```python
}
```
解法2：
从n的二进制角度看，如果n是2的power，那么n的二进制表示中有且仅有一个1位。
如果判断这个1位？
只需要判断 n&(n-1) 是否为0，如果是，则说明n中只有一个1位。
```python
bool
```
```python
isPowerOfTwo(
```
```python
int
```
```python
n) {
```
```python
return
```
```python
n>
```
```python
0
```
```python
&& !(n&(n-
```
```python
1
```
```python
));
    }
```
这种解法更加高明，位运算很快，而且充分利用了二进制的性质。

