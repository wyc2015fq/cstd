# 关于IE11,我们所知道的以及我们所能预料到的 document.all不支持 ？ - z69183787的专栏 - CSDN博客
2014年03月06日 10:35:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3272
**从IE 4开始，document.all在IE中举足轻重。比起document.getElementById()来说，document.all是IE方式的获取元素的引用的方法。尽管IE 5增加对DOM的支持，但document.all一直沿用至IE 10。而在Internet Explorer 11中终于被废弃了，这意味着在Internet Explorer 11中使用document.all的方法将可能导致执行失败，尽管使用了document.all的代码实际上还是可以工作。**
最近,一个开发代号为Windows Blue的Windows操作系统泄漏到了互联网上,该操作系统的内置浏览器为IE11,本文将介绍一下这个泄漏版的IE11中有哪些关键的新变化和新特性.
预先声明: 本文中所讲的内容都来自互联网,我自己没有安装过这个泄漏版的IE11,虽然我目前正在帮助微软的[userAgents社区](http://useragents.ie/)做一些工作,但我没有任何关于IE11未来计划的内部消息,本文只是对网上的那些消息做了一下总结,并加入了自己的看法和预测.
## 一个新的身份标识
关于IE11的第一个新闻就是它有了一个[新的用户代理(UA)字符串](http://www.neowin.net/news/ie11-to-appear-as-firefox-to-avoid-legacy-ie-css):
```
Mozilla/5.0 (IE 11.0; Windows NT 6.3; Trident/7.0; .NET4.0E; .NET4.0C; rv:11.0) like Gecko
```
其中有两个比较显著的变化:
- MSIE变成了IE
- 多了一个like Gecko字段
第一个变化很明显是想让网站上现有的UA检测失效(译者注:因为现有的检测代码都是在检测MSIE字样).UA检测是为那些老版本的IE设计的,比如它们不支持`addEventListener等符合标准的特性,就得走专门的代码分支`.IE这样做就是在表示,它不再需要那些专门的检测代码,它可以执行符合标准的代码了.
关于后一个变化,有报道称这是IE在伪装成Firefox,这并不完全正确.Gecko的确是Firefox的渲染引擎,这没问题,但like gecko这个字段是WebKit最先使用的,IE添加该字段的理由和WebKit当年的理由一样,那就是,网站通常不会这么问浏览器:"你想要IE私有的代码还是基于标准的代码?",而是这么问:"你是IE还是Gecko?",也就是说,即使你支持标准也没有用,你的名字必须得是Gecko.虽然目前WebKit和IE实际上比Gecko更流行,但为了兼容已有的那些老站点,必须要加上这个字段.被识别成Gecko比被识别成Webkit要好的多,因为假如被识别成Webkit,就会遇到很多WebKit私有的甚至带属性前缀的代码,尤其是在移动站点上,伪装成Gecko能好一点.
改变UA字符串并不是这个版本的IE11用来伪装自己身份的唯一手段,还有navigator.`appName属性现在返回了``Netscape,而不是以前的M``icrosoft Internet Explorer,从而与`WebKit和Gecko的实现达成一致.虽然Opera/Presto没有这么干,不过它马上要切换到Blink内核了,所以所有浏览器的`appName属性都将会是`Netscape.
**最后一个伪装方式,就是IE11现在会假装不支持`document.all`.如果你使用特性检测来检测浏览器是否支持`document.all`,它会像Firefox,WebKit,以及Opera一样,反回一个`false(译者注:在非IE浏览器中,`document.all`是个非常特殊的属性,它是一个对象值,但它同时又是一个假值,按照JS标准,这是不可能的,查看[http://www.w3help.org/zh-cn/causes/BX9002](http://www.w3help.org/zh-cn/causes/BX9002))`.为什么要这样做?难道特性检测不是一直提倡的做法吗?是的,但是,很多开发者们总用"特性检测"来充当"浏览器检测"(译者注:Maintainable JavaScript一书中把这种检测方式称之为"浏览器推断",是不推荐的做法),这是不对的,在Opera工作时,我曾无数次看到过下面这样的代码:**
```
var isIE = null;
if (document.all) {
 isIE = true;
}
if (isIE) {
  // IE私有的代码,比如ActiveX,滤镜相关的
} else {
  // 符合标准的代码
}
```
如果`document.all为真值`,则你马上断定它是IE,然后执行一些毫不相关的代码(译者注:指的是与document.all这个特性毫不相关的代码,这就是浏览器推断的表现).
如果IE以后假装不支持它,就可以`避免被识别出来`.另外一种可能是真的完全删除掉这个属性,但这样会破坏很多网站已有的代码.伪装不支持是一个很折衷的选择,浏览器开发商喜欢这么做.
但你必须记住,所有这些变化都发生在一个泄漏版的IE11中.很有可能是IE的开发者们想试验一下,看看这些变化执行之后的修复程度是否大于其破坏程度.我们不能过早的断定这些变化都会出现在最终版本的IE11中.
所有这些变化意味着什么呢.微软实际上在告诉全世界:"IE已经成长了,它是与标准兼容的,它能够支持网站上所有符合标准的代码,我们不再希望有专门为IE准备的代码".我比较倾向于相信这一点.IE10已经向前迈了一大步,IE11也许真的能够就标准实现上和其他浏览器进行竞争.
## ES6
在这个泄漏版的IE11中,发现了两个ECMAScript 6(下一代JavaScript)中的新特性:
### __proto__
在ECMAScript规范中,对象的原型用[[Prototype]]内部属性来表示,我们并没有方法直接操作这个属性.ES6通过对[`__proto__`](http://people.mozilla.org/~jorendorff/es6-draft.html#sec-B.3.1)`属性进行标准化`改变了这一点.IE11现在也支持了__proto__属性,和Firefox,WebKit以及Opera一起.__proto__
 属性目前是非标准的,且我认为是个非常丑的API.
### WeakMap
一个[`WeakMap`](http://people.mozilla.org/~jorendorff/es6-draft.html#sec-15.15)对象是一个由键值对组成的映射,其中每个键值对中的键必须是一个对象值,并且如果除了这个键以外,一旦没有其他的引用指向这个对象时,这个对象就会被垃圾回收器回收掉,当然,引用它的那个键值对也就不复存在了.正因为这种表现,所以`WeakMap对象中的键是不能被遍历的`.WeakMap的一个典型的使用情形就是用它来保存对DOM节点的引用,每当一个DOM节点从文档中删除之后,对应的DOM对象就会被回收掉,WeakMap对象引用它的那个键值对也会被自动删除,这样减少了对内存的占用.
`支持WeakMap的浏览器有`IE11,Firefox,以及Chrome(需要在chrome:flags页面中开启相关选项).
## WebGL
如果说存在一个IE反对者们可以用来声称"目前的IE仍然不属于现代浏览器"的理由,那么这个理由就是IE还不支持WebGL.不过我个人认为,对于大部分web开发者来说,WebGL并不是一个最重要的特性,因为它是一个很复杂的技术,通常的网站不需要3D图形的显示.
但在某些场景下,WebGL的确是个很关键的因素,比如说游戏.Mozilla和Epic最近宣布了[利用asm.js](http://blog.mozilla.org/blog/2013/03/27/mozilla-is-unlocking-the-power-of-the-web-as-a-platform-for-gaming/)[将Unreal
 3引擎移植到JavaScript](http://blog.mozilla.org/blog/2013/03/27/mozilla-is-unlocking-the-power-of-the-web-as-a-platform-for-gaming/)的消息,这也就证明了Web越来越有能力成为真正的游戏平台,以至于微软也迫不及待的实现相关技术.
在这个泄漏版的IE11中,已经能够使用WebGL了,不过使用前必须要先[导入一个注册表文件才行](http://withinwindows.com/within-windows/2013/3/30/blues-clues-how-to-enable-webgl-in-internet-explorer-11),毕竟还在开发阶段,不能直接开放.一开始人们发现它只支持IESL着色语言(基于DirectX),而不是符合标准的GLSL(基于OpenGL),后来才发现原来注册表中的一个值能够控制浏览器在这两者之间切换.
不过我们不能高兴的太早.直到最终的IE11正式版发布之前,我们都不能确定它是否真的会成为IE11的特性.我记得当初Opera中的WebGL就是默认关闭的,因为当时让Opera访问一个带有WebGL内容的页面,就会导致操作系统的蓝屏死机(我认为这是着色器导致的).Safari目前也是默认禁用了WebGL.不过,既然微软正在实现WebGL,未来的IE11不包含它的可能性是非常小的.
## 网络
有证据显示,这个泄漏版的IE11[已经支持了SDPY](http://withinwindows.com/within-windows/2013/4/1/blues-clues-internet-explorer-11-getting-spdy-support),虽然目前还不是全功能的.所以我们有理由猜测,该特性会出现在最终版本的IE11中.
SPDY是一个由Google提出的,基于HTTP的网络协议.它的主要目的是想通过降低网络延迟,压缩请求头,以及减少客户端的连接数来加速网页的加载.虽然目前SPDY还不是标准,但IETF已经用它作为了[HTTP
 2.0标准的](http://en.wikipedia.org/wiki/HTTP_2.0)基础.
SPDY目前已经被Opera,Firefox,以及Chrome支持.
## DOM和JavaScript API
### Mutation观察者
如果你想监视某些DOM变化,传统的做法就是去监听[Mutation事件](http://www.w3.org/TR/DOM-Level-2-Events/events.html#Events-eventgroupings-mutationevents).很多人都已经知道,Mutation事件是有设计缺陷的,因为它会导致严重的[性能问题](http://www.w3.org/2008/webapps/wiki/MutationReplacement#Background).正因为这个问题,Mutation事件现已被[废弃](http://dev.w3.org/2006/webapi/DOM-Level-3-Events/html/DOM3-Events.html?rev=1.100#events-mutationevents).
但是Mutation事件的功能是非常有用的,我们需要一个替代品,于是就有了DOM4中的[Mutation观察者](http://www.w3.org/TR/domcore/#mutation-observers).IE11实现了这一特性,和Chrome,Safari
 6,以及Firefox一起,Opera一旦切换到Blink内核,也自然会继承到这一特性.
### 全屏API
有时,我们希望能够隐藏掉浏览器的窗口部分,让网页占据整个屏幕来显示.最常见的使用案例应该就是在播放视频的时候了,游戏算是第二常见的案例.[全屏API](http://www.w3.org/TR/fullscreen/)能够让我们实现这一需求.
看起来IE11也要实现这一规范,因为这个泄漏版的IE11中已经有了`requestFullscreen方法,当然,是带有``ms前缀的,`剩余部分的规范应该会在接下来的版本中实现.另外,Opera也已经实现了这一规范中的所有API,且是不带前缀的.Safari,Chrome,以及Firefox中的实现是带前缀的.其中Firefox的当前实现是依据旧版规范草案的,Safari
 5.1和Chrome 15–19也是(参考[caniuse.com](http://caniuse.com/fullscreen)).
由于Metro IE(也许还有其他的名字)始终是全屏的,那么该特性也就主要在桌面环境中使用了.
## CSS
关于IE11新增了哪些CSS特性,并没有太多的新闻,也许是因为还处于开发初期,并且IE10已经实现了不少新的CSS特性的原因.其中有一个好消息是,IE11更新了FlexBox的实现到最新版的规范草案(很有可能是最终版).Flexbox语法在IE10正式发布的前两天发生了变化,所以导致IE10实现了一个过时的语法.我在编写SmashingBook 3中的FlexBox一节时,就被这个问题所困扰.目前只有Opera 12.1,Chrome(带前缀),Firefox 22实现了新版的FlexBox规范,Safari实现的仍然是旧版的规范.
## 接下来呢?
下面说说我希望接下来IE11能够实现的新特性.比如我希望能够支持CSS中的tr`ansform-style: preserve-3d,这样可以让`3D效果更加完善.还有希望能支持[CSS条件规则](http://www.w3.org/TR/css3-conditional/)中的`@supports,`曾经我在摩托罗拉工作时,通过那里的Webkit团队我推进了`@supports`特性在Webkit中的实现,现在不光WebKit,Firefox和Opera也都已经实现.另外,如果能支持`border-image也不错,`要是能支持WebM等其他的视频格式就更好了,不过这一点我不报太大希望.
因为ES6中的`WeakMap已经实现了`,所以以后实现更多的ES6特性也不会很奇怪了,尤其是考虑到现在[TypeScript](http://www.typescriptlang.org/)也包含了不少ES6中的东西.类似的,还有微软自己的[CU-RTC-Web](http://html5labs.interoperabilitybridges.com/prototypes/cu-rtc-web-roaming/cu-rtc-web-roaming/info)以及[设备方向](http://html5labs.interoperabilitybridges.com/prototypes/device-orientation-events/device-orientation-events/info)等特性的实现.我想我们会在过两天旧金山举办的[微软Build2013开发者大会](http://www.buildwindows.com/)上得知更多.
最后,感谢[François
 Remy](http://fremycompany.com/BG/2013/Internet-Explorer-11-rsquo-s-leaked-build-395/)和[Rafael Rivera](http://withinwindows.com/)发掘并公布出此泄漏版IE11中的这么多隐藏特性.
