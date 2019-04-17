# AMD在机器学习领域追击英伟达：发布Vega新卡，性价比是N厂两倍 - 知乎
# 


![](https://pic1.zhimg.com/v2-49952b32bc3000b97fe9dd97cb683a64_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1280' height='664'></svg>)李杉 编译整理
量子位 报道 | 公众号 QbitAI

A卡和N卡之间的战火，正在机器学习领域不断延烧。

上周，英伟达刚刚发布了性能提升5倍的GPU新架构Volta、Tesla V100深度学习加速处理器以及搭载Volta GPU的新超级计算机DGX-1、工作站DGX Station。

今年年初就在CES透露了新GPU架构的AMD，有点坐不住了。

在今天的财务分析师会议上，他们展示了公司接下来在数据中心、CPU、显卡等领域的一系列新品发布计划，其中关注度最高的，是Vega架构的第一款新卡Radeon Vega Frontier Edition。

另外，AMD还公布了在机器学习ROCm开源软件栈方面取得的进展，在提及Vega Frontier的用途和目标用户时，也处处强调机器学习。

# Vega新卡![](https://pic1.zhimg.com/v2-00c0f076b7429df8d2604634ce626298_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1280' height='466'></svg>)

Radeon Vega Frontier Edition是一个用于工作站的GPU，瞄准机器学习开发环境。AMD表示，Vega的战略是提供比英伟达Pascal更好的性能，而性价比则达到后者的两倍。


AMD高级副总裁、首席架构师Raja Koduri在Vega Frontier的介绍文章中称，在机器学习和专业图形领域，它是全球最快GPU。

下表，是目前已知的Vega Frontier参数：


计算单元        64单精度计算性能(FP32)       ~13 TFLOPS半精度计算性能(FP16)       ~25 TFLOPS像素填充         ~90 Gpixels/sec内存容量       16 GB高带宽缓存内存带宽       ~480 GBs/sec
《福布斯》网站把Vega Frontier和英伟达Tesla P100的性能进行了对比，结果如下：![](https://pic3.zhimg.com/v2-77c60190f1639e5db834ee2c2130ed8e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='960' height='367'></svg>)

虽说Vega Frontier性能超越了英伟达上一代PASCAL架构的P100，不过与刚刚发布的Volta架构Tesla V100相比，还有一定的距离。

根据英伟达上周宣布的参数，V100的单精度计算性能(FP32)和半精度计算性能(FP16)分别是15TFLOPS和30TFLOPS。

AMD希望吸引那些想要评估Vega机器学习产品的研发人员，帮助其开发开源软件和库。

值得注意的是，AMD希望通过两种方式来降低拥有成本：一是基于Zen的Naples服务器CPU具备较低的价格和较高的I/O容量，二是高性价比的Vega机器学习GPU。

AMD去年12月发布了Radeon Instinct MI6、MI8和基于Vega的MI25，同时表示Vega有望在2017年中发货。MI25的定位是对计算密集型任务展开训练，对手则是基于Pascal的英伟达Tesla P100。

《福布斯》评论说，这些新的Vega GPU似乎将具备与备受赞誉的英伟达PASCAL GPU比肩的性能。与AMD开发的优化版ROCm机器学习软件配合后，该公司向着进军这一市场迈出了坚实的第一步。

但最终还要取决于ROCm开源软件利用硬件的潜在性能发挥实际价值的能力。

# ROCm机器学习软件

AMD去年开始利用开源Linux驱动器、库和工具打造关键的机器学习软件生态系统。该公司已经展示了这项工作取得的良好进展，这对于把硬件的理论配置转化成应用级人工智能性能至关重要。

AMD在该公司的财务分析师大会上透露，随着库的优化，该公司的软件性能自从11月以来提升了13至16倍。

AMD高管还规划了软件产品路线图，详细阐述了将于今年夏天发布的Caffe2、Tensorflow、PyTorch和MXNet，从而对Vega形成支持，另外还有经过优化的ROCm解算器。![](https://pic3.zhimg.com/v2-df4ef79b81cf854b44afc036647adc3a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='960' height='517'></svg>)

**△** 从去年11月到今年3月，AMD通过优化机器学习库，将软件性能提升了13-16倍

# 结论

AMD认识到英伟达在机器学习市场大幅领先的地位，并且花费数年时间研发了软件和硬件，而且刚刚发布了新的高端Volta GPU。但机器学习市场仍然很年轻，增长很迅猛，AMD仍拥有与英伟达竞争的空间。

因此，AMD还是要进入这个市场，追击一下英伟达。

AMD似乎正在硬件、软件和开发者关系上努力打好基础，希望借此吸引企业用户。该公司已经完全调整和优化了软件栈，并且设计了新的GPU，针对机器学习算法进行了优化。

最重要的是，决定开发开源软件使得整个行业生态系统都可以为该公司的发展计划提供动力。

AMD现在需要完美执行这些计划，打造一套生态系统，然后与塑造整个行业发展的关键企业建立关系。该公司能否利用它的半定制业务单元（Semi-Custom Business Unit）在自动化机器人和汽车领域吸引一两个合作伙伴，将非常值得关注。

事实上，AMD之前就是凭借这种方式成为了游戏机市场的领导者。

AMD认识到，想要在这个利润丰厚的市场开发有竞争力的硬件和软件生态系统，需要经过长期努力，该公司目前正在小心翼翼地迈出第一步。他们接下来的步骤将非常值得关注。


> 
### 招聘

量子位正在招募编辑记者、运营、产品等岗位，工作地点在北京中关村。相关细节，请在公众号对话界面，回复：“招聘”。

### One More Thing…

今天AI界还有哪些事值得关注？在量子位（QbitAI）公众号对话界面回复“今天”，看我们全网搜罗的AI行业和研究动态。笔芯~

另外，欢迎加量子位小助手的微信：qbitbot，如果你研究或者从事AI领域，小助手会把你带入量子位的交流群里。



