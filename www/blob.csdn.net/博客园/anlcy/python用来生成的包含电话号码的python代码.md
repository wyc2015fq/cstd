
# python用来生成的包含电话号码的python代码 - anlcy - 博客园






# [python用来生成的包含电话号码的python代码](https://www.cnblogs.com/camilla/p/7161019.html)
\# -*- coding:utf-8 -*-
\#用python生产包含电话号码的代码
temp = """arr = %s
index = %s
tel = ''
for i in index:
tel += arr[i]
print("电话号码" + tel)
"""
def encode(tel):
arr = list(set(list(tel)))
index = [arr.index(r) for r in list(tel)]
return temp % (arr,index)
code=encode("186XXXXXXXX")
print(code)
\#因为code字符串是一段合法的python代码，用exec执行后可以电话号码解出来。
exec(code)
运行结果：
```python
arr = ['X', '6', '8', '1']
```

```python
index = [3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0]
```

```python
tel = ''
```

```python
for i in index:
```

```python
tel += arr[i]
```

```python
print("电话号码" + tel)
```

```python
电话号码186XXXXXXXX
```





