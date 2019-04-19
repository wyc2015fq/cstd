# 用python的BeautifulSoup分析html - 宇宙浪子的专栏 - CSDN博客
2014年12月15日 14:42:16[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：327标签：[python																[beautifulSoup](https://so.csdn.net/so/search/s.do?q=beautifulSoup&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[Python](https://blog.csdn.net/bluehawksky/article/category/2542577)
转自：[http://www.cnblogs.com/twinsclover/archive/2012/04/26/2471704.html](http://www.cnblogs.com/twinsclover/archive/2012/04/26/2471704.html)
序言
　　之前用python爬取网页的时候，一直用的是regex或者自带的库sgmllib里的SGMLParser。但是遇到复杂一点的情况时，SGMLParser往往就不那么给力了！（哈，难道说我 too native了？毕竟beautifulSoup是继承sgmlparser的么~）所以，我寻寻觅觅寻寻觅觅，发现了BeautifulSoup这么个玩意。BeautifulSoup提供了很人性化的parser tree，有了它，我们可以简单的抽取出tagname, attrs, text等等等等...
　　install什么的，看这里 -> [http://www.crummy.com/software/BeautifulSoup/](http://www.crummy.com/software/BeautifulSoup/)
入门
（ps:其实入门什么的看官方文档是最好的了，这里只是记录一下简单的用法。）
（official document link : [http://www.crummy.com/software/BeautifulSoup/bs3/documentation.zh.html](http://www.crummy.com/software/BeautifulSoup/bs3/documentation.zh.html)）
　　首先先介绍实际工作中最常用的几个方法：
　　举例的html代码（就用官方例子好了）:
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 <html>
 2     <head>
 3         <title>Page title</title>
 4     </head>
 5     <body>
 6         <p id="firstpara" align="center">
 7         This is paragraph<b>one</b>.
 8         </p>
 9         <p id="secondpara" align="blah">
10         This is paragraph<b>two</b>.
11         </p>
12      </body>
13 </html>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
0、初始化：
```
1 soup = BeautifulSoup(html) # html为html源代码字符串，type(html) == str
```
1、用tag获取相应代码块的剖析树：
　　既然要分析html，首先要找到对我们有用的tag块，beautiful提供了非常便利的方式。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
#当用tag作为搜索条件时，我们获取的包含这个tag块的剖析树：
#<tag><xxx>ooo</xxx></tag>
#这里获取head这个块
head = soup.find('head')
# or
# head = soup.head
# or
# head = soup.contents[0].contents[0]
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
运行后，我们会得到：
```
1 <head>
2     <title>Page title</title>
3 </head>
```
　　这里小灯我还是推荐使用第二种方法，find方法在当前tag剖析树（当前这个html代码块中）寻找符合条件的子树并返回。find方法提供多种查询方式，包括用喜闻乐见的regex哦~之后会详细介绍。
　　contents属性是一个列表，里面保存了该剖析树的直接儿子。
如：
```
1 html = soup.contents[0] # <html> ... </html>
2 head = html.contents[0] # <head> ... </head>
3 body = html.contents[1] # <body> ... </body>
```
2、用contents[], parent, nextSibling, previousSibling寻找父子兄弟tag
　　为了更加方便灵活的分析html代码块，beautifulSoup提供了几个简单的方法直接获取当前tag块的父子兄弟。
假设我们已经获得了body这个tag块，我们想要寻找<html>, <head>, 第一个<p>, 第二个<p>这四个tag块：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
# body = soup.bodyhtml = body.parent # html是body的父亲
head = body.previousSibling # head和body在同一层，是body的前一个兄弟
p1 = body.contents[0] # p1, p2都是body的儿子，我们用contents[0]取得p1
p2 = p1.nextSibling # p2与p1在同一层，是p1的后一个兄弟, 当然body.content[1]也可得到
print p1.text
# u'This is paragraphone.'
print p2.text
# u'This is paragraphtwo.'
#  注意：1，每个tag的text包括了它以及它子孙的text。2，所有text已经被自动转
#为unicode，如果需要，可以自行转码encode(xxx)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　然而，如果我们要寻找祖先或者孙子tag怎么办呢？？ 用while循环吗？ 不， beautifulsoup已经提供了方法。
3、用find, findParent, findNextSibling, findPreviousSibling寻找祖先或者子孙 tag：
有了上面的基础，这里应该很好理解了，例如find方法（我理解和findChild是一样的），就是以当前节点为起始，遍历整个子树，找到后返回。
　　而这些方法的复数形式，会找到所有符合要求的tag，以list的方式放回。他们的对应关系是：find->findall, findParent->findParents, findNextSibling->findNextSiblings...
　　如：
```
1 print soup.findAll('p')
2 # [<p id="firstpara" align="center">This is paragraph <b>one</b>.</p>, <p id="secondpara" align="blah">This is paragraph <b>two</b>.</p>]
```
　　这里我们重点讲一下find的几种用法，其他的类比：
　　find(name=None, attrs={}, recursive=True, text=None, **kwargs)
（ps:只讲几种用法，完整请看官方link : [http://www.crummy.com/software/BeautifulSoup/bs3/documentation.zh.html#The%20basic%20find%20method:%20findAll%28name,%20attrs,%20recursive,%20text,%20limit,%20**kwargs%29](http://www.crummy.com/software/BeautifulSoup/bs3/documentation.zh.html#The%20basic%20find%20method:%20findAll%28name,%20attrs,%20recursive,%20text,%20limit,%20**kwargs%29)）
　　1) 搜索tag：
```
1 find(tagname)        # 直接搜索名为tagname的tag 如：find('head')
2 find(list)           # 搜索在list中的tag，如: find(['head', 'body'])
3 find(dict)           # 搜索在dict中的tag，如:find({'head':True, 'body':True})
4 find(re.compile('')) # 搜索符合正则的tag, 如:find(re.compile('^p')) 搜索以p开头的tag
5 find(lambda)         # 搜索函数返回结果为true的tag, 如:find(lambda name: if len(name) == 1) 搜索长度为1的tag
6 find(True)           # 搜索所有tag
```
　　2) 搜索属性(attrs)：
```
1 find(id='xxx')                                  # 寻找id属性为xxx的
2 find(attrs={id=re.compile('xxx'), algin='xxx'}) # 寻找id属性符合正则且algin属性为xxx的
3 find(attrs={id=True, algin=None})               # 寻找有id属性但是没有algin属性的
```
　　3) 搜索文字（text）:
注意，文字的搜索会导致其他搜索给的值如：tag, attrs都失效。
方法与搜索tag一致
　　4) recursive, limit:
　　recursive=False表示只搜索直接儿子，否则搜索整个子树，默认为True。
　　当使用findAll或者类似返回list的方法时，limit属性用于限制返回的数量，如findAll('p', limit=2)： 返回首先找到的两个tag
*4、用next，previous寻找上下文tag（少用）
　　这里我们主要看看next, next是取得当前的tag的下一个（按代码从上到下的顺序）tag块。这与contents是不一样的，千万别混淆了哦^ ^
　　我们举个栗子来看看
```
1 <a>
2     a
3     <b>b</b>
4     <c>c</c>
5 </a>
```
 我们看看next的实际效果：
```
1 a = soup.a
2 b = soup.b
3 n1 = b.next
4 n2 = n1.next
```
 输出一下：
```
1 print a.next
2 # u'a'
3 print n1
4 # u'b'
5 print n2
6 # <c>c</c>
```
所以，next仅仅是获取文档上的“下一个”的tag，和剖析树中的位置无关。
当然也有findNext和findAllNext方法。
至于previous， 表示上一个tag块，就类比一下吧~^ ^
=======
待续
