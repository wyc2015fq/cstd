
# Java Tricks —— 不小于一个数的最小2的幂次方 - Zhang's Wikipedia - CSDN博客


2018年09月06日 21:35:35[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：404


不小于一个数的最小2的幂次方，对于 10 就是 16，对于 21 就是 32.
以下实现摘自 java HashMap 的源码：
```python
static
```
```python
final
```
```python
int
```
```python
tableSizeFor(
```
```python
int
```
```python
cap) {
```
```python
int
```
```python
n = cap -
```
```python
1
```
```python
;
    n |= n >>>
```
```python
1
```
```python
;
    n |= n >>>
```
```python
2
```
```python
;
    n |= n >>>
```
```python
4
```
```python
;
    n |= n >>>
```
```python
8
```
```python
;
    n |= n >>>
```
```python
16
```
```python
;
```
```python
return
```
```python
(n <
```
```python
0
```
```python
) ?
```
```python
1
```
```python
: (n >= MAXIMUM_CAPACITY) ? MAXIMUM_CAPACITY : n +
```
```python
1
```
```python
;
}
```
>>>：无符号右移；
给定的cap减1，是为了避免参数cap本来就是2的幂次方，这样一来，经过后续的未操作的，cap将会变成2 * cap,是不符合我们预期的。
n >>> 1 使得 n 的最高位为0；
n |= n>>>1，则 n 的前两位为1；
n |= n >>> 2：n 的前 4 位均为 1；
n |= n >>> 4：n 的前 8 为均为 1；
n |= n >>> 8：n 的前 16 为均为 1；
n |= n >>> 16：n 的前 32 为均为 1；
如果入参为 20：
intn =cap-1;// 19, 10011n |= n >>>1;// 21, 11011n |= n >>>2;// 31, 11111n |= n >>>4;// 31, 11111n |= n >>>8;// 31, 11111n |= n >>>16;// 31, 11111n+1// 32 = 2^5
---
[源码分析之 HashMap](https://juejin.im/post/58f2f47061ff4b0058f4b7cc)

