
# C 语言经典面试题 —— 宏 - Zhang's Wikipedia - CSDN博客


2018年06月01日 21:42:00[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：583



## 1. 运算符优先级与括号
```python
#define Cube(
```
```python
a
```
```python
)
```
```python
a
```
```python
*
```
```python
a
```
```python
*
```
```python
a
```
无法解决Cube(1+1) ⇒ 1+1*1+1*1+1 ⇒ 4，期待的应当是 8，故将其改造为
\#define Cube(a) (a)*(a)*(a)
如此，自身运算的优先级是能解决了，和其他表达式结合时便又存在先运算和后运算的算符优先级的问题，比如其前跟着一个除法运算符：
16/Cube(2)  ⇒ 16 / 2* 2 *2 ⇒ 32，而不是期待的 16 / (2*2*2)
故我们期待的宏定义其实应当是一个整体：
\#define Cube(a) ((a)*(a)*(a))

