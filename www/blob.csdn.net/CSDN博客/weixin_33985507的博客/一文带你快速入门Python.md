# 一文带你快速入门Python - weixin_33985507的博客 - CSDN博客
2018年10月15日 15:38:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
作者：语法糖Quant
个人公众号： Python与算法之美
**一，输入输出**
输入：input(输入的都是字符串)
输出：print
![3901436-fd02ee253e581369](https://upload-images.jianshu.io/upload_images/3901436-fd02ee253e581369)
image
**二，导入模块**
import ...
或 from ... import ...
或 import ... as ...
![3901436-3568cc3405683472](https://upload-images.jianshu.io/upload_images/3901436-3568cc3405683472)
image
**三，基础语法**
**1，标识符**
标识符由字母、数字、下划线组成，区分大小写，不能以数字开头。
以下划线开头的标识符有特殊含义。以单下划线开头的（_foo）的代表不能直接访问的类属性，以双下划线开头的（__foo）代表类的私有成员；以双下划线开头和结尾的(**foo**)代表Python里特殊方法专用的标识，如**init**() 代表类的构造函数。
**2，缩进**
Python的代码块不使用大括号来控制类、函数、以及其他逻辑判断，而是使用缩进来写实现代码分组。通常用四个空格来进行缩进。
**3，注释**
python中单行注释采用 # 开头。
多行注释使用三个单引号(''')或三个双引号(""")。
**4，一条语句分多行显示**
Python语句中一般以新行作为为语句的结束符。
但是我们可以使用斜杠（ \）将一行的语句分为多行显示，如下所示：
![3901436-6f19f4d0e2117bce](https://upload-images.jianshu.io/upload_images/3901436-6f19f4d0e2117bce)
image
语句中包括[],{}或()括号就不需要使用多行连接符。
![3901436-7ce5524717cdb10c](https://upload-images.jianshu.io/upload_images/3901436-7ce5524717cdb10c)
image
**5，同一行显示多条语句**
Python可以在同一行中使用多条语句，语句之间使用分号分割。
**四，数据结构**
python内建的数据结构有列表，元组，字符串，字典，集合等。此外常用的还有numpy中的array，以及pandas中的dataframe和series。
**1，有序数据结构：**
List（列表），是有序集合，没有固定大小，可以通过对偏移量以及其他方法修改列表大小。列表的基本形式如：[1,2,3,4]
Tuple（元组），是有序集合，是不可变的，可以进行组合和复制运算后会生成一个新的元组。元组的基本形式比如：(1,3,6,10)
String（字符串），也是有序集合，字符串的基本形式比如：’hello'。
**2，无序数据结构：**
Set（集合），是一个无序不重复元素的集。
基本功能包括关系运算和消除重复元素。集合的基本形式如：
{'apple','orange','banana'}
Dictionary（字典）是无序的键：值对 (key:value 对)的集合。
键必须是互不相同的(在同一个字典之内)。
字典的基本形式如：{'jack': 4098, 'sape': 4139}
**五，列表**
**1，建立列表**
![3901436-e5984fcf8a5b08a8](https://upload-images.jianshu.io/upload_images/3901436-e5984fcf8a5b08a8)
image
**2，访问列表**
可以使用下标访问和切片访问
![3901436-795658ab57307943](https://upload-images.jianshu.io/upload_images/3901436-795658ab57307943)
image
**3，修改列表**
![3901436-a778f8ff7a683b45](https://upload-images.jianshu.io/upload_images/3901436-a778f8ff7a683b45)
image
**4，列表常用函数**
![3901436-ac2a22033f6707bc](https://upload-images.jianshu.io/upload_images/3901436-ac2a22033f6707bc)
image
**5，列表常用方法**
![3901436-d5d3d67de1c50fd9](https://upload-images.jianshu.io/upload_images/3901436-d5d3d67de1c50fd9)
image
**六，字典**
**1，创建字典**
![3901436-4bce1c02de82fb4f](https://upload-images.jianshu.io/upload_images/3901436-4bce1c02de82fb4f)
image
**2，常用字典操作方法**
![3901436-602f200b5ea8d65b](https://upload-images.jianshu.io/upload_images/3901436-602f200b5ea8d65b)
image
**七，元组**
**1，创建元组**
![3901436-f9c5bc11ef97ce24](https://upload-images.jianshu.io/upload_images/3901436-f9c5bc11ef97ce24)
image
**2，修改元组**
![3901436-c81a6fc70b8875ad](https://upload-images.jianshu.io/upload_images/3901436-c81a6fc70b8875ad)
image
**八，字符串**
****1，创建字符串****
可以用单引号，双引号，或者三重引号创建字符串。
(\n表示换行符，\t表示制表符)
![3901436-20663891275ae82c](https://upload-images.jianshu.io/upload_images/3901436-20663891275ae82c)
image
****2，字符串基本操作****
![3901436-e1046fdaab7ddd1c](https://upload-images.jianshu.io/upload_images/3901436-e1046fdaab7ddd1c)
image
![3901436-895446d33e0ad1e3](https://upload-images.jianshu.io/upload_images/3901436-895446d33e0ad1e3)
image
![3901436-07d18e3aa6c3e8a4](https://upload-images.jianshu.io/upload_images/3901436-07d18e3aa6c3e8a4)
image
****3，格式化字符串****
![3901436-04a2ac52e07ccf18](https://upload-images.jianshu.io/upload_images/3901436-04a2ac52e07ccf18)
image
**九，条件语句**
**1，多分支结构**
python中不支持switch语句，只能用if...(elif...)else...来实现多分支选择结构。此外，还可以巧妙地通过字典数据结构实现多分支选择结构。
![3901436-1130dad144b511ef](https://upload-images.jianshu.io/upload_images/3901436-1130dad144b511ef)
image
![3901436-f9042e59738969f6](https://upload-images.jianshu.io/upload_images/3901436-f9042e59738969f6)
image
**2，and 和 or**
python中的与、或、非分别用关键字 and，or，not 表示。python做逻辑运算时把空的列表、元组、集合等当做False。对or而言，Python会由左到右求算操作对象，然后返回第一个为真的操作对象。Python会在其找到的第一个真值操作数的地方停止，通常叫短路计算。and 会停在第一个为假的对象上。
![3901436-40d7bc138dac5942](https://upload-images.jianshu.io/upload_images/3901436-40d7bc138dac5942)
image
**十，循环语句**
Python提供了for循环和while循环（在Python中没有do...while循环）。
for循环一般比while计数器循环运行得更快。
break语句，在语句块执行过程中终止循环，并且跳出整个循环。
continue语句，在语句块执行过程中终止当前循环，跳出该次循环，执行下一次循环。
pass语句，是空语句，是为了保持程序结构的完整性。不做任何事情，一般用做占位语句。俗称代码桩。
![3901436-9cdef9b32eeb3aad](https://upload-images.jianshu.io/upload_images/3901436-9cdef9b32eeb3aad)
image
**十一，函数**
**1，定义函数**
使用 def关键字定义函数，用return返回函数值，
若没有return，返回值为None。
![3901436-af86fd2b0d178318](https://upload-images.jianshu.io/upload_images/3901436-af86fd2b0d178318)
image
**2，默认参数**
默认参数放在普通参数后面
![3901436-4494d1ca5e5a510e](https://upload-images.jianshu.io/upload_images/3901436-4494d1ca5e5a510e)
image
**3，可变参数**
可变参数就是传入的参数个数是可变的，可以是1个、2个到任意个，还可以是0个。可以用 *k 的方式定义可变参数。可变参数传入函数后自动组装为一个tuple。
![3901436-459fbaca22e8812a](https://upload-images.jianshu.io/upload_images/3901436-459fbaca22e8812a)
image
![3901436-07ad67467f661765](https://upload-images.jianshu.io/upload_images/3901436-07ad67467f661765)
image
**4，关键字参数**
关键字参数允许你传入 0 个或任意个含参数名的参数，
这些关键字参数在函数内部自动组装为一个 dict。
![3901436-8516bf19407c3d96](https://upload-images.jianshu.io/upload_images/3901436-8516bf19407c3d96)
image
**5，混合参数**
![3901436-553890807f9b08ee](https://upload-images.jianshu.io/upload_images/3901436-553890807f9b08ee)
image
**6，递归函数**
递归可以代替循环，很多时候会更加简洁。
![3901436-4c4ccb54a5be35ab](https://upload-images.jianshu.io/upload_images/3901436-4c4ccb54a5be35ab)
image
**7，装饰器**
装饰器可以在不改变函数代码前提下赋予你的函数额外的功能。
![3901436-42150b1467b5cc8a](https://upload-images.jianshu.io/upload_images/3901436-42150b1467b5cc8a)
image
![3901436-4ba85d8a185457ae](https://upload-images.jianshu.io/upload_images/3901436-4ba85d8a185457ae)
image
![3901436-22c6e2fda68896e9](https://upload-images.jianshu.io/upload_images/3901436-22c6e2fda68896e9)
image
**十二，lamda 匿名函数**
lambda只是一个表达式，适合定义较为简单的函数。
lambda函数拥有自己的名字空间，且不能访问自有参数列表之外或全局名字空间里的参数。
lamda函数定义语法是:
**fun = lamda 参数序列: 返回值表达式**
**一般来说通过使用lamda函数可以节约程序开支并加快运行速度。**
![3901436-48fbd111d8f5f4ac](https://upload-images.jianshu.io/upload_images/3901436-48fbd111d8f5f4ac)
image
**十三，迭代器**
迭代器在python中是以C语言的速度运行的，常用的迭代器如下。
![3901436-8e8fec37898cbd33](https://upload-images.jianshu.io/upload_images/3901436-8e8fec37898cbd33)
image
![3901436-9b74a7e62e53fa54](https://upload-images.jianshu.io/upload_images/3901436-9b74a7e62e53fa54)
image
![3901436-c3b721c200c4ad1d](https://upload-images.jianshu.io/upload_images/3901436-c3b721c200c4ad1d)
image
**十四，Python 推导式**
Python中的推导式是Python语言所有语法规则中最大的一个彩蛋，简直是一个超级语法糖。淋漓尽致地体现了Python语言 simple，readable 和 powerful 的特点。一旦掌握则爱不释手，欲罢不能，永生难忘。Python推导式可以生成列表，集合和字典。
**1，列表推导式**
![3901436-754a253c8ccce438](https://upload-images.jianshu.io/upload_images/3901436-754a253c8ccce438)
image
![3901436-2d117f763959152c](https://upload-images.jianshu.io/upload_images/3901436-2d117f763959152c)
image
**2，字典推导式**
![3901436-8cab910756152d8c](https://upload-images.jianshu.io/upload_images/3901436-8cab910756152d8c)
image
**3，集合推导式**
![3901436-38535a8b339b5946](https://upload-images.jianshu.io/upload_images/3901436-38535a8b339b5946)
image
**十五，Python类和对象**
在Python中，万物皆为对象。对象由类创建而来，所有的类都是object基类的子类。
**1，类和对象基本概念**
类： class， 抽象数据结构，数据和算法的封装。如：定义一个类，dog。
对象： object，类的实例。如：dog类的一个实例，点点 dot。
属性：properties，和对象关联的数据部分。如：weight 体重，breed 品种。
方法：methods，和对象关联的算法部分。如：run(),  eat(), bark()。
**2，创建类和对象**
![3901436-5e858d275fab72ce](https://upload-images.jianshu.io/upload_images/3901436-5e858d275fab72ce)
image
![3901436-a639e7779a7b8b7b](https://upload-images.jianshu.io/upload_images/3901436-a639e7779a7b8b7b)
image
**3，获取对象信息**
![3901436-4ac88e334293d799](https://upload-images.jianshu.io/upload_images/3901436-4ac88e334293d799)
image
![3901436-6395422100ebbd47](https://upload-images.jianshu.io/upload_images/3901436-6395422100ebbd47)
image
**[图片上传失败...(image-a4631e-1539589025610)]**
**Python爱好者社区历史文章大合集**：
[Python爱好者社区历史文章列表（每周append更新一次）](http://mp.weixin.qq.com/s?__biz=MzI5NDY1MjQzNA==&mid=2247487172&idx=1&sn=61285c5949876c54bf5d5777e0b64474&chksm=ec5ed1b9db2958afe85014443a4919e5c857569f88b5c69dda0d2b31571e8e6e8b41a00a27f7&scene=21#wechat_redirect)
![3901436-ab987c07c90f508d](https://upload-images.jianshu.io/upload_images/3901436-ab987c07c90f508d)
image
福利：文末扫码立刻关注公众号，**“Python爱好者社区”**，开始学习Python课程：
关注后在公众号内回复**“**课程**”**即可获取：
**小编的Python入门免费视频课程****！！！**
**【最新免费微课】小编的Python快速上手matplotlib可视化库！！！**
崔老师爬虫实战案例免费学习视频。
陈老师数据分析报告制作免费学习视频。
玩转大数据分析！Spark2.X+Python 精华实战课程免费学习视频。
