# Python 数据类型及基本操作 - Alex_McAvoy的博客 - CSDN博客





2018年07月23日 21:58:54[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：365








# 【概述】

不同的数据，需要定义不同的数据类型。

Python 定义了五个标准类型，用于存储各种类型的数据：
- Numbers（数字）
- String（字符串）
- List（列表）
- Tuple（元组）
- Dictionary（字典）

# 【数字】

数字数据类型用于存储数值，它们是不可改变的数据类型，这意味着改变数字数据类型会分配一个新的对象。

当指定一个值时，数字对象就会被创建：

```python
var1 = 1
var2 = 10
```

除创建外，也可以使用 del 语句删除一些对象的引用：**del var1[,var2[,var3[....,varN]]]]**

```python
del var
del var_a, var_b
```

Python支持四种不同的数字类型：
- 整型(Int)：通常被称为是整型或整数，是正或负整数，不带小数点。
- 长整型(long integers)：无限大小的整数，整数最后是一个大写或小写的 L。
- 浮点型(floating point real values)：浮点型由整数部分与小数部分组成，浮点型也可以使用科学计数法表示（2.5e2 = 2.5 x 10^2 = 250）
- 复数(complex numbers) ：复数由实数部分和虚数部分构成，可以用 a + bj，或 complex(a,b)表示， 复数的实部 a 和虚部 b 都是浮点型。

实例
|int|long|float|complex|
|----|----|----|----|
|10|51924361L|0.0|3.14j|
|100|-0x19323L|15.20|45.j|
|-786|0122L|-21.9|9.322e-36j|
|080|0xDEFABCECBDAECBFBAEl|32.3e+18|.876j|
|-0490|535633629843L|-90.|-.6545+0J|
|-0x260|-052318172735L|-32.54e100|3e+26J|
|0x69|-4721885298529L|70.2E-12|4.53e-7j|

**注：为避免与数字 1 混淆，一般在长整型结尾使用大写的 L 来显示长整型。**

# 【字符串】

字符串是由数字、字母、下划线组成的一串字符，一般记为：**s="a1a2···an"(n>=0)**，它是编程语言中表示文本的数据类型。

## 1.访问

python 的字串列表有2种取值顺序：
- 从左到右索引默认0开始的，最大范围是：字符串长度-1
- 从右到左索引默认-1开始的，最大范围是：字符串开头

例如：

![](https://img-blog.csdn.net/20180723163052192?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如果要从字符串中获取一段子字符串的话，可以使用** [头下标:尾下标] **来截取相应的字符串，获取的子字符串包含头下标的字符，但不包含尾下标的字符。其中下标是从 0 开始算起，可以是正数或负数，也可以为空，表示取到头或尾。

实例：

```python
str='Hello World!'

print(str)
print(str[0])
print(str[2:5])
print(str[2:])
```

![](https://img-blog.csdn.net/20180723163954994?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 2.字符串转义字符

在需要在字符中使用特殊字符时，python 用反斜杠 \ 来转义字符。
|\(在行尾时)|续行符|
|----|----|
|\\|反斜杠|
|\'|单引号|
|\"|双引号|
|\b|退格|
|\a|响铃|
|\e|转义|
|\000|空|
|\n|换行|
|\v|纵向制表符|
|\t|横向制表符|
|\r|回车|
|\f|换页|
|\oyy|八进制数，yy代表的字符，例如：\o12代表换行|
|\xyy|十六进制数，yy代表的字符，例如：\x0a代表换行|
|\other|其它的字符以普通格式输出|

## 3.三引号的用法

python 中，三引号允许一个字符串跨多行，字符串中可以包含换行符、制表符以及其他特殊字符，其语法是一对连续的单引号或者双引号，成对使用。

三引号让程序员从引号和特殊字符串的泥潭里面解脱出来，自始至终保持一小块字符串的格式是所谓的 WYSIWYG（所见即所得）格式的。其典例是，当需要一块 HTML 或者 SQL 时，这时用字符串组合，特殊字符串转义将会非常的繁琐，使用三引号可以摆脱转义字符的泥潭。

例如：

```python
str='''Hello
World!'''

print(str)
```

![](https://img-blog.csdn.net/2018072317270786?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 【列表】

序列是 Python 中使用最频繁的数据结构，其数据项不需要具有相同的类型，字符，数字，字符串、包含列表（即嵌套）均可，使用 [ ] 标识，可以进行索引，切片，加，乘，检查成员等操作。

创建一个列表，只要把逗号分隔的不同的数据项使用方括号括起来即可。

```python
list1 = ['physics', 'chemistry', 1997, 2000]
list2 = [1, 2, 3, 4, 5 ]
list3 = ["a", "b", "c", "d"]
```

## 1.访问

列表中值的切割也使用：**变量 [头下标:尾下标]** 的方式，与字符串相同，从左到右索引默认 0 开始，从右到左索引默认 -1 开始，下标可以为空表示取到头或尾。
![](https://img-blog.csdn.net/20180723174928307?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 2.添加元素

如果想添加列表元素，可以使用 append() 方法来添加。

```python
list = ['Hello']

list.append('World')
list.append('!')

print(list)
```

## 3.删除元素

如果想删除列表元素，可以使用 del 语句来删除。

```python
list = ['Hello','World','!']
print(list)#删除前

del list[2]
print(list)#删除后
```

![](https://img-blog.csdn.net/2018072318083925?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 【元组】

元组类似于列表，不同之处在于元组不能二次赋值，相当于只读列表。

元组用 () 标识，内部元素用逗号隔开，如果想创建元组，只需要在括号中添加元素，并用逗号隔开即可。

## 1.访问

元组中值的切割也使用：**变量 [头下标:尾下标]** 的方式，与字符串相同，从左到右索引默认 0 开始，从右到左索引默认 -1 开始，下标可以为空表示取到头或尾。

```python
tup1 = ('physics', 'chemistry', 1997, 2000)
tup2 = (1, 2, 3, 4, 5, 6, 7 )
 
print ( "tup1[0]: ", tup1[0] )
print ( "tup2[1:5]: ", tup2[1:5] )
```

![](https://img-blog.csdn.net/20180723205116133?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 2.连接组合

元组中的元素值是不允许修改的，但可以用 + 加号来对元组进行连接组合。

```python
tup1 = (12, 34.56)
tup2 = ('abc', 'xyz')
 
# 创建一个新的元组
tup3 = tup1 + tup2
print (tup3)
```

## 3.删除元组

元组中的元素值是不允许删除的，但可以使用 del 语句来删除整个元组。

```python
tup = ('physics', 'chemistry', 1997, 2000)

del tup
```

# 【字典】

字典是除列表以外最灵活的内置数据结构类型。

列表是有序的对象集合，字典是无序的对象集合。两者之间的区别在于：字典当中的元素是通过键来存取的，而不是通过偏移存取。

字典由索引（key）和它对应的值（value）组成，称为键值对（key=>value），每对键值之间用冒号 : 分割，每个键值对之间用逗号 , 分割，整个字典包括在花括号 {} 中 ,格式为：**dir = {key1 : value1, key2 : value2 , ... ,keyN : valueN}**

键是唯一的，因此只可用数字、字符串或元组来充当，值可以取任何数据类型的任何值，如果重复一个键值对则会替换前面的键值对。

```python
dict1 = { 'abc': 456 }
dict2 = { 'abc': 123, 98.6: 37 }
```

## 1.访问

将想要访问的键放入方括号 [] 内即可得到该键所对应的值。

```python
dict = {'Name': 'Zara', 'Age': 7, 'Class': 'First'};
 
print ("dict['Name']: ", dict['Name'])
print ("dict['Age']: ", dict['Age'])
```

![](https://img-blog.csdn.net/20180723214431514?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 2.添加键值对

如果想向字典中添加新的键值对，直接将想要添加的键放到方括号 [] 中，以赋值的形式添加值即可。

```python
dict = {'Name': 'Zara', 'Age': 7, 'Class': 'First'};
 
dict['School'] = "DPS School";
```

## 3.修改键值

如果想修改字典中某一键所对应的值，直接将想要修改的键放到方括号 [] 中，以赋值的形式修改值即可。

```python
dict = {'Name': 'Zara', 'Age': 7, 'Class': 'First'};
 
dict['Age'] = 8;
```

## 4.删除键值对

如果想要删除某一条目，使用 del 语句将想要删除的键值对的键放到方括号 [] 中即可。

```python
dict = {'Name': 'Zara', 'Age': 7, 'Class': 'First'}
 
del dict['Name'] # 删除键是'Name'的条目
```

## 5.清空字典

如果想要清空字典条目，使用其内置函数 dir.clear() 即可。

```python
dict = {'Name': 'Zara', 'Age': 7, 'Class': 'First'}
 
dict.clear()
```

## 6.删除字典

使用 del 语句可以直接删除一个字典。

```python
dict = {'Name': 'Zara', 'Age': 7, 'Class': 'First'}
 
del dict
```

# 【集合】

集合是一个无序不重复元素的序列，创建集合时，如果集合内存在相同的元素，则只取一个，其他相同的元素视为不存在。

可以使用大括号 {} 来创建集合，格式：setname = {value01,value02,...}

或者 set() 函数来创建集合，格式：setname=set(value)

```python
s={"Google", "Runoob", "Taobao"}
s=set(("Google", "Runoob", "Taobao"))
```

## 1.添加元素

将元素 x 添加到集合 s 中，如果元素已存在，则不进行任何操作，语法格式：s.add(x)

```python
s=set(("Google", "Baidu", "Taobao"))
s.add("Tencent")
```

注：add() 只可添加一个元素

若想添加多个元素到集合 s 中，可以使用 update()，其参数可以是列表、元组、字典等，语法格式：s.update(x)

x 可以有多个，用逗号分隔

```python
s=set(("Google", "Baidu", "Taobao"))
s.update(("Tencent","Facebook"))
```

## 2.移除元素

将元素 x 添加到集合 s 中移除，如果元素不存在，则会发生错误，语法格式：s.remove(x)

```python
s=set(("Google", "Baidu", "Taobao"))
s.remove("Baidu")
s.remove("Tencent")#不存在会发生错误
```

除 remove() 外，还可以使用 discard() 来移除元素，且如果元素不存在，不会发生错误，语法格式：s.discard(x)

```python
s=set(("Google", "Baidu", "Taobao"))
s.discard("Baidu")
s.discard("Tencent")#不存在不会发生错误
```

也可以设置随机删除集合 s 中的一个元素，语法格式：s.pop() 

```python
s=set(("Google", "Baidu", "Taobao"))
s.pop()
```

## 3.计算集合元素个数

可用 len() 来计算集合 s 中的元素个数，值得注意的是，若集合中有相同的元素，则所有相同的元素仅视为一个元素。语法格式：len(s)

```python
s=set(("Google", "Baidu", "Taobao"))
length=s.len()
```

## 4.清空集合

可用 clear() 来清空集合 s 中的所有元素，语法格式：s.clear()

```python
s=set(("Google", "Baidu", "Taobao"))
s.clear()
```



