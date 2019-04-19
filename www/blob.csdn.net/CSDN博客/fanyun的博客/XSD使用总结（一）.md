# XSD使用总结（一） - fanyun的博客 - CSDN博客
2019年03月24日 22:02:10[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：27
XML Schema 是基于 XML 的 DTD 替代者。XML Schema 描述 XML 文档的结构。XML Schema 语言也称作 XML Schema 定义（XML Schema Definition，XSD）。
## 一、XML Schema 简介
### 1、什么是 XML Schema？
XML Schema 的作用是定义 XML 文档的合法构建模块，类似 DTD。
XML Schema:
- 定义可出现在文档中的元素
- 定义可出现在文档中的属性
- 定义哪个元素是子元素
- 定义子元素的次序
- 定义子元素的数目
- 定义元素是否为空，或者是否可包含文本
- 定义元素和属性的数据类型
- 定义元素和属性的默认值以及固定值
### 2、XML Schema 是 DTD 的继任者
我们认为 XML Schema 很快会在大部分网络应用程序中取代 DTD。
理由如下：
- XML Schema 可针对未来的需求进行扩展
- XML Schema 更完善，功能更强大
- XML Schema 基于 XML 编写
- XML Schema 支持数据类型
- XML Schema 支持命名空间
### 3、XML Schema 是 W3C 标准
XML Schema 在 2001 年 5 月 2 日成为 W3C 标准。
## 二、为什么要使用 XML Schema？
### 1、XML Schema 比 DTD 更强大 
### 2、XML Schema 支持数据类型
XML Schema 最重要的能力之一就是对数据类型的支持。
通过对数据类型的支持：
- 可更容易地描述允许的文档内容
- 可更容易地验证数据的正确性
- 可更容易地与来自数据库的数据一并工作
- 可更容易地定义数据约束（data facets）
- 可更容易地定义数据模型（或称数据格式）
- 可更容易地在不同的数据类型间转换数据
编者注：数据约束，或称 facets，是 XML Schema 原型中的一个术语，中文可译为“面”，用来约束数据类型的容许值。
### 3、XML Schema 使用 XML 语法
另一个关于 XML Schema 的重要特性是，它们由 XML 编写。
由 XML 编写 XML Schema 有很多好处：
- 不必学习新的语言
- 可使用 XML 编辑器来编辑 Schema 文件
- 可使用 XML 解析器来解析 Schema 文件
- 可通过 XML DOM 来处理 Schema
- 可通过 XSLT 来转换 Schema
### 4、XML Schema 可保护数据通信
当数据从发送方被发送到接受方时，其要点是双方应有关于内容的相同的“期望值”。
通过 XML Schema，发送方可以用一种接受方能够明白的方式来描述数据。
一种数据，比如 "03-11-2004"，在某些国家被解释为11月3日，而在另一些国家为当作3月11日。
但是一个带有数据类型的 XML 元素，比如：<date type="date">2004-03-11</date>，可确保对内容一致的理解，这是因为 XML 的数据类型 "date" 要求的格式是 "YYYY-MM-DD"。
### 5、XML Schema 可扩展
XML Schema 是可扩展的，因为它们由 XML 编写。
通过可扩展的 Schema 定义，您可以：
- 在其他 Schema 中重复使用您的 Schema
- 创建由标准类型衍生而来的您自己的数据类型
- 在相同的文档中引用多重的 Schema
## 三、如何使用 XSD？
### 1、XML Schema及其引用
下面这个例子是一个名为 "note.xsd" 的 XML Schema 文件：
```
<?xml version="1.0"?>
<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema"
targetNamespace="http://www.w3school.com.cn"
xmlns="http://www.w3school.com.cn"
elementFormDefault="qualified">
<xs:element name="note">
    <xs:complexType>
      <xs:sequence>
    <xs:element name="to" type="xs:string"/>
    <xs:element name="from" type="xs:string"/>
    <xs:element name="heading" type="xs:string"/>
    <xs:element name="body" type="xs:string"/>
      </xs:sequence>
    </xs:complexType>
</xs:element>
</xs:schema>
```
 note 元素是一个复合类型，因为它包含其他的子元素。其他元素 (to, from, heading, body) 是简易类型，因为它们没有包含其他元素。您将在下面的章节学习更多有关复合类型和简易类型的知识。
以下文件包含对 XML Schema 的引用：
```
<?xml version="1.0"?>
<note
xmlns="http://www.w3school.com.cn"
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
xsi:schemaLocation="http://www.w3school.com.cn note.xsd">
<to>George</to>
<from>John</from>
<heading>Reminder</heading>
<body>Don't forget the meeting!</body>
</note>
```
### 2、<schema> 元素
<schema> 元素是每一个 XML Schema 的根元素：
```
<?xml version="1.0"?>
<xs:schema>
...
...
</xs:schema>
```
 <schema> 元素可包含属性。一个 schema 声明往往看上去类似这样：
```
<?xml version="1.0"?>
 
<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema"
targetNamespace="http://www.w3school.com.cn"
xmlns="http://www.w3school.com.cn"
elementFormDefault="qualified">
...
...
</xs:schema>
```
- xmlns:xs：指当前xsd所要遵循的标签规范，一般是http://www.w3.org/2001/XMLSchema
- xmlns：一般值为targetNamespace，作用是注明文件内引用类型时的命名空间，防止与其他xsd混淆
- xmlns:?：引用其它命名空间，一般要与import同时使用，之后可以在文件内调用其它文件的类型
- targetNamespace：当前xsd的命名空间，由作者自定义，供XML调用
- elementFormDefault（qualified/unqualified）：使用qualified即可，配置更清晰简单
- attributeFormDefault（qualified/unqualified）
参考：
[https://www.downloadcollection.com/software/xml-xsd-tool.htm](https://www.downloadcollection.com/software/xml-xsd-tool.htm)
[http://www.stylusstudio.com/xml-download.html](http://www.stylusstudio.com/xml-download.html)
[http://www.w3school.com.cn/schema/index.asp](http://www.w3school.com.cn/schema/index.asp)
[http://www.apache.org/dist/xerces/c/3/sources/](http://www.apache.org/dist/xerces/c/3/sources/)
[http://xerces.apache.org/xerces-c/](http://xerces.apache.org/xerces-c/)
[http://mirrors.shu.edu.cn/apache//xerces/c/3/](http://mirrors.shu.edu.cn/apache//xerces/c/3/)
