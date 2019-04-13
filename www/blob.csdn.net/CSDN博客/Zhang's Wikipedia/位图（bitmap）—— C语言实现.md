
# 位图（bitmap）—— C语言实现 - Zhang's Wikipedia - CSDN博客


2018年07月13日 20:38:50[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：1418


[高级数据结构及应用 —— 使用 bitmap 进行字符串去重](https://blog.csdn.net/lanchunhui/article/details/80554725)
位图应当具备的置一，清零，以及判断三大功能：
```python
#
```
```python
define
```
```python
BITS_PER_WORD 32
```
```python
#
```
```python
define
```
```python
MASK 0x1f
```
```python
#
```
```python
define
```
```python
SHIFT 5
```
```python
// BITS_PER_WORD 与 MASK、SHIFT 是相匹配的，
```
```python
// 如果 BITS_PER_WORD 为 8，则 SHIFT 为 3，MASK 为 0x07
```
```python
// 如果 BITS_PER_WORD 为 64，则 SHIFT 为 6，MASK 为 0x3f
```
```python
// 同样的存储位图的数组的元素类型也要发生相应的改变，BITS_PER_WORD == 8，char
```
```python
// BITS_PER_WORD == 64, ⇒ long long
```
```python
#
```
```python
define
```
```python
N 10000000
```
```python
int
```
```python
a[
```
```python
1
```
```python
+N/BITS_PER_WORD];
```
```python
void
```
```python
set
```
```python
(
```
```python
int
```
```python
i) {
    a[i >> SHIFT] != (
```
```python
1
```
```python
<< (i & MASK));
}
```
```python
// a[i >> SHIFT] ⇒ 返回的是int整型，也是长度为 32 的 bit 比特串；
```
```python
void
```
```python
clr(
```
```python
int
```
```python
i) {
    a[i >> SHIFT] &= ~(
```
```python
1
```
```python
<< (i & MASK));
}
```
```python
int
```
```python
test(
```
```python
int
```
```python
i) {
```
```python
return
```
```python
a[i >> SHIFT] & (
```
```python
1
```
```python
<< (i & MASK));
}
```
## 1. 排序以及去重
```python
int
```
```python
main(
```
```python
void
```
```python
) {
```
```python
int
```
```python
i;
```
```python
for
```
```python
(i =
```
```python
0
```
```python
; i < N; ++i)
        clr(i);
```
```python
while
```
```python
(
```
```python
scanf
```
```python
(
```
```python
"%d"
```
```python
, &i) != EOF)
```
```python
set
```
```python
(i);
```
```python
for
```
```python
(i =
```
```python
0
```
```python
; i < N; ++i) {
```
```python
// 自然是有序，且不重复的
```
```python
if
```
```python
(test(i)) {
```
```python
printf
```
```python
(
```
```python
"%d "
```
```python
, i);   
        }
    }
```
```python
printf
```
```python
(
```
```python
"\n"
```
```python
);
}
```
## 2. 问题与思考
序列中出现负数怎么办；

