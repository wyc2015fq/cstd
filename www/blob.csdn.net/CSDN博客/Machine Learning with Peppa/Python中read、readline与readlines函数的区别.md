# Python中read、readline与readlines函数的区别 - Machine Learning with Peppa - CSDN博客





2018年04月05日 20:56:34[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：231







- read()        一次性读取文本中全部的内容，以字符串的形式返回结果
- readline()    只读取文本第一行的内容，以字符串的形式返回结果
- readlines()   读取整个文件到一个迭代器以供我们遍历（读取到一个list中，以供使用，比较方便）

Python 将文本文件的内容读入可以操作的字符串变量非常容易。文件对象提供了三个“读”方法： .read()、.readline() 和 .readlines()。每种方法可以接受一个变量以限制每次读取的数据量，但它们通常不使用变量。 .read() 每次读取整个文件，它通常用于将文件内容放到一个字符串变量中。然而 .read() 生成文件内容最直接的字符串表示，但对于连续的面向行的处理，它却是不必要的，并且如果文件大于可用内存，则不可能实现这种处理。

一、read方法

read([size])方法从文件当前位置起读取size个字节，若无参数size，则表示读取至文件结束为止，它范围为字符串对象



```python
f = open("a.txt")
lines = f.read()
print lines
print(type(lines))
f.close()
```
输出结果：
```python
Hello
Welcome
What is the fuck...
<type 'str'> #字符串类型
```
二、readline()方法


从字面意思可以看出，该方法每次读出一行内容，所以，读取时占用内存小，比较适合大文件，该方法返回一个字符串对象。

```python
f = open("a.txt")
line = f.readline()
print(type(line))
while line:
 print line,
 line = f.readline()
f.close()
```

```python
输出结果：
<type 'str'>
Hello
Welcome
What is the fuck...
```


三、readlines()方法

读取整个文件所有行，保存在一个列表(list)变量中，每行作为一个元素，但读取大文件会比较占内存。

```python
f = open("a.txt")
lines = f.readlines()
print(type(lines))
for line in lines:
 print line，
f.close()
```

```python
输出结果：
```

```python
<type 'list'>
Hello
Welcome
What is the fuck...
```




