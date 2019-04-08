# Beautiful Soup基础总结

![96](https://upload.jianshu.io/users/upload_avatars/3004516/90173860691b.jpeg?imageMogr2/auto-orient/strip|imageView2/1/w/96/h/96)

 

[Jeason_zhao](https://www.jianshu.com/u/f9dc24fde244)

 

关注

2017.04.20 12:10*



最后编辑于 2017.12.06 22:24

 

字数 261

 

阅读 633

评论 0

喜欢 0

# 1.安装

BS安装： `pip install beautifulsoup4`
解析器安装：`pip install lxml`

# 2.预热

```
from bs4 import BeautifulSoup
soup = BeautifulSoup(read_file('qiushibaike.html'), 'html.parser')
print type(soup) # <class 'bs4.BeautifulSoup'>
print dir(soup)
# ['ASCII_SPACES', 'DEFAULT_BUILDER_FEATURES', 'HTML_FORMATTERS', 'NO_PARSER_SPECIFIED_WARNING', 'ROOT_TAG_NAME', 'XML_FORMATTERS',
#  '__call__', '__class__', '__contains__', '__copy__', '__delattr__', '__delitem__', '__dict__', '__doc__', '__eq__', '__format__',
# '__getattr__', '__getattribute__', '__getitem__', '__getstate__', '__hash__', '__init__', '__iter__', '__len__', '__module__', '__ne__',
#  '__new__', '__nonzero__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__setitem__', '__sizeof__', '__str__', '__subclasshook__',
#  '__unicode__', '__weakref__', '_all_strings', '_attr_value_as_string', '_attribute_checker', '_check_markup_is_url', '_feed', '_find_all', '_find_one',
#  '_formatter_for_name', '_is_xml', '_lastRecursiveChild', '_last_descendant', '_most_recent_element', '_popToTag', '_select_debug', '_selector_combinators',
#  '_should_pretty_print', '_tag_name_matches_and', 'append', 'attribselect_re', 'attrs', 'builder', 'can_be_empty_element', 'childGenerator', 'children', 'clear',
#  'contains_replacement_characters', 'contents', 'currentTag', 'current_data', 'declared_html_encoding', 'decode', 'decode_contents', 'decompose', 'descendants',
#  'encode', 'encode_contents', 'endData', 'extract', 'fetchNextSiblings', 'fetchParents', 'fetchPrevious', 'fetchPreviousSiblings', 'find', 'findAll', 'findAllNext',
#  'findAllPrevious', 'findChild', 'findChildren', 'findNext', 'findNextSibling', 'findNextSiblings', 'findParent', 'findParents', 'findPrevious', 'findPreviousSibling',
#  'findPreviousSiblings', 'find_all', 'find_all_next', 'find_all_previous', 'find_next', 'find_next_sibling', 'find_next_siblings', 'find_parent', 'find_parents',
# 'find_previous', 'find_previous_sibling', 'find_previous_siblings', 'format_string', 'get', 'getText', 'get_text', 'handle_data', 'handle_endtag', 'handle_starttag',
#  'has_attr', 'has_key', 'hidden', 'index', 'insert', 'insert_after', 'insert_before', 'isSelfClosing', 'is_empty_element', 'is_xml', 'known_xml', 'markup', 'name',
#  'namespace', 'new_string', 'new_tag', 'next', 'nextGenerator', 'nextSibling', 'nextSiblingGenerator', 'next_element', 'next_elements', 'next_sibling', 'next_siblings',
#  'object_was_parsed', 'original_encoding', 'parent', 'parentGenerator', 'parents', 'parse_only', 'parserClass', 'parser_class', 'popTag', 'prefix', 'preserve_whitespace_tag_stack',
#  'preserve_whitespace_tags', 'prettify', 'previous', 'previousGenerator', 'previousSibling', 'previousSiblingGenerator', 'previous_element', 'previous_elements', 'previous_sibling',
#  'previous_siblings', 'pushTag', 'quoted_colon', 'recursiveChildGenerator', 'renderContents', 'replaceWith', 'replaceWithChildren', 'replace_with', 'replace_with_children',
#  'reset', 'select', 'select_one', 'setup', 'string', 'strings', 'stripped_strings', 'tagStack', 'tag_name_re', 'text', 'unwrap', 'wrap']

# 文档内容被转化为Unicode编码类型
print type(soup.title.name)

# print soup.prettify()        # 以标准格式展示出html内容
print soup.title               # 输出带有标签的元素 : <title>糗事百科 - 超搞笑的原创糗事笑话分享社区</title>
print type(soup.title)         # <class 'bs4.element.Tag'>
print soup.title.name          # title
print soup.title.string        # 糗事百科 - 超搞笑的原创糗事笑话分享社区

# head (注：<由于网站有未闭合的标签，导致该结果输出可能是meta>
print soup.title.parent.name
# <p>战争女神希维尔的回旋之刃</p> 注：为什么就输出一个<p>元素呢，是因为该形式的属性访问只输出匹配到的第一个元素
print soup.p
# 该方法的返回结果为一个列表，包含所有的a标签的元素 [<a href="/">\n<h1>\u7cd7\u4e8b\u767e\u79d1</h1>\n</a>, <a href="/" id="highlight" rel="nofollow">\u70ed\u95e8</a>,....]
print soup.find_all('a')

print soup.find(id="highlight") # 返回到匹配到的单个元素<a href="/" id="highlight" rel="nofollow">热门</a>
print soup.find(id='zhaojunwei') # 如果未匹配到结果，返回值为None
print soup.find(class_="share-qq") # 该方法也是只返回匹配到的第一个元素

print '--------------------'
for link in soup.find_all('a'):
    print(link.get('href')) # 输出标签元素的href属性。
```

# 3.对象介绍

### 1> . Tag对象

```
from bs4 import BeautifulSoup
soup = BeautifulSoup(read_file('qiushibaike.html'), 'html.parser')
print soup.title               # 输出带有标签的元素 : <title>糗事百科 - 超搞笑的原创糗事笑话分享社区</title>
print type(soup.title)         # <class 'bs4.element.Tag'>
```

### 2>.NavigableString对象

```
from bs4 import BeautifulSoup
soup = BeautifulSoup(read_file('qiushibaike.html'), 'html.parser')
print type(soup.find(id='c-118910070').findChild('i', class_='number').string)  # <class 'bs4.element.NavigableString'>
```

### 3>.BeautifulSoup,Comment对象

经过BeautifulSoup类实例化的对象就是Beautifulsoup对象类型，Comment对象为实例化后的BeautifulSoup中备注的标签元素

# 4.属性方法介绍

```
print soup.find(id="highlight").attrs    # {u'href': u'/', u'id': u'highlight', u'rel': [u'nofollow']}

print soup.find(class_="stats-buttons")['class'] # [u'stats-buttons', u'bar', u'clearfix'] 多值属性

print soup.find(id='c-118910070')
print soup.find(id='c-118910070').string  # 该内容为空的原因为当前节点有很多子节点，.string不知道要输出哪个子节点的内容，可以.strings属性来输出所有内容
print soup.find(id='c-118910070').strings #.strings属性的结果为一个生成器对象，遍历可输出所有子节点的.string属性结果
# for j in [i for i in soup.find(id='c-118910070').strings]: #stripped_strings 会去掉多余的空白
#     print j
print type(soup.find(id='c-118910070').findChild('i', class_='number').string)  # <class 'bs4.element.NavigableString'>


# print soup.head.title   # <title>糗事百科 - 超搞笑的原创糗事笑话分享社区</title>

#[u'\n', <meta content="text/html; charset=unicode-escape" http-equiv="Content-Type"/>, u'\n',
#  <meta content="chrome=1,IE=edge" http-equiv="X-UA-Compatible"/>, u'\n', <meta content="webkit" name="renderer"/>, u'\n',
#  <meta content="pc" name="applicable-device"/>, u'\n', <title>\u7cd7\u4e8b\u767e\u79d1 - \u8d85\u641e\u7b11\u7684\u539f\u521b\u7cd7\u4e8b\u7b11\u8bdd\u5206\u4eab\u793e\u533a</title>,
#  u'\n', <meta content="\u7b11\u8bdd,\u641e\u7b11,\u7b11\u8bdd\u5927\u5168 \u7206\u7b11,\u7b11\u8bdd\u5927\u5168,\u7cd7\u4e8b\u767e\u79d1,\u5e7d\u9ed8\u7b11\u8bdd,\u7206\u7b11\u7b11\u8bdd" name="keywords"/>,
# u'\n', <meta content="\u7cd7\u4e8b\u767e\u79d1\u662f\u4e00\u4e2a\u539f\u521b\u7684\u7cd7\u4e8b\u7b11\u8bdd\u5206\u4eab\u793e\u533a,\u7cd7\u767e\u7f51\u53cb\u5206\u4eab\u7684\u641e\u7b11\u6bb5\u5b50\u3001\u641e\u7b11\u56fe\u7247\u5927\u5168,\u90fd\u662f\u7cd7\u53cb\u6700\u73cd\u8d35\u7684\u5f00\u5fc3\u7ecf\u5386,\u7206\u7b11\u7cd7\u4e8b\u7b11\u8bdd\u53ea\u5728\u7cd7\u4e8b\u767e\u79d1\uff01" name="description"/>, u'\n', <meta content="format=xhtml;url=//www.qiushibaike.com/" http-equiv="mobile-agent"/>, u'\n', <meta content="format=html5;url=//www.qiushibaike.com/" http-equiv="mobile-agent"/>, u'\n', <link href="//www.qiushibaike.com" rel="canonical"/>,
#  u'\n', <meta content="noarchive" name="robots"/>,
# u'\n', <link href="//static.qiushibaike.com/css/dist/web/app.min.css?v=ad7801a2f6230c71cfc1ee1fd6b1d2b4" media="screen, projection" rel="stylesheet" type="text/css"/>,
#  u'\n', <script type="text/javascript">\n            \n// Baidu Automatic push content\nvar _hmt = _hmt || [];\n(function() {\nvar hm = document.createElement("script");\nhm.src = "https://hm.baidu.com/hm.js?2670efbdd59c7e3ed3749b458cafaa37";\nvar s = document.getElementsByTagName("script")[0];\ns.parentNode.insertBefore(hm, s);\n})();\n\n        </script>, u'\n']
# print soup.head.contents #所有子节点以列表的形式输出

# for i in soup.head.children:
    # soup.head.children 是一个生成器，包含所有的子节点，内容和soup.head.contents相同
    # print i

print soup.head.descendants # <generator object descendants at 0x0000000003F95900>
# for i in soup.head.descendants:
    #.descendants 属性可以对所有tag的子孙节点进行递归循环
    # print i




# print soup.find('div', class_='thumb')
# print soup.find('div', class_='thumb').parent  # 父节点的所有内容
#实际文档中的tag的 .next_sibling 和 .previous_sibling 属性通常是字符串或空白. 为什么呢，是因为前后元素标签之间的顿号和换行符
print soup.find('div', class_='stats').find(class_='stats-vote').next_sibling     # 该属性返回的是元素的兄弟元素
print soup.find('div', class_='stats').find(class_='stats-vote').next_siblings    # 该属性返回的是元素的兄弟元素们的生成器
# next_element属性指向解析过程中下一个被解析的对象(字符串或tag),结果可能与 .next_sibling 相同,但通常是不一样的.
# 通过 .next_elements 和 .previous_elements 的迭代器就可以向前或向后访问文档的解析内容,就好像文档正在被解析一样
print soup.find('div', class_='stats').find(class_='stats-vote').next_element.next_element     # 该属性返回的是元素的下一个元素
# print dir(soup.find('div', class_='stats').find(class_='stats-vote'))
print soup.find('div', class_='stats').find(class_='stats-vote').previous_sibling
print soup.find('div', class_='stats').find(class_='stats-vote').previous_element





# for tag in soup.find_all(re.compile(r'^t')):
#     #所有一t开头的标签元素
#     print tag.name
# for tag in soup.find_all(['a', 'b']):
#     #所有a元素和所有b元素
#     print tag.name
## 如果没有合适过滤器,那么还可以定义一个方法,方法只接受一个元素参数 [4] ,如果这个方法返回 True 表示当前元素匹配并且被找到,如果不是则反回 False
def has_class_but_no_id(tag):
    return tag.has_attr('class') and not tag.has_attr('id')
print soup.find_all(has_class_but_no_id)
```

# 5.find_all()方法介绍

```
    class ResultSet(list):
         """A ResultSet is just a list that keeps track of the SoupStrainer
          that created it."""
          def __init__(self, source, result=()):
               super(ResultSet, self).__init__(result)
               self.source = source

    def _find_all(self, name, attrs, text, limit, generator, **kwargs):
        "Iterates over a generator looking for things that match."

        if text is None and 'string' in kwargs:
            text = kwargs['string']
            del kwargs['string']

        if isinstance(name, SoupStrainer):
            strainer = name
        else:
            strainer = SoupStrainer(name, attrs, text, **kwargs)

        if text is None and not limit and not attrs and not kwargs:
            if name is True or name is None:
                # Optimization to find all tags.
                result = (element for element in generator
                          if isinstance(element, Tag))
                return ResultSet(strainer, result)
            elif isinstance(name, basestring):
                # Optimization to find all tags with a given name.
                result = (element for element in generator
                          if isinstance(element, Tag)
                            and element.name == name)
                return ResultSet(strainer, result)
        results = ResultSet(strainer)
        while True:
            try:
                i = next(generator)
            except StopIteration:
                break
            if i:
                found = strainer.search(i)
                if found:
                    results.append(found)
                    if limit and len(results) >= limit:
                        break
        return results
    def find_all(self, name=None, attrs={}, recursive=True, text=None, limit=None, **kwargs):
        """Extracts a list of Tag objects that match the given
        criteria.  You can specify the name of the Tag and any
        attributes you want the Tag to have.

        The value of a key-value pair in the 'attrs' map can be a
        string, a list of strings, a regular expression object, or a
        callable that takes a string and returns whether or not the
        string matches for some custom definition of 'matches'. The
        same is true of the tag name."""

        generator = self.descendants
        if not recursive:
            generator = self.children
        return self._find_all(name, attrs, text, limit, generator, **kwargs)
    findAll = find_all       # BS3
    findChildren = find_all  # BS2
```

可以使用tag的name来查询：`soup.find_all('div')`
可以使用keyword关键字：`soup.find_all(id='highlight')``soup.find_all(href=re.compile("pictures"))`
`soup.find_all(href=re.compile("elsie"), id='link1')`
可以使用css查询：`soup.find('div', class_='stats')`
可以使用string参数来查询:

```
soup.find_all(string="Elsie")
# [u'Elsie']

soup.find_all(string=["Tillie", "Elsie", "Lacie"])
# [u'Elsie', u'Lacie', u'Tillie']

soup.find_all(string=re.compile("Dormouse"))
[u"The Dormouse's story", u"The Dormouse's story"]

def is_the_only_string_within_a_tag(s):
    ""Return True if this string is the only child of its parent tag.""
    return (s == s.parent.string)

soup.find_all(string=is_the_only_string_within_a_tag)
# [u"The Dormouse's story", u"The Dormouse's story", u'Elsie', u'Lacie', u'Tillie', u'...']
```

limit参数，查询的结果达到limit的值后就不在搜索
如果只想搜索tag的直接子节点,可以使用参数 recursive=False

# 6.find(),find_parent(),find_parents(),find_next_siblings(),find_next_sibling(),find_previous_siblings(),find_previous_sibling(),

find_all_next(),find_next(),find_all_previous(),find_previous()不在一一介绍。

# 7.css选择器

在 Tag 或 BeautifulSoup 对象的 .select() 方法中传入字符串参数, 即可使用CSS选择器的语法找到tag:

```
soup.select("body a")
# [<a class="sister" href="http://example.com/elsie" id="link1">Elsie</a>,
#  <a class="sister" href="http://example.com/lacie"  id="link2">Lacie</a>,
#  <a class="sister" href="http://example.com/tillie" id="link3">Tillie</a>]

soup.select("html head title")
# [<title>The Dormouse's story</title>]
soup.select("#link1")
# [<a class="sister" href="http://example.com/elsie" id="link1">Elsie</a>]
soup.select(".sister")
# [<a class="sister" href="http://example.com/elsie" id="link1">Elsie</a>,
#  <a class="sister" href="http://example.com/lacie" id="link2">Lacie</a>,
#  <a class="sister" href="http://example.com/tillie" id="link3">Tillie</a>]
soup.select_one(".sister")
# <a class="sister" href="http://example.com/elsie" id="link1">Elsie</a>
```

小礼物走一走，来简书关注我