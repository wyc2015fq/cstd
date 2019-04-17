# 文件操作与数据存储json - 博客堂 - CSDN博客





2017年05月01日 17:10:47[最小森林](https://me.csdn.net/u012052268)阅读数：2433








# 5 文件操作与数据存储json


- [文件操作与数据存储json](#5-文件操作与数据存储json)- [文件读取](#文件读取)- [键盘读取](#键盘读取)
- [从本地读取](#从本地读取)- [打开和关闭文件](#打开和关闭文件)


- [文件写入](#文件写入)
- [数据存储json](#数据存储json)- [jsondumps 与 jsonloads 实例](#jsondumps-与-jsonloads-实例)





## 文件读取

### 键盘读取

Python2中有两个内置的函数可从标准输入读取数据，它默认来自键盘。这些函数分别是：input() 和 raw_input()。

但在Python3中，raw_input()函数已被**弃用**。此外， input() 函数是从键盘作为**字符串**读取数据，不论是否使用引号(”或“”)与否。

```python
#!/usr/bin/python3

>>> x=input("something:")
something:10
>>> x
'10'
>>> x=input("something:")
something:'10' #entered data treated as string with or without ''
>>> x
"'10'"
```

### 从本地读取

#### 打开和关闭文件

**open（）函数**注意：
- 文件被打开后一定得记得关闭close（）。否则可能会损害文件。所以尽量使用 **with** 。让python自己判断什么时候该关闭，并自己去关闭。
- open()后是一个对象，这个对象有read()方法与write()方法。
- 常用打开模式：

```
r 只能读 
r+ 可读可写，不会创建不存在的文件，从顶部开始写，会覆盖之前此位置的内容
w 只能写，覆盖整个原有文件（不要乱用），不存在则创建 
w+ 可读可写，如果文件存在，则覆盖整个文件，不存在则创建
a 只能写，从文件底部添加内容 不存在则创建 
a+ 可读可写 从文件顶部读取内容 从文件底部添加内容 不存在则创建
```

如果我们在open文件后，没有进行任何读写，则默认r模式

```
with open ('pi.txt') as file_object :
    contents = file_object.read()
    print( contents )
```

逐行读取：

```
filename = 'pi.txt'
with open (filename) as file_object :
    for line in file_object:
        print (line)
```

## 文件写入

w模式：覆盖原有数据

```
filename = 'pi.txt'
with open (filename,'w') as file_object :
    file_object.write("i love you \n")
    file_object.write("i love you \n") # 加上\n可以一行一行的加入
```

a模式：添加新的内容，不会覆盖原有内容

```
filename = 'pi.txt'
with open (filename,'a') as file_object :
    file_object.write("i love you \n")
    file_object.write("i love you \n") # 加上\n可以一行一行的加入
```

## 数据存储json

JSON (JavaScript Object Notation) 是一种轻量级的数据交换格式。它基于ECMAScript的一个子集。

Python3 中可以使用 json 模块来对 JSON 数据进行编解码，它包含了两个函数：

```
json.dumps(): 对数据进行编码。
json.loads(): 对数据进行解码。
```

在json的编解码过程中，python 的原始类型与json类型会相互转换，具体的转化对照如下：
|Python|JSON|
|----|----|
|dict|object|
|list, tuple|array|
|str|string|
|int, float, int- & float-derived Enums|number|
|True|true|
|False|false|
|None|null|

### json.dumps 与 json.loads 实例

以下实例演示了 Python 数据结构转换为JSON：

```
#!/usr/bin/python3

import json

# Python 字典类型转换为 JSON 对象
data = {
    'no' : 1,
    'name' : 'Runoob',
    'url' : 'http://www.runoob.com'
}

json_str = json.dumps(data)
print ("Python 原始数据：", repr(data))
print ("JSON 对象：", json_str)
```

执行以上代码输出结果为：

```java
Python 原始数据： {'url': 'http://www.runoob.com', 'no': 1, 'name': 'Runoob'}
JSON 对象： {"url": "http://www.runoob.com", "no": 1, "name": "Runoob"}
```

通过输出的结果可以看出，简单类型通过编码后跟其原始的repr()输出结果非常相似。 

接着以上实例，我们可以将一个JSON编码的字符串转换回一个Python数据结构：

```python
#!/usr/bin/python3

import json

# Python 字典类型转换为 JSON 对象
data1 = {
    'no' : 1,
    'name' : 'Runoob',
    'url' : 'http://www.runoob.com'
}

json_str = json.dumps(data1)
print ("Python 原始数据：", repr(data1))
print ("JSON 对象：", json_str)

# 将 JSON 对象转换为 Python 字典
data2 = json.loads(json_str)
print ("data2['name']: ", data2['name'])
print ("data2['url']: ", data2['url'])
```

执行以上代码输出结果为：

```java
Python 原始数据： {'name': 'Runoob', 'no': 1, 'url': 'http://www.runoob.com'}
JSON 对象： {"name": "Runoob", "no": 1, "url": "http://www.runoob.com"}
data2['name']:  Runoob
data2['url']:  http://www.runoob.com
```

如果你要处理的是文件而不是字符串，你可以使用 json.dump() 和 json.load() 来编码和解码JSON数据。例如：

```
# 写入 JSON 数据
with open('data.json', 'w') as f:
    json.dump(data, f)

# 读取数据
with open('data.json', 'r') as f:
    data = json.load(f)
```



