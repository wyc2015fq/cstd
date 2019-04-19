# qt 之解析XML元素（QXmlStreamReader） - baidu_33570760的博客 - CSDN博客
2017年05月04日 11:34:28[carman_风](https://me.csdn.net/baidu_33570760)阅读数：397
转载自：http://blog.csdn.net/liang19890820/article/details/52817386
# 简述
对于 XML 的内容，通常情况下，我们只关心 XML 元素的解析。这时，可以通过 QXmlStreamReader 中的便利函数 readNextStartElement() 来实现。
- [简述](http://blog.csdn.net/liang19890820/article/details/52817386#%E7%AE%80%E8%BF%B0)
- [详细介绍](http://blog.csdn.net/liang19890820/article/details/52817386#%E8%AF%A6%E7%BB%86%E4%BB%8B%E7%BB%8D)
- [使用](http://blog.csdn.net/liang19890820/article/details/52817386#%E4%BD%BF%E7%94%A8)
- [更多参考](http://blog.csdn.net/liang19890820/article/details/52817386#%E6%9B%B4%E5%A4%9A%E5%8F%82%E8%80%83)
# 详细介绍
之前使用的方式主要使用 readNext() 来读取下一个标记，并返回对应的类型。
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
这对于读取注释、字符、DTD、结束标签等类型比较方便。当只关心 XML 元素的解析，这时，此种方式就显得比较复杂了，下面介绍一种简单方式。
在介绍之前，先明确一个概念：
- 当前元素： 
当前元素是匹配最近解析的开始元素的元素，其中匹配的结束元素尚未到达。当解析器到达结束元素时，当前元素将成为父元素。
下面，主要用到 QXmlStreamReader 的两个接口：
> 
bool readNextStartElement()
读取，直到当前元素的下一个开始元素。当达到开始元素时，返回 true；当达到结束元素或发生错误时，返回 false。
> 
void skipCurrentElement()
读取，直到当前元素的结尾，跳过任何子节点。此函数对于跳过未知元素非常有用。
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
详细说明见：[Qt之生成XML（QXmlStreamWriter）](http://blog.csdn.net/u011012932/article/details/52806379)
封装一个解析类 XMLReader，XMLReader.h 如下所示：
```
#ifndef XMLREADER_H
#define XMLREADER_H
#include <QXmlStreamReader>
class XMLReader
{
public:
    XMLReader();
    bool read(QIODevice *device);
    QString errorString() const;  // 错误信息
private:
    void readXBEL();  // 读取根元素 <Blogs>
    void readBlog();  // 读取元素 <Blog>
    void readAuthor();  // 读取元素 <作者>
    void readHOME();   // 读取元素 <主页>
    void readInstruction();  // 读取元素 <个人说明>
    QXmlStreamReader xml;
};
#endif // XMLREADER_H
```
XMLReader.cpp 如下所示：
```
#include "XMLReader.h"
#include <QDebug>
#define ROOT_ELEMENT "Blogs"
#define BLOG_ELEMENT "Blog"
#define AUTHOR_ELEMENT QString::fromLocal8Bit("作者")
#define HOME_ELEMENT QString::fromLocal8Bit("主页")
#define INSTRUCTION_ELEMENT QString::fromLocal8Bit("个人说明")
#define VERSION_ATTRIBUTE "Version"
XMLReader::XMLReader()
{
}
bool XMLReader::read(QIODevice *device)
{
    xml.setDevice(device);
    if (xml.readNextStartElement()) {
        QString strName = xml.name().toString();
        if (strName== ROOT_ELEMENT) {  // 获取根元素
            QXmlStreamAttributes attributes = xml.attributes();
            if (attributes.hasAttribute(VERSION_ATTRIBUTE)) {  // 存在属性 Version
                QString strVersion = attributes.value(VERSION_ATTRIBUTE).toString();
                if (strVersion == "1.0") {  // 可以作为版本兼容性判断
                    qDebug() << "Version : " << strVersion;
                    readXBEL();
                } else {
                    xml.raiseError("The file is not an XBEL version 1.0 file.");
                }
            }
        } else {
            xml.raiseError("XML file format error.");
        }
    }
    return !xml.error();
}
// 错误信息
QString XMLReader::errorString() const
{
    return QString("Error:%1  Line:%2  Column:%3")
            .arg(xml.errorString())
            .arg(xml.lineNumber())
            .arg(xml.columnNumber());
}
// 读取根元素 <Blogs>
void XMLReader::readXBEL()
{
    Q_ASSERT(xml.isStartElement() && xml.name().toString() == ROOT_ELEMENT);
    while (xml.readNextStartElement()) {
        if (xml.name().toString() == BLOG_ELEMENT) {
            readBlog();
        } else {
            xml.skipCurrentElement();  // 跳过当前元素
        }
    }
}
// 读取元素 <Blog>
void XMLReader::readBlog()
{
    Q_ASSERT(xml.isStartElement() && xml.name().toString() == BLOG_ELEMENT);
    while (xml.readNextStartElement()) {
        if (xml.name().toString() == AUTHOR_ELEMENT)
            readAuthor();
        else if (xml.name().toString() == HOME_ELEMENT)
            readHOME();
        else if (xml.name().toString() == INSTRUCTION_ELEMENT)
            readInstruction();
        else
            xml.skipCurrentElement();  // 跳过当前元素
    }
}
// 读取元素 <作者>
void XMLReader::readAuthor()
{
    Q_ASSERT(xml.isStartElement() && xml.name().toString() == AUTHOR_ELEMENT);
    QString strAuthor = xml.readElementText();
    qDebug() << QString::fromLocal8Bit("作者：%1").arg(strAuthor);
}
// 读取元素 <主页>
void XMLReader::readHOME()
{
    Q_ASSERT(xml.isStartElement() && xml.name().toString() == HOME_ELEMENT);
    QString strHome = xml.readElementText();
    qDebug() << QString::fromLocal8Bit("主页：%1").arg(strHome);
}
// 读取元素 <个人说明>
void XMLReader::readInstruction()
{
    Q_ASSERT(xml.isStartElement() && xml.name().toString() == INSTRUCTION_ELEMENT);
    QString strInstruction = xml.readElementText();
    qDebug() << QString::fromLocal8Bit("个人说明：%1").arg(strInstruction);
}
```
使用时，调用 readXML() 即可。
```
#include <QFile>
#include "XMLReader.h"
// 解析 XML
void readXML() {
    QString strFile("Blogs.xml");
    QFile file(strFile);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {  // 以只读模式打开
        qDebug() << QString("Cannot read file %1(%2).").arg(strFile).arg(file.errorString());
        return;
    }
    XMLReader reader;
    if (!reader.read(&file)) {
        qDebug() << QString("Parse error in file %1(%2).").arg(strFile).arg(reader.errorString());
    }
}
```
