# Growth Hacking让Facebook首页5年未改版，人人网改版自掘坟墓的背后，缺少的是什么 - 文章 - 伯乐在线
原文出处： [知乎专栏 - 覃超](http://zhuanlan.zhihu.com/qinchao/20226008)
![facebook-hp-reversion-00](http://jbcdn2.b0.upaiyun.com/2015/11/f4ec499c7586906f52c7d53d82481f07.jpg)
本文是作者的系列文章：揭开 Growth Hacking 的神秘面纱的中篇，想阅读上篇可点击：《[在帮Facebook实现一天内10亿用户在线的Growth团队工作，我都经历了什么](http://blog.jobbole.com/95131/)》。
#### 现在我们来看什么是 Growth Hacking ？来看Wikipedia上面的介绍：
Growth hacking is a marketing technique developed by technology startups which uses creativity, analytical thinking, and social metrics to sell products and gain exposure. It can be seen as part of the online marketing ecosystem, as in many cases growth hackers are using techniques such as search engine optimization, website analytics, content marketing and A/B testing. Growth hackers focus on low-cost and innovative alternatives to traditional marketing, e.g. utilizing social media and viral marketing instead of buying advertising through more traditional media such as radio, newspaper, and television. Growth hacking is particularly important for startups, as it allows for a “lean” launch that focuses on “growth first, budgets second.”Facebook, Twitter, LinkedIn, Airbnb and Dropbox are all companies that use growth hacking techniques.
这里有几个重点词汇：creativity、analytical thinking & social metrics。另外在方法上有：SEO、web analytics、 A/B testing等。比起传统的growth方法（比如投放在电视或者报纸上投放广告）来说，growth hacker们更加专注于使用低成本高数据化和创新性的办法。
说了这么多理论的东西，难免枯燥，下面解释 growth hacking 中的几个要点，同时配上一些经典案例的分析。
#### Data Driven 的意识
Data Driven 表示“用数据说话”：也就是说产品和运营上的决定以最终的数据结果为准，不因为某人的官大，某人是boss，或者某人讲道理能力强而左右最后的决定。把这个纲要放在第一位，因为这是前提，也是growth hacking得以执行的基础。这个道理说起来很容易，但是真正做起来却是非常难。
这篇文章对于技术细节的阐述很多，为了避免枯燥，我从一个案例开始说起。
众说周知：Facebook 以** move fast and break things**著称，强调 Hack 精神，通俗说来就是：
> 
##### 少废话，先动手干出来再说！
Facebook在2010年的首页如下：
![facebook-hp-reversion-01](http://jbcdn2.b0.upaiyun.com/2015/11/b5d03c0a2011eabf3bc079202c7a792f.jpg)
而2015年的今天Facebook首页如下：（十一刚截的）
![facebook-hp-reversion-02](http://jbcdn2.b0.upaiyun.com/2015/11/6756ecda125aa0e746e45257657128a2.jpg)
可以看出，除了一些细小地方，总体结构和UI基本没有变化。这在如此崇尚快速迭代的公司里其实是难以想象的。但是事实是：
> 
##### 在这五年的时间里，Facebook不止三次曾尝试去改版 news feed，也就是做一个传说中的 news feed 2.0。
5年试过各种方案，比如 Newspaper 样式，比如 Clipboard 样式，比如 两列模拟timeline 的样式等等，虽然都实现了出来，但都因为数据原因没有上线。
最大的一次尝试出现在2012年：当年的VP Sam Lessin 带着FB阵中顶尖的几个 designer, engineer 和 PM 开始着手进行新版本的 Facebook WWW首页的升级。经过大半年的迭代和测试，以及内部发布，新版的首页已经准备就绪。
![facebook-hp-reversion-03](http://jbcdn2.b0.upaiyun.com/2015/11/9314d638a08dce46e312ed7380bcb35b.jpg)
犹如上图这样：这里最大的改进是：左侧之前的繁杂的导航栏变得清爽简洁，feed story里面的图片被加强，因为designer认为图片是整个news feed中最为吸引人的东西，所以应该加强！然后整个页面走扁平化路线，和移动上的导航栏保持一致（和桌面和移动的页面保持 consistent，并且上图左侧导航栏鼠标移上去后有非常平滑的动画效果）。
这个版本让人觉得页面焕然一新，有medium.com的极简风。特别是你对着这个新版页面看了5分钟后，再会过去看一副原版的news feed图，你会觉得上一版的news feed UI就犹如上个世纪的产品一样。以上的分析是典型的按照人类逻辑为出发点，而非 data driven。
感觉就是：一切都显得如此合理，观点如此让人折服。
按照FB的传统，这么大的改版不仅要过 Zuck review，还要灰度发布到：
> 
##### 1% -> 2% -> 5% -> 10% … 到用户那里。
项目组的人会密切关注相应的数据：
> 
##### 比如 DAU（日活）、user engagement（参与度），比如 ads impression（广告的显示度）等防止有任何意料之外的数据下滑。
可惜不幸的是：从 release rate 为 1% 开始，一直到 5%的时候，新版news feed开启的用户群里各项数据指标都在下滑。开始以为是数据采集或者展示的程序里有bug，去查，没有；后来以为是用户对于新界面没有适应而出现的短暂抖动，继续做小优化，同时慢慢开放到更多用户。这样又过了三个月，用户比例一度开到了 12%。因为毕竟这个项目是 Sam 组接近一年的倾力努力，所有人都希望新版的news feed可以开放给所有的用户。但是 data dashboard 上的数据依然很难看，对于那 12% 的用户**，engagement （参与度）和 session length（在线时长）**都下降很明显，这也直接导致** ads impression（广告投放）以及 revenue（营收）**对于这部分用户的下降。
> 
##### All of these impacts are just unacceptable!
经过三个月的游说和优化，数据还是无法改善，Sam哥最终只能低头。Zuck和整个 E team 决定不再进行尝试新版news feed，之前被开启的 12% 用户全部被roll back。Facebook在100%的用户组中回归到之前的版本，只是在局部借鉴新版界面的可取之处，小规模地优化（这些优化也要进行灰度发布和数据验证）。
这次的WWW主页改版的尝试大概花费了Facebook 30人的精英团队接近一年的工作，这对于Facebook来说投入不算小。Sam哥的辩论和“忽悠”能力也堪称一流，但是所有这一切在数据结果面前都必须低头。
Data driven 这一纲领在Facebook的贯彻程度可见一斑。当然Facebook自己也给予他们项目组足够的耐心和时间，宁愿扛着3个月的revenue上暂时损失让他们来优化自己的产品。
事情大概就这样过去了，Facebook又恢复了往日的平静。用户们回到自己习惯的老界面，而Facebook的程序员们又开始在各自的项目上忙碌着。又是三个月过后，覃超小魔王惊讶地发现：人人网改版了！人人宣布内测自己的新版 WWW首页，美其名曰 人人网 V8版：
![facebook-hp-reversion-04](http://jbcdn2.b0.upaiyun.com/2015/11/1e66bc337338995d1d775a7456c811c7.jpg)
使用之后，我的第一感觉就是：What the fuck! 这和Facebook之前改版的 WWW首页实在是太像了。容我再贴一次新版Facebook：
![facebook-hp-reversion-05](http://jbcdn2.b0.upaiyun.com/2015/11/7c64a6491ea1ea246de8dad7d75cde3f.jpg)
对比上下两图，一样的界面，一样的导航栏，除了颜色风格不一致。
我一方面佩服人人网产品经理们敏锐的嗅觉和洞察力，竟然在4-5个月的时间内就把Facebook的一个试验版本这么快地”借鉴”过来；另一方面我仿佛看到一出悲剧正在上演。因为这一个大改版的界面对于用户数据的损伤如此之大，连Zuck都不得不忍痛割爱，放弃项目组接近一年的努力。人人网这么埋头一抄岂不是自掘坟墓？
有趣的是，当时Facebook已经全面地放弃新版首页UI好几个月了，所以很多国内博客上开始说人人网已经摆脱对于Facebook的像素级抄袭……
我之前看到知乎上一些人回答，说人人网里面过产品需求的是：
- Manager：为什么要做功能A？
- PM：因为本土用户如何 blablabla，这个功能如何好blabla，如何能更好满足用户需求blabla……
- Manager：那FB有没有这个功能？
- PM：貌似木有。
- Manager：那这个功能开发先暂停。你再回去多想想。另外你要多花脑筋想想为什么FB不这么做。
- PM：。。。
- Manager：下一个。功能B为什么要上？
- PM2：因为这个功能Facebook上有了，上个月刚出来的。
- Manager：好的，那上！
- PM2：Yes！:-D
我想人人网很可能是没有很全面的监控和灰度发布，亦或是有工具但是里面的管理团队没有 data driven 的意识。要不然他们会发现自己的V8上线后各项数据指标的下降（或者加速下降）。
最后附带我在svbtle上看到一个blog里写的话：
![facebook-hp-reversion-06](http://jbcdn2.b0.upaiyun.com/2015/11/978eb69a468e00361504d18693b60eea.jpg)
##### 最受启发的话：
- “We’re blind. It doesn’t matter what any individual person thinks about something new. Everything must be tested.”:
— 即使像 Facebook 这样的航母，在创业的大海里还是犹如“盲人”一样，很多产品的改动没人真正知道方向到底在哪儿。所以这里采用的方式就是 “Everything must be tested”。在灰度发布后，data dashboard + A/B testing 就犹如航母上的雷达或者声纳一样，对于方向和航线起到验证作用。
-  ”We throw out an idea, and when the data comes back we look at the numbers. Whatever goes up, that’s what we do. We are slaves to the numbers.”
当我们有任何产品上的试验性的想法时，我们先做试验，灰度发布到少量用户，看反馈的数据如何。这句话后半部分多多少少有点 pessimistic（悲观）— 我们是数据的奴隶，或者说我们应该敬畏数据。
#### 数据采集（Data Infrastructure） & 数据展示页面（Data Dashboard）
Data Driven 的意识应该从founding team就被重视，而这一切的前提是拥有良好的数据采集（data infrastructure）和数据展示页面（data dashboard）。
刚进入Facebook的第一天，HR给我们展示Facebook的Data Dashboard，上面有密密麻麻的各种数据，比如说：
> 
##### 月活跃用户（monthly active user）、日活跃（daily active user）、用户参与度（user engagement）；
##### 每一个具体的功能的活跃程度（比如：Photos用户，group用户，public page用户等）。
##### 每个具体的数据参数点进去，还可以看每段时间（比如这个月，今年以来等）内的详细变化， 另外还有分国家和地区的统计。
##### 总之，应有尽有，就好像一个股票的查询系统一样。
这就反映出来 growth hacking 最基础的一步：
> 
##### 建立完善的 Data dashboard（数据收集和展示页），然后确定关于增长的 key metrics（核心指标）。
##### 基础数据指标包括：网页的 page views、user views，而app一般看 downloads、daily active users、weekly active users等等，同时还有用户在网页或者app上的在线时长。
这些基础数据页一般在现在流行的数据统计平台默认收集和显示（比如友盟、百度统计之类的）。
另外需要强调的就是除了上面提到的这些基础数据之外，一个公司还应该专门花时间来总结自己业务特有的数据指标。一般说来，app分为两类：
- 
##### Transactional App：交易型应用。
也就是这个app只是一个电商产品的销售渠道，app的属性以卖东西为主，比如 Amazon、京东、淘宝；
- 
##### Product App：产品型应用。
这类app自己本身就是一个产品，比如：Facebook、Nice、Quip、今日头条等等。
对于交易型app，一般来说 key metrics 比较看重的是：“ Conversion Rate” 转化率 和 “Basket Value” 客单价。而对于产品型app，则更加看重它的日活、月活、用户留存度、在线时长，还有连续登录天数之类的。
![facebook-hp-reversion-07](http://jbcdn2.b0.upaiyun.com/2015/11/e8c79e28c38d9e3b75703b8988be4e21.jpg)
#### 一些轶事：
1、遥想当年（2005-2007）：MySpace 和其他一些社交网站都是关注在自己的注册用户上，Zuck则希望整个公司忽略注册用户数，而是只看月活和日活。现在看来，你可能觉得是天经地义的事情，但是在2005那个年代，的确很多公司都是以注册用户数（而非活跃用户）作为公司关注的指标。
2、同时，不同的公司里，对于不同的数据指标的重视程度是不一样。比如Facebook或者图片社交instagram、nice等，属于中等频次的社交，所以对于月活或者周活需要最为关注。而Whatsapp和微信属于messenger，也就是天生高频次的应用，所以日活是他们应该正视的目标（看月活是在欺骗自己）。所以由于每个公司的业务的不同，就应该对于增长数据有着不同的侧重点；而这些侧重点应该由创始团队和高层们在产品最开始就决定下来，进而贯彻到整个公司的产品和运营中。
3、将这些 dashboard 上的指标当做是公司运营和功能迭代（或者取舍）的标准。这些 dashboard 就犹如之前提到的公司内部的工具一样，应该和对外的产品一样受到足够的重视，然后安排最厉害且有责任心的工程师来进行维护。当然，硅谷在这方面做得很好，使得这些数据相关的产品和框架不断被大牛们优化，变成了最近特别热门的“spark”，“kafka” 和FB内部的神器 （PTail）。只可惜PTail最后没有开源。
![facebook-hp-reversion-08](http://jbcdn2.b0.upaiyun.com/2015/11/b88c9e536024efc02bea7f16dbc0e7e9.jpg)
#### 把 User retention（用户留存）作为命根
Facebook Growth组的元老 Alex Schultz 说过：Retention is the single most important thing for growth。用户留存是一个需要反复进行调优和推敲的地方，也是一个公司的产品最最核心的数据。但是FB和其他Growth Hack组织得出的结论是：留存率最根本的决定因素还是你的产品是否能有效地满足用户的核心需求。
这里Alex提出过 好、坏 产品在留存上的区别图：
![facebook-hp-reversion-09](http://jbcdn2.b0.upaiyun.com/2015/11/90eec56e0a07f97ab84aaab36fdbd416.jpg)
好的产品会像蓝色的那条曲线，在N天后趋于稳定。对于我们投资人来说，也最喜欢投资这样的产品，当然对于创业者来说，你们要做的就是在每轮融资前努力优化自己的留存；只有留存保证，后来资本才能起到实打实的加成力量。
虽然留存的核心是产品满足用户的核心需求，但是在战术上倒是有好些技术点：
##### 1、新用户拉取要循序渐进
这是第一条也是最最基本的一条。其核心思想就是你每次做推广活动拉来的新用户就好似股市里的现金，在没有完全调优你的模式得到很好的用户留存率之前，切忌不要急着拼命拉新用户。就好像枪手弹夹里的子弹，要节约使用，打两枪，看一下效果，做出相应的调整和优化，然后再开枪，直到在用完手中的子弹后，得到最好的命中率（留存率）。另外要特别注意，用户对于你的app是有一定的”免疫力”，如果这次它试用过你的app，发现有它不喜欢或者做得不够完善的地方，它可能放弃使用甚至直接删除。这对用户的影响还不如压根不进行推广来得好。
##### 2、尽快给用户一次“aha”的体验
用户使用你的app或者网站，很多时候是被你震撼了一下，原因可能是你的功能满足了他们的刚需，也可能是某个功能让他们用得意外地爽。不管如何，Facebook的试验发现，越快的让用户体验到“aha”的感觉，越能有效地留存下你的用户。所以，在制作app的时候要好好思考一下：你的app中的aha功能是什么？给你旁边的几个朋友先试试，看他们平均多久“爽”到？如何能加快这个aha体验到来的过程？ 甚至在一些极端情况下， 我们甚至可以广发邮件给你的用户，直接教育他们使用的一些小窍门来引发这么一个aha时刻。
##### 3、Emails & Push消息
用它们来当做和用户交流的媒介，记得经常去唤醒自己的用户。这就类似于我们中国社会里的关系，朋友之间要时不时来往那么一下，拉近关系。 Email 和 push 这招在中国的app里感觉已经被滥用，所以这里可能要使用critical thinking，好好思考下如何控制好发 email 和 push 的频次。
##### 4、用户访谈
打电话或者直接在推广的时候询问用户，对于自己的app或者web的看法，看到底是在什么环节让用户不爽。这是一个苦力活，也是一个传统活；但是它真的很有效。
##### 5、引入用户粘性机制
众所周知，有一些机制可以很有效地增加用户粘性，创业者们可以根据自己的app或者web的实际业务情况进行借鉴。比如说，在Facebook之类的社交网络里经常使用的一套鼓励机制就是经常性地给你推荐好友，其本质就是想让你在Facebook平台上可以更多地沉淀你的社交关系。另外，Facebook timeline的功能则是有效地将你的个人历史事件记录下来，这样你的 facebook timeline 就成为了一种个人历史，在这样丰厚（而且有情感）的数据积累面前，你很难抛弃Facebook而转入一个新的社交平台（比如Google+）。
##### 6、对于超级用户给予鼓励
超级用户就是平台上的黄金用户，这一类的用户天生就是你的app的拥护者和口碑传播者。所以任何一个公司都应该好好思考如何有效地留住他们，然后激励他们。常用的办法是给予他们一些奖励（比如红包、优惠券或者返点什么的），而对于社交平台，则很多时候通过推广你的内容，或者有效地增加你的粉丝来进行鼓励。比如知乎上经常会推一些好的答案（甚至上日报）。在这方面做得最好的是网络游戏们，我曾经饶有兴趣地研究过它们这些游戏里面的积分机制、等级机制和荣耀机制，觉得设计得极为透视人性。另外在WOW里面的工会制度，更是有效地将游戏和社交创新性地结合起来。
##### 7、持续优化体验，让用户用得爽
这似乎是废话，但是很多时候一些app或者网站经常犯这样的错误。这是最容易被忽视的一点，千万千万注意！一般反例是：创业团队或者老大们为了觉得在做事在奋斗，不断地上各种feature，1.0->2.0->3.0不断迭代，UI的界面改版来改版去。问起原因，无外乎就是为了有事做嘛，不然我们产品或者技术团队不就下岗或者磨洋工了？或者也经常会说：竞争对手们都上了，我们也要上。或者Tinder最近火，所以我们也要上那种左滑右滑来进行选择的界面。
但是在用户看来，很多时候却是丈二和尚摸不清头脑，觉得这鸟app为什么按钮位置老是变来变去，经常找不到。又或者是几次的改版让用户觉得使用体验每况愈下，最终变成 ghost app。更可怕的是快速迭代的过程中，bug不断涌现，最后把用户惹毛。那如何避免呢？ 还是最开始所阐述的：产品迭代和更新要以数据驱动，以用户满意度为指标，从 A/B testing 的方式来验证一个新的迭代是否有效地提高了用户体验。
![facebook-hp-reversion-10](http://jbcdn2.b0.upaiyun.com/2015/11/d924698edf5a1a31887ddde7ad148026.jpg)
中篇就在这里。我写得真累，当然也感谢你们能够静下❤来阅读这么长的文字。下篇我将介绍 Facebook 的增长神器——灰度发布 + A/B testing。 另外说明一下growth team在Facebook和其他硅谷公司里的经典配置。最后结合我在国内一年多的创业和投资经历来阐述一下我觉得中国初创公司在使用 growth hacking 的时候要注意的地方（比如 growth hacking 的某些策略对于创业初期的公司极有可能是有害的，大家要特别注意）。
