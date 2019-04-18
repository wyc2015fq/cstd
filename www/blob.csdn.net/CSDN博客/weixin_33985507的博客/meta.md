# meta - weixin_33985507的博客 - CSDN博客
2016年12月20日 16:34:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
# meta
  元素可提供相关页面的元信息（meta-information），比如针对搜索引擎和更新频度的描述和关键词。 标签位于文档的头部，不包含任何内容。 标签的属性定义了与文档相关联的名称/值对。
|外文名|所处位置|含义|属性|
|----|----|----|----|
|meta|文档的头部，不包含任何内容|元素可提供相关页面的元信息|定义了与文档相关联的名称/值对|
## **简介**
meta标签共有两个属性，它们分别是http-equiv属性和name属性，不同的属性又有不同的参数值，这些不同的参数值就实现了不同的网页功能。
<meta> 元素可提供有关页面的元信息（meta-information），比如针对[搜索引擎](https://link.jianshu.com?t=http://baike.baidu.com/view/1154.htm)和更新频度的描述和关键词。
<meta> 标签位于文档的头部，不包含任何内容。<meta> 标签的属性定义了与文档相关联的名称/值对。
meta是html语言head区的一个辅助性标签。几乎所有的网页里，我们可以看到类似下面这段的html代码：
```
<head>
    <meta http-equiv="content-Type" content="text/html; charset=gb2312" >
  </head>
```
也许你认为这些代码可有可无。其实如果你能够用好meta标签，会给你带来意想不到的效果，例如加入[关键字](https://link.jianshu.com?t=http://baike.baidu.com/view/390935.htm)会自动被大型搜索网站自动搜集；可以设定页面格式及刷新等等。
**提示和注释**
- 注释：<meta> 标签永远位于 head 元素内部。
注释：元数据总是以名称/值的形式被成对传递的。
#### **name属性**
name属性主要用于描述[网页](https://link.jianshu.com?t=http://baike.baidu.com/view/828.htm)，与之对应的[属性值](https://link.jianshu.com?t=http://baike.baidu.com/view/121567.htm)为content，content中的内容主要是便于搜索引擎机器人查找信息和分类信息用的。
meta标签的name属性语法格式是：<meta name="参数" content="具体的参数值">；。
其中name属性主要有以下几种参数：
A、Keywords（关键字）
说明：keywords用来告诉[搜索引擎](https://link.jianshu.com?t=http://baike.baidu.com/view/1154.htm)你网页的关键字是什么。
举例：
```
<meta name ="keywords" 
        content="science,education,culture,politics,
                 ecnomics,relationships,entertainment,human">
```
B、description（网站内容描述）
说明：description用来告诉搜索引擎你的网站主要内容。
网站内容描述（description）的设计要点：
①网页描述为自然语言而不是罗列关键词（与keywords设计正好相反）；
②尽可能准确地描述网页的核心内容，通常为网页内容的摘要信息，也就是希望搜索引擎在检索结果中展示的摘要信息；
③网页描述中含有有效关键词；
④网页描述内容与网页标题内容有高度相关性；
⑤网页描述内容与网页主体内容有高度相关性；
⑥网页描述的文字不必太多，一般不超过搜索引擎检索结果[摘要](https://link.jianshu.com?t=http://baike.baidu.com/view/123897.htm)信息的最多字数（通常在100中文字之内，不同搜索引擎略有差异）。
举例：
```
<meta name="description" 
       content="This page is about the meaning of science,education,culture.">
```
C、robots（机器人向导）
说明：robots用来告诉搜索机器人哪些页面需要索引，哪些页面不需要索引。
content的参数有all,none,index,noindex,follow,nofollow。默认是all。
举例：
```
<meta name="robots" content="none">
```
D、author（作者）
说明：标注网页的作者
#### **http-equiv属性**
http-equiv顾名思义，相当于http的文件头作用，它可以向浏览器传回一些有用的信息，以帮助正确和精确地显示网页内容，与之对应的属性值为content，content中的内容其实就是各个参数的[变量值](https://link.jianshu.com?t=http://baike.baidu.com/view/6005516.htm)。
meta标签的http-equiv属性语法格式是：`<meta http-equiv="参数" content="参数变量值">`；其中http-equiv属性主要有以下几种参数：
A、Expires（期限）
说明：可以用于设定网页的到期时间。一旦网页过期，必须到服务器上重新传输。
用法：
```
<meta http-equiv="expires" content="Fri,12 Jan 2001 18:18:18 GMT">
```
注意：必须使用GMT的时间格式。
B、Pragma(cache模式）
说明：禁止浏览器从本地计算机的[缓存](https://link.jianshu.com?t=http://baike.baidu.com/view/907.htm)中访问页面内容。
用法：
```
<meta http-equiv="Pragma" content="no-cache">
```
注意：这样设定，访问者将无法脱机浏览。
C、Refresh（刷新）
说明：自动刷新并转到新页面。
用法：
```
<meta http-equiv="Refresh" content="2;URL">；
    //（注意后面的分号，分别在秒数的前面和网址的后面，URL可为空）
```
注意：其中的2是指停留2秒钟后自动刷新到URL网址。
D、Set-Cookie(cookie设定）
说明：如果网页过期，那么存盘的cookie将被删除。
用法：
```
<meta http-equiv="Set-Cookie" 
             content="cookievalue=xxx; expires=Friday,12-Jan-2001 18:18:18 GMT; path=/">
```
注意：必须使用GMT的时间格式。
E、Window-target（显示窗口的设定）
说明：强制页面在当前窗口以独立页面显示。
用法：
```
<meta http-equiv="Window-target" content="_top">
```
注意：用来防止别人在框架里调用自己的页面。
F、content-Type（显示字符集的设定）
说明：设定页面使用的字符集。
用法：
```
<meta http-equiv="content-Type" content="text/html; charset=gb2312">
```
G、content-Language（显示语言的设定）
用法：
```
<meta http-equiv="Content-Language" content="zh-cn" />
```
### 功能
上面我们介绍了meta标签的一些基本组成，接着我们再来一起看看meta标签的常见功能：
**1、帮助主页被各大搜索引擎登录**
meta标签的一个很重要的功能就是设置关键字，来帮助你的主页被各大搜索引擎登录，提高网站的访问量。在这个功能中，最重要的就是对Keywords和description的设置。因为按照搜索引擎的工作原理，搜索引擎首先派出机器人自动检索页面中的keywords和description，并将其加入到自己的数据库，然后再根据关键词的密度将网站排序。因此，我们必须设置好关键字，来提高页面的搜索点击率。
下面我们来举一个例子供大家参考：<meta name="keywords" content="政治，经济，科技，文化，卫生，情感，心灵，娱乐，生活，社会，企业，交通">
```
<meta name="description" content="政治，经济，科技，文化，卫生，
                                      情感，心灵，娱乐，生活，社会，企业，交通">
```
设置好这些关键字后，搜索引擎将会自动把这些关键字添加到数据库中，并根据这些关键字的密度来进行合适的排序。
**2、定义页面的使用语言**
这是meta标签最常见的功能，在制作网页时，我们在纯HTML代码下都会看到它，它起的作用是定义你网页的语言，当浏览者访问你的网页时，浏览器会自动识别并设置网页中的语言，如果你网页设置的是GB码，而浏览者没有安装GB码，这时网页只会呈现浏览者所设置的浏览器默认语言。同样的，如果该网页是英语，那么charset=en。
下面就是一个具有代表性的例子：
```
<meta http-equiv=″content－Type″ content=″text/html; charset=gb2312″>
```
该代码就表示将网页的语言设置成国标码。
**3、自动刷新并指向新的页面**
如果你想使您的网页在无人控制的情况下，能自动在指定的时间内去访问指定的网页，就可以使用meta标签的自动刷新网页的功能。下面我们来看一段代码：
```
<meta http-equiv=″refresh″ content=″2; URL=
```
这段代码可以使当前某一个网页在2秒后自动转到页面中去，这就是meta的刷新作用，在content中，2代表设置的时间（单位为秒），而URL就是在指定的时间后自动连接的网页地址。
**动画效果**
- 
使用meta标签，我们还可以在进入网页或者离开网页的一刹那实现动画效果，我们只要在页面的html代码中的<head></head>；标签之间添加如下代码就可以了：
```
<meta http-equiv="Page-Enter" content="revealTrans(duration=5.0,transition=20)">
```
一旦上述代码被加到一个网页中后，我们再进出页面时就会看到一些特殊效果，这个功能其实与FrontPage2000中的Format/Page Transition一样，但我们要注意的是所加网页不能是一个Frame页； RevealTrans动态[滤镜](https://link.jianshu.com?t=http://baike.baidu.com/view/18145.htm)
Duration表示滤镜特效的持续时间（单位：秒）
Transition滤镜类型。表示使用哪种特效，取值为0-23:
 0矩形缩小
 1矩形扩大
 2 圆形缩小
 3 圆形扩大
 4 下到上刷新
 5 上到下刷新
 6 左到右刷新
 7 右到左刷新
 8 竖[百叶窗](https://link.jianshu.com?t=http://baike.baidu.com/view/705755.htm)
 9 横百叶窗
 10 错位横百叶窗
 11 错位竖百叶窗
 12 点扩散
 13 左右到中间刷新
 14 中间到左右刷新
 15 中间到上下
 16 上下到中间
 17 右下到左上
 18 右上到左下
 19 左上到右下
 20 左下到右上
 21 横条
 22 竖条
 23 以上22种随机选择一种
<big>网页定级评价</big>
 IE4.0以上版本的浏览器可以防止浏览一些受限制的网站，而之所以浏览器会自动识别某些网站是否受限制，就是因为在网站meta标签中已经设置好了该网站的级别，而该级别的评定是由美国RSAC，即娱乐委员会的评级机构评定的，如果你需要评价自己的网站，按要求提交表格，那么RSAC会提供一段meta代码给你，复制到自己网页里就可以了。下面就是一段代码的样例：
```
<meta http-equiv=″PICS－Label″
      content=′（PICS－1.1
      l gen true comment ″RSACi North America Server″
      for
      on ″2001.08.16T08:15－0500″
      r (n 0 s 0 v 0 l 0））′>
```
<big>控制页面缓冲</big>
 meta标签可以设置网页到期的时间，也就是说，当你在Internet Explorer 浏览器中设置浏览网页时首先查看本地缓冲里的页面，那么当浏览某一网页，而本地缓冲又有时，那么浏览器会自动浏览缓冲区里的页面，直到meta中设置的时间到期，这时候，浏览器才会去取得新页面。例如下面这段代码就表示网页的到期时间是2001年1月12日18时18分18秒。
```
<meta http-equiv=″expires″ content=″Friday,12-Jan-2001 18:18:18 GMT″>
```
<big>控制网页窗口</big>
 我们还可以使用meta标签来控制网页显示的窗口，只要在网页中加入下面的代码就可以了：
```
<metahttp-equiv="window-target" content="_top">；
```
，这段代码可以防止网页被别人作为一个Frame调用。
**其他用法**
- 
**scheme(方案)**
说明：scheme can be used when name is used to specify how the value of content should be interpreted.
用于指定要用来翻译属性值的方案。此方案应该在由 <head> 标签的 profile 属性指定的概况文件中进行了定义。
用法：`<meta scheme="ISBN" name="identifier" content="0-14-XXXXXX-1" >`
**Link （链接）**
说明：链接到文件
用法：`<Link href="soim.ico" rel="Shortcut Icon">
注意：很多网站如果你把它保存在收件夹中后，会发现它连带着一个小图标，如果再次点击进入之后还会发现地址栏中也有个小图标。现在只要在你的页头加上这段话，就能轻松实现这一功能。<LINK> 用来将文件与其它 URL 作连结，但不会有连结按钮，用于<HEAD> 标记间， 格式如下：
```
<link href="URL" rel="relationship">
<link href="URL" rev="relationship">
```
**Base (基链接)**
说明：插入网页基链接属性
注意：你网页上的所有相对路径在链接时都将在前面加上URL。其中target="_blank"是链接文件在新的窗口中打开，你可以做其他设置。将“_blank”改为“_parent”是链接文件将在当前窗口的父级窗口中打开；改为“_self”链接文件在当前窗口（帧）中打开；改为“_top”链接文件全屏显示。
**使用方法**
- Meta标签是用来描述网页属性的一种语言，标准的Meta标签可以便于搜索引擎排序，提高搜索引擎网站权重排名。想网站做的更符合搜索引擎标准就必须了解meta标签，下面由Seoer惜缘给大家讲讲meta标签含义与使用方法：
**keywords**
[META标签](https://link.jianshu.com?t=http://baike.baidu.com/view/740572.htm)的keywords
写法为：`<meta name="Keywords" content="信息参数" >`
meta标签的Keywords的的信息参数，代表说明网站的关键词是什么。
**Description**
META标签的Description
写法为：`<meta name="Description" content="信息参数" >`
[meta标签](https://link.jianshu.com?t=http://baike.baidu.com/view/740572.htm)的Description的信息参数，代表说明网站的主要内容，概况是什么。
**Content-Type**
META标签的http-equiv=Content-Type content="text/html
http-equiv=Content-Type代表的是HTTP的头部协议，提示浏览器网页的信息，
写法为：`<meta http-equiv="Content-Type" content="text/html; charset=信息参数" >`
 meta标签的charset的信息参数如GB2312（或GBK）时，代表说明网站是采用的编码是简体中文；
 meta标签的charset的信息参数如BIG5时，代表说明网站是采用的编码是繁体中文；
 meta标签的charset的信息参数如iso-2022-jp时，代表说明网站是采用的编码是日文；
 meta标签的charset的信息参数如ks_c_5601时，代表说明网站是采用的编码是韩文；
 meta标签的charset的信息参数如ISO-8859-1时，代表说明网站是采用的编码是英文；
 meta标签的charset的信息参数如UTF-8时，代表世界通用的语言编码。
**注：在HTML5中，在表达charset信息时可以简写，前面的内容可以去掉，即写法是： <meta charset="GBK">**
**generator**
META标签的generator
写法为：`<meta name="generator" content="信息参数" />`
meta标签的generator的信息参数，代表说明网站的采用的什么软件制作。
**author**
META标签的author
写法为：`<meta name="author" content="信息参数">`
meta标签的author的信息参数，代表说明网页版权作者信息。
META标签的http-equiv="Refresh"
写法为：`<Meta http-equiv="Refresh" Content="时间； Url=网址参数">`
meta标签的Refresh代表多少时间网页自动刷新，加上Url中的网址参数就代表，多长时间自动链接其他网址。
**Pragma**
META标签的HTTP-EQUⅣ="Pragma" CONTENT="no-cache"
写法为：`<META HTTP-EQUⅣ="Pragma" CONTENT="no-cache">；`
代表禁止浏览器从本地计算机的缓存中访问页面内容，这样设定，访问者将无法脱机浏览。
**COPYRIGHT**
META标签的COPYRIGHT
写法为：`<META NAME="COPYRIGHT" CONTENT="信息参数">`
meta标签的COPYRIGHT的信息参数，代表说明网站版权信息。
**imagetoolbar**
META标签的http-equiv="imagetoolbar"
写法为：`<meta http-equiv="imagetoolbar" content="false" >`
指定是否显示图片工具栏，当为false代表不显示，当为true代表显示。
**Content-Script-Type**
META标签的Content-Script-Type
写法为：`<Meta http-equiv="Content-Script-Type" Content="text/javascript">`
W3C网页规范，指明页面中脚本的类型。
**revisit-after**
META标签的revisit-after
写法为：`<META name="revisit-after" CONTENT="7 days" >`
revisit-after代表网站重访，7 days代表7天，依此类推。
**Robots**
META标签的Robots
写法为：`<meta name="Robots" content="信息参数">`
Robots代表告诉搜索引擎机器人抓取哪些页面
**其中的属性**
- 信息参数为all：文件将被检索，且页面上的链接可以被查询；
信息参数为none：文件将不被检索，且页面上的链接不可以被查询；
信息参数为index：文件将被检索；
信息参数为follow：页面上的链接可以被查询；
信息参数为noindex：文件将不被检索，但页面上的链接可以被查询；
信息参数为nofollow：文件将被检索，但页面上的链接不可以被查询；
**标签优化**
- meta标签的内容优化很有可能将会成为一个网站点击率获取最关键的一个内容，如何更好进行meta标签内容的优化？
1、细化监控范围；
2、建立动态网页标题和meta描述；
3、监视你的谷歌网站管理员网站简介；
4、包括你的独特的价值主张；
5、写的用户，而不是机器人。
**英文前缀**
- 前缀
pref.
⒈表示"变化","变换"
⒉表示"继","在...之后"
⒊表示"超越"
⒋表示"在...之间","介于"
5.在逻辑学中是“关于那个的，更高层次”的意思
例词：metaphysics，Metamathematics(表示高层次的数理哲学)
n.
⒈形而上学；玄学
⒉深奥莫测的推理；空谈；空头理论
  来自希腊语，最初来源是作为亚里士多德所著《形而上学》一书的书名，意指 “ 第一哲学 ”，也就是以 “ 作为存在的存在（being as being）” 为研究对象的形而上学，其意义为 “ 在具体科学之后 ”。
**模板meta语言**
模板meta语言由GDMO提出，采用类似于BNF的语法，因此与ASN.1相似，只要了解了它与ASN.1的不同之处就可以在ASN.1有关知识的基础上正确使用。因此，这里只将有关要点进行如下说明：
⒈分号（；）用于终止结构和中止模板
⒉空格，空行，注释和行尾只起分割符的作用。在需要标志一个元素结束，另一个元素开始时使用。
⒊注释由双连字符（--）引导，在行尾或遇到另外的双连字符终止。可以出现在任何分隔区中，但不能出现在结构名或模板名所包含的空格之间。
⒋方括号（[ ]）用于指出模板定义中的可选元素。
⒌右圆括号中的星号`（*）`指出模板定义中的可选元素。
⒍选择对象由竖线（|）分割。这个符号旨在支持件的定义中使用。
⒎将由用户确定的字符串扩在尖括号（<>；）中。
⒏附件用一个引用标号，后接符号->>；，后接一个由文本字符串和符号构成的语法定义组成。
⒐分隔串出现在模板定义中自然语言文本或形式说明文本之中。他们由任意的字符串组成，字符串可以由以下任意一个分隔符引导和终止。分隔符是“$ % ^ & * ` ' ~ @ \”。如果分隔串由某个分隔符开始，则这个分隔串直到再次遇到相同的分隔符才结束。
