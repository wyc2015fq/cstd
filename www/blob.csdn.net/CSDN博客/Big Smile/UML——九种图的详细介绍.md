# UML——九种图的详细介绍 - Big Smile - CSDN博客
2015年09月19日 20:39:17[王啸tr1912](https://me.csdn.net/tr1912)阅读数：1021
看完了UML视频，就到了画图阶段，但是究竟都有哪些图要画，他们的要点是什么呢，下面将一一道来。
# 一、UML九种图的作用
![](https://img-blog.csdn.net/20151004174700330?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
UML图在整个软件的周期中占据着不小的用途，了解他们的用处也是我们呀掌握的事情，有了上面的图各种图的作用就清楚多了。
# 二、分图介绍
## （一）、用例图
![](https://img-blog.csdn.net/20151004180355054?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
上图为用例图的基本元素和关系，其中人为参与者，用例为系统的功能，子系统为该系统调用的子系统，其中最重要的就是这几种关系。
         其中包含和扩展只是用例之间的关系，而其他的三个可以是参与者与用例之间的关系也可以是用例与用例之间的关系。
讲一下包含和扩展（关联和依赖在类图中总结）：
包含：就是指的一个用例中所包含的用例（功能），没有了这个功能，这个用例便没有办法实现。
扩展：就是指当前用例中非必须的子用例（功能），没有了这个功能，这个用例还是可以实现的，这个功能只是辅助的作用。
## （二）、类图/对象图
由于类和对象图所用的图例和关系都差不多，所以在此一并描述
**1.四大关系：**
1）、依赖：两个事物之间的语义关系，其中一个事物发生变化会影响另一个事物的主义。简单地，两个类互为依赖关系，一个类的变化会影响另一个类，因此依赖总是单方面的。
2）、关联：有两种情况。聚合和组合，都是描述一组对象之间连接的结构关系。聚合，强的关联关系，是整体和部分的结构关系，整体和部分可独立，如车与零部件。组合，更强的关联关系，整体和部分同一生命周期，不可独立。如人与躯干。
3）、泛化：一般化——特殊化的关系。比如类的继承。
4）、实现：两个类，其中的一个类指定了由另一个类保证执行的契约。可以看成是接口实现的关系。
![](https://img-blog.csdn.net/20151004182144737?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
上图为几种关系的线
**2.图例**
**![](https://img-blog.csdn.net/20151004182124685?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
这其中包括了包的图例，因为包图就是依赖于类图而生的，包图的意义就是把不同功能的类划分在不同功能区内，包又是功能空间。包把建立的各种模型组织起来，形成各种功能或用途的模块，并可以控制包中元素的可见性以及描述包之间的依赖关系。通过这种方式，系统模型的实现者可在高层把握系统的结构。所以包图在分类上来讲就是类图的整体层面上的分类。
    画类图的时候最重要的就是从需求中把类抽象出来，其中可以从不同的角度来抽象，但是最关键的是找相同的关键词，这样的有相同关键词的对象就是一个类了。
## （三）、状态图和活动图
![](https://img-blog.csdn.net/20151004184908562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        状态图要注意的就是每个点都是一个状态，主要是以对象的状态变化为基准来形成的图，其中要把握好状态的时机和关系。
![](https://img-blog.csdn.net/20151004184957014?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
      活动图和状态图的不同之处是它体现的是每个动作所在的功能之间的关系，其中涉及到了功能执行的前后的关系，还有泳道图的用法，泳道图每个泳道的标题写入的都是功能，其中的动作是夸功能而实现的。
## （四）、序列图与协作图
![](https://img-blog.csdn.net/20151004185934586?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
     以上序列图图片转自于亮师哥的博客，感觉画的很不错，只是还有些东西没有表现出来。下面是我补充的对象和连线：
![](https://img-blog.csdn.net/20151004191202902?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       这些片段是存在于序列图中的，现在我们还不怎么常用，其实用法和他们的名字是差不多的，就是在生命线上存在的片段，里面可以放一些激活的动作的。这四中消息的传递也就是序列图的核心，所有的激活的动作都是靠他们来完成的，画图的时候要注意他们的用法。
协作图：
![](https://img-blog.csdn.net/20151004191559798?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       从他们都有序号的动作来看，其实序列图和协作图本来就是可以相互转化的，他们的区别主要在于生命线的使用上，生命线使得序列图比较侧重于时序的动作发生过程，而协作图则侧重于对象之间的关系，从而表示对象的交互作用，序列图主要用在表示设计方案上，而协作图主要用在详细设计上。
## （五）、部件图和部署图
       部件图主要表示的是软件的各种支持文件或者系统之间的联系，没有什么好说的，上图：
![](https://img-blog.csdn.net/20151004192401745?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
部署图为物理设备的安装部署的相关原理图，主要是给安装和维护物理系统的人员使用的，它只表示机器之间的关系
![](https://img-blog.csdn.net/20151004192536078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# 总结：
    这次UML的学习也算是对软件工程学习的一个补充，它在方方面面都体现了面向对象的设计的特点，主要针对把抽象的结构变成可见的模型。UML的学习在很大程度上都是一次次软件工程的体验，但是我确没有怎么学明白，即时是这样，相信在绘图的道路上还会在回来的。
