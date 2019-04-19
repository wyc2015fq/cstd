# Qt中的Jason解析详解 - fanyun的博客 - CSDN博客
2016年12月25日 16:11:32[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1357
JSON([JavaScript](http://lib.csdn.net/base/javascript) Object Notation) 是一种轻量级的数据交换格式。它比XML还小巧。 JSON采用完全独立于语言的文本格式，但是也使用了类似于[C语言](http://lib.csdn.net/base/c)家族的习惯（包括C、C++、C#、[Java](http://lib.csdn.net/base/javaee)、[JavaScript](http://lib.csdn.net/base/javascript)等）。这些特性使JSON成为理想的数据交换语言。
 易于人阅读和编写，同时也易于机器解析和生成(一般用于提升网络传输速率)。
关于Qt中对JSON的生成与解析，Qt5以前的版本，需要去进行单独下载、编译，才能使用。到了Qt5，提供了专门的QJsonDocument类来读取和写入JSON文档。
其基本数据类型有：
数字（整数或浮点数）
字符串（在双引号中）
逻辑值（true 或 false）
数组（在方括号[]中）
对象（在花括号{}中）
null
待补充
Qt中常用到的JSON类有以下这些：
![](https://img-blog.csdn.net/20151223095451182?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
QJsonDocument
QJsonDocument既可以从一个基于文本表示的UTF-8编码，又可以从Qt自己的二进制格式读取和写入这个文件。
JSON文档可以从它的基于文本的表示使用QJsonDocument::fromJson()转换为QJsonDocument，用.toJSON()将其转换回文字。解析器非常快速和高效，将JSON转换为二进制表示。
QJsonObject
QJsonObject类用于封装JSON对象。
JSON对象是键值对，其中键是唯一的字符串，其值由QJsonValue代表。一个QJsonObject可以从QVariantMap转换/被转换。
QJsonArray
QJsonArray类用于封装JSON数组。
一个JSON数组列表值。该列表可以通过从阵列插入和移除QJsonValue的操纵。一个QJsonArray可以从QVariantList转换为/被转换。
QJsonDocument有效解析后的文档可以使用!iSNull()判断。使用isArray()和isObject()来判断是否包含一个数组或对象。文档中包含的数组或对象可以使用array()或object()进行检索，然后读取或操纵。
