# Qt 之生成 XML（QXmlStreamWriter） - xqhrs232的专栏 - CSDN博客
2018年09月07日 13:41:02[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：83
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[https://blog.csdn.net/liang19890820/article/details/52806379](https://blog.csdn.net/liang19890820/article/details/52806379)
# 简述
QXmlStreamWriter 类提供了一个使用简单的流 API，用于写入 XML，与之相对应的是 QXmlStreamReader（读取 XML）。
正如其相关类，它可以在 setDevice() 指定的 QIODevice 上操作。API 很简单明了：对于想要写的每个 XML 标签或事件，QXmlStreamWriter 都提供了一个专门的函数。
- [简述](https://blog.csdn.net/liang19890820/article/details/52806379#%E7%AE%80%E8%BF%B0)
- [详细介绍](https://blog.csdn.net/liang19890820/article/details/52806379#%E8%AF%A6%E7%BB%86%E4%BB%8B%E7%BB%8D)
- [使用](https://blog.csdn.net/liang19890820/article/details/52806379#%E4%BD%BF%E7%94%A8)
- [更多参考](https://blog.csdn.net/liang19890820/article/details/52806379#%E6%9B%B4%E5%A4%9A%E5%8F%82%E8%80%83)
# 详细介绍
元素标签使用 writeStartElement() 打开，紧随其后的是 writeAttribute() 或 writeAttributes()、元素内容，然后使用 writeEndElement() 关闭标签。writeEmptyElement() 则可以写入空元素。
元素内容由字符、实体引用或嵌套元素组成，使用 writeCharacters() 写入，它还负责转义所有禁用的字符和字符序列、writeEntityReference() 或对 writeStartElement() 的后续调用。有一个很方便的函数 writeTextElement()，可用于元素仅写入文本信息。
QXmlStreamWriter 负责前缀命名空间，在编写元素或属性时，所有需要做的是指定 namespaceUri。假如必须遵守特定的前缀，可以通过调用 writeNamespace() 或者 writeDefaultNamespace() 手动声明名称空间，让 writer 强制使用它们。或者，可以绕过 QXmlStreamWriter 的命名空间支持，并使用重载的方法，而不是使用限定名称。命名空间 [http://www.w3.org/XML/1998/namespace](http://www.w3.org/XML/1998/namespace) 是隐式的，并映射到前缀 xml。
QXmlStreamWriter 可以自动格式化生成的 XML 数据，通过在元素之间的空白部分添加换行符和缩进，使得 XML 数据更易读，并且对于大多数源代码管理系统更容易使用。可以使用 autoFormatting 属性（默认被禁用）打开此功能，并使用 autoFormattingIndent 属性（包含空格或制表符的数量，默认是 4）进行自定义。
还有其它函数 writeCDATA()、writeComment()、writeProcessingInstruction()、writeDTD()，XML streams 链由 writeCurrentToken() 支持。
默认情况下，QXmlStreamWriter 以 UTF-8 编码 XML，如果使用其它编码，调用 setCodec() 。
如果在写入底层设备时发生错误，hasError() 会返回 true，并忽略后续的写入。
# 使用
下面，我们编写一个格式化的 XML 缩进：
```
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<!--纯正开源之美，有趣、好玩、靠谱。。。-->
<?xml-stylesheet type="text/css" href="style.css"?>
<!DOCTYPE Blogs [ <!ENTITY Copyright "Copyright 2016《Qt实战一二三》"> <!ELEMENT Blogs (Blog)> <!ELEMENT Blog (作者,主页,个人说明)> <!ELEMENT 作者     (#PCDATA)> <!ELEMENT 主页     (#PCDATA)> <!ELEMENT 个人说明  (#PCDATA)> ]>
<Blogs Version="1.0">
    <Blog>
        <作者>一去丶二三里</作者>
        <主页>http://blog.csdn.net/liang19890820</主页>
        <个人说明>青春不老，奋斗不止！</个人说明>&Copyright;<![CDATA[<Qt分享&&交流>368241647</Qt分享&&交流>]]>><Empty/>
    </Blog>
</Blogs>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
第一行：XML 声明，它定义 XML 的版本（1.0）和所使用的编码（UTF-8）。
第二行：一个简单地注释（`<!--纯正开源之美，有趣、好玩、靠谱。。。-->`）。
第三行：处理指令，用来指挥解析引擎如何解析XML文档内容。例如，在 XML 文档中可以使用 xml-stylesheet 指令，通知 XML 解析引擎，应用 CSS 文件显示 XML 文档内容。
第四行开始：表示 DTD，携带一个有关自身格式的描述。
根元素是 `<Blogs>`，并包含属性 Version，其值为 “1.0”，文档中的所有 `<Blog>` 元素都被包含在 `<Blogs>` 中。`<Blog>` 元素,有 3 个子元素：`<作者>`、`< 主页>`、`<个人说明>`。后紧跟着实体引用、CDATA （不应由 XML 解析器进行解析的文本数据）以及一个空元素 `<Empty/>`。
源码如下：
```
void writeXML() {
    QString strFile("Blogs.xml");
    QFile file(strFile);
    if (!file.open(QFile::WriteOnly | QFile::Text)) { // 只写模式打开文件
        qDebug() << QString("Cannot write file %1(%2).").arg(strFile).arg(file.errorString());
        return;
    }
    QXmlStreamWriter writer(&file);
    // writer.setCodec("GBK");  // XML 编码
    writer.setAutoFormatting(true); // 自动格式化
    writer.writeStartDocument("1.0", true);  // 开始文档（XML 声明）
    writer.writeComment(QString::fromLocal8Bit("纯正开源之美，有趣、好玩、靠谱。。。"));  // 注释
    writer.writeProcessingInstruction("xml-stylesheet type=\"text/css\" href=\"style.css\"");  // 处理指令
    // DTD
    writer.writeDTD(QString::fromLocal8Bit("<!DOCTYPE Blogs [ <!ENTITY Copyright \"Copyright 2016《Qt实战一二三》\"> <!ELEMENT Blogs (Blog)> <!ELEMENT Blog (作者,主页,个人说明)> <!ELEMENT 作者     (#PCDATA)> <!ELEMENT 主页     (#PCDATA)> <!ELEMENT 个人说明  (#PCDATA)> ]>"));
    writer.writeStartElement("Blogs");  // 开始根元素 <Blogs>
    writer.writeAttribute("Version", "1.0");  // 属性
    writer.writeStartElement("Blog");  // 开始子元素 <Blog>
    writer.writeTextElement(QString::fromLocal8Bit("作者"), QString::fromLocal8Bit("一去丶二三里"));
    writer.writeTextElement(QString::fromLocal8Bit("主页"), "http://blog.csdn.net/liang19890820");
    writer.writeTextElement(QString::fromLocal8Bit("个人说明"), QString::fromLocal8Bit("青春不老，奋斗不止！"));
    writer.writeEntityReference("Copyright");
    writer.writeCDATA(QString::fromLocal8Bit("<Qt分享&&交流>368241647</Qt分享&&交流>"));
    writer.writeCharacters(">");
    writer.writeEmptyElement(QString::fromLocal8Bit("Empty"));  // 空元素
    writer.writeEndElement();  // 结束子元素 </Blog>
    writer.writeEndElement();  // 结束根元素 </Blogs>
    writer.writeEndDocument();  // 结束文档
    file.close();  // 关闭文件
}
```
**注意：**使用 writeStartElement() 打开元素标签后，要有相应的 writeEndElement() 来关闭。
# 更多参考
- [XML](https://www.w3.org/XML/)
- [Qt之XML](http://blog.csdn.net/u011012932/article/details/52805902)
- [Qt之解析XML（QXmlStreamReader）](http://blog.csdn.net/u011012932/article/details/52808829)
- [Qt之解析XML元素（QXmlStreamReader）](http://blog.csdn.net/u011012932/article/details/52817386)
- [Qt之XML（DOM）](http://blog.csdn.net/u011012932/article/details/52981488)
- [Qt之XML（SAX）](http://blog.csdn.net/u011012932/article/details/52984445)
