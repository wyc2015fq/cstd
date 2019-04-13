
# xml文档声明及基本语法 - 李昆鹏的博客 - CSDN博客


2018年07月02日 10:47:41[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：1434标签：[xml																](https://so.csdn.net/so/search/s.do?q=xml&t=blog)个人分类：[XML																](https://blog.csdn.net/weixin_41547486/article/category/7771052)


---------------------------------------xml文档声明及基本语法-------------------------------------------
## XML文档声明

### 1　什么是xml文档声明
可以把xml文档声明看成是xml文档说明。
最简单的xml文档声明：<?xml version="1.0"?>
**注意**，XML是区别大小写，这一点不同与HTML！

### 2　xml文档声明结构
l  version属性：用于说明当前xml文档的版本，因为都是在用1.0，所以这个属性值大家都写1.0，version属性是必须的；
l  encoding属性：用于说明当前xml文档使用的字符编码集，xml解析器会使用这个编码来解析xml文档。encoding属性是可选的，默认为UTF-8。注意，如果当前xml文档使用的字符编码集是gb2312，而encoding属性的值为UTF-8，那么一定会出错的；
l  standalone属性：用于说明当前xml文档是否为独立文档，如果该属性值为yes，表示当前xml文档是独立的，如果为no表示当前xml文档不是独立的，即依赖外部的约束文件。默认是yes
l  没有xml文档声明的xml文档，不是格式良好的xml文档；
lxml文档声明必须从xml文档的1行1列开始。
------------------------------------------------------------------------------------------
## XML
## 元素

### 1　XML元素的格式1
l  xml元素包含：开始标签、元素体（内容）、结束标签。例如：<hello>大家好</hello>
l  空元素：空元素只有开始标签，没有元素体和结束标签，但空元素一定要闭合。例如：<hello/>

### 2　XML元素的格式2
l  xml元素可以包含子元素或文本数据。例如：<a><b>hello</b></a>，a元素的元素体内容是b元素，而b元素的元素体内容是文本数据hello。
l  xml元素可以嵌套，但必须是合法嵌套。例如：<a><b>hello<a></b>就是**错误**的嵌套。

### 3　XML文档的根元素
**格式良好**的xml文档必须且仅有一个根元素！
student1.xml
|<?xml version="1.0"  encoding="utf-8" standalone="no"?>
|<students>
|<student  number="1001">
|<name>zhangSan</name>
|<age>23</age>
|<sex>male</sex>
|</student>
|<student  number="1002">
|<name>liSi</name>
|<age>32</age>
|<sex>female</sex>
|</student>
|</students>
|
student2.xml
|<?xml version="1.0"  encoding="utf-8" standalone=|"|no"?>
|<student number="1001">
|<name>zhangSan</name>
|<age>23</age>
|<sex>male</sex>
|</student>
|<student number="1002">
|<name>liSi</name>
|<age>32</age>
|<sex>female</sex>
|</student>
|
student1.xml是格式良好的xml文档，只有一个根元素，即students元素。
student2.xml不是格式良好的xml文档，有两个根元素，即两个student根元素。

### 4　元素中的空白
xml元素的元素体可以包含文本数据和子元素。
a.xml
|<a><b>hello</b></a>
|
b.xml
|<a>
|<b>
|hello
|</b>
|</a>
|
a.xml中，<a>元素只有一个子元素，即<b>元素。<b>元素只有一个文本数据，即hello。
b.xml中，<a>元素中第一部分为换行缩进，第二部分为<b>元素，第三部分为换行。b元素的文本数据为换行、缩进、hello、换行、缩进。
其中换行和缩进都是空白，这些空白是为了增强xml文档的可读性。但xml解析器可能会因为空白出现错误的解读，这说明在将来编写解析xml程序时，一定要小心空白。

### 5　元素命名规范
xml元素名可以包含字母、数字以及一些其它可见字符，但必须遵循下面的一些规范：
l  区分大小写：<a>和<A>是两个元素；
l  不能以数字开头：<1a>都是错误的；
l  最好不要以xml开头：<xml>、<Xml>、<XML>；
l  不能包含空格；
### 6. 元素属性
l  属性由属性名与属性值构成，中间用等号连接；
l  属性值必须使用引号括起来，单引或双引；
l  定义属性必须遵循与标签名相同的命名规范；
l  属性必须定义在元素的开始标签中；
l  一个元素中不能包含相同的属性名；
### 7. 注释
l  注释以<!--开头，以-->结束；
l  注释中不能包含--；

### 8.转义字符和CDATA区
因为在xml文档中有些字符是特殊的，不能使用它们作为文本数据。例如：不能使用“<”或“>”等字符作为文本数据，所以需要使用转义字符来表示。
例如<a><a></a>，你可能会说，其中第二个<a>是a元素的文本内容，而不是一个元素的开始标签，但xml解析器是不会明白你的意思的。
把<a><a></a>修饰为<a>&lt;a&gt;</a>，这就OK了。
![](https://img-blog.csdn.net/2018070210455815?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
转义字符都是以“&”开头，以“;”结束。这与后面我们学习的实体是相同的。

### 9. CDATA区（CDATA段）
当大量的转义字符出现在xml文档中时，会使xml文档的可读性大幅度降低。这时如果使用CDATA段就会好一些。
在CDATA段中出现的“<”、“>”、“””、“’”、“&”，都无需使用转义字符。这可以提高xml文档的可读性。
|<a>|<![CDATA[|<a>|]]>|</a>
|
在CDATA段中不能包含“]]>”，即CDATA段的结束定界符。
代码示例：
|<?xml version="1.0"  encoding="UTF-8"?>
|<|students|>
|<|student|stuno|="|01|">
|<|name|>|张三|</|name|>
|<|age|>|23|</|age|>
|<|gender|>|男|</|gender|>
|</|student|>
|<!--|注释中不能有两横线|-->
|<|student|stuno|="|02|">
|<|name|>|张三|</|name|>
|<|age|>|age&lt;23|</|age|>|<!--|年龄小于23|-->
|<|gender|>|男|</|gender|>
|</|student|>
|<!--|定义cdata|区|-->
|<![CDATA[
|<student  stuno="01'">
|<name>|张三</name>
|<age>age<23</age>
|<gender>|男</gender>
|</student>
|]]>
|</|students|>
|


