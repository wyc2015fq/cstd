# Hulu宣讲会小记（9月5日计算所） - 三少GG - CSDN博客
2012年09月07日 10:22:45[三少GG](https://me.csdn.net/scut1135)阅读数：5804标签：[coffeescript																[actionscript																[youtube																[工作																[前端开发](https://so.csdn.net/so/search/s.do?q=前端开发&t=blog)](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=youtube&t=blog)](https://so.csdn.net/so/search/s.do?q=actionscript&t=blog)](https://so.csdn.net/so/search/s.do?q=coffeescript&t=blog)
个人分类：[PKU Research](https://blog.csdn.net/scut1135/article/category/801457)
### [Hulu宣讲会小记（9月5日计算所）](http://blog.csdn.net/keaiweiwei/article/details/7950390)
2013的校园招聘已经正式拉开序幕。Hulu的宣讲会是我参加的第一个宣讲会，心情十分激动。晚上7点正式开始，因为担心人多，我5点半就跑过去了。可是大家好像没有那么积极，6点半以后才陆续来人，不过最后还是把计算所的报告厅挤得满满的，看来Hulu还是很有号召力哒。就像“早起的鸟儿有虫吃”一样，我来得早既有好座位，还有Hulu发的杯子，好喜欢，这里炫耀一小下下。![吐舌头](http://static.blog.csdn.net/xheditor/xheditor_emot/default/tongue.gif)
![](https://img-my.csdn.net/uploads/201209/06/1346909180_7998.png)
下面言归正传，说一下宣讲会的情况。这里声明，都是我做的笔记的内容，不一定准确，一切数据都以Hulu官方发布的消息为准。我主要是想说一下自己的听后感。
每一个大公司有会有自己的个性口号，它可以说是公司文化的浓缩版。Hulu的Mission就是“Ambitious and Never-ending”。虽然宣讲时Hulu的人很自谦地说他们是个小公司，但是从它们的口号就可以感受到它们的魄力和决心。而且在场给出的几个数据更说明这个小公司颇像那种“人小鬼大”的精灵般的小孩子，非常受欢迎，也很有影响力。从2007年十月Hulu的beta版上线开始到目前为止，Hulu已经拥有了400+内容提供商、1000+广告提供商，注册用户有3千万，尤其注意美国的人口只有3亿，这样看来美国至少有十分之一的人是Hulu的注册用户，当然还有很多用户是没有注册而直接观看的，将这部分计算在内的话Hulu的用户就更多了。
由于某wall的原因，当时宣讲会上大部分同学都说没有访问过Hulu，所以大家往往认为Hulu和YouTube是一回事。其实据Hulu的人说，YouTube和Hulu的业务重点完全不同。YouTube的视频有很多来源于普通用户的上传，比如生活中一些搞笑的小片段等，但是Hulu的工作重心却是专业长视频，比如新闻呀美剧呀什么的。因此Hulu的竞争对手应该是AOL.Inc、MSN、ABC.com、CBS.com、NBC.com、Fox.com、Yahoo!等等。
今天来宣讲的是Hulu的北京团队。开始让我非常吃惊的是，Hulu没有中国业务，但是开发团队的很重要一部分都在北京。据说这是因为历史原因，因为Hulu创业之初便是购买了一个北京的技术开发团队，后来慢慢发展起来的。而且Hulu北京团队做的业务和美国团队的业务是不冲突的，不存在竞争关系，美国业务的很多内容以及日本业务的内容都是由北京团队来做，因此北京团队有很广阔的舞台和平台可以发挥。
北京团队所涉及的工作包括web player、site framework、device、search engine以及engineering infrastructure等等。Hulu希望能够打造一个360度的智能平台，让用户更舒心地观看视频。虽然访问不了美国的Hulu，但是据说www.hulu.jp是可以访问的。
在听宣讲的过程中，我着重关注了一下Hulu所使用的开发语言和技术。令我非常开心的是，Hulu把各种我心仪的前端技术用得炉火纯青。在Hulu，web前端开发绝对不是做做网页而已，而是包括模块解耦、序列化等很多很有技术含量的内容在里面。具体地，前端的技术有HTML5、CSS3、JavaScript、ActionScript3、ActionScript2，还有一个不常听说的东东叫coffee script。我到网上查了一下，coffee script是用Ruby写的类JavaScript的新语言，和jQuery结合之后威力无穷。感兴趣的同学可以读一下这两篇文章链接：[当jQuery遭遇CoffeeScript的时候——妙，不可言](http://www.cnblogs.com/filod/archive/2011/09/17/2179578.html)，[脚本语言
 CoffeeScript](http://www.oschina.net/p/coffeescript)。而后端的语言不是我们熟知的LAMP，而是它们的升级版：Linux+Ngnix+Memcached+Varnish，Linux+Apache+Memcached+Varnish，还有Python+Cherrypy，Ruby+Rails，Python+Cherrypy+Gunicorn+Sqlalchemy。当然对Java、C#也有应用，但通常是针对移动设备，再就是Object-C和C/C++。大家看这里面是不是有听都没听过的技术？反正我是再次感到自己要学习和努力的地方真是太多了。
上面提到的技术主要是web player和device team中使用的。尤其是介绍device team的那个人说他们device team曾经为了发布一个版本需要和16个team打交道，是非常非常锻炼一个人的技术水平和交流能力的一个地方。
做前端的时候我一向很关注页面的布局，因此对Hulu的布局也特意留心了一下。他们介绍说hulu是采用基于tray的UI布局，每个tray都是可配置的，这样就能及时将推荐系统计算所得视频内容推送到页面上的tray里面。而tray configuration便是前面提到的site framework的一部分。除此之外，site framework还包括AJAX page load、Tracking、content service、Image Service、Error Log and Alert。听后感到Ajax真不只是异步传输字面上那么简单，像这里就还用到了HTML5的history
 API。而那个Tracking则能记录用户的使用习惯，提供质量服务。最好玩的是那个Image Service，可以自动生成不同尺寸的图片，对前端开发人员而言真是太好用的工具了。
上面所介绍的都是Hulu的开发相关的内容。而下面就是Hulu的Research Lab做的工作了。这里做介绍的是大名鼎鼎的项亮，毕业于中科院的自动化所。早在星哥、多比他们参加KDD的时候就听说过这个大牛级人物了，高山仰止啊。通过他的介绍感到Hulu对于推荐系统真的很重视。Hulu这里研究的工作主要有下面几点（没有记全）：
- 全站个性化：据说目前完全做到这一点的只有Amazon和Netflix（说起这个公司，有个新闻是讲[自动化所研究生项亮在Netflix百万美元大奖赛中取得优异成绩](http://www.bjb.cas.cn/kytd/kjdt/200908/t20090805_2330119.html)）。
- 设计算法发掘可能流失的用户并有针对性地给予挽留。 
- Genomes：引入专家知识，深入分析视频内容。一看到这个“专家知识”几个字我特别兴奋，因为我之前做的本体也是基于专家知识的人工智能研究呀。 
- 场景分割 
- 人脸识别 
- de-blending：播放视频的时候我们时常会点暂停，但是如果恰好在两帧中间暂停的话，就会导致视频图像模糊，这就需要一定的处理。 
- AD-Targeting：有针对性地做广告。 
- Audience Platform：用Hadoop、Hbase、Mongo、Cassandra等技术对之前提到的算法研究与运算提供技术支持。
除了这些内容之外，正在发展的还有两项：
- 基于内容的广告推荐：设计思路大致是：字幕分析->音频分析->视频分析->情感分析 
- 广告理解性：理解广告的目的，从而做出更有吸引力的广告
Hulu宣讲会的主要内容就是这些。最后有很多同学提了各式各样的问题。大家找工作那份焦急的心情看来都是一样一样一样的啊。宣讲会真的对我们了解公司有好大的帮助，我都有点遗憾自己应该早一年听宣讲会的，然后才知道该如何选择方向努力啊，学习上会少走很多弯路。因此把宣讲会记录于此，供现在正在找工作和将来要找工作的同学参考。下面是宣讲会的照片，真是人山人海啊~
![](https://img-my.csdn.net/uploads/201209/06/1346913368_5542.png)
注：不知道把宣讲会的内容写在这里是不是可以？如果不可以的话，请Hulu的工作人员告知我，我立即撤下文章。
祝大家都找到满意的工作！
