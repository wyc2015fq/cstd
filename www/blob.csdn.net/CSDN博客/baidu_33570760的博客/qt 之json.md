# qt 之json - baidu_33570760的博客 - CSDN博客
2017年05月03日 17:44:14[carman_风](https://me.csdn.net/baidu_33570760)阅读数：407
转载自： http://blog.csdn.net/liang19890820/article/details/52764239
# 简述
JSON（[JavaScript](http://lib.csdn.net/base/javascript) Object Notation）是一种轻量级的数据交换格式。易于人阅读和编写，同时也易于机器解析和生成。它基于[JavaScript
 Programming Language](http://www.crockford.com/javascript/)、[Standard ECMA-262 3rd Edition - December 1999](http://www.ecma-international.org/publications/files/ECMA-ST/Ecma-262.pdf) 的一个子集。JSON
 采用完全独立于语言的文本格式，但是也使用了类似于 C 语言家族的习惯（包括：C/C++、C#、[Java](http://lib.csdn.net/base/javase)、JavaScript、Perl、[Python](http://lib.csdn.net/base/python)
 等），这些特性使 JSON 成为理想的数据交换语言。
- [简述](http://blog.csdn.net/liang19890820/article/details/52764239#%E7%AE%80%E8%BF%B0)
- [JSON 结构](http://blog.csdn.net/liang19890820/article/details/52764239#json-%E7%BB%93%E6%9E%84)
- [JSON 形式](http://blog.csdn.net/liang19890820/article/details/52764239#json-%E5%BD%A2%E5%BC%8F)
- [Qt 对 JSON 的支持](http://blog.csdn.net/liang19890820/article/details/52764239#qt-%E5%AF%B9-json-%E7%9A%84%E6%94%AF%E6%8C%81)
- [更多参考](http://blog.csdn.net/liang19890820/article/details/52764239#%E6%9B%B4%E5%A4%9A%E5%8F%82%E8%80%83)
# JSON 结构
JSON 基于两种结构：
- “key/value 对”的集合 
不同的语言中，它被理解为对象（object）、纪录（record）、结构（struct）、字典（dictionary）、哈希表（hash table）、有键列表（keyed list）、或者关联数组 （associative array）。
`{ "Name": "Qt", "Company": "Digia", "From": 1991 }`- 值的有序列表 
在大部分语言中，它被理解为数组（array）。
```bash
"Colors": ["green", "blue", "yellow"]
```
这些都是常见的[数据结构](http://lib.csdn.net/base/datastructure)。事实上大部分现代计算机语言都以某种形式支持它们。这使得一种数据格式在同样基于这些结构的编程语言之间交换成为可能。
# JSON 形式
JSON 具有以下这些形式：
- 
对象（object） 
是一个无序的“key/value 对”集合。一个对象以“{”（左花括号）开始，“}”（右花括号）结束。每个 key 后跟一个“:”（冒号）；“key/value 对”之间使用“,”（逗号）分隔。
![这里写图片描述](https://img-blog.csdn.net/20161009142337919)
- 
数组（array） 
是值（value）的有序列表。一个数组以“[”（左中括号）开始，“]”（右中括号）结束。值之间使用“,”（逗号）分隔。
![这里写图片描述](https://img-blog.csdn.net/20161009142410591)
- 
值（value） 
可以是双引号括起来的字符串、数值、true、false、 null、对象或者数组。这些结构可以嵌套。
![这里写图片描述](https://img-blog.csdn.net/20161009142437948)
- 
字符串（string） 
是由双引号包围的任意数量 Unicode 字符的集合，使用反斜线转义。一个字符即一个单独的字符串。
字符串与 C 或者 Java 的字符串非常相似。
![这里写图片描述](https://img-blog.csdn.net/20161009142459795)
- 
数值（number）
也与 C 或者 Java 的数值非常相似。除去未曾使用的八进制与十六进制格式。除去一些编码细节。
![这里写图片描述](https://img-blog.csdn.net/20161009142642264)
可以在 [json.org](http://json.org/) 和 [RFC-4627](https://tools.ietf.org/html/rfc4627) 中找到有关 JSON 数据格式的更多细节。
# Qt 对 JSON 的支持
通过上述内容，我们对 JSON 有了详细的了解，在 Qt 使用过程中需要注意两点：
- JSON 没有明确指定数字的有效范围，但 Qt 的支持仅限于 double 的有效范围和精度。
- 对象（object）是“key/value 对”的集合，所有的 key 都是字符串，且不能重复 。
来看一个简单的 JSON 格式（包含：名称、公司、页面等）：
```
{
    "Name": "Qt",
    "Company": "Digia",
    "From": 1991,
    "Page": {
        "Developers": "https://www.qt.io/developers/",
        "Download": "https://www.qt.io/download/",
        "Home": "https://www.qt.io/"
    },
    "Version": [
        4.8,
        5.2,
        5.7
    ]
}
```
上面示例，包含了一个拥有五个“key/value 对”的对象，values 中的两个（Company、Name）是字符串，一个（From）是数字，一个（Page）是对象，一个（Version）是数组。
一个有效的 JSON 文档要么是对象，要么是数组，所以 JSON 文档总由一对花括号（{ … }）或中括号（[ … ]）表示。
Qt 对 JSON 的支持包含这些类：
|类|说明|
|----|----|
|QJsonArray|封装一个 JSON 数组|
|QJsonDocument|读和写 JSON 文档的方法|
|QJsonObject|封装 JSON 对象|
|QJsonObject::const_iterator|QJsonObject::const_iterator 类为 QJsonObject 提供了一个 STL 风格的常量迭代器|
|QJsonObject::iterator|QJsonObject::iterator 类为 QJsonObject 提供了一个 STL 风格的非常量迭代器|
|QJsonParseError|在 JSON 解析过程中用于报告错误|
|QJsonValue|封装 JSON 值|
