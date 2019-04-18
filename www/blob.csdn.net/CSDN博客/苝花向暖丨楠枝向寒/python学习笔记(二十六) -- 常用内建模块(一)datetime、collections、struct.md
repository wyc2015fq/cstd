# python学习笔记(二十六) -- 常用内建模块(一)datetime、collections、struct - 苝花向暖丨楠枝向寒 - CSDN博客

2018年09月04日 23:55:40[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：53标签：[datetime																[collections																[struct](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=collections&t=blog)](https://so.csdn.net/so/search/s.do?q=datetime&t=blog)
个人分类：[python基础](https://blog.csdn.net/weixin_40247263/article/category/7905597)

所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)


**目录**

[datetime](#main-toc)

[datetime、timestamp相互转换](#datetime%E3%80%81timestamp%E7%9B%B8%E4%BA%92%E8%BD%AC%E6%8D%A2)

[datetime、str相互转换](#datetime%E3%80%81str%E7%9B%B8%E4%BA%92%E8%BD%AC%E6%8D%A2)

[datetime加减](#datetime%E5%8A%A0%E5%87%8F%C2%A0)

[datetime时区，以及时区运算](#datetime%E6%97%B6%E5%8C%BA%EF%BC%8C%E4%BB%A5%E5%8F%8A%E6%97%B6%E5%8C%BA%E8%BF%90%E7%AE%97)

[collections](#collections)

[namedtuple    -- 优化tuple](#namedtuple%C2%A0%20%C2%A0%20--%20%E4%BC%98%E5%8C%96tuple)

[deque    -- 优化list](#deque%C2%A0%20%C2%A0%20--%C2%A0%E4%BC%98%E5%8C%96list)

[defaultdict   --  优化dict](#defaultdict%C2%A0%20%C2%A0--%C2%A0%20%E4%BC%98%E5%8C%96dict)

[OrderedDict --  优化dict](#OrderedDict%20--%C2%A0%20%E4%BC%98%E5%8C%96dict)

[Counter  -- 计数器](#Counter%C2%A0%20--%20%E8%AE%A1%E6%95%B0%E5%99%A8)

[struct](#struct)

[使用方式](#%E4%BD%BF%E7%94%A8%E6%96%B9%E5%BC%8F)

[struct模块中的函数](#struct%E6%A8%A1%E5%9D%97%E4%B8%AD%E7%9A%84%E5%87%BD%E6%95%B0)

[格式化字符串](#%E6%A0%BC%E5%BC%8F%E5%8C%96%E5%AD%97%E7%AC%A6%E4%B8%B2)

[对齐方式](#%E5%AF%B9%E9%BD%90%E6%96%B9%E5%BC%8F)

[格式符](#%E6%A0%BC%E5%BC%8F%E7%AC%A6)

# datetime

### datetime、timestamp相互转换

datetime是一个表示当前系统时间的对象，与时区相关，比如同一时间在中国和在美国电脑上获取的当前时间是不一样的。

timestamp是一个float类型的对象，它代表当前系统时间 与 1970年1月1日 00:00:00 UTC+0:00 相差的秒数。

UTC+0:00 代表国际标准的时区，比如中国就是 UTC+8:00

1970年1月1日 00:00:00 UTC+0:00   与  1970年1月1日 08:00:00 UTC+8:00 实际上是等价的

```python
>>> from datetime import datetime   # 从datetime模块导入datetime类
>>> date = datetime(2018, 9, 4, 0, 0)  # 创建一个datetime对象，参数分别为年月日时分
>>> print(date)
2018-09-04 00:00:00
>>> time = date.timestamp() # 将datetime转换为 timestamp
>>> print(time)    # timestamp为当前时间减去1970-1-1 00:00:00 的秒数
1535990400.0  
>>> type(time) # time 为float类型，小数点后面代表毫秒
<class 'float'> 
>>> new_date = datetime.fromtimestamp(time) # 将timestamp转为datetime，当地时间
>>> print(new_date)
2018-09-04 00:00:00
>>> utc_new_date = datetime.utcfromtimestamp(time) # 将timestamp转为datetime,UTC标准时间
>>> print(utc_new_date)
2018-09-03 16:00:00
```

### datetime、str相互转换

```python
# 字符串转时间
>>> cday = datetime.strptime('2018-9-4 10:24:59','%Y-%m-%d %H:%M:%S') # 参数1字符串，参数2字符串对应的日期位置
>>> print(cday)
2018-09-04 10:24:59
# 时间转字符串
>>> print(cday.strftime('%a,%b %d %H:%M')) # 这里类似于re.match().group()通过正则提取字符
Tue,Sep 04 10:24
```

日期格式化符号总结:

```
%a 星期几的简写;如 星期三为Web 
%A 星期几的全称;如 星期三为Wednesday 
%b 月份的简写; 如4月份为Apr 
%B 月份的全称; 如4月份为April 
%c 标准的日期的时间串;（如： 04/07/10 10:43:39） 
%C 年份的后两位数字 
%d 十进制表示的每月的第几天 
%D 月/天/年 
%e 在两字符域中，十进制表示的每月的第几天 
%F 年-月-日 
%g 年份的后两位数字，使用基于周的年 
%G 年分，使用基于周的年 
%h 简写的月份名 
%H 24小时制的小时 
%I 12小时制的小时 
%j 十进制表示的每年的第几天 
%m 十进制表示的月份 
%M 十时制表示的分钟数 
%n 新行符 
%p 本地的AM或PM的等价显示 
%r 12小时的时间 
%R 显示小时和分钟：hh:mm 
%S 十进制的秒数 
%t 水平制表符 
%T 显示时分秒：hh:mm:ss 
%u 每周的第几天，星期一为第一天 （值从0到6，星期一为0） 
%U 第年的第几周，把星期日做为第一天（值从0到53） 
%V 每年的第几周，使用基于周的年 
%w 十进制表示的星期几（值从0到6，星期天为0） 
%W 每年的第几周，把星期一做为第一天（值从0到53） 
%x 标准的日期串 
%X 标准的时间串 
%y 不带世纪的十进制年份（值从0到99） 
%Y 带世纪部分的十制年份 
%z，%Z 时区名称，如果不能得到时区名称则返回空字符。 
%% 百分号
```

### datetime加减 

```python
from datetime import datetime, timedelta
>>> now = datetime.now()
>>> now
datetime.datetime(2018, 9, 4, 15, 24, 33, 395521) 
>>> now + timedelta(hours=10)  # 十天以后
datetime.datetime(2018, 9, 5, 1, 24, 33, 395521)
>>> now - timedelta(days=1)  # 前一天
datetime.datetime(2018, 9, 3, 15, 24, 33, 395521)
>>> now + timedelta(days=2, hours=12)  #  2天10小时之后
datetime.datetime(2018, 9, 7, 3, 24, 33, 395521)
>>> now + timedelta(weeks = 1)  #  一周以后
datetime.datetime(2018, 9, 11, 15, 24, 33, 395521)
```

### datetime时区，以及时区运算

```python
# utcnow() 可以获取到 UTC 时间 
# datetime有个时区属性（tzinfo），默认为None，但我们可以通过replace函数对其进行赋值。
# tzinfo 需要一个 timezone对象，timezone.utc代表 UTC时区对象。
>>> utc_dt = datetime.utcnow().replace(tzinfo = timezone.utc)
>>> print(utc_dt)
2018-09-04 07:57:54.835855+00:00
# 通过 astimezone函数，可以将datetime对象的时区进行变换
>>> bj_dt = utc_dt.astimezone(timezone(timedelta(hours=8))) 
>>> print(bj_dt)
2018-09-04 15:57:54.835855+08:00
>>> tokyo_dt = utc_dt.astimezone(timezone(timedelta(hours=9)))
>>> print(tokyo_dt)
2018-09-04 16:57:54.835855+09:00
>>> tokyo_dt = bj_dt.astimezone(timezone(timedelta(hours=9)))
>>> print(tokyo_dt)
2018-09-04 16:57:54.835855+09:00
```

# collections

### namedtuple    -- 优化tuple

对元组的封装，使代码所代表的含义更加清晰，同时提供多种取值方式。

如 (1,2) 表示 坐标，你这么写，看你代码的人多半不会懂。

如果  写一个类 来表示 坐标点那又小题大作了。

namedtuple为我们解决了这个问题，它会为我们返回一个自定义的tuple子类，我们可以规定tuple里面元素的个数，以及为这个tuple命名。

使用方式如下：

```python
>>> from collections import namedtuple 
#  参数1，给tuple命名，使代码阅读起来更清晰
#  参数2，一个列表，规定了 tuple里元素的个数  
>>> Point = namedtuple('Point',['x','y'])   # 返回的Point是 tuple的一种子类
>>> isinstance(p, Point)
True
>>> isinstance(p, tuple)
True
>>> p = Point(1, 2) # 传值
>>> p.x                                           
1
>>> p.y
2
```

### deque    -- 优化list

deque是对list的优化，list单向存储数据，所以根据索引来查询数据很快。

但是数据量大的时候，插入和删除效率很低。

deque是为了高效实现插入和删除操作的双向列表，适合用于队列和栈：

使用如下：

```python
>>> q = deque(['a', 'b', 'c'])
>>> q.append('x')   # 在末尾添加一个元素
>>> q.appendleft('y')  # 在开头添加一个元素
>>> q
deque(['y', 'a', 'b', 'c', 'x'])
>>> q.pop()   # 从末尾删除一个元素 
'x'
>>> q.popleft()  # 从开头删除一个元素
'y'
>>> q
deque(['a', 'b', 'c'])
```

### defaultdict   --  优化dict

如下，从dict中获取key时，key不存在会抛出一个KeyError

```python
>>> a = {'a':'1','b':'2','c':'3'}
>>> a['a']
'1'
>>> a['d']
Traceback (most recent call last):
  File "<pyshell#155>", line 1, in <module>
    a['d']
KeyError: 'd'
```

如想在key不存在时返回一个默认值而不是异常，可以使用defaultdict

如下：

```python
>>> from collections import defaultdict
>>> dd = defaultdict(lambda: 'N/A')  # 参数必须是callable的
>>> dd['key1'] = 'abc'
>>> dd['key1']
'abc'
>>> dd['key2']
'N/A'
```

其实与dict中的get函数有点类似

```python
>>> od = {'a':1,'b':2,'c':3}
>>> od.get('d',4)  # 如果不存在key为d的，那么就返回4
4
```

 只不过一个是针对于某一个key，而另一个是针对所有key。

相似的还有dict中的setdefault函数

```python
>>> od = {'a':1,'b':2,'c':3}
>>> od.setdefault('d',4)   # 获取 key为d 的value，如果不存在，向dict中插入该key，value为第二个参数
4
>>> od
{'a': 1, 'b': 2, 'c': 3, 'd': 4}
```

### OrderedDict --  优化dict

python在3.6之前的dict中的key都是无序的，如果想让其按照插入的顺序可以直接使用OrderedDict。

```python
>>> from collections import OrderedDict
>>> d = dict([('a', 1), ('b', 2), ('c', 3)])
>>> d # dict的Key是无序的
{'a': 1, 'c': 3, 'b': 2}
>>> od = OrderedDict([('a', 1), ('b', 2), ('c', 3)])
>>> od # OrderedDict的Key是有序的
OrderedDict([('a', 1), ('b', 2), ('c', 3)])
```

OrderedDict往往会与Sorted函数结合使用，Sorted对dict进行排序，然后再放入OrderedDict当中。

```python
>>> from collections import OrderedDict
>>> d = {'banana': 3, 'apple':4, 'pear': 1, 'orange': 2}

#按key排序
>>>OrderedDict(sorted(d.items(), key=lambda t: t[0]))
OrderedDict([('apple',4), ('banana', 3), ('orange', 2), ('pear', 1)])

#按value排序
>>>OrderedDict(sorted(d.items(), key=lambda t: t[1]))
OrderedDict([('pear',1), ('orange', 2), ('banana', 3), ('apple', 4)])

#按key的长度排序
>>>OrderedDict(sorted(d.items(), key=lambda t: len(t[0])))
OrderedDict([('pear',1), ('apple', 4), ('orange', 2), ('banana', 3)])
```

### Counter  -- 计数器

`Counter`是一个简单的计数器，例如，统计字符出现的个数：

```python
>>> from collections import Counter
>>> c = Counter()  # 创建对象
>>> for ch in 'programming':
	c[ch] = c[ch] + 1  # 计数

	
>>> c
Counter({'r': 2, 'g': 2, 'm': 2, 'p': 1, 'o': 1, 'a': 1, 'i': 1, 'n': 1})
```

r、g、m各出现2次，其它的一次，其实通过Counter的数据格式可以看出其也是dict的子类

```python
>>> isinstance(c,Counter)
True
>>> isinstance(c,dict)
True
```

# struct

## 使用方式

由于数据的传输无论是内存到硬盘还是后端到前端，本质都是传输的二进制数据。二进制是计算机的最底层数据格式，所以python提供了一个struct模块，它使用一定格式的字符串作为C语言结构布局的简洁描述以及到或从Python值的预期转换。

Python提供了一个`struct`模块来解决`bytes`和其他二进制数据类型的转换。

`struct`的`pack`函数把任意数据类型变成`bytes`：

```python
>>> import struct
>>> struct.pack('>I', 10240099)
b'\x00\x9c@c'   # 10240099当作为10进制的即每一个数字4bit，8*4 = 32bit  32/8 = 4字节
                # 将10240099 转换为 4个字节并存入 byte数组
                # \x00\x9c@c 分为四部分  \x00   \x9c   @   c    
                # 前两个是16进制表示的， 后两个对应的是ascii码

>>> bs          
b'\x00\x9c@c'
>>> for a in bs:
	print(a)

	
0
156
64
99
```

```python
>>> struct.unpack('>I', b'\x00\x9c@c')
(10240099,)
```

`pack`的第一个参数是处理指令，`'>I'`的意思是：

`>`表示字节顺序是big-endian，也就是网络序，`I`表示4字节无符号整数。 字节顺序不明白的 [看这里](https://blog.csdn.net/weixin_40247263/article/details/81838143)

后面的参数个数要和处理指令一致。

`unpack`把`bytes`变成相应的数据类型：

```python
>>> struct.unpack('>IH', b'\xf0\xf0\xf0\xf0\x80\x80') 
(4042322160, 32896)

# 4个f0转换成二进制然后拼接再转换为10进制  为 4042322160
# 2个80转换成二进制然后拼接再转换为10进制  为 32896
```

根据`>IH`的说明，后面的`bytes`依次变为`I`：4字节无符号整数和`H`：2字节无符号整数。

struct 解析bmp文件

用画图工具画了一个椭圆

然后读取前30个字节，再解析

BMP格式采用小端方式存储数据，文件头的结构按顺序如下：

两个字节：`'BM'`表示Windows位图，`'BA'`表示OS/2位图； 一个4字节整数：表示位图大小； 一个4字节整数：保留位，始终为0； 一个4字节整数：实际图像的偏移量； 一个4字节整数：Header的字节数； 一个4字节整数：图像宽度； 一个4字节整数：图像高度； 一个2字节整数：始终为1； 一个2字节整数：颜色数。

```
>>> fd = open(r'D:\pythonJDK\1.bmp','rb')
>>> b = fd.read(30)
>>> b
b'BM6\xbb\x17\x00\x00\x00\x00\x006\x00\x00\x00(\x00\x00\x00\xc0\x03\x00\x00\x1c\x02\x00\x00\x01\x00\x18\x00'

>>> struct.unpack('<ccIIIIIIHH', b)
(b'B', b'M', 1555254, 0, 54, 40, 960, 540, 1, 24)
```

关于 struct函数整理

## struct模块中的函数
|函数|return|explain|
|----|----|----|
|pack(fmt,v1,v2…)|string|按照给定的格式(fmt),把数据转换成字符串(字节流),并将该字符串返回.|
|pack_into(fmt,buffer,offset,v1,v2…)|None|按照给定的格式(fmt),将数据转换成字符串(字节流),并将字节流写入以offset开始的buffer中.(buffer为可写的缓冲区,可用array模块)|
|unpack(fmt,v1,v2…..)|tuple|按照给定的格式(fmt)解析字节流,并返回解析结果|
|pack_from(fmt,buffer,offset)|tuple|按照给定的格式(fmt)解析以offset开始的缓冲区,并返回解析结果|
|calcsize(fmt)|size of fmt|计算给定的格式(fmt)占用多少字节的内存，注意对齐方式|

## 格式化字符串

当打包或者解包的时,需要按照特定的方式来打包或者解包.该方式就是格式化字符串,它指定了数据类型,除此之外,还有用于控制字节顺序、大小和对齐方式的特殊字符.

### 对齐方式

为了同c中的结构体交换数据，还要考虑c或c++编译器使用了字节对齐，通常是以4个字节为单位的32位系统，故而struct根据本地机器字节顺序转换.可以用格式中的第一个字符来改变对齐方式.定义如下
|Character|Byte order|Size|Alignment|
|----|----|----|----|
|@(默认)|本机|本机|本机,凑够4字节|
|=|本机|标准|none,按原字节数|
|<|小端|标准|none,按原字节数|
|>|大端|标准|none,按原字节数|
|!|network(大端)|标准|none,按原字节数|

如果不懂大小端,见[大小端参考网址](http://blog.csdn.net/lis_12/article/details/52698634).

### 格式符
|格式符|C语言类型|Python类型|Standard size|
|----|----|----|----|
|x|pad byte(填充字节)|no value||
|c|char|string of length 1|1|
|b|signed char|integer|1|
|B|unsigned char|integer|1|
|?|_Bool|bool|1|
|h|short|integer|2|
|H|unsigned short|integer|2|
|i|int|integer|4|
|I(大写的i)|unsigned int|integer|4|
|l(小写的L)|long|integer|4|
|L|unsigned long|long|4|
|q|long long|long|8|
|Q|unsigned long long|long|8|
|f|float|float|4|
|d|double|float|8|
|s|char[]|string||
|p|char[]|string||
|P|void *|long||

注- -!
- _Bool在C99中定义,如果没有这个类型,则将这个类型视为char,一个字节;
- q和Q只适用于64位机器;
- 每个格式前可以有一个数字,表示这个类型的个数,如s格式表示一定长度的字符串,4s表示长度为4的字符串;4i表示四个int;
- P用来转换一个指针,其长度和计算机相关;
- f和d的长度和计算机相关;

