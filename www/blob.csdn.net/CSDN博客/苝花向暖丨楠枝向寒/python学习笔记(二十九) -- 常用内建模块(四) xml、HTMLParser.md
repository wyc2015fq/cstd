# python学习笔记(二十九) -- 常用内建模块(四) xml、HTMLParser - 苝花向暖丨楠枝向寒 - CSDN博客

2018年09月07日 16:48:47[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：51标签：[XML																[HTMLParser](https://so.csdn.net/so/search/s.do?q=HTMLParser&t=blog)](https://so.csdn.net/so/search/s.do?q=XML&t=blog)
个人分类：[python基础](https://blog.csdn.net/weixin_40247263/article/category/7905597)

所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)


**目录**

[XML](#XML)

[SAX](#SAX)

[ElementTree](#ElementTree)

[HTMLParser](#HTMLParser)

# XML

常见的三种解析方式DOM、SAX、ElementTree

DOM会把整个XML读入内存，解析为树，因此占用内存大，解析慢，优点是可以任意遍历树的节点。

SAX是流模式，边读边解析，占用内存小，解析快，缺点是我们需要自己处理事件。

ElementTree模块提供了一个轻量级、Pythonic的API，同时还有一个高效的C语言实现，即xml.etree.cElementTree。与DOM相比，ET的速度更快，API使用更直接、方便。与SAX相比，ET.iterparse函数同样提供了按需解析的功能，不会一次性在内存中读入整个文档。ET的性能与SAX模块大致相仿，但是它的API更加高层次，用户使用起来更加便捷。

下面举例 SAX和ElementTree

## SAX

SAX每读到一个开始标签会触发StartElementHandler事件

读到结束标签触发EndElementHandler事件

读到标签之间触发CharacterDataHandler 事件

```python
from xml.parsers.expat import ParserCreate

class DefaultSaxHandler(object):   # 自定义事件内容
    def start_element(self, name, attrs):
        print('开始标签名: %s, 标签属性: %s' % (name, str(attrs)))

    def end_element(self, name):
        print('结束标签名: %s' % name)

    def char_data(self, text):
        print('标签中数据: %s' % text)

xml = r'''<?xml version="1.0"?>
<ol>
    <li><a href="/python">Python</a></li>
    <li><a href="/ruby">Ruby</a></li>
</ol>
'''

handler = DefaultSaxHandler()
parser = ParserCreate()
parser.StartElementHandler = handler.start_element # 指向我们自定义的事件
parser.EndElementHandler = handler.end_element
parser.CharacterDataHandler = handler.char_data
parser.Parse(xml)  # 开始解析
```

结果如下 

```
D:\pythonJDK\python.exe D:/learnpython/datemodel.py
开始标签名: ol, 标签属性: {}
标签中数据: 

标签中数据:     
开始标签名: li, 标签属性: {}
开始标签名: a, 标签属性: {'href': '/python'}
标签中数据: Python
结束标签名: a
结束标签名: li
标签中数据: 

标签中数据:     
开始标签名: li, 标签属性: {}
开始标签名: a, 标签属性: {'href': '/ruby'}
标签中数据: Ruby
结束标签名: a
结束标签名: li
标签中数据: 

结束标签名: ol

Process finished with exit code 0
```

## ElementTree

xml文件如下：

```
<?xml version="1.0"?>
<doc>
  <branch name="codingpy.com" hash="1cdf045c">
    text,source
  </branch>
  <branch name="release01" hash="f200013e">
    <sub-branch name="subrelease01">
      xml,sgml
    </sub-branch>
  </branch>
  <branch name="invalid">
  </branch>
</doc>
```

```python
>>> import xml.etree.ElementTree as ET
>>> tree = ET.ElementTree(file=r'D:\doc1.xml')  # 读取xml
>>> root = tree.getroot() # 获取根节点
>>> root.tag # 获取根节点标签名
'doc'
>>> root.attrib # 获取根节点标签属性的键值对
{}
>>> for elem in tree.iter():  # 遍历节点树
	print(elem.tag,elem.attrib) # 打印每个节点的标签名、属性名

	
doc {}
branch {'name': 'codingpy.com', 'hash': '1cdf045c'}
branch {'name': 'release01', 'hash': 'f200013e'}
sub-branch {'name': 'subrelease01'}
branch {'name': 'invalid'}
>>> for elem in tree.iter(tag='branch'): # 遍历节点树取出标签为branch的节点
	print(elem.tag,elem.attrib)

	
branch {'name': 'codingpy.com', 'hash': '1cdf045c'}
branch {'name': 'release01', 'hash': 'f200013e'}
branch {'name': 'invalid'}

# iter和iterfind的区别：
# iterfind 无法获取到根节点层次，iterfind主要是根据path去查找节点，内层节点无法直接通过tag获取到
# iter可以获取到根节点层次，iter根据标签可以查到任意一个节点
>>> for elem in tree.iter('branch/sub-branch'):
	print(elem.tag, elem.attrib)

	
>>> for elem in tree.iterfind('branch/sub-branch'):
	print(elem.tag, elem.attrib)

	
sub-branch {'name': 'subrelease01'}
>>> for elem in tree.iter('doc'):
	print(elem.tag, elem.attrib)

	
doc {}
>>> for elem in tree.iterfind('doc'):
	print(elem.tag, elem.attrib)

>>> for elem in tree.iter('branch[@name="release01"]'):
	print(elem.tag,elem.attrib)
	
>>> for elem in tree.iterfind('branch[@name="release01"]'):
	print(elem.tag,elem.attrib)

	
branch {'name': 'release01', 'hash': 'f200013e'}
>>> for elem in tree.iter('branch'):
	print(elem.tag, elem.attrib)

	
branch {'name': 'codingpy.com', 'hash': '1cdf045c'}
branch {'name': 'release01', 'hash': 'f200013e'}
branch {'name': 'invalid'}
>>> for elem in tree.iter('sub-branch'):
	print(elem.tag, elem.attrib)

	
sub-branch {'name': 'subrelease01'}
>>> for elem in tree.iterfind('branch'):
	print(elem.tag, elem.attrib)

	
branch {'name': 'codingpy.com', 'hash': '1cdf045c'}
branch {'name': 'release01', 'hash': 'f200013e'}
branch {'name': 'invalid'}
>>> for elem in tree.iterfind('sub-branch'):
	print(elem.tag, elem.attrib)
```

上面我们是通过ElementTree读取的xml，这样其实也是将所有的节点全部都加载到内存中了。

优化：iterparse也可以加载xml文件，并且我们可以通过其一边解析一边判断并删除没有用的节点。

```python
>>> for event, elem in ET.iterparse(r'D:\doc1.xml'):
	if event == 'end':
		if elem.tag == 'branch':
			count += 1
	elem.clear()

	
>>> print(count)
3
```

我们这里只在树被构建时，遍历一次。而使用parse的标准方法是先完成整个树的构建后，才再次遍历查找所需要的元素。

iterparse的性能与SAX相当，但是其API却更加有用：iterparse会循序地构建树；而利用SAX时，你还得自己完成树的构建工作。

# HTMLParser

这是一个用来解析HTML的模块，当我们扒取完页面源码以后就可以通过其进行解析

```python
>>> class MyHTMLParser(HTMLParser):
    def handle_starttag(self, tag, attrs):  # 开始标签触发
        print("Start tag:", tag)
        for attr in attrs:
            print("     attr:", attr)

    def handle_endtag(self, tag):  # 结束标签触发
        print("End tag  :", tag)

    def handle_data(self, data):   # 开始、结束标签中间触发
        print("Data     :", data)

    def handle_comment(self, data):  # 注释触发
        print("Comment  :", data)

    def handle_entityref(self, name):  # 实体名称触发 如 >
        c = chr(name2codepoint[name])
        print("Named ent:", c)

    def handle_charref(self, name):  # 实体编号触发 如 >和>
        if name.startswith('x'):
            c = chr(int(name[1:], 16))
        else:
            c = chr(int(name))
        print("Num ent  :", c)

    def handle_decl(self, data):  # DOC 触发
        print("Decl     :", data)

        
>>> parser = MyHTMLParser()
>>> 
>>> parser.feed('<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">')
Decl     : DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd"
>>> parser.feed('<img src="python-logo.png" alt="The Python logo">')
Start tag: img
     attr: ('src', 'python-logo.png')
     attr: ('alt', 'The Python logo')
>>> parser.feed('<style type="text/css">#python { color: green }</style>')
Start tag: style
     attr: ('type', 'text/css')
Data     : #python { color: green }
End tag  : style
>>> parser.feed('<!-- a comment --><!--[if IE 9]>IE-specific content<![endif]-->')
Comment  :  a comment 
Comment  : [if IE 9]>IE-specific content<![endif]
>>> parser.feed('>>>')  # 这里不知道为什么触发的是data
Data     : >>>
```


