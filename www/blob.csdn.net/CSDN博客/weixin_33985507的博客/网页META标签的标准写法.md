# 网页META标签的标准写法 - weixin_33985507的博客 - CSDN博客
2010年08月19日 11:08:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
最近，Danny Sullivan谈到了如何处理的搜索引擎meta标签的问题，以下是一些关于在Google中如何处理这些问题的答案。
**多内容数值**
我们建议你把所有的Meta标签内容放在一个meta中。这将使得meta标签更易于读取并降低发生错误的概率。例如：
<META NAME="ROBOTS" CONTENT="NOINDEX, NOFOLLOW">
如果一行包含对个meta标签，我们将会累加标签的内容，例如，我们会将
<META NAME="ROBOTS" CONTENT="NOINDEX">
<META NAME="ROBOTS" CONTENT="NOFOLLOW">
理解为：
<META NAME="ROBOTS" CONTENT="NOINDEX, NOFOLLOW">
如果内容的数值有冲突，我们将使用最严格的那个。所以，如果有个页面包含下面的meta标签：
<META NAME="ROBOTS" CONTENT="NOINDEX">
<META NAME="ROBOTS" CONTENT="INDEX">
我们将只遵守noindex的数值。
**单词拼写规范**
Googlebot可以理解任何大小写的组合，所有大小写都是同样的效果。如果包含多内容的数值，数值之间需要使用逗号分隔。
**同时使用robots.txt和robots meta标签**
如果你同时使用了robots.txt文件和robots meta标签，如果robots.txt和meta标签有冲突，googlebot将使用限制更严的。具体来说：
如果你在robots.txt里禁止了某一页，Googlebot永远也不会去索引那一页，也不会读取那一页的任何标识。
如果你在robot.txt中允许，但是在页面中使用 meta 标签对其进行限制，那么Googlebots会访问那一页，读取里面的标签，但不会对其进行索引。
**有效的meta标签数值**
Googlebot可以对下面的标签进行如下的解读：
**NOINDEX** - 阻止页面被列入索引。
**NOFOLLOW** - 阻止对于页面中任何超级链接进行索引。（注，这不同于超级链接上的nofollow属性，那个属性只是组织索引单独的链接）
**NOARCHIVE** - 不保存该页面的网页快照。
**NOSNIPPET** - 不在搜索结果中显示该页面的摘要信息，同时不保存该页面的网页快照。
**NOODP** - 在搜索结果中不使用Open Directory Project中的描述信息作为其摘要信息。
**NONE** - 相当于“NOINDEX, NOFOLLOW”。
**数值NONE的含义**
据robotstxt.org的定义，下面的标记等价于 NOINDEX, NOFOLLOW 。
<META NAME="ROBOTS" CONTENT="NONE">
不过，有些网站站长使用这个标签来限制和组织所有搜索引擎来的内容索引。
<META http-equiv=Content-Type content="text/html; charset=gb2312">
//设定页面语言为简体中文
<META content="关键字" name="keywords">
//设定关键字
<META content="内容描述" name="description">
//设定描述
<META http-equiv="Refresh" Content="30">
//设定该页面30秒自动刷新
<META name="robots" content="index, follow" />
//设定允许搜索引擎可以检索，页面上链接可以查询
<META name="googlebot" content="index, follow" />
//针对google的搜索机器人设定允许搜索引擎可以检索，页面上链接可以查询
<META name="Author" Content="酷站代码 www.5icool.org">
//页面作者
<META name="Copyright" Content="All Rights Reserved icediary">
//页面版权
<META name="revisit-after" CONTENT="1 days" >
//设定回访为1天
META标签分两大部分：HTTP标题信息(HTTP-EQUIV)和页面描述信息(NAME)。
**一、HTTP-EQUIV **
HTTP-EQUIV类似于HTTP的头部协议，它回应给浏览器一些有用的信息，以帮助正确和精确地显示网页内容。常用的HTTP-EQUIV类型有：
**1、Content-Type和Content-Language (显示字符集的设定)**
用法：<Meta http-equiv="Content-Type" Content="text/html; Charset=gb2312">或<Meta http-equiv="Content-Language" Content="zh-CN">
设定页面使用的字符集，用以说明主页制作所使用的文字或语言，浏览器会根据此来调用相应的字符集显示页面内容。简体中文一般为GB2132，繁体中文为BIG5，日文的字符集是iso-2022-jp，韩文的是ks_c_5601等。
**2、Refresh (刷新)**
用法：<Meta http-equiv="Refresh" Content="30">或<Meta http-equiv="Refresh" Content="5; Url=http://www.5icool.org">
让网页多少秒刷新自己，或在多少秒后让网页自动链接到其它网页。
**3、Expires (期限)**
用法：<Meta http-equiv="Expires" Content="0">或<Meta http-equiv="Expires" Content="Wed, 26 Feb 1997 08:21:57 GMT">
指定网页在缓存中的过期时间，一旦网页过期，必须到服务器上重新调阅。注意：必须使用GMT的时间格式。
**4、Pragma (cach模式)**
用法：<Meta http-equiv="Pragma" Content="No-cach">
网页不保存在缓存中，每次访问都刷新页面。这样设定，访问者将无法脱机浏览。
**5、Set-Cookie (cookie设定)**
<Meta http-equiv="Set-Cookie" Content="cookievalue=xxx; expires=Wednesday,21-Oct-98 16:14:21 GMT; path=/">
浏览器访问某个页面时会将它存在缓存中，下次再次访问时就可从缓存中读取，以提高速度。当你希望访问者每次都刷新你广告的图标，或每次都刷新你的计数器，就要禁用缓存了。通常HTML文件没有必要禁用缓存，对于ASP等页面，就可以使用禁用缓存，因为每次看到的页面都是在服务器动态生成的，缓存就失去意义。如果网页过期，那么存盘的cookie将被删除。必须使用GMT的时间格式。
**6、Window-target (显示窗口的设定)**
用法：<Meta http-equiv="Widow-target" Content="_top">
强制页面在当前窗口以独立页面显示。这个属性是用来防止别人在框架里调用你的页面。Content选项：_blank、_top、_self、_parent。
**7、Pics-label (网页RSAC等级评定)**
用法：<META http-equiv="Pics-label" Contect="(PICS－1.1'http://www.bazhan.net' I gen comment 'RSACi North America Sever' by
['inet@microsoft.com'](mailto:%27inet@microsoft.com%27) for 'http://www.microsoft.com' on '1997.06.30T14:21－0500' r(n0 s0 v0 l0))">
在IE的Internet选项中有一项内容设置，可以防止浏览一些受限制的网站，而网站的限制级别就是通过该参数来设置的。
**8、Page-Enter、Page-Exit (进入与退出)**
用法：<Meta http-equiv="Page-Enter" Content="blendTrans(Duration=0.5)"> <Meta http-equiv="Page-Exit" Content="blendTrans(Duration=0.5)">
或者：<Meta http-equiv="Page-Enter" Content="revealTrans(duration=x, transition=y)"> <Meta http-equiv="Page-Exit" Content="revealTrans(duration=x, transition=y)">
这个是页面被载入和调出时的一些特效。注意：blendTrans是动态滤镜的一种，产生渐隐效果。另一种动态滤镜RevealTrans也可以用于页面进入与退出效果: Duration 表示滤镜特效的持续时间(单位：秒)；Transition 滤镜类型。表示使用哪种特效，取值为0-23。
**9、IE6 (页面生成器)**
用法：<Meta http-equiv="IE6" Content="Generator">
页面生成器generator，是ie6。
**10、Content-Script-Type (脚本相关)**
用法：<Meta http-equiv="Content-Script-Type" Content="text/javascript">
注意：这是近来W3C的规范，指明页面中脚本的类型。
**二、NAME变量**
name是描述网页的，对应于Content（网页内容），以便于搜索引擎机器人查找、分类（目前几乎所有的搜索引擎都使用网上机器人自动查找meta值来给网页分类）。
**11、Keywords (关键字)**
用法：<Meta name="Keywords" Content="关键词1,关键词2,关键词3,关键词4">
为搜索引擎提供的关键字列表。注意：各关键词间用英文半角逗号“,”隔开。关键词数量最好保持在5个以内，关键词要和页面内容相关。
**12、Description (简介)**
用法：<Meta name="Description" Content="你网页的简述,如酷站代码 www.5icool.org">
Description用来告诉搜索引擎你的网站或该页面的主要内容，描述内容要和页面内容相关，字数在150个字以内。
**13、Robots (机器人向导)**
用法：<Meta name="Robots" Content="All|None|Index|Noindex|Follow|Nofollow">
说明：Robots用来告诉搜索机器人哪些页面需要索引，哪些页面不需要索引。Content的参数有all、none、index、noindex、follow、nofollow。默认是all。
all：文件将被检索，且页面上的链接可以被查询；none：文件将不被检索，且页面上的链接不可以被查询；(和 "noindex, no follow" 起相同作用)；index：文件将被检索；（让robot/spider登录）；follow：页面上的链接可以被查询；noindex：文件将不被检索，但页面上的链接可以被查询；(不让robot/spider登录)；nofollow：文件将不被检索，页面上的链接可以被查询。(不让robot/spider顺着此页的连接往下探找)。
**14、Author (作者)**
用法：<Meta name="Author" Content="酷站代码,www.5icool.org">
标注网页的作者或制作组。Content可以是：你或你的制作组的名字或Email。
**15、Copyright (版权)**
用法：<Meta name="Copyright" Content="本页版权归酷站代码所有。All Rights Reserved icediary">
标注版权。
**16、Generator (编辑器)**
用法：<Meta name="Generator" Content="PCDATA|FrontPage|">
编辑器的说明。
**17、revisit-after (重访)**
用法：<META name="revisit-after" CONTENT="7 days" >
一般用做引导搜索引擎重新访问你的站点的时间间隔，一般使用days。
