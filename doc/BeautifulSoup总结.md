# BeautifulSoup总结



# 1. 参考

[Beautiful Soup 4.2.0 文档](https://www.crummy.com/software/BeautifulSoup/bs4/doc.zh/#)

[Beautiful Soup Documentation （4.4.0 英文部分内容有别于4.2.0中文）](https://www.crummy.com/software/BeautifulSoup/bs4/doc/#)

 

[CSS 选择器参考手册](http://www.w3school.com.cn/cssref/css_selectors.ASP)

阮一峰 [CSS选择器笔记](http://www.ruanyifeng.com/blog/2009/03/css_selectors.html)

 

# 2. 安装

pip install beautifulsoup4
pip install lxml
pip install html5lib

# 3. 使用

## 3.1将字符串或文件句柄传入BeautifulSoup 的构造方法

html_doc = """
<html><head><title>The Dormouse's story</title></head>
<body>
<p class="title"><b>The Dormouse's story</b></p>
<p class="story">Once upon a time there were three little sisters; and their names were
<a href="http://example.com/elsie" class="sister" id="link1">Elsie</a>,
<a href="http://example.com/lacie" class="sister" id="link2">Lacie</a> and
<a href="http://example.com/tillie" class="sister" id="link3">Tillie</a>;
and they lived at the bottom of a well.</p>
<p class="story">...</p>
"""

from bs4 import BeautifulSoup
soup = BeautifulSoup(html_doc)


with open("index.html") as fp:
    soup = BeautifulSoup(fp, 'lxml')

支持文档类型：html, xml和html5
默认优先顺序：lxml, html5lib和html.parser，最好显式指定文档解析器
区别：

|               | `BeautifulSoup("<a></p>", "解析器")`                    |                    |                        |                                     |
| ------------- | ------------------------------------------------------- | ------------------ | ---------------------- | ----------------------------------- |
| `lxml`        | `<html><body><a></a></body></html>`                     | 忽略没有开头的</p> | `补全部分<html>`       | 推荐，高效，唯一支持xml，需自行安装 |
| `html5lib`    | `<html><head></head><body><a><p></p></a></body></html>` | 没有忽略           | `补全所有，包括<head>` | 容错性最好，速度慢                  |
| `html.parser` | `<a></a>`                                               | 忽略没有开头的</p> | `不作补全`             | 内置标准库                          |

## 3.2 格式化显示文档树

print(soup.prettify())

默认formatter="minimal"，专注生成有效的HTML/XML

formatter=None，方便观察实际显示效果

```
In [70]: print(soup.prettify(formatter=None))
<html>
 <body>
  <p>
   Il a dit <<Sacré bleu!>>   #可能生成无效HTML
  </p>
 </body>
</html>
```

## 3.3 简单用法 

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 In [86]: soup.a    #定位tag
 2 Out[86]: <a class="sister" href="http://example.com/elsie" id="link1">Elsie</a>
 3 
 4 In [87]: soup.a.name
 5 Out[87]: 'a'
 6 
 7 In [88]: soup.a.string    #The string argument is new in Beautiful Soup 4.4.0. In earlier versions it was called text
 8 Out[88]: u'Elsie'
 9 
10 In [89]: soup.a.attrs  #字典属性
11 Out[89]: {'class': ['sister'], 'href': 'http://example.com/elsie', 'id': 'link1'}
12 
13 In [90]: soup.a.get('class')  #在Beautiful Soup中多值属性的返回类型是list，最常见的多值的属性是 class
14 Out[90]: ['sister']
15 
16 In [91]: soup.a['class']
17 Out[91]: ['sister']
18 
19 In [92]: soup.a.get('href')  #一般属性返回字符串，若无return None
20 Out[92]: 'http://example.com/elsie'
21 
22 In [93]: soup.a['href']
23 Out[93]: 'http://example.com/elsie'
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

## 3.4 主要对象种类 

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
1 In [100]: type(soup)
2 Out[100]: bs4.BeautifulSoup
3 
4 In [101]: type(soup.a)
5 Out[101]: bs4.element.Tag
6 
7 In [103]: type(soup.a.string)  #可以通过unicode()方法转换成Unicode字符串
8 Out[103]: bs4.element.NavigableString
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

BeautifulSoup 对象表示的是一个文档的全部内容.大部分时候,可以把它当作 Tag 对象,它支持 遍历文档树 和 搜索文档树 中描述的大部分的方法。没有attribute，但有name特殊值为u'[document]'。

 

In [120]: isinstance(soup.a.string,unicode)
Out[120]: True

In [401]: from bs4 import NavigableString
In [402]:  isinstance(soup.a.string, NavigableString)
Out[402]: True

如果想在Beautiful Soup之外使用 NavigableString 对象,需要调用 unicode() 方法,将该对象转换成普通的Unicode字符串,否则就算Beautiful Soup已方法已经执行结束,该对象的输出也会带有对象的引用地址.这样会浪费内存.

 

## 3.5 遍历文档树（定位tag，提取name，string，attrs{}）

### 3.5.1 直接定位单个tag

soup.a    #匹配的第一个tag

soup.head.title  #相当于 soup.find('head').find('title')， find直接返回第一个结果

### 3.5.2 find_all()

find_all(self, name=None, attrs={}, recursive=True, text=None, limit=None, **kwargs)

name

soup.find_all("a") #实际是第一个位置参数name="a"，完全匹配，等同soup.select('a')
soup.find_all(re.compile('^b'))  #name部分匹配开头b： b body

attrs

\#传入非方法参数名，将被认定为tag的属性

soup.find_all(id="link2")  #tag的id属性
soup.find_all(href=re.compile("elsie"))  #tag的href属性

string/text
soup.find_all(string=re.compile("sisters"))  #只有一个NavigableSring子节点的tag的string属性 #The string argument is new in Beautiful Soup 4.4.0. In earlier versions it was called text

混合使用name, attrs和string

soup.find_all("a", id='link1', string="Elsie", ) #第一个name，再加任意个属性
soup.find_all("a", "sister")  #name+attrs，只有class_才可以省略关键字！！！

列表的任一个条件

soup.find_all(["a", "b"])
soup.find_all(id=['link1','link2'])  
soup.find_all(class_=['sister','story'])
soup.find_all(href=re.compile("elsie|lacie"))  #|或

特殊属性和通用解决方法

soup.find_all(class_="sister")  #使用到class关键字，改写为class_ #class_是多值属性，所以class_="strikeout"也能匹配到实际的class="body strikeout"
soup.find_all(data-foo="value") #HTML5中的 data-* 属性也会报错！！！
find_all(name=”email”)  #原文<input name=”email”/>’，只会返回空，因为没有name为email的tag
soup.find_all(attrs={"data-foo": "value"})  #通用解决方法

其他参数和用法

soup.find_all("a", limit=2) #限制了返回数量
soup.html.find_all("title", recursive=False)  #限制只查找直接子节点

soup.find_all("a") #所有.find_all都可以省略，缩写soup("a")

 

## 3.6 获取所有文本内容

soup.get_text("|", strip=True) #可以添加分隔符，去除前后空白（含\n）

[text for text in soup.strings] #可以观察到u'\n'，或通过for循环print(repr(string))
[text for text in soup.stripped_strings]

 

## 3.7 节点层次关系

###  

| soup.a.contents(children)                   | #直接子节点列表（生成器）                                    |
| ------------------------------------------- | ------------------------------------------------------------ |
| soup.a.descendants                          | #递归循环子孙节点生成器                                      |
| soup.a.string                               | #只有一个NavigableString 类型子节点，此类子节点没有contents属性！！！ |
| soup.a.strings（stripped_strings）          | #生成器（去除多余空白内容）                                  |
| .parent（s）                                |                                                              |
| .next_sibling（s）和 .previous_sibling（s） | #同父同层   经常出现空白（‘\n’）                             |
| .next_element（s）和.previous_element（s）  | #按照文档树从外到内解析tag和string                           |

 

## 3.8 CSS selector

soup.select("title")  #name节点列表


soup.select("p:nth-of-type(3)")  #p的父元素的第三个p tag，从1开始



[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
for i in soup.select('tbody tr'):  #即先定位父节点，再选择父节点的第三个td子节点
    print i.select("td:nth-of-type(3)")[0].text

#只实现了一个 nth-of-type？？？
---> 15     print i.select("td:nth-child(3)").text

c:\program files\anaconda2\lib\site-packages\bs4\element.pyc in select(self, selector, _candidate_generator, limit)
   1435                 else:
   1436                     raise NotImplementedError(
-> 1437                         'Only the following pseudo-classes are implemented: nth-of-type.')
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 

soup.select("html head title")  #通过tag标签逐层查找，可以跳空
soup.select("head > title")  #直接子标签 
soup.select("p > #link1")   # id=link1

soup.select(".sister")  #class
soup.select("#link1")  #id
soup.select('[href]')  #包含属性
soup.select('[href="http://example.com/elsie"]')  #属性value完全匹配
soup.select('[href*=".com/el"]')  #部分匹配
soup.select('[href$="tillie"]')  #结尾匹配

soup.select("a.sister")  #以上都可以搭配name一起使用
soup.select("#link1,#link2")  #列表任意一个

i.select('span, div'):  #列表，先遍历span，再遍历div！！！



[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
# 代理IP http://www.goubanjia.com/free/gngn/index1.shtml
# 106.39.160.135:8888 正确
# 39.160.1358888106.    错误     
# <td class="ip">
    # <div style="display:inline-block;">10</div>
    # <div style="display:inline-block;">6.</div>
    # <p style="display: none;">39</p>
    # <span>39</span>
    # ...
    # :
    # <span class="port HBBAE">8888</span></td>        

# <p style="display: none;">39</p> 为干扰项

for i in soup.select('tbody tr td.ip'):
    rst = ''
    # for k in i.select('span, div'):  #先遍历span，再遍历div！！！      
        # rst += k.text
    # print rst
    for j in  i.contents:
        if j.name in ['span', 'div']:  #网页源代码部分hide干扰数字
            rst += j.text
        if j == ':':
            rst += ':'
    ip, port =  rst.split(':')
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 
soup.select_one(".sister")  #匹配的第一个

soup.select("#link1 ~ a")  #id=link1的所有兄弟，且满足后半部分的 a
soup.select("#link1 + a"）  #id=link1的第一个兄弟，依旧是list

#  4. 其他

https://www.crummy.com/software/BeautifulSoup/bs4/doc.zh/#id85

如果没有合适过滤器,那么还可以定义一个方法,方法只接受一个元素参数 [[4\]](https://www.crummy.com/software/BeautifulSoup/bs4/doc.zh/#id85) ,如果这个方法返回 `True` 表示当前元素匹配并且被找到,如果不是则反回 `False`

[[4\]](https://www.crummy.com/software/BeautifulSoup/bs4/doc.zh/#id31)元素参数,HTML文档中的一个tag节点,不能是文本节点