# 可能是迄今为止最好的 GitHub 代码浏览插件 - 文章 - 伯乐在线
原文出处： [geeksense](http://www.geeksense.cn/plugin/)
【伯乐在线转注】：以前曾介绍过一个 Chrome 插件 [Octotree](http://t.cn/RPZJZfD)。安装之后，浏览托管在 Github上的项目，可看到左侧的树形结构，更方便查看代码。今天这篇文章介绍另外一个同类插接。
GitHub是很多开发者远(tong)程(xing)协(jiao)作(you)的重要工具，其社交化编码的理念伴随着开源运动改变着整个开发社区的生态，无数优质项目依托GitHub在全球开源开发者的参与下蓬勃发展。
**不过，讲真，你有在GitHub上好好读过代码吗？**
对于大部分人来说恐怕是没有，GitHub作为一款基于Git的代码管理工具和协同工具是很优秀的，然而作为代码浏览和搜索工具，就和IDE差的很远了。不然为什么到今天GitHub连个侧栏的文件树没有，很多开发者要专门去下载浏览器插件才能有文件树？为什么你还是需要git clone到本地在把代码装到你的IDE里才能愉快的阅读代码？是不是有想过如果能像IDE那样在GitHub上浏览代码多好？
今天我们就来介绍一款Chrome浏览器插件Insight.io，让你能像在GitHub上像你的IDE里一样浏览和搜索代码，让你的Github就像穿上了钢铁侠的战衣。
![](http://wx2.sinaimg.cn/large/7cc829d3gy1fhzg5vidjjg20dq094wp8.gif)
## 功能总览
[下载安装](https://chrome.google.com/webstore/detail/insightio/pmhfgjjhhomfplgmbalncpcohgeijonh)后，打开一个GitHub上的项目，会看到左边有个侧边栏，是不是已经有了自己在IDE里面的感觉了？插件提供以下功能
- **跳转到定义**
- **查找引用**
- **目录树功能**
- **基于符号的代码搜索**
![Glance](http://wx3.sinaimg.cn/large/7cc829d3gy1fhzg5waddog21410osb29.gif)
## 交叉引用
用[Apache Spark](https://github.com/apache/spark)做例子，悬浮在某个节点(类，方法，变量等)上就可以看到相应的类型和文档说明。点击左边的结构(Structure)按钮就可以看到改文件的类结构，包括成员变量，成员函数等，点击也可以跳到相应的行。
![Cross Reference](http://wx3.sinaimg.cn/large/7cc829d3gy1fhzg5y5wqwg21400ot1l0.gif)
## 查找引用
点击任何节点的用例就能跳到这个类，方法或者变量定义的地方（类似于IDE中的Ctrl + click）。
点击类，方法和变量的定义节点，则可以找出所有该节点的使用例子。其中使用例子又会以被引用(Referenced At), 继承 (Inherit), 被继承(Inherited By), 重写(Override)等类型进一步分类方便查找。如果结果太多，用例的右上方也会有支持regular expression的文本框可以用来过滤出感兴趣的文件。
![Find References](http://wx3.sinaimg.cn/large/7cc829d3gy1fhzg5zayzeg21650q4kjn.gif)
## 目录树
点击文件(File)按钮你会看到树状的目录结构，忘记GitHub上切换文件的痛苦吧，文件树让你轻松的在项目之内的文件中自如切换。
对于私有项目需要点击下方的**Sign In**按钮并到insight.io上导入私有项目文件树才会生效。
![Directory Tree](http://wx4.sinaimg.cn/large/7cc829d3gy1fhzg60wyodg21420othdv.gif)
## 代码搜索
GitHub只支持纯文本搜索，这时常让搜索代码变得很困难，特别是当你想搜的关键词是常用词的时候，大量的无用结果会让搜索功能完全不可用。而是用Insight.io插件，点击搜索(Search)按钮可以按类名，方法名，变量名，文件名等等搜索代码，并且有自动补全功能。试试在Insight.io和GitHub原生搜索上搜一下RDD.map对比下搜索结果吧？
![Code Search](http://wx2.sinaimg.cn/large/7cc829d3gy1fhzg62hd81g21660q44qr.gif)
## 跳转到Insight.io
你也可以一键点击跳转到Insight.io上获得更好的浏览体验。
![View on insight.io](http://wx2.sinaimg.cn/large/7cc829d3gy1fhzg645xmfg21410oru0y.gif)
## 提交项目给Insight.io
Insight.io目前收录了2000多个最活跃的开源项目，包括[Apache Kafka](https://github.com/apache/kafka), [Linux-kernel](https://github.com/torvalds/linux)这样的大型项目，也有像[AndroidSwipeLayout](https://github.com/daimajia/AndroidSwipeLayout)这样优质的个人项目。如果你需要的项目并未被我们收录，点击**Information**栏下的**Request Analyze**就可以提交你目前正在浏览的项目了。
![Request Analyze](http://wx3.sinaimg.cn/large/7cc829d3gy1fhzg65lh83g213y0oru0x.gif)
## 快捷键
用快捷键可以快速打开关闭插件功能:
- **文件树**: `⌘+⇧+f`
- **类结构**: `⌘+⇧+o`
- **信息**: `⌘+⇧+i`
- **搜索**: `⌘+⇧+s`
[插件下载传送门](https://chrome.google.com/webstore/detail/insightio-for-github/pmhfgjjhhomfplgmbalncpcohgeijonh/)
目前插件支持的语言有Java, C/C++, Scala, Python和Ruby。想跟开发团队吐个槽或者点个赞？想知道最好的编程语言PHP什么时候支持？想知道怎么让公司代码库也能用上Insight.io？欢迎邮件到 support@insight.io 。想实时吐槽的也可以加入我们的gitter房间
![Gitter chat](https://badges.gitter.im/Insight-io/Lobby.png)
