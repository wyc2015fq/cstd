# Python爬虫--beautifulsoup 4 用法 - 默槑 - 博客园







# [Python爬虫--beautifulsoup 4 用法](https://www.cnblogs.com/modaidai/p/6962328.html)





Beautiful Soup将复杂HTML文档转换成一个复杂的树形结构,

每个节点都是Python对象,所有对象可以归纳为4种: Tag , NavigableString , BeautifulSoup , Comment



一、Tag:

Tag对象与XML或HTML原生文档中的tag相同，Tag有很多方法和属性，最重要的是name和attributes
soup = BeautifulSoup('<b class="boldest">Extremely bold</b>')
tag = soup.b
type(tag)
　　　name:

　　　　　　每个Tag都有自己名字，通过.name来获取

tag.name

　　　　　　也可以改变这个名字，将影响所有通过当前Beautiful Soup对象生成的HTML文档:

tag.name = "blockquote"　　　　#会改变它的名字

　　　attribute：



　　　　　　一个tag可能有很多个属性. tag <b class="boldest"> 有一个 “class” 的属性,

　　　　　　值为 “boldest” . tag的属性的操作方法与字典相同:


tag['class']


　　　　　　也可以直接”点”取属性, 比如: .attrs :


　tag.attrs
Tag的属性操作方法与字典一样，也是键对应值，可以向下面一样
tag['class'] = 'verybold'
tag['id'] = del tag['class']
del tag['id']
tag　　　　　　　　# <blockquote>Extremely bold</blockquote>

删除了就没有了：tag['class']
# KeyError: 'class'
print(tag.get('class'))
# None

　　　　多值属性
HTML 4定义了一系列可以包含多个值的属性.在HTML5中移除了一些,却增加更多.最常见的多值的属性是
class (一个tag可以有多个CSS的class). 还有一些属性 rel , rev , accept-charset
,headers , accesskey . 在Beautiful Soup中多值属性的返回类型是list:
css_soup = BeautifulSoup('<p class="body strikeout"></p>')
css_soup.p['class']　　　　　　　　　　　　　　　　　　# ["body", "strikeout"]

css_soup = BeautifulSoup('<p class="body"></p>')
css_soup.p['class']　　　　　　　　　　　　　　　　　　# ["body"]如果某个属性看起来好像有多个值,但在任何版本的HTML定义中都没有被定义为多值属性,
那么Beautiful Soup会将这个属性作为字符串返回id_soup = BeautifulSoup('<p id="my id"></p>')
id_soup.p['id']　　　　　　　　　　　　　　　　　　　　　# 'my id'


将tag转换成字符串时,多值属性会合并为一个值


rel_soup = BeautifulSoup('<p>Back to the <a rel="index">homepage</a></p>')
rel_soup.a['rel']　　　　　　　　　　　　　　# ['index']
rel_soup.a['rel'] = ['index', 'contents']
print(rel_soup.p)　　　　　　　　　　　　　　# <p>Back to the <a rel="index contents">homepage</a></p>



如果转换的文档是XML格式,那么tag中不包含多值属性


xml_soup = BeautifulSoup('<p class="body strikeout"></p>', 'xml')
xml_soup.p['class']　　　　　　　　　　　　　# u'body strikeout'


二、NavigableString

字符串常被包含在tag内.Beautiful Soup用 NavigableString 类来包装tag中的字符串:


tag.string　　　　　　　　　 # u'Extremely bold'
type(tag.string)　　　　　　# <class 'bs4.element.NavigableString'>


　　一个 NavigableString 字符串与Python中的Unicode字符串相同,并且还支持包含在 

遍历文档树和 搜索文档树 中的一些特性. 


unicode_string = encode('utf-8')
unicode_string　　　　　　　　　　　　# u'Extremely bold'
type(unicode_string)　　　　　　　　 # <type 'unicode'>


tag中包含的字符串不能编辑,但是可以被替换成其它的字符串,用 replace_with([)](https://www.crummy.com/software/BeautifulSoup/bs4/doc.zh/index.html#replace-with) 方法:


tag.string.replace_with("No longer bold")
tag　　　　　　　　　　　　　　　　　　# <blockquote>No longer bold</blockquote>


三、BeautifulSoupBeautifulSoup 对象表示的是一个文档的全部内容.大部分时候,
可以把它当作 Tag 对象,它支持 遍历文档树 和 搜索文档树 中描述的大部分的方法.

因为 BeautifulSoup 对象并不是真正的HTML或XML的tag,所以它没有name和attribute属性.

但有时查看它的 .name 属性是很方便的,所以 BeautifulSoup 对象包含了一个值为 “[document]” 的特殊属性 .name


soup.name　　　　　　　　# u'[document]'


四、Comment

Tag , NavigableString , BeautifulSoup 几乎覆盖了html和xml中的所有内容,

但是还有一些特殊对象.容易让人担心的内容是文档的注释部分:


markup = "<b><!--Hey, buddy. Want to buy a used parser?--></b>"
soup = BeautifulSoup(markup)
comment = soup.b.string
type(comment)　　　　　　　　# <class 'bs4.element.Comment'>



Comment 对象是一个特殊类型的 NavigableString 对象:


comment　　　　　　　　　　　　# u'Hey, buddy. Want to buy a used parser'


但是当它出现在HTML文档中时, Comment 对象会使用特殊的格式输出:


print(soup.b.prettify())
# <b>
#  <!--Hey, buddy. Want to buy a used parser?-->
# </b>


Beautiful Soup中定义的其它类型都可能会出现在XML的文档中: CData , ProcessingInstruction, Declaration , 

Doctype .与 Comment 对象类似,这些类都是 NavigableString 的子类,只是添加了一些额外的方法的字符串独享.

下面是用CDATA来替代注释的例子:


from bs4 import CData
cdata = CData("A CDATA block")
comment.replace_with(cdata)

print(soup.b.prettify())
# <b>
#  <![CDATA[A CDATA block]]>
# </b>

一、遍历文档树















