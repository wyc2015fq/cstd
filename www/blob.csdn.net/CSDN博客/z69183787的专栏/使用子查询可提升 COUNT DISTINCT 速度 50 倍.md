# 使用子查询可提升 COUNT DISTINCT 速度 50 倍 - z69183787的专栏 - CSDN博客
2016年06月22日 11:15:43[OkidoGreen](https://me.csdn.net/z69183787)阅读数：9418
|注：这些技术是通用的，只不过我们选择使用Postgres的语法。使用独特的[pgAdminIII](http://www.pgadmin.org/)生成解释图形。### 很有用，但太慢Count distinct是SQL分析时的祸根，因此它是我第一篇博客的不二选择。首先：如果你有一个大的且能够容忍不精确的数据集，那像[HyperLogLog](http://algo.inria.fr/flajolet/Publications/FlFuGaMe07.pdf)这样的概率计数器应该是你最好的选择。（我们会在以后的博客中谈到HyperLogLog。）但对于需要快速、精准答案的查询，一些简单的子查询可以节省你很多时间。|![Garfielt](http://static.oschina.net/uploads/user/73/147631_50.jpg?t=1370742483000)###### [Garfielt](http://my.oschina.net/Garfielt)翻译于 2年前3人顶顶 翻译的不错哦!|
|让我们以我们一直使用的一个简单查询开始：哪个图表的用户访问量最大？[?](http://www.oschina.net/translate/use-subqueries-to-count-distinct-50x-faster#)````select``  ``dashboards.``name``, ``  ``count``(``distinct``time_on_site_logs.user_id)``from``time_on_site_logs ``join``dashboards ``on``time_on_site_logs.dashboard_id = dashboards.id``group``by``name``order``by``count``desc````首先，我们假设user_id和dashboard_id上已经设置了索引，且有比图表和用户数多得多的日志条目。一千万行数据时，查询需要48秒。要知道原因让我们看一下SQL解析：![Explain Slow](http://static.oschina.net/uploads/img/201401/25075015_4ouo.png)它慢是因为数据库遍历了所有日志以及所有的图表，然后join它们，再将它们排序，这些都在真正的group和分组和聚合工作之前。|1234567|`select``  ``dashboards.``name``, ``  ``count``(``distinct``time_on_site_logs.user_id)``from``time_on_site_logs ``join``dashboards ``on``time_on_site_logs.dashboard_id = dashboards.id``group``by``name``order``by``count``desc`|![Garfielt](http://static.oschina.net/uploads/user/73/147631_50.jpg?t=1370742483000)###### [Garfielt](http://my.oschina.net/Garfielt)翻译于 2年前3人顶顶 翻译的不错哦!|
|----|----|----|----|
|1234567|`select``  ``dashboards.``name``, ``  ``count``(``distinct``time_on_site_logs.user_id)``from``time_on_site_logs ``join``dashboards ``on``time_on_site_logs.dashboard_id = dashboards.id``group``by``name``order``by``count``desc`| | |
首先，我们假设user_id和dashboard_id上已经设置了索引，且有比图表和用户数多得多的日志条目。
一千万行数据时，查询需要48秒。要知道原因让我们看一下SQL解析：
![Explain Slow](http://static.oschina.net/uploads/img/201401/25075015_4ouo.png)
它慢是因为数据库遍历了所有日志以及所有的图表，然后join它们，再将它们排序，这些都在真正的group和分组和聚合工作之前。
![Garfielt](http://static.oschina.net/uploads/user/73/147631_50.jpg?t=1370742483000)
###### [Garfielt](http://my.oschina.net/Garfielt)翻译于 2年前
3人顶
顶 翻译的不错哦!
|### 先聚合，然后Joingroup-聚合后的任何工作代价都要低，因为数据量会更小。group-聚合时我们不需使用dashboards.name，我们也可以先在数据库上做聚集，在join之前：[?](http://www.oschina.net/translate/use-subqueries-to-count-distinct-50x-faster#)````select``  ``dashboards.``name``,``  ``log_counts.ct``from``dashboards``join``(``  ``select``    ``dashboard_id,``    ``count``(``distinct``user_id) ``as``ct``  ``from``time_on_site_logs ``  ``group``by``dashboard_id``) ``as``log_counts ``on``log_counts.dashboard_id = dashboards.id``order``by``log_counts.ct ``desc````现在查询运行了20秒，提升了2.4倍。再次通过解析来看一下原因：![Explain Faster](http://static.oschina.net/uploads/img/201401/25075027_Bx6W.png)正如设计的，group-聚合在join之前。而且，额外的我们可以利用time_on_site_logs表里的索引。|12345678910111213|`select``  ``dashboards.``name``,``  ``log_counts.ct``from``dashboards``join``(``  ``select``    ``dashboard_id,``    ``count``(``distinct``user_id) ``as``ct``  ``from``time_on_site_logs ``  ``group``by``dashboard_id``) ``as``log_counts ``on``log_counts.dashboard_id = dashboards.id``order``by``log_counts.ct ``desc`|![Garfielt](http://static.oschina.net/uploads/user/73/147631_50.jpg?t=1370742483000)###### [Garfielt](http://my.oschina.net/Garfielt)翻译于 2年前2人顶顶 翻译的不错哦!|
|----|----|----|----|
|12345678910111213|`select``  ``dashboards.``name``,``  ``log_counts.ct``from``dashboards``join``(``  ``select``    ``dashboard_id,``    ``count``(``distinct``user_id) ``as``ct``  ``from``time_on_site_logs ``  ``group``by``dashboard_id``) ``as``log_counts ``on``log_counts.dashboard_id = dashboards.id``order``by``log_counts.ct ``desc`| | |
现在查询运行了20秒，提升了2.4倍。再次通过解析来看一下原因：
![Explain Faster](http://static.oschina.net/uploads/img/201401/25075027_Bx6W.png)
正如设计的，group-聚合在join之前。而且，额外的我们可以利用time_on_site_logs表里的索引。
![Garfielt](http://static.oschina.net/uploads/user/73/147631_50.jpg?t=1370742483000)
###### [Garfielt](http://my.oschina.net/Garfielt)翻译于 2年前
2人顶
顶 翻译的不错哦!
|### 首先，缩小数据集我们可以做的更好。通过在整个日志表上group-聚合，我们处理了数据库中很多不必要的数据。Count distinct为每个group生成一个哈希——在本次环境中为每个dashboard_id——来跟踪哪些bucket中的哪些值已经检查过。我们可以预先计算差异，而不是处理全部数据，这样只需要一个哈希集合。然后我们在此基础上做一个简单的聚集即可。[?](http://www.oschina.net/translate/use-subqueries-to-count-distinct-50x-faster#)````select``  ``dashboards.``name``,``  ``log_counts.ct``from``dashboards ``join``(``  ``select``distinct_logs.dashboard_id, ``  ``count``(1) ``as``ct``  ``from``(``    ``select``distinct``dashboard_id, user_id``    ``from``time_on_site_logs``  ``) ``as``distinct_logs``  ``group``by``distinct_logs.dashboard_id``) ``as``log_counts ``on``log_counts.dashboard_id = dashboards.id``order``by``log_counts.ct ``desc````我们采取内部的count-distinct-group，然后将数据拆成两部分分成两块。第一块计算distinct (dashboard_id, user_id) 。第二块在它们基础上运行一个简单group-count。跟上面一样，最后再join。![Explain Fastest](http://static.oschina.net/uploads/img/201401/25075033_PVf5.png)|123456789101112131415|`select``  ``dashboards.``name``,``  ``log_counts.ct``from``dashboards ``join``(``  ``select``distinct_logs.dashboard_id, ``  ``count``(1) ``as``ct``  ``from``(``    ``select``distinct``dashboard_id, user_id``    ``from``time_on_site_logs``  ``) ``as``distinct_logs``  ``group``by``distinct_logs.dashboard_id``) ``as``log_counts ``on``log_counts.dashboard_id = dashboards.id``order``by``log_counts.ct ``desc`|![Garfielt](http://static.oschina.net/uploads/user/73/147631_50.jpg?t=1370742483000)###### [Garfielt](http://my.oschina.net/Garfielt)翻译于 2年前2人顶顶 翻译的不错哦!|
|----|----|----|----|
|123456789101112131415|`select``  ``dashboards.``name``,``  ``log_counts.ct``from``dashboards ``join``(``  ``select``distinct_logs.dashboard_id, ``  ``count``(1) ``as``ct``  ``from``(``    ``select``distinct``dashboard_id, user_id``    ``from``time_on_site_logs``  ``) ``as``distinct_logs``  ``group``by``distinct_logs.dashboard_id``) ``as``log_counts ``on``log_counts.dashboard_id = dashboards.id``order``by``log_counts.ct ``desc`| | |
我们采取内部的count-distinct-group，然后将数据拆成两部分分成两块。第一块计算distinct (dashboard_id, user_id) 。第二块在它们基础上运行一个简单group-count。跟上面一样，最后再join。
![Explain Fastest](http://static.oschina.net/uploads/img/201401/25075033_PVf5.png)
![Garfielt](http://static.oschina.net/uploads/user/73/147631_50.jpg?t=1370742483000)
###### [Garfielt](http://my.oschina.net/Garfielt)翻译于 2年前
2人顶
顶 翻译的不错哦!
呵呵，大发现：这样只需要0.7秒！这比上面的查询快28倍，比原来的快了68倍。
通常，数据大小和类型很重要。上面的例子受益于基数中没多少换算。distinct (user_id, dashboard_id)相对于数据总量来说数量也很少。不同的对数越多，用来group和计数的唯一数据就越多——代价便会越来越大。
下一遇到长时间运行的count distinct时，尝试一些子查询来减负吧。
