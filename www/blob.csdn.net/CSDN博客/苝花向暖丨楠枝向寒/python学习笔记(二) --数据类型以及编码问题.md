# python学习笔记(二)  --数据类型以及编码问题 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年08月08日 22:14:54[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：68
所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)



## 常用基本数据类型

int 整型

boolean 布尔

string 字符串

list 列表

tuple 元组

## 数据类型是否可变

首先要说明，类似于 a = [1,2,3]   a=[1,2,3,4] 这种都不叫改变其中数据，这叫 给变量a，重新引用另外一个对象，我们必须通过

python提供的api才可以对这个对象做修改

### 不可变数据类型  

int,string,tuple

int 和 string  并没有为我们提供改变其中数据的api

显然下面的做法会改变地址

```
>>> a = 4
>>> id(a)
25702672
>>> a = 5
>>> id(a)
25702648
>>> b = "123"
>>> id(b)
139645340190360
>>> b = "234"
>>> id(b)
139645340191600
```

这里的tuple比较特殊

```
>>> a = (1,2,[3])
>>> id(a)
139645340109552
>>> a[2][0] = 4
>>> print a
(1, 2, [4])
>>> id(a)
139645340109552
```

通过两次的地址查看，可以看出tuple的地址并没有改变，因为我改变的并不是tuple，而是tuple中的第3个位置存放的list。

```
>>> a = (1,2,[3])
>>> a[0] = 4
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: 'tuple' object does not support item assignment
>>>
```

如果改变的是tuple直接会报错

### 可变类型 

list,dict

```
>>> a = [1,2,3]
>>> id(a)
139645340155272
>>> a.append(5)
>>> id(a)
139645340155272
```

```
>>> a = [1,2,3]
>>> id(a)
139645340076368
>>> a[2] = 4
>>> a
[1, 2, 4]
>>> id(a)
139645340076368
```

### 数据类型转换

int() 强转换成int类型

```
>>> a = "123"
>>> a = a+1
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: cannot concatenate 'str' and 'int' objects
>>> a = int(a) + 1
>>> a
124
```

下面这种python中不允许强转和 java一样，不过php是可以的

```
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ValueError: invalid literal for int() with base 10: '123aaaa'
```

bool()   --- 将布尔值结果返回回来，我们可以赋给一个变量，方便后续在程序中使用

python中的  ==  会判断数据类型是否相等，以及 值 是否相等 

```
>>> print 1 == 1
True
>>> print 1 == "1"
False
>>> a = bool(1 == 1)
>>> a
True
```

## 说一下字符串

python的中字符串有点类似于 java中的 char数组，可以通过索引位来获取到指定的字符

```
>>> a = "abcdefg"
>>> a[0]
'a'
>>> a[-1]
'g'
>>> a[-2]
'f'
```

字符串可以用 逗号 来拼接，逗号的位置会被替换为 空格（python3）

```
>>> print('The quick brown fox', 'jumps over', 'the lazy dog')
The quick brown fox jumps over the lazy dog
```

在python2 版本中，逗号并不会被替换为空格

```
>>> print('The quick brown fox', 'jumps over', 'the lazy dog')
('The quick brown fox', 'jumps over', 'the lazy dog')
```

字符串的格式化(拼接)

一 ：通过 %s和%d 来占位

```
>>> a ="my name is %s,%d years old" % ('zzh',22)
>>> a
'my name is zzh,22 years old'
```

```
>>> a = "this is %(whose)s %(fruit)s" % ({'whose':'my','fruit':'apple'})
>>> a
'this is my apple'
```

二：用format还是来拼接字符串，通过{ } 来占位

```
>>> b = "this is {} {}".format("my","apple")   花括号中不写 需按顺序 传入参数
>>> b
'this is my apple'
>>> b = "this is {1} {0}".format("apple","my")  花括号可以指定 对应的位置
>>> print b
this is my apple
>>> b = "this is {whose} {fruit}".format(fruit = "apple",whose = "my") 根据名字传参
>>> b
'this is my apple'
```

## decode()和encode()两个函数

### 情景一：

如果我们想通过len()来获取字符串的长度，且字符串中包含中文。

则需要使用decode()函数，将字符编码转换，才能将正确显示出中文 字数

```
>>> a = '函数'
>>> a
'\xe5\x87\xbd\xe6\x95\xb0'
>>> len(a)
6
>>> b = a.decode('utf-8')
>>> b
u'\u51fd\u6570'
>>> len(b)
2
```

实际上 字符串分为 两种数据类型，一个为str，一个为unicode.我们声明一个字符串，这时候是strstr.decode('utf-8')就是将str转换为Unicode。其中decode()中的参数为str的编码类型，str的编码类型为系统的编码类型，查看系统编码类型可以通过locale来查看。一般ubuntu的就是utf-8。

```
zzh@zzh-VirtualBox:~$ locale
LANG=zh_CN.UTF-8
LANGUAGE=zh_CN:zh
LC_CTYPE="zh_CN.UTF-8"
LC_NUMERIC="zh_CN.UTF-8"
LC_TIME="zh_CN.UTF-8"
LC_COLLATE="zh_CN.UTF-8"
LC_MONETARY="zh_CN.UTF-8"
LC_MESSAGES="zh_CN.UTF-8"
LC_PAPER="zh_CN.UTF-8"
LC_NAME="zh_CN.UTF-8"
LC_ADDRESS="zh_CN.UTF-8"
LC_TELEPHONE="zh_CN.UTF-8"
LC_MEASUREMENT="zh_CN.UTF-8"
LC_IDENTIFICATION="zh_CN.UTF-8"
LC_ALL=
```

### 情景二：

从网站扒取数据，然后打印出来以后乱码。

这是因为页面的编码格式与我们系统的编码格式不一样。

页面扒取下来的数据是一个 str，如果想进行编码格式转换的话，通过decode("网页编码")解码，decode()函数会将指定编码的str，转换成unicode编码。

```
>>> import re,urllib2
>>> html = urllib2.urlopen('http://www.163.com').read()  获取页面
>>> title = re.findall(r'<title>(.*?)</title>',html)[0]  获取title标签内的文本内容
>>> title
'\xcd\xf8\xd2\xd7'    查看编码
>>> print title     输出乱码
���
>>> import chardet
>>> chardet.detect(title)   查看编码格式 
{'confidence': 0.99, 'encoding': 'GB2312'}   该网页编码格式为GB2312

>>> newTitle = title.decode('GB2312') 将GB2312编码的str转换为 unicode编码
>>> print newTitle  输出，解决乱码问题
网易
>>> new_title = newTitle.encode('utf-8')  再转换成utf-8 
>>> print new_title    输出，也正常
网易

分别查看他们的编码格式
>>> chardet.detect(newTitle)
{'confidence': 1.0, 'encoding': 'ascii'}
>>> chardet.detect(new_title)
{'confidence': 0.7525, 'encoding': 'utf-8'}

这时候我的疑惑来了
一：为什么网上都说 decode 是将其他编码转换为unicode编码，而我这里是ascii？
二：为什么中文能够被转换成 ascii编码？
三：为什么打印ascii编码 能够正常输出中文？
目前的理解：
一：输入 type(newTitle) 会返回unicode，我目前只能理解为网上所说的unicode指的是这个字符串类型
二：目前还不能理解
三：网上有这么一句话：调用print来输出 unicode类型的字符串，内部会自动调用encode来对unicode类型的字符串编码，
最终转换为str，而encode中的参数即编码类型是通过 sys.getdefaultencoding来获得的。
但是 我通过这个方法获得的编码 是 ascii，所以也就是说 调用了 encode('ascii')这个方法。
将unicode类型的字符串转换为了str，str的编码格式为ascii，然后输出中文却能正常显示，到这里还是不太能理解。
```

------------------------------------------------------------------------------------------------------------------

经过一天的思考，对上述问题有了新的理解。

今天看到一句话，从磁盘或网页上读取到的数据，读出来的是字节，直接输出这个自己乱码是因为这个字节的编码与我们系统的默认编码不一致。即如果我们直接 用print 去输出 刚从网页读取到的中文，那么print 会去调用系统默认的编码(linux操作系统的默认编码为utf-8)去给这个字节串解码，但是上面我的字节串编码是gb2312，然后系统用utf-8去解码转换成字符显示给我们，编码不一致所以乱码，decode()方法实际的作用的将字节串转换成字符串，所以decode()以后打印该字符串就可以直接解决乱码问题了，而上述案例中，我decode()以后又以 utf-8 来进行编码，然后再输出，系统又会以utf-8来解码，所以也能正常显示。

这仅仅是我个人的理解，如果有人看到这里可以当做一种理解方式，望带着辩证的眼光去看，如有更好的理解方式，希望提出一起探讨。

