# qt 之解析 XML（QXmlStreamReader） - baidu_33570760的博客 - CSDN博客
2017年05月04日 11:31:44[carman_风](https://me.csdn.net/baidu_33570760)阅读数：449
转载自：http://blog.csdn.net/liang19890820/article/details/52808829
# 简述
QXmlStreamReader 类提供了一个快速解析器，用于通过简单的流 API 读取格式良好的 XML。与之相对应的是 QXmlStreamWriter（写入 XML）。
相比较 Qt 自己的 SAX 解析器（见 QXmlSimpleReader），QXmlStreamReader 更快，更方便。某些情况下，在使用 DOM 树（见 QDomDocument）的应用程序中，它也可能是一个更快，更方便的替代方案。
QXmlStreamReader 可以从 QIODevice（见 setDevice()）或原始 QByteArray（见 addData()）中读取数据。
- [简述](http://blog.csdn.net/liang19890820/article/details/52808829#%E7%AE%80%E8%BF%B0)
- [详细介绍](http://blog.csdn.net/liang19890820/article/details/52808829#%E8%AF%A6%E7%BB%86%E4%BB%8B%E7%BB%8D)
- [用法](http://blog.csdn.net/liang19890820/article/details/52808829#%E7%94%A8%E6%B3%95)
- [命名空间](http://blog.csdn.net/liang19890820/article/details/52808829#%E5%91%BD%E5%90%8D%E7%A9%BA%E9%97%B4)
- [增量解析](http://blog.csdn.net/liang19890820/article/details/52808829#%E5%A2%9E%E9%87%8F%E8%A7%A3%E6%9E%90)
- [性能和内存消耗](http://blog.csdn.net/liang19890820/article/details/52808829#%E6%80%A7%E8%83%BD%E5%92%8C%E5%86%85%E5%AD%98%E6%B6%88%E8%80%97)
- [使用](http://blog.csdn.net/liang19890820/article/details/52808829#%E4%BD%BF%E7%94%A8)
- [更多参考](http://blog.csdn.net/liang19890820/article/details/52808829#%E6%9B%B4%E5%A4%9A%E5%8F%82%E8%80%83)
# 详细介绍
QXmlStreamReader 的基本概念是将 XML 文档呈现为标记流，类似于 SAX。
QXmlStreamReader 和 SAX 的主要不同点在于如何呈现 XML 标记。
- 使用 SAX，应用程序必须提供处理程序（回调函数），在解析器方便的情况下，从解析器中接收所谓的 XML 事件。
- 使用 QXmlStreamReader，应用程序代码本身驱动循环，并从 reader 中取出标记，一个接一个。通过调用 readNext() 完成，reader 从输入流中读取，直到完成下一个标记，此时返回 tokenType()。然后可以使用一组方便的函数 isStartElement() 和 text() 来检查标记，以获取已读取的信息。这种取出标记的方式最大优点是可以使用它构建递归下降解析器，也就是说，可以将 XML 解析代码轻松地分成不同的方法或类，这使得在解析 XML 时可以轻松跟踪应用程序自己的状态。
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
QXmlStreamReader 是一个格式良好的 XML 1.0 解析器，不包括外部解析实体。只要没有错误发生，应用程序代码就可以确保由 QXmlStreamReader 提供的数据满足 W3C 对于格式良好的 XML 的标准。例如，可以确定所有标签都已正确嵌套和关闭，对内部实体的引用已替换为正确的文本，并且属性已进行规范化或根据 DTD 内部子集进行添加。
如果在解析时发生错误，atEnd() 和 hasError() 将返回 true，而 error() 返回发生的错误。函数 errorString()、lineNumber()、columnNumber()、characterOffset() 用于构造相应的错误或警告消息。为了简化应用程序代码，QXmlStreamReader 提供了一个 raiseError() 机制，用于添加自定义错误，触发同样的错误处理描述。
# 用法
枚举 QXmlStreamReader::Error：
该枚举指定不同的错误情况。
|常量|值|描述|
|----|----|----|
|QXmlStreamReader::NoError|0|没有发生错误|
|QXmlStreamReader::CustomError|2|由 raiseError() 引发的自定义错误|
|QXmlStreamReader::NotWellFormedError|3|由于读取的不是格式良好的 XML，解析器内部引发了一个错误|
|QXmlStreamReader::PrematureEndOfDocumentError|4|输入流在解析格式正确的 XML 文档之前结束，如果有更多的 XML 到达流中，将从错误中恢复。可以通过调用 addData() 或等待数据到达 device()。|
|QXmlStreamReader::UnexpectedElementError|1|解析器遇到一个与预期不同的元素|
枚举 QXmlStreamReader::ReadElementTextBehaviour：
该枚举指定 readElementText() 的不同行为。
|常量|值|描述|
|----|----|----|
|QXmlStreamReader::ErrorOnUnexpectedElement|0|引发 UnexpectedElementError 并返回遇到子元素时读到的内容|
|QXmlStreamReader::IncludeChildElements|1|递归地包括子元素的文本|
|QXmlStreamReader::SkipChildElements|2|跳过子元素|
枚举 QXmlStreamReader::TokenType：
该枚举指定 reader 刚刚读取的标记类型。
|常量|值|描述|
|----|----|----|
|QXmlStreamReader::NoToken|0|reader 还没有读取任何东西|
|QXmlStreamReader::Invalid|1|发生错误，记录在 error() 和 errorString() 中|
|QXmlStreamReader::StartDocument|2|reader 记录了 XML 版本号（documentVersion()），以及 XML 文档中指定的编码（documentEncoding()），如果文档被声明为独立的，isStandaloneDocument() 返回 true；否则返回 false。|
|QXmlStreamReader::EndDocument|3|表示文档的结束|
|QXmlStreamReader::StartElement|4|表示一个拥有 namespaceUri() 和 name() 的元素开始。空元素也由 StartElement 表示，紧跟着 EndElement。可以调用便利函数 readElementText() 来来获取所有内容，直到遇到对应的 EndElement。属性在 attributes() 中声明，命名空间则在 namespaceDeclarations() 中声明。|
|QXmlStreamReader::EndElement|5|表示一个拥有 namespaceUri() 和 name() 的元素的结束|
|QXmlStreamReader::Characters|6|reader 由 text() 表示字符， 如果字符都是空格，isWhitespace() 返回true。如果字符来自 CDATA 部分，isCDATA() 返回 true。|
|QXmlStreamReader::Comment|7|reader 由 text() 表示注释|
|QXmlStreamReader::DTD|8|reader 由 text() 表示 DTD，notationDeclarations() 表示符号声明，entityDeclarations() 表示实体声明，DTD 声明的详细信息则由 dtdName()、dtdPublicId() 和 dtdSystemId() 表示。|
|QXmlStreamReader::EntityReference|9|reader 记录一个无法解析的实体引用，引用的名称由 name() 表示，替换文本为 text()。|
|QXmlStreamReader::ProcessingInstruction|10|reader 由 processingInstructionTarget() 和 processingInstructionData() 来表示处理指令|
# 命名空间
QXmlStream 理解并解析 XML 命名空间。例如，在 StartElement 情况下，namespaceUri() 返回元素所在的命名空间，而 name() 返回元素的本地名称，namespaceUri 和 name 的组合唯一地标识了一个元素。如果命名空间前缀没有在 reader 解析的 XML 实体中声明，则 namespaceUri 为空。
如果解析 XML 数据不使用命名空间根据 XML 规范，或者根本不使用命名空间，则可以使用元素的 qualifiedName() 来代替。限定名称是元素的 prefix() 后跟冒号，再加上元素的本地 name() - 与原始 XML 数据中显示的元素完全相同。由于映射 namespaceUri 到前缀既不是唯一的也不是通用的，因此应该避免对符合命名空间的 XML 数据使用 qualifiedName()。
为了解析使用未声明命名空间前缀的独立文档，可以使用 namespaceProcessing 属性完全关闭命名空间处理。
# 增量解析
QXmlStreamReader 是一个增量解析器。它可以处理文档不能一次解析的情况，因为它到达块（例如，从多个文件或通过一个网络连接）。当在完整的文档被解析之前，reader 遇到数据不足的情况下，它会记录一个 PrematureEndOfDocumentError。当更多数据到达时，无论调用 addData()，还是通过网络 device() 有更多的数据可用，reader 会从 PrematureEndOfDocumentError 错误中恢复，并继续解析新数据在下一次调用 readNext()。
例如，如果应用程序使用 QNetworkAccessManager 从网络读取数据，将向 manager 发出网络请求，然后接收网络响应。由于 QNetworkReply 是一个 QIODevice，将其 readyRead() 信号连接到自定义的槽函数，在槽函数中使用 readAll() 读取所有可用数据，并使用 addData() 将其传递到 QXmlStreamReader。然后调用自定义解析函数，从 reader 读取 XML 事件。
# 性能和内存消耗
QXmlStreamReader 的设计比较内存保守，因为不在内存中存储整个 XML 文档树，只记录当前的标志。此外，QXmlStreamReader 避免了许多小字符串的分配，它通常需要将 XML 文档映射到一个方便的 Qt-ish API，通过将所有字符串数据记录为 QStringRef，而非实际的 QString 对象来实现这一点。QStringRef 是一个围绕 QString 子字符串的简单包装，它提供了 QString API 的一个子集，而没有内存分配和引用计数的开销，对任何这些对象调用
 toString() 将返回一个等效的真正 QString 对象。
# 使用
为了便于演示，使用上节生成的格式化 XML（Blogs.xml）：
```xml
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
```xml
```
详细说明见： [Qt之生成XML（QXmlStreamWriter）](http://blog.csdn.net/u011012932/article/details/52806379)
解析源码如下：
```
QFile file("Blogs.xml");
if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {  // 以只写模式打开文件
    QXmlStreamReader reader(&file);
    // 解析 XML，直到结束
    while (!reader.atEnd()) {
        // 读取下一个元素
        QXmlStreamReader::TokenType nType = reader.readNext();
        switch (nType) {
        case QXmlStreamReader::StartDocument: {  // 开始文档
            qDebug() << QString::fromLocal8Bit("********** 开始文档（XML 声明） ********** ");
            // XML 声明
            QString strVersion = reader.documentVersion().toString();
            QString strEncoding = reader.documentEncoding().toString();
            bool bAlone = reader.isStandaloneDocument();
            qDebug() << QString::fromLocal8Bit("版本：%1  编码：%2  Standalone：%3")
                        .arg(strVersion).arg(strEncoding).arg(bAlone) << "\r\n";
            break;
        }
        case QXmlStreamReader::Comment: {  // 注释
            qDebug() << QString::fromLocal8Bit("********** 注释 ********** ");
            QString strComment = reader.text().toString();
            qDebug() << strComment << "\r\n";
            break;
        }
        case QXmlStreamReader::ProcessingInstruction: {  // 处理指令
            qDebug() << QString::fromLocal8Bit("********** 处理指令 ********** ");
            QString strProcInstr = reader.processingInstructionData().toString();
            qDebug() << strProcInstr << "\r\n";
            break;
        }
        case QXmlStreamReader::DTD: {   // DTD
            qDebug() << QString::fromLocal8Bit("********** DTD ********** ");
            QString strDTD = reader.text().toString();
            QXmlStreamNotationDeclarations notationDeclarations = reader.notationDeclarations();  // 符号声明
            QXmlStreamEntityDeclarations entityDeclarations = reader.entityDeclarations();  // 实体声明
            // DTD 声明
            QString strDTDName = reader.dtdName().toString();
            QString strDTDPublicId = reader.dtdPublicId().toString();  // DTD 公开标识符
            QString strDTDSystemId = reader.dtdSystemId().toString();  // DTD 系统标识符
            qDebug() << QString::fromLocal8Bit("DTD : %1").arg(strDTD);
            qDebug() << QString::fromLocal8Bit("DTD 名称 : %1").arg(strDTDName);
            qDebug() << QString::fromLocal8Bit("DTD 公开标识符 : %1").arg(strDTDPublicId);
            qDebug() << QString::fromLocal8Bit("DTD 系统标识符 : %1").arg(strDTDSystemId);
            qDebug() << "\r\n";
            break;
        }
        case QXmlStreamReader::StartElement: {  // 开始元素
            QString strElementName = reader.name().toString();
            if (QString::compare(strElementName, "Blogs") == 0) {  // 根元素
                qDebug() << QString::fromLocal8Bit("********** 开始元素<Blogs> ********** ");
                QXmlStreamAttributes attributes = reader.attributes();
                if (attributes.hasAttribute("Version")) {
                    QString strVersion = attributes.value("Version").toString();
                    qDebug() << QString::fromLocal8Bit("属性：Version(%1)").arg(strVersion);
                }
                parseBlog(reader);
            }
            break;
        }
        case QXmlStreamReader::EndDocument: {  // 结束文档
            qDebug() << QString::fromLocal8Bit("********** 结束文档 ********** ");
            break;
        }
        }
    }
    if (reader.hasError()) {  // 解析出错
        qDebug() << QString::fromLocal8Bit("错误信息：%1  行号：%2  列号：%3  字符位移：%4").arg(reader.errorString()).arg(reader.lineNumber()).arg(reader.columnNumber()).arg(reader.characterOffset());
    }
    file.close();  // 关闭文件
}
```
针对 `<Blog>` 元素中的内容，进行单独解析：
```
void parseBlog(QXmlStreamReader &reader) {
    while (!reader.atEnd()) {
        reader.readNext();
        if (reader.isStartElement()) {  // 开始元素
            QString strElementName = reader.name().toString();
            if (QString::compare(strElementName, "Blog") == 0) {
                qDebug() << QString::fromLocal8Bit("********** 开始元素<Blog> ********** ");
            } else if (QString::compare(strElementName, QStringLiteral("作者")) == 0) {  // 方式一
                reader.readNext();
                if (reader.isCharacters()) {
                    qDebug() << QStringLiteral("作者：%1").arg(reader.text().toString());
                }
            } else if (QString::compare(strElementName, QStringLiteral("主页")) == 0) {  // 方式二
                qDebug() << QStringLiteral("主页：%1").arg(reader.readElementText());
            } else if (QString::compare(strElementName, QStringLiteral("个人说明")) == 0) {
                qDebug() << QStringLiteral("个人说明：%1").arg(reader.readElementText());
            }
        } else if (reader.isEntityReference()) {  // 实体引用
            QString strName = reader.name().toString();
            QString strText = reader.text().toString();
            qDebug() << QString("EntityReference : %1(%2)").arg(strName).arg(strText);
        } else if (reader.isCDATA()) {  // CDATA
            QString strCDATA = reader.text().toString();
            qDebug() << QString("CDATA : %1").arg(strCDATA);
            reader.readNext();
            if (reader.isCharacters()) {
                QString strCharacters = reader.text().toString();
                qDebug() << QString("Characters : %1").arg(strCharacters);
            }
        } else if (reader.isEndElement()) {  // 结束元素
            QString strElementName = reader.name().toString();
            if (QString::compare(strElementName, "Blogs") == 0) {
                qDebug() << QString::fromLocal8Bit("********** 结束元素<Blogs> ********** ");
                break;  // 跳出循环（解析 QXmlStreamReader::EndDocument）
            } else if (QString::compare(strElementName, "Blog") == 0) {
                qDebug() << QString::fromLocal8Bit("********** 结束元素<Blog> ********** ");
            }
        }
    }
}
```
