# 爬虫urilib库之parse - miner_zhu的博客 - CSDN博客





2018年08月20日 00:03:30[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：83








前面说过，urllib库里还提供了`parse`这个模块，它定义了处理URL的标准接口，例如实现URL各部分的抽取、合并以及链接转换。它支持如下协议的URL处理：file、ftp、gopher、hdl、http、https、imap、mailto、 mms、news、nntp、prospero、rsync、rtsp、rtspu、sftp、 sip、sips、snews、svn、svn+ssh、telnet和wais。本节中，我们介绍一下该模块中常用的方法来看一下它的便捷之处。

## **1. `urlparse()`**

该方法可以实现URL的识别和分段，返回结果是一个`ParseResult`类型的对象，它包含6部分，分别是`scheme`、`netloc`、`path`、`params`、`query`和`fragment`。除了这种最基本的解析方式外，看一下`urlparse()`方法的API用法：

```python
urllib.parse.urlparse(urlstring, scheme='', allow_fragments=True)
```
- **`urlstring`**：这是必填项，即待解析的URL。
- **`scheme`**：它是默认的协议（比如`http`或`https`等）。假如这个链接没有带协议信息，会将这个作为默认的协议。
- **`allow_fragments`**：即是否忽略`fragment`。如果它被设置为`False`，`fragment`部分就会被忽略，它会被解析为`path`、`parameters`或者`query`的一部分，而`fragment`部分为空。

## 2. `urlunparse()`

有了`urlparse()`，相应地就有了它的对立方法`urlunparse()`。它接受的参数是一个可迭代对象，但是它的长度必须是6，否则会抛出参数数量不足或者过多的问题。

```python
from urllib.parse import urlunparse
 
data = ['http', 'www.baidu.com', 'index.html', 'user', 'a=6', 'comment']
print(urlunparse(data))
```

这样我们就成功实现了URL的构造。

## 3. `urlsplit()`

这个方法和`urlparse()`方法非常相似，只不过它不再单独解析`params`这一部分，只返回5个结果。上面例子中的`params`会合并到`path`中。

## 4. `urlunsplit()`

与`urlunparse()`类似，它也是将链接各个部分组合成完整链接的方法，传入的参数也是一个可迭代对象，例如列表、元组等，唯一的区别是长度必须为5。

## 5. `urljoin()`

有了`urlunparse()`和`urlunsplit()`方法，我们可以完成链接的合并，不过前提必须要有特定长度的对象，链接的每一部分都要清晰分开。

此外，生成链接还有另一个方法，那就是`urljoin()`方法。我们可以提供一个`base_url`（基础链接）作为第一个参数，将新的链接作为第二个参数，该方法会分析`base_url`的`scheme`、`netloc`和`path`这3个内容并对新链接缺失的部分进行补充，最后返回结果。`base_url`提供了三项内容`scheme`、`netloc`和`path`。如果这3项在新的链接里不存在，就予以补充；如果新的链接存在，就使用新的链接的部分。而`base_url`中的`params`、`query`和`fragment`是不起作用的。

通过`urljoin()`方法，我们可以轻松实现链接的解析、拼合与生成。

## 6. `urlencode()`

这里我们再介绍一个常用的方法——`urlencode()`，它在构造GET请求参数的时候非常有用，示例如下：

```python
from urllib.parse import urlencode
 
params = {
    'name': 'germey',
    'age': 22
}
base_url = 'http://www.baidu.com?'
url = base_url + urlencode(params)
print(url)
```

这里首先声明了一个字典来将参数表示出来，然后调用`urlencode()`方法将其序列化为GET请求参数，参数就成功地由字典类型转化为GET请求参数了。

这个方法非常常用。有时为了更加方便地构造参数，我们会事先用字典来表示。要转化为URL的参数时，只需要调用该方法即可。

## 7. `parse_qs()`

有了序列化，必然就有反序列化。如果我们有一串GET请求参数，利用`parse_qs()`方法，就可以将它转回字典

```python
from urllib.parse import parse_qs
 
query = 'name=germey&age=22'
print(parse_qs(query))
```

## 8. `parse_qsl()`

另外，还有一个`parse_qsl()`方法，它用于将参数转化为元组组成的列表，而列表中的每一个元素都是一个元组，元组的第一个内容是参数名，第二个内容是参数值。

## 9. `quote()`

该方法可以将内容转化为URL编码的格式。URL中带有中文参数时，有时可能会导致乱码的问题，此时用这个方法可以将中文字符转化为URL编码，示例如下：

```python
from urllib.parse import quote

keyword = '壁纸'
url = 'https://www.baidu.com/s?wd=' + quote(keyword)
print(url)
```

这里我们声明了一个中文的搜索文字，然后用`quote()`方法对其进行URL编码，最后得到的结果如下：

```python
https://www.baidu.com/s?wd=%E5%A3%81%E7%BA%B8
```

## 10. `unquote()`

有了`quote()`方法，当然还有`unquote()`方法，它可以进行URL解码，利用`unquote()`方法可以方便地实现解码。



转载原文：[https://cuiqingcai.com/5508.html](https://cuiqingcai.com/5508.html)



