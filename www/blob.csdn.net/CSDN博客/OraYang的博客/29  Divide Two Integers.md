
# 29. Divide Two Integers - OraYang的博客 - CSDN博客

2017年09月13日 09:55:23[OraYang](https://me.csdn.net/u010665216)阅读数：118个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Divide two integers without using multiplication, division and mod operator.
If it is overflow, return MAX_INT.
## 思路
本题不能用除法、乘法、取模算子，但是学过计算机基础的人都知道，乘除法的本质就是加减法。故，本题用减法来计算，但是为了减少“减法”操作次数，故每次将除数乘2来迅速逼近被除数，此外需要注意的是，本题的小细节：1.结果的正负号，可以直接算出；2.当除数为0时，溢出；3.当被除数取最大值MAX_INT,如果除数为-1，也溢出
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
divide(
```
```python
int
```
```python
dividend,
```
```python
int
```
```python
divisor) {
```
```python
if
```
```python
(!divisor || (dividend == INT_MIN && divisor == -
```
```python
1
```
```python
))
```
```python
return
```
```python
INT_MAX;
```
```python
int
```
```python
sign = ((dividend <
```
```python
0
```
```python
) ^ (divisor <
```
```python
0
```
```python
)) ? -
```
```python
1
```
```python
:
```
```python
1
```
```python
;
```
```python
long
```
```python
long
```
```python
dvd =
```
```python
labs
```
```python
(dividend);
```
```python
long
```
```python
long
```
```python
dvs =
```
```python
labs
```
```python
(divisor);
```
```python
int
```
```python
res =
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
(dvd >= dvs) {
```
```python
long
```
```python
long
```
```python
temp = dvs, multiple =
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
(dvd >= (temp <<
```
```python
1
```
```python
)) {
                temp <<=
```
```python
1
```
```python
;
                multiple <<=
```
```python
1
```
```python
;
            }
            dvd -= temp;
            res += multiple;
        }
```
```python
return
```
```python
sign ==
```
```python
1
```
```python
? res : -res;          
    }
};
```

