# 机器之心论文解读：可用于十亿级实时检索的循环二分嵌入模型（RBE） - 知乎
# 



机器之心原创，作者：Olli Huang，编辑：Hao。

> 今年 2 月，来自微软 Bing 的研究人员在今年的 KDD 会议上发表了论文《Recurrent Binary Embedding for GPU-Enabled Exhaustive Retrieval from Billion-Scale Semantic Vectors》。该论文提出了能够生成紧凑语义表征的「循环二分嵌入」（RBE），这些表征可存储在 GPU 上，RBE 使得十亿级的检索能够实时进行。机器之心对这篇论文进行了解读。

论文链接：[https://arxiv.org/pdf/1802.06466.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1802.06466.pdf)

## **1 简介**

信息检索（IR）是根据用户查询从存储在计算机上的源数据集合中检索信息的活动。信息检索已有长达一个世纪的历史 [1]，它是许多常见应用（如 web 搜索、产品推荐和社交网络上的个性化推送 feed 流服务）的核心。上世纪六七十年代，该领域的研究取得了重大突破 [2]，研究人员开始将查询和文档作为高维向量进行编码。

然而，处理高维向量并非易事。在涉及高维数据的检索任务中，基于熵和 KL 散度的统计测量方法，过去被广泛使用。然而，这类方法通常面临维数灾难 [3,4]。

k 最近邻算法（k-NN）是一种广为人知的经典算法，用于化解维数灾难。k-NN 算法采用「暴力搜索」（或者「穷举搜索」）方法找到查询点在参考点集中的 k 个最近邻。但是，k-NN 算法的计算开销巨大。因此，研究人员们提出了诸如近似最近邻（ANN）等算法 [5,6]，使用 kd 树对数据预先进行组织，从而减小计算量。

近十年，图形处理单元（GPU）在图像和视频处理任务中得到了广泛应用。然而，在开发基于 GPU 的信息检索（特别是穷举搜索）方面，却鲜有突出的成果。论文《Fast k nearest neighbor search using GPU》[7] 是对基于 GPU 的 k-NN 穷举搜索的早期尝试。

本文将解读微软 Bing 研究人员的一篇论文《Recurrent Binary Embedding for GPU-Enabled Exhaustive Retrieval from Billion-Scale Semantic Vectors》，他们在这篇论文中提出了一个新模型「循环二分嵌入」（Recurrent Binary Embedding，RBE），它首次将基于 GPU 的穷举 k-NN 算法应用到十亿级的实时信息检索任务上。

这篇论文主要的贡献是设计了能够生成紧凑语义表征的「循环二分嵌入」（RBE），使十亿级的检索能够实时进行，这些表征可存储在 GPU 上。RBE 模型使用 CNTK 中的 BrainScript 实现（BrainScript 不久后将被开源，项目地址： [https://docs.microsoft.com/en-us/cognitive-toolkit/](https://link.zhihu.com/?target=https%3A//docs.microsoft.com/en-us/cognitive-toolkit/)），且在 GPU 集群上训练而成。此外，这篇论文在一个自定义多 GPU 服务器上实现了基于 GPU 的 RBE 信息检索模型（简称 rbeGIR），如图 1 所示。
![](https://pic1.zhimg.com/v2-7ad4631d3cea831d284b3047c3c307a4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='766' height='369'></svg>)图 1：具备 4 张英伟达 GeForce GTX 1080 GPU、两个 8 核 CPU、128GB DDR 内存的 rbeGIR 服务器。
论文的作者利用微软付费搜索引擎收集的数据，对 rbeGIR 系统进行评估。RBE 模型的训练数据包含 1.75 亿（175M）唯一点击对，这些数据是从两年的有效搜索日志中采样得到的。通过交叉抽样给每一个点击增加 10 个负样本，使训练样本的总数达到 19.25 亿（1925M）。为了避免重复，验证数据由一个月后采样得到的 1.2M 点击对组成。此外，测试数据包括人类标注的 0.8M 个点击对。

此外，rbeGIR 系统在召回和延迟方面取得了非常好的表现。在基于 12 亿个关键词和 1 万条查询的评估中，该系统的平均召回率达到了 99.99%，而平均延迟仅为 31.17 毫秒，这充分体现了 rbeGIR 系统的实时检索能力。

## **2. 要点**

RBE 模型是在大型搜索引擎的搜索广告（sponsored search）背景下提出的。本质上，搜索广告机制是同时显示出广告与搜索结果。该生态系统中三个重要的相关利益方为「用户」、「广告商」和「搜索平台」。搜索平台的目标是显示出用户可能想要点击的广告。

用户将在搜索框中输入「查询」，而搜索引擎会查找到相关的信息。然后，搜索引擎将使用信息检索技术，提取出能够将用户和广告商意图相结合的「关键词」。最终，搜索引擎会根据这些关键词显示一些广告（或称「展现」）。如果用户点击了广告，则记录这个「点击」事件。

本节将介绍这篇论文的三个要点。第一个要点是 RBE 模型，它提供了一种为搜索广告语境中的查询和关键词生成紧凑向量表征的新方法。第二个要点是基于 RBE 的信息检索，这一部分关注的是将 RBE 模型应用在基于 GPU 信息检索系统中。最后一个要点，是使用基于 GPU 的穷举 k-NN 选择算法，这也是 rbeGIR 系统的重要组成部分，专门为十亿级检索而设计。

**2.1 RBE 模型**

如图 3 所示，RBE 模型旨在为搜索广告中的查询和关键词，生成紧凑的向量表征。RBE 模型是基于 CLSM 模型 [8] 构建而成（CLSM 模型架构如图 2 所示）。

RBE 模型与 CLSM 模型在「multi-layers」之前的部分是相同的前馈过程，multi-layers 以上的部分叫做 RBE 层。RBE 层由公式 8-11 形式化表示的。如图 3 和公示 9-11 所示，正是其中的循环结构使得 RBE 模型具备「循环」的性质。然而，RBE 模型与其他网络结构（如 RNN 或 LSTM）没有联系。这里「循环」仅仅指的是 RBE 模型中的循环模式。对于 RNN 或 LSTM 类模型，从时间步 t 到 t-1 的转换会共享同一组参数，以学习持续的记忆单元。而 RBE 模型在确定参数集是否固定或随着时间变化方面更加灵活。
![](https://pic3.zhimg.com/v2-36c0087a0588285d9e38997bafe58d26_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='425' height='341'></svg>)图 2：CLSM 模型架构![](https://pic1.zhimg.com/v2-ed1bd57bd4a35130743a8d51c851611c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='422' height='495'></svg>)图 3：RBE 模型架构![](https://pic1.zhimg.com/v2-141fe8fb44a5d69913e49a924c0afa58_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='687' height='218'></svg>)
本质上，公式 8-11 背后的关键思想是通过最大化从实值向量 f_i 中提取出的信息来建立二值分解 b_i^t。为了重构这样的二值分解，训练过程中会生成多个中间向量。




**2.2 基于 RBE 的信息检索**

用于关键词检索的系统架构如图 4 所示。该系统被称为基于 GPU 的 RBE 信息检索系统，或 rbeGIR 系统。首先，该系统采用了多个 GPU 来存储和处理 RBE 嵌入。圆角矩形内展示了第 p 个 GPU 的组成部分。在图 4 底部我们可以看到，关键词首先被离线转换为 RBE 向量，然后从 CPU 存储单元（寄存器、cache 等）转移到 GPU 显存中。另一方面，查询会被即时转换为 RBE 向量。GPU 内的穷举匹配组件，则负责计算查询的 RBE 嵌入与每个关键词之间的相似度。为了找到最佳关键词，匹配结果将指导第 p 个 GPU 负责局部选择和全局选择过程。所有 GPU 的计算结果都将通过选择合并（Selection Merge）对生成的 top N 关键词作出贡献。

内存效率是 RBE 模型的关键优势。举例而言，存储十亿个关键词需要 14.90GB 的内存空间，而使用浮点类型进行存储只需要 238GB 的存储空间。这为在多 GPU 进行 内存检索（in-memory retrieval）铺平了道路。此外，RBE 还能够学习针对于特定应用的表征，因此它比通用的量化算法更加准确。
![](https://pic4.zhimg.com/v2-d42cf3201a05ef24407232ff0f84f97b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='730' height='458'></svg>)图 4：基于 GPU 的 RBE 信息检索（rbeGIR）系统图示
rbeGIR 系统是基于论文作者开发的生产检索系统评估的。评估过程用到了两个对比基线：prod_1 指的是具有等量存储空间的生产环境；prod_2 指具备同等数量关键词的设置。此外，rbeGIR 系统中还存储了 12 亿个唯一关键词的嵌入。

如表 3 所示，在 2000 个查询中，每个查询返回的前 5 个关键词的平均质量为「差」、「一般」、「良」或「优」。表 3 中的每一列代表对应于每列标签的查询-关键词对数量的百分比差距。例如「良」的结果，rbeGIR 系统分别比 prod_1 和 prod_2 高出了 18.52% 和 11.19%。
![](https://pic3.zhimg.com/v2-f22b2fc6b1a0ed86aedfe72d6948dcb6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='643' height='160'></svg>)
为了评估 rbeGIR 系统的召回性能，论文作者首先使用精确的最近邻算法，将 1 万条查询和 12 亿个关键词，与 RBE 嵌入进行匹配。每个查询的 recall @1000 表示，rbeGIR 获得的 top 关键词，与相关关键词重叠的总数除以 1000 后的结果。实验结果显示，rbeGIR 系统的平均召回率（recall @1000）高达 99.99%。此外，rbeGIR 系统的平均延迟时间为 31.17 毫秒，这充分体现了该系统的实时检索能力。




**2.3 基于 GPU 的穷举 k-NN 选择算法**

rbeGIR 系统的一个重要组成部分是，可用于十亿级检索的穷举 k-NN 选择算法。该算法从局部的分段处理开始，它依赖于 k-NN 内核（如 Algorithm 1 所示）。该算法将查询和每个关键词的 RBE 嵌入作为输入，并输出一个具有最高相似度得分和索引的优先队列（priority queue）。输出的优先队列被传输给全局选择和合并选择过程，从而获得排序最靠前的关键词。全局选择和合并选择都采用了 Radix 排序方法 [9]，这也是最快的排序算法之一。
![](https://pic2.zhimg.com/v2-f1a33636cb46489a1806c1ff97164ccd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='742' height='334'></svg>)
## **3. 结论**

这篇论文介绍了一种可为十亿级信息检索任务生成语义向量表征的 RBE 模型，它能在 GPU 上存储和运行。RBE 表征可以进一步与穷举 k-NN 搜索相结合。此外，文中实现的 rbeGIR 系统，是利用深度学习算法和强大 GPU 算力的一次早期尝试。

论文作者指出，RBE 表征不局限于 CLSM 模型。如图 5 所示，未来 RBE 的概念可进一步泛化到语义哈希 [10] 或 word2vec [11] 等网络中。
![](https://pic1.zhimg.com/v2-33fbe693f1e6635e205e05aaa8440954_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='746' height='478'></svg>)图 5：RBE 的概念可以推广到其他网络，如语义哈希（左图）和 word2vec（右图）。*图 5：RBE 的概念可以推广到其他网络，如语义哈希（左图）和 word2vec（右图）。*



参考文献：

[1] Sanderson, M., and Croft, W. B. The history of information retrieval research. Proceedings of the IEEE 100, Special Centennial Issue (2012), 1444-1451.

[2] Salton, G., Wong, A., and Yang, C.-S. A vector space model for automatic indexing. Communications of the ACM 18, 11 (1975), 613-620.

[3] Weber, R., Schek, H.-J., and Blott, S. A quantitative analysis and performance study for similarity-search methods in high-dimensional spaces. In VLDB (1998), vol. 98, pp. 194-205.

[4] Aggarwal, C. C., Hinneburg, A., and Keim, D. A. On the surprising behavior of distance metrics in high dimensional spaces. In ICDT (2001), vol. 1, Springer, pp. 420-434.

[5] Friedman, J. H., Bentley, J. L., and Finkel, R. A. An algorithm for finding best matches in logarithmic expected time. ACM Transactions on Mathematical Software (TOMS) 3, 3 (1977), 209-226.

[6] Datar, M., Immorlica, N., Indyk, P., and Mirrokni, V. S. Locality-sensitive hashing scheme based on p-stable distributions. In Proceedings of the twentieth annual symposium on Computational geometry (2004), ACM, pp. 253-262.

[7] Garcia, V., Debreuve, E., and Barlaud, M. Fast k nearest neighbor search using GPU. arXiv preprint arXiv:0804.1448.

[8] Shen, Y., He, X., Gao, J., Deng, L., and Mesnil, G. A latent semantic model with convolutional-pooling structure for information retrieval. In Proceedings of the 23rd ACM International Conference on Conference on Information and Knowledge Management (2014), ACM, pp. 101-110.

[9] Merrill, D. G., and Grimshaw, A. S. Revisiting sorting for gpgpu stream architectures. In Proceedings of the 19th international conference on Parallel architectures and compilation techniques (2010), ACM, pp. 545-546.

[10] Salakhutdinov, R., and Hinton, G. Semantic hashing. RBM 500, 3 (2007), 500.

[11] Mikolov, T., Chen, K., Corrado, G., and Dean, J. Efficient estimation of word representations in vector space. arXiv preprint arXiv:1301.3781 (2013).




