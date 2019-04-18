# Python-GoogleCourse 学习 Day2 - fighting！！！ - CSDN博客
2019年02月22日 23:21:46[dujiahei](https://me.csdn.net/dujiahei)阅读数：22
## 参考
[http://www.runoob.com/python/python-strings.html](http://www.runoob.com/python/python-strings.html)
[https://www.liaoxuefeng.com/wiki/0014316089557264a6b348958f449949df42a6d3a2e542c000/001431664106267f12e9bef7ee14cf6a8776a479bdec9b9000](https://www.liaoxuefeng.com/wiki/0014316089557264a6b348958f449949df42a6d3a2e542c000/001431664106267f12e9bef7ee14cf6a8776a479bdec9b9000)
[https://www.bilibili.com/video/av4050443/](https://www.bilibili.com/video/av4050443/)
[https://www.cnblogs.com/hyyq/p/8922075.html](https://www.cnblogs.com/hyyq/p/8922075.html)
[https://www.cnblogs.com/jamsent/p/7183905.html](https://www.cnblogs.com/jamsent/p/7183905.html)
[https://www.jb51.net/article/67161.htm](https://www.jb51.net/article/67161.htm) 自己编程中遇到的Python错误和解决方法汇总整理
[https://www.cnblogs.com/ifantastic/p/3811145.html](https://www.cnblogs.com/ifantastic/p/3811145.html) python 中正确复制列表的方式
[https://www.cnblogs.com/yyds/p/6123692.html](https://www.cnblogs.com/yyds/p/6123692.html) 序列
- 基础
	- string 字符串
		- 定义
			字符串或串(String)是由数字、字母、下划线组成的一串字符，在python中，我们定义使用单引号‘’和双引号“”定义/创建字符串。
			​需要注意的是 1. python不支持单字符类型，单字符在Python中也是作为一个字符串使用。
			2. ​一对单引号、一对双引号没有区别，都是string类型，但是不能有双引号或者单引号出现两对进行直接嵌套，如果有需要，则使用转义字符。
			3. 使用三引号。​
			原字符串：比如下面这个例子， 希望"\"后面的字母不被python看作是转义字符，在字符串前面加上一个字母‘r’即可。
			例子： print （r"c:\temp\node\jeapedu.py"）
			​
- 基本操作（+，*，读取方式）
			- 更新
				字符串的更新不是在原地址上更新，而是重新开辟了一个新的地址空间。
- 访问/读取
				a.索引访问：字符串是python的一种序列型的数据类型，字符串里的每一个字符都有一个标号可以标识其在字符串中的位置，从左至右依次是0，1，2...n-1，从右至左依次是-1，-2，-3....-n（其中n是字符串的长度），所有我们就可以通过索引来访问字符串中的某个字符。 print s[0]
				​b.切片访问：访问字符串中某个范围的子串。语法格式：str_name[start:end:step]，start是访问字符串的起点，end为终点，step为步长，得到的子串由start到end-1这些字符组成(前闭后开)。​ print s[0:6]
- 加法和乘法
				python中不但支持字符串相加，还支持字符串的乘法，使用起来相当方便。加法就是将两个字符串连接在一起，而乘法就是字符串的多次相加。
- 字符串相关方法
			- 查找find、index
- 计数count
- 指定编码 解码 encode decode
- 替换 replace
- 分割 split ，拼接 join
- 字符串格式化（支持占位符）
- list 列表
		python中的list列表是一种序列型数据类型，一有序数据集合用逗号间隔用方括号括起来，和字符串一样可以通过索引index和切片来访问某个元素或子列表。
		- 标志
			列表的标志符号是[]
			例如：menber=[a,b,c，1，2，3，"哈哈”]
- 基本操作
			- 创建
- 加法 乘法
- 列表相关方法
			- append（）
- pop()
- del()
- 拷贝(!!!)
				
![](https://img.mubu.com/document_image/fd88e645-87d1-4343-8b40-1e2c441a5c9c-2534569.jpg)
				new = old[:], 它复制列表old到new。它对于新手来说是种困惑而且应该避免使用这种方法。不幸的是[:]标记法被广泛使用，可能是Python程序员不知道更好的列表复制法吧。首先我们需要了解Python是如何管理对象和变量。Python没有C语言中的变量。在C语言中，变量不止是个名字，它是字节集合并真实存在于内存某个位置上。而在Python中，变量仅仅是指向对象的标签。
- 多维列表：列表里可以嵌套一个列表，这就形成了多维列表。
- tuple 元组
		元组的结构和访问、使用方法和列表基本一致，区别主要有两点：1.使用圆括号将各个数据项包括起来  ；2.元组里的值不可修改。
		- 标志-圆括号（）
			demotuple = (1,3,5,6,8,9) 定义元组，用圆括号
			print demotuple 只能读不能写
- 基本操作
			- 创建
				demotuple = (1,3,5,6,8,9) 定义元组，用圆括号
- 不可变性
				元组相当于一个只读的列表，它的元素不能修改，
- 多维元组：和多维列表相似，同时具备元组的相关特性
			同样也可以类似于二维数组
			tuple1 = ((1,2),(3,4),(5,6)) #定义多维元组
			print( tuple1) #打印整个二维元组
			print (tuple1[0]） #打印第一行
			print (tuple1[0][1]） #打印第一行第二个元素
- dict 字典
		- 标志
			字典的元素是由一对对键值对组成，每一对之间用逗号隔开，将所有的键值对用花括号括起来就构成了字典。它是无序的，键和值之间用冒号隔开。
			语法格式：dic_name={key:value,key:value,key:value}
			简单来说：
			　　1.类似于json,也就是key-value键值对
			　　2.不允许同一个键值出现两次，所以一般对字典的操作都是对键进行操作。
			　　3.键必须不可变，所以可以用数字，字符串，元组充当，而不能用列表
			例如：1 dic={"key1":"value1","key2":"value2","key3":"value3"} #定义字典​​
- 基础操作
			- 创建
				d0=dict() #创建空字典
				d1=​​{“key1”:"value1", "key2":"value2", "key3":"value3"}
- 增加、删除、修改、访问
- 遍历
- 方法
			- 复制
- 查找、更新
- 提升
	- 序列
		序列是Python中最基本的数据结构。序列中的每个元素都分配一个数字 - 它的位置，或索引，第一个索引是0，第二个索引是1，依此类推。
		Python有6个序列的内置类型，但最常见的是列表和元组。
		序列都可以进行的操作包括索引，切片，加，乘，检查成员。
		此外，Python已经内置确定序列的长度以及确定最大和最小的元素的方法。
		- 序列类型
			Python中的序列主要以下几种类型：
			3种基本序列类型(Basic Sequence Types)：list、tuple、range
			专门处理文本的附加序列类型(Text Sequence Types)：str
			专门处理二进制数据的附加序列类型(Binary Sequence Types): bytes、bytearray、memoryview
			按照序列是否可被改变分类：
			可变序列: list
			不可变序列：tuple、str
- 序列类型间的相互转换
			- 字符串、元组转变成列表 []
- 字符串、列表转变成元组 ()
- 列表、元组转变成字符串 “”
- 序列相关方法/操作
			- 通用序列操作
				
![](https://img.mubu.com/document_image/2eba668f-cdba-44c6-b063-1b6ec3b387c8-2534569.jpg)
				这里说的通用序列操作是指大部分可变序列与不可变序列都支持的操作。一般操作包括 增、删、改、查，但是这里说的是包括不可变序列也支持的通用操作，因此只能是“查”操作。
- 可变序列类支持的型操作
				
![](https://img.mubu.com/document_image/e79def76-10ba-4110-ba8f-f81fc6e18dcb-2534569.jpg)
				这里来说下可变序列类型支持，而不可变序列类型不支持的操作。在序列通用操作中主要说明的是“查”操作，这里要说的是可变序列的 "增"、“删”、“改”操作。
- 不可变序列支持的操作
				不可变序列类型通常实现而可变序列没有实现的唯一操作是对内建hash()方法的支持。对内建hash()的支持允许不可变序列（例如tuple）用作dict的键并存储在set和frozenset的实例中。如果尝试hash一个包含不可被hash的数据的不可变序列会导致TypeError错误。
- 运算符
		- 成员运算符：in, not in
- 身份运算符: is, is not
练习代码
```
# 作业1 ： 打印Hello World
# 直接打印
print("Hello World")
print("%s" % "Hello World")
print("{}".format("Hello World"))
# 作业2 ：
"""
定义一个列表，包含自己的家庭成员名字，
定义完成后在你名字前加入"大鱼AI"，再
将"大鱼AI"这一名字删除，此时判断"
大鱼AI"是否在列表中。
"""
# 定义列表
my_family = ["Dad", "Mom", "Me","Bro","Sis"]
my_family.insert(2, "FishAI")
print(my_family)
my_family.remove("FishAI")
print(my_family)
if "FishAI" in my_family:
    print("FishAI在我家")
else:
    print("FishAI不在我家")
```
![](https://img-blog.csdnimg.cn/20190222232109146.png)
![](https://img-blog.csdnimg.cn/2019022223212718.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
