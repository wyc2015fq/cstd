
# scala for循环 - bitcarmanlee的博客 - CSDN博客


2017年09月19日 16:29:14[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：465



## 1.与yield配合生成一个新的序列
for循环与yield配合，可以生成一个新的序列。看个简单的例子：
```python
val
```
```python
res =
```
```python
for
```
```python
(i <-
```
```python
1
```
```python
to
```
```python
8
```
```python
if
```
```python
i%
```
```python
2
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
yield
```
```python
i
println(res)
```
最后的输出结果为：
```python
Vector(
```
```python
2
```
```python
,
```
```python
4
```
```python
,
```
```python
6
```
```python
,
```
```python
8
```
```python
)
```
## 2.生成一个递减的序列
如果我们想要实现`int i=10;i>=0;i--`的功能，可以这样：
```python
for
```
```python
(i <-
```
```python
10
```
```python
to
```
```python
0
```
```python
by
```
```python
-
```
```python
1
```
```python
) print(i +
```
```python
" "
```
```python
)
```
注意如果没有后面的`by -1`，只是写成这样：
```python
for
```
```python
(i <-
```
```python
10
```
```python
to
```
```python
0
```
```python
)
```
```python
print
```
```python
(i +
```
```python
" "
```
```python
)
```
是没法输出正确结果的。

