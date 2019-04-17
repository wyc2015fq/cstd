# 实体链接关系链接：EARL模型 Joint Entity and Relation Linking QA over KG - ChihkAnchor的博客 - CSDN博客





2018年12月21日 18:18:55[Chihk-Anchor](https://me.csdn.net/weixin_40871455)阅读数：131标签：[EARL模型																[语义解析																[知识图谱问答](https://so.csdn.net/so/search/s.do?q=知识图谱问答&t=blog)
个人分类：[知识图谱																[问答系统](https://blog.csdn.net/weixin_40871455/article/category/8355978)](https://blog.csdn.net/weixin_40871455/article/category/8355979)





EARL：Joint Entity and Relation Linking for Question Answering over Knowledge Graphs

![](https://img-blog.csdnimg.cn/20181218220422917.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

基于知识图谱问答，是当前的一个研究热点，所谓问答就是输入一句自然语言，系统输出一个答案

基于语义解析的问答系统可以是完全基于规则的系统也可以是一个完全基于端到端的系统

语义问答系统当前面临的技术挑战有：1.实体识别与链接  2.关系识别与链接  3.问句意图识别  4.形式化查询语言的生成

目前在一些简单问题上的问答系统其准确率已经达到了不错的水平，但是对于复杂的问题就显得有些难以应对，这里简单问题是指在知识图谱上至多只有一个实体或者关系与问句匹配到，本文讨论的是复杂问题，即问句中有多个实体或关系

通常来说，为了正确的生成形式化的查询语言并且返回正确的答案，问句中的所有实体和关系需要被正确的链接到知识图谱上，（链接就是指问句中出现的实体或关系 与 知识图谱中的实体或关系 相对应，比如问句中的 “苹果” 需要和知识图谱中的 “苹果” 建立对应关系，那到底是水果的苹果还是苹果公司，这就需要我们的算法来解决了，也就是这篇论文讨论的问题）

EARL (Entity and Relation Linker)，是一个实体和关系的联合消歧系统（注：这里说的消歧就是链接消歧，问题中的 "苹果" 这个实体链接到知识图谱中的实体，你怎么知道是水果还是公司，这个存在歧义的，所以也称为消歧），它将实体链接和关系链接合在一起处理，EARL是一个单任务的系统，目标也很简单：减少每步相互依赖产生的误差。它运行在一个知识图谱之上，有关知识图谱的内容可以看我另外一篇[博客](https://blog.csdn.net/weixin_40871455/article/details/84074450)。

EARL通过观察实体周围的关系来给实体消歧，同样的，通过观察关系周围的实体为关系进行消歧，这里不明白没有关系，接着往下看，后面还会提到。EARL系统支持问题中出现的多个实体和关系（我们称这样的问题为复杂问题）的链接消歧，EARL模型提出了两种消歧方式：1.把实体关系联合链接任务看做是广义旅行商问题（GTSP  TSP）的一个例子，因为GTSP是一个np难问题，文章采用了一个近似解决方法；2.使用机器学习的方法去学习知识图谱中节点的连接密度，这种方法依赖三个基本特征并需要进行重新排名才能预测KG中的实体和关系

![](https://img-blog.csdnimg.cn/20181217172859247.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

上图，右图中的关系用节点表示，而非我们熟知的在边上标注关系 ，我们称为细分图。问题： "Where was the founder of Tesla and SpaceX born?" 这里我们就需要对Tesla进行消歧，是特斯拉公司还是科学家特斯拉？这如何进行消歧我们接着往下看

假设：我们这里有三个假设，通过我们的实验我们会证实这三个假设

1.从问题中提取的每个实体和关系，他们分别对应着KG中分别挑选的候选列表，所谓正确的解决方案就是：最小代价的遍历，即恰好访问每个列表中的一个候选项

2.给定来自问题的实体和关系的候选列表，与错误的候选集相比，正确的候选者在知识图中呈现相对密集和短跳的连接

3.与单独执行这些任务相比，将实体和关系联合起来可以获得更高的准确性。

若看不懂这些假设也没有关系，它们不是我们研究的前提，看完文章你就懂得这三个假设的含义了

## 一些定义：

Definition 1 (Graph 图). A (simple, undirected) graph is an ordered pair G = (V; E) V是实体定点的集合，E是连接实体的边的集合

Definition 2 (Knowledge Graph).we define a knowledge graph as a labelled directed multi-graph.多元有向图  KG = (V; E; L)  V是顶点的集合，L是所有边上的标签的集合，E被定义为E ⊆ V × L × V 

Definition 3 (Subdivision Graph). 细分图 S(G)  将定义1 中的Graph的边替换成节点，然后再将这个节点与刚刚没替换之前与边连接的两个节点相连。 S(G) obtained from G by replacing each edge e = (u; v) of G by a new vertex we and 2 new edges (u; we) and (v; we)

### EARL结构：

![](https://img-blog.csdnimg.cn/20181218211647762.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

总的来说，实体链接分为两步，第一步，从问句中识别实体，即找出实体在问句的起点和终点，第二步，消歧，就是将问句中的实体准确链接到知识图谱中，对于链接过程，是从图谱中产生一个候选项列表然后再选取最佳的那个实体进行链接。对于关系来说也是同样的步骤

实体/关系 是怎么生成的？

1.浅层分析：EARL使用SENNA作为关键词抽取器，同时在这个阶段会去掉问句中的停用词，"Where was the founder of Tesla and SpaceX born?" 会抽取出<founder, Tesla, SpaceX, born>

2.实体/关系 预测：有了关键词，EARL下一步要做的就是识别关键词是实体还是关系，这步使用LSTM网络，使用KG中的实体和关系作为标签进行训练，回到例子中就是"founder"   "born" 被识别为关系, "Tesla"   "SpaceX" 被识别为实体

候选列表生成：

对于每个识别出来的实体或关系都生成一个候选列表（每个实体和关系对应图谱上的多个节点，节点组成候选列表），为了检索识别出来候选列表中的最佳候选项，我们创建了<URI-label>的Elasticsearch索引，通过Wikipedia中的数据进行对应训练

通过以上步骤我们得到了：1.问句中的关键词   2.每个关键词被标注为实体或关系   3.每个关键词对应到多个候选的图谱中的节点的URI表示形式。

### 使用GTSP进行消歧：

实体和关系的联合消歧的目标是正确的找到各个候选列表中的节点，而这些节点在知识图谱中的距离必须很近，这样他们才有联系那多近才算近呢？我们有一个公式：![](https://img-blog.csdnimg.cn/20190304203514522.png)， 值越低说明关系越近，V和L请看定义2，为了方便我们计算实体和关系的距离我们将知识图谱转换成细节图，即将知识图谱中的边也改用节点来表示，细节图中实体和关系都表现为节点，然后再将有联系的实体和关系用边连起来（见定义3）

![](https://img-blog.csdnimg.cn/20181221175342837.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

如上图，我们从问句中识别出了四个关键词，然后根据算法找出知识图谱中的候选列表，每个列表称为节点团，这样我们转换成广义旅行商问题就是要找到上图中的一个子图V'，子图是由每个节点团只取一个节点组成，总体代价函数为 ![](https://img-blog.csdnimg.cn/20181221180454774.png)，这里我们并不要求子图形成一个环，同时我们也不要求节点团必须互斥，不同的关键词的候选节点可以重合

GTSP近似求解方法：Lin–Kernighan–Helsgaun 算法

### 根据连接密度计算：

我们有n个候选列表：![](https://img-blog.csdnimg.cn/20181221181209945.png)，![](https://img-blog.csdnimg.cn/20181221181306483.png)∈ Li，m是每个关键词对应候选列表中候选元素的个数，i 是第i个候选列表

![](https://img-blog.csdnimg.cn/20181221181432599.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

hop distance：![](https://img-blog.csdnimg.cn/20190305114048471.png)，指的是候选项![](https://img-blog.csdnimg.cn/20190305114133695.png)到![](https://img-blog.csdnimg.cn/2019030511414796.png)的跳数，在细分图中从![](https://img-blog.csdnimg.cn/20190305114133695.png)到![](https://img-blog.csdnimg.cn/20190305114243355.png)需要遍历的边的个数为h，那么![](https://img-blog.csdnimg.cn/20190305114319108.png)，引入dConnect，其计算方式：

![](https://img-blog.csdnimg.cn/20190305114420883.png)

![](https://img-blog.csdnimg.cn/20190305114642546.png)：

![](https://img-blog.csdnimg.cn/20190305114613980.png)

![](https://img-blog.csdnimg.cn/20190305114707498.png)：

![](https://img-blog.csdnimg.cn/20190305114732390.png)

有了以上两个参数![](https://img-blog.csdnimg.cn/20190305114844589.png)，再加上初始的候选列表排名![](https://img-blog.csdnimg.cn/20190305114910344.png)，我们就可以组成一个特征空间![](https://img-blog.csdnimg.cn/20190305114932946.png)，用这个特征空间我们可以重新计算候选列表的排名![](https://img-blog.csdnimg.cn/20190305115144231.png)，有了![](https://img-blog.csdnimg.cn/20190305115156922.png)我们取每个列表的第一项，或者前几项，就可以组成一个或多个子图，这就是所谓的实体和关系链接到知识库，我们可以拿着这个子图进行子图匹配完成问答

连接密度算法：

![](https://img-blog.csdnimg.cn/20190305115434181.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)](https://so.csdn.net/so/search/s.do?q=语义解析&t=blog)](https://so.csdn.net/so/search/s.do?q=EARL模型&t=blog)




