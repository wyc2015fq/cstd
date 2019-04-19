# 最神秘的大数据公司Palantir（二）：魔幻之旅 - 文章 - 伯乐在线
原文出处： [何明科](http://zhuanlan.zhihu.com/hemingke/20577369)
Palantir最早的总部在加州的旧金山，然后往南搬到了Palo Alto（也是我大斯坦福的总部，此处必须软文植入，斯坦福大概占据这个小镇60%以上的面积），这个办公场所之前还曾是Facebook的办公场地。这个地方被Palantir命名为The Shire，名字同样来自于《魔戒》作者托尔金（Tolkien）的另外一部小说《霍比特人》。The Shire（夏尔）是霍比特人的老巢。于是Palantir的员工还将一个大会议室装扮成小说中的一个房间的样子。可见其魔幻之心，而Palantir本身的历程，也是一段魔幻之旅。
![1](http://jbcdn2.b0.upaiyun.com/2016/02/28c8edde3d61a0411511d3b1866f06363.png)
#### **一、****初心**
先讲情怀。Palantir这个名字是Thiel取的，来源于他所爱的《指环王》：在剧中，Palantiri（多一个i，这里不是笔误）是一个可以穿越时空、看到一切的水晶球，它可以帮助各位大神小怪和其它水晶球建立联系，从而可以看到附近的图像（其实它是魔幻时代的摄像头搭配监视器）。再讲价值观。Thiel作为一个右派和自由主义者，目睹911之后日益加强的反恐行动的副作用——对个人隐私的侵害，深感心痛。Thiel觉得能够用技术去缓和甚至是去解决这样的矛盾冲突。（说到这里，面对当今的情况，估计再没有人骂地铁安检是脑残了吧？）情怀+价值观+技术能力，三者的结合就是Thiel创立Palantir的初衷：用技术手段构建魔法球Palantir看清世界，在安全和隐私保护之间找到平衡。
> 
想起来容易做起来难，即使Thiel有这样的愿景。 2010 年Palantir依旧遭遇了隐私侵犯的信任危机。当时，一家与美国政府关系密切的软件公司HP Gary的邮件系统被黑客侵入，并被挖出 Palantir 向美国银行做的一个展示文档，里面说明了 Palantir 可以如何通过与第三方合作，有效打击维基解密。这就与Palantir一直宣扬的价值底线——尊重言论自由及保护个人隐私，大大冲突。
事件爆出后，即使Alex Karp 很快发表声明说撇清关系。但是由来已久的对Palantir暗地帮助美国政府侵犯隐私的猜忌，一时喧嚣尘上。
下图是泄露出来的带有 Palantir标记的软件界面，其中有提到专门针对维基解密支持者 Glen Greenwald的部分。Glen Greenwald因受斯诺登委托不断曝光材料，而成为政府的眼中钉。
![2](http://jbcdn2.b0.upaiyun.com/2016/02/665f644e43731ff9db3d341da5c827e12.png)
但在Palantir创立之初，并不顺利，整个团队都是靠Thiel提供个人资金来养活。而无论是在融资上还是找客户打单上都遇到不少困难，没有人愿意相信这几个22岁的小鲜肉，和做为年轻人服务的Social App不同，做ToB服务还是需要一些头有白发的老腊肉，才能更容易打动投资人和潜在客户。
> 
从这里其实可以发现些端倪：Thiel已经不准备非常深得介入这家公司的运营，而是把自己定位在早期投资人及Chairman的位置，否则以Thiel的技术、运营及管理经验，出任CEO是最合适不过的。
于是Thiel开始为这家初创的公司寻找CEO。最初的目标当然是名校MBA，但是他们没有人满意，因为MBA除了口吐莲花冒商业术语，没人了解他们远大的目标，仅仅把他们当做一个普通的做项目的咨询公司。但是Karp让几位创始人虎躯微震，虽然没有技术背景及丰富的商业经验，但是他和欧美有钱人的深厚关系以及哲学追寻本源的能力（比如：迅速理解复杂技术问题并能用极度简单的语言来解释），却打动了几位创始人。
Karp为这家公司交上的投名状就是去欧美各地融资，然而最初的进程非常不顺。硅谷传统的两大VC基金都对他们说了不：红杉的Michael Moritz（硅谷的VC双子星之一）在会议中一直心不在焉；KPCB的合伙人更甚，花了一个半小时来说服他们，Palantir这个项目是一个必然失败的狗屎。
然而最后拯救他们的是CIA，不仅带来了订单，还通过CIA的创投基金 In-Q-Tel给他们贡献了两轮投资，超过200万美元。CIA最看重的是他们解读数据的能力。
有了钱有了客户，Palantir开始了正规的发展，Karp也自然成为了CEO。找方向找钱找客户找人，是CEO的四大技能树。Karp虽然不是技术背景，但是凭借其跨界的优势，反而吸引来了不少明星工程师，即使在Palantir出名后简历潮水般涌来的时候，他还是最佳的试金石，能从众多的求职者中找到能力最强以及最适合Palantir文化的未来之星。
**二、起步**
2004-2009年，Palantir还是主要为CIA等美国政府服务，为他们开发定制的软件，实现整合的信息检索和提取及分析关键信息。大概情况就如同我们在《犯罪心理》等经典美剧中看到的一样，面对整墙整墙的纷杂信息，抽丝剥茧找出最核心的部分。其实直到2008年，CIA都一直是Palantir唯一的大客户。
![3](http://jbcdn2.b0.upaiyun.com/2016/02/38026ed22fc1a91d92b5d2ef93540f202.png)
在最初的项目进展过程中，其实并不顺利。这些工程师以前的工作经验是倒腾Paypal等顶尖互联网公司的成熟系统，然而面对美国政府内分散而略显初级的系统（包括数据库、邮件、Excel等等），同时还要在数据收集过程中满足各种权限及保密的需求，真是颇费脑筋。然而，在这样的困难中，Palantir的工程师并没有忘记初心，始终坚持不做项目而开发系统级的产品。将这些敏感政府中的信息整合在一起的额外好处也得以体现，所有查询这些信息的人将在系统中留下痕迹，这可以防止斯诺登类似的泄密事件以及亡羊补牢。在吕克贝松的电影《致命黑兰》中就有相关情节，作为杀手的女主角不小心留下照片给男友，结果男友的男友找到警局的暧昧黑妹查询其信息，被FBI的系统发现正在查询这个信息的黑妹，最终把整条线牵出来，让隐秘多年的女杀手浮出水面。
![4](http://jbcdn2.b0.upaiyun.com/2016/02/011ecee7d295c066ae68d4396215c3d02.png)
到2010年，Palantir一下子抓住翻身的机会。在与[Information Warfare Monitor](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Information_Warfare_Monitor)（IWM）的合作过程中，逼格满满得发现了两个黑客网络：Ghostnet和the Shadow Network（俗称鬼魂特工网及影子幽灵特工网）。
> 
IWM，其实也是一个半官方半民间的组织，而且是由美国的铁腿哈巴狗加拿大的两所学校发起的，并于2012年年初就关闭了。结合这些敏感的时间点及信息，有理由怀疑IWM和Palantir其实就是沆瀣一气赚国会的钱，顺手满足政府意愿一起坑害某超级大国。
并宣传这两个黑客网络都来自于某超级大哥的蓝翔技校，并被用于攻击和盗取印度及美国等国家的敏感信息。凭借此机会，Palantir逐渐打开名声，踩着蓝翔技校的尸体逐渐走向高富帅的道路，兼职是踏香（翔）而来。想想美国的斯诺登、维基泄密以及近期的苹果后门，对于灯塔之国的行为真是醉了。
![5](http://jbcdn2.b0.upaiyun.com/2016/02/4e44f1ac85cd60e3caa56bfd4afb675e2.png)
2010年，Palantir终于摆脱了单纯依赖于国会大老爷们赏几口的低声下气，走向企业服务而实现业务多元化。年初的时候JPMorgan成为Palantir的第一个企业客户，Palantir帮助其解决欺诈问题。在4月份的时候，宣布与Reuters集团达成战略合作，其Palantir Metropolis卖给Reuters。华尔街上，Palantir已经成为一个重要的服务商，其主要任务号称是对抗来自与某天朝上国的翔攻击。同时，与政府部门的合作也在拓展，开始涉及到医疗和金融等多个领域，其中还被用于监控金融危机后美国政府大放水刺激经济的众多资金的合理使用以防欺诈。
> 
Palantir Gotham平台和Palantir Metropolis平台，前者主要用于国防安全领域，后者更偏重于金融领域的应用。Palantir Metropolis可用于整合所有的定量数据、 生成和测试创意并云协作。Palantir Metropolis是有效和定量的分析平台，能够提供适当配套的分析工具，以便满足多学科研究和复杂的研究的需要。关于产品的细节在下一章会有更多介绍。
![6](http://jbcdn2.b0.upaiyun.com/2016/02/3d2f8900f2e49c02b481c2f717aa90201.png)
**三、腾飞**
到2013年，据不完全统计，和Palantir合作的美国国家机构已经达到12所，基本是好莱坞动作片的大咖秀，大家耳熟能详的包括：CIA、美国国土安全局、FBI、美国国家安全局等等。特别是在2013年9月，Palantir拿下美国证监会（SEC）高达1.96亿美元的大单。
关于Palantir历年的收入见下图（[具体数据及图表原型请点击](https://link.zhihu.com/?target=http%3A//wentu.io/publish%233d3e1483bc751a95)）。以政府项目起家，但是随后不断扩展到华尔街和各种商业公司，到2015年政府部门贡献的收入已经占到25%以下，可见在收入多元化方面进展神速。
（数据来源：华创证券）
根据2013年美国大数据企业收入排行榜，Palantir已经名列第八，足见其实力（[具体数据及图表原型请点击](https://link.zhihu.com/?target=http%3A//wentu.io/publish%233d3e1483bc751a95)）。
![7](http://jbcdn2.b0.upaiyun.com/2016/02/cd7fd1517e323f26c6f1b0b6b96e3b3d.png)
而真正让Palantir飞入寻常百姓家的是靠着两个大人物的垫背。一家受雇于Securities Investment Protection Corporation（SEC旗下组织）曾爆出猛料，他们通过使用Palantir的软件，在整合40年的记录及海量数据并充分挖掘之后，终于发现了麦道夫(Bernie Madoff)的“庞氏骗局”。
> 
联想到中国最近不少P2P的庞氏骗局，无论是e租宝还是鑫利源，是不是有了Palantir，人民群众的损失会小些？
![8](http://jbcdn2.b0.upaiyun.com/2016/02/815e6212def15fe76ed27cec7a393d59.png)
除了翻出麦道夫，在随后不久的美国政府公布的官方报告中，Palantir在抓住并击毙本拉登（Osama bin Laden）的行动中，又立上了一功。
![9](http://jbcdn2.b0.upaiyun.com/2016/02/4c0d13d3ad6cc317017872e51d01b238.png)
自此Palantir完全走入人们的视线，成为明星级的公司。而担心极端主义者的报复，Palantir公司也为Karp和Thiel配置了保镖，均为前海军陆战队的精英，每天24小时不离身的全方位保护。在通过技术部分剥夺美国大多数人的隐私之时，作为一种回报，Karp和Thiel也通过另外一种方式丧失了自己的自由和隐私。**四、明星**混到这个地步，即使亏损或者不IPO，也不会差钱了。Palantir早已成为投资圈追逐的明星企业。关于Palantir历年的融资情况见下图（[具体数据及图表原型请点击](https://link.zhihu.com/?target=http%3A//wentu.io/publish%233d3e1483bc751a95)
），总共经历了十几轮融资，从A轮已经快到Z轮了。最早的投资来自于CIA旗下的In-Q-Tel风投基金和Thiel的Founders Fund。而最近一次融资发生在2015年底，共计8.84亿美元，投资基金包括In-Q-Tel、Founders Fund和老虎基金。估值超过200亿美元，成为仅次于Uber和Airbnb的第三大独角兽。
![10](http://jbcdn2.b0.upaiyun.com/2016/02/8d8e353b98d5191d5ceea1aa3eb05d43.png)
而对于硅谷的各大牛人来说，Palantir也是趋之若鹜的地方。在员工待遇方面，Palantir也是首屈一指。根据美国的薪酬平台Glassdoor在2014年的数据，在实习生月薪排行榜上，Palantir高居第一，达到7012美元（[具体数据及图表原型请点击](https://link.zhihu.com/?target=http%3A//wentu.io/publish%233d3e1483bc751a95)）。
![11](http://jbcdn2.b0.upaiyun.com/2016/02/7bfc85c0d74ff05806e0b5a0fa0c1df1.png)
在办公环境方面也足够舒适和尊重员工的个性，而且很有极客范儿。用各种魔幻小说及漫画来命名办公室名字，用元素周期表中的元素来命名软件的更新。
![12](http://jbcdn2.b0.upaiyun.com/2016/02/c8b2f17833a4c73bb20f88876219ddcd.png)
文献：
1、Wiki
2、Forbes：[How A ‘Deviant’ Philosopher Built Palantir, A CIA-Funded Data-Mining Juggernaut](https://link.zhihu.com/?target=http%3A//www.forbes.com/sites/andygreenberg/2013/08/14/agent-of-intelligence-how-a-deviant-philosopher-built-palantir-a-cia-funded-data-mining-juggernaut/%3Futm_source%3Ddailyfinance.com%26utm_medium%3Dpartner%26utm_campaign%3Ddos%2520santos%26partner%3Daol%2363459d793da8)
