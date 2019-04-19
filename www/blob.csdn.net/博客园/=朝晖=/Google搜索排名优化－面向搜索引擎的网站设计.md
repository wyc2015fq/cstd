# Google搜索排名优化－面向搜索引擎的网站设计 - =朝晖= - 博客园
# [Google搜索排名优化－面向搜索引擎的网站设计](https://www.cnblogs.com/dhcn/p/7105416.html)
内容摘要：
网站在搜索营销方面最主要的缺点：
- 行业知识：不知道搜索引擎对吸引的新用户的重要性，在搜索引擎排名服务中追求“傻瓜相关”，购买一些其实没有太多实际意义的行业关键词。其实能够用户输入的关键词越多，其目标性越强，这样的客户价值越高。用户能够直接定位到产品具体内容页比到网站首页有价值的多；
- 发布技术：网站的网页进入Google的索引量非常小，主要是由于大量使用动态网页造成的。需要将[动态页面链接改写成静态 链接](http://www.chedong.com/cgi-bin/mt/google_url.html)；
- 页面设计：页面标题重复，关键词不突出，过渡使用JavaScript脚本/图片/Flash等不适合搜索引擎索引的非文本形式；在用户友好(user friendly)方面和机器友好(robots friendly)之间的平衡；
而以上这些问题的根本原因还是网站自身缺乏[日志统计分析](http://www.chedong.com/cgi-bin/mt/awstats.html)造成：没有统计就无法看到并评估SEO对网站自身产生的巨大影响。 
SEO(Search engine optimization)的目的是通过一些简单的策略，让网站自身的水平在互联网中有一个真实的体现。网站结构设计中面向[搜索引擎](http://lib.csdn.net/base/searchengine)的优化注意事项包括：
- [链接引用的重要性；](http://www.chedong.com/cgi-bin/mt/mt.cgi#link)
- [如何突出关键词：网页标题、主题的设计；](http://www.chedong.com/cgi-bin/mt/mt.cgi#keyword)
- [页面及站点结构设计注意事项；](http://www.chedong.com/cgi-bin/mt/mt.cgi#web)
- [以及站点访问统计的重要性等；](http://www.chedong.com/cgi-bin/mt/mt.cgi#stat)
- [Google的站点设计指南](http://www.chedong.com/cgi-bin/mt/mt.cgi#guide)
（注意：本网站设计本身就利用了其中一些方法）。
## 什么是PageRank
Google等新一带搜索引擎的优势之一在于不仅索引量很大，而且还将最好的结果排在搜索结果的最前面，具体的原理可以参考[Google の秘密 - PageRank 徹底解説](http://www.kusastro.kyoto-u.ac.jp/~baba/wais/pagerank.html)一文，PageRank简单的说类似于科技论文中的引用机制：谁的论文被引用次数多，谁就是权威。在互联 网上PageRank就是基于网页中相互链接关系的分析得出的。
此外，从计算方法角度阐述PageRank机制还有这篇文章：[http://pr.efactory.de/](http://pr.efactory.de/)里 面有更详细的PageRank[算法](http://lib.csdn.net/base/datastructure)说明和各种清晰的小型个案分析：
比如：子页中导航条的重要性
B <=> A => C   
Vs   
B <=> A <=> C   （好）
页面数量因素：
B <=> A <=> C
Vs
F <=\   /=> G
B <=> A <=> C   （好）
D <=/   \=> E
一个意想不到的结论：
(B <=> A <=> C) ( E <=> D <=> F) Vs. (B <=> A <=> C)  <=> ( E <=> D <=> F)
PageRank升高的只是被链接的2个首页A和D，而网站子页面的PageRank平均会略有下降。同时：一个网站进入Google的索引量越大其受类 似因素影响越小。
PageRank不对称的页面互链： Google会用BadRank之类的算法进行纠正，而且一个网页着有来自“与其自身不相称”的高PageRank站点的链接，而自身又缺少足够数量和质 量的反相链接的话，其PageRank会自动降低为0，A(pr=7) <=> B(pr=0)
简单的说就是：偶尔要被权威站点反相链接不算数，要被足够多的权威站点引用才能提高自身网页的PageRank。
[Web超链分析算法综述（南京大学论文）](http://mes.nju.edu.cn/research/papers/Survy03-10.doc) 更多论文可以通过搜索：“filetype:pdf google pagerank anchor text bayesian”得到
## **链接就是一切**
在互联网的海洋中，最重要的就是互联互通，不被其他网站引用的网站就是“信息孤岛”。“酒好也怕藏的深”，也许这话说起来有点像垃圾邮件广告，但事实就是这样。所以如果做网站的目的不是孤芳自赏，就需要积极的推广自己的网站。
通过搜索引擎推广自己需要注意以下几个方面：
- 以量取胜：不一定加入传统门户网站的分类目录才是网站推广，来自其他网站的任何反相链接都是有用的
网站推广比较经典的方式就是加入比较大型门户网站的分类目录，比如：[Yahoo!](http://www.yahoo.com/)，[dmoz.org](http://dmoz.org/)等。其实这里有一个误区：不一定非要加入大型网站的分类目录才是网站推广，因 为现在搜索引擎已经不再只是网站目录的索引，而是更全面的网页索引，所以无论来自其他网站任何地方的反相链接都是非常有价值的，哪怕是出现在新闻报道，论坛，邮件列表归档中。Blogger（Weblog的简称）们也许最深刻地理解了“链接就是一切”这句话的含义，由于Blog的内容之间有大量的相互链接（他们之间利用RSS很容易进行链接大量传播），因此最经常被引用的Blog页面在搜索引擎中的排名往往比一些大型商业网站的页面还要高。而WIKI这样的文档管理系统更加突出了良好结构，良好引用的特征。而目前很多网站通过在各种WIKI/留言簿中灌注垃圾留言的方法，这种方式是非常不可取的。这种链接不稳定：
- 以质取胜：被PageRank高的网站引用能更快地提高PageRank
数量只是关键因素之一，来自PageRank高的页面的链接还能更快的提高被链接目标的PageRank，我 只是将一些文章投稿在了[ZDNet中国](http://www.zdnet.com.cn/)上，由于页面上有文章出处链接， 相应网页和网站整体的PageRank过了一段时间后就有了很大的提升。有时候被什么样的网站引用有时候比引用次数多更重要。这里我要特别感谢的是，当时[ZDNet中国](http://www.zdnet.com.cn/)是唯一遵循了我的版权声明的要求表明了文章出处，并且有反相链 接的网站。
按照这个原则：能够名列Yahoo!和DMOZ这样的大型权威目录的头2层是非常有价值的。
- 了解搜索引擎的"价值观"：
[Lucene简介](http://www.chedong.com/cgi-bin/mt/lucene.html)这篇文章被Jakarta.apache.org的[lucene项目](http://jakarta.apache.org/lucene/)引 用以后，这篇文章就成为了所有页面中PageRank最高的页面，而Google支持的一些项目，比如：[Folding@Home](http://www.equn.com/folding/)，让我一直怀疑他们对政府，教育和非盈利 组织的站点有特别加分，毕竟.org .edu才代表了互联网的实质：分权和共享。
但更合理的解释是：.org很多都是开放技术平台的开发者，他们会在首页这样的地方加入Powered By Apache, Power by FreeBSD之类的链接表示对其他开源平台的尊重，所以象[Apache](http://www.apache.org/), [PHP](http://www.php.net/), [FreeBSD](http://www.freebsd.org/)这样的开源站点在GOOGLE中都有非常高的PageRank。 而在.edu这些站点中，很多都是学术性比较强的文档，以超链接形式标明参考文献的出处已经成为一种习惯，而这也无疑正是PageRank最好的依据。
注意：不要通过Link Farm提高自身的站点排名：Google会惩罚那些主动链接到Link Farm站点以提高自身排名站点，相应站点的页面将不会被收入到索引中。但如果你的页面被别的Link Farm链接了也不必担心，因为这种被动的链接是不会被惩罚的。
- 不要吝啬给其他网站的链接：如果一个网页只有大量的进入链接，而缺乏导出链接，也会被搜索引擎认为是没有价值的站点。保证你的网站能够帮助搜 索引擎更准确地判断哪些是对用户最有价值的信息，也就是说如果你的网站只有外部反向链接而没有导出链接的话，也会对你的网站在搜索结果中的表现带来负面影 响。当然网站中连一个导出链接都没有的情况非常罕见，除非你是刻意这么做。正常情况下大家都会自然地在网页中加上一些其他网站的链接，带领访问者去到我们 认为重要或能够提供更多有价值信息的地方，
另外在推广自己网站之前也许首先需要了解自己网站目前在一些搜索引擎中的知名度，原理非常简单，可以参考[如何评价网站的人气](http://www.chedong.com/tech/link_pop_check.html)一 文。
网站推广只是手段，如何突出内容、让需要相关信息的用户能够尽快的找到你的网站才是目的，PageRank高并不代表像Yahoo!这样的门户站点 就能在所有搜索结果中排名靠前，因为搜索引擎的结果是搜索关键词在页面中的匹配度和页面的PageRank相结合的排名结果。因此第二个要点：如何突出关 键词。
## **如何突出关键词：面向主题(Theme)的关键词匹配**
- [Theme Engine](http://www.google.com/search?q=%22theme+engine%22)正在逐步超过PR，成为结果排序中更主要的因素，可以比较一下以下现象：
- 为什么查“新闻”，“汽车”之类的文字，各种门户网站的首页不在第一位？要知道他们的页面中都有相应频道的链接文字的
- 一篇新闻页面中，搜索引擎如何不通过模板匹配，自动将新闻的主体和页面中的页头，栏目导航条，页尾等部分的内容区分开的？
其实以上问题都可以归结为网页内容摘要的提取策略和关键词在其中的命中：
首先将能够描述一个页面内容的分成以下几种类型：
- 链入文字描述：inbound link text
http://www.searchenginedictionary.com/terms-inbound-link.shtml
- HTML页面标题：title 好标题一般会将页面中最重要的关键词放在最前面，比如：ABC-10型吸尘器 - XX家电城
- HTML内容主体：content text
- 链出文字：outbound link text
如果按照以下规则：
一个页面中关键词命中权重：链入文字 > HTML标题文字 > HTML页面主体内容 >> 出链文字，以上现象就比较好解释了。
链入文字是页面上看不见的，但链入文字对被链接页面有巨大的作用：在现代搜索引擎在关键词匹配的过程中，匹配的过程不只看当前页面的内容摘要：很大程度 上，不只看这个网页自己说自己有什么内 容，还要看别人如何链接时，如何描述你的网站别人怎么称呼你，比你自己说自己更重要。。
比如查：“世界卫生组织”，返回结果中有 http://www.who.int/ 而这个页面中是没有中文的，之所以能匹配上，是因为很多链接它的中文网站使用了：<a xhref="http://www.who.int/" mce_href="http://www.who.int/">世界卫生组织</a>，所以，这个页面中不存在的中文关键词也成为了页面摘要 的一 部分。 这样一想，就可以知道链出链接的文字其实是为被链接的子频道首页或者内容详情页服务的。对自身页面的关键词密度只有负面的影响，这也是Google建议一 个页面中链接不要超过100个的原因：他根本不索引100个链接以后的内容。 按照以上规则，搜索引擎将一篇新闻详情页中的新闻内容提取出来就是把页面上所有带HTTP链接的文字都去掉，就是新闻的内容部分了，更精确一些可以通过取 最长文本段落等策略实现； 而很多网站首页或频道首页中几乎全部都是链接，经过搜索引擎分析的结果就是：什么内容也没有，能够被命中的关键词仅仅是别人链接你用的“网站首页”和频道 标题Title中的站点名称等的几个关键词，而页面中其他的文字则远远不如相应子 频道和具体内容页面的匹配度高，而搜索引擎能够通过以上规则，让用户更快的直接定位到有具体内容的内容详情页面。因此希望通过一个首页，尽可能多的命中所 有希望推广的关键词是不可能的。[让网页尽可能多的进入搜索引擎的索引](http://www.chedong.com/cgi-bin/mt/google_url.html)，然后把握好整个网站 的主题风格是非常 重要的，让网 站的主题关键词能够比较均匀的按照金字塔模式分布到网站中可以参考：[网站的主题 金字塔设计](http://www.searchengineworld.com/engine/theme_pyramids.htm)         网站名称（用户通过1－2个抽象关键词）
         /       \
    子频道1   子频道2 （用户通过2－3个关键词命中）
   /    \      /    \
产品1  产品2 文章1 文章2 (用户通过3－4个关键词命中：这种搜索用户最有商业价值)
- 不要空着标题：空着<title></title>无异于浪费了最有价值的一块阵地；
**不要使用meta keywords**
传统的页面中，HTML页面中会有类似以下的隐含信息，用于说明当前网页的主要内容关键字：
<header>
    <meta name="keyword" content="mp3 download music...">
</header>
后来由于这种人工添加关键词的方式被滥用，大量网页中为了提高被搜索引擎命中的概率，经常添加一些和实际网页内容无关的热门关键比如：“music mp3 download”等，所以新一代的搜索引擎已经不再关心页面头文件中的人工meta keyword声明，而页面标题在搜索引擎的关键词的命中命中过程中往往有着更高的比重，如果一个关键词在标题中命中会比在页面中命中有更高的得分，从而 在相应的搜索结果排名中更靠前。meta description的用途主要在于更新在搜索引擎中的摘要：但对排名没有影响。 相同的排名下，可读性更好的摘要对于用户点击也有一点益处，相比之下： 如果摘要全部是页面头部的导航信息，可能不如页面内容本身更吸引用户；
- 标题长度和内容：不要过长，一般在40个字(80个字节)以内，并充分突出关键词的比重；
如果更长的标题搜索引擎一般会忽略掉，所以要尽可能将主要关键词放在标题靠前的位置。使用：文章标题_类目名称_频道名称。省略掉不必要的形容词吧，毕竟用户主要通过名词来找到需要的内容。标 题内容：尽量用一些别人可以通过关键词找到的字眼（也别太过头，如果标题中的字眼超过1半内容中都没有，有可能被搜索引擎排除出索引），因此基于web日 志中来自其他搜索引擎的关键词查询统计非常必要。而且可以通过搜索引擎的相关关键词发现更多的用户[习惯搜索关键词](http://www.chedong.com/blog/archives/001411.html)，比如潜在用户更习惯搜索问题的症状，而不是专业术语；
- 如果网页很多的话，尽量使用不同的网页标题，争取让自己网站的内容更多的进入搜索引擎索引范围；
因为搜索引擎会根据页面内容的相似度把一些内容当成重复页面排除出索引范围；
[http://www.chedong.com/phpMan.php](http://www.chedong.com/phpMan.php)是 我的网站上的一个小应用：一个web界面的unix命令手册（man page），在以前的设计中所有动态页面使用的都是同样的标题："phpMan: man page /perldoc /info page web interface" ，Google索引了大约3000多个页面，后来我将页面标题改成了"phpMan:  [命令名]"这样的格式，比如："phpMan: ls"，这样大部分动态页面的标题就都不一样了，一个月后Google从这个页面入口索引了大约6000个页面。因此，如果网站中很多网页都使用相同的标 题，比如：“新闻频道”，“论坛”，这些页面中很大一部分就会被排重机制忽略掉。
- 除了<title></title>外，还可以用<h1></h1>标题行突出内容主题， 加强标题的效果；
在我的网站设计中：我会把用<h1>[标题]</h1>这样的模板把标题突出显示，而不是通过改变字体的方式突出标题。
## **其他网站设计提示**
- 静态链接：Blog网站另外一个优势在于其网页是静态链接：动态网页占到整个互联网内容的90％以上。各个搜索引擎其实能够表现的都只不过是互联网的冰山一角(3-5%)，不同的只是谁让优质网页 排名靠前的策略更优秀而已：大 部分搜索引擎都认为静态链接的网页是优质网页，Google在优先抓取索引的网页中70%以上是不带参数链接的静态网页。而且即使同样的内容，静态网页也 会比动态网页权重高：很容易理解：query?a=1&b=2这样的链接由于参数顺序颠倒的query?b=2&a=1完全相同。尽量使用静态网页：目前能够动态网页进行全面的索引还比较难，而即使是Google也更不会索引所有的内容，因此很少会对参数超过2个的动态网页进行进一步的抓取和分析。以下是一个phpBB论坛页面返回的HTTP头信息：
HTTP/1.1 200 OK
Date: Wed, 28 Jan 2004 12:58:54 GMT
Server: Apache/1.3.29 (Unix) mod_gzip/1.3.26.1a PHP/4.3.4
X-Powered-By: PHP/4.3.4
Set-Cookie: phpBB_data=a%3A0%3A%7B%7D; expires=Thu, 27-Jan-2005 12:58:54 GMT; path=/;
Set-Cookie: phpBB_sid=09f67a83ee108ecbf11e35bb6f36fcec; path=/;
Content-Encoding: gzip
Cache-Control: private, pre-check=0, post-check=0, max-age=0
Expires: 0
Pragma: no-cache
Connection: close
Content-Type: text/html
为了避免隐私问题：Googlebot可以通过对页面http header中的session id和session cookie进行一些甄别，这样很多需要认证信息的论坛内容就无法进入索引了。
总体上说[Google喜欢新的，静态的内容](http://googlewebmastercentral.blogspot.com/2009/02/specify-your-canonical.html)。因此无论从效率上讲还是方便搜索引擎收录：使用内容发布系统将网站内容发布成静态网页都是非常必要的，从某种程度上说google friendly = anonymous cache friendly。
比如：http://www.chedong.com/phpMan.php/man/intro/3
比http://www.chedong.com/phpMan.php?mode=man¶meter=intro§ion=3这样的链接更容易进入搜索引擎的索引。而且在URL中的命中也能突出关键词。虽然Google官方的Blog不建议改写URL，但猜测更真实的意思是不要轻易更新已经被收录的URL，而且也提供了[canonical url](http://googlewebmastercentral.blogspot.com/2009/02/specify-your-canonical.html)机制方便重新声明不同的URL地址；
- 能够进入Google索引的页面数量越多越好。用类似以下的脚本可以统计自己的网站被搜索引擎索引的情况。
#!/bin/sh
YESTERDAY=`date -d yesterday +%Y%m%d`
# for FreeBSD: YESTERDAY=`date -v-1d +%Y%m%d`
LOG_FILE='/home/apache/logs/access_log'
grep -i Googlebot $LOG_FILE.$YESTERDAY|awk '{print $7}' |sort | uniq -c | sort -rn > spider/$YESTERDAY.googlebot.txt
grep -i baiduspider $LOG_FILE.$YESTERDAY|awk '{print $7}' |sort | uniq -c | sort -rn > spider/$YESTERDAY.baiduspider.txt
grep -i msnbot $LOG_FILE.$YESTERDAY|awk '{print $7}' |sort | uniq -c | sort -rn > spider/$YESTERDAY.msnbot.txt
grep -i inktomi $LOG_FILE.$YESTERDAY|awk '{print $7}' |sort | uniq -c | sort -rn > spider/$YESTERDAY.inktomi.txt
grep -i openbot $LOG_FILE.$YESTERDAY|awk '{print $7}' |sort |uniq -c | sort -rn > spider/$YESTERDAY.openbot.txt
    
- 网站目录结构要扁平，因为每深一级目录，PAGERANK降低1－2个档次。假设首页是3，其子可能目录就是1了，更深可能就无法列入评级范围了。
- 表现和内容的分离：“绿色”网页
网页中的javascript和css尽可能和网页分离，一方面提高代码重用度（也方便页面缓存），另外一方面，由于有效内容占网页长度的百分比高，也能提高相关关键词在页面中的比重也增加了。总之，应该鼓励遵循w3c的规范，使用更规范的XHTML和XML作为显示格式便于内容更长时间的保存。
- 让所有的页面都有方便蜘蛛发现的快速入口：[站点地图](http://www.chedong.com/sitemap.html) /[RSS ping](http://blogsearch.google.com/ping)， 方便网页爬虫（spider）快速遍历网站所有需要发布的内容。如果首页就是用Flash或图片进入的话，无异于将搜索引擎拒之门外，除了UI设计的用户 友好外，spider friendly也是非常重要的。
- 保持网站自身的健康：经常利用[坏链检查工具](http://home.snafu.de/tilman/xenulink.html)检查网站中是否有死链。
- 保持网页内容/链接的稳定性和持久性：在搜索引擎索引中网页存在的历史也是一个比较重要的因素，而且历史比较久的网页被链接的几率越高。为了 保证自己网页能够被比较持久的被其他网站的页面引用，如果自己网页中有链接更新时，最好能保留旧的页面并做好链接转向，以保持内容的连续性。要知道，把一 个网站和内容在搜索引擎中的排名“培养”的很高是一件非常不容易的事情，谁都不希望好不容易自己的内容被别人找到了，点击却是“404 页面不存在”吧，因此站点管理员对自身站点error.log的分析也是非常必要的。
- 文件类型因素：Google有对PDF, Word(Power Point, Excel), PS文档的索引能力，由于这种文档的内容比一般的HTML经过了更多的整理，学术价值一般比较高，所以这些类型的文档天生就比一般的HTML类型的文档 PageRank要高。因此，对于比较重要的文档：技术白皮书，FAQ，安装文档等建议使用PDF PS等高级格式存取，这样在搜索结果中也能获得比较靠前的位置。
- 常常能发现门户站点的一条新闻往往比其他站点的首页排名还要靠前。因此一个站点总体PageRank提高了以后，往往自身一些并不重要的内容 也会被同那些高PageRank的内容一起带入被搜索引擎优先查询的列表中。这样经常造成很多大的开发站点的邮件列表归档往往比其他站 点的首页PageRank还要高。
## **知己知彼——站点访问统计/日志分析挖掘的重要性**
网站设计不仅仅只是被动的迎合搜索引擎的索引，更重要是充分利用搜索引擎带来的流量进行更深层次的用户行为分析。目前，来自搜索引擎关键词统计几乎 是各种WEB日志分析工具的标准功能，相信商业日志统计工具在这方面应该会有更强化的实现。WEB日志统计这个功能如此重要，以至于新的RedHat 8中已经将日志分析工具webalizer作为标准的服务器配置应用之一。
以Apache/webalizer为例，具体的做法如下：- 记录访问来源：
在Apache配置文件中设置日志格式为combined格式，这样的日志中会包含扩展信息：其中有一个字段就是相应访问的转向来源： HTTP_REFERER，如果用户是从某个搜索引擎的搜索结果中找到了你的网页并点击过来，日志中记录的HTTP_REFERER就是用户在搜索引擎结 果页面的URL，这个URL中包含了用户查询的关键词。
- 在webalizer中缺省配置针对搜索引擎的统计：如何提取HTTP_REFERER中的关键词
webalizer中缺省有针对yahoo, google等国际流行搜索引擎的查询格式：这里我增加了针对国内门户站点的搜索引擎参数设置
SearchEngine yahoo.com p=
SearchEngine altavista.com q=
SearchEngine google.com q=
SearchEngine    sina.com.cn word=
SearchEngine    baidu.com   word=
SearchEngine    sohu.com    word=
SearchEngine    163.com q=
通过这样设置webalizer统计时就会将HTTP_REFERER中来自搜索引擎的URL中的keyword提取出来，比如：所有来自 google.com链接中，参数q的值都将被作为关键词统计下来：，从汇总统计结果中，就可以发现用户是根据什么关键词找到你的次数，以及找到你的用户 最感兴趣的是那些关键词等，进一步的，在webalizer中有设置还可以将统计结果倒出成CSV格式的日志，便于以后导入[数据库](http://lib.csdn.net/base/mysql)进行历史统计，做更深层 次的数据挖掘等。
以前通过WEB日志的用户分析主要是简单的基于日志中的访问时间/IP地址来源等，很明显，基于搜索引擎关键词的统计能得到的分析结果更丰富、更直 观。因此，搜索引擎服务的潜在商业价值几乎是不言而喻的，也许这也是[Yahoo!](http://www.yahoo.com/)[Altavista](http://www.av.com/)等传统搜索引擎网站在门户模式后重新开始重视搜索引擎市场的原因， 看看[Google的年度关键词统计](http://www.google.com/press/zeitgeist2002.html)就 知道了，在互联网上有谁比搜索引擎更了解用户对什么更感兴趣呢？
请看本站的反相链接统计：[chedong.com的awstats统计](http://www.chedong.com/cgi-bin/awstats/awstats.pl?config=chedong)
需要注意的是：由于Google针对Windows 2000中的IE使用的是UTF-8方式的编码，因此很多统计有时候需要在UTF-8方式下查看才是正确字符显示。从统计中能够感受到：在使用水平比较高 的IT开发人员中Google已经成为最常用的搜索引擎。而使用百度的用户也已经大大超过了传统的搜狐，新浪等门户站点，因此传统门户网站在搜索引擎上的优势将是非常脆弱的。而从技术的发展趋势来看，以后还会有更多的利用互联网媒体做更深层次数据挖掘的服务模式出现。
[转载自cnblog.org——“突发”文字可能揭示社会趋势](http://www.cnblog.org/blog#89372927)
在“新科学家”(New Scientist)在线杂志上，公布了康奈尔大学的一个新研究成果，引人注目，也许与Google 收购Pyra的动机有关。
这所大学的计算机科学家 Jon Klenberg 开发了一个计算机算法，能够识别一篇文章中某些文字的“突发”增长，而且他发现，这些“突发”增长的文字可以用来快速识别最新的趋势和热点问题，因此能够 更有效地筛选重要信息。过去很多搜索技术都采用了简单计算文字/词组出现频率的方法，却忽略了文字使用增加的速率。
Jon 特别指出，这种方法可以应用到大量Weblog上，以跟踪社会趋势，这对商业应用也很有潜力。例如，广告商可以从成千上万的个人Blog 中快速找到潜在的需求风尚。而且只要Blog 覆盖话题范围足够大（实际上发展趋势确实如此），这项技术对政治、社会、文化和经济等领域也都会有实际意义了。
虽然Google 新闻的内部算法至今没有公开，但是人们猜测这种完全由机器所搜集的头条新闻应当不是Google搜索引擎中惯用的鸽子算法，很可能与这种“突发”判断算法 有关。如此说来，[Google收购Blog工具供应商的举动确实还有更深层次的远见了](http://www.chedong.com/blog/archives/000433.html)。
- NewScientist.com news, [Word 'bursts' may reveal online trends](http://www.newscientist.com/news/news.jsp?id=ns99993405)
- 还没有写完这些介绍，在 SlashDot 上也看到了很多有关这个发现的讨论
## 附：[Google官方的站点设计指南](http://www.google.com/webmasters/guidelines.html)
- Make a site with a clear hierarchy and text links. Every page should be reachable from at least one static text link.  让网站有着清晰的结构和文本链接，所有的页面至少要有一个静态文本链接入口
批注：尽量不要用图片和JAVASCRIPT
- Offer a site map to your users with links that point to the important parts of your site. If the site map is larger than 100 or so links, you may want to break the site map into separate pages.
为用户提供一个站点地图：转向网站的重要部分。如果站点地图页面超过100个链接，则需要将页面分成多个页面。
批注：索引页不要超过100个链接：SPIDER只考虑页面中头100个链接
- Create a useful, information-rich site and write pages that clearly and accurately describe your content.
用一些有用的，信息量丰富的站点，清晰并正确的描述你的信息。
- Think about the words users would type to find your pages, and make sure that your site actually includes those words within it.
想像用户可能用来找到你的关键词，并保证这些关键词在网站中出现。
批注：少用“最大”，“最好”之类的形容词，用用户最关心的词，比如：下载，歌星名字，而不是一些抽象名词。
- Try to use text instead of images to display important names, content, or links. The Google crawler doesn't recognize text contained in images.
尽可能使用文本，而不是图片显示重要的名称，内容和链接。GOOGLE的机器人不认识图片中的文字。
- Make sure that your TITLE and ALT tags are descriptive and accurate.
保证：页面的TITLE和ALT标记正确的精确描述
- Check for broken links and correct HTML.
检查坏链并修正这些HTML错误。
- If you decide to use dynamic pages (i.e., the URL contains a '?' character), be aware that not every search engine spider crawls dynamic pages as well as static pages. It helps to keep the parameters short and the number of them small.
如果你打算使用动态页面：链接中包含"?"，必须了解：并非所有的搜索引擎的机器人能想对待静态页面一样对待动态页面，保持动态页面的参数尽可能的少也会 很有帮助。
- Keep the links on a given page to a reasonable number (fewer than 100).
让一个页面中的链接少于100个。
批注：用lynx -dump http://www.chedong.com 可以模拟从robot角度看到的页面。其最后有链接统计
输出类似：
   [1]Google Free Search _______________________________ Google Search
   (_) Search WWW (_) Search chedong.com
   更 新 [2]站点地图 / Site Map [3]留 言簿 / Guest Book [4]意见反馈 /
   Feed Back
    ...
References
   Visible links
   1. http://www.google.com/services/free.html
   2. http://www.chedong.com/sitemap.html#sitemap
   3. http://www.chedong.com/guestbook/
   4. http://www.chedong.com/formmail.htm
    ...
   Hidden links:
  50. http://www.chedong.com/bbcweb/
    ...
搜索引擎的宗旨在于提取互联网中质量最好的内容提供给用户，任何有利于帮助用户获得相对公正，优质内容的策略都是搜索引擎追求目标。PageRank是一个非常好的策略，但是并非所有策略都是基于非常复杂的算法。
从搜索引擎看来什么是互联网中“好”的内容呢？
- 首先：互联网大约是8G个网页，而且以每天2M的速度增长的。其中80%以上是动态网页，而占总量20%的静态网页就是一个相对比较简单的过滤规则。
- 其次：用户友好（User friendly）也是很重要的方面，搜索引擎利用算法帮助提升这些优质网站，包括： 通过CSS将内容和表现分离：较少的javascript和frame结构，spider本身也很难深入抓取这些网页：Javascript和frame结构大部分是广告。
- 标题明确：无标题，重复标题或者标题SPAM（类似于：游戏游戏游戏游戏游戏游戏这样的标题）进行过滤或降低得分 页面大小：因为页面过大会导致用户下载缓慢，所以很多引擎只计算页面大小在100k以内的网页。
- 链接引用：不仅需要有链接链入，也需要帮助用户找到其他更有价值的内容；
- 文件类型：PDF和DOC等专业文档和来自edu,gov等非赢利网站的内容；
- 链入网站的文字：所有用户不可见的因素全部被忽略。此外：[用户搜索的行为本身也被Google记录](http://weblog.chedong.com/archives/000397.html)，可能对目标网站的主题相关度有帮助。
参考资料：
面向Google搜索引擎的网站设计优化
[http://www.google-search-engine-optimization.com/](http://www.google-search-engine-optimization.com/)
[关于Google排名优化的7个误区，如："Meta tag", "泛域名"，首页等](http://www.rankforsales.com/seo-myths.html)
如何评价一个网站的人气
[http://www.chedong.com/tech/link_pop_check.html](http://www.chedong.com/tech/link_pop_check.html)
如何提高网站在Google中的排名——面向搜索引擎的广告模式
[http://www.chedong.com/tech/google_ads.html](http://www.chedong.com/tech/google_ads.html)
如何提高网站在Google中的排名——面向搜索引擎的网站链接设计
[http://www.chedong.com/tech/google_url.html](http://www.chedong.com/tech/google_url.html)
Google不断改进相应的算法：HillTop
[Hilltop: A Search Engine based on Expert Documents](http://www.cs.toronto.edu/~georgem/hilltop/)
Google の秘密 - PageRank 徹底解説
[http://www.kusastro.kyoto-u.ac.jp/~baba/wais/pagerank.html](http://www.kusastro.kyoto-u.ac.jp/~baba/wais/pagerank.html)这篇文章是在查"Google PageRank"的时候查到的，这篇文章不仅有一个算法说明，也是一个Google的weblog，记录了很多关于Google的新闻和一些市场动态信 息。
[Google 的秘密- PageRank 彻底解说 中文版](http://www.kreny.com/pagerank_cn.htm)
更详细的PageRank算法说明：
[http://pr.efactory.de/](http://pr.efactory.de/)
[WEB日志统计工具AWStats的使用：增加了Unicode的解码和中国主要门户搜索的定义](http://www.chedong.com/tech/awstats.html)
http://www.chedong.com/tech/awstats.html
Robots的说明：
[http://bar.baidu.com/robots/](http://bar.baidu.com/robots/)
[http://www.google.com/bot.html](http://www.google.com/bot.html)
搜索引擎通过一种程序robot（又称spider），自动访问互联网上的网页并获取网页信息。您可以在您的网站中创建一个纯文本文件 robots.txt，在这个文件中声明该网站中哪些内容可以被robot访问，哪些不可以。
反Google站点：观点也很有趣
[http://www.google-watch.org/](http://www.google-watch.org/)
关于Google的WebLog
[http://google.blogspace.com/](http://google.blogspace.com/)
[关于Google的HillTop算法](http://www.21cnbj.com/industrynews/articles_2003/Expert00-3.htm)
搜索引擎相关论坛
[http://searchengineforums.com/](http://www.jimworld.com/apps/webmaster.forums/)
[http://searchenginewatch.com](http://searchenginewatch.com/)
[http://www.webmasterworld.com/](http://www.webmasterworld.com/)
