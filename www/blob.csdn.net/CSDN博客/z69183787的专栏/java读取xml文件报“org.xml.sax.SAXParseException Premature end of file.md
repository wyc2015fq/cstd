# java读取xml文件报“org.xml.sax.SAXParseException: Premature end of file - z69183787的专栏 - CSDN博客
2014年03月08日 07:06:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6249
背景：java读取xml文件，xml文件内容只有“<?xml version="1.0" encoding="UTF-8"?>”一行
java读取该xml文件时，报“org.xml.sax.SAXParseException: Premature end of file”错误。
 java解析xml文件时抛出异常：
> 
> 
org.xml.sax.SAXParseException: Premature end of file.
at com.sun.org.apache.xerces.internal.parsers.DOMParser.parse(DOMParser.java:264)
at com.sun.org.apache.xerces.internal.jaxp.DocumentBuilderImpl.parse(DocumentBuilderImpl.java:292)
at javax.xml.parsers.DocumentBuilder.parse(DocumentBuilder.java:98)
at com.eos.system.utility.XmlUtil.parseFileThrowsException(XmlUtil.java:1788)
解决方案：发现xml文件中只有<?xml version="1.0" encoding="UTF-8"?>一句，没有内容，随便添加一个空的标签后，异常就没有了
文件如下：
<?xml version="1.0" encoding="UTF-8"?>
<xml-body>
</xml-body>
