# Drupal网站系统的应用 - 三少GG - CSDN博客
2011年02月07日 00:58:00[三少GG](https://me.csdn.net/scut1135)阅读数：2101

# Drupal网站管理系统
[http://www.intematrix.cn/book/export/html/74](http://www.intematrix.cn/book/export/html/74)
Drupal，全球最流行的开源网站的内容管理(CMS)和发布系统，它是用PHP写成的。主要用于构造提供多种功能和服务的动态网站，这些功能包括用户管理(UserAdministration)、发布工作流(Publishing Workflow)、讨论、新闻聚合(NewsAggregation)、元数据(Metadata)操作和用于内容共享的XML发布。它综合了强大并可自由配置的功能，能支持从个人博客(PersonalWeblog)到大型社区驱动(Community-Driven)的网站等各种不同应用的网站项目。Drupal最初是由DriesBuytaert所开发的一套社群讨论软件。之后，由于它的灵活的架构，方便的扩展等特性，使得世界上成千上万个程序员加入了Drupal的开发与应用中。
Drupal是基于GPL("GNU General Public License")协议发布的一个开源软件平台，由一个拥有数以万计的成员和开发者组成的社区维护。从最开始作为社群讨论软件，到之后演变成广受欢迎的内容管理系统（CMS），到现在进化成内容管理框架、网络开发平台，Drupal在世界范围内广受赞誉，并多次并评为最优秀的开源系统和CMS系统。
- 1－－连续2年全球开源CMS排名第一
- 800－－强大的开发团队，全球有800多活跃的开发者在驱动Drupal不断改进和发展
- 400,000－－全球Drupal网站超过40万
Drupal采用模块化设计，功能特性和设计样式都可灵活配置。这些强大的特性使得很多大型机构都采用Drupal的框架，如美国白宫网站（Whitehouse.gov）、法国政府站点(jeunes.gouv.fr)、福布斯新闻杂志站(goingon.com)、火狐网上社区站（spreadfirefox.com）、国际著名娱乐公司：华纳兄弟（warnerbrosrecords.com），索尼/BMG(myplay.com)等。
# Drupal系统的功能特性
Drupal 是一个优秀的模块化结构内容管理系统，通过不同模块来实现网站所需功能。Drupal 的核心模块是Drupal 最重要的组成部分，他们是Drupal 主要功能的承载。Drupal 自带有33个模块，基本上涵盖了当前网站所应具有的全部功能：用户管理、博客、论坛、评论、相册以及日志管理的，还有新闻聚合等web 2.0特性的功能。这些模块都是Drupal 的下载包中所附带的。有些被开启，有些没有被开启，当需要使用一个功能的时候，先看看核心的模块是否可以满足哟实现的功能，若没有就去下载第三方模块，它们都是经过很好的测试、符合各种编码并兼容主流浏览器；若有，那就优先使用核心模块。
###### 网站功能特性列表
|![textphoto](http://www.intematrix.cn/sites/default/files/huiheng/textphoto_0.png)文本和图片 — 功能强大的后台编辑器不但可以让企业商家按照自己的意愿任意设置信息栏目、板块，还可以实现图文混编、影音混编、支持FLASH等多种形势的图片、表格、连接，设计制作出个性化的宣传网页、企业网站。|![留言板](http://www.intematrix.cn/sites/default/files/huiheng/olinefd.png)**留言板 **— 提供了一个公共的信息发布平台，特别适用于作为企业内部个人办公助手以及企业与企业之间进行信息交流；在Internet上储存留言资料，方便查阅。使得随时随地查询信息的移动办公成为可能。|
|----|----|
|![论坛](http://www.intematrix.cn/sites/default/files/huiheng/13_0.png)**论坛 **— 是网站中信息多、人气旺的地方，好的BBS可以吸引相当数量的访客，同时也担负着网站对外宣传、发布消息、收集客户反馈的重任，是网站、单位内联网必不可少的一部分。|![博客](http://www.intematrix.cn/sites/default/files/huiheng/blog.png)**博客 **— 创建属于自己的博客或企业博客，树立良好形象。|
|![新闻](http://www.intematrix.cn/sites/default/files/huiheng/news_0.png)**新闻 **— 自主发布公司新闻、动态，让客户了解公司，彰显企业大气形象。|![评论](http://www.intematrix.cn/sites/default/files/huiheng/pinglun_0.png)**评论 **— 评论—及时了解用户需求，帮助用户做出一些购买决策，挖掘潜在客户。|
|![邮件](http://www.intematrix.cn/sites/default/files/huiheng/email_0.png)**Email订阅 **— 快捷、简单。只需要一个邮件地址，就可获得企业最新动态。|![投票](http://www.intematrix.cn/sites/default/files/huiheng/vote_0.png)**投票 **— 发起意见投票栏，快速了解用户体验信息；星级标志评分功能，让客户更放心。|
|![标签云](http://www.intematrix.cn/sites/default/files/huiheng/tab_0.png)**标签云 **— 显示站内热门搜索关键词，让用户可以仔细了解站点内容。|![rss](http://www.intematrix.cn/sites/default/files/huiheng/textphoto_0.png)**RSS feed **— 让您的用户更容易获取所需信息，让你的产品更易于推广。|
|![产品展示](http://www.intematrix.cn/sites/default/files/huiheng/show_0.png)**产品展示 **— 绚丽的视觉效果，清晰的产品描述，给您的客户带来全新的感觉。|![视频语音](http://www.intematrix.cn/sites/default/files/huiheng/video_0.png)**视频语音 **— 为网站添加flash、视频、音乐等。|
|![数据备份](http://www.intematrix.cn/sites/default/files/huiheng/backup_2.png)**数据备份 **— 轻松方便地备份站点数据，及时恢复网站意外事件。|![站点统计](http://www.intematrix.cn/sites/default/files/huiheng/textphoto_0.png)**站点统计 **— 在这里可以看到网站流量及有多少人浏览过你的站点。|



# Drupal系统核心模块
Drupal 是一个优秀的模块化结构内容管理系统，通过不同模块来实现网站所需功能。Drupal 的核心模块是Drupal 最重要的组成部分，他们是Drupal 主要功能的承载。
**网站核心模块列表：**
|![聚合器模块](http://www.intematrix.cn/sites/default/files/huiheng/rss_1.png)**聚合器模块 **— 为站点提供强大的新闻聚合与阅读功能，能自动从指定的任意新闻或是从其他网站收集相关内容，并聚合显示到站点。|![Acchive模块](http://www.intematrix.cn/sites/default/files/huiheng/acttive.png)**Acchive模块 **— 按时间查看存档内容，可以查看指定日期发布的内容。|
|----|----|
|![区块](http://www.intematrix.cn/sites/default/files/huiheng/blocks.png)**区块 **— 用来控制除正文内容之外的其他边栏显示内容。|![过滤器](http://www.intematrix.cn/sites/default/files/huiheng/lu.png)过滤器 — 管理员可以设置站点的文本输入格式。|
|![帮助模块](http://www.intematrix.cn/sites/default/files/huiheng/help.png)**帮助模块 **— 显示上下文相关的信息，用户如果不能明白模块的相关功能，可以点击帮助模块提供的链接，更快的了解模块的功能。|![本地化模块](http://www.intematrix.cn/sites/default/files/huiheng/local.png)**本地化模块 **— 允许用户用本土语言来表现drupal站点，而不是系统默认的英语，可以用来建立一个多语言站点或是替换内置文本达到定制站点的目的。|
|![菜单模块](http://www.intematrix.cn/sites/default/files/huiheng/memu.png)**菜单模块 **— 轻松方便地备份站点数据，及时恢复网站意外事件。|![用户模块](http://www.intematrix.cn/sites/default/files/huiheng/user.png)**用户模块 **— 允许用户注册，退出，登录。|
|![上传模块](http://www.intematrix.cn/sites/default/files/huiheng/upload.png)**上传模块 **— 允许用户上传文件到站点。|![搜索模块](http://www.intematrix.cn/sites/default/files/huiheng/research.png)**搜索模块 **— 能够通过关键字来搜索内容。|
|![Profile模块](http://www.intematrix.cn/sites/default/files/huiheng/shi.png)**Profile模块 **— 允许站点管理员在用户个人信息中自定义字段，例如国籍、姓名等。|![拥堵控制模块](http://www.intematrix.cn/sites/default/files/huiheng/ko.png)**拥堵控制模块 **— 可以自动探测访问流量波动的平衡机制。|
|![跟踪模块](http://www.intematrix.cn/sites/default/files/huiheng/gz.png)**跟踪模块 **— 在网站上允许用户看到最新添加或者是更新守的内容。|![watchdog模块](http://www.intematrix.cn/sites/default/files/huiheng/wat.png)**搜索模块 **— 用来监测站点，捕捉系统事件，记入日志文件，供授权使用者查看。|



# Drupal系统的第三方模块
Drupal第三方模块是用户在核心模块的基础上，根据自己的需要所开发出来的模块。
第三方模块根据功能分为以下几类：
###### 1. 站点安全类：主要是增强站点安全设置的模块。
下面几个模块用于站点安全设置
（1）Captcha
图灵测试/字符校验模块，防止机器人自动注册。
（2）Challege-Response Authentication：(4.5.0)
查问/应答校验模块。帮助防范relay attacks，提高系统安全。
（3）LDAP integration
支持LDAP（Lightweight Directory Access Protocol）。
（4） Legal：例SQL
在用户注册时加入“我同意条款”的过程。
（5）Spam：SQL
垃圾信息过滤，使用贝叶斯过滤器自动统计识别垃圾信息。
###### 2. 基本站点管理功能：站点基本功能的升级，如分配用户权根，搜索功能等。
（1）Admin block：例adminblock截屏
用于帮助管理员方便的监管评论和文章。
（2）Automember：(4.5.0)
自动用户权限管理。管理员可以设定不同的标准，自动分配给用户不同的权限。
（3）Comment closer：
可以自动关闭对老文章的评论，设置时间后通过cron来自动执行。
（4）CustomError：
自定义错误显示页面（404-页面未找到，403-访问拒绝）。
###### 3. 文本字体的输入格式：包括各种所见即所得的文本编辑器。
（1）Attached node：(4.5.0) 例
开启该模块并将attached node加入输入法过滤器后，将可以在发表文章时使用 [node: node-id] 的格式来引用另一节点的内容
（2）Bbcode：(4.5.0)例
支持BBCode输入格式
（3）Contextlinks：例
用户在输入链接时，可以赋予链接以不同的类型，此链接将按照预先设定的方式显示。
（4）HTML corrector
自动检测用户输入的HTML语法，是否每个标签都有对应的结束标志以及嵌套是否正确
###### 4. 文件图片管理：所有涉及文件上传，图片显示与管理等功能的模块。
（1）Album：(4.6.0)
影集模块。需要先安装folksonomy和image模块。
（2）Attachement：
支持上传附件，可以用来替代Drupal自带的upload模块。
（3）Filestore2
允许用户上传/下载文件，文件存储在文件系统中而不在数据库里。
（4）Gallery
用于在线像册管理和展示。
###### 5. 节点内容功能增强：对单个节点内容特性管理功能的增强。
（1）Diff
增强了drupal自带的‘修订(revision)’功能，可以方便的显示出不同版本内容见的不同。
（2）Event：
跟踪事件并通知用户。
（3）Pathauto
自动为节点，类别，用户等生成路径别名。
（4）Scheduler：
事先规划节点内容发布或撤销的时间表，到时自动执行。需要通过cron运行。
###### 6. 内容规则设定与站点导航：用于按照一定规则来显示多个节点内容。
（1）Article
将任意类型的节点根据类别（taxonomy）整理在一起集中显示。
（2）Collimator
一般blog的文章都是垂直显示，此模块可以将节点按最近更新时间排序后从左到右平行分列显示。
（3）Jsdomenu：
本模块是基于开源Jsdomenu库开发的，用于生成多层弹出式菜单(popup menu)，自带一个“站点菜单(site menu)”区块用于菜单式浏览站点的分类结构。
（4）Live Discussion
通过区块显示最新评论。
###### 7. 定义节点类型：定制专属的节点类型，方便特定内容的发布。
（1）Book Review
定义了特殊的书评节点，方便发布图书评论。
（2）Flexinode：
可以用此模块根据需要生成新的自定义节点(node)类型。
（3）Daily：
此模块定义了两种新的节点类型：daily和daily container。
###### 8. 个性化设置与私人信息：允许用户进行个人喜好的设定。
（1）Blog Theme：
允许用户选择自己blog的外观主题。
（2）Buddylist
用户可以编辑自己的好友名单，同时显示‘好友的blog’区块。
（3）Contact directory：
使用户可以添加自己的好友联络名单。
（4）Workspace：
提供一个用户专有的工作区，可以集中察看管理自己的文章。
###### 9. 社区交流：支持用户与管理员以及用户之间的信息交流。
（1）Chatbox：
简单的聊天框模块。
（2）Feedbak：
反馈(feedback)模块，可用于生成反馈表格收集用户意见或问题，用户填表后将通过email发送给指定地址。
（3）Project：
一个简易的项目管理模块。项目组成员可以由此跟踪项目的进展，可以通过email发送项目更新通知。
（4）Webform：
创建‘网络表单(webform)’节点类型，可以用于提交调查问卷，投票，注册等。
###### 10. 新闻聚合与回引等
（1） Blogroll：(4.5.0)
提供Blogroll功能，每个用户的blog可以有自己的blogroll。
(2)Syndication：
集中显示所有站内RSS feeds。
###### 11. 会员广告电子商务
（1） E-Commerce：
一整套电子商务工具。此外还包括拍卖/订阅/捐款等功能。
（2）Banner：
自动轮换显示广告条。
（3）Adsense：
显示Google Adsense广告。可以方便的定制显示方式，支持从banner到link等6种显示格式。
（4）Amazon associate tools：
Amazon 广告工具。定义了两种新的节点类型：
（1）“amazon”节点：用于发布书评介绍等。
（2）“amazon-node”节点：包含相关的amazon产品信息/书号/价格/图片等
# Drupal系统的后台管理及操作-内容管理
###### RSS发布
RSS发布设置是drupal6中增加的新功能，可以设置诸如首页图标，点击以后出现列表的数量和显示的方式。
导航“内容管理”---“RSS发布”；如下图:
![rss发布](http://www.intematrix.cn/sites/default/files/huiheng/rss_0.png)
第一个选项，则是选择点击rss图标以后，下面列出项目的数量，默认为10
第二个选项，则是选择列表文章显示的方式，默认为“标题和摘要”
###### 内容管理
是查看、编辑、删除你的站点的所有内容，站点管理员可以在这里对你站点的内容进行管理。
在内容管理的页面中，大体分为3个部分，上面部分是用来“筛选”文章的，中间部分是对文章的操作，包括“删除”，“置顶”，“发布”等操作，第三部分则是下面的文章列表，提供对文章的预览和编辑的功能；
（1）如何过滤文章
在最上面的下拉列表框中勾选相应得选项后，点击右边的“过滤器”即可。
（2）如何对文章进行批量操作
勾选需要操作的文章，在中间的更新选项中，选择相应的操作选项即可进行批量操作。
发表：即发布文章，匿名用户可以浏览；不发表的文章，只有有文章管理的用户可以查看。
撤销：不发表
推荐到首页：在首页展示。
从首页撤下：不在首页展示。
置顶：固定在首页最前面显示。
取消置顶：撤销文章的置顶。
（3）编辑修改文章
点击文章列表后面的“编辑”按钮，即可进入编辑状态。
###### 内容类型
内容类型，默认的内容这几种：blog、story、page等等,针对这些内容类型的编辑、删除都在内容类型管理类型当中。
导航到“管理”--“内容管理”--“内容类型”；看到如下图，我开启了所有核心模块:
![内容类型](http://www.intematrix.cn/sites/default/files/huiheng/content.png)
###### 发布设置
发布设置:是用来设置文章发布时的一些配置选项,例如：
导航“内容管理”--“发布设置
![发布设置](http://www.intematrix.cn/sites/default/files/huiheng/fb.png)
（1）第一个选项：是设置文章首页超过设置以后，会自动分页。
（2）摘要长度：没有设定摘要时，系统会自动截取一部分文字
（3）预览文章：选择发布文章时，预览是否是必须的
分类
评论
# Drupal系统的后台管理及操作－站点构建
站点构建：主要负责Drupal的基本要素(如区块、菜单、模块等)的管理，同时drupal的功能是通过各种模块实现的，故在开启模块时，也会在站点构建菜单中增加管理链接。
###### 1. 区块
在区块管理部分，你可以配置开启哪些区块、可以设置如何在页面上布置；
Drupal 对区块是这样定义的：区块是一个内容单元，可以放置在页面中的不同区域（Region）。网站中的页面通常有一种布局方式，称为网页布局（Layout）。每一个主题一般有一个统一的网页总局。如Drupal 的默认主题将页面分为：顶端、左侧栏、中心内容、右侧栏和部几个区块。那么在这些区域内就可以放置你所想要摆置的区块。
（1）查看区块列表
导航到“管理”--“站点创建”--“区块”如下图：
![区块](http://www.intematrix.cn/sites/default/files/huiheng/bl_0.jpg)
在这个页面中可以看到页面上有很多用虚线描绘的块，这些就是区块可以被放置的位置，被称为区域（Region）。在页面的中央的区块列表上方是站点所拥有的主题列表。不同的主题可能会有不同的区块列表。请注意，在此处进行的区块配置，只针对于所选的主题。如果更换了主题，那么就需要进行不同的区块设置。
区块列表主要包含下列字段：区块名称、所在区域、权重及操作。下面将分别介绍其功用：
1、区块名称：用来标识区块的名称。如“Navigation”，就表示“导航”区块。
2、所在区域：此处有一个下拉框，列出了有效的所有区域。当前值就是此区块当前所在的位置。
3、权重：选择要放置的区域后，可以设定每个区块的权重。权重越小，越排在前面。
4、配置链接：单击“配置（configure）”即可进入区块配置页。
（2）添加区块
区块描述： 填写区块名称，将在区块的列表页面显示此名称
区块名称：此名称用于在前台显示区块时显示的名称
区块正文：即区块中的内容，此方法经常用于填写一些静态的内容，比如“版权声明”“友情链接”等。
###### 2. 菜单
菜单是Drupal 中用于导航的链接的统称，它分布在每个页面上，便于用户快速的链接到需要的页面中。网站中所有的内容的展示也主要通过菜单。Drupal6重写了菜单系统，更为强大，可定制性也更加好。
导航“管理”--“站点创建”--“菜单”，将列出现有的菜单项目，默认有主菜单、二级菜单和导航菜单。
（ 1）导航菜单
点击“Navigation”，进入主导航菜单的配置
（2）主菜单（Primary links）
点击“Primary links”，进入主菜单的列表页面，这里显示的是主题最上方的导航菜单。
（3）次链接（Secondary links）
点击“Secondary links”，进入主菜单的列表页面，这里显示的是主题最上方的导航菜单。
###### 3. 模块
模块管理部分可以决定开启和关闭哪些模块。
（1）安装模块
导航到“管理”--“站点创建”--“模块”，将列出Drupal站点现有的所有模块，勾选的是现在正在使用的模块，灰色的代表被其他模块所“依赖”，无法关掉，而没有勾选的证明还没有被启用了
（2）关闭和卸载模块
卸载模块之前要将其关闭，方式和开启时一样，将其前面的勾勾去掉，点击保存就可以；点击页面上面的“卸载”，勾选要卸载的模块名称，点击“卸载”即可。
###### 4. 主题
Drupal的风格是基于主题的，在主题管理当中，可以更换主题和对主题进行相应的配置。
导航“管理”--“站点创建”--“主题”中，会看到您现在Drupal中安装的所有的主题列表。
  (１)折叠显示（Toggle Display）。这部分主要是页面上的各个元素的开关，即决定这些页面元素显示与否。这部分控制的元素主要有：
1、站点图标（Logo）：一般在页面上方显示的站点的图标。
2、站点名称（Site name）：站点图标旁的大字体的文字即为站点名称。
3、宣传语（Site slogan）：就是站点的宣传语。
4、任务声明（Mission statement）：用于展示网站主旨的言语字。
5、浏览器地址栏里的站点图标（Shortcut icon）。
（２）文章相关信息显示（Display post information on）
（３）站点图像设置（Logo image setting）
（４）自定义浏览器地址栏中的站点图标（favicon）
###### 5. URL别名
URL 别名是设置Drupal 站点内部路径的别名，比如把node/11 设置为aboutus，方便其设置和SEO。
# Drupal系统的后台管理及操作－站点设置
站点设置：主要用来站点的基本信息设置方面，如站点的名称、管理页面专用主题的设置等
由于站点设置的基本设置较多，所以只选几个常用到的设置来做具体介绍：
###### 1. 图像工具包
这个比较简单，用来设置上传图片的压缩质量。导航“管理”--“站点配置”--“图像工具包”，此功能需要开启GD库支持。一般选择默认即可，不用更改。
###### 2. 性能
导航“管理”--“站点配置”--“性能”（或者使用acquia的快捷工具），进入配置页面；
（1）页面缓存，是指Drupal 会采用静态页面的方式代替动态页面。这样能明显的提高网站的性能和浏览速度。
（2）带宽优化（Bandwidth optimizations）
带宽优化，是指通过减少页面请求的大小与数量，以达到减轻服务器负荷、减少带宽使用并加快平均页面响应速度的目的。
（3）区块缓存
Drupal6中新增加了对区块的缓存，Drupal5中没有这个配置选项的。区块的缓存，原理应该和页面一样。当我修改一个区块时，匿名用户同样看不到变化.
（4） 清空缓存
Drupal6中新增加的功能，点击清空缓存的话，相当于清空了 cache表中的数据，包括可以清空views的缓存，修改了主题info文件，即得清空一下缓存，此功能相当于Devel模块的清空缓存功能。
###### 3. 文件上传
导航“管理”--“站点创建”--“文件上传”
这里用来对Drupal上传文件时的设置，可以设置允许的最大分辨率，文件类型等，也可以对一次上传大小作限制。
###### 4. 站点信息（Site information）
站点信息部分，主要可以更改一些关于站点的基本信息方面的配置。这个部分的配置比较简单，主要有以下几个部分。导航“管理”--“站点配置”--“站点信息”
（1） 基本站点信息
基本站点信息主要包括以下几个内容
网站名称（Name）：全站的名称，一个网站必须有一个合适而有特色的名字，才容易被人们记住。这里可填入如“我的网站”等文字。当然，这不是一个好的名字。
网站邮件地址（E-mail address）：这个邮件地址，主要用于当有新用户在网站上进行注册时，系统将会自动发送一封确认信至刚注册的用户的邮箱。
口号（Slogan）：此处填入网站的宣传语，属选填字段。填写后，这段话将出现在页面头部或标题部分，这是由主题模板决定的。
底部信息（Footer Message）：将显示在页面下方的文字。通常是版权信息。
（2） 匿名用户的名称
可以设置匿名用户所使用的名称，这个名称将显示在评论，论坛帖子等作者的字段处。其默认值为“Anonymous”。
（3） 设置默认首页
用来设置网站的首页的指向
###### 5. 站点维护（Site maintenance）
站点维护，主要是设置站点是否处于可访问状态，导航“管理”--“站点配置”--“站点维护”。
###### 6. 日志和警告
此功能主要配置系统日志记录和警告。导航“管理”--“站点配置”--“日志和警告”。
###### 7. 管理界面主题
Drupal 强大的主题机制可以使用户随心所欲的更换网站的主题。
###### 8. 输入格式
输入格式（Input format）在Drupal 中指的是：在创建站点内容时，对所输入的内容进行过滤所依照的方法。
下面三种是比较基本的输入格式：
Filtered HTML：这个选项会将用户输入的包含HTML 编码的文字进行过滤。去掉一些会有一定影响的HTML 编码。
PHP Code：这个选项Drupal6中需要开启php模块，是将用户输入的内容以PHP 编码的方式进行保存。
Full HTML：这个选项就是不过滤任何用户输入的HTML 标签，原样呈现。
# Drupal系统的后台管理及操作－报告
报告：会列出当前站点的运行情况，以及各个组件是否正常运转。例如可用更新、状态报告等。
最近的日志条目：查看最近的事件日志。
最常见的“拒绝访问”错误：从这里可以查看到“拒绝访问”错误的次数。
最常见的“没有找到页面”错误：查看“没有找到页面”错误的次数还有具体参数。
热门搜索：查看当前站点热门搜索的文章或关键字。
可用更新：可以获取当前站点所安装的模块和模块的可用更新的报告。
状态报告：获取站点管理操作和运行中所发现的问题的报告:
点击导航“管理”--“Reports”，进入站点状态报告页面。站点状态报告页面会列出当前系统的运行情况，以及各个组件是否正常运转。例如配置文件的权限、数据库的状态、文件系统状态等。
默认安装好Drupal6以后，Reports中只有一项，如果开启其他模块，此项将有相应内容添加。
点击“状态报告”，进入如下界面：
![状态报告](http://www.intematrix.cn/sites/default/files/huiheng/bg.jpg)
在条目名称前面有绿色对号标识的，即为正常状态；有红色叉号标识的，即为错误状态或有待更新的状态。Drupal 会在后面给出错误信息，及处理建议。
本页面主要显示了如下系统组成部分的状态：
配置文件的权限是否正确（Configuration file）
1. 计划任务执行情况（Cron maintenance tasks）
2. 数据库架构是否最新（Database schema）
3. 文件系统（上传文件夹）是否可写（File system）
4. GD 库是否安装（GD Library）：PHP 的一个用于图像处理的扩展库。
5. MySQL 数据库版本（MySQL database）
6. PHP 版本（PHP）
7. Unicode 库（Unicode library）
8. Web 服务器信息（Web server）：后面显示的是已开启的，Drupal 使用的模块列表。
在Drupal6中，多了一项：Update notifications，是用来监控模块更新状态的，此功能需要开启Update status模块
# Drupal系统的后台管理界面及操作－如何创建内容
###### Drupal的常用三种内容类型适用范围？
Page：如果你想添加一个静态页面的话，比如说“联系”页面或者“关于我们”,使用page.
Story：Story是最简单的文章，主体架构为：标题、摘要、主体。通常用于新闻、消息发布。
Blog：用户可以创建自己的blog，里面你可以发表自己的感想、体会等。一个网络日志的形式。为自己和公司树立良好的形象。bolg通常用于专业性的见解，或是都对某件事件讲解。
###### 如何在drupal里创建内容？
创建内容：首先点击“内容管理－－创建内容”，之后选择你所要创建的内容类型。
![content](http://www.intematrix.cn/sites/default/files/huiheng/content.jpg)
在这里大致说下创建内容具体细节部分：
Title：是指你所创建的文章标题。
菜单设置：是指创建的内容属于哪一级菜单或是导航条。也可以把当前创建的内容设置为独立的菜单。上级菜单中你可以选择当前内容属于哪级菜单。
Body：用来编辑、添加你所要上传的页面内容。body里：![](http://www.intematrix.cn/sites/default/files/huiheng/dm.jpg)功能作用是编辑器与纯代码操作界面切换。如果页面内容需要添加图片的话，就要先上传图片，然后取图片的图径即可。
关键字，描述的填写：在Meta tags选项中：描述这个文本框中用来写对当前内容简短的概括。
![descrption](http://www.intematrix.cn/sites/default/files/huiheng/description.jpg)
每个关键字之间以分号分开.
附件：主要用来创建页面内容所需的文件。比如：图片，FLASH等。
输入格式：一般默认选择为FullHTML。附注：如果页面内容中含有php代码，则选PHP code
URL路径设置：用来当前页面内容的URL路径，输入形式为英文或拼音。比如“产品－－食品”　products/foods
# Drupal系统的后台管理及操作－用户管理
用户管理：是对站点的注册用户进行管理，主要有权限控制、用户设置、用户资料、角色及用户状态等的操作。
角色：系统默认的角色有二个，一个是匿名用户，另一个是注册用户，由于二个角色是锁定的，是不可以删除的，站点管理员可以添加新角色，并对这个角色所拥有的权限进行配置。点击“添加角色”
权根：每个用户角色都有其自己的权限。
![](http://www.intematrix.cn/sites/default/files/huiheng/roles.jpg)
# Drupal网站系统的应用
###### Drupal的优势：
- 连续世界开源CMS排名第一
- 强大的开发团队，上千名活跃的开发者，400多种第三方模块支持
- 成百上千的站点使用Drupal
核心小，开源更彻底，操作、管理更简捷，让不同领域都青睐于Drupal 。从最初作为社群讨论软件，到之后演变成受欢迎的内容管理系统，再到现在的内容管理框架、网络开发平台，Drupal 在世界范围内广受赞誉，并多次被评为最优秀的开源系统和CMS系统。从个人、企业到政府部门，使用Drupal 建站的成功案例甚多。
###### 国外案例：
- 个人门户或博客：BSWeekly (音乐站点)
- 跨国公司－联邦快递、诺基亚、Adobe、Sun
- 国际著名娱乐公司－华纳兄弟、索尼/BMG、Fox、环球唱片、迪斯尼/ABC
- 新闻杂志－福布斯
- 购物shop－Lotus Boutique
- 网上社区－FireFox 、Davis Applied Technology College
- 高等学府－哈佛大学、MIT
- 政府机构－法国政府、法国卫生组织、 纽约市长Mike Bloomberg 、联合国
###### 国内案例：
- 旅游网站：世外游园 、镜界网
- 酒店餐饮：全球订房网 、甲21号西南民族菜
- 社区类型：中国移动研究院 、说对网 、爱播吧
- 企业网站：时代名品网 、易维讯信息咨询 、INTLR官网 、慧衡数字营销
- 个人站点：Drupal使用者 、主题花园 、台湾花水妹博客 、CMS主题苑
###### 应用行业展示：
![公益和非盈利组织网站](http://www.intematrix.cn/sites/default/files/huiheng/szmg.jpg)**3. 公益和非盈利组织网站—深圳大运当前**
这是深圳SZMG集团为了迎合大运会所开创的户外型运动。网站地址为：http://dydq.szmg.com.cn


