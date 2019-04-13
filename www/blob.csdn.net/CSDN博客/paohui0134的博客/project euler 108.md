
# project euler 108 - paohui0134的博客 - CSDN博客


2016年09月04日 17:42:21[paohui0134](https://me.csdn.net/paohui0134)阅读数：261


# peoject euler 108
## 题目
### 原题
Diophantine reciprocals I
In the following equation x, y, and n are positive integers.
1/x + 1/y = 1/n
For n = 4 there are exactly three distinct solutions:
1/5 + 1/20 = 1/4
1/6 + 1/12 = 1/4
1/8 + 1/8 = 1/4
What is the least value of n for which the number of distinct solutions exceeds one-thousand?
NOTE: This problem is an easier version of Problem 110; it is strongly advised that you solve this one first.
### 简单翻译
丢番图的倒数
如下所示的等式
1/x + 1/y = 1/n
对于n=4，有三组正整数解：
1/5 + 1/20 = 1/4
1/6 + 1/12 = 1/4
1/8 + 1/8 = 1/7
请计算最小的n值使得解的数量超过1000组
注意：本题是第110题的简化版本，强烈建议先解决本题
## 思路
本思路的源网站：[源地址](http://www.mathblog.dk/project-euler-108-diophantine-equation/)
通过等式可得：x y都大于n，所以将等式转换如下：
1 / （n + r） + 1/(n + s) = 1 / n
最终转换可得：
n * n = s * r
问题转变为n * n有多少因数的问题
已知一个数的约数个数有公式如下：
![这里写图片描述](https://img-blog.csdn.net/20160904170513547)[ ](https://img-blog.csdn.net/20160904170513547)
![这里写图片描述](https://img-blog.csdn.net/20160904170634392)[ ](https://img-blog.csdn.net/20160904170634392)
pn为N的质因数
d(N)即为N的因数个数，例如
24 = 2 * 2 * 2  *   3
d(N) = (3 + 1) * (1 + 1) = 8
24的因数为：1,2,3,4,6,8,12,24,共8个
那么对于n*n：
![这里写图片描述](https://img-blog.csdn.net/20160904170853176)
## python程序
```python
def
```
```python
problem108
```
```python
(limit)
```
```python
:
```
```python
#3*3*3*3*3*3*3 = 2187，已经大于2000了，所以质数列表只取到17
```
```python
primelist = [
```
```python
2
```
```python
,
```
```python
3
```
```python
,
```
```python
5
```
```python
,
```
```python
7
```
```python
,
```
```python
11
```
```python
,
```
```python
13
```
```python
,
```
```python
17
```
```python
]
```
```python
#获取输入参数的约数数量
```
```python
def
```
```python
NoDSquared
```
```python
(number)
```
```python
:
```
```python
nod =
```
```python
1
```
```python
;
        exponent =
```
```python
0
```
```python
;
        remain = number;
```
```python
for
```
```python
i
```
```python
in
```
```python
range(
```
```python
0
```
```python
, len(primelist)):
```
```python
if
```
```python
(primelist[i] * primelist[i] > number):
```
```python
return
```
```python
nod *
```
```python
3
```
```python
exponent =
```
```python
1
```
```python
while
```
```python
(remain % primelist[i] ==
```
```python
0
```
```python
):
                exponent +=
```
```python
2
```
```python
remain = remain / primelist[i]
            nod *= exponent
```
```python
# If there is no remainder, return the count
```
```python
if
```
```python
(remain ==
```
```python
1
```
```python
):
```
```python
return
```
```python
nod
```
```python
return
```
```python
nod
    n =
```
```python
1
```
```python
result =
```
```python
0
```
```python
while
```
```python
(
```
```python
1
```
```python
):
```
```python
if
```
```python
((NoDSquared(n) +
```
```python
1
```
```python
) /
```
```python
2
```
```python
> limit -
```
```python
1
```
```python
):
            result = n
```
```python
break
```
```python
n +=
```
```python
1
```
```python
print
```
```python
result
```

