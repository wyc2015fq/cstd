
# 【一题多解】Python 字符串逆序 - Zhang's Wikipedia - CSDN博客


2018年07月15日 22:32:07[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：1179


[https://blog.csdn.net/seetheworld518/article/details/46756639](https://blog.csdn.net/seetheworld518/article/details/46756639)
[https://blog.csdn.net/together_cz/article/details/76222558](https://blog.csdn.net/together_cz/article/details/76222558)
## 1. 使用索引
```python
>> strA =
```
```python
'abcdefg'
```
```python
>> strA[
```
```python
:
```
```python
:-
```
```python
1
```
```python
]
```
```python
'gfedcba'
```
## 2. 使用 list 的 reverse 方法
```python
>> l = [c
```
```python
for
```
```python
c
```
```python
in
```
```python
strA]
>> l.
```
```python
reverse
```
```python
()
>>
```
```python
''
```
```python
.
```
```python
join
```
```python
(l)
```
```python
'gfedcba'
```
## 3. 使用 python 原生函数：reversed
```python
>>
```
```python
''
```
```python
.
```
```python
join
```
```python
(c
```
```python
for
```
```python
c
```
```python
in
```
```python
reversed(strA))
```
```python
'gfedcba'
```

