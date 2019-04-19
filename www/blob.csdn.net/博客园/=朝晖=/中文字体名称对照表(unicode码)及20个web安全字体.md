# 中文字体名称对照表(unicode码)及20个web安全字体 - =朝晖= - 博客园
# [中文字体名称对照表(unicode码)及20个web安全字体](https://www.cnblogs.com/dhcn/p/7105626.html)
在Web编码中，CSS默认应用的Web字体是有限的，虽然在新版本的CSS3，我们可以通过新增的@font-face属性来引入特殊的浏览器加载字体。但多数情况下，考虑各个因素的影响我们还是在尽量充分利用
|中文名|英文名|Unicode|Unicode 2|
|----|----|----|----|
| | | | |
|华文细黑|STHeiti Light [STXihei]|\534E\6587\7EC6\9ED1|华文细黑|
|华文黑体|STHeiti|\534E\6587\9ED1\4F53|华文黑体|
|华文楷体|STKaiti|\534E\6587\6977\4F53|华文楷体|
|华文宋体|STSong|\534E\6587\5B8B\4F53|华文宋体|
|华文仿宋|STFangsong|\534E\6587\4EFF\5B8B|华文仿宋|
|丽黑 Pro|LiHei Pro Medium|\4E3D\9ED1 Pro|丽黑 Pro|
|丽宋 Pro|LiSong Pro Light|\4E3D\5B8B Pro|丽宋 Pro|
|标楷体|BiauKai|\6807\6977\4F53|标楷体|
|苹果丽中黑|Apple LiGothic Medium|\82F9\679C\4E3D\4E2D\9ED1|苹果丽中黑|
|苹果丽细宋|Apple LiSung Light|\82F9\679C\4E3D\7EC6\5B8B|苹果丽细宋|
| | | | |
|新细明体|PMingLiU|\65B0\7EC6\660E\4F53|新细明体|
|细明体|MingLiU|\7EC6\660E\4F53|细明体|
|标楷体|DFKai-SB|\6807\6977\4F53|标楷体|
|黑体|SimHei|\9ED1\4F53|黑体|
|宋体|SimSun|\5B8B\4F53|宋体|
|新宋体|NSimSun|\65B0\5B8B\4F53|新宋体|
|仿宋|FangSong|\4EFF\5B8B|仿宋|
|楷体|KaiTi|\6977\4F53|楷体|
|仿宋_GB2312|FangSong_GB2312|\4EFF\5B8B_GB2312|仿宋_GB2312|
|楷体_GB2312|KaiTi_GB2312|\6977\4F53_GB2312|楷体_GB2312|
|微软正黑体|Microsoft JhengHei|\5FAE\x8F6F\6B63\9ED1\4F53|微软正黑体|
|微软雅黑|Microsoft YaHei|\5FAE\8F6F\96C5\9ED1|微软雅黑|
| | | | |
|隶书|LiSu|\96B6\4E66|隶书|
|幼圆|YouYuan|\5E7C\5706|幼圆|
|华文细黑|STXihei|\534E\6587\7EC6\9ED1|华文细黑|
|华文楷体|STKaiti|\534E\6587\6977\4F53|华文楷体|
|华文宋体|STSong|\534E\6587\5B8B\4F53|华文宋体|
|华文中宋|STZhongsong|\534E\6587\4E2D\5B8B|华文中宋|
|华文仿宋|STFangsong|\534E\6587\4EFF\5B8B|华文仿宋|
|方正舒体|FZShuTi|\65B9\6B63\8212\4F53|方正舒体|
|方正姚体|FZYaoti|\65B9\6B63\59DA\4F53|方正姚体|
|华文彩云|STCaiyun|\534E\6587\5F69\4E91|华文彩云|
|华文琥珀|STHupo|\534E\6587\7425\73C0|华文琥珀|
|华文隶书|STLiti|\534E\6587\96B6\4E66|华文隶书|
|华文行楷|STXingkai|\534E\6587\884C\6977|华文行楷|
|华文新魏|STXinwei|\534E\6587\65B0\9B4F|华文新魏|
在Web编码中，CSS默认应用的Web字体是有限的，虽然在新版本的[CSS3](http://lib.csdn.net/base/css3)，我们可以通过新增的@font-face属性来引入特殊的浏览器加载字体。
但多数情况下，考虑各个因素的影响我们还是在尽量充分利用这些默认调用的字体实现CSS的编写，这里整理了20个Web安全字体，让你无需任何顾虑的情况下畅快使用。
1,  Arial
微软公司的网页核心字体之一，最常用的sans serif字体，当字号很小时不容易阅读。但是，大写的“I”和小写的“l”是无法区别的，你可以考虑用Tahoma字体来替代。
（苹果系统没有这种字体，但有一种对应于Arial的字体叫Helvetica，它是MAC机上与Arial 字体最相似的WEB字体,是别一种非衬线字体.它是一种性能优良的打印字体,但在屏幕上表现不是很好，说道Helvetica字体，昨天发现了很多有意思的站点，主要是纪念Helvetica字体诞生50年而设的。）
![Arial](http://ipmtea.net/uploads/allimg/css/1011/0015225350-0.png)
CSS写法：`font-family: Arial, Helvetica, sans-serif;`
2, Arial Black
![ArialBlack](http://ipmtea.net/uploads/allimg/css/1011/0015223X7-1.png)
CSS写法：font-family: ‘Arial Black’, Gadget, sans-serif;
3, Arial Narrow
![ArialNarrow](http://ipmtea.net/uploads/allimg/css/1011/00152220G-2.png)
CSS写法：font-family: ‘Arial Narrow’, sans-serif;
4, Verdana
微软公司的网页核心字体之一，微软公司专门为屏幕显示而开发的。应用广泛。易于阅读。是显示器中最清晰的字体，即使字号很小，也很容易阅读。半肥猫比较喜欢采用10PX的Verdana来做英文正文字体，也推荐大家使用，但字号最好介于10~14像素之间，超出这个范围就不好看了。
![Verdana](http://ipmtea.net/uploads/allimg/css/1011/0015221A7-3.png)
CSS写法：`font-family: Verdana, Geneva, sans-serif;`
5,  Georgia
微软公司的网页核心字体之一，可用性好。可读性比Times New Roman强。是网站设计中，浏览效果最好的serif字体，因为它是专为网上阅读设计的。）
![Georgia](http://ipmtea.net/uploads/allimg/css/1011/0015225330-4.png)
CSS写法：`font-family: Georgia, serif;`
6,  Times New Roman
微软公司的网页核心字体之一，可能是最常用的serif字体，是网站浏览器默认的字体，12pt以上的字体容易阅读，但小字号的字体易读性差。（苹果系统没有这个字体，有一个对应于Times New Roman的字体叫Times）
![Times New Roman](http://ipmtea.net/uploads/allimg/css/1011/00152224G-5.png)
CSS写法：`font-family: 'Times New Roman', Times, serif;`
7, Trebuchet MS
微软公司的网页核心字体之一，与Arial相似，半肥猫觉得：Trebuchet MS比Arial看起来优雅、古典一点。可以用来做标题，但小字号阅读起来会很困难（低于13PIX阅读起来就很累了，不太推荐用来做正文字体）。在苹果系统上也可以用Helvetica做替代。
![Trebuchet MS](http://ipmtea.net/uploads/allimg/css/1011/0015225294-6.png)
CSS写法：`font-family: 'Trebuchet MS', Helvetica, sans-serif;`
8 , Courier New
微软公司的网页核心字体之一，老式打印机字体，有一种独特的机械工整感觉。呈现计算机编码时，还会用到这种字体。12 pt的Courier New字体曾是美国国务院的公文标准字体，但于2004年1月停用，改使用14 pt的Times New Roman，因为其具“现代性”和“易读性”。
![Courier New](http://ipmtea.net/uploads/allimg/css/1011/0015222317-7.png)
CSS写法：`font-family: 'Courier New', Courier, monospace;`
9,  Impact
微软公司的网页核心字体之一，Impact是1965年发表的一个无衬线字体，其特粗的笔画、紧缩的间距。半肥猫觉得：字体较为粗犷，适合使用在标题上，而不常用在内文。
![Impact](http://ipmtea.net/uploads/allimg/css/1011/0015225094-8.png)
CSS写法：`font-family: Impact, Charcoal, sans-serif;`
10,  Comic Sans MS
微软公司的网页核心字体之一，手写体。这是一种争议很大的字体，讲实话，半肥猫也不喜欢这个字体，一点美感都没有，不过在一大堆规规矩矩的字体里面，有这么一个随意性比较的字体，可以变换一下口味，也算不错吧，建议不要用在正规的金融、政府、商业机构站点。
![Comic Sans MS](http://ipmtea.net/uploads/allimg/css/1011/0015223261-9.png)
CSS写法：`font-family: 'Comic Sans MS', cursive;`
11,  Tahoma
Tahoma是一个十分常见的无衬线字体，Tahoma和Verdana师出同为名设计师马修·卡特的作品，由微软在1999年推出，被采用为Windows 2000、Windows XP、Windows Server 2003等系统的默认字型。半肥猫觉得：它的字体结构和Verdana很相似，其字符间距较小，用来作为标题，效果好过Arial（Tahoma的大写I 和小写l比Arial容易识别），但如果作为正文，他的字号不能小于13PIX，否则很多笔画粘连到一起，不利于阅读。
![Tahoma](http://ipmtea.net/uploads/allimg/css/1011/0015224610-10.png)
CSS写法：`font-family: Tahoma, Geneva, sans-serif;`
12,  Courier
Courier是一个等宽字体的粗衬线字体，主要是依据打字机所打印出来的字型来设计。原来Courier New的字体是IBM公司在1950年代设计给打印机使用的字体，后来这个字型成为整个打字机制造业的标准。Courier New是Courier的变体，比Courier更具机械味道。
![Courier New](http://ipmtea.net/uploads/allimg/css/1011/0015222317-7.png)
CSS写法：`font-family: Courier, monospace;`
13,  Lucida Sans Unicode
是一种OpenType型的无衬线字体 。1993年制作并随微软公司的Windows NT 3.1[操作系统](http://lib.csdn.net/base/operatingsystem)发布。有较大的x字高，具有很好的可读性，被广泛用于显示、出版等各种用途。
它支持Unicode2.0版本的基本字符，包括拉丁字母，希腊字母，西里尔字母，希伯来字母，以及国际音标字符。该字体是首个 Unicode代码的字体， 该字体从Windows 98开始一直作为系统预装字体发行。
后来发布的 Lucida Grande字体作为苹果公司Mac OS X系统的默认字体发布。
CSS写法：`font-family: 'Lucida Sans Unicode', 'Lucida Grande', sans-serif;`
14, Lucida Console
同Lucida Sans Unicode类似。
![Lucida Console](http://ipmtea.net/uploads/allimg/css/1011/0015223119-12.png)
CSS写法：`font-family: 'Lucida Console', Monaco, monospace;`
15,  Garamond
Garamond（加拉蒙德）是一类西文衬线字体的总称，自16世纪40年代开始至今，有很多家公司和很多设计师参与到Garamond字体设计，如： Adobe Garamond, Monotype Garamond, Sioncini Garamond，和 Stempel Garamond等等。半肥猫觉得：字体给人端庄典雅，有些古典的感觉，在博物馆和历史性悠久的项目中使用，应该可以获得不错的效果。
![Garamond](http://ipmtea.net/uploads/allimg/css/1011/001522G49-13.png)
CSS写法：`font-family: Garamond, serif;`
16 ,  MS Sans Serif
微软系统自带字体。屏幕显示的像素字体。非衬线字体。
![MS Sans Serif](http://ipmtea.net/uploads/allimg/css/1011/001522G34-14.png)
CSS写法：`font-family: 'MS Sans Serif', Geneva, sans-serif;`
17 ,  MS Serif
微软系统自带字体。屏幕显示的像素字体。衬线字体。
CSS写法：`font-family: 'MS Serif', 'New York', sans-serif;`
18, Palatino Linotype
![Palatino Linotype](http://ipmtea.net/uploads/allimg/css/1011/0015224b8-15.png)
CSS写法：`font-family: 'Palatino Linotype', 'Book Antiqua', Palatino, serif;`
19, Symbol
![Symbol](http://ipmtea.net/uploads/allimg/css/1011/0015221407-16.png)
CSS写法：`font-family: Symbol, sans-serif;`
20, Bookman Old Style
![Bookman Old Style](http://ipmtea.net/uploads/allimg/css/1011/0015221P1-17.png)
CSS写法：`font-family: 'Bookman Old Style', serif;`
本文参考了WEB安全字体，希望在Web编写过程中这些核心的Web字体能对你有用。
总结几套实用而简单的font-family
font-family: Tahoma, Helvetica, Arial, sans-serif;
Tahoma 系的中性字体。推荐使用在13px以上的环境。
font-family: Trebuchet MS, Verdana, Helvetica, Arial, sans-serif;
Verdana 系的宽扁字体。推荐在11px以下环境使用。
font-family: Geogia, Times New Roman, Times, serif;
衬线字体的不二之选。
font-family: Lucida Console, Monaco, Courier New, mono, monospace;
一系列等宽字体。写代码很好用。另外，如果觉得Lucida Console太宽的话，可以换成比较窄的Lucida Sans Typewriter。话说老赵blog上的代码块使用的就是Lucida Sans Typewriter 哟~
## - 你知道吗？
- 字体的别名
系统中的一个字体是允许有多种别名形式存在的。比如，在windows 下，Georgia 也可以用 Georgia MS 来命名，它们其实是同一种字体。宋体的正式名称是SimSon，而"宋体"只是它的别名。
按照规范，浏览器应该能自动识别字体的别名，并映射到正确的字体文件。比如，font-famliy: SimSon 和 font-family: "宋体" 应该具有等价的效果。可惜，似乎很多浏览器都不能正确执行前一条定义……
- 什么时候在字体名称前面加引号
大家来看这个字体样式定义：
font-family: Times New Roman, 宋体, serif;
很多人都会说，这个样式写法是错的，因为 Times New Roman 和宋体都应该用引号括起来，像下面这样：
font-family: "Times New Roman", "宋体", serif;
实际上呢，上面两种写法都是对的。和很多人想象中的不一样，字体名称外面的引号其实并非必须的。那么加引号和不加引号有什么区别呢？
其实最大的不同在于对字体名称中空白字符（如空格、制表符）的解释。
不加引号的时候，浏览器对于字体名称中空白字符的解释应该和XML中一样，即忽略字体名称左右的空白字符，并且单词中间的空白字符被解释为一个空格。比如 font-family:         Times       New Roman                , serif; 会被解释成 font-family: Times New Roman, serif;
加引号的时候，浏览器必须保留引号内所有的空白字符。如果写成 font-family: "Times       New Roman"; 那么浏览器不会显示 Times New Roman 字体，而是搜索一个叫做"Times       New Roman"的字体。
至 于"宋体"这样的字体名称，因为中间没有空白字符，因此完全没有必要加引号。但是考虑到并非左右的操作系统都有汉字支持，并且并非所有的程序员都会注意 css文件的正确编码问题，所有保险起见，一般会加上引号。当然，解决这种问题的最好方法是使用别名。比如宋体，其实应该写成SimSon，这样哪怕浏览 者的系统不支持中文，并且这个css文件被错误的编码成了GB2132也没问题，浏览器还是知道这是宋体，并且做出正确的字体搜索。可惜，不是所有浏览器 都支持就是了……

