
# scala字符串变量替换 - bitcarmanlee的博客 - CSDN博客


2017年05月23日 20:34:46[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：1135


在scala中，字符串可以带s,f,raw前缀。这几个前缀都可以用来进行变量替换。下面来简单分析实验一下。
## 1.s前缀
s前缀的作用就是用来表示变量替换。
```python
def
```
```python
test() = {
```
```python
val
```
```python
word =
```
```python
"hello"
```
```python
println(s
```
```python
"$word, world"
```
```python
)
    }
```
函数运行的结果为：
```python
hello, world
```
## 2.f前缀
f前缀在表示变量替换的同时，还可以在变量名后面添加格式化参数。
```python
def
```
```python
test() = {
```
```python
val
```
```python
year1 =
```
```python
2017
```
```python
println(f
```
```python
"hello, $year1%.2f"
```
```python
)
```
```python
val
```
```python
year2 =
```
```python
"2017"
```
```python
println(f
```
```python
"hello, $year2%.3s"
```
```python
)
    }
```
输出的结果为：
```python
hello, 2017.00
hello, 201
```
## 3.raw前缀
raw前缀在表示变量替换的同事，不对特殊字符转义。
```python
def
```
```python
t6() = {
```
```python
val
```
```python
year =
```
```python
2017
```
```python
println(raw
```
```python
"hello\t$year"
```
```python
)
    }
```
输出结果为：
```python
hello
```
```python
\t
```
```python
2017
```

