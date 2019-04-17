# 用Python处理HTML转义字符的5种方式 - zhusongziye的博客 - CSDN博客





2017年12月12日 21:48:26[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：9257









写爬虫是一个发送请求，提取数据，清洗数据，存储数据的过程。在这个过程中，不同的数据源返回的数据格式各不相同，有 JSON 格式，有 XML 文档，不过大部分还是 HTML 文档，HTML 经常会混杂有转移字符，这些字符我们需要把它转义成真正的字符。


### 什么是转义字符


在 HTML 中 `<`、`>`、`&` 等字符有特殊含义（<，>
 用于标签中，& 用于转义），他们不能在 HTML 代码中直接使用，如果要在网页中显示这些符号，就需要使用 HTML 的转义字符串（Escape Sequence），例如 `<` 的转义字符是 `<`，浏览器渲染
 HTML 页面时，会自动把转移字符串换成真实字符。


转义字符（Escape Sequence）由三部分组成：第一部分是一个 & 符号，第二部分是实体（Entity）名字，第三部分是一个分号。 比如，要显示小于号（<），就可以写`<` 。

![](https://img-blog.csdn.net/20171212214452301?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


### Python 反转义字符串


用 Python 来处理转义字符串有多种方式，而且 py2 和 py3 中处理方式不一样，在 python2 中，反转义串的模块是 `HTMLParser`。

```
# python2
import HTMLParser
>>> HTMLParser().unescape('a=1&b=2')
'a=1&b=2'
```


Python3 把 HTMLParser 模块迁移到 `html.parser`

```
# python3
>>> from html.parser import HTMLParser
>>> HTMLParser().unescape('a=1&b=2')
'a=1&b=2'
```


到 python3.4 之后的版本，在 html 模块新增了 unescape 方法。

```
# python3.4
>>> import html
>>> html.unescape('a=1&b=2')
'a=1&b=2'
```


推荐最后一种写法，因为 HTMLParser.unescape 方法在 Python3.4 就已经被废弃掉不推荐使用，意味着之后的版本有可能会被彻底移除。


另外，sax 模块也有支持反转义的函数

```
>>> from xml.sax.saxutils import unescape
>>> unescape('a=1&b=2')
'a=1&b=2'
```


当然，你完全可以实现自己的反转义功能，不复杂，当然，我们崇尚不重复造轮子。



