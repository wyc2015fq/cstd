# python requests的content和text方法的区别 - YZXnuaa的博客 - CSDN博客
2018年05月15日 11:26:17[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：176
个人分类：[Python库](https://blog.csdn.net/YZXnuaa/article/category/7389269)
- 问题：
> 
一直在想requests的content和text属性的区别，从print 结果来看是没有任何区别的
看下源码：
```python
@property
    def text(self):
        """Content of the response, in unicode.
        If Response.encoding is None, encoding will be guessed using
        ``chardet``.
        The encoding of the response content is determined based solely on HTTP
        headers, following RFC 2616 to the letter. If you can take advantage of
        non-HTTP knowledge to make a better guess at the encoding, you should
        set ``r.encoding`` appropriately before accessing this property.
        """
    #content的完整代码就不贴了。
    @property
    def content(self):
        """Content of the response, in bytes."""
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
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 结论是：
> 
resp.text返回的是Unicode型的数据。
resp.content返回的是bytes型也就是二进制的数据。
> 
也就是说，如果你想取文本，可以通过r.text。
如果想取图片，文件，则可以通过r.content。
（resp.json()返回的是json格式数据）
- 举个栗子
```python
# 例如下载并保存一张图片
import requests
jpg_url = 'http://img2.niutuku.com/1312/0804/0804-niutuku.com-27840.jpg'
content = requests.get(jpg_url).content
with open('demo.jpg', 'wb') as fp:
    fp.write(content)
```
