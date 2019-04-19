# 我泡在 GitHub 的 504 天 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [Sam Lin](http://www.jobbole.com/members/heray1990) 翻译。未经许可，禁止转载！
英文出处：[David Leonard](http://drksephy.github.io/2015/07/02/commit/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
（伯乐在线注：原文写于 2015 年 7 月）
在 Github 上怒刷代码提交 streak 是一件不可思议的事。 可能是出于打赌、琐碎事或者一股巨大的动力使得你去做这件事。受到《[jQuery之父：每天都写点代码](http://blog.jobbole.com/66227/)》这篇文章和我的好友 [Sahat](https://github.com/sahat) 的鼓舞，我开始每天都写代码，提升自己并使自己成为一名开发者和思考者。
![](http://ww3.sinaimg.cn/large/7cc829d3gw1f6nl3ym15fj20n0095wgz.jpg)
在开始这场[“连击”](https://github.com/DrkSephy)的时候，我只有很少自己名下的项目和有限的技能。过了一段时间之后，我很快燃起了新的激情，导致我建立了很多项目并且提升了自己的技能。我开始痴迷于构建交互式的工具和游戏，有时候会呆在某个地方一天，并花费 1-12 个小时去编写代码（有时候甚至待在某个地方好几天）。 在这篇博客中，我将会重点提到几个项目。这些项目塑造了我的未来，使我成为一名程序员并且把我带到我现在所处之高度。
### Water Emblem 水之纹章
在 2014 年 2 月 19 日，我和一位好友 [Kevin Chan](https://github.com/chessmasterhong) 一起建立了[Water Emblem（“水之纹章”）](https://github.com/chessmasterhong/WaterEmblem)这个项目。水之纹章是[火焰纹章烈火之剑](http://fireemblem.wikia.com/wiki/Fire_Emblem:_Rekka_no_Ken)的重制版，一款回合制策略角色扮演类游戏。玩家可以像玩国际象棋一样去控制一支军队。在开始的时候，我们不确定这款游戏将会往哪个方向发展。我们需要在短时间内完成[很多功能](https://github.com/chessmasterhong/WaterEmblem/blob/master/notes/task_tree.txt)，但是缺少开发这种复杂游戏的经验。火焰纹章是我梦想要重制的游戏，因此我很快专注起来，尽可能使之接近原作。时光飞逝，一晃眼就是几个月。
![](http://ww3.sinaimg.cn/mw690/7cc829d3gw1f6nlbsx5byg20go0b4e87.gif)
> 
译注：如果我是上面游戏截图的玩家，我更愿意用魔导师站在远距离攻击。另外，我可能不会让魔导师冲到这么前。^_^
在这期间，我没日没夜地为[游戏](https://github.com/chessmasterhong/WaterEmblem/pull/44)开发[新的](https://github.com/chessmasterhong/WaterEmblem/pull/141)[功能](https://github.com/chessmasterhong/WaterEmblem/pull/81)。有时我会连续 [12 到14 小时编写代码](https://github.com/chessmasterhong/WaterEmblem/graphs/punch-card)，只为开发下一个[大功能](https://github.com/chessmasterhong/WaterEmblem/pull/122)。这是我第一个感兴趣的项目， 也是第一个使我参与到编程方面的项目 。我把这个项目作为我在 Github 上“连击”开始的象征。我对知识的渴望源源不断，到了连续 6 个月的时候，我花费在编程和开发水之纹章、原型和解决漏洞的时间超过了 1,500 个小时。
开发完水之纹章之后，我继续学习使用 MongoDB、Express.js、Angular.js 和 Node.js 的[全栈 JavaScript](http://www.fullstackacademy.com/faq)。
### Technetium (Git)
[Git Technetium](https://github.com/DrkSephy/git-technetium) 是 [Technetium](http://technetium.herokuapp.com/) 的重制版，该项目最初是由我和 [Jorge Yau](https://github.com/codenameyau) 用 Django 创建的。为了学习 MEAN 堆栈，我在解决大项目之前编写了一个小的[应用程序原型](https://github.com/DrkSephy/angular-nhl)，从而理解后端 API 与动态前端相结合的概念。在那段时间，我阅读了各种技术博客，去理解在这个堆栈里数据是如何传输的，以及使用这个堆栈的优缺点。由于我们只是简单地传递 JSON，所以这种技术很适合我们项目。
![](http://ww1.sinaimg.cn/large/7cc829d3gw1f6nl3zlrf9j213h0gc0wa.jpg)
我做这个项目的动机是，想在学习这种新的、热门的 JavaScript 开发堆栈的时候，成功构建一个可运行的、有用的 Web 应用程序。最终我们建立了一个使用 Node.js、Express.js 和 Github 的 API 的 RESTful API，该 API 允许用户查询一些数据，例如，在一个仓库中每位开发者的代码提交量、问题的关闭/打开/已确认等状态、每位开发者贡献了多少行代码等。当执行一次查询时，会返回 JSON 到 Angular.js 客户端，然后该客户端会基于接收到的数据渲染模板。当这个应用还很简单、处于基础水平的时候，我学到了异步编程中的各种范式，运用 [async.js](https://github.com/caolan/async) 来避免 [callback hell](http://callbackhell.com/)（回调地狱），以及使用 [requests](https://github.com/request/request) 库执行节点的 web 请求。
2014 年的秋天，我开始攻读计算机科学硕士学位。当时我学了一门名为编译原理的课程，该课程引导我建立下一个项目。
### Pascal 编译器
正如课程的名字一样，编译原理课程要求学生针对自己选择的一种编程语言，编写该语言的编译器。由于我熟悉 Python，所以我用它实现[一个 Pascal 编译器](https://github.com/DrkSephy/pascal-compiler)。作为额外的挑战，我决定使用纯 Python 来实现这个编译器，即不使用外部的库来开发词法分析器和解析器。
![](http://ww3.sinaimg.cn/mw690/7cc829d3gw1f6nlbud206g20go0b4trn.gif)
在课程结束的时候，我学会了如何开发自己的语法来构建单遍解析器，从头开始构建一个扫描器和词法分析器，生成可执行的机器代码，以及编写一个模拟器来执行这些定制的汇编指令。我的编译器能够处理包含数组、循环、所有控制语句和轻量性能优化等复杂的程序。
在编写编译器的时候，我也开始了[美国国家海洋和大气管理局](http://www.noaa.gov/)（NOAA）的研究生科研工作。
### 研究生科研
作为一名研究生新生，我找到了一个与我热爱的 Web 技术有联系的研究项目。这个新的项目包含处理各种各样的微粒（本质上是 HDF 或 NetCDF4 格式的大数据集）和构建交互式可视化界面，它可以显示像海洋表面温度或者将鼠标悬停在一张世界地图影像上的结果误差等数据。
![](http://ww2.sinaimg.cn/large/7cc829d3gw1f6nlbw351xg20xc0m8u0x.gif)
为了构建这些[可视化界面](https://github.com/DrkSephy/NOAA-Projects)，我使用强大的 [D3](https://d3js.org/) 库来创建 [Voronoi 图](https://en.wikipedia.org/wiki/Voronoi_diagram)（该图包含一张所有微粒中心的 JSON 列表）。通过把鼠标悬停事件附加到每个单元上，用户可以将鼠标停在任意区域，然后马上可以看到已处理微粒的缩略图。我在这个项目中学会使用 Python 科学库（SciPy、Matplotlib 和 Numpy），用于地图投影的 [Basemap](http://matplotlib.org/basemap/) 和用于隔离容器的 Docker。
### Django Hackathon Starter
[Django Hackathon Starter](https://github.com/DrkSephy/django-hackathon-starter) 是我的第一个真正开源的项目。目的是为了给 Django 开发者一个样本应用程序，它提供了各种各样的社交登录例子（Facebook、Twitter、Tumblr、Instagram、Dropbox 等）和 API 例子（Yelp、Twitter、Github 等）。
这个项目是在 2015 年春季，我的硕士课程中的《Advanced Topics in Web Programming》课程里面开始的。它鼓励我带领一个团队，朝着一个目标（从头开始构建一个开源项目并易于开发者使用）奋斗。
虽然这个项目还远远没有完成，但是它是一次很好的学习经验。并且随着时间的推移，我们将会不断完善它（非常欢迎开源爱好者为这个项目添砖加瓦）。
### Tales of Kratos
[Tales of Kratos](https://github.com/DrkSephy/Tales-of-Kratos) 是我和 [Lisa Maldonado](https://github.com/venegu) 在 2015 年春季时开发的一款横轴滚动 2D 角色扮演类游戏，在纽约市立学院的 Computing Machinery Indie Software Competition 协会中取得第一名。它以经典的横轴滚动为特色，同时具有快速的、沉浸式的和令人入迷的游戏玩法，拥有号称塞尔达传说式的游戏机制和一套深度技能发展系统。
![](http://ww1.sinaimg.cn/large/7cc829d3gw1f6nlc3cac4g20go0b44r2.gif)
### 未来的展望
截至今天，我的 streak 依然在增加。这并不意味着我是一名编程专家，但是我的自信心已经得到提高，我对知识的追求已经扩大，我的志向也变得远大。因此，我[举办](http://slides.com/drksephy/deck-)了各种关于[技术](http://slides.com/drksephy/deck-1)和 [JavaScript 游戏开发](http://slides.com/drksephy/deck-2)的[演讲](http://slides.com/drksephy/deck)，也写了很多[教程](http://drksephy.github.io/tutorials/)。
在这段时间，我激发了对一切关于 JavaScript 的事物的激情，涉足了[JavaScript 的最新更改](https://github.com/addyosmani/es6-equivalents-in-es5)和[技术](https://github.com/facebook/react-native)，同时也对很多开源项目作出贡献。我敢说，对于我自己，Github 比 Facebook 更加令我着迷。
目前，我在加利福尼亚州桑尼维尔市的 [Yahoo! Developer Network](https://developer.yahoo.com/) 团队中工作，我在这里除了使用 [Node.js](https://nodejs.org/en/)、[Express.js](http://expressjs.com/)、[React.js](https://facebook.github.io/react/) 和 [ES6 JavaScript](https://github.com/lukehoban/es6features) 开发一款度量面板以及我们用到的测试工具包（Karma、Mocha、Chai、Webpack、[Protractor](http://www.protractortest.org/#/) 和 Babel）之外，还制作了 Swift [教程](https://developer.yahoo.com/flurry/docs/analytics/gettingstarted/swift/)、[视频](https://www.youtube.com/watch?v=UEYmjJgD-JI)和应用。
如果没有 streak 极大地鼓舞着我开始疯狂的学习之旅，上述的这些项目都不会实现。我的野心比以往任何时候都大，而我也开始研究[设计 NES 模拟器](https://medium.com/@fogleman/i-made-an-nes-emulator-here-s-what-i-learned-about-the-original-nintendo-2e078c9b28fe)，为我的下一个大项目做准备——使用 JavaScript 构建我自己的模拟器。对于 streak，我不确定什么时候会结束，但是我确定我会继续不断学习、不断进步。
> 
**打赏支持我翻译更多好文章，谢谢！**
[打赏译者](#rewardbox)
#### 打赏支持我翻译更多好文章，谢谢！
![](http://www.jobbole.com/wp-content/uploads/2016/04/2d041661f5c5720aed6862fa91af766d.png)
