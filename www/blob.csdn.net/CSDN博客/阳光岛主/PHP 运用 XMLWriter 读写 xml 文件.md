
# PHP 运用 XMLWriter 读写 xml 文件 - 阳光岛主 - CSDN博客

2017年11月06日 20:41:43[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：1748


米扑科技旗下的多个产品，需要脚本自动生成sitemap.xml，于是重新温习一遍PHP XML读写操作。
读写xml的方式，主要围绕XMLWriter和XMLReader进行，前者用于生成xml，后者则是用来读取并解析xml
原文请见米扑博客：[PHP 运用 XMLWriter 读写 xml 文件](https://blog.mimvp.com/article/18421.html)
**写入 xml**
**test_xml_write.php**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|36
|37
|38
|39
|40
|41
|42
|43
|44
|45
|46
|47
|48
|49
|50
|51
|52
|53
|<?php
|/**
|* mimvp.com
|* 2017.06.22
|*/
|header(|"Content-type: text/html; charset=utf-8"|);
|$xml|=|new|XMLWriter();
|$xml|->openUri(|"php://output"|);   // 输出到网页控制台
|$xml|->openUri(|"mimvp.xml"|);|// 输出到文件，需要读写权限，推荐 chmod 766 mimvp.xml
|// 设置缩进字符串
|$xml|->setIndentString(|"\t"|);
|$xml|->setIndent(true);
|// xml文档开始
|$xml|->startDocument(|'1.0'|,|'utf-8'|);
|// 创建根节点
|$xml|->startElement(|"MimvpInfo"|);
|// 节点1
|$xml|->startElement(|"Item"|);
|$xml|->writeAttribute(|"info"|,|"1"|);|// 属性
|$xml|->startElement(|"id"|);
|$xml|->text(|"01"|);
|$xml|->endElement();
|$xml|->startElement(|"name"|);
|$xml|->text(|"米扑代理"|);
|$xml|->endElement();
|$xml|->endElement();
|// 节点2
|$xml|->startElement(|"Item"|);
|$xml|->writeAttribute(|"info"|,|"1"|);|// 属性
|$xml|->startElement(|"id"|);
|$xml|->text(|"02"|);
|$xml|->endElement();
|$xml|->startElement(|"name"|);
|$xml|->text(|"米扑财富"|);
|$xml|->endElement();
|$xml|->endElement();
|$xml|->endElement();
|$xml|->endDocument();
|//header("Content-type: text/xml");
|//取得缓冲区里的xml字符串
|//echo $xml->outputMemory();
|?>
|

**输出mimvp.xml文件内容：**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|<?|xml|version|=|"1.0"|encoding|=|"UTF-8"|?>
|<|MimvpInfo|>
|<|Item|info|=|"1"|>
|<|id|>01</|id|>
|<|name|>米扑代理</|name|>
|</|Item|>
|<|Item|info|=|"1"|>
|<|id|>02</|id|>
|<|name|>米扑财富</|name|>
|</|Item|>
|</|MimvpInfo|>
|

注意：
1. 新建mimvp.xml 文件：
touch mimvp.xml
2. 授权mimvp.xml 文件：
chmod 766 mimvp.xml
**读取 xml**
**test_xml_read.php**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|<?php
|/**
|* mimvp.com
|* 2017.06.22
|*/
|header (|"Content-type: text/html; charset=utf-8"|);
|$xml|=|new|XMLReader ();
|// 载入xml文件，如果是字符串直接使用xml方法
|$xml|->open (|"mimvp.xml"|);
|// 开始读取xml
|while|(|$xml|->read () ) {
|// 根据节点类型和元素名称取得文本节点或属性
|if|(|$xml|->nodeType == XMLREADER::ELEMENT &&|$xml|->localName ==|'item'|) {
|print|$xml|->getAttribute (|"info"|) .|"</br>"|;
|}
|if|(|$xml|->nodeType == XMLREADER::ELEMENT &&|$xml|->localName ==|'id'|) {
|$xml|->read ();|// 移动指针到下一个节点
|print|$xml|->value .|"\t"|;
|}
|if|(|$xml|->nodeType == XMLREADER::ELEMENT &&|$xml|->localName ==|'name'|) {
|$xml|->read ();
|print|$xml|->value .|"</br>"|;
|}
|if|(|$xml|->nodeType == XMLREADER::ELEMENT &&|$xml|->localName ==|'age'|) {
|$xml|->read ();
|print|$xml|->value .|"</br>"|;
|}
|}
|?>
|

输出结果：
01 米扑代理
02 米扑财富
**源代码及应用**
请见米扑科技的Github：[https://github.com/mimvp/sitemap-php](https://github.com/mimvp/sitemap-php)


