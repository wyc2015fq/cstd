
# Python的递归 - sinat_33731745的博客 - CSDN博客

2018年02月12日 23:56:15[Tao-Tao-Tao](https://me.csdn.net/sinat_33731745)阅读数：66标签：[python																](https://so.csdn.net/so/search/s.do?q=python&t=blog)个人分类：[python																](https://blog.csdn.net/sinat_33731745/article/category/7252259)



```python
def
```
```python
Factorial
```
```python
(n)
```
```python
:
```
```python
if
```
```python
n==
```
```python
1
```
```python
:
```
```python
return
```
```python
1
```
```python
else
```
```python
:
```
```python
return
```
```python
n*Factorial(n-
```
```python
1
```
```python
)
print(Factorial(
```
```python
3
```
```python
))
```
```python
6
```
上述代码用于计算阶乘n!=n∗(n−1)∗⋯∗1n!=n*(n-1)*\cdots*1
当n不为1时，将执行返回n*Factorial(n-1)，而Factorial(n-1),又会去执行Factorial这个函数，即调用他本身。
当n=3n=3时，Factorial(3)Factorial(3)的返回值为3*Factorial(2),而Factorial(2)Factorial(2)的返回值为2*Factorial(1)，Factorial(1)=1Factorial(1)=1,所以Factorial(3)Factorial(3)由递归得到6。


