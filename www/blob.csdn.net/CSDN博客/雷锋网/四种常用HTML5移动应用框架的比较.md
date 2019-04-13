
# 四种常用HTML5移动应用框架的比较 - 雷锋网 - CSDN博客


2012年09月07日 10:32:55[leiphone](https://me.csdn.net/leiphone)阅读数：1215


![](http://www.leiphone.com/wp-content/uploads/2012/08/html51.jpg)【编者按】本文译者范小虎，对于Mobile
 Web来说，现在是快速成长时代。由于采用了HTML5和CSS3技术，移动浏览器的性能加强了许多，同时，移动app的框架也扩展了，这意味着为移动设备创建丰富的互动的web体验的可行性又提升了。
本文通过对四种常用的框架进行比较，看看新技术带来了哪些改变。
采用诸如PhoneGap这样的封装软件，您就可以使用native app Store以及单个代码库，就可以分布式部署iPhone，iPad和Android等不同的目标平台了。
对于Mobile Web的开发人员来说，切换框架代价很高：因为动画的转换，工具栏，按钮，列表的显示，以及线下存储等都很麻烦。因为大部分上述功能都是新技术，以及这些领域的技术还在迅速地改变。作者玩转了许多Mobile Web的框架并且对它们进行了分析比较，下面将为您讲解他的研究发现。
## [jQTouch](http://www.jqtouch.com/)
jQTouch易用性强，相关文档也很全面。它的特色是在[使用HTML,CSS和JavaScript创建iPhone
 App](http://ofps.oreilly.com/titles/9780596805784/index.html)方面拥有出色的能力。jQTouch使用渐进增强的方案，在您相应的HTML顶层来实现像iPhone那样的用户体验。它简单易用，提供了一个基础的小工具集以及动画方案，开发人员只需要编程控制其动态行为即可。
不过在作者的简单测试中发现app的性能存在一定的问题，页面在转换时可能出现跳转或者缺失的情况，以及在响应tap事件的时候还有周期性延迟。该项目在技术上还活跃着，不过原作者的进展和部署都显得太慢了。
![](http://www.leiphone.com/wp-content/uploads/2012/09/jQTouch.jpg)
只需要遵守MIT的license许可就可以使用jQTouch了，MIT lic是作者最喜欢的开源许可之一。
## [jQueryMobile](http://jquerymobile.com/)
jQuery Mobile是这个领域的新丁，2010年8月才正式宣布成立，但是已经迅速进展到功能丰富的阿尔法2测试版本了。jQuery Mobile跟jQTouch相比很相似，但是更加标准，更有适应性，感觉很像jQTouch的后继版本，对用户接口和style的支持范围更加宽广了。
![](http://www.leiphone.com/wp-content/uploads/2012/09/jquery.jpg)
jQuery Mobile的性能是不稳定的，（虽然比jQTouch好一些）特别是在响应TAP事件的动画延迟补偿的时候。此外，还缺少一些关键的程序hook，所以不能轻松地让app更加具有动态性能。例如：当一个页面启动的时候事件触发了，这时候却无法通知响应的代码页面将转向哪个用户接口，也不能传递附加的信息给处理模块。针对上述问题，创建工作区来解决还是可行的，但是在这里作者希望其将来的版本能从jQTouch那里学习一下，并把现在的功能缺陷处理掉。
jQuery Mobile的相关文档资料很零散但是有所改进，作者很希望它们能变得像核心jQuery库那样具有鲁棒性。（请注意，jQuery Mobile是和jQuery UI相辅相成的，并不是建在单纯jQuery之上的）
想获得jQuery Mobile只需获得MIT或者GPL2 license。
## [Sencha
 Touch](http://www.sencha.com/)
这是个与Ext JS框架完全不同的产物，其方案与jQTouch/jQuery完全不同：Sencha生成自己的DOM（基于用JavaScript创建的对象）代替了先前存在的HTML增强方式。如此，使用Sencha工作的感觉不像是web编程，而更像是使用Java或者Flex等技术来做app的样子。比起jQuery来，Sencha的感觉更像是YUI。作者个人比较偏好渐进增强的方案，尽管其性能还真有些不尽人意的地方。
sencha跟其竞争对手们相比，扩展性强了很多：它拥有大量的用户接口组件，直接的iPad支持，拥有JSON和HTML5线下存储技术使得存储和数据绑定更加方便。（使用Sencha的数据结构来操作app的数据十分酷~它可以实时响应列表的更新）此外，Sencha还是唯一在工具栏上支持内嵌的对象支持，其他方式都是滚轮列表的样子。
![](http://www.leiphone.com/wp-content/uploads/2012/09/Sencha-Touch.png)
在作者的测试程序中，使用Sencha与jQTouch/jQuery相比，虽然app很明显地不那么轻量级，但是其性能和可靠性方面明显提高了，不过其初始化加载时间略慢。
当您使用库library或者框架frame进行开发的时候，不遵守框架或者用你自己的方式通常都不会获得成功。但Sencha的支持范围足够宽广，这意味着您可以使用Sencha的开发方式来实现任何需求。作者最开始用的是WebKit的内嵌SQLite数据库来做线下存储，但是最终还是因为其复杂性和各种bug问题的烦恼而放弃了，转而使用了Sencha数据存储的功能。
在文档方面，Sencha做的不太好，虽然很广泛，但是又有很多旧版本的老漏洞没有及时更新，作者就在这些框架中与bug作斗争，调试过程浪费了很多时间，因为文档不够健全，很多问题难以追踪或理解。而在开发者论坛响应作者提问的频率还算较高，不过最终感觉还是不太够。Sencha提供的付费技术支持起价是$300每年，作者很强烈地打算付费了，但是Sencha的回应是很好奇地打听为啥这么急着给他们送钱，真搞不懂。
获取Sencha需要遵守GPL3 license，以及在某种不是GPL标准又很相似LGPL的授权下也能用，以及遵循非商业license也可以获得。
## [TitaniumMobile](http://www.appcelerator.com/)
与Sencha Touch很相似，Appcelerator公司的Titanium Mobile可以让您使用Javascript API来编写app。不过与Sencha不同的是，Titanium把你的代码编译成Native的iPhone或Android app，这意味着它并不是一个真正的Web框架，而是一个兼容层或者编译器。（请注意Titanium Mobile的近亲Titanium Desktop是一个基于web的，让您可以使用HTML /js来编写桌面封装的本地应用的一款软件）
![](http://www.leiphone.com/wp-content/uploads/2012/09/titanium-mobile.jpg)
这么说来Titanium允许web开发人员使用JavaScript和一点点XML之类的其他相关技术，可以实现高性能、更换皮肤很方便的Native App，而不需要额外去学习Objective-C或者Cocoa Touch等技术了。作者的简单测试表明其性能不错，吹散了框架方面的疑云，而且整合起来也不是太难。
不过这个优点也是其致命的缺点，您只能作出Titanium所支持的平台上面的应用，你被它们的开发工具限制住了。作者想证明这一点只需要换一个不是iPhone的平台上来跑一下就知道了。同时，Titanium的调试器也不怎么样，不能使用XCode方式运行或者调试，就算在其仿真器上面程序跑的还算不错，还是需要作者自己去实际机器上自己再找问题。
**分析**
作者在这4个框架上面挑选了3种并编造了自己的app来试一试，过程虽然很冗繁，但是收获也颇丰。作者很喜欢jQTouch，但是不太相信它会在现有版本上再前进多少了。对于jQuery Mobile，很赞赏其简单易用性以及其以web为中心的开发方法，不过它的缺点是缺少核心特色，跟Sencha比性能差很多。
用一个阿尔法2版本的产品来跟一个1.0版本的正式版相比或许有失公平，但是在用户具有很强烈的刚性用户需求时就必须做出选择了，于是作者选择了Sencha Touch。作者最初被其强大的性能和宽广的支持程度所吸引，最终更喜欢其开发风格。随着开发的深入，其文档的漏洞让作者十分沮丧，但是其广泛的支持程度依旧吸引着作者，渐渐适应了其开发风格。如果他们愿意回复邮件的话，作者很有意向付费以获取技术支持。不过现在，Pints的发布已经是一个基于Sencha的app了。
**结论**
作者还没有回答最大的问题呢：一个基于web的app在没有本地app的情况下能否hold住局面？如果可以的话，实现这样的技术是否值得舍弃原来那种单一代码库方式所带来的代价呢？
鉴于两星期以来Pints的实际应用，作者倾向于说不。Pints在性能和bug方面陷入僵局，平均每隔10-15秒页面就乱跳，在滚动页面的时候容易乱跳，动画效果也不是很连贯。
VIA：[dzyngiri](http://www.dzyngiri.com/?p=752)
译文出处：WebAppTrend[几种常用HTML5移动应用框架的比较](http://www.webapptrend.com/2012/05/3009.html)

**（****[Warlial](http://www.leiphone.com/author/xiaolong)****供****雷锋网****专稿，转载请注明来自雷锋网及作者，并链回本页)**

