# 免费知识哪里来？Arxiv 使用指南 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [ThoughtWorks](http://www.jobbole.com/members/thoughtworkschina) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
如果你非常确定自己想要找什么，比如知道论文的名字（算法的名字）或者作者的名字，直接去Google Scholar上搜索是最快的。然而如果你并不是很确定自己想要什么，只是想要看看某个领域的最新发展，知道大家都在干什么，然而却发现Google Scholar给你的结果多半不靠谱，请读下去。
![](http://insights.thoughtworks.cn/wp-content/uploads/2018/01/500350520-1024x576.jpg)
### 引子
最近半年，在我身上时常发生下面这类对话：
> 
“Hey， 你知道XXXXXXXXXXXXX问题怎么解决吗？”
“哦，我刚好读过两篇相关的论文，等下发给你。”
……
尴尬的沉默，一般来说拿到文章之后对方就再也不会来找我了，虽然我是真心的想要交流，然而有一些好学的孩子会接着问：
> 
“你的论文是哪里找到的呢？”
鉴于论文背景不同，我会丢一个链接过去——这个链接（网站）在大部分数学/物理/计算机/统计领域里面，都是[https://arxiv.org/](https://arxiv.org/)。
### Arxiv是什么
arxiv设计的初衷是一群物理学家们想要交流自己将要发表的论文，可以想象一下，在上世纪九十年代，人们用的还是软盘（包括5.25吋软盘和3.5寸什么的，存储空间可以说以KB计算），邮箱也不例外，投稿高峰期分分钟几百个KB的邮箱就会被文章塞满。Paul Ginsparg一看这不行，论文如果想要很好的被分享并审核，大约是需要集中存储的，就在1991年于LANL（[http://www.lanl.gov/](http://www.lanl.gov/)洛斯阿拉莫斯国家实验室）建立了arxiv的雏形，如下图所示。
![](http://insights.thoughtworks.cn/wp-content/uploads/2018/01/1-Arxiv.png)
当时有个萌萌的域名：[http://xxx.lanl.gov/](http://xxx.lanl.gov/) ，到现在也可以被访问。不过由于LANL作为一个严谨的科学实验室实在是懒得运营这个网站，后来就被康奈尔大学接管支持。
然而，今天当我们谈到arxiv，就不能不说Open Access【开放获取】。 我们都知道，以前看文章是要钱的，直到今天，看大部分的文章也是要钱的。如下图所示，看一篇Nature一般要20美元，折合100多人民币。
![](http://insights.thoughtworks.cn/wp-content/uploads/2018/01/2-Open-Access.png)
这个价格吧，说贵不贵说便宜不便宜，在大部分有钱公司和牛逼学校买个site license或者报个销都是不成问题的，但是对那些有科研兴趣的个人或者是那些落后地区的学校，就很成问题了。像马拉维啊中非啊这种国家，到2016年，官方数据中人均GDP才400美刀（大约20篇文章），你让人家怎么玩嘛。难道要让知识像财富一样集中在少数人手里么？
就好像我们一直希望互联网拥有中立性（电信运营商、有线电视公司等互联网服务提供商应当平等地对待所有通过他们网络的流量，网络服务提供商有区别地对待不同的流量可能会使大公司能够限制消费者的自由）一样，我们并不希望因为钱而阻挡了知识的传播。 所以，我们有了[布达佩斯宣言](http://www.budapestopenaccessinitiative.org/read):
> 
There are many degrees and kinds of wider and easier access to this literature. By ‘open access’ to this literature, we mean its free availability on the public internet, permitting any users to read, download, copy, distribute, print, search, or link to the full texts of these articles, crawl them for indexing, pass them as data to software, or use them for any other lawful purpose, without financial, legal, or technical barriers other than those inseparable from gaining access to the internet itself. The only constraint on reproduction and distribution, and the only role for copyright in this domain, should be to give authors control over the integrity of their work and the right to be properly acknowledged and cited.
作为开放获取的先驱者，我们要为arxiv——这一开放获取的先驱鼓掌！为我们带来的好处就是，免费！免费！免费！
基于我的个人访问经验，大家如果想知道都有哪些杂志或者期刊提供免费的资源，可以参考这个列表：[https://en.wikipedia.org/wiki/List_of_open_access_journals](https://en.wikipedia.org/wiki/List_of_open_access_journals) 图中是“数学”类目下提供开放获取的期刊。
![](http://insights.thoughtworks.cn/wp-content/uploads/2018/01/3-mathematics.png)
**实在找不到免费文章咋办？**
我一般会直接发邮件问作者要，顺便聊聊自己的研究领域以及跟这篇文章相关的几个最新问题，成功率还是蛮高的，还可能交到好朋友。
说到Arxiv还有一样不能不说的就是LaTex——个人认为最美的文本编辑器（或者语言？），只是从学术圈儿跳出来之后发现大家好像都懒得用，在此由于篇幅所限，不再赘述。
### Arxiv有什么
由于各种历史原因，Arxiv中文献的主要研究领域还是数理哲学，包括数学/物理/计算机/统计/天文/定量生物/定量金融等领域。截止到2016年的发表量统计可以参考下面这张图。
![](http://insights.thoughtworks.cn/wp-content/uploads/2018/01/4-Arxiv.png)
左图中是每年新提交的文章数目，右图是每年的发表量所占百分比（总和为1）， “hep-“代表高能物理学，(hep-th+hep-ph+hep-lat+hep-ex), “cond-mat” 代表凝聚态物理学, “astro-ph” = 天体物理学, “math” 代表数学, “other physics” 表示物理学的其他领域（ physics+nucl+gr-qc+quant-ph+nlin） “biology” 指的是量化生物学，“finance” 指的是量化金融，“cs”指的是计算机科学。
我们可以看到，在2002年之前，计算机科学领域的占比几乎可以忽略不计，然而到2016年已然占据了近五分之一，并且还在以极快的速度增长。而1992至1996年间占据了大半江山的高能物理学，到了今天地盘几乎被蚕食殆尽，只剩约10%的份额苟延残喘。沧海桑田，可见一斑。
还有论文发表总量如下图：
![](http://insights.thoughtworks.cn/wp-content/uploads/2018/01/5-Arxiv.png)
（信息来自：[https://arxiv.org/help/stats/2016_by_area/index](https://arxiv.org/help/stats/2016_by_area/index) ）
从图中，我们可以清楚的发现三个事情：
- 所有论文发表量都在时间轴上爆发式的增长，在上面这张历史发表总量图中体现的尤为明显；
- 数学一家独大，不管是在年度发表量还是历史发表总量上；
- 计算机（cs）在历史发表总量中只占了8.3%，然而2016年的提交量占了18.3%，结合提交数量图的分析，真是长势喜人、值得期待。
- 每个月近10000篇文章提交（正式接受的会少一些，如果精准到感兴趣的垂直领域会更少）。
那么，不想只关心数理领域的咋办？
很简单，就好像任何市场一样，arxiv火了之后大批人跟风，所以我们现在有了生物学版本arxiv [https://www.biorxiv.org/](https://www.biorxiv.org/)，心理学版本arxiv [https://psyarxiv.com/](https://psyarxiv.com/) ，等等等等。当然，知识的沉淀往往需要时间，这些垂直领域的“arxiv”们到目前为止还不是非常成熟，所以我仍然建议大家结合上文提到的Open Access列表里的机构，去寻找想要的免费资源。
### Arxiv怎么用
就像我在本文开头提到的那样，这个引擎最大的效果是当你并不确定自己想要什么的时候，去看看领域的最新发展，知道大家都在干什么。它最大的好处是值得信任。 当然，同样值得信任的还有很多——虽然它们大部分都要钱，针对大多数期刊们，可以按照影响因子从大往小排个序（众所周知，影响因子这个评定标准就像高考成绩一样非常片面，然而这也是目前最普遍的解决方案了），前文中提到的Nature系列和Science系列，都非常值得信任。不做赘言。
很高兴的一点是，收费（还收的比较贵的）基本都是期刊，然而与其他领域尤其是生物学方向不太一样，计算机方向最顶级的往往是会议而不是期刊，而会议往往是不收费的！这里举个例子，International Conference on Machine Learning 【ICML】，机器学习方面的顶级会议之一，其中所有的文章都可以在下面的链接里找到。[https://icml.cc/Conferences/2017/Schedule?type=Poster](https://icml.cc/Conferences/2017/Schedule?type=Poster)
太幸福了！——这个也是我最最最推荐大家找到值得读的文章的办法：盯着领域著名会议列表刷（虽然有个缺点，就是刷到啥算啥）。
但是，往往新入门的人往往没有办法靠关键字搜索【看脸】来分辨哪个期刊哪个会议才是真正有价值的。每年的会议不知凡几，我在wikiCFP上随便搜了一下人工智能，在接下来一年内仅仅这个垂直领域就有3130个会议。平心而论，有多少是真正有价值的呢？每个会议按照50篇paper来算，15万篇paper，一个人类就是不吃不睡不工作全职看一年paper，能看几篇？
当然，据个人经验，关于计算机方向的会议排名，大家可以参考下面的链接 [https://www.aminer.cn/ranks/conf](https://www.aminer.cn/ranks/conf) ，我截取了AI/PR (人工智能/模式识别）领域排名靠前的一段，如下图。有想看计算机视觉的可以参考里面带”vision”的，具体会议的分析和比较将另文分析，这里不做赘言。
![](http://insights.thoughtworks.cn/wp-content/uploads/2018/01/6.png)
但是，如果不想这样盯着列表一个一个会议慢慢看下来呢？如果就是想要找一下某个主题或者关键字呢？或者就是想要知道这个月出现了什么新算法呢？——会议可都是一年一次的。 这个时候，就是arxiv大放异彩的时候了。它给了我们一个更集中的搜索平台，而且相对来说更值得信任（反正比起来什么都可以搜索的Google Scholar，用户要筛选的噪声少了很多）。
没有证据都是耍流氓？ 请点开下面这个链接 [https://arxiv.org/list/cs.LG/recent](https://arxiv.org/list/cs.LG/recent)，这是对Machine Learning领域的一个搜索。仅仅看这一页简介那些熟悉的作者名字，就知道大部分都不会让你失望的。
![](http://insights.thoughtworks.cn/wp-content/uploads/2018/01/7.png)
尽管arxiv定位是preprint，但其中也囊括了各种已经被NIPS，AAAI等等行业顶级会议接受的文章。而且，所有这些，都有直接的pdf原文可以免费下载。搜索和筛选成本都低到极致。同时，只需要点击感兴趣的作者名字，所有他发表过的文章都会被列出来（如下图中的Lei Shu），而完全不用担心其他搜索引擎中常常发生的同名同姓作者的麻烦——这一点相信搜索过中文名拼音的都深有体会——张王李赵遍天下绝对不是说说的。
![](http://insights.thoughtworks.cn/wp-content/uploads/2018/01/8.png)
得数据库如此，夫复何求？
作为对比，下面是我在Google Scholar同样搜索Machine Learning关键字得到的结果。大家可以自行体会时效性，相关度和质量。
![](http://insights.thoughtworks.cn/wp-content/uploads/2018/01/9.png)
当然，时效性这个可以通过点击左手边的“Sort by date”来解决，然而点击了之后是这个样子的：
![](http://insights.thoughtworks.cn/wp-content/uploads/2018/01/10.png)
——反正前面Springer家的都是要钱的，质量不提，单单是能下载的pdf也真的不常有。
更重要的是，google作为跨平台老牌全网搜索引擎，看到某个关键字的时候并不会针对特殊的领域进行搜索，所以大批量主题无关的文章（不信的可以搜一下lenet，vgg这类，看看搜索结果），就会干扰搜索结果。
所以，当你要找的参考文献属于数理领域尤其是AI/ML/Stat相关，而Google Scholar又没能给你满意的结果（又或者很贵），就试试看arxiv吧！
