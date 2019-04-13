
# xml解析技术及JDOM和DOM4J的比较 - 李昆鹏的博客 - CSDN博客


2018年07月04日 07:51:56[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：271


------------------------------------xml解析技术及JDOM和DOM4J的比较----------------------------------------
## 1.操作XML文档概述

### 1　如何操作XML文档
XML文档也是数据的一种，对数据的操作也不外乎是“增删改查”。也被大家称之为“CRUD”。
l  C：Create；
l  R：Retrieve；
l  U：Update；
l  D：Delete

### 2　XML解析技术
XML解析方式分为两种：DOM（Document Object Model）和SAX（Simple API for XML）。这两种方式不是针对Java语言来解析XML的技术，而是跨语言的解析方式。例如DOM还在Javascript中存在！
DOM是W3C组织提供的解析XML文档的标准接口，而SAX是社区讨论的产物，是一种事实上的标准。
DOM和SAX只是定义了一些接口，以及某些接口的缺省实现，而这个缺省实现只是用空方法来实现接口。一个应用程序如果需要DOM或SAX来访问XML文档，还需要一个实现了DOM或SAX的解析器，也就是说这个解析器需要实现DOM或SAX中定义的接口。提供DOM或SAX中定义的功能。

## 2.解析原理

### 1　DOM解析原理
使用DOM要求解析器把整个XML文档装载到一个Document对象中。Document对象包含文档元素，即根元素，根元素包含N多个子元素…
一个XML文档解析后对应一个Document对象，这说明使用DOM解析XML文档方便使用，因为元素与元素之间还保存着结构关系。
优先：使用DOM，XML文档的结构在内存中依然清晰。元素与元素之间的关系保留了下来！
缺点：如果XML文档过大，那么把整个XML文档装载进内存，可能会出现内存溢出的现象！
|Dom方式解析xml：把整个xml文档加载到内存中创建出来一个java文档对象
|缺点：如果xml文档非常大就无法加载
|优点：能记录文档的所有信息
|

### 2　设置Java最大内存
运行Java程序，指定初始内存大小，以及最大内存大小。
java -Xms20m -Xmx100mMyClass
![](https://img-blog.csdn.net/2018070407513415?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 3　SAX解析原理
SAX会一行一行的读取XML文档，最终会把XML文档所有数据存放到Document对象中。SAX也是一行一行的读取XML文档，但是当XML文档读取结束后，SAX不会保存任何数据，同时整个解析XML文档的工作也就结束了。
但是，SAX在读取一行XML文档数据后，就会给感兴趣的用户一个通知！例如当SAX读取到一个元素的开始时，会通知用户当前解析到一个元素的开始标签。而用户可以在整个解析的过程中完成自己的业务逻辑，当SAX解析结束，不会保存任何XML文档的数据。
优先：使用SAX，不会占用大量内存来保存XML文档数据，效率也高。
缺点：当解析到一个元素时，上一个元素的信息已经丢弃，也就是说没有保存元素与元素之间的结构关系，这也大大限制了SAX的使用范围。如果只是想查询XML文档中的数据，那么使用SAX是最佳选择！
|Sax方式解析xml：以事件驱动，按着元素来加载文档
|优点：不需要把整个文档加载到内存，所有不受文档大小的限制
|缺点：不能保存解析过得完整信息
|

## 3. JDOM和
## DOM4J

### 1　JDOM和DOM4J概述
JDOM和DOM4J都是针对Java解析XML设计的方式，它们与DOM相似。但DOM不是只针对Java，DOM是跨语言的，DOM在Javascript中也可以使用。而JDOM和DOM4J都是专业为Java而设计的，使用JDOM和DOM4J，对Java程序员而言会更加方便。

### 2　JDOM和DOM4J比较
JDOM与DOM4J相比，DOM4J完胜！！！所以，我们应该在今后的开发中，把DOM4J视为首选。
在2000年，JDOM开发过程中，因为团队建议不同，分离出一支队伍，开发了DOM4J。DOM4J要比JDOM更加全面。

### 3　DOM4J查找解析器的过程
DOM4J首先会去通过JAXP的查找方法去查找解析器，如果找到解析器，那么就使用之；否则会使用自己的默认解析器Aelfred2。
DOM4J对DOM和SAX都提供了支持，可以把DOM解析后的Document对象转换成DOM4J的Document对象，当然了可以把DOM4J的Document对象转换成DOM的Document对象。
DOM4J使用SAX解析器把XML文档加载到内存，生成DOM对象。当然也支持事件驱动的方式来解析XML文档。


