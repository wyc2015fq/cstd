
# xml约束的概述和DTD的引入 - 李昆鹏的博客 - CSDN博客


2018年07月02日 10:50:56[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：159


--------------------------------------------xml约束的概述和DTD的引入-----------------------------------------
# XML约束
## 1. XML约束概述
一个XML文档一旦有了约束，那么这个XML文档就只能使用约束中创建的元素及属性。如果约束没有创建<a>元素，那么XML文档就不能使用<a>元素！！！
### 1　什么是xml约束
因为xml文档是可以自定义元素的，这会让使用xml文档的应用程序无法知道xml文档的真实结构。通常应用程序都会要求xml文件的结构是固定的，以满足自己的需求，这就说明，不同的应用程序要求自己的xml文档必须符合一定的要求。
例如，当使用xml文档作为某个Java swing应用程序的配置文件时，要求xml文档结构如下：
|<frame title="test xml"  layout="java.awt.BorderLayout">
|<bgcolor>
|<red>200</red>
|<green>0</green>
|<blue>0</blue>
|</bgcolor>
|<size>
|<width>300</width>
|<heigth>200</heigth>
|</size>
|<content>
|<label>
|<text>hello xml</text>
|<label>
|</content>
|</frame>
|
当某个学生管理系统程序需要使用xml文档作为数据库时，要求xml文档结构如下：
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
|<student  number="1003">
|<name>wangWu</name>
|<age>55</age>
|<sex>male</sex>
|</student>
|</students>
|
xml文档约束是用另一个文件来约束xml文档的结构，例如要求xml文档的根元素必须是<students>，在<students>元素下可以包含0~n个<student>元素，每个<student>元素必须有一个number属性，而且还要有三个子元素，分别为<name>、<age>、<sex>，这三个子元素的内容必须为文本内容，而不能是子元素。

### 2　XML文档约束的类型
xml文档约束有两种类型：dtd和schema
l  dtd：dtd是平面式文档，dtd文件不是xml文档，通常扩展名为“.dtd”。它是最早的xml约束；
l  schema：schema本身也是xml文档，它比dtd要更加强大，通常扩展名为“.xsd”。它是新的xml文档约束，用来替代dtd。
DTD是老的XML约束
Schema是新的，用Schema替换掉DTD
**会读即可！**

## 2. DTD

### 1　什么是DTD
DTD（Document TypeDefinition），文档类型定义，用来约束XML文档。例如要求xml文档的根元素必须是<students>，在<students>元素下可以包含0~n个<student>元素，每个<student>元素必须有一个number属性，而且还要有三个子元素，分别为<name>、<age>、<sex>，这三个子元素的内容必须为文本内容，而不能是子元素。

### 2　DTD展示
|<!ELEMENT students (student+)>
|<!ELEMENT student (name,age,sex)>
|<!ELEMENT name (\#PCDATA)>
|<!ELEMENT age (\#PCDATA)>
|<!ELEMENT sex (\#PCDATA)>
|
解读上面DTD：
l  students元素中可以包含1~n个student元素；
l  student元素中必须包含name、age、sex元素，而且顺序也是固定的；
l  name元素内容为文本，即字符串；
l  age元素内容为文本；
l  sex元素内容为文本。
代码示例
|<?xml version="1.0"  encoding="UTF-8" standalone="no"?>
|<!DOCTYPE students [
|<!ELEMENT  students (student+)>
|<!ELEMENT  student (name, age, gender)>
|<!ELEMENT name  (\#PCDATA)>
|<!ELEMENT age  (\#PCDATA)>
|<!ELEMENT  gender (\#PCDATA)>
|]>
|<|students|>
|<|student|>
|<|name|>|张三|</|name|>
|<|age|>|20|</|age|>
|<|gender|>|男|</|gender|>
|</|student|>
|<|student|>
|<|name|>|李四|</|name|>
|<|age|>|21|</|age|>
|<|gender|>|男|</|gender|>
|</|student|>
|</|students|>
|

### 3　在XML文档中指定本地DTD
|<?xml version="1.0"  encoding="utf-8" standalone="no" ?>
|<!DOCTYPE students  SYSTEM "students.dtd">
|<students>
|<student>
|<name>zhangSan</name>
|<age>23</age>
|<sex>male</sex>
|</student>
|</students>
|
|<!ELEMENT students (student+)>
|<!ELEMENT student (name, age, sex)>
|<!ELEMENT name (\#PCDATA)>
|<!ELEMENT age (\#PCDATA)>
|<!ELEMENT sex (\#PCDATA)>
|
其中<!DOCTYPEstudents SYSTEM "students.dtd">，表示指定dtd文件。
指定DTD的语法：<!DOCTYPE 根元素 SYSTEM "DTD文件路径">
l  指定DTD的语法，以“<!DOCTYPE”开头，以“>”结束；
l  students表示根元素；
l  SYSTEM表示dtd文件在本地；
l  "students.dtd"表示DTD文件路径。
**DTD****中的顺序是不能颠倒使用的**
|<?xml version="1.0"  encoding="UTF-8"?>
|<!DOCTYPE students SYSTEM "demo1.dtd">
|<|students|>
|<|student|>
|<|name|/>
|<|age|/>
|<|gender|/>
|</|student|>
|</|students|>
|

### 4　在XML文档中指定内部DTD
|<?xml version="1.0"  encoding="utf-8" standalone="no" ?>
|<!DOCTYPE students [
|<!ELEMENT students  (student+)>
|<!ELEMENT student  (name, age, sex)>
|<!ELEMENT name  (\#PCDATA)>
|<!ELEMENT age  (\#PCDATA)>
|<!ELEMENT sex  (\#PCDATA)>
|]>
|<students>
|<student>
|<name>zhangSan</name>
|<age>23</age>
|<sex>male</sex>
|</student>
|</students>
|

### 5　在XML文档中指定外部公共DTD
|<?xml version="1.0"  encoding=|"|utf-8"  standalone="no" ?>
|<!DOCTYPE students  PUBLIC "|-//rlteach//DTD ST 1.0//ZH"|"http://www.rlteach.com/xml/dtds/st.dtd">
|<students>
|<student>
|<name>zhangSan</name>
|<age>23</age>
|<sex>male</sex>
|</student>
|</students>
|
代码示例：
|<?xml version="1.0"  encoding="UTF-8"?>
|<!DOCTYPE configuration
|PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
|"http://mybatis.org/dtd/mybatis-3-config.dtd">
|<|configuration|>
|<|environments|default|="">
|<|environment|id|="">
|<|transactionManager|type|=""></|transactionManager|>
|<|dataSource|type|=""></|dataSource|>
|</|environment|>
|</|environments|>
|</|configuration|>
|
外部公共DTD是说，DTD由某个公司或权威组织发布，例如如下：
<!DOCTYPE 根元素 PUBLIC "DTD名称" "DTD网址">
当然你需要知道要使用的DTD的名称和网址。如果某个机构公布了DTD，那么一定也会公布DTD的名称和网址。
上面例子中的公共DTD是不存在的。
在eclipse中的配置
![](https://img-blog.csdn.net/20180702105010644?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180702105020402?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180702105030683?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
重新开启配置文件即可。

