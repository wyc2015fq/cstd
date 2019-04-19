# qt 之 XML - baidu_33570760的博客 - CSDN博客
2017年05月04日 11:25:53[carman_风](https://me.csdn.net/baidu_33570760)阅读数：183
个人分类：[qt](https://blog.csdn.net/baidu_33570760/article/category/6898155)
转载自：http://blog.csdn.net/liang19890820/article/details/52805902
# 简述
XML（EXtensible Markup Language - 可扩展标记语言）是一种用于记录多种数据类型的标记语言。使用 XML 可以将各类型的文档定义为容易读取的格式，便于用户读取。而且，在应用程序中使用 XML，可以轻松实现数据交换。
- [简述](http://blog.csdn.net/liang19890820/article/details/52805902#%E7%AE%80%E8%BF%B0)
- [使用 XML 模块](http://blog.csdn.net/liang19890820/article/details/52805902#%E4%BD%BF%E7%94%A8-xml-%E6%A8%A1%E5%9D%97)
- [XML 的访问方式](http://blog.csdn.net/liang19890820/article/details/52805902#xml-%E7%9A%84%E8%AE%BF%E9%97%AE%E6%96%B9%E5%BC%8F)- [DOM](http://blog.csdn.net/liang19890820/article/details/52805902#dom)
- [SAX](http://blog.csdn.net/liang19890820/article/details/52805902#sax)- [SAX2 介绍](http://blog.csdn.net/liang19890820/article/details/52805902#sax2-%E4%BB%8B%E7%BB%8D)
- [XML 流](http://blog.csdn.net/liang19890820/article/details/52805902#xml-%E6%B5%81)- [QXmlStreamReader](http://blog.csdn.net/liang19890820/article/details/52805902#qxmlstreamreader)
- [QXmlStreamWriter](http://blog.csdn.net/liang19890820/article/details/52805902#qxmlstreamwriter)
- [使用哪种方式](http://blog.csdn.net/liang19890820/article/details/52805902#%E4%BD%BF%E7%94%A8%E5%93%AA%E7%A7%8D%E6%96%B9%E5%BC%8F)
- [更多参考](http://blog.csdn.net/liang19890820/article/details/52805902#%E6%9B%B4%E5%A4%9A%E5%8F%82%E8%80%83)
# 使用 XML 模块
要链接到 XML 模块，需要在 qmake 项目文件 .pro 中添加：
`QT += xml```
包含模块中类的定义，使用：
`#include <QtXml>```
# XML 的访问方式
Qt 提供了两种访问 XML 文档的方式：DOM 和 SAX。
- 
DOM 方式：将 XML 文档转换为树形结构存储到内存中，再进行读取，消耗的内存比较多。此外，由于文档都已经存储到内存，所以需要频繁实现修改等操作时，使用起来比较方便。
- 
SAX 方式：相比于 DOM，SAX 是一种速度更快，更有效的方法，它逐行扫描文档，一边扫描一边解析（由于应用程序只是在读取数据时检查数据，因此不需要将数据存储在内存中，这对于大型文档的解析是个巨大优势）。而且相比于 DOM，SAX 可以在解析文档的任意时刻停止解析。但操作复杂，很难修改 XML 数据。
## DOM
DOM Level 2 是用于 XML 接口的一个 W3C 推荐标准，将 XML 文档的组成映射为一个树结构。DOM Level 2 的规范可以在 [http://www.w3.org/DOM/](http://www.w3.org/DOM/) 上找到。
DOM 提供了一个访问和更改 XML 文件内容和结构的接口。它创建了一个文档的分层视图（树视图），因此，与 SAX2 接口相反，在解析之后，文档的对象模型常驻在内存中，这使得操作变得更加容易。
文档树中的所有 DOM 节点都是 QDomNode 的子类，文档本身由 QDomDocument 对象表示。
Qt 为 DOM 提供了用于操作 XML 的多种 C++类，均以 QDom 开头：
|类|描述|
|----|----|
|QDomAttr|表示一个 QDomElement 的属性|
|QDomCDATASection|表示一个 XML CDATA 部分|
|QDomCharacterData|表示 DOM 中的一个通用字符串|
|QDomComment|表示一个 XML 注释|
|QDomDocument|表示一个 XML 文档|
|QDomDocumentFragment|QDomNodes 树，通常不是一个完整的 QDomDocument|
|QDomDocumentType|表示文档树中的 DTD|
|QDomElement|表示 DOM 树中的一个元素|
|QDomEntity|代表一个 XML 实体|
|QDomEntityReference|代表一个 XML 实体引用|
|QDomImplementation|DOM 实现的功能的信息|
|QDomNamedNodeMap|包含一个节点集合，节点可以通过名字来访问|
|QDomNode|一个 DOM 树中所有节点的基类|
|QDomNodeList|QDomNode 对象列表|
|QDomNotation|代表一个 XML 表示法|
|QDomProcessingInstruction|代表 XML 处理指令|
|QDomText|表示解析的 XML 文档中的文本数据|
QDom 类通常使用如下：
```
QDomDocument doc("mydocument");
QFile file("mydocument.xml");
if (!file.open(QIODevice::ReadOnly))
    return;
if (!doc.setContent(&file)) {
    file.close();
    return;
}
file.close();
// 打印出作为最外层元素的所有直接子元素的元素名称
QDomElement docElem = doc.documentElement();
QDomNode n = docElem.firstChild();
while(!n.isNull()) {
    QDomElement e = n.toElement(); // 尝试将节点转换为元素
    if(!e.isNull()) {
        cout << qPrintable(e.tagName()) << endl; // 节点真的是一个元素
    }
    n = n.nextSibling();
}
// 这里，我们在文档的末尾添加一个新元素
QDomElement elem = doc.createElement("img");
elem.setAttribute("src", "myimage.png");
docElem.appendChild(elem);
```
一旦 doc 和 elem 超出范围，代表 XML 文档的整个内部树被删除。
要使用 DOM 创建文档，请使用如下代码：
```
QDomDocument doc("MyML");
QDomElement root = doc.createElement("MyML");
doc.appendChild(root);
QDomElement tag = doc.createElement("Greeting");
root.appendChild(tag);
QDomText t = doc.createTextNode("Hello World");
tag.appendChild(t);
QString xml = doc.toString();
```
``
有关文档对象模型的更多信息，请参阅文档对象模型（DOM）[Level 1](https://www.w3.org/TR/REC-DOM-Level-1/) 和[evel 2 Core](https://www.w3.org/TR/DOM-Level-2-Core/)
 核心规范。
## SAX
SAX 是用于 XML 解析器的基于事件的标准接口。XML 类的设计遵循 [SAX2 Java interface](http://www.saxproject.org/)，名称适合 Qt 的命名约定。对于任何使用 SAX2 的人来说，使用 Qt XML 类应该非常容易。
### SAX2 介绍
SAX2 接口是一种事件驱动的机制，能够为用户提供文档信息。这里提到的“事件”意味着解析器报告的东西。例如，它遇到了一个开始标记，或者结束标记等。
为了使它不那么抽象，考虑以下示例：
```xml
<quote>A quotation</quote>
```
```xml
```
在读取（SAX2 解析器通常被称为“读取器”）时，上述文档将触发三个事件：
- 出现开始标记（`<quote>`）。
- 找到字符数据（即文本）“A quotation”。
- 解析结束标记（`</ quote>`）。
每次发生这样的事件时，解析器就会报告它，可以设置事件处理程序来响应这些事件。
虽然这是一种快速、简单地读取 XML 文档的方法，但是操作困难，因为数据不存储，而是串行处理并丢弃。DOM 接口读取并以树结构存储整个文档，这需要更多的内存，但更容易操作文档的结构。
Qt XML 模块提供了一个抽象类 QXmlReader，它定义了潜在的 SAX2 读取器的接口，它有一个简单的 XML 读取器的实现 - QXmlSimpleReader（目前只有这一个，在将来的版本中，可能有更多的具有不同属性的读取器。例如：验证解析器），通过子类化，很容易适应。
读取器通过特殊的 handler 类来报告解析事件：
|Handler 类|描述|
|----|----|
|QXmlContentHandler|报告与文档内容相关的事件（例如：开始标签或字符）|
|QXmlDTDHandler|报告与 DTD 相关的事件（例如：符号声明）|
|QXmlErrorHandler|报告解析期间发生的错误或警告（QXmlParseException 用于向使用 QXmlErrorHandler 的接口报告错误）|
|QXmlEntityResolver|在解析期间报告外部实体，并允许用户解析外部实体本身，而不是将其留给读取器。|
|QXmlDeclHandler|报告进一步的 DTD 相关事件（例如：属性声明）|
|QXmlLexicalHandler|报告与文档的词法结构相关的事件（DTD 的开头、注释等）|
由于处理程序类只描述接口，因此必须实现所有函数。Qt 提供了 QXmlDefaultHandler 类使之变得更容易：它实现所有函数的默认行为（不做任何操作），所以可以子类化它，只需要实现感兴趣的函数即可。
要读取输入的 XML 数据，需要使用特殊类 QXmlInputSource（QXmlReader 子类的输入数据）。
除了已经提到的，以下 SAX2 支持类还提供了其他有用的功能：
|类|描述|
|----|----|
|QXmlAttributes|用于向来是元素事件中传递属性|
|QXmlLocator|用于获取事件的实际解析位置|
|QXmlNamespaceSupport|用于为读取器实现命名空间支持。**注意：**命名空间不会更改解析行为，它们只通过处理程序进行报告。|
# XML 流
从 4.3 版本开始，Qt 提供了两个新的类来读写 XML：QXmlStreamReader 和 QXmlStreamWriter。
流读取器将 XML 文档报告为标记流。这与 SAX 不同，因为 SAX 应用程序提供处理程序从解析器接收 XML 事件，而 QXmlStreamReader 驱动循环，在需要时从读取器中提取标记。这种拉取方法使得构建递归下降解析器成为可能，允许 XML 解析代码被拆分成不同的方法或类。
QXmlStreamReader 是一个格式良好的 XML 1.0 解析器，排除外部解析实体。因此，只要没有错误发生，应用程序代码就可以确保由 QXmlStreamReader 提供的数据满足 W3C 对于格式良好的 XML 的标准。否则，可以使用诸如 atEnd()、error() 和 hasError() 的函数来检查和查看错误。
## QXmlStreamReader
应用程序中，QXmlStreamReader 类比用于 SAX 解析的 QXmlSimpleReader 类速度更快，且更好用。QXmlStreamReader 类用于读取数据，他可以通过 QIODevice 和 QByteArray 读取列数据。
一个典型的 QXmlStreamReader 循环：
```
QXmlStreamReader xml;
...
while (!xml.atEnd()) {
    xml.readNext();
    ... // 做处理
}
if (xml.hasError()) {
    ... // 做错误处理
}
```
``
## QXmlStreamWriter
QXmlStreamWriter 类仅通过数据流写入 XML 数据，该类由成员函数 setDevice() 指定的 QIODevice 运行。
下面的简化代码片段，显示了该类用缩进编写格式化 XML 的基本用法：
```
QXmlStreamWriter stream(&output);
stream.setAutoFormatting(true);
stream.writeStartDocument();
...
stream.writeStartElement("bookmark");
stream.writeAttribute("href", "http://qt-project.org/");
stream.writeTextElement("title", "Qt Project");
stream.writeEndElement(); // bookmark
...
stream.writeEndDocument();
```
``
# 使用哪种方式
虽然 Qt XML 模块提供了 XML 对 SAX 和 DOM 标准的 C++ 实现，但是助手中有这么一句话：
> 
The module is not actively maintained anymore. Please use the QXmlStreamReader and QXmlStreamWriter classes in Qt Core instead.
此模块不再进行积极地维护，请使用Qt Core 中的 QXmlStreamReader 和 QXmlStreamWriter 类代替。
也就是说，助手更建议我们使用 QXmlStreamReader 和 QXmlStreamWriter 类，而非 XML 模块。
