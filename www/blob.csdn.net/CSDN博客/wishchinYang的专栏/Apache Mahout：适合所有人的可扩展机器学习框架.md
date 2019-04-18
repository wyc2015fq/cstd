# Apache Mahout：适合所有人的可扩展机器学习框架 - wishchinYang的专栏 - CSDN博客
2013年11月04日 20:03:48[wishchin](https://me.csdn.net/wishchin)阅读数：991
个人分类：[计算机视觉](https://blog.csdn.net/wishchin/article/category/1871617)
原文链接：[http://blog.csdn.net/gdp5211314/article/details/7173505](http://blog.csdn.net/gdp5211314/article/details/7173505)
**简介：** Apache Mahout 专家 Grant Ingersoll 引领您快速了解最新版本 Mahout 中的机器学习库，并通过示例说明如何部署和扩展 Mahout 的某些最常用的算法。
在软件的世界中，两年就像是无比漫长的时光。在过去两年中，我们看到了社交媒体的风生水起、大规模集群计算的商业化（归功于 Amazon 和 RackSpace 这样的参与者），也看到了数据的迅猛增长以及我们诠释这些数据的能力的显著提升。“[Apache
 Mahout 简介](http://www.ibm.com/developerworks/cn/java/j-mahout/)” 最初在 developerWorks 上发表也已经是两年之前的事情。这之后，Mahout 社区（以及项目的代码库和功能）取得了长足的发展。Mahout 也得到了全球各地各种规模的企业的积极采用。
在我撰写的 [Apache Mahout 简介](http://www.ibm.com/developerworks/cn/java/j-mahout/) 中，我介绍了许多机器学习的概念以及使用 Mahout 提供的一套算法的基础知识。我在那篇文章中介绍的概念仍然有效，但这套算法已经发生了显著的变化。这篇文章不会重述基础知识，而是重点关注
 Mahout 的当前状态，以及如何利用 Amazon 的 EC2 服务和包含 700 万个电子邮件文档的数据集在一个计算集群上扩展 Mahout。如需回顾基础知识，请参阅 [参考资料](http://www.ibm.com/developerworks/cn/java/j-mahout-scaling/#resources) 部分，特别是*《Mahout 实战》* 一书。此外，我假设读者具备
 Apache Hadoop 和 Map-Reduce 范式方面的基本知识。（有关 Hadoop 的更多信息，请参阅 [参考资料](http://www.ibm.com/developerworks/cn/java/j-mahout-scaling/#resources) 部分。）
Mahout 现状
Mahout 在极短的时间内取得了长足的发展。项目的关注点仍然可以归纳为我所说的 “3 个要点”：协同过滤（推荐机制）、聚类和分类。除此之外，这个项目还增加了其他一些功能。我将重点强调两个领域中的一些关键扩展和改进：机器学习的核心算法（实现），以及包括输入/输出工具、与其他库的集成点和更多参考示例的支持基础架构。然而，务必注意，本文对于现状的叙述并不完整。此外，由于篇幅有限，我只能通过寥寥数语简单介绍各项改进。建议读者阅读
 Mahout 网站的“新闻”部分和各 Mahout 发布版的发布说明，以了解这方面的更多信息。
算法，算法，还是算法
只要尝试解决过机器学习问题，您就应该认识到，没有任何一种能够适合所有情况的万能算法。为此，Mahout 添加了众多新实现。表 1 包含我总结出的 Mahout 中最重要的新算法实现，还列举了一些用例示例。在这篇文章稍后的内容中，我将实际运用其中的几个算法。
表 1. Mahout 中的新算法
|算法|简要描述|用例|
|----|----|----|
|逻辑回归，使用随机梯度下降法 (SGD) 求解|速度超快、简单的顺序分类器，能够在要求严格的环境中实现在线学习|为用户推荐广告，将文本分类为多种类别|
|隐藏式马可夫模型 (HMM)|经典分类算法的顺序和并行实现，设计用于在基本生成过程不可知的情况下建模实际过程。|文本词类标注、语音识别|
|奇异值分解 (SVD)|旨在减少大型矩阵内的噪声，使噪音更小、更容易处理|作为聚类、推荐机制和分类的前导，自动执行特性选择|
|狄利克雷聚类|基于模型的聚类方法，根据数据是否适合基本模型来确定从属关系|在数据存在重叠或层次时非常有用|
|谱聚类|一系列类似的方法，使用基于图形的方法来确定聚类从属关系|与所有聚类算法相似，适用于探索大规模、不可见的数据集|
|Minhash 聚类|利用散列战略，将类似的项目组织在一起，从而生成聚类|与其他聚类方法相同|
|大量推荐机制改进|分布式共生、SVD、交替最小二乘法|交友网站、电子商务、电影或书籍推荐|
|并置|支持 “Map-Reduce” 的并置实现|以统计学的方式在文本中查找关注的词组|
Mahout 还新增了对用户极为有用的大量低级数学算法（请参见数学包），表 1 所述算法使用了其中的许多低级数学算法，但这些低级数学算法是为一般用途设计的，因此有可能也适合您的需要。
改进和扩展 Mahout 的基础架构
## Mahout 命令行
不久之前，Mahout 发布了一个 shell 脚本，处理类路径、环境变量和其他设置项，从而使 Mahout 程序（包含`main()` 的 Mahout 程序）的运行更加轻松。该脚本 — 名为 mahout — 位于 $MAHOUT_HOME/bin 目录中。
利用开放源码项目并竭力使项目的代码与自己的代码协同工作的人越多，基础架构就越充实。对于 Mahout 来说，这种演进方式促成了多项改进。最显著的一项就是经过重大改进、一致的命令行界面，它使得在本地和 Apache Hadoop 上提交和运行任务更加轻松。这个新脚本位于 Mahout 顶层目录（下文中将称之为 $MAHOUT_HOME）下的 bin 目录中。（请参阅侧栏中的 [Mahout
 命令行](http://www.ibm.com/developerworks/cn/java/j-mahout-scaling/#script)。）
任何机器学习库都有两个关键组件，即可靠的数学库和一个有效的集合包。数学库（位于 $MAHOUT_HOME 下的数学模块中）提供了多种功能：范围从表示向量、矩阵的数据结构、操作这些数据结构的相关操作符一直到生成随机数的工具和对数似然值等有用的统计数据（请参阅 [参考资料](http://www.ibm.com/developerworks/cn/java/j-mahout-scaling/#resources)）。Mahout
 的集合库包含的数据结构与 Java 集合提供的数据结构相似（`Map`、`List` 等），不同之处在于它们原生地支持 Java 原语，例如 `int`、`float` 和 `double`，而非其 `Object` 对应部分 `Integer`、`Float` 和 `Double`。这一点非常重要，因为在处理拥有数百万项特征的数据集时，需要精打细算地考虑每个位。此外，在较大的规模上，原语及其 `Object` 对应部分之间的封包成本将成为严重的问题。
Mahout 还引入了一种新的集成模块，其中包含的代码旨在补充或扩展 Mahout 的核心功能，但并非所有用户在所有情况下都需要使用这种模块。例如，推荐机制（协同过滤）代码现在支持将其模型存储在数据库（通过 JDBC）、MongoDB 或 Apache Cassandra 中（请参阅 [参考资料](http://www.ibm.com/developerworks/cn/java/j-mahout-scaling/#resources) 部分）。集成模块还包含多种将数据转为
 Mahout 格式的机制，以及评估所得到的结果的机制。例如，其中包含可将充存满文本文件的目录转为 Mahout 向量格式的工具（请参阅集成模块内的 `org.apache.mahout.text` 包）。
最后，Mahout 提供了大量新示例，包括通过 Netfix 数据集计算推荐内容、聚类 Last.fm 音乐以及其他许多示例。此外，我为这篇文章开发的示例也添加了 Mahout 的代码库。建议您抽出一些时间，进一步研究一下示例模块（位于 $MAHOUT_HOME/examples 中）。
现在，您已经了解了 Mahout 的现状，下面我们将深入探究主要内容：如何外扩 Mahout。
[回页首](http://www.ibm.com/developerworks/cn/java/j-mahout-scaling/#ibm-pcon)
在云中扩展 Mahout
使 Mahout 有效实现扩展并不容易，无法通过简单地为 Hadoop 集群添加更多节点来实现此扩展。在决定 Mahout 能否能够有效进行扩展时，算法选择、节点数量、特性选择和数据的稀疏性等因素（以及内存、带宽和处理器速度这些常见要素）扮演着至关重要的角色。为了便于讨论，我将演示一个示例，合理地利用 Amazon 的 EC2 计算基础架构和 Hadoop（请参阅 [参考资料](http://www.ibm.com/developerworks/cn/java/j-mahout-scaling/#resources)），对
 Apache Software Foundation (ASF) 提供的一个公共邮件存档数据集运行某些 Mahout 算法。
在 “设置” 小节之后的每一小节中都会探讨外扩 Mahout 的部分关键问题，并研究在 EC2 上运行示例的语法。
设置
示例的设置包括两个部分：一个本地设置和一个 EC2（云）设置。要运行示例，您需要：
- [Apache Maven](http://maven.apache.org/download.html) 3.0.2 或更高的版本。
- [Git](http://git-scm.com/download) 版本控制系统（您可能还需要一个 [Github](http://www.github.com/) 帐户）。
- 一个基于 *NIX 的操作系统，例如 Linux 或 Apple OS X。Cygwin 可在 Windows® 中正常工作，但我没有做过这方面的测试。
为了实现本地设置，请在命令行中运行以下命令：
- `mkdir -p scaling_mahout/data/sample`
- `git clone git://github.com/lucidimagination/mahout.git mahout-trunk`
- `cd mahout-trunk`
- `mvn install`（Mahout 的测试需要一定的运行时间，如果希望跳过该测试，请添加一个 `-DskipTests`）
- `cd bin`
- `/mahout`（您应看到可运行的项目清单，例如 `kmeans`）
这将编译并恰当地安装您需要的全部代码。单独 [下载示例数据](http://www.lucidimagination.com/devzone/technical-articles/scaling-mahout)，将其保存在 scaling_mahout/data/sample
 目录中，并解压缩（`tar -xf scaling_mahout.tar.gz`）。出于测试方面的目的，这里仅包含将在 EC2 上使用的一个较小的数据子集。
为完成 Amazon 设置，您需要提供一个 [Amazon Web 服务](http://aws.amazon.com/) (AWS) 帐户（请注意安全密钥、访问密钥和帐户 ID），还需要对 Amazon 的 EC2 和 Elastic Block Store
 (EBS) 服务的工作原理有一定的了解。按照 Amazon 网站上的文档说明，获得必要的访问权限。
满足了先决条件之后，即可启动集群。最佳做法或许是从单独一个节点开始，然后根据需要添加更多的节点。当然需要注意，在 EC2 上运行需要付费。因此请务必在运行完成后关闭节点。
要启动一个集群，以便使用本文中的示例，请按以下步骤操作：
- 从一个 ASF 镜像 [下载](http://www.apache.org/dyn/closer.cgi/hadoop/common/) Hadoop 0.20.203.0，并在本地解压缩。
- `cd hadoop-0.20.203.0/src/contrib/ec2/bin`
- 在编辑器中打开 hadoop-ec2-env.sh，并执行以下操作：- 填写您的 `AWS_ACCOUNT_ID`、`AWS_ACCESS_KEY_ID`、`AWS_SECRET_ACCESS_KEY`、`EC2_KEYDIR`、`KEY_NAME` 和`PRIVATE_KEY_PATH`。如需了解更多信息，请参见
 Mahout Wiki 中的 “使用现有 Hadoop AMI” 页面（请参阅 [参考资料](http://www.ibm.com/developerworks/cn/java/j-mahout-scaling/#resources)部分）。
- 将 `HADOOP_VERSION` 设置为 `0.20.203.0`。
- 将 `S3_BUCKET` 设置为 `490429964467`。
- 设置 `ENABLE_WEB_PORTS=true`。
- 设置 `INSTANCE_TYPE`，至少应设置为 `m1.xlarge`。
- 在编辑器中打开 hadoop-ec2-init-remote.sh，并执行以下操作：- 在创建 hadoop-site.xml 的部分中，添加以下属性：<span style="font-family:Times New Roman;font-size:12px;"><property>
<name>mapred.child.java.opts></name>
<value>-Xmx8096m></value>
</property>
</span>
**注意**：如果希望运行分类，则需要使用更大的实例和更多的内存。我使用了两个 X-Large 实例和 12 GB 的 Java Heap 值。
- 将 `mapred.output.compress` 更改为 `false`。
- 启动您的集群：<span style="font-family:Times New Roman;font-size:12px;">./hadoop-ec2 launch-cluster mahout-clustering <em>X</em></span>
`*X*` 是您希望启动的节点数量（例如 `2` 或 `10`）。建议先使用较小的值，在熟悉之后添加更多节点。这种做法有助于控制成本。
- 为 ASF 公共数据集创建一个 EBS 卷（快照：snap--17f7f476），将其附加到 /dev/sdh 中的主节点实例（这是 mahout-clustering-master 安全组中的实例）。（[参考资料](http://www.ibm.com/developerworks/cn/java/j-mahout-scaling/#resources) 部分中提供了
 EC2 在线文档内具体指南的链接。）- 如果使用的是 EC2 命令行 API（请参阅 [参考资料](http://www.ibm.com/developerworks/cn/java/j-mahout-scaling/#resources) 部分），则可执行：- `ec2-create-volume --snapshot snap-17f7f476 --z ZONE`
- `ec2-attach-volume $VOLUME_NUMBER -i $INSTANCE_ID -d /dev/sdh`，其中 `$VOLUME_NUMBER` 是 create-volume 步骤的输出，`$INSTANCE_ID` 是
 launch-cluster 命令启动的主节点的 ID
- 此外，也可以通过 AWS Web 控制台完成此任务。
- 将 setup-asf-ec2.sh 脚本（请参阅 [下载](http://www.ibm.com/developerworks/cn/java/j-mahout-scaling/#download) 部分）上传到主实例：<span style="font-family:Times New Roman;font-size:12px;">./hadoop-ec2 push mahout-clustering $PATH/setup-asf-ec2.sh</span>
- 登录您的集群：<span style="font-family:Times New Roman;font-size:12px;">./hadoop-ec2 login mahout-clustering</span>
- 执行 shell 脚本来更新系统、安装 Git 和 Mahout，并清除部分存档，以使之更易于运行：<span style="font-family:Times New Roman;font-size:12px;">./setup-asf-ec2.sh</span>
完成具体设置之后，下一步我们来看看如何运用一些较为流行的 Mahout 算法，并对其进行上扩。我主要关注的是上扩的实际任务，但在此过程中也会介绍有关特性选择的一些问题，以及做出某些特定选择的原因。
推荐
协同过滤是 Mahout 最流行、最易用的功能之一，因此也是讨论如何外扩 Mahout 的合理起点。再次提醒，我们使用的是来自 ASF 的邮件存档。对于推荐任务来说，一种有趣的可能性就是搭建一个系统，根据其他用户已经阅读过的邮件线程向一名用户推荐他可能会感兴趣的邮件线程。为将作为协同过滤问题处理，我将定义一个项，系统会根据邮件标头中的消息 ID 和引用，确定是否将其作为邮件线程进行推荐。用户将由邮件消息中的
 “发件人” 地址定义。换句话说，我关心的是谁发出或回复了一个邮件消息。至于偏好本身的值，我直接将与邮件线程的交互作为一个布尔偏好处理：如果用户 X 与线程 Y 交互，则偏好值为 on；否则为 off。这种选择唯一的负面效应就是我们必须使用可处理布尔偏好的似然性指标，例如 Tanimoto 似然法或对数似然法 (log-likelihood)。这通常能加快计算速度，并且可能减少系统内的噪声，但您的经历可能有所不同，所以您可能希望尝试使用不同的权重。
## 线程、消息 ID、避免好高骛远
请注意，我处理消息线程的方法并不完美，存在邮件列表中常见的*线程劫持 (thread hijacking)*。线程劫持的含义是，某人通过回复邮件列表中已有消息的方式发出新消息（即具有新主题/话题的消息），附带传递了原始消息引用内容。这篇文章没有尝试解决这个问题，而是直接选择忽略，但实际解决方案可能需要正面解决此问题。因此，我选择了 “足够好” 的方法，力求完美。
协同过滤方面的特性选择较为简单（用户、项、可选偏好），因此我们可以跳过这一步，直接介绍如何从原始邮件存档中获取在本地运行的内容，然后再在云中运行。请注意，在很多情况下，最后一步通常不是必需的，因为在单独一台机器上可足够快地获得结果，没有必要引入 Hadoop 使问题复杂化。按照大致估算，Mahout 社区基准测试表明可在单独一个节点上为多达 1 亿名用户提供推荐。电子邮件数据达不到如此之多的项数（约为
 700 万条消息），但无论如何，我仍会继续在 Hadoop 上运行它。
为了查看代码的实际效果，我在 $MAHOUT_HOME/examples/bin/build-asf-email.sh 文件中的 shell 脚本内打包了必要的步骤。请执行这个 shell 脚本，传递您的输入数据位置和所需的结果输出位置，例如：
<span style="font-family:Times New Roman;font-size:12px;">./build-asf-email.sh ./scaling_mahout/data/sample/content ./scaling_mahout/output/</span>
看到提示时，选择 `recommender`（选项 1），静候 Mahout 和 Hadoop 冗长的日志输出完成。完成后，您将看到类似清单 1 所示的内容：
清单 1. 运行推荐机制代码的输出示例 
<span style="font-family:Times New Roman;font-size:12px;">				
11/09/08 09:57:37 INFO mapred.JobClient: Reduce output records=2072
11/09/08 09:57:37 INFO mapred.JobClient: Spilled Records=48604
11/09/08 09:57:37 INFO mapred.JobClient: Map output bytes=10210854
11/09/08 09:57:37 INFO mapred.JobClient: Combine input records=0
11/09/08 09:57:37 INFO mapred.JobClient: Map output records=24302
11/09/08 09:57:37 INFO mapred.JobClient: SPLIT_RAW_BYTES=165
11/09/08 09:57:37 INFO mapred.JobClient: Reduce input records=24302
11/09/08 09:57:37 INFO driver.MahoutDriver: Program took 74847 ms
</span>
此作业的结果是面向输入数据内所有用户的全部推荐。结果存储在输出目录的子目录 prefs/recommendations 中，包含名称以 part-r- 开头的一个或多个文本文件。（这就是 Hadoop 输出文件的方式。）查看其中的一个文件即可注意到推荐已格式化为以下形式（同时还有一条警告）：
<span style="font-family:Times New Roman;font-size:12px;">				<em>user_id</em> [<em>item_id</em>:<em>score</em>, <em>item_id</em>:<em>score</em>, ...]</span>
例如，用户 ID 25 具有针对电子邮件 ID 26295 和 35548 的推荐。警告指出，`*user_id*` 和 `*item_id*` 并非原始 ID，而是从原始 ID 映射而来的整数。为帮助您理解这样做的原因，我将解释一下在执行
 shell 脚本时实际发生的情况。
在生成推荐结果的过程中，涉及三个步骤：
- 使用 Mahout 的 `SequenceFilesFromMailArchives`，将原始 mbox 文件转换为 Hadoop 的 `SequenceFile` 格式。
- 从消息中提取消息 ID 和 From 签名，并以 Mahout 能够理解的格式输出结果。
- 运行 Mahout 的 `RecommenderJob` 类。
本文不会详细介绍第 1 步，只建议对此感兴趣的读者参阅代码。
对于第 2 步，需要花一些工夫从文件中提取相关的信息片段（消息 ID、回复引用和 From 地址），随后以三元组（`From`ID、`Message-ID`、偏好）的形式存储它们，以供 `RecommenderJob` 使用。此过程由 `MailToPrefsDriver` 驱动，其中包含三个
 “Map-Reduce” 作业：
- 创建一个字典，将基于字符串的 Message-ID 映射为唯一的 `long` 值。
- 创建一个字典，将基于字符串的 From 电子邮件地址映射为唯一的 `long` 值。
- 提取 Message-ID、References 和 From；使用第 1 步和第 2 步中创建的字典将其映射为 `long` 值，将三元组输出到一个文本文件中。
完成所有这些操作之后，即可开始生成一些推荐。为了创建推荐，`RecommenderJob` 将执行图 1 所示的步骤：
图 1. 推荐机制作业流
![图 1. 推荐机制作业流](http://www.ibm.com/developerworks/cn/java/j-mahout-scaling/recommender-job.gif)
完成工作流中大部分工作的主要步骤是 “计算共生（calculate co-occurrences）” 步骤。这个步骤负责在整个矩阵内执行成对比较，寻找共性。除此之外，对于执行矩阵内任何行之间的成对计算（而不仅仅限于评分/评论），这个步骤（由 Mahout `RowSimilarityJob`支持实现）通常也非常有用。
shell 脚本中使用以下命令调用了 `RecommenderJob`：
<span style="font-family:Times New Roman;font-size:12px;">bin/mahout recommenditembased --input $PREFS_REC_INPUT --output $RECS_OUT --tempDir
    $PREFS_TMP --similarityClassname SIMILARITY_LOGLIKELIHOOD
   >
</span>
第一个参数告诉 Mahout 要运行哪个命令（`RecommenderJob`）；其他几个参数（`input`/`output`/`tempDir`）的含义不言自明。`similarityClassname` 告诉
 Mahout 在计算共生时如何计算各项之间的似然性。我选择使用对数似然性是因为它的简单性、速度和质量。
获得结果之后，下一步就是评估结果。Mahout 附带了一个评估包（`org.apache.mahout.cf.taste.eval`），提供了一些有用的工具，可帮助您检查结果的质量。遗憾的是，它们无法与基于 Hadoop 的算法一起使用，但在其他一些情况下可能非常有用。这些工具抽出一定百分比的数据作为测试数据，随后将这些数据与系统生成的数据对比，从而判断质量。
这就是生成推荐所需的全部操作，这种做法的优点在于可以直接在集群上运行。为此，请登录您之前设置的 EC2 集群，并运行与之前相同的 shell 脚本（/mnt/asf-email/mahout-trunk/examples/bin）。在向集群添加节点时，您应看到运行这些步骤所需的总时间缩短了。例如，在本地机器上运行一个完整的数据集需要超过三天的时间才能完成。在 EC2 上的一个 10 节点集群中，运行主要推荐任务加上将电子邮件转换为可用格式的准备时间只需约
 60 分钟。
最后一部分是将推荐作为应用程序的一部分使用，这部分留给读者作为练习。典型情况下，如果系统中有大量的项和用户，推荐将定期生成 — 根据业务需求的不同，生成频率大致为每小时一次至每天一次。无论如何，如果系统中的用户和推荐达到一定的数量，那么对所生成的推荐进行的更改将会更加微妙。
接下来，我们来观察一下电子邮件消息的分类，在某些情况下，可将此视为一种*上下文推荐系统*。
分类
Mahout 拥有多种分类算法，其中大多数算法都是为了在 Hadoop 上运行而编写的（只有一个值得注意的例外，即随机梯度下降法）。出于本文的目的，我将使用贝氏分类器 (naïve bayes classifier)，许多人最初都选择这种分类器，在有效扩展的时候，它也能给出合理的结果。有关其他分类器的更多细节，请参见*《Mahout 实战》*一书中的相关章节，或者 Mahout wiki
 中的 “算法” 部分（请参阅 [参考资料](http://www.ibm.com/developerworks/cn/java/j-mahout-scaling/#resources) 部分）。
电子邮件文档按照 Apache 项目（Lucene、Mahout、Tomcat 等）细分，通常每个项目有两个或两个以上的邮件列表（用户、开发等等）。考虑到 ASF 电子邮件数据集是按照项目分区的，因此一个合情合理的分类问题就是：尝试预测一条新传入的消息应交付至哪个项目。例如，一条新消息是属于 Lucene 邮件列表还是属于 Tomcat 邮件列表？
要使 Mahout 的分类算法发挥效力，必须实现一个表示要识别的模式的模型，随后根据一个数据子集进行测试。在大多数分类问题中，都必须有一名或多名人员身体力行，手动标注将要在训练中使用的数据子集。但在本文所讨论的情况下，数据集已经按项目进行了划分，因此无需手动标注，但我要依靠人们在发送电子邮件时通常能够选取正确的列表这个事实，而我们都知道，事情并非总是能这样顺利。
就像在推荐机制示例中所介绍的一样，必要的步骤已经预先打包在 build-asf-email.sh 脚本之中，只要从菜单中选择选项 3（随后在出现第二条提示时选择选项 2，即标准贝氏分类器）即可执行分类。类似于推荐，外扩代码的部分工作是准备好要使用的数据。对于文本分类，这实际上意味着对特性进行编码，随后根据特性创建*向量*，但还包括设置训练和测试集。需要执行的全部步骤如下：
- 使用 Mahout 的 `SequenceFilesFromMailArchives`，将原始 mbox 文件转换为 Hadoop 的 `SequenceFile` 格式。（请注意，这里的运行时选项略有不同。）<span style="font-family:Times New Roman;font-size:12px;">bin/mahout org.apache.mahout.text.SequenceFilesFromMailArchives --charset "UTF-8" 
    --body --subject --input $ASF_ARCHIVES --output $MAIL_OUT </span>
- 将 `SequenceFile` 条目转为稀疏向量，并修改标签：- `bin/mahout seq2sparse --input $MAIL_OUT --output $SEQ2SP --norm 2 --weight TFIDF --namedVector --maxDFPercent 90 --minSupport 2 --analyzerName org.apache.mahout.text.MailArchivesClusteringAnalyzer`
- `bin/mahout org.apache.mahout.classifier.email.PrepEmailDriver --input $SEQ2SP --output $SEQ2SPLABEL --maxItemsPerLabel 1000`
- 将输入拆分为训练（training）集和测试集：<span style="font-family:Times New Roman;font-size:12px;">bin/mahout split --input $SEQ2SPLABEL --trainingOutput $TRAIN --testOutput $TEST
    --randomSelectionPct 20 --overwrite --sequenceFiles
</span>
- 运行贝氏分类器，执行训练和测试：- `bin/mahout trainnb -i $TRAIN -o $MODEL -extractLabels --labelIndex $LABEL`
- `bin/mahout testnb -i $TEST -m $MODEL --labelIndex $LABEL`
值得注意的两个重要步骤就是第 2 步和第 4 步。第 2a 步是主要的特性选择和编码步骤，一系列的输入参数负责控制如何将输入文本表示为向量中的权重。表 2 具体列举了第 2 步中与特性选择相关的选项：
表 2. 创建向量时的特性选择选项
|选项|描述|示例和备注|
|----|----|----|
|`--norm`|norm 通过一个计算向量长度的函数 (norm) 来修改所有向量|1 norm = 曼哈顿距离，2 norm = 欧几里得距离|
|`--weight`|计算任何给定特性的权重，并将其作为 TF-IDF（术语频率，逆向文档频率）或者单纯的术语频率。|TF-IDF 是搜索和机器学习中常用的权重模式，通常用于将文本表示为向量。|
|`--maxDFPercent`, `--minSupport`|这些选项均用于删除文档集合中出现得过于频繁（最大值）或过于不频繁的术语|在自动删除对于计算价值不高的高频术语或低频术语时非常有用|
|`--analyzerName`|一个 Apache Lucene 分析器类，可用于对文档中的词执行分词、词干化、删除或其他更改|如需进一步了解 Lucene，请参阅 [参考资料](http://www.ibm.com/developerworks/cn/java/j-mahout-scaling/#resources) 部分|
第 2a 步中的分析过程值得深究，因为它承担了特性选择的大部分工作。Lucene `Analyzer` 包括一个 `Tokenizer` 类以及零个或多个`TokenFilter` 类。`Tokenizer` 负责将原始输入划分成零个或多个标记（例如，单词）。`TokenFilter` 实例彼此连接，随后修改`Tokenizer` 生成的标记。例如，示例中所用的 `Analyzer`：
- 按照空格分词，此外还会处理一些标点符号的边缘情况。
- 将所有标记转为小写。
- 将非 ASCII 字符转为 ASCII，尽可能采用转换音调符号等方法。
- 删除字符数超过 40 个的标记。
- 删除停止词（请参见代码中提供的清单，由于内容过多，此处不便说明）。
- 使用 Porter 词干分析器提取标记的词干（请参阅 [参考资料](http://www.ibm.com/developerworks/cn/java/j-mahout-scaling/#resources) 部分）
这项分析的最终结果将使各文档的对应向量显著减少，并消除向量中可能会使分类器产生混淆的常见 “噪声” 词（*the*、*a*、*an* 等）。这个 `Analyzer` 以迭代的方法开发，首先在电子邮件中查找示例，随后通过 `Analyzer` 对其加以处理并检查输出，从而判断处理效果。遗憾的是，这个流程可以说是科学的，但也仅仅是一种直观的体验。流程和结果并不完美，但足够好。
第 2b 步对数据稍加转化以便处理，还删除了一些内容，使各种不同的标签平均地出现在训练数据中。这一点非常重要，因为我最初尝试处理数据时产生了机器学习中的常见问题，即相关训练示例过多的标签的过度拟合 (overfitting)。实际上，对完整的数据集运行聚类时，将 `--maxItemsPerLabel` 的设置降低至
 1000 仍然不够好，无法得到良好的结果，因为部分邮件列表中发布的内容少于 1000。这很可能是由 Mahout 的一个 bug 导致的，社区目前仍然在对此进行研究。
第 4 步是执行实际工作的部分，包括构建模型和随后测试模型是否有效。在第 4a 步中，`--extractLabels` 选项告诉 Mahout 根据输入判断训练标签。（另外一种做法是直接传入标签。）这个步骤的输出是一个可通过`org.apache.mahout.classifier.naivebayes.NaiveBayesModel` 类读取的文件。第
 4b 步获取模型和测试数据，检查训练成效如何。输出结果是一个混合矩阵，如 “[Apache Mahout 简介](http://www.ibm.com/developerworks/cn/java/j-mahout/)” 一文所述。对于示例数据，输出结果如清单 2 所示：
清单 2. 运行分类器代码的输出示例
<span style="font-family:Times New Roman;font-size:12px;">				
Correctly Classified Instances : 41523 61.9219%
Incorrectly Classified Instances : 25534 38.0781%
Total Classified Instances : 67057
=======================================================
Confusion Matrix
-------------------------------------------------------
a b c d e f ><--Classified as
190440 12 1069 0 0 | 20125 a= cocoon_apache_org_dev
2066 0 1 477 0 0 | 2544 b= cocoon_apache_org_docs
165480 2370 704 0 0 | 19622 c= cocoon_apache_org_users
58 0 0 201090 0 | 20167 d= commons_apache_org_dev
147 0 1 4451 0 0 | 4599 e= commons_apache_org_user
</span>
您应注意到，这对分类器来说实际上是一种非常糟糕的显示（尽管好于猜测）。产生这种糟糕的显示的原因很可能是给定 Apache 项目的用户和开发邮件列表的词汇过于相近，难以区分。16,548 条 cocoon_user 消息被错误地分类为 cocoon_dev 即可说明这一点。实际上，如果直接使用项目名称重新运行任务，而不区分示例数据中的用户类表和开发列表，将得到清单 3 所示的结果：
清单 3. 仅使用项目名称重新运行分类器代码的输出示例
<span style="font-family:Times New Roman;font-size:12px;">				
Correctly Classified Instances : 38944 96.8949%
Incorrectly Classified Instances : 1248 3.1051%
Total Classified Instances : 40192
=======================================================
Confusion Matrix
-------------------------------------------------------
a b c ><--Classified as
18733 1241 0 | 19974 a = cocoon_apache_org
7 20211 0 | 20218 b = commons_apache_org
</span>
您一定同意，96% 的准确度比 61% 好太多了！实际上，该结果过于完美，不可能是真的。这样的评分可能源于这个特定的小数据集的本质，或者是有待调查的更深层面的问题。事实上，这样的评分应警示我们进一步开展调查，添加数据并检查生成这种评分的代码。就目前而言，我的目的是通过这个示例来展示结果的效果，因此不再深究。然而，我们应该尝试利用其他技术或者更好的特性选择，或许还应该采用更多的训练示例，以便提高准确性。对结果执行*交叉验证* 也是一种常见的做法。交叉验证包括从训练样本中反复获取部分数据，并将其纳入测试样本，或者将这部分数据留出不用。随后系统将判断全部运行过程的质量，而不仅考虑一次。
将此纳入云就像是使用推荐机制一样简单。整个脚本将在您的集群中运行，只需传入恰当的路径即可。如果在 EC2 的 10 节点集群上运行，除通常的准备工作之外，训练和测试的整个过程只需短短几分钟。遗憾的是，如果您这样运行，则对云中整个数据集的运行质量将下降，因为某些邮件列表所包含的数据点极少。这些问题很可能未得到充分的考虑。
实现生产的后几步包括使模型作为运行时系统的一部分，设置工作流以确保从系统获得反馈时更新模型。下面，我将介绍聚类。
聚类
与分类相似，Mahout 也有无数种聚类算法，每种算法都有着与众不同的特点。例如，K-Means 的伸缩能力出色，但需要您预先指定需要的聚类数量；而狄利克雷聚类要求您选择所需的模型分布和聚类数量。聚类与分类有很多共同之处，在某些情况下甚至可以共同使用，将聚类作为分类的一部分。除此之外，分类的大部分数据准备工作都与聚类相似，例如将原始内容转换为顺序文件，随后再转换为稀疏向量，因此您可以参考 [分类](http://www.ibm.com/developerworks/cn/java/j-mahout-scaling/#classification) 部分，了解这些信息。
对于聚类来说，要回答的主要问题是：我们能否根据内容的相似度来合情合理地组织所有消息（而无论项目如何）？例如，或许 Apache Solr 邮件列表中有关使用 Apache Tomcat 作为一种 Web 容器的消息与针对 Tomcat 项目的消息更为接近，而非原始项目。
就本例而言，前几个步骤与分类相似，但在转换为稀疏向量之后就发生了差别。具体步骤如下：
- 与分类的第 1 步和第 2 步相同的步骤。
- 
```
$ bin/mahout kmeans --input "$SEQ2SP/tfidf-vectors" --output $CLUST_OUT -k 50 --maxIter 20 --distanceMeasure org.apache.mahout.common.distance.CosineDistanceMeasure
 --clustering --method mapreduce --clusters "$CLUST_OUT/clusters"
```
本例中运行 K-Means 来执行聚类，但 shell 脚本也支持运行狄利克雷聚类。（执行脚本时，您将看到选择希望运行哪种算法的提示。）在之前的示例中，值得深入研究的参数如下：
- `-k`：要创建的聚类的数量。我随意地选择了 50，但也可以选择其他值。
- `--maxIter`：K-Means 是一种迭代式算法，聚类中心将作为各次迭代的一部分更新。有时凭算法本身不能保证成功完成，因此可利用此参数来确保算法完成。
- `--distanceMeasure`：确定当前质心（centroid）与所检查的点之间的相似度的距离指标。在本例中，我选择了余弦距离指标，这通常是适合文本数据的合理选择。Mahout 还有多种其他实现，值得您抽出时间用自己的数据进行尝试。
- `--clustering`：告诉 Mahout 输出哪些属于哪个质心的点。默认情况下，Mahout 仅计算质心，因为这通常就能满足全部的要求。
在运行完成之后，您可以使用 Mahout 的 `ClusterDump` 程序转储聚类质心（和相关的点）。最终结果将存储在 kmeans 目录下名称以 clusters- 开头、以 -final 结尾的子目录中。具体的值将取决于运行任务使用了多少次迭代，例如 `clusters-2-final` 是第三次迭代的输出结果。例如，以下命令将转储聚类，而不是运行小规模数据样本：
<span style="font-family:Times New Roman;font-size:12px;">bin/mahout clusterdump --seqFileDir ../output/ibm/clustering/kmeans/clusters-2-final
    --pointsDir ../output/ibm/clustering/kmeans/clusteredPoints/
</span>
`--seqFileDir` 指向所创建的质心，`-pointsDir` 是聚类点的目录。清单 4 给出了小部分结果抽样：
清单 4. 运行 `ClusterDumper` 的输出示例
<span style="font-family:Times New Roman;font-size:12px;">				
:VL-337881{n=4420 c=[
  Top Terms:
    user                                    =>0.060885823267350335
    mailscann                               => 0.05059369006868677
    cocoon                                  =>0.048781178576134204
    virus                                   => 0.04285897589148712
    derek                                   => 0.04084340722527813
    legal                                   =>0.040052624979813184
    scan                                    => 0.03861016730680097
    danger                                  => 0.03848600584647758
    csir                                    => 0.03712359352614157
    transtec                                => 0.03388019099942435
  Weight : [props - optional]:  Point:
  1.0 : [distance=0.888270593967813]: 
  /cocoon.apache.org/dev/200004.gz/NBBBIDJGFOCOKIOHGDPBKEPPEEAA.XXXXX = 
  [ag:0.165, briefli:0.250, chang:0.075, close:0.137, cocoon:0.060, 
  cocoon1:0.226, cocoon2:0.218, concept:0.277, develop:0.101, differ:0.144, 
  explain:0.154, greet:0.197, klingenderstrass:0.223, langham:0.223, look:0.105, 
  mailserv:0.293, matthew:0.277, mlangham:0.240, paderborn:0.215, processor:0.231, 
  produc:0.202, put:0.170, scan:0.180, tel:0.163, understand:0.127, virus:0.194]
</span>
在 [清单 4](http://www.ibm.com/developerworks/cn/java/j-mahout-scaling/#listing4) 中，请注意输出包括一个术语列表，算法已经确定这是聚类中最有代表性的术语列表。在生成用于生产的标签、在准备步骤中调优特性选择时，这可能很有帮助，因为停止词（本例中，`user` 很可能就是一个停止此）往往出现在前几次试运行数据时的列表中。
正如您期待的那样，在集群中运行此任务就像在本地运行一样简单 — 就像这两个示例中一样简单。在我的测试中，除了转换内容所用的时间之外（约为 150 分钟），10 个节点上的实际聚类作业仅用了大约 40 分钟。
遗憾的是，对于聚类，尽管 Mahout 确实提供了一些评估工具（请参见输出顶级术语的 `CDbwEvaluator` 和 `ClusterDumper` 选项），但评估结果往往会简化为 “气味测试（smell test）”。在气味测试中，可视化聚类往往是最有益的做法，但很多图形可视化工具包难以处理大型数据集，因此您可能需要依靠自己的设备实现可视化。
就像推荐和分类一样，实现生产的后续步骤包括确定获取数据的工作流以及执行处理的频率，当然，还包括在您的业务环境中实际应用。您可能需要尝试不同的算法，确定哪种算法最适合您的数据。
[回页首](http://www.ibm.com/developerworks/cn/java/j-mahout-scaling/#ibm-pcon)
Mahout 未来展望
Apache Mahout 仍在通过多种方式向前发展。目前，社区的关注重点是通过执行性能测试、文档整理、API 改进和新算法添加来促进 1.0 发布版的推出。下一个发布版（即 0.6 版）很有可能在 2011 年的年底或不久之后推出。在较为深入的层面，该社区也开始着眼于分布式、内存中的机器学习问题解决方法。在很多情况下，单凭一台机器难以处理复杂的机器学习问题，但 Hadoop 又引入了过多磁盘
 I/O 方面的负担。无论采用哪种方法，关注可伸缩性的 Mahout 都能很好地帮助您解决当前最艰难的大数据问题，使得复杂机器学习算法的使用更加轻松。
致谢
特别感谢 Timothy Potter 协助我完成 AMI 打包，感谢 Mahout 专家 Sebastian Schelter、Jake Mannix 和 Sean Owen 的技术审查。本文的部分内容由 Amazon Apache 测试计划提供支持。
[回页首](http://www.ibm.com/developerworks/cn/java/j-mahout-scaling/#ibm-pcon)
下载
|描述|名字|大小|下载方法|
|----|----|----|----|
|Shell 脚本|j-mahout-scaling.zip|2KB|[HTTP](http://www.ibm.com/developerworks/apps/download/index.jsp?contentid=781453&filename=j-mahout-scaling.zip&method=http&locale=zh_CN)|
[关于下载方法的信息](http://www.ibm.com/developerworks/cn/whichmethod.html)
参考资料
**学习**
- "[Apache Mahout 简介](http://www.ibm.com/developerworks/cn/java/j-mahout/)”（Grant Ingersoll，developerWorks，2009 年 9 月）：Mahout 联合创始人
 Ingersoll 介绍了机器学习的基本概念，展示了如何使用 Mahout 来聚类文档、提供推荐以及组织内容。
- [Apache Mahout](http://mahout.apache.org/)：随时关注 Apache Mahout 主页，了解所有最新新闻、获得文档和新下载内容。 
- [Powered By Mahout](https://cwiki.apache.org/confluence/display/MAHOUT/Powered+By+Mahout)：Mahout 的 “powered by” 页面列举了愿意公开其使用 Mahout
 算法的情况的众多公司。 
- [*Taming Text*](http://www.manning.com/ingersoll)（Grant S. Ingersoll、Thomas S. Morton 和 Andrew L. Farris，Manning Publications，2011
 年）：这本书介绍了 Mahout 和用于构建基于文本的应用程序的相关开放源码技术。 
- [*Mahout 实战*](http://www.manning.com/owen)（Sean Owen、Robin Anil、Ted Dunning 和 Ellen Friedman，Manning Publications，2011
 年）：这本书是理解 Mahout 的终极指南。 
- [Apache Hadoop](http://hadoop.apache.org/)：了解 Hadoop。 
- [算法](https://cwiki.apache.org/confluence/display/MAHOUT/Algorithms)：在 Mahout Wiki 中阅读有关 Mahout 实现的算法的更多内容，并进一步了解社区关注的领域。 
- [惊喜和巧合](http://tdunning.blogspot.com/2008/03/surprise-and-coincidence.html)：在 Mahout 专家 Ted Dunning 的博客中了解有关对数似然性测量的有用之处。 
- [MongoDB](http://www.mongodb.org/)：访问 MongoDB 网站，发现有关 MongoDB 的更多内容。 
- [Apache Cassandra](http://cassandra.apache.org/)：访问 Apache Cassandra 网站，获得有关 Apache Cassandra 的更多信息。在 Sean Owen 的博客文章 “[Recommending
 (from) Cassandra](http://www.acunu.com/blogs/sean-owen/recommending-cassandra/)” 中进一步了解如何将其与 Mahout 一起使用。 
- [Amazon Web 服务，包括 EC2 在内](http://aws.amazon.com/)：进一步了解 Amazon 的 EC2 基础架构。 
- [使用现有 Hadoop AMI](https://cwiki.apache.org/confluence/display/MAHOUT/Use+an+Existing+Hadoop+AMI)：Mahout wiki 中的这个页面详述了在 EC2 中启动
 Hadoop 集群的流程。 
- [Amazon EC2 用户指南](http://docs.amazonwebservices.com/AWSEC2/latest/UserGuide/index.html)：查询 EC2 用户指南，了解本文中的 [设置](http://www.ibm.com/developerworks/cn/java/j-mahout-scaling/#setup) 步骤：- [使用公共数据集](http://docs.amazonwebservices.com/AWSEC2/latest/UserGuide/index.html?using-public-data-sets.html)：如何使用 Amazon EC2 公共数据集。
- [Apache Software Foundation 公共邮件存档](http://aws.amazon.com/datasets/7791434387204566)：使用 Apache Software Foundation 公共邮件存档的一个快照来测试
 Mahout。
- [使用 Amazon EBS 卷](http://docs.amazonwebservices.com/AWSEC2/latest/UserGuide/index.html?ebs-using-volumes.html)：如何使 Amazon EBS
 卷可用于您的操作系统。
- [命令行工具入门](http://docs.amazonwebservices.com/AWSEC2/latest/UserGuide/index.html?SettingUp_CommandLine.html)：如何设置您的 Amazon EC2 环境，以便利用
 Amazon EC2 命令行工具（也称为 *API 工具* 或 *CLI 工具*）。
- [Message-ID](http://en.wikipedia.org/wiki/Message-ID)：进一步了解邮件标头和 Message-ID（消息 ID）。 
- [Apache Lucene](http://lucene.apache.org/)：访问 Apache Lucene 网站，了解 Apache Lucene 的更多信息。 
- [Martin Porter 博士的雪球式词干分析器](http://snowball.tartarus.org/)：这些词干分析器通常适用于处理搜索和机器学习项目中的文本。 
- 浏览 [技术书店](http://www.ibm.com/developerworks/apps/SendTo?bookstore=safari)，阅读有关这些主题和其他技术主题的图书。 
- [developerWorks 中国网站 Java 技术专区](http://www.ibm.com/developerworks/cn/java/)：查看数百篇关于 Java 编程各个方面的文章。 
**获得产品和技术**
- [Apache Mahout](https://github.com/lucidimagination/mahout)：下载 Grant Ingersoll 的 Mahout 源代码，其中包括示例。 
- 下载 [IBM 产品评估试用版软件](http://www.ibm.com/developerworks/cn/downloads/) 或者[IBM
 SOA 人员沙箱](http://www.ibm.com/developerworks/cn/downloads/soasandbox/people/)，亲身体验 DB2®、Lotus®、Rational®、Tivoli® 和 WebSphere® 方面的应用程序开发工具和中间件产品。 
**讨论**
- [Mahout 用户邮件列表](https://cwiki.apache.org/confluence/display/MAHOUT/Mailing+Lists%2C+IRC+and+Archives)：订阅此邮件列表，以便提出问题、分享知识、讨论话题。 
- 加入 [developerWorks 社区](http://www.ibm.com/developerworks/mydeveloperworks/mydeveloperworks)。与其他 developerWorks 用户联系，浏览开发人员推动的博客、论坛、小组和
 wiki。
关于作者
![Grant Ingersoll](http://www.ibm.com/developerworks/i/p-gingersoll.jpg)
Grant Ingersoll 是 Lucid Imagination 的创始人和技术成员之一。Grant 的编程兴趣包括信息检索、机器学习、文本分类和提取。Grant
 是 Apache Mahout 机器学习项目的创始人之一，并且是 Apache Lucene 和 Apache Solr 项目的负责人和发言人。他还是描述使用开源工具进行自然语言处理的图书 *Taming Text*（Manning 出版社，即将上市）的合著者。
