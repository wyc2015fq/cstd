# Python字符串的encode与decode - Arthur的随笔 - CSDN博客
2010年08月10日 12:35:00[largetalk](https://me.csdn.net/largetalk)阅读数：548标签：[python																[codec																[ide](https://so.csdn.net/so/search/s.do?q=ide&t=blog)](https://so.csdn.net/so/search/s.do?q=codec&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[python](https://blog.csdn.net/largetalk/article/category/715661)
转自：http://ipie.blogbus.com/logs/19379694.html
首先要搞清楚，字符串在Python内部的表示是unicode编码，因此，在做编码转换时，通常需要以unicode作为中间编码，即先将其他编码的字符串解码（decode）成unicode，再从unicode编码（encode）成另一种编码。
decode的作用是将其他编码的字符串转换成unicode编码，如str1.decode('gb2312')，表示将gb2312编码的字符串转换成unicode编码。
encode的作用是将unicode编码转换成其他编码的字符串，如str2.encode('gb2312')，表示将unicode编码的字符串转换成gb2312编码。
在某些IDE中，字符串的输出总是出现乱码，甚至错误，其实是由于IDE的结果输出控制台自身不能显示字符串的编码，而不是程序本身的问题。
如在UliPad中运行如下代码：
s=u"中文"
print s
会提示：UnicodeEncodeError: 'ascii' codec can't encode characters in position 0-1: ordinal not in range(128)。这是因为UliPad在英文WindowsXP上的控制台信息输出窗口是按照ascii编码输出的（英文系统的默认编码是ascii），而上面代码中的字符串是Unicode编码的，所以输出时产生了错误。
将最后一句改为：print s.encode('gb2312')
则能正确输出“中文”两个字。
若最后一句改为：print s.encode('utf8')
则输出：/xe4/xb8/xad/xe6/x96/x87，这是控制台信息输出窗口按照ascii编码输出utf8编码的字符串的结果。
Updated at: 2008.11.05
另外，代码中字符串的默认编码与代码文件本身的编码一致，如：
s='中文'
如果是在utf8的文件中，该字符串就是utf8编码，如果是在gb2312的文件中，则其编码为gb2312。 这种情况下，要进行编码转换，都需要先用decode方法将其转换成unicode编码，再使用encode方法将其转换成其他编码。通常，在没有指定特定的编码方式时，都是使用的系统默认编码创建的代码文件，在[这篇文章](http://ipie.blogbus.com/logs/19505115.html)中可以看到如何获得系统的默认编码。
如果字符串是这样定义：
s=u'中文' 
则该字符串的编码就被指定为unicode了，即python的内部编码，而与代码文件本身的编码无关。因此，对于这种情况做编码转换，只需要直接使用encode方法将其转换成指定编码即可。
如果一个字符串已经是unicode了，再进行解码则将出错，因此通常要对其编码方式是否为unicode进行判断：
isinstance(s, unicode)  #用来判断是否为unicode
update: 2015.1.12
```python
u'\xe4\xb8\xad'.encode('raw_unicode_escape')
u'\xe4\xb8\xad'.encode('unicode_escape').decode('string_escape')
```
unicode字符串中包含的是非unicode编码的字符，　直接str都会出错，所以要用上面语句变成非unicode字符串
