
# n 中选 m —— 随机采样的艺术 - Zhang's Wikipedia - CSDN博客


2018年07月13日 21:17:54[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：1260


0,1,2…n-1，n 个数中随机取 m 个数，要求 0, 1, n-1，此 n 个数每个数被取到的概率相同：
Knuth 书中的随机化方法，很容易写出：
```python
void
```
```python
genkunth (
```
```python
int
```
```python
n,
```
```python
int
```
```python
m) {
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
; i < n; ++i) {
```
```python
if
```
```python
(bigrand() % (n-i) < m) {
            m--;
```
```python
cout
```
```python
<< i << endl;
        }
    }
}
```
该算法的特点分析如下：
当 n == m 时，if 判断式恒成立，输出的结果也恒定为 0, 1, 2, … n-1;
n-i 每次循环一定发生，m– 未必发生；则 n-i 一定小于 m，对 n-i 取模也必然小于 m；
当 n > m 时，最坏的情况，前 n-m 次随机都不满足条件（if 均不成立），第 n-m+1 次随机必然成立；
故一定可以输出 m 个随机数，
当 n == m, 输出为 0, 1, 2, … n-1
当 n > m, 输出 m 个有序的介于 0-n-1 之间的数；


