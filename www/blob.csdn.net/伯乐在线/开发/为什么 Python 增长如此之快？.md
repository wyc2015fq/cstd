# 为什么 Python 增长如此之快？ - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [艾凌风](http://www.jobbole.com/members/hanxiaomax) 翻译。未经许可，禁止转载！
英文出处：[stackoverflow](https://stackoverflow.blog/2017/09/14/python-growing-quickly/?cb=1)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
【伯乐在线导读】：根据 Stack Overflow 最近公布的报告，基于 SO 上不同问题帖的访问量统计，Python 可以说是增长最快的主流编程语言。Python 也是高收入国家访客最爱浏览的标签。一起看看 SO 数据科学家 David Robinson 的分析。
为什么 Python 增长的这么快？它有很多的用途，从 Web 开发到数据科学，再到 DevOps，而且你要知道，现在有越来越多的应用是使用 Python 开发的。我是一个用 R 语言的数据科学家，所以我自然很想知道，在我这个领域，Python 已经增长到了什么程度。还有哪些公司和组织最喜欢用 Python。
我们分析得出了以下两个结论。首先，快速增长的 Python 主要是因为数据科学、机器学习以及学术研究。这一点可以从 pandas 软件包的增长上看出，pandas 已经成为增长最快的与 Python 相关的标签了。至于哪些行业在使用Python，我们发现如下行业对其最为关注，比如电子、制造业、软件、政务，尤其是高校。但是，Python 在各个行业的增长几乎是差不多的。这也恰好说明了，数据科学和机器学习对于各行各业来说，变得越来越常见，而且 Python 在这两个领域已经成为了最为普遍的选择。
与之前一样，本文的分析也只针对世界银行所公布的高收入国家进行。
# Python能够做哪些工作？
Python 是一个用途广泛的语言，比如说web开发和数据科学。我们是如何理清Python在这些领域的增长的呢？
每个领域都有一些比较著名的库，我们可以研究一下这些库所对应标签的访问量。我们可以比较 web 开发框架 Django 和 Flask 以及数据科学领域的 NumPy，matplotlib 和 pandas （你可以使用 Stack Overflow Trends 来比较某个问题被问到的比率，而不是仅仅是访问量）
![](https://zgab33vy595fw5zq-zippykid.netdna-ssl.com/wp-content/uploads/2017/09/related_tags_over_time-1-1024x1024.png)
对于来自高收入国家的访问量，padnas 是增长最快的Python软件包：它于2011年问世，而现在，Stack Overflow 中有 1％ 的问题都是关于它的。关于 numpy 和 matplotlib 的问题也在不断增长。与之相对的是，与 Django 相关问题的增长比较平缓，Flask 虽然有所增长，但仍旧比较小众。这表明，大多数 Python 问题的的增长主要还得归功于数据科学，而不是 Web 开发。
但是，这只是一方面，因为它只能测量常用的 Python 库。Python 在系统管理员和 DevOps 工程师中也非常的流行，他们会同时访问Linux，Bash 和 Docker 等领域的问题。同样，很多 Python web 应用并不是基于 Django 或者 Flask 的，这些开发者很可能去访问 JavaScript、HTML 和 CSS ，而我们不能简单的计算 Linux、bash、javascript 这些标签的增长并且假定它们和Python相关。因此，我们想要计算一下，与 Python 标签一起访问的标签有哪些。
我们仅考虑今夏（2017年 6-8 月期间）的访问量，这样做可以减小学生们带来的流量影响，专注于最近的流量数据，同时还可以减少计算量。此外，我们仅考虑注册用户，且该用户在这段时间内应该至少访问过50个问题。我们基于两点判断一个用户是Python使用者：第一，他最多访问的标签是Python，第二，Python问题在他浏览的所有问题中，占比至少要达到20%。
**浏览Python标签的人，通常还会浏览什么标签呢？**
![](https://zgab33vy595fw5zq-zippykid.netdna-ssl.com/wp-content/uploads/2017/09/common_other_tags-1-1024x1024.png)
对于一个 Python 开发者来说，访问最多的标签是 Pandas（优势非常明显），这点我们丝毫不感到意外。访问量第二多的标签是JavaScript，这些流量代表了使用 Python 做 Web 开发的程序员。这证实了我们的猜想：我们应该考虑的是与Python标签一起访问的那些标签，而不仅仅是与 Python 相关的标签的增长（这两者是有区别的）。
我们还能看到其他的一些“技术簇”（“clusters” of technologies）。这些聚在一起的标签，表现出了一定的相关性，借此我们可以分析出它们之间的关系：比如说Python程序员是不是更加倾向于访问某些特定的标签。通过过滤出一组具有较高的皮尔森相关系数（ Pearson correlation）的标签，我们可以在图表中看到三组关系：
![](https://zgab33vy595fw5zq-zippykid.netdna-ssl.com/wp-content/uploads/2017/09/tag_network_graph-1-1024x1024.png)
我们可以看到一些较大的“技术簇”，它表明了Python可以解决的一类问题。在图片的中上部分我们可以看到有关数据科学和机器学习相关的内容：有pandas，NumPy和Matplotlib，并且你可以发现它们和R语言，Keras以及TensorFlow息息相关。它下面的一个“技术簇”表现了有关web开发领域的一些关系，这里有JavaScript, HTML, CSS, Django, Flask 和 JQuery。另外还有两个“技术簇”，左边是与系统管理／DevOps相关的（以linux和bash为中心），右边是与数据工程相关的（包括Spark, Hadoop, 和 Scala）
## 话题的增长
我们发现，Python相关的访问流量可以被归纳为一些特定的话题。这使得我们可以获知，Python访问量的增长，与哪些话题息息相关。
想象一下，我们正在研究一个用户的历史纪录并发现Python是他最常访问的标签。我们怎么才能进一步判断出这个用户的细分属性呢？是web开发者？数据科学家？系统管理员？还是其他角色？为了达成这一目标，我们还需要去研究他访问量第二多的标签，第三多的标签，以此类推。沿着这条路走下去，最终我们会在上述“技术簇”模型中，找到属于这位程序员的位置。
因此，我们提出了一种简单的方法，通过对以下九个标签的访问统计 ，我们将某个用户归类到某个话题中。
- **数据科学**：Pandas, NumPy 或 Matplotlib
- **Web 开发者**：JavaScript, Django, HTML
- **系统管理员／DevOps**：Linux, Bash 或 Windows.
- **无法归类**：上述 9 个标签的访问量，没有任何一个能够占据该用户5%以上的流量。
虽说并不是特别精确，但是该方法可以让我们快速的估计出每个领域对Python增长的影响力。我们也尝试使用了更加严谨的计算手段，比如说潜在狄利克雷分布（Latent Dirichlet Allocation），得到的结果是相似的。
**上述哪种类别的Python开发者越来越常见了呢？**
考虑到我们现在主要研究的是用户而不是问题，我们首先为您展示Stack Overflow全体注册用户访问上述话题的比例变化趋势（不论其是否访问了Python话题）
![](https://zgab33vy595fw5zq-zippykid.netdna-ssl.com/wp-content/uploads/2017/09/user_categories_plot-1-1024x731.png)
我们可以看到，Web开发和系统管理员这两大类在过去的三年中增长平缓，但是Python程序员访问数据科学相关技术的占比却飞速增长。这就说明，Python的流行主要还是由数据科学以及机器学习驱动的。
通过计算Python程序员在2016和2017两年中访问某个标签点流量，我们还可以研究一下每个单独标签的增长水平。比方说，可能出现的情况是：Javascript标签的流量总体平稳，但若仅考虑Python程序员对此标签的访问，Javascript标签的访问占比可能会下降。当我们计算出每个标签的增长速率后，我们可以在网络图表中展现出某个话题的增长或萎缩。
![](https://zgab33vy595fw5zq-zippykid.netdna-ssl.com/wp-content/uploads/2017/09/tag_network_graph_color-1-1024x1024.png)
这些工作有利于证明我们的猜想：Python的增长主要和数据科学以及机器学习相关，图表中大多数的技术簇都在向橙色演变，这说明这些标签开始成为Python生态系统的主角了。
## 来自工业界的数据
理解Python的增长还有一个途径，那就是考察一下访问它的流量都来自哪些类型的公司。
我们仅关注Python增长速度最为显著的两个国家：美国和英国。在这俩国家中，我们可以将流量按照行业归类（就像我们对比AWS和Azure一样）
![](https://zgab33vy595fw5zq-zippykid.netdna-ssl.com/wp-content/uploads/2017/09/by_industry_graph-1-1024x1024.png)
访问Python最多的是学术界（优势明显），包括各类科研院校。是因为为本科生开了Python课么？
可能有这方面的原因，但绝不是全部的原因。在上一篇文章中我们看到，Python的流量在夏季的时候也很多，不仅仅是春秋两季。比如，Python和Java是高校最常访问的标签，我们可以看出两者的季节性变化是不同的。
![](https://zgab33vy595fw5zq-zippykid.netdna-ssl.com/wp-content/uploads/2017/09/percent_academic_by_month_plot-1-1024x1024.png)
我们可以看到，Java在夏季访问量下降的非常多，因为Java是最常见的本科课程。与之相对的是，Python在夏季流量的占比非常高。来自高校的Python的高访问量主要是来自科研人员，他们整年都在工作。这也为我们的猜想提供了更多的证据，Python的增长主要是因为它强大的科学计算能力和数据分析能力。
至于其他行业，我们发现Python在政府部门非常流行，增长迅速。同时，Python在电子和制造业也被广泛使用。我对这些行业不太了解，很想了解它们是如何使用Python的。在零售业和保险业，Python还不是主流，一些研究表明，Java在这些行业仍然占据统治地位。
**本文主要研究的是Python的增长。那么，Python在某个行业的增长速度，是否明显高于其他行业呢？**
![](https://zgab33vy595fw5zq-zippykid.netdna-ssl.com/wp-content/uploads/2017/09/industry_growth_plot-1-1024x1024.png)
过去一年里，Python在各行业的增长几乎是差不多的，至少在英美两国是这样。各个行业访问Python的流量增长了2-3个百分点（对于原本使用Python不多的行业，这样的增长是非常显著的，比如之前提到的保险和零售业）
在大部分行业中，Java仍然是最多访问的标签（2017年截止到目前的最新数据），但是Python进步明显。比如，在金融领域（该领域是SO站点访问量占比较大的行业之一），Python的访问量从2016年排名第四升至2017年的排名第二。
## 结论
作为一个数据科学家，我以前使用 Python 但是现在用 R 语言工作，这些研究是否会让我重回 Python 的怀抱？
我并不这样认为，一方面，R 语言增长的同样很快；在上一篇文章里面我们可以看到，R 语言是增长速度第二快的编程语言，仅次于Python。我更倾向于使用 R 语言进行数据分析，这和它的流行度并没有关系。（我准备写一篇个人博客，讲讲我为何从 Python 迁移到R，为何两种语言我都喜欢但却不愿意重回 Python 的怀抱）
不论如何，数据科学是一个令人激动，增长迅速的领域，这一领域空间很大，还能容得下很多语言，我鼓励开发者们在他们职业生涯的早期应该考虑去学习一些数据科学的技能。数据科学是软件开发生态系统中增长最为迅速的领域，并且越来越多的渗透到各行各业中。
### SO 原文的读者评论精选：
onemerlin ： 作者对Python在电子和制造业的增长有兴趣，我是一个嵌入式老兵，我来给大家介绍一下。在电子行业，以前测试工具都是C/C++/Java写的，和脚本语言比起来，维护它们代价太大，有了Python，我们的硬件工程师只需要借助一些 I/O库就可以快速的编写一些脚本了。此外，以前用于布线和仿真的CAD软件都适用 TCL 编程，和 Python 语言相比，TCL太反人类了。
Robert Mennell： 我很好奇，如果说一个人使用Python是为了它和C语言交互的能力，为什么不用NodeJs呢（不知道还可以用NodeJs来封装么？）我见过很多优秀的NodeJs程序员，和C++程序员合作，使用javascrip进行硬件交互。这些人都晋升的比较快，没时间访问stack overflow，相反，那些不知道Nodejs可以做这些事情的科研人员才老喜欢到Stack Overflow搜搜搜。
jdenvir ： 我是一个数学家、程序员，工作是一个生物医学家。生物和生物医学最近一二十年向着计算方向发展，我必须要教那些20几岁的博士去学习编程，我是一个Java程序员，但是Java不适合这些初学者，R语言感觉有点怪，除非你是个统计学家。Python正好弥补了这些，容易上手，足够强大可以去帮助我们研究真正的科学。
Mike ： “为什么Python（蟒蛇）增长的这么快？”“我不晓得，不过我建议你打电话给虫害管理机构，等它们把你吞掉了就来不及了”
（Mike，你是Reddit派来的逗比么？）
> 
**打赏支持我翻译更多好文章，谢谢！**
[打赏译者](#rewardbox)
#### 打赏支持我翻译更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/05/d3704dca07a3c6ce19c14a9762a0d776.jpg)![](http://www.jobbole.com/wp-content/uploads/2016/03/d8aa159efcd108a908cd3d6ab74b8f5f.jpg)
