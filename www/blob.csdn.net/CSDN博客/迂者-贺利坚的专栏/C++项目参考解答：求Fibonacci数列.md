
# C++项目参考解答：求Fibonacci数列 - 迂者-贺利坚的专栏 - CSDN博客

2016年03月08日 11:17:05[迂者-贺利坚](https://me.csdn.net/sxhelijian)阅读数：2189



## 【项目：求Fibonacci数列】
Fibonacci数列在计算科学、经济学等领域中广泛使用，其特点是：第一、二个数是1，从第3个数开始，每个数是其前两个数之和。据此，这个数列为：1 1 2 3 5 8 13 21 34 55 89 ……，请设计程序，输出这个数列，直到这个数字超过10000。
【提示】数列可以表示为：

$$
\begin{cases}
f_1=f_2=1 \\
f_n=f_{n-1}+f_{n-2}, n>2
\end{cases}
$$

## 【参考解答】
```python
#include <iostream>
```
```python
using
```
```python
namespace
```
```python
std
```
```python
;
```
```python
int
```
```python
main( )
{
```
```python
int
```
```python
f1,f2,fn,n;
    f1=f2=
```
```python
1
```
```python
;
    n=
```
```python
2
```
```python
;
```
```python
cout
```
```python
<<f1<<
```
```python
'\t'
```
```python
<<f2<<
```
```python
'\t'
```
```python
;
    fn=f1+f2;
```
```python
while
```
```python
(fn<
```
```python
10000
```
```python
)
    {
```
```python
cout
```
```python
<<fn<<
```
```python
'\t'
```
```python
;
        n++;
```
```python
if
```
```python
(n%
```
```python
5
```
```python
==
```
```python
0
```
```python
)
```
```python
cout
```
```python
<<endl;
        f1=f2;
        f2=fn;
        fn=f1+f2;
    }
```
```python
return
```
```python
0
```
```python
;
}
```

