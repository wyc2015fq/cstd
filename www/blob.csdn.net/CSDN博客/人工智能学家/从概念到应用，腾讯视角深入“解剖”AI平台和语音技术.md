# 从概念到应用，腾讯视角深入“解剖”AI平台和语音技术 - 人工智能学家 - CSDN博客
2018年09月21日 20:10:02[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：35
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVPZiaNT8f0ZT5y9EfBicH0tCLHuMDQicc0PNYQiapVg6MT9EyjEWYsfPx8icCrdrt1bz68ZcZP3pl6Jqw/640?wx_fmt=jpeg)
来源：AI科技大本营
摘要：9 月 15 日，以“破局人工智能：AI平台及智能语音应用解析”为题的沙龙在上海举办，本次活动由 CSDN、腾讯云联合主办。
近年来，人工智能技术快速发展，与其他行业的结合也成为业界不断探索的方向。在人工智能基础和工具方面，AI平台已成为降低人工智能门槛的关键因素；而在人工智能应用方面，作为最成熟的应用方向之一，智能语音已在智能家居、智能车载等领域有了迅猛发展。以腾讯为代表的巨头企业，凭借强大的技术积累和在社交聊天、游戏娱乐等垂直领域定制化服务的丰富经验，在AI平台和智能语音两方面都推出了丰富的解决方案。
9 月 15 日，以**“破局人工智能：AI平台及智能语音应用解析”**为题的沙龙在上海举办，本次活动由 CSDN、腾讯云联合主办。
**本次分享从人工智能技术的大背景切入，根据技术发展现状讲述AI平台如何降低人工智能门槛，帮助企业快速构建AI能力；以及深度学习在语音识别上的应用，语音技术在智能音箱等场景中的应用和解决方案，并探讨智能语音的未来发展趋势。**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/BnSNEaficFAYUbB24TSG1LjM5NJzYXVZMOkW0KWBSbyGDMic3Ov8j3GlaxcGTggZQ74QCabVQ9qiatVEK5UR1O3UQ/640?wx_fmt=jpeg)
腾讯云大数据的高级研究员罗冬日讲述了**“深度学习在语音识别上的应用”**。为什么现在语音识别的准确率得到大幅提升？他给出了三点原因：
- 
一是互联网存储了大量的语音数据；
- 
二是GPU/CPU硬件的计算速度有量级的提升；
- 
三是深度学习算法方面的应用。
关于深度学习在声学模型上的应用，罗冬日主要讲述了 DNN、LSTM、CLDNN 三种深度学习网络。
首先是 DNN，输入一帧数据，然后得到发音单元的分类结果；LSTM 单元会利用到分割时间点的一些数据源辅助判断当前数据如何分类，当然双向的 LSTM 效果会更好；CLDNN 是比较成熟和稳定的一种结构，在卷积网络后面再接 LSTM，后来再接 Dense，有一些高效的企业会提出更加新的网络，最后的识别效果也会更好。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/BnSNEaficFAYUbB24TSG1LjM5NJzYXVZMk8JLicwlqyUE6HCM47DO966EPKpfRfBgY0GibMys1hWmZT9hTOOUvhwA/640?wx_fmt=png)
此外，还有端到端的识别方法，如 CTC、encoder—decoder+attention 以及 Google 的 LAS 模型。
罗冬日提到，虽然现在语音厂商们都称语音识别的准确率达到了 97%，但这是在安静的情况下，如果是高噪音环境、口音识别或带情绪的声音情形下，还需采用更高质量的阵列麦克风，更多的远场数据，以及语义理解的辅助。
**腾讯高级产品架构师倪捷讲述了智能语音扩展数字化服务。**他认为，语音在数字化的时代重要性主要是用户不断追求更好人机交互体验的结果。
基于腾讯云的语音识别、语音合成等语音类能力，他介绍了一些实际应用的解决方案。首先是直播安全的解决方案，除了基于图像层面的鉴别外，还包括语音甚至音频声音识别的能力。其次是智慧法庭解决方案。以前的书记员需要做庭审记录，但会遇到可读性不强的问题，而语音识别技术则可对庭审记录准确转写，并且便于后续做档案检索。
还有客服质检，由于现在话务员跟客户的对话质量不可控，这就需要基于客服录音做质检，但靠人工质检的方式耗时长，资源消耗大，而通过语音转写等技术，在文字层面上就可通过关键词或者特定业务逻辑对客服做评分。
携程的客服服务使用的正是腾讯的语音识别技术，**其数据智能部总监张童皓在随后的演讲中就旅游场景中的客服质检做了重点介绍。**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/BnSNEaficFAYUbB24TSG1LjM5NJzYXVZMfULOvxJk2pj9ribLNTT6uVH5Mf9ultHVJ1djN2ocq3WO40ddP0T8apw/640?wx_fmt=png)
他展示了智能质检的识别过程。通过标注数据拿出来跟场景相关的关键词做比对，并对无用词进行删除，通过双向 LSTM+注意力机制，如果发现它跟现有的价格标签关联度很高，就可以得出一个关联关系。在这个过程当中有可能关联出降价，然后打出来价格倒挂的标签，当用户在携程上下了订单后，如果发现酒店的价格比线上便宜很多，这个时候会打电话投诉。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/BnSNEaficFAYUbB24TSG1LjM5NJzYXVZMWnokXbjplKm9BFOmQn1ibaUbXOotFalLHhrsH5yLa3ic9GQZpFBX2iaiag/640?wx_fmt=png)
他展望了智能质检的未来，利用 AI 技术未来可能会有一个永不离岗的资深高能质检员，使用 5 台服务器，便可实现每日通话全覆盖，已分类问题全时覆盖，并对突发问题快速检索；另外要打造出智能培训师，进行高分低分对比，提炼金牌话术；另外还有可能打造出实时辅导员，像在线知识库那样，让坐席人员解决问题更有效率。
腾讯语音识别中心的专家研究员王珺则分享了智能音箱的语音识别技术。她讲述了智能音箱语音交互技术的麦克风阵列、声纹识别、语音识别、语义理解和 TTS 五大链条，并对每个步骤做了前沿技术介绍。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/BnSNEaficFAYUbB24TSG1LjM5NJzYXVZMVW21LDh4QSSq1f5xWyAtAXibnUnXyoE2551OWbG6LCHhUvv4ukjG23Q/640?wx_fmt=png)
关于基于唤醒词的关键技术，目前最新的技术有哪些缺陷？王珺表示，首先是拓展性差，并且专用隐层或专用偏置训练的方法不能有效捕获目标说话人特征。自适应到目标说话人的波束形成深度学习方法，目前最先进的技术都要求平均至少 10s 的自适应语音，远超真实应用场景可接受的自适应语音（例如唤醒词）长度，可用性差，不易落地。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/BnSNEaficFAYUbB24TSG1LjM5NJzYXVZMADXY9CZkq9dDjclwk5h4MKOicyUsWquZ4LLkDibKhyQsuicrCdvPUOqHw/640?wx_fmt=png)
而腾讯的基于唤醒词信息的目标说话人语音提取技术的攻关目标，在性能上更优，包括在信号的失真比、主观语音质量评估、干扰说话人数、鲁棒性。而且从系统实时性，模型参数复杂度全方面评估，另外还包括拓展性、扩用性、深度研究价值等方面。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/BnSNEaficFAYUbB24TSG1LjM5NJzYXVZM27WEicEH3OYQIeoiaRGtI1m6TwBqW1lV2dQZaLPpCe23vk4wBmK6ZH6w/640?wx_fmt=png)
**王珺还分享了端到端语言识别的 Attention 建模方法的关键技术点，**最前沿的序列到序列技术各有它们的优缺点：CTC 的方法是序列到序列前沿技术的经典方法，它的优势是从左到右的序列到序列的模型，比较简单，解码速度快；同时它的缺陷是模型建立的前提是假设当前帧的输出和之前的输出标注独立。
基于 Attention 的优点是什么？王珺解释，它不需要假设与之前输出标注独立，对下一层的预测可以用到声学模型和语言模型的分析有自带的语言模型方法。但是也有非常明显的缺陷，Attention 缺乏从左到右的对齐限制。而CTC与Attention结合的方法可以得到正确的对齐结果。目前研究人员也在尽力的缩小它和经典混合系统的差距。
**腾讯高级产品经理郭律探讨了机器学习平台与人工智能的关系。**简单而言，把海量数据通过机器学习的手段来进行处理，所形成的模型就是人工智能。所谓机器学习的本质，解决的就是“精细”问题。这其中有4个约束条件：
- 
第一是业务场景，把业务问题转化为机器学习的建模问题；
- 
第二是海量数据的机器学习，从原理上面基于统计来实现；
- 
第三是数据质量的问题；
- 
第四是数据闭环，不断让模型学习新的数据。
除了业务和数据，郭律认为 AI 能力不可或缺的元素有两个，一个是人才，另一个是技术能力，包含计算资源等能力。但是相对而言 AI 工程化、建模的人才，AI 的计算资源和计算能力，包括计算资源和框架，其实相差不多。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/BnSNEaficFAYUbB24TSG1LjM5NJzYXVZMZkcvjplKAgOiarcvW6cNRLtna7WN1zSkqgld6duQYiaRl5NJiaEweC0dA/640?wx_fmt=png)
他认为AI 具有多样性，但 AI 人才和计算能力具有通用性。他们按 AI 的人才成熟度的不同，归纳为 4 类：
- 
第一是 AI Geek，从底层计算资源搭建到计算框架的引入，一直到算法编写层面，包括模型调优都能够独立进行编码。
- 
第二是 AI 研究者，他们自己会写比较高级的算法。
- 
第三种是 AI 从业者，他们知道整个建模的流程，也能够用一些开源的算法做一些模型训练。
- 
第四种是 AI 应用者，他们是 AI 非从业人员，希望使用人工智能解决业务痛点，大部分人都属于这一类。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/BnSNEaficFAYUbB24TSG1LjM5NJzYXVZMgy2XP9yHyeLvEyltI7iazlr3P77ht5uicwJIRHRXRjT4c0NCicKiaNCu6A/640?wx_fmt=png)
因此，针对不同的用户层次，需要提供不一样机器学习产品和服务。郭律以腾讯的智能钛机器学习平台服务为例给出了不同用户所需要的差异化服务，比如 TI-A 是深度学习加速器，主要为专业级用户进行模型训练加速，同时在模型训练完之后，可以部署成服务对外进行在线预测，解决了从模型落地到具体应用场景的问题。TI-ONE，智能一站式机器学习平台，它有一个可视化的 UI，客户可以一目了然地看到整个建模工作流，而且里面有丰富的内置算法组件。TI-S 的特点是全自动，只需要给出数据，就可以反馈出可用的模型，并部署成一个应用。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/BnSNEaficFAYUbB24TSG1LjM5NJzYXVZMPHCmibvuOCIsAGLZ8fichM4SDNbib9yC7qnIibzak8w4FHjIoH5ib172uOQ/640?wx_fmt=png)
此外，郭律还介绍了智能钛机器学习平台服务具体的功能。该平台对企业而言意味着降低了企业获得 AI 能力的门槛，可以帮助企业打造自己的 AI 生态，它能帮助形成“数据+模型”双闭环，支持模型自更新。
以下为腾讯的三款机器学习平台产品详细介绍：
**▌TI-ONE**
智能钛一站式机器学习（TI OneStop，TI-ONE）适合 AI 初学者，同时又兼顾专业用户需求的一站式机器学习平台。
**全流程一站式服务**
TI-ONE 提供从数据预处理到模型部署的全流程服务，各个环节无缝衔接、流畅易用，从此用户不再需要为完成一个机器学习任务在不同的产品间切换，并疲于各种兼容性的问题。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/BnSNEaficFAYUbB24TSG1LjM5NJzYXVZMXCNsT5IDMwQOjvdhS6eGCzPGo9hnX8aY5rMoav8oQYnymv2HKuX0iaA/640?wx_fmt=png)
**拖拽式任务流**
通过图形化界面的拖拉拽操作，组件与节点间自动连线，数据流向自动生成，绘制任务流畅便捷。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/BnSNEaficFAYUbB24TSG1LjM5NJzYXVZMDOhrtJKfvQ6Mkzxj9eaFh8uQ22ajBpeoxkcLzyrPPVgXQk8QjQia8BQ/640?wx_fmt=png)
**多框架支持**
支持多种机器学习框架，方便用户按照自己的习惯与需求自由选择框架来搭建任务。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/BnSNEaficFAYUbB24TSG1LjM5NJzYXVZMHlZeAvGDC9iaN7mxBibuZrbIiaJm9MWxT1oosiaOFlbZYYnW8vFM2JbMRA/640?wx_fmt=png)
**丰富内置算法**
TI-ONE 平台内置丰富的各类算法，从数据预处理流程到各类机器学习和深度学习算法，AI 初学者们可以通过组件的拖拽在任务流中直接使用，无论是机器学习的回归预测、深度学习的图片分类、目标检测，用户不再需要编写晦涩难懂的算法代码即可解决各场景需求。当然，专业级用户也可选择自定义算法的上传。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/BnSNEaficFAYUbB24TSG1LjM5NJzYXVZMQUmiboPBHicZC2d9ibraaLUPn1VVMla7ynNbQs7cjydv5e2MSPibgI1N6A/640?wx_fmt=png)
**灵活的调度模式**
在完成了任务流的搭建，在任务调度环节，TI-ONE 支持对参数进行数值型与枚举型的预设定，自动完成多次任务，同时提供定时调度功能满足周期性的调度，大大减少手动多次调度的重复性工作量，在完成了多次运行后，用户可前往历史实例模块，快速横向比对所有模型，选择最优结果。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/BnSNEaficFAYUbB24TSG1LjM5NJzYXVZMbF09FEdNS8MpYLVbsfedwlUzSuA9fpKHDfePLmFmSRPibVIRHspbx0g/640?wx_fmt=png)
**结果可视化**
运行产生的模型结果，鼠标悬浮即可显示，同时 TI-ONE 内置丰富的模型评估方式和图标形式，让用户直观地对比多个结果，模型优劣一眼可辨。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/BnSNEaficFAYUbB24TSG1LjM5NJzYXVZM1Hsthy95VQgBXp2PpabVZsZJdicibkwulicNEErahQPmYH2r5L1VFnAJw/640?wx_fmt=png)
**一键部署服务**
TI-ONE 提供了一键式部署工具，用户可以将训练好的模型部署成 Application，然后装载成多个实例，一个实例还可以有多个版本。第三方的用户使用 REST API 便可轻松实现调用。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/BnSNEaficFAYUbB24TSG1LjM5NJzYXVZMCzvgmibPHqDp6TqzeLnH6rExLpooBnDGZibqX3qFiazaDfN3mfNXfkOZw/640?wx_fmt=png)
如此就完成了用 TI-ONE 构建任务流到发布模型服务的全流程。
**▌TI-A**
智能钛机器学习加速器（TI Accelerator，TI-A）是专为 AI 算法工程师设计的专业级机器学习平台。旨在为用户提供专业级的大规模训练工具，并提供极致的性能优化。
**命令行操作**
区别于 TI-ONE 图像化的界面风格，TI-A 提供命令行的操作界面，为专业级用户提供熟悉的工作环境。
**大规模训练**
几乎所有算法工程师团队都面临过大规模训练时速度太慢、计算集群成本高、IT 维护步骤复杂等困境。通过 TI-A 用户可以一键构建分布式 Tensorflow 框架，同时内部封装了 CNN、DNN、LSTM 等模型的分布式训练版本，大规模训练从此不再和“繁琐”相绑定。
**高性能**
TI-A 基于 Tensorflow 框架进行性能优化，对比原生框架不论是单机性能，还是多级多卡的性能都有显著的加速比提升，为用户提供更极致的性能体验。
**▌TI-S**
如何使整个机器学习过程更加自动化，减少数据科学家和 AI 算法专家在整个机器学习过程中的繁重劳动力，同时让更多没有 AI 背景的人也可以轻松使用 AI 技术，一直是机器学习领域热议的课题。如何能够把完整的机器学习工作流做成云端易用的产品，让用户仅需要在界面上拖拽样本数据就可以完成数据处理、特征抽取、模型训练等全流程？  
即将推出的智能钛自动机器学习（TI SelfLearning，TI-S）将全面支持结构化数据、图像等领域的 AI 建模，使非 AI 专业人士能够通过 TIS 获得人工智能决策的能力。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/BnSNEaficFAYUbB24TSG1LjM5NJzYXVZM221U3iayxbm5aX206CjFuuxAuV0cLqeJJoU5n0TUPNbcibDr9abaRwUg/640?wx_fmt=png)
TI-S 整体流程图
TI-S 提供从数据预处理、特征工程、模型调参、模型评估、模型预测到结果分析的一站式服务，全流程覆盖，实现从训练至预测的完整闭环，让用户无需算法经验也能基于场景数据生成模型，真正降低 AI 技术落地各行业的门槛。
同时，智能钛自主研发的自动数据清洗、自动择参、自动调参、自动特征选择、自动算法选择等技术，亦能高效辅助解决 AI 专业人员在工作各环节中繁琐、耗时的问题。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
