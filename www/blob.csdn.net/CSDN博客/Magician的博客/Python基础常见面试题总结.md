
# Python基础常见面试题总结 - Magician的博客 - CSDN博客


2019年03月08日 16:36:40[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：309标签：[Python基础																](https://so.csdn.net/so/search/s.do?q=Python基础&t=blog)[面试题																](https://so.csdn.net/so/search/s.do?q=面试题&t=blog)[总结																](https://so.csdn.net/so/search/s.do?q=总结&t=blog)[常见																](https://so.csdn.net/so/search/s.do?q=常见&t=blog)[大全																](https://so.csdn.net/so/search/s.do?q=大全&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=常见&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=总结&t=blog)个人分类：[Python																](https://blog.csdn.net/qq_16633405/article/category/7645748)
[
																					](https://so.csdn.net/so/search/s.do?q=总结&t=blog)所属专栏：[Python总结](https://blog.csdn.net/column/details/26813.html)[
							](https://so.csdn.net/so/search/s.do?q=总结&t=blog)
[
																	](https://so.csdn.net/so/search/s.do?q=面试题&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=Python基础&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=Python基础&t=blog)


### 文章目录
[基础知识题](#_4)
[看程序写结果题](#_647)
[编程题](#_725)

以下是总结的一些常见的Python基础面试题，帮助大家回顾基础知识，了解面试套路。会一直保持更新状态。
PS：加粗为需要注意的点。
# 基础知识题
**1、深拷贝和浅拷贝的区别是什么？**
深拷贝是将对象本身复制给另一个对象。这意味着如果对对象的副本进行更改时不会影响原对象。
浅拷贝是将对象的引用复制给另一个对象。因此，如果我们在副本中进行更改，则会影响原对象。
**2、能否解释一下 *args 和**kwargs?**
如果我们不知道将多少个参数传递给函数，比如当我们想传递一个列表或一个元组值时，就可以使用*args。
当我们不知道将会传入多少关键字参数时，使用**kwargs 会收集关键字参数。
2.1、arg和 *kwarg作用？
万能参数，解决了函数参数不固定的问题
***arg：会把位置参数转化为tuple（非键值对的参数组）**
****kwarg：会把关键字参数转化为dict（键值对参数组）**
3、Python和Java、PHP、C、C\#、C++等其他语言的对比？
python代码，简介，明确，优雅，简单易懂
开发效率高
可扩展性强
**4、简述解释型和编译型编程语言？**
解释型：在执行程序时，计算机才一条一条的将代码解释成机器语言给计算机来执行
编译型：是把源程序的每一条语句都编译成机器语言，并保存成二进制文件，这样计算机运行该程序时可以直接以机器语言来运行此程序，运行速度很快。
5、Python解释器种类以及特点？（知道即可）
Cpython，IPython，Jpython，pypy，Ironpython
Python是一门解释器语言，代码想运行，必须通过解释器执行，Python存在多种解释器，分别基于不同语言开发，每个解释器有不同的特点，但都能正常运行Python代码，以下是常用的五种Python解释器：
CPython：当 从Python官方网站下载并安装好Python2.7后，就直接获得了一个官方版本的解释器：Cpython，这个解释器是用C语言开发的，所以叫 CPython，在命名行下运行python，就是启动CPython解释器，CPython是使用最广的Python解释器。
IPython：IPython是基于CPython之上的一个交互式解释器，也就是说，IPython只是在交互方式上有所增强，但是执行Python代码的功能和CPython是完全一样的，好比很多国产浏览器,虽然外观不同，但内核其实是调用了IE。
PyPy：PyPy是另一个Python解释器，它的目标是执行速度，PyPy采用JIT技术，对Python代进行动态编译，所以可以显著提高Python代码的执行速度。
Jython：Jython是运行在Java平台上的Python解释器，可以直接把Python代码编译成Java字节码执行。
IronPython：IronPython和Jython类似，只不过IronPython是运行在微软.Net平台上的Python解释器，可以直接把Python代码编译成.Net的字节码。
在Python的解释器中，使用广泛的是CPython，对于Python的编译，除了可以采用以上解释器
进行编译外，技术高超的开发者还可以按照自己的需求自行编写Python解释器来执行Python代码，十分的方便！
6、位和字节的关系？
一个字节=8位
**7、b、B、KB、MB、GB 的关系？**
1B(字节) = 8b(位)
1KB = 1024B
1MB = 1024KB
1GB = 1024MB
8、请至少列举5个 PEP8 规范（知道）
缩进：每一级4个缩进。连续跨行应该使用圆括号或大括号或者使用悬挂缩进。
代码长度约束
一行列数：PEP8 规定最大为79列，如果拼接url很容易超限
一个函数：不可以超过30行；直观来讲就是完整显示一个函数一个屏幕就够了，不需要上下拖动 一个类：不要超过200行代码，不要超过10个方法
一个模块：不要超过500行
import
不要在一句import中引用多个库
命名规范
注释
总体原则，错误的注释不如没有注释。所以当一段代码发生变化时，第一件事就是要修改注释！
9、通过代码实现如下转换（知道即可）
二进制转换成十进制：v = “0b1111011”
print(int('0b1111011',2))十进制转换成二进制：v = 18
print(bin(18))八进制转换成十进制：v = “011”
print(int('011',8))十进制转换成八进制：v = 30
print(oct(30))十六进制转换成十进制：v = “0x12”
print(int('0x12',16))十进制转换成十六进制：v = 87
print(hex(87))10、ascii、unicode、utf-8、gbk 区别？(知道即可)
ASCII码：使用一个字节编码，所以它的范围基本是只有英文字母、数字和一些特殊符号 ，只有256个字符。
Unicode：能够表示全世界所有的字节
GBK：是只用来编码汉字的，GBK全称《汉字内码扩展规范》，使用双字节编码。
UTF-8：是一种针对Unicode的可变长度字符编码，又称万国码。
11、列举 Python2和Python3的区别？
默认编码：2–>ascii，3–>utf-8
print的区别：python2中print是一个语句，不论想输出什么，直接放到print关键字后面即可。python3里，print()是一个函数，像其他函数一样，print()需要你将要输出的东西作为参数传给它。
input的区别：
python2有两个全局函数，用在命令行请求用户输入。第一个叫input()，它等待用户输入一个python表达式(然后返回结果)。
第二个叫做raw_input(),用户输入什么他就返回什么。 python3 通过input替代了他们。
字符串：python2中有两种字符串类型：Unicode字符串和非Unicode字符串。Python3中只有一种类型：Unicode字符串。
xrange()
python２里，有两种方法获得一定范围内的数字：range(),返回一个列表，还有xrange(),返回一个迭代器。
python3　里，range()返回迭代器，xrange()不再存在。
12、xrange和range的区别？
python２里，有两种方法获得一定范围内的数字：range(),返回一个列表，还有xrange(),返回一个迭代器。
python3　里，range()返回迭代器，xrange()不再存在。
13、文件操作时：xreadlines和readlines的区别？
readlines返回一个list，xreadlines方法返回一个生成器
14、字符串、列表、元组、字典每个常用的5个方法？
字符串：repleace,strip,split,upper,lower,join,find,index…
replace(old, new [, max])
把将字符串中的 str1 替换成 str2,如果 max 指定，则替换不超过 max 次。
strip([chars])\#去空格及特殊符号
在字符串上执行 lstrip()和 rstrip()
s.strip().lstrip().rstrip(',')split(str="", num=string.count(str))
num=string.count(str)) 以 str 为分隔符截取字符串，如果 num 有指定值，则仅截取 num+1 个子字符串
join(seq)
以指定字符串作为分隔符，将 seq 中所有的元素(的字符串表示)合并为一个新的字符串
find(str, beg=0 end=len(string))
检测 str 是否包含在字符串中，如果指定范围 beg 和 end ，则检查是否包含在指定范围内，如果包含返回开始的索引值，否则返回-1
index(str, beg=0, end=len(string))
跟find()方法一样，只不过如果str不在字符串中会报一个异常.
sStr1 = 'strchr'
sStr2 = 's'
nPos = sStr1.index(sStr2)
print nPos列表：append,pop,insert,remove,reverse,sort,count,index…
增：
list.append(obj)
在列表末尾添加新的对象
list.insert(index, obj)
将对象插入列表
删：
list.pop([index=-1])
移除列表中的一个元素（默认最后一个元素），并且返回该元素的值
list.remove(obj)
移除列表中某个值的第一个匹配项
改：
list.reverse()
反向列表中元素
\#将字符串反转
st=List(s)
result=“”.join(st.reverse())查：
list.sort(cmp=None, key=None, reverse=False)
对原列表进行排序
list.count(obj)
统计某个元素在列表中出现的次数
元组：index,count,len(),dir()
元组不可更改所以只能查
查：
max(tuple)
返回元组中元素最大值。
字典：get,keys,values,pop,popitems,clear,update,items…
查：
radiansdict.get(key, default=None)
返回指定键的值，如果值不在字典中返回default值
radiansdict.keys()
返回一个迭代器，可以使用 list() 来转换为列表
radiansdict.values()
返回一个迭代器，可以使用 list() 来转换为列表
radiansdict.items()
以列表返回可遍历的(键, 值) 元组数组
删：
pop(key[,default])
删除字典给定键 key 所对应的值，返回值为被删除的值。key值必须给出。 否则，返回default值
popitem()
随机返回并删除字典中的一对键和值(一般删除末尾对)
radiansdict.clear()
删除字典内所有元素
改：
radiansdict.update(dict2)
把字典dict2的键/值对更新到dict里
直接增/改
dict = {'Name': 'Runoob', 'Age': 7, 'Class': 'First'} 
dict['Age'] = 8; \# 更新 Age 
dict['School'] = "菜鸟教程" \# 添加信息 
print ("dict['Age']: ", dict['Age']) 
print ("dict['School']: ", dict['School'])15、lambda表达式格式以及应用场景？
lambda函数赋值给其他函数
表达式格式：lambda后面跟一个或多个参数，紧跟一个冒号，以后是一个表达式。冒号前是参数，冒号后是返回值。例如：lambda x : 2x
**应用场景：**经常与一些内置函数相结合使用，比如说map(),filter(),sorted(),reduce()等
frame.applymap(lambda x: '%.2f' % x)
frame.apply(lambda x: x.max() - x.min())16、Pandas 中map, applymap and apply的区别
apply()是一种让函数作用于列或者行（一维向量）操作（重点：选取数据的某行或者列）
applymap()是一种让函数作用于DataFrame每一个元素的操作（选取的是所有数据即Dataframe）
map是一种让函数作用于Series每一个元素的操作（数据选取的是某行或某列（即Series），然后对其中的每个元素进行操作）
总结：要对数据进行应用函数时，先看数据结构是DataFrame还是Series,Series结构直接用map(),DataFrame结构的话再看是要按行还是按列进行操作来选择对应的函数即可。重点还是取决于数据源是什么数据结构，然后你想做什么样的操作。
map(function, iterable, …)
map()函数接收两个参数，一个是函数，一个是可迭代的对象，map将传入的函数依次作用到序列的每个元素，并把结果作为新的list返回。
基本等价于 [f(x) for x in interable]
17、pass的作用？
空语句 do nothing
保证格式完整
保证语义完整
18、is和==的区别？
is:判断内存地址是否相等
==：判断数值是否相等
19、Python垃圾回收机制？（知道即可）
python采用的是引用计数机制为主，标记-清除和分代收集(隔代回收、分代回收)两种机制为辅的策略
计数机制
Python的GC模块主要运用了引用计数来跟踪和回收垃圾。在引用计数的基础上，还可以通过“标记-清除”解决容器对象可能产生的循环引用的问题。通过分代回收以空间换取时间进一步提高垃圾回收的效率。
标记-清除：
标记-清除的出现打破了循环引用，也就是它只关注那些可能会产生循环引用的对象
缺点：该机制所带来的额外操作和需要回收的内存块成正比。
隔代回收
原理：将系统中的所有内存块根据其存活时间划分为不同的集合，每一个集合就成为一个“代”，垃圾收集的频率随着“代”的存活时间的增大而减小。也就是说，活得越长的对象，就越不可能是垃圾，就应该减少对它的垃圾收集频率。那么如何来衡量这个存活时间：通常是利用几次垃圾收集动作来衡量，如果一个对象经过的垃圾收集次数越多，可以得出：该对象存活时间就越长。
20、python的可变类型和不可变类型？
数据字符串赋新值时，只是开辟了新的空间创建了一个新值，并未更改旧值。
不可变类型（数字、字符串、元组、不可变集合）
可变类型（列表、字典、可变集合）
21、列举常见的内置函数？
map,filter（当返回值为正数时才返回函数结果进入下个元素的计算）,zip,len,bin,oct,hex,int,float,bool,sum,min,max,str,list,tuple,dict,range,next,hash,help,id…
enumerate,reduce,eval,format
map() 会根据提供的函数对指定序列做映射。
第一个参数 function 以参数序列中的每一个元素调用 function 函数，返回包含每次 function 函数返回值的新列表。
map(function, iterable, …)
参数
•	function – 函数
•	iterable – 一个或多个序列
返回值
Python 2.x 返回列表。
Python 3.x 返回迭代器。
迭代器是访问集合元素的一种方式。迭代器对象从集合的第一个元素开始访问，直到所有的元素被访问完结束。迭代器只能往前不会后退。
>>>def square(x) : \# 计算平方数 
... return x ** 2 ... 
>>> map(square, [1,2,3,4,5]) \# 计算列表各个元素的平方 
[1, 4, 9, 16, 25] 
>>> map(lambda x: x ** 2, [1, 2, 3, 4, 5]) \# 使用 lambda 匿名函数 
[1, 4, 9, 16, 25] 
\# 提供了两个列表，对相同位置的列表数据进行相加 
>>> map(lambda x, y: x + y, [1, 3, 5, 7, 9], [2, 4, 6, 8, 10]) 
[3, 7, 11, 15, 19]filter() 函数用于过滤序列，过滤掉不符合条件的元素，返回由符合条件元素组成的新列表。
该接收两个参数，第一个为函数，第二个为序列，序列的每个元素作为参数传递给函数进行判，然后返回 True 或 False，最后将返回 True 的元素放到新列表中。
参数
•	function – 判断函数。
•	iterable – 可迭代对象。
返回值
返回列表。
def is_odd(n): 
return n % 2 == 1 
newlist = filter(is_odd, [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]) 
print(newlist)
输出结果 ：
[1, 3, 5, 7, 9]zip() 函数用于将可迭代的对象作为参数，将对象中对应的元素打包成一个个元组，然后返回由这些元组组成的列表。
如果各个迭代器的元素个数不一致，则返回列表长度与最短的对象相同，利用 * 号操作符，可以将元组解压为列表。
zip 语法：
zip([iterable, …])
参数说明：
•	iterabl – 一个或多个迭代器;
返回值
返回元组列表。
>>>a = [1,2,3] 
>>> b = [4,5,6] 
>>> c = [4,5,6,7,8] 
>>> zipped = zip(a,b) \# 打包为元组的列表 [(1, 4), (2, 5), (3, 6)] 
>>> zip(a,c) \# 元素个数与最短的列表一致 [(1, 4), (2, 5), (3, 6)] 
>>> zip(*zipped) \# 与 zip 相反，*zipped 可理解为解压，返回二维矩阵式 [(1, 2, 3), (4, 5, 6)]bin() 返回一个整数 int 或者长整数 long int 的二进制表示.
参数
x – int 或者 long int 数字
返回值
字符串。
>>>bin(10) 
'0b1010' 
>>> bin(20) 
'0b10100'oct() 函数将一个整数转换成8进制字符串。
>>>oct(10) 
'012' 
>>> oct(20) 
'024' 
>>> oct(15) 
'017' 
>>>hex() 函数用于将10进制整数转换成16进制，以字符串形式表示。
>>>hex(255) 
'0xff' 
>>> hex(-42) 
'-0x2a' 
>>> hex(1L) 
'0x1L' 
>>> hex(12) 
'0xc' 
>>> type(hex(12)) 
<class 'str'> \# 字符串hash() 用于获取取一个对象（字符串或者数值等）的哈希值。
hash 语法：
hash(object)
参数说明：
•	object – 对象；
返回值
返回对象的哈希值。
>>>hash('test') \# 字符串 
2314058222102390712 
>>> hash(1) \# 数字 
1 
>>> hash(str([1,2,3])) \# 集合 
1335416675971793195 
>>> hash(str(sorted({'1':1}))) \# 字典 
7666464346782421378 
>>enumerate(sequence, [start=0])
enumerate() 函数用于将一个可遍历的数据对象(如列表、元组或字符串)组合为一个索引序列，同时列出数据和数据下标，一般用在 for 循环当中。
参数
sequence – 一个序列、迭代器或其他支持迭代对象。
start – 下标起始位置。
返回值
返回 enumerate(枚举) 对象。
for 循环使用 enumerate
>>>seq = ['one', 'two', 'three'] 
>>> for i, element in enumerate(seq): 
... print i, element 
... 
0 one 
1 two 
2 threeeval(expression[, globals[, locals]])
eval() 函数用来执行一个字符串表达式，并返回表达式的值。
参数
•	expression – 表达式。
•	globals – 变量作用域，全局命名空间，如果被提供，则必须是一个字典对象。
•	locals – 变量作用域，局部命名空间，如果被提供，可以是任何映射对象。
返回值
返回表达式计算结果。
>>>x = 7 
>>> eval( '3 * x' ) 
21 
>>> eval('pow(2,2)') 
4str.format()
格式化字符串的函数 str.format()，它增强了字符串格式化的功能。
基本语法是通过 {} 和 : 来代替以前的 % 。
format 函数可以接受不限个参数，位置可以不按顺序。
实例
>>>"{} {}".format("hello", "world") \# 不设置指定位置，按默认顺序 'hello world' >>> "{0} {1}".format("hello", "world") \# 设置指定位置 'hello world' 
>>> "{1} {0} {1}".format("hello", "world") \# 设置指定位置 'world hello world'reduce(function, iterable[, initializer])
reduce() 函数会对参数序列中元素进行累积。
函数将一个数据集合（链表，元组等）中的所有数据进行下列操作：用传给 reduce 中的函数 function（有两个参数）先对集合中的第 1、2 个元素进行操作，得到的结果再与第三个数据用 function 函数运算，最后得到一个结果。
参数
•	function – 函数，有两个参数
•	iterable – 可迭代对象
•	initializer – 可选，初始参数
返回值
返回函数计算结果。
实例
以下实例展示了 reduce() 的使用方法：
>>>def add(x, y) : \# 两数相加 
... return x + y ... 
>>> reduce(add, [1,2,3,4,5]) \# 计算列表和：1+2+3+4+5 
15 
>>> reduce(lambda x, y: x+y, [1,2,3,4,5]) \# 使用 lambda 匿名函数 
15class set([iterable])
set() 函数创建一个无序不重复元素集，可进行关系测试，删除重复数据，还可以计算交集、差集、并集等。
参数说明：
•	iterable – 可迭代对象对象；
返回值
返回新的集合对象。
实例
以下实例展示了 set 的使用方法：
>>>x = set('runoob') 
>>>y = set('google') 
>>> x, y 
(set(['b', 'r', 'u', 'o', 'n']), set(['e', 'o', 'g', 'l'])) \# 重复的被删除 
>>> x & y \# 交集 
set(['o']) 
>>> x | y \# 并集 
set(['b', 'e', 'g', 'l', 'o', 'n', 'r', 'u']) 
>>> x - y \# 差集 
set(['r', 'b', 'u', 'n']) 
>>>22、filter、map、reduce的作用？
filter(function，iterable)过滤函数
map(function，iterable)循环函数
reduce(function, iterable)累积函数
23、如何安装第三方模块？以及用过哪些第三方模块？
pip3 install 模块名
24、至少列举8个常用模块都有那些？
os,sys,time,random,re,hashlib,logging,json,pickle…
os模块提供了不少与操作系统相关联的函数.
sys通用工具脚本经常调用命令行参数.
re模块为高级字符串处理提供了正则表达式工具。对于复杂的匹配和处理，正则表达式提供了简洁、优化的解决方案:
random提供了生成随机数的工具。
re的match和search区别？
match:从字符串的开头位置匹配，必须以此为开头
search:从开头开始查，找到符合的就返回结果
25、如何实现 “1,2,3” 变成 [‘1’,‘2’,‘3’] ?
a = "1,2,3"
list = a.split(",")
print(list)26、如何实现[‘1’,’2’,’3’]变成[1,2,3] ?
li = ['1','2','3']
series = pd.Series(li)
series=list(series.map(lambda x:int(x)))
print(series)27、比较： a = [1,2,3] 和 b = [(1),(2),(3) ] 以及 b = [(1,),(2,),(3,) ] 的区别？
a = [1,2,3]正常的列表
b = [(1),(2),(3)] 虽然列表的每个元素加上了括号，但是当括号内只有一个元素并且没有逗号时，其数据类型是元素本身的数据类型
b = [(1,),(2,),(3,)]列表中的元素类型都是元组类型
28、如何在函数中设置一个全局变量 ?
使用python的内置语法 globals 全局变量
29、logging模块的作用？以及应用场景？
logging模块的作用：
程序调试
了解软件程序运行情况，是否正常
软件程序运行故障分析与问题定位
应用场景：网站的运维工作，程序实时监控
30、常用字符串格式化哪几种？
1、%s %d
print("这位同学叫%s,年龄是%d岁"  %('小明',10))
2、format格式化输出
print("这位同学叫{name},年龄是{age}岁".format(name="小明", age=18))
3、print(f'内容{变量名}')31、简述 生成器、迭代器、可迭代对象 以及应用场景？
生成器：在 Python 中，一边循环一边计算的机制，称为生成（generator），通过next()取值，两种表现形式
1、将列表生成式的[]改为()
2、含有yield关键字的函数
应用场景：优化代码，节省内存
带有 yield 的函数在 Python 中被称之为 generator（生成器）
迭代器：是访问集合元素的一种方式。迭代器同时实现了__iter__和__next__方法
可迭代对象：只要实现了__iter__方法的对象就是可迭代对象
32、谈谈你对闭包的理解？
在一个外函数中定义了一个内函数，内函数里运用了外函数的临时变量，并且外函数的返回值是内函数的引用。这样就构成了一个闭包。
一般情况下，在我们认知当中，如果一个函数结束，函数的内部所有东西都会释放掉，还给内存，局部变量都会消失。
但是闭包是一种特殊情况，如果外函数在结束的时候发现有自己的临时变量将来会在内部函数中用到，就把这个临时变量绑定给了内部函数，然后自己再结束。
33、os和sys模块的作用？
os模块负责程序与操作系统的交互，提供了访问操作系统底层的接口;
sys模块负责程序与python解释器的交互，提供了一系列的函数和变量，用于操控python的运行时环境。
34、如何使用python删除一个文件？
import os
os.remove(r'path')35、谈谈你对面向对象的理解？
面向对象的程序设计的核心是对象（上帝式思维），要理解对象为何物，必须把自己当成上帝，上帝眼里世间存在的万物皆为对象，
不存在的也可以创造出来。对象是特征和技能的结合，其中特征和技能分别对应对象的数据属性和方法属性。
优点是：解决了程序的扩展性。对某一个对象单独修改，会立刻反映到整个体系中，如对游戏中一个人物参数的特征和技能修改都很容易。
缺点：可控性差，无法向面向过程的程序设计流水线式的可以很精准的预测问题的处理流程与结果，
面向对象的程序一旦开始就由对象之间的交互解决问题，即便是上帝也无法预测最终结果。
应用场景：需求经常变化的软件，一般需求的变化都集中在用户层，互联网应用，企业内部软件，游戏等都是面向对象的程序设计大显身手的好地方。
36、Python面向对象中的继承有什么特点？
在继承中基类的构造(init()方法)不会被自动调用，它需要在其派生类的构造中亲自专门调用。
在调用基类的方法时，需要加上基类的类名前缀，且需要带上self参数变量。
区别于在类中调用普通函数时并不需要带上self参数
Python总是首先查找对应类型的方法，如果它不能在派生类中找到对应的方法，它才开始到基类中逐个查找。(先在本类中查找调用的方法，找不到才去基类中找)。
37、面向对象中super的作用？
super在面向对象继承类中代指父类，书写方法super(类名,self).属性或者方法或super().属性或者方法
super方法可以增加类之间调用的灵活性，当父类名发生变化时不必修改
super方法在类的多继承时可以简化代码，避免代码冗余
super机制里可以保证公共父类仅被执行一次，执行的顺序遵循MRO，广度优先查询方法
38、是否使用过functools中的函数？其作用是什么？
functools用于高阶函数：指那些作用于函数或者返回其他函数的函数。通常情况下，只要是可以被当做函数调用的对象就是这个模块的目标。
39、列举面向对象中带双下划线的特殊方法，如： new 、 init
new：构造方法，创建一个对象，实例化时第一个被执行，返回一个创建好的对象及__init__(self)的self，只有继承了object的类才会有这个方法
init：初始化方法，__init__在__new__的基础上完成一些其它初始化的动作，__init__没有返回值
40、如何判断是函数还是方法？
函数和方法都封装了一些独立的功能，如果在类中定义的函数那就是方法(对象或者类名点方法名调用)，
否则就是函数(函数名()直接调用)
41、静态方法和类方法区别？
静态方法：是既不是用类中的属性又不使用对象中的属性，由类或者对象调用的方法，依赖python装饰器@staticmethod来实现
类方法：只使用类中的静态变量，一般都是由类调用，依赖python装饰器@classmethod来实现
42、列举面向对象中的特殊成员以及应用场景？
call：对象的构造方法，对象加上()，可以触发这个类的__call__方法。
len：内置函数的len函数是依赖类中的__len__方法
eq：判断值是否相等的时候依赖__eq__方法
hash:判断hash值是否相等的时候依赖__hash__方法(拓展：set的去重机制其实就是根据__hash__和__eq__方法实现的)
str：和str() print() %s 都是息息相关的，返回值一定是字符串类型
repr：和 repr() %r都是息息相关的，在没有__str__方法时，repr__可以完全取代__str。
del析构方法，对应着一个对象的删除之前执行的内容
43、什么是反射？以及应用场景？（知道）
定义：通过用字符串数据类型的变量名来访问这个变量的值，在python面向对象中的反射,通过字符串的形式操作对象相关的属性或方法.
应用场景：用于处理通过用户输入，文件读取，或者网络传输所得到的字符串形式的指令来完成对应的操作
44、metaclass作用？以及应用场景？（知道）
metaclass，直译为元类，简单的解释就是：当我们定义了类以后，就可以根据这个类创建出实例，
所以：先定义类，然后创建实例。但是如果我们想创建出类呢？那就必须根据metaclass创建出类，换句话说，你可以把类看成是metaclass创建出来的“实例”
45、isinstance作用以及应用场景？
isinstance作用是来判断一个对象是否是一个已知的类型
46、异常处理写法以及如何主动抛出异常（应用场景）
异常处理的常规写法：    
try:
    \#执行的主体函数
except Exception as e:
    print(str(e))
    
主动抛出异常：
raise TypeError('出现了不可思议的异常')\#TypeError可以是任意的错误类型47、简述 yield和yield from关键字。
yield 是一个类似 return 的关键字，只是这个函数返回的是个生成器当你调用这个函数的时候，函数内部的代码并不立马执行 ，这个函数只是返回一个生成器对象，当你使用for进行迭代的时候，函数中的代码才会执行
yield from 的主要功能是打开双向通道，把最外层的调用方与最内层的子生成器连接起来，这样二者可以直接发送和产出值，还可以直接传入异常，而不用在位于中间的协程中添加大量处理异常的样板代码。
有了这个结构，协程可以通过以前不可能的方式委托职责。
48、解释 Python 中的三元表达式
与 C++不同, 在 Python 中我们不需要使用 ? 符号，而是使用如下语法：
[on true] if [expression]else [on false]
如果 [expression] 为真, 则 [on true] 部分被执行。如果表示为假则 [on false] 部分被执行
49、解释继承
一个类继承自另一个类，也可以说是一个孩子类/派生类/子类，继承自父类/基类/超类，同时获取所有的类成员（属性和方法）。
继承使我们可以重用代码，并且还可以更方便地创建和维护代码。Python 支持以下类型的继承：
单继承- 一个子类类继承自单个基类
多重继承- 一个子类继承自多个基类
多级继承- 一个子类继承自一个基类，而基类继承自另一个基类
分层继承- 多个子类继承自同一个基类
混合继承- 两种或两种以上继承类型的组合
关于继承的更多内容参考 Python Inheritance（[https://data-flair.training/blogs/python-inheritance/）](https://data-flair.training/blogs/python-inheritance/%EF%BC%89)
50、什么是 Flask？
Flask 是一个使用 Python 编写的轻量级 Web 应用框架，使用 BSD 授权。其 WSGI 工具箱采用 Werkzeug，模板引擎则使用 Jinja2。除了 Werkzeug 和 Jinja2 以外几乎不依赖任何外部库。因为 Flask 被称为轻量级框架。
Flask 的会话会话使用签名 cookie 来允许用户查看和修改会话内容。它会记录从一个请求到另一个请求的信息。但如果要修改会话，则必须有密钥 Flask.secret_key。
51、当退出 Python 时是否释放所有内存分配？
答案是否定的。那些具有对象循环引用或者全局命名空间引用的变量，在 Python 退出是往往不会被释放
另外不会释放 C 库保留的部分内容。
52、解释 Python 中的//，％和运算符**
//运算符执行地板除法，返回结果的整数部分 (向下取整)。
**符号表示取幂. a**b 返回 a 的 b 次方   
% 是取模符号。返回除法后的余数。53、什么是pickling和unpickling？
Pickle模块读入任何Python对象，将它们转换成字符串，然后使用dump函数将其转储到一个文件中——这个过程叫做pickling。
反之从存储的字符串文件中提取原始Python对象的过程，叫做unpickling。
54、将字符串str1的前三位替换为‘R’：
3*’R’+str1[3:]
创建一个与Str1顺序相反的字符串
方法一(推荐)：
Str1[::-1]
方法二：
l=list（Str1）
l.reverse()\#reverse应用于list
print(“”.join(l))看程序写结果题
55、求结果
v = dict.fromkeys(['k1','k2'],[])
v['k1'].append(666)
print(v)
v['k1'] = 777
print(v)
答案：{'k1':[666],'k2':[666]}
{'k1':777,'k2':[666]}
解析：formkeys()默认参数为可变数据类型时有坑56、求结果：
def num():
return [lambda x: i*x for i in range(4)]
print([m(2) for m in num()])
答案：[6, 6, 6, 6]
解析： 问题的本质在与python中的属性查找规则，LEGB（local，enclousing，global，bulitin），
在上面的例子中，i就是在闭包作用域（enclousing），而Python的闭包是 迟绑定 ，
这意味着闭包中用到的变量的值，是在内部函数被调用时查询得到的
所以：[lambda x: i*x for i in range(4)]打印出来是含有四个内存地址的列表，每个内存地址中的i在本内存中都没有被定义，而是通过闭包作用域中的i值，当for循环执行结束后，i的值等于3，所以再执行[m(2) for m in num()]时，每个内存地址中的i值等于3，当x等于2时，打印出来的结果都是6，从而得到结果[6, 6, 6, 6]。57、求结果：
a. [ i % 2 for i in range(10) ] ===>[0,1,0,1,0,1,0,1,0,1]
b. ( i % 2 for i in range(10) )===>返回一个生成器的内存地址58、求结果：
a. 1 or 2 =========>1    
b. 1 and 2 ========>2    
c. 1 < (2==2)======>false    
d. 1 < 2 == 2======>ture59、def func(a,b=[]) 这种写法有什么坑？
def func(a,b=[]):
	b.append(a)
   print(b)  
函数的第二个默认参数是一个list，当第一次执行的时候实例化了一个list，
第二次执行还是用第一次执行的时候实例化的地址存储，以后每次实例化都是60、关于Fliter&lambda的题目
输出nums的值：
nums = range(2,20)
for i in nums:
  nums = filter(lambda x:x==i or x % i,nums)
\#注意lambda输出为非空时，才能进入下个数进行计算（即x%i的值为非空时才返回值进入下个数的运算）
所以满足x==i且x%i的值为正数时才执行fliter函数。输出结果：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190308162954984.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1,size_16,color_FFFFFF,t_70)
61、关于dict的一些操作
[
](https://img-blog.csdnimg.cn/20190308162954984.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1,size_16,color_FFFFFF,t_70)key = 'abcde'
value = range(1, 6)
b0=dict(zip(key, value))
b1=range(10)
b2=[i for i in b1 if i in b0]
print(b2)
\#for s in b0返回的是对应的dict的键值即索引值
print([b0[s] for s in b0])
b3=[b0[s] for s in b0]
b4=[i for i in b1 if i in b3]
print(b4)
b5={i:i*i for i in b1}
print(b5)
b6=[[i*i] for i in b1]
print(b6)[
](https://img-blog.csdnimg.cn/20190308162954984.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1,size_16,color_FFFFFF,t_70)输出：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019030816305892.jpg)
编程题
1、请编写一个函数实现将IP地址转换成一个整数。
如 10.3.9.12 转换规则为：
10 00001010
3 00000011
9 00001001
12 00001100
再将以上二进制拼接起来计算十进制结果：00001010 00000011 00001001 00001100 = ？
答案：
def func(x):
    lis = x.strip().split('.')
\#注意这种连贯用法（直接将for循环接函数处理，需要用[]括起来）
    li = [bin(int(i)) for i in lis]
\#replace的参数为str类型；
    li2 = [i.replace('0b',(10-len(i))*'0') for i in li]
\#join参数为list类型；
    return int(''.join(li2),2)
ret = func('10.3.9.12')
print(ret)2、一行代码实现9*9乘法表。
lis = ['%s*%s=%s'%(i,j,i*j) for i in range(1,10) for j in range(i,10)]3、如何用一行代码生成[1,4,9,16,25,36,49,64,81,100] ?
li = [x*x for x in range(1,11)]4、一行代码实现删除列表中重复的值 ?
li = [1, 1, 1, 23, 3, 4, 4]
new_li = list(set(li))5、请用代码简答实现stack 。
def Stack(object):
    def __init__(self):
        self.stack = []
    def push(self,value):
    \# 进栈
        self.stack.append(value)
    def pop(self):
    \# 出栈
        if self.stack:
            self.stack.pop()
        else:
            raise LookupError('stack is empty!')
    def is_empty(self):
    \# 查看stack是否为空
        return bool(self.stack)
    def top(self):
    \# 取出stack中最新的值
        return self.stack[-1]6、用Python实现一个二分查找的函数。
lis = [0, 1, 3, 4, 5, 6, 7, 9, 10, 11,12,16,17]
def two_find(x, lis, start=0, end=None):
    if end == None:
	end = len(lis) - 1
    num = (end - start) // 2 + start
    if end > start:
        if lis[num] > x:
            return two_find(x, lis, start=start, end=num)
        elif lis[num] < x:
            return two_find(x, lis, start=num + 1, end=end)
        elif lis[num] == x:
            return num
    elif lis[end] == x:return end
    else:return None
print(two_find(17, lis))7、如何生成一个随机数？
import random
def rdm(n):
    lis = []
    for i in range(n):
        n = random.randint(1,9)
        lis.append(str(n))
        s = ''.join(lis)
    return int(s)
print(rdm(3))8、1、2、3、4、5 能组成多少个互不相同且无重复的三位数
count = 0
for i in range(1,6):
    for j in range(1,6):
        for k in range(1,6):
            if (i != j) and (i != k) and (j != k):
                count += 1
                if count % 6:
                    print(f'{i}{j}{k}', end='|')
                else:
                    print(f'{i}{j}{k}')
print(count)9、用尽量多的方法实现单例模式。
1、基于__new__()方法
class Person:
    def __new__(cls, *args, **kwargs):
        if not hasattr(cls,cls._instance):
            cls._instance = super().__new__(cls)
            return cls._instance
2、基于模块导入方式，现在一个py文件中写好一个类，实例化一个对象。以后用这个类直接导入这个模块就是单例模式。
3、基于装饰器方法实现
def singleton(cls, *args, **kwargs):
    instance_dic = {}
    def inner(*args, **kwargs):
        if cls not in instance_dic:
            instance_dic['cls'] = cls(*args, **kwargs)
            return instance_dic['cls']
        return inner
@singleton
class Person:
    pass
装饰器的写法以及应用场景。
装饰器的写法：
def wrapper(func):
def inner(*args,**kwargs):
'被装饰之前的操作'
ret = func(*args,**kwargs)
'被装饰之后的操作'
return ret
return inner
装饰器的应用场景： 
比如注册登录、插入日志，性能测试，事务处理，缓存等等场景10、判断一个数是否是质数
def isPrime(num):
    if num<=3:
        return num>1
    for i in range(2,num):
        if(num%i==0):
            return False
    return True
print(isPrime(4))

