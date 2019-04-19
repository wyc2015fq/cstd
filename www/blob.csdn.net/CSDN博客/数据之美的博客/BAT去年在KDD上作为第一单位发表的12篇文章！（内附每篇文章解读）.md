# BAT去年在KDD上作为第一单位发表的12篇文章！（内附每篇文章解读） - 数据之美的博客 - CSDN博客
2019年03月22日 13:33:56[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：125
个人分类：[技术资料																[paper](https://blog.csdn.net/oppo62258801/article/category/8784073)](https://blog.csdn.net/oppo62258801/article/category/8784069)
转自 [http://www.eeworld.com.cn/mp/QbitAI/a58948.jspx](http://www.eeworld.com.cn/mp/QbitAI/a58948.jspx)，如有侵权，请及时联系我
如今，全世界每天都有几十亿人在使用计算机、平板电脑、手机和其它数字设备产生海量数据。各个行业和领域都已经被数据给渗透，数据已成为非常重要的生产因素的大数据时代，数据挖掘这一领域俨然引起了国际、国内工业界的广泛关注。
本文将为大家介绍国内工业界在数据挖掘方面的研究工作（主要来源于发表在数据挖掘顶级国际会议KDD上的相关论文）。这些工作既包括理论性的研究也包括一些实际应用的研究工作。
下面首先以阿里巴巴、腾讯、百度为例简单介绍一下国内工业界在数据挖掘领域顶级国际会议KDD上发表的研究工作。
# 阿里巴巴（Alibaba）
阿里巴巴在电子商务方面做了大量的数据挖掘研究。尤其是在表示学习和增强学习做了几个很有意思的工作。2018年阿里巴巴在数据挖掘顶级国际会议KDD上作为第一作者单位一共发表了8篇论文，我们将先为大家简要解读其中的部分文章。（论文列表附在其后，表里还包括了部分2017和2016文章）
![](http://6.eewimg.cn/mp/uploads/2019/01/02/3953d480-0e70-11e9-91b7-00163e2e672a.jpg)
**《Deep Reinforcement Learning for Sponsored Search Real-time Bidding》**
> 
该论文介绍了阿里巴巴展示广告里面的实时投标系统的设计与实现。
以前的系统一般是通过广告搜索来做的，包括定义大量的复杂特征，通过特征学习来实现。然而这样的系统很难实现迁移。这篇文章主要介绍了一个深度增强学习方法可以有效的在复杂环境下进行自学习。下图给出了该系统的总体设计。系统底层还是一个搜索系统，再用增强学习方法通过TensorFlow进行学习。
![](http://6.eewimg.cn/mp/uploads/2019/01/02/395c2932-0e70-11e9-91b7-00163e2e672a.jpg)
**《Reinforcement Learning to Rank in E-Commerce Search Engine: Formalization, Analysis, and Application》**
> 
该论文在搜索结果排序方面提出了在考虑利用不同步排序结果的联系的基础上，通过利用增强学习实现最大化预期累积奖励的排序策略。文章中正式定义了search session Markov decision process(SSMDP)的概念并以此表示了电子商务搜索场景下的多步排序问题。
在此基础上，作者通过证明最大化累积回报的必要性从而证明了不同步骤的排序之间是存在紧密的联系，而并非是相互独立的；作者提出了利用DPG-FBE（deterministic policy gradient with full backup estimation）算法来解决高回报差异以及不平衡的回报分配问题，实现了SSMDP下的最优排序策略。
上图中表示了在策略π下，用户进行商品搜索时的三种行为及对应行为发生的概率：购买（B，b）、继续浏览（C,c）、离开（L,l），浏览项目历史（h）及对应的状态转移，并定义了状态值函数。
![](http://6.eewimg.cn/mp/uploads/2019/01/02/3964db5e-0e70-11e9-91b7-00163e2e672a.jpg)
**《Visual Search at Alibaba》**
> 
该论文介绍的是阿里巴巴的一个可视化搜索工作，基本使用场景是用户可以实现手机端或者桌面端的基于图片的搜索。如下图的例子，当用户选择一个图片的时候，系统自动识别图片中的实体对象，然后搜索相似或者相关的图片。
![](http://6.eewimg.cn/mp/uploads/2019/01/02/396abb96-0e70-11e9-91b7-00163e2e672a.jpg)
在具体实现方面，阿里巴巴采用词典和特征相结合的方法，首先离线对所有图片进行表示学习（表示学习的时候同时利用了图片内容信息和对象识别结果），然后构建词典，也就是将图片映射到词典中。在在线系统中通过CNN进行图片内的对象识别和表示学习。最后和已有的索引进行匹配。
下图给出了可视化搜索系统的离线学习和在线搜索系统的总体架构。具体实现方法是结合分类建模和近邻搜索技术减小了搜索范围，实现了更加精准有效的类别预测方法。利用softmaxloss函数对GoogLeNet V1网络进行图像分类训练，得到每一种图像类别的概率；通过检索最相似的Top30图像，并对每个图像进行加权计算概率；最后，将以上的到的两个概率进行加权平均，得到最终结果。
![](http://6.eewimg.cn/mp/uploads/2019/01/02/3973b228-0e70-11e9-91b7-00163e2e672a.jpg)
此外，通过弱监督学习方式并基于深度CNN架构，实现了目标检测和特征表示的联合学习；通过直接使用用户的点击图像的行为作为样本来训练模型。最后通过二值特征索引和重排序技术，实现了用户移动端毫秒级别的响应。
![](http://6.eewimg.cn/mp/uploads/2019/01/02/397ce348-0e70-11e9-91b7-00163e2e672a.jpg)
**《Perceive Your Users in Depth:Learning Universal User Representations from Multiple E-commerce Tasks》**
> 
在推荐算法方面，Yabo Ni等人提出通过多任务的综合分析学习来了解用户的普遍行为，实现更精确的预测和推荐。
文章提出了一种基于多任务学习的表示学习，DUPN（Deep User Perception Network），该网络可以从多个任务中生成一种普遍的用户行为表示，这种表示能够从用户的复杂行为中提取主要特征，并可以被用到其他任务中；
文章提出了一种新的基于注意力机制网络和RNN的深度架构，将电子商务中用户和项目建模为序列行为，并利用一种新的上下文行为的注意力机制来整合相应的内容和行为信息来使得能够更好地学习用户的行为向量。
![](http://6.eewimg.cn/mp/uploads/2019/01/02/398427ca-0e70-11e9-91b7-00163e2e672a.jpg)
模型将用户行为序列作为输入，并将每个行为传递到一个嵌入的向量空间。然后，使用LSTM和Attention-based Polling来获得一个用户表示向量。LSTM帮助建模用户行为序列，注意力网帮助从序列中提取不同权重的信息。不同人物之间可以共享这些用户表示。系统维护一个item集。对给定用户的查询，系统检索标题中包含查询词的条目，对条目进行排序，并向用户提供rank最靠前的list。
![](http://6.eewimg.cn/mp/uploads/2019/01/02/398bbf44-0e70-11e9-91b7-00163e2e672a.jpg)
**《Billion-scale Commodity Embedding for E-commerce Recommendation in Alibaba》**
> 
该文介绍了Jizhe Wang等人和香港科大的Huan Zhao等人在十亿级的电子商务网络中商品的表示学习（见上图）。他们从用户的行为历史中构建了一个项目图，并用提出用图嵌入算法来学习图中项目的表示向量，从而得到一个基于计算相似性得到的项目集。
在图嵌入算法方面，在Base Graph Embedding(BGE)的基础上，他们提出Graph Embedding with Side Information（GES）算法，通过利用辅助信息来解决与其他商品联系较少的商品问题，并进一步提出了Enhanced Graph Embedding with Side Information(EGES)，通过加权机制来实现对不同辅助信息的有效利用。此外，为实现十亿级别的用户和商品计算，他们将该图嵌入系统部署在了Xtensorflow(XTF)上。
![](http://6.eewimg.cn/mp/uploads/2019/01/02/39926236-0e70-11e9-91b7-00163e2e672a.jpg)
上图中SI代表辅助信息，“SI 0”为商品本身。在实际中情况中
- 
Sparse features 常常是商品和不同辅助信息的one-hot-encoder向量集
- 
Dense Embedding是商品的表示和相应的SI
- 
Hidden representation 是一个商品和其对应的辅助信息的嵌入集
![](http://6.eewimg.cn/mp/uploads/2019/01/02/3999a366-0e70-11e9-91b7-00163e2e672a.jpg)
**《Cascade Ranking for Operational E-commerce Search》**
> 
该论文提出了适用于运营电子商务搜索的多级级联排名模型（CLOES）。该模型综合考虑了搜索精确度、计算成本、搜索延迟以及搜索结果规模，并且在降低计算成本的同时提高了搜索的精确度。
这是第一次将级联模型用于大型电子商务搜索的公共研究（见上图）。该模型在级联的前期阶段通过利用简单并且代价较小的特征和模型过滤掉一些相关性较低的项目；之后的阶段利用更加精确但是更复杂的特征和模型来实现更加精确的排名，并提出了修改的逻辑回归补偿函数来处理前一阶段“数量过少”和“搜索延迟过高”的情况。在文中，逻辑sigmoid函数被用作单阶段分类器。2017年用于淘宝的3-stage级联模型在保证搜索精确度的情况下能够降低20%的cpu消耗。
![](http://6.eewimg.cn/mp/uploads/2019/01/02/39a050ee-0e70-11e9-91b7-00163e2e672a.jpg)
**《KunPeng: Parameter Serverbased Distributed Learning Systems and Its Applications in Alibaba and AntFinancial》**
> 
该论文介绍了蚂蚁金服提出了基于参数服务器的分布式学习系统“鲲鹏”。“鲲鹏”使得现有的一系列算法在十亿级别的样本和特征数据上的性能及效率有了极大的提高。
鲲鹏的总体架构的核心模块包括以下几部分：Server nodes：对模型做分片存储、Worker nodes：对训练数据做分片并计算、Coordinator：控制算法整体流程，如初始化，迭代，终止等、ML Bridge：使用脚本形式的工作流对数据进行预处理、PS-Core：核心的参数服务器组件 (servers/workers/coordinator)、Fuxi：监控所有机器运行状态，必要时进行容错。上图给出了鲲鹏的总体架构。
鲲鹏的使用流程对用户是完全透明的，而且开发者能够通过非常非常简单的代码实现复杂的通信和调度过程。鲲鹏架构使得常用的机器学习算法的大规模化成为了可能，截止目前，已经有众多机器学习算法在鲲鹏上得以实现和应用，包括但不限于LR，FTRL，MART，FM，HashMF，DSSM，DNN，LDA。
![](http://6.eewimg.cn/mp/uploads/2019/01/02/39a968c8-0e70-11e9-91b7-00163e2e672a.jpg)
**《A Hybrid Framework for Text Modeling with Convolutional RNN》**
> 
该论文在文本分类方面提出了conv-RNN框架。Conv-RNN综合使用和CNN和RNN并且整合了两个模型在不同方面的优点—— RNN 模型对不定长跨度的上下文依赖的编码能以及CNN 模型中常用的最大池化机制，增强了机器学习和分类的能力。
conv-RNN结构：Word embedding层：原始输入句子S中的词映射为对应的词向量，并用矩阵来表示句子；Bi-RNN层：双向RNN层，得到正向的hidden state 和反向的hidden state；Convolution layer：在RNN层得到的hidden state的基础上进行卷积，使用ReLU作为激活函数；Pooling层：用max-pooing作为池化函数，对每个卷积得到的向量进行max-pooling操作得到其中最大值。则生成对应的输入向量。
文中提出了基于句子分类的conv-RNN，在conv-RNN 之上加一个联合层，将conv-RNN得到的Xq以及之前的得到的两个hidden state向量拼接起来作为输入文本的最终表示。最后经过softmax层将向量表示映射到各个类别上，完成分类预测。
在 conv-RNN 语义编码算法基础之上，进一步提出了一种新的问答匹配模型。此外，在该模型中，还引入了一种”权值共享”机制以及 attention 方法，用以进一步提升 question-answer 匹配效果。
**阿里巴巴近两年发表于KDD的论文列表**
《Reinforcement Learning to Rank in E-Commerce Search Engine: Formalization, Analysis, and Application》
收录会议：KDD’18
《Perceive Your Users in Depth: Learning Universal User Representations from Multiple E-commerce Tasks》
收录会议：KDD’18
《Billion-scale Commodity Embedding for E-commerce Recommendation in Alibaba》
收录会议：KDD’18
《Visual Search at Alibaba》
收录会议：KDD’18
《Deep Reinforcement Learning for Sponsored Search Real-time Bidding》
收录会议：KDD’18
《Learning and Transferring IDs Representation in E-commerce》
收录会议：KDD’18
《Deep Interest Network for Click-Through Rate Prediction》
收录会议：KDD’18
《Learning Tree-based Deep Model for Recommender Systems》
收录会议：KDD’18
《Cascade Ranking for Operational E-commerce Search》
收录会议：KDD’17
《KunPeng: Parameter Server based Distributed Learning Systems and Its Applications in Alibaba and Ant Financial》
收录会议：KDD’17
《A Hybrid Framework for Text Modeling with Convolutional RNN》
收录会议：KDD’17
《Local Algorithm for User Action Prediction Towards Display Ads》
收录会议：KDD’17
《Optimized Cost per Click in Taobao Display Advertising》
收录会议：KDD’17
# 腾讯（Tencent）
2018年腾讯在数据挖掘顶级国际会议KDD上作为第一作者单位一共发表了2篇论文，下面简要解读其中的部分文章。（论文列表附在其后，表里还包括了部分合作文章）
![](http://6.eewimg.cn/mp/uploads/2019/01/02/39b00d22-0e70-11e9-91b7-00163e2e672a.jpg)
**《MIX: Multi-Channel Information Crossing for Text Matching》**
> 
该论文提出利用多通道信息交叉进行文本匹配模型MIX。MIX融合了CNNs和注意力机制，通过提取多个粒度的特征得到MIX表示的文本片段，然后通过semetic information channel 和 structutal information channel（用作attention mechanism）进行文本匹配得到精确的匹配结果。
上图给出了多粒度建模的总体框架。具体方法分为三步：
Step1：句子在不同的粒度被分割成不同的片段；
Step2：在attention units部分，通过提取语法信息来设计attention channels中的注意矩阵。
Step3：如图weighed channels 和2D-convolution部分所示，通过交叉局部匹配通道和注意通道，提取显著特征组合进行局部匹配。
![](http://6.eewimg.cn/mp/uploads/2019/01/02/39b673ec-0e70-11e9-91b7-00163e2e672a.jpg)
**《On the Generative Discovery of Structured Medical Knowledge》**
> 
在智能医疗领域，芝加哥大学的Chenwei Zhang和腾讯的Yaling Li等人引入了一种生成式的视角来研究关系医学实体对发现问题，旨在在最小化数据需求的同时，扩大高质量而又新颖的结构化新医学知识的规模。
他们提出了Conditional Relationship Variational Autoencoder（CRVAE）模型，在没有复杂特征工程的情况下，通过单独地从不同维度表达的实体对学习共性，该模型可以生成性的发现特定医疗关系下的实体对，并在模型编码的过程中获得关系增强的实体表示。
![](http://6.eewimg.cn/mp/uploads/2019/01/02/39bc1c8e-0e70-11e9-91b7-00163e2e672a.jpg)
编码器接收关系医疗实体对和关系指示作为输入，通过训练来加强医疗实体对的表示，并编码对每一种医疗关系的不同种实体对表示，作为潜在空间。解码器进行共同训练，并重建实体对。生成器与解码器结构相同，然而，它并没有重建输入中给出的关系医疗实体对，而是直接从学习到的潜在变量分布中抽取样本，为特定关系生成有意义的医疗关系。
**腾讯2018年发表于KDD的论文列表**
《MIX: Multi-Channel Information Crossing for Text Matching》
收录会议：KDD’18
《On the Generative Discovery of Structured Medical Knowledge》
收录会议：KDD’18
《An Efficient Two-Layer Mechanism for Privacy-Preserving Truth Discovery》
收录会议：KDD’18
# 百度（Baidu）
2018年百度在数据挖掘顶级国际会议KDD上作为第一作者单位一共发表了2篇论文。下面简要解读其中的部分文章。（论文列表附在其后，表里还包括了2017年的文章）
![](http://6.eewimg.cn/mp/uploads/2019/01/02/39c4be0c-0e70-11e9-91b7-00163e2e672a.jpg)
**《Du-Parking: Spatio-Temporal Big Data Tells You Real time Parking Availability》**
> 
该论文提出了基于时间和空间大数据的实时停车可用性方案Du-Parking。该方案是一种基于DNN的学习方法，它由三个主要部分组成，分别是建模速度、时间和一般影响，通过利用在线传感器以及多种数据库的结合并对时间和空间特征有区别的处理，来提供实时停车可用性信息，上图给出了整个框架图。
Grid Computing：该组件是一个离线分布式系统，主要有三种功能:
- 
基本静态特征提取：POI相关特性被从百度地图的数据仓库中提取出来用于训练和预测。
- 
sample processing: 系统将每30分钟从百度地图获取的实时停车场占用数据转换为标记样本数据。存储在数据库中的示例数据，用于离线学习和评估。
- 
模型训练：训练数据集由带标签的样本数据和所有由静态特征和动态特征组成的特征生成。该模型分别针对每个POI类别进行了训练。
Real time streaming computation: 一个实时处理地理位置坐标、导航数据的流计算系统。系统获取地理坐标和导航轨迹，并将每个坐标映射成一个地理网格索引。然后将映射的数据存储在内存数据库中进行在线预测。
Online service: 通过获取POI相关特性,从数据库检索实时位置和导航轨迹,将所有特性融合成一个单一的特征向量，通过训练的模型预测停车可用性水平。
![](http://6.eewimg.cn/mp/uploads/2019/01/02/39ce6998-0e70-11e9-91b7-00163e2e672a.jpg)
**《Exploring the Urban Region-of-Interest through the Analysis of Online Map Search Queries》**
> 
该论文提出通过Region-of-Interest来研究城市中人的活动，并通过对在线地图查询日志中大规模数据的挖掘提出了一套系统性的研究方法。
他们首先将城市地区分成小的网格区域，然后通过提取查询数据中区域网格之间的流动量形成一个转移矩阵，然后通过PageRank算法计算出每个网格的流行度，并进一步利用密度算法对网格进行聚类以检测ROIs。
在第二个任务中，设计了一个时空潜在因子模型URPTM，用于发现ROI访问者潜在的旅游主题。在模型中，每个ROI都被看作是一个文档，而地图查询中的时间、原点和POI标记则被看作是单词。在学习模型后，我们可以得到每个ROI的访问者的时空偏好，这可以用于很多应用，比如旅游需求分析和目标ROI分割。
![](http://6.eewimg.cn/mp/uploads/2019/01/02/39d44764-0e70-11e9-91b7-00163e2e672a.jpg)
**《Recruitment Market Trend Analysis with Sequential Latent Variable Models》**
> 
该论文用序列潜变量模型分析招聘市场趋势，首次尝试了利用无监督学习方法对招聘市场的趋势进行自动建模。为招聘市场分析提供了新的研究范式。
文中提出了一种名为MTLVM的新的顺序潜在变量模型，它是为了捕获企业招聘状态的时间依赖性而设计的，能够在贝叶斯生成框架中自动学习潜在招聘主题。此外，为了捕获随时间变化的招聘主题，文中为MTLVM设计了分级Dirichlet流程。这些过程允许动态生成招聘主题。最后，文中实现了一个基于大规模实际招聘数据的原型系统来对其的方法进行实证评估。该模型生成的方法可以发现招聘市场趋势，为招聘人员和求职者提供指导。
在样本中通过Diriclet过程生成的潜在因子以及结合现有招募状态，来生成招募要求。再由生成招募主题，每一个招募职位都由相应的决定的多项式分布生成。
![](http://6.eewimg.cn/mp/uploads/2019/01/02/39dd9e22-0e70-11e9-91b7-00163e2e672a.jpg)
**《Prospecting the Career Development of Talents: A Survival Analysis Perspective》**
> 
该论文介绍了Huayu Li等人研究的企业内部人才职业发展问题，提出了一种基于多任务学习和排序约束公式的新的生存分析方法对员工的生涯路径进行建模。在不同的排名约束和预测目标下，它能够对人才管理中的两个关键问题——人员流动和职业发展——进行建模。
具体来说，为了对员工的离职行为进行建模，文中通过考虑将每段时间间隔的预测作为任务，从而将每隔一段时间序列的生存状态预测作为多任务学习的问题。为了对经过审查和未经审查的数据进行建模，并利用非周期性和周期性事件捕获一般生命周期建模中显示出的本质属性，文中对每一对不同的生存状态标签施加了排名约束。对于职业生涯发展的建模，文中将每个时间间隔的相对职业水平预测作为一个任务，使用不同等级的排名约束来提高性能准确率。最后，在真实世界的数据上的广泛实验结果清楚地验证了模型的有效性。
**百度近三年发表于KDD的论文列表**
《Du-Parking: Spatio-Temporal Big Data Tells You Realtime Parking Availability》
收录会议：KDD’18
《Exploring the Urban Region-of-Interest through the Analysis of Online Map Search Queries》
收录会议：KDD’18
《Prospecting the Career Development of Talents: A Survival Analysis Perspective》
收录会议：KDD’17
《Recruitment Market Trend Analysis with Sequential Latent Variable Models》
收录会议：KDD’16
《Days on Market: Measuring Liquidity in Real Estate Markets》
收录会议：KDD’16
