
# 纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT） - 雷锋网 - CSDN博客


2016年12月26日 17:05:08[leiphone](https://me.csdn.net/leiphone)阅读数：3884


导语：黄正，百度安全实验室 X-Team 掌门人。他在这里分享了诸多漏洞挖掘的干货，目标成为大神黑客的你不能错过。

浏览器就像一扇窗，通过这扇窗，黑客可以攻入电脑的心脏。
就像情场高手，通过眼睛，融化一个人的心灵。
**黄正，百度安全实验室 X-Team 掌门人。**2016年，这个信仰“技术可以改变世界”的低调黑客大牛以一己之力挖掘无数浏览器漏洞，创下了排名微软 MSRC 2016 年度黑客贡献榜中国区第一（世界第八）的壮举。
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585b952ac7204.jpg?imageMogr2/format/jpg/quality/90)
从一个安全开发工程师华丽转身，成为安全研究员，黄正最终站在了中国浏览器漏洞挖掘的顶峰。本期硬创公开课，我们将会请到黄正为雷锋网宅客频道的读者童鞋们奉献一场纯干货——如何用正确的姿势挖掘浏览器漏洞。
从上千万行代码中，精准地找到那个微小的漏洞，恰如站在万里之遥，拉弓搭箭。正中靶心。
**以下，是雷锋网宅客频道（公众号：宅客频道）诚意奉上的公开课全文及完整 PPT。**
## 关于我自己
我是百度安全实验室的黄正，在百度参与过网页挂马检测、钓鱼欺诈检测、病毒木马分析、伪基站检测、泛站群打击、漏洞挖掘和漏洞利用等。
在百度做恶意网页检测相关的开发工作，可以说是一个职业“鉴黄师”，其实“鉴黄”是一个非常有挑战的事情，每天要检测几亿/几十亿的网页，怎样设计存储、调度、检测算法，才能在有限的服务器、带宽，在有限的时间内检测完，还要去保证检出率和误报率。
所以恶意网页检测是一个比较偏工程的方向，我呢还是有一颗黑客的心，还挑战一些新的安全技术方向。想去挖漏洞，所以在早期组建安全实验室团队时就加入了 X-Team，从安全开发工程师切换到做浏览器漏洞挖掘，今天给大家分享的议题是浏览器漏洞挖掘，希望大家能有所收获。
今天先给大家科普一下浏览器的一些基础知识，再回顾看一下浏览器漏洞类型，重点讲怎么去做一个 Fuzz 系统，以及分享一些我们曾经发现过的一些漏洞POC，最后再分享一下怎么给微软、GOOGLE、苹果报告安全漏洞。
## 浏览器基础知识
首先，浏览器是干嘛用的。大家都知道是用来看网页用的，再往底层一点呢，它是把html代码按照一定的标准绘制成在屏幕上肉眼可见的图像用的。
比如，像这段html代码，浏览器会按照一定的标准去把他变成一张肉眼可见的网页：
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585b9ebfa7b4b.png?imageMogr2/format/jpg/quality/90)
浏览器广泛存在各个地方，Windows，Mac OS X，iOS/iPad，Android，Play station(PS 4)，QT，ATM。微信里也有浏览器，可能你每天都在用，但是你不知道。
那么浏览器是怎么把 Html 代码渲染成网页的呢？来看一张浏览器渲染引擎的结构图：
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585b9ecacf4e4.png?imageMogr2/format/jpg/quality/90)
渲染引擎大致可以分成4块：
> HTML解释器：
> 将HTML文本解释成DOM；

> CSS解释器：
> 解析CSS，为DOM中的各个元素计算样式信息；

> 布局：
> 根据元素与样式信息，计算大小、位置、布局；

> Javascript引擎：
> 解释js脚本，修改DOM、CSS。
**最后使用图像库，**将布局绘制成图像结果，也就是人眼看见的网页。
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585b8b1a6fe11.png?imageMogr2/format/jpg/quality/90)
**什么是DOM树呢？**
DOM树是把 HTML 文档呈现为带有元素、属性和文本的树结构。比如以下HTML将被解析成如下DOM树：
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585ba01661e7d.png?imageMogr2/format/jpg/quality/90)
使用 DOM 给 JS 开放的接口，可以用用 JS 操作 DOM 对象的属性。这里列举一些常见的 DOM 方法：
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585ba030148df.png?imageMogr2/format/jpg/quality/90)
调用这些 DOM 方法，对 DOM 的修改，最终会体现在绘图上，也就是肉眼可见的网页变化。DOM的标准也在不断在升级：
> DOM level 1、DOM level 2、DOM level 3、DOM level 4...
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585ba05065e64.png?imageMogr2/format/jpg/quality/90)![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585ba051c5fda.png?imageMogr2/format/jpg/quality/90)![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585ba053f3bc2.png?imageMogr2/format/jpg/quality/90)
浏览器是一个很复杂的工程，不仅要去支持不同时候的DOM标准，而且不同的浏览器为了让解析的速度变得更快，让用户使用得更方便，在不断地增加新功能。从13年的新闻来看，Chrome 浏览器的代码应该至少有1000万行。
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585ba0af1801d.png?imageMogr2/format/jpg/quality/90)
代码越多，开发的人员越多，就容易出现安全问题。接着介绍一下浏览器的漏洞类型，我自己的分类方法，不一定合理。
## 浏览器漏洞类型
### 第一类漏洞是信息泄漏漏洞，有以下几种场景：
> 比如老的IE浏览器可以用 JS 检测是否存在某文件。
> 这样会泄漏用户是否安装了杀毒软件，以便进行下一步操作。

> 有些信息泄漏漏洞会泄漏内存的信息，
> 攻击者可能使用泄漏的信息来绕过操作系统的保护机制，
> 比如：ASLR、DEP 等等。

> 还有一类信息泄漏漏洞：UXSS，通用跨域漏洞。
> 这种跨越域隔离策略的漏洞，可以在后台偷偷地打开其它域的网页，把你在其它网站上的数据偷走。

![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585ba0bc7a033.png?imageMogr2/format/jpg/quality/90)
### 第二类漏洞是内存破坏漏洞：
**内存破坏漏洞又可以细分为释放后重用漏洞（UAF），越界读写漏洞，类型混淆漏洞。**越界读写漏洞比较神，他也能用来泄漏内存信息，所以越界读写漏洞非常好利用。
### 第三类漏洞，是国产浏览器的一些神漏洞，特权域 XSS+特权域 API：
比如搜狗浏览器浏览网页可下载任意文件到任意位置，重启之后，电脑可能被完全控制。百度浏览器可静默安装插件，且存在目录穿越漏洞。解压缩到启动目录，重启之后，电脑被完全控制。
### 利用这些浏览器漏洞可以做什么可怕的事情呢？
想像一下：
> 打开一个网页，完全控制你的电脑，APT攻击；

> 打开一个网页，完全控制你的手机，监听通话短信；

> 打开一个网页，越狱你的游戏机，玩盗版游戏；

> 打开一个网页，偷走你京东帐号、控制微博帐号…….
我们曾经抓到一个打 DDoS 的嫌疑人，他控制了200个肉鸡，接单打 DDoS。这200个肉鸡每天给他创造2000块的收入，如果有一套组合的浏览器0day+一个大站 Webshell，一天上千的肉鸡非常简单，日入万元不是梦，哈哈。。。
## 浏览器漏洞挖掘方法
那怎么挖掘浏览器漏洞呢？
### 第一种方法，人工
像Chrome 和 Webkit 这些是开源的，通过理解代码逻辑来找 Bug。比如 marius.mlynski 读 Chrome 代码发现数十个 UXSS 漏洞，每个漏洞Google奖励 7500 美元。
国产浏览器的漏洞大多是人工测试出来的，用自己的经验，逆向分析，手工测试。
### 第二种方法，自动化 Fuzz
用机器代替人来自动化挖漏洞。
有一些开源的框架可以学习：Grinder、Fileja、funfzz等。我们来看一下一个Fuzz框架是怎样实现的。
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585ba0fddd788.png?imageMogr2/format/jpg/quality/90)
简单地说就是控制浏览器去加载你生成的测试样本，捕捉导致浏览器崩溃的样本，传回服务器作下一步分析。今天给大家分享一个最简单的Fuzz框架，一行代码，启动调试器打开样本生成器网页：
> windbg -c “!py chrome_dbg_test.py” -o “C:\Program Files\Google\Chrome\Application\chrome.exe --js-flags=”--expose-gc" --no-sandbox --disable-seccomp-sandbox --allow-file-access-from-files --force-renderer-accessibility http://127.0.0.1/fuzzer.php
解释上面的命令：
> 使用windbg调试启动chrome打开样本生成器http://127.0.0.1/fuzzer.php

> http://127.0.0.1/fuzzer.php生成测试样本并切换样本。
一个最简单调试器，十几行代码，pydbg 是 windbg 的一个插件，使用它可以编程控制 windbg。
比如用这段代码，监控调试进程产生的调试事件：
> while True:

> exc =  e(".lastevent")

> print exc

> if exc.find("Exit process") > 0:

> os.system("taskkill /F /IM windbg.exe")
观察windbg收到的调试事件，如果是退出进程事件，那么把windbg退出（同时chrome进程也退出了）。同理，如果windbg收到的调试事件是读写内存异常，那就是一个有效崩溃。
这时候你只需要执行windbg命令r,k，把结果传回服务器就完成了一个崩溃信息的收集了。
要想挖掘浏览器漏洞，推荐大家去分析以往浏览器的漏洞，收集一些POC，这样能更好地理解浏览器漏洞形成的原因以及调试分析方法。来看一个典型IE漏洞的POC：
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585ba2035a3bc.png?imageMogr2/format/jpg/quality/90)
所以我们的目标是怎样去生成有效的、复杂的、测试用例，让浏览器崩溃。
我们需要去构建 fuzz 生成算法用的字典，通常可以写爬虫到 MSDN、MDN、W3C 去抓取，还有可以用 IDA 分析 mshtml 找隐藏的属性键值，还可以看浏览器源码的头文件。我们需要去构建全面的字典，去覆盖浏览器的各个特性。
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585ba20f3f910.png?imageMogr2/format/jpg/quality/90)
网上也有些自动构建字典的方法，可以参考
> var elements = [“abbr”, “acronym”, “address”, “article”, “aside”, “b”……];

> for (var i = 0; i < elements.length; i++) {

> var element = document.createElement(elements[i]);

> for (var key in element) {

> if (typeof element[key] == "function") {

> function_list.push(key);

> }

> }
构建出来的字典像这样：
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585ba21eb88dd.png?imageMogr2/format/jpg/quality/90)![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585ba21c9eb2d.png?imageMogr2/format/jpg/quality/90)
再来一个生成算法，用一个CSS生成算法来举例，使用刚才收集回来的字典，随机生成测试用的CSS。
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585b98b6cc870.png?imageMogr2/format/jpg/quality/90)
好了，有了源材料和随机生成方法，现在你可以生成测试样本了。
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585b8d1c48bfe.png?imageMogr2/format/jpg/quality/90)
然而还有一个大问题：
如果只是这样随机生成，跑一年有可能会出来一个有效的崩溃，这是个很可悲的事情。为什么？？？？
> 一是因为测试的总样本集非常大，比如一共有110个html标签的类型，如果我们随机创建10个，那就是110的10次方种可能，这是个超级大的数，大到你可能永远都跑不完，二是前辈们可能10年前就跑过了，能发现的问题早就报告修复完了。
**所以FUZZ的核心是样本生成算法。**
我们需要新的样本生成策略，去发现别人发现不了的漏洞。曲博在 2014 BlackHat 分享了一个有意思的故事：
> 曲博他老婆的iPhone屏幕坏了，让曲博修，他拆开后，装上了新屏幕，差不多快好了，发现多了两个螺丝，摄像头不见了……

> 最终iPhone废了
QU BO 得出来的启示是：
> 工程师把东西拆开的时候容易犯错误，工程师把东西恢复的时候容易犯错误。
应用到fuzz这块，有两个策略，去做相反的事情，去改变初始的状态。比如：
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585b8da760820.png?imageMogr2/format/jpg/quality/90)
我们应用这种想法也找到了一些漏洞：
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585b8dbc289a0.png?imageMogr2/format/jpg/quality/90)![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585b8dbdb6184.png?imageMogr2/format/jpg/quality/90)![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585b8dbf0fdf1.png?imageMogr2/format/jpg/quality/90)
其中在**document.execCommand("undo");**这块我们至少收获了5枚漏洞。但是也有不少撞洞的情况。
**所以我们需要样本生成策略，去发现别人发现不了的漏洞。**
我们首先想的是把调用关系做得非常深，制造非常复杂的回调场景。比如以下这个POC，我们多次在事件响应函数中去触发另一个事件响应函数，最终导致解析出错：
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585b8de22fcd7.png?imageMogr2/format/jpg/quality/90)
看下这个崩溃的调用栈：
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585ba31b397be.jpg?imageMogr2/format/jpg/quality/90)
我们还可以去fuzz别人搞得少的方向。
### Fuzz 别人搞得少的方向，第一个是
### OBJECT。
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585b8e75b642d.png?imageMogr2/format/jpg/quality/90)
**CLSID:8856F961-340A-11D0-A96B-00C04FD705A2**应该是一个浏览器控件，浏览器控件是有自己独立的前进、后退功能的，如果与父页面的前进后退功能混用，会导致问题。
我们整理了系统中所有clsid：
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585b8ed01e24d.jpg?imageMogr2/format/jpg/quality/90)
在OBJECT这块我们至少也收获了5个漏洞。
### Fuzz 别人搞得少的方向，第二个是unicode。

![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585b8f07976e2.png?imageMogr2/format/jpg/quality/90)
很少看见有人做fuzz的时候会随机的用一些unicode字符串来测试，我们在这块收获了EDGE一个越界读漏洞。
### Fuzz 别人搞得少的方向，第三种是正则表达式。
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585b8f399c687.png?imageMogr2/format/jpg/quality/90)
### fuzz别人搞得少的方向，第四种是JS解析引擎。
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585b8f7729187.png?imageMogr2/format/jpg/quality/90)
现在JS引擎的内存破坏漏洞越来越多了。我们在这块也挖到不少漏洞，由于目前还都是 0day 的状态，所以不拿我们自己发现的 POC 举例，拿一个 Google Project Zero 分开的 POC 给大家看看长什么样：
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585b8fcba3997.jpg?imageMogr2/format/jpg/quality/90)
### Fuzz 别人搞得少的方向，第五种是混合生成。
现在很多人生成的fuzz样本都是用JS生成的，但是对HTML的初始状态关注并不多，比如我们在初始状态时加了N个复杂的表格嵌套，也发现了不少的问题。
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585b8fefae33a.jpg?imageMogr2/format/jpg/quality/90)
### 去fuzz非常复杂的元素，比如表格、表单、Frame。
表格相关的各种元素都有特有方法，比如insertRow、moveRow、deleteRow，还有表单的特有的属性和方法，如果跟CSS结合，那么渲染起来就更加复杂了，比如下面这个POC：
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585b8f5ccfe03.png?imageMogr2/format/jpg/quality/90)
通过fuzz非常复杂的元素，我们至少收获了10个漏洞。
去fuzz非常复杂的元素，还有CSS。CSS也有很多种操作方法，比如：
> style，<div data-for="result" style="height:0;width:0;overflow:hidden;" id="swfsocketdiv">

> obj.style.backgroundColor= "black";

> .style1:hover{ background-color:\#66B3FF; cursor:pointer;}

> obj.style.cssText = "background-color:black; display:block;color:White;

> obj.setAttribute("class", "style2");

> <link href="css1.css" rel="stylesheet" type="text/css" id="css"/>

> obj.setAttribute("href","css2.css");

> document.styleSheets
CSS混合生成的方法，我们收获了几枚很好利用的类型混淆漏洞。
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585b966b81e70.png?imageMogr2/format/jpg/quality/90)
还有，可以去fuzz新的特性，Html5，SVG，WebGL，Chrome 原生支持的 PDF。
## 怎样提交漏洞
好了，POC分享完了，下面讲一下怎么给产商报告漏洞。
### 怎么给微软提交漏洞
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585b91aacdd93.png?imageMogr2/format/jpg/quality/90)
注意的是，如果空指针的问题导致系统蓝屏的，微软也会收的。
### 怎么给Google提交漏洞
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585b923f79110.png?imageMogr2/format/jpg/quality/90)
**注意Chrome有很多个版本，只有stable版本的才会有CVE。（****Canary、Dev、Stable）**
**注意Chrome自己也有fuzz集群，如果你跟他撞洞，Sorry。。。**
### 怎么给苹果提交漏洞
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585b924052412.png?imageMogr2/format/jpg/quality/90)
好了，我的分享就到这里了。以下是引用的一些参考资料，大家也可以学习一下。
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585b9260902b1.png?imageMogr2/format/jpg/quality/90)
## Q&A
**雷锋网宅客频道读者：在浏览器的漏洞挖掘里老师用的最多的工具是神马。**
**黄正：**
大部分都是自己写代码写出来的，没有特别的工具，比如为了方便我们调试，我会写一个工具来自动配置启动浏览器：
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585b9366d43bb.jpg?imageMogr2/format/jpg/quality/90)
不知道是否回答了这位朋友的问题，其它的工具用的是 windbg，VS，python，php 等。都是用这些造出来的，没有用现成的像 sqlmap, burpsuite 那样的工具。
**雷锋网宅客频道读者：如何评价Pwn2Own2016中360安全卫士11秒攻破chrome?**
**黄正：**
其实11秒这个数字不重要，关键是攻破了，360牛人多、积累多，从上次他们的博客可以看出，手里有大把的0day，攻破chrome需要很多个漏洞组合，不止是浏览器漏洞，确实是能力超强啊。
**雷锋网宅客频道读者：深度学习与****[网络安全](http://www.leiphone.com/category/letshome)****（比如反APT，漏洞挖掘等）有没有很好的结合点？**
**黄正：**
这个百度已经有了一些尝试了，百度手机卫士移动端病毒检测使用百度开源的机器学习平台paddle对病毒样本的特征进行深度学习，检出率在国际权威排名中已经多次世界排名第一，感兴趣的可以看一下blackhat Europe 2016王磊的议题。
另外，百度安全实验室也在应用机器学习算法来做撞库检测、支付欺诈检测，效果都很不错，欢迎来交流。
**雷锋网宅客频道读者：八卦一下，黄正老师挖的第一个漏洞是什么？**
**黄正：**
发现的第一个是释放后重用漏洞，我可以详细说一下我从安全工程怎样革自己的命要搞漏洞的，并最终搞出来的。
成立安全实验室后，我给自己定的KPI是半年内，要搞通浏览器漏洞分析方法，搭建浏览器 Fuzz，并且挖出一个浏览器漏洞。
压力大啊，没有人带我，完全自己搞，每天都在不断地学习，不停地尝试修改生成算法，不断地去网上爬资料找POC，3个月过去了，一个漏洞都没发现，我想这下完了，年终奖要没了。后来终于在使用 setTimeout 来改变 location 发现了第一个可以稳定崩溃的 POC。并且是可利用完全没有任何保护机制防护的，下一步就是去精简样本，当时很土，手工慢慢删，最终受不了了写了自动化精简样本的脚本，然后就提交了，完成了半年发现一个漏洞
 KPI。
这是我的第一个漏洞：
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585b9977c4d5a.jpg?imageMogr2/format/jpg/quality/90)
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585b99767f7c4.jpg?imageMogr2/format/jpg/quality/90)
**雷锋网****(公众号：雷锋网)****：很多同学都想成为黄正老师一样的漏洞挖掘大牛。对于这些同学，黄老师有什么干货的建议吗？**
**黄正：**
别停留于想法，赶紧去做，哈哈，网上资料多得是。看看我打印的学习资料，这还只是一部分：
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585b99b807c00.jpg?imageMogr2/format/jpg/quality/90)
![纯干货：微软漏洞中国第一人黄正——如何用正确姿势挖掘浏览器漏洞（附完整 PPT）｜硬创公开课](http://static.leiphone.com/uploads/new/article/740_740/201612/585bb5635bd18.jpg?imageMogr2/format/jpg/quality/90)
雷锋网原创文章，未经授权禁止转载。详情见[转载须知](http://dwz.cn/4ErMxZ)。



