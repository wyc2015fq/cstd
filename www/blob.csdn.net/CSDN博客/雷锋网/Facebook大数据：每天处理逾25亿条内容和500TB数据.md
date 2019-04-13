
# Facebook大数据：每天处理逾25亿条内容和500TB数据 - 雷锋网 - CSDN博客


2012年08月23日 15:35:51[leiphone](https://me.csdn.net/leiphone)阅读数：1409


![](http://www.leiphone.com/wp-content/uploads/2012/08/facebook-big-data-numbers.jpg)当地时间今日，Facebook在加州总部向几位记者透露了一些关于“大数据”的统计数字，诸如[Facebook](http://www.leiphone.com/tag/facebook)系统每天要处理25亿条消息、500+
 TB的数据、用户点击Like按钮的次数达到27亿次、上传3亿张照片、每半个小时扫描的数据大约为105TB。另外Facebook还首次透露了新项目“Project Prism”的有关细节。
Facebook基础设施技术部门副总裁Jay Parikh称，这些数据对于Facebook来说尤为重要。通过快速处理这些数据，Facebook能够推出新产品，知晓用户反应以及近乎实时地调整产品设计。
![](http://www.leiphone.com/wp-content/uploads/2012/08/life-of-data-at-facebook.jpg)
Facebook透露的另外一项统计数字显示：在一个单独的Hadoop磁盘集群内存储了超过100 PB的数据，Parikh表示这是全球最大的单一Hadoop系统。不过他同时指出，虽然这种数据规模对于小企业来说很庞大，但是几个月之后就没有人会在乎你的数据库内存储了100PB的数据。因为数据增长的速度非常之快，而且我们对数据变得愈加渴求，所以再过几个月，100 PB的磁盘集群就再也不是新闻了。
另外，Parikh还称，这些数据不仅仅只对Facebook有帮助，广告商同时也是受益者。Parikh解释道：“通过追踪网站上发布的广告对各个层面用户（性别、年龄、兴趣爱好）的影响，我们可以有针对性地加大广告力度，使其效果更加明显。打个比方，如果广告效应在加州相比其他地方更好，我们就会在加州投放更多的广告，从而让广告商的效益达到最大化。
Facebook甚至都不需要作出任何改变就能看到这些数据带来的影响。只要透过历史数据，Faceboo就可以建立一个模型，然后进行数据模拟，就能看到广告点击率（CTR）成倍增长。与此同时，还有一个叫做Gatekeeper的系统在测试少数比例的用户群的数据所带来的变化。
![](http://www.leiphone.com/wp-content/uploads/2012/08/facebook-big-data.jpg)
接下来谈论的是新项目“Project Prism”。现在Facebook实际上是将其所有的用户数据库（一直在不断变化增大）存储在某个特定的数据中心，其他数据中心则用来存储其他数据和冗余数据。不过随着用户数据库不断增大，一个数据中心将不足以存储所有的数据，那么就需要将整个用户数据库转移到更大的数据中心去。整个数据的转移过程其实也是一种资源浪费。
Parikh说：“Project Prism”让我们可以对这个“巨大仓库”（指用户数据库）进行分别存储但是依然不会影响整个数据视图，也就是意味着这些数据可以分别托管在Facebook在加州、弗吉尼亚州、俄勒冈州、北卡罗来纳州甚至是瑞典等地的数据中心。
在内部，Facebook选择不对数据进行分区处理或者给不同业务部门（如广告部门和客户支持服务部门）之间设置障碍。产品开发人员可以跨部门查看数据，以评估他们作出的小调整是否会增加用户在网站的停留时间、是否会引发用户的投诉或者是否增加广告的点击次数。
这样一来，作为用户，想到Facebook的员工可以对自己的活动了如指掌，肯定会感到丝丝的不安。但Facebook承诺用户，将会采取多重保护措施以免用户的数据被滥用。所有的数据访问记录会被Facebook记录下来，这样就可以追踪哪些员工查看了哪些数据。并且Facebook还会对员工进行强化训练，每个人都有各自管辖的数据领域，如果员工越权偷看了不该看的数据，那么将会被炒鱿鱼。Parikh严正声明：“我们采取的是零容忍政策，绝对不姑息任何非法使用用户数据的情况发生。”
Via[ TC ](http://techcrunch.com/2012/08/22/how-big-is-facebooks-data-2-5-billion-pieces-of-content-and-500-terabytes-ingested-every-day/)

**（****[Danice](http://www.leiphone.com/author/danice)****供****雷锋网****专稿，转载请注明来自雷锋网及作者，并链回本页)**

