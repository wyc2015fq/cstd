# 不只是马甲 简析Chrome和Webkit的渊源 - xqhrs232的专栏 - CSDN博客
2013年02月24日 10:15:44[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：566
个人分类：[3G/RIL/TAPI/网络技术](https://blog.csdn.net/xqhrs232/article/category/906928)
原文地址::[http://pcedu.pconline.com.cn/soft/wl/brower/1205/2788574_all.html](http://pcedu.pconline.com.cn/soft/wl/brower/1205/2788574_all.html)
1深得世界人民喜爱的Webkit[回顶部](http://pcedu.pconline.com.cn/soft/wl/brower/1205/2788574_all.html#article_brief)
　　互联网的浪潮从未停息，而用以网上冲浪的冲浪板也一直在变得愈加精良。自人们进入互联网时代以来，即已经发生了三次[浏览器](http://pcedu.pconline.com.cn/2010/1029/zt2255362.html)大战。第一次[浏览器](http://dl.pconline.com.cn/sort/104.html)大战的主角是IE和[Netscape](http://dl.pconline.com.cn/html_2/1/104/id=647&pn=0.html)，最终IE凭借着Windows的庞大身躯推倒了Netscape；第二次浏览器大战Netscape浴火重生化身为火狐狸[Firefox](http://dl.pconline.com.cn/html_2/1/104/id=11283&pn=0.html)，一口咬了IE身上一大块肥肉；正在[Firefox](http://dl.pconline.com.cn/download/52175.html)和IE正缠绵不息之时，突然凭空杀出个[Chrome](http://dl.pconline.com.cn/download/51614.html)——这名出身豪门Google的小伙子一下子成长得额外精壮，上串势头凶猛，追得两位前辈娇喘吁吁。
![Chrome正在猛击两位前辈](http://img0.pconline.com.cn/pconline/1205/16/2788574_chrome.jpg)
[Chrome](http://dl.pconline.com.cn/download/51614.html)正在猛击两位前辈
　　这位Chrome究竟是何方人物，能练就如此神功，在短短几年内就成为互联网浏览的一大主流，市场份额赶超了Firefox不说，甚至还曾在短时间内压过了微软帝国的IE，形成天下三分的第三次浏览器大战的格局？
![Chrome已在市场份额上击败Firefox](http://img0.pconline.com.cn/pconline/1205/16/2788574_ChromeFirefox.jpg)
Chrome已在市场份额上击败了Firefox
**Chrome的血统**
　　出于好奇，不少人都八卦了一下Chrome的来历，然后发现Chrome的背后深藏着Webkit这个名字。对浏览器有所研究的朋友，应该也会或多或少地闻过Webkit的大名。Webkit源于KDE开源项目，兴盛于苹果公司的[Safari](http://dl.pconline.com.cn/html_2/1/104/id=44487&pn=0.html)项目，它身上有诸多创新，近年来风行的HTML5以及CSS3潮流都和Webkit脱不开关系。Webkit小巧、灵活但又十分强大，而且源代码开放，深得业界喜爱。从诺基亚S60上的浏览器，到价比肾贵的iPhone上的Safari，我们都能看到Webkit的身影。
![1](http://img0.pconline.com.cn/pconline/1205/16/2788574_Webkitsafari.jpg)
Webkit（左）和Safari的logo，一看就孽缘深
　　Google从来都不是嗅觉迟钝的公司，Webkit的优秀自然也吸引了这位互联网枭雄的眼光。2008年9月，Google发布了Chrome的测试版，Chrome面世了。Chrome使用了Webkit的代码，继承了Webkit的优良排版引擎，渲染页面速度惊人。
![Chrome第一个测试版0.2.149.27](http://img0.pconline.com.cn/pconline/1205/16/2788574_Chromebeta.png)
Chrome首个测试版
　　既然Chrome使用了Webkit的源代码，也使用了Webkit的排版引擎，那么我们是否就可以认为，Google只是在Webkit上面加了一层壳就做出了Chrome呢？
**Chrome的基因突变**
　　Chrome是Webkit的马甲，这种说法并不准确。实际上Webkit由两部分组成，一部分是WebCore排版引擎，用以解析HTML语言和CSS框架；另一部分为JSCore JavaScript执行引擎，用以执行网页JS脚本。Chrome只是继承了Webkit的WebCore部分，在JS引擎上使用了Google引以为豪的“V8”引擎，大大地提高了脚本执行速度，这也是为什么Chrome会如此快的重要原因。
![Webkit包括WebCore和JSCore](http://img0.pconline.com.cn/pconline/1205/16/2788574_Webkitst.jpg)
Webkit包括WebCore和JSCore
　　不仅如此，Chrome还在Webkit上封装了一层Webkit Glue，Chrome中只调用Webkit Glue层的接口使用Webkit组件。与此同时，Chrome的源代码和Webkit也有了很大区别，Google对Webkit的源代码重新进行了梳理，使代码的可读性更好，编译效率更高，并拥有自己的开源项目——Chromium。
2大受国内群众欢迎的Chromium[回顶部](http://pcedu.pconline.com.cn/soft/wl/brower/1205/2788574_all.html#article_brief)
**Chromium和Chrome：试制品和成品**
　　既然提到了Chromium，这里就顺带谈一下Chromium和[Chrome](http://dl.pconline.com.cn/download/51614.html)的区别。不知道从什么时候开始，流行着这样一种说法——[Chrome](http://dl.pconline.com.cn/download/51614.html)开源。这是错误的。Chrome并非一款自由软件，也没有开放源代码。甚至和[Firefox](http://dl.pconline.com.cn/html_2/1/104/id=11283&pn=0.html)比起来，Chrome还很封闭——因为它无法提供像[Firefox](http://dl.pconline.com.cn/download/52175.html)一样繁多而高权限的应用接口，这使得Chrome在扩展以及界面定制方面都不如Firefox自由。那Chrome是开源软件一说何来之有？
![Firefox十分开放](http://img0.pconline.com.cn/pconline/1205/16/2788574_Firefox.jpg)
Firefox十分开放
　　之所以这种说法，多是因为人们把Chrome和Chromium搞混了。虽然名字很像，图标界面功能什么的都差不多，也同样隶属于Google，但事实上这两者真的不同。Chromium是一款自由软件，遵循BSD许可证开源，开发者可以使用Chromium的源代码进行开发。我们可以认为Chromium是Google为了发展Chrome而开启的开源计划，Chromium相当于Chrome的实验开源版。两者功能也有微妙的差异，Chrome比Chromium多了[PDF阅读器](http://dl.pconline.com.cn/html_2/1/81/id=1322&pn=0.html)、[Flash
 Player](http://dl.pconline.com.cn/html_2/1/114/id=8122&pn=0.html)及Google Update等一些小部件。
![Chromium（左）和Chrome](http://img0.pconline.com.cn/pconline/1205/16/2788574_ChromiumChrome.jpg)
Chromium（左）和Chrome
　　简单来说，Chromium是一个实验项目，Chrome则是一件商品。开发者所能使用的代码只能是开源的Chromium，而不会是闭源的Chrome。若听到有人说，某某浏览器基于Chrome内核，速度快得很！这时候我们应该知道，这款浏览器实际上使用的是Chromium内核。
**广受欢迎的Chromium**
　　目前使用Chromium内核的著名国产浏览器有360极速浏览器、[搜狗浏览器](http://www.pconline.com.cn/tlist/1233.html)、[世界之窗](http://dl.pconline.com.cn/html_2/1/104/id=40059&pn=0.html)浏览器极速版等等，为数不少。但我们发现，在国内虽然Chromium内核很受浏览器厂商欢迎，但是更原始的Webkit却几近无人问津。这又是为何？
![人们更热衷于Chromium](http://img0.pconline.com.cn/pconline/1205/16/2788574_four1.jpg)
人们更热衷于Chromium
　　当然这也是有原因的。前面提到，Google对Webkit的代码重新梳理，Chromium代码的可读性和编译效率远比Webkit高。对比Chromium的代码，Webkit的代码堪比天书，开发难度高得多。Webkit这货不是你想弄，想弄就能弄的啊。更何况Chromium的V8 JS引擎比Webkit的JSCore效率更高，好用又强大的东西干嘛不用呢。
　　鉴于Chromium和Webkit拥有较大的区别，我们不应该单纯地认为Chrome/Chromium只是在Webkit上面套一个马甲。给Google的工程师们一些掌声吧，因为他们的优秀和努力，世界上多了一个优异的浏览器内核。
