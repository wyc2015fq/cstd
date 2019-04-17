# DeepMind眼疾诊断里程碑：临床专家级、「解决」黑箱问题 - 知乎
# 



**选自DeepMind，作者：Mustafa Suleyman，机器之心编译，机器之心编辑部。**

> DeepMind 刚刚发布博客宣布其与伦敦摩尔菲尔兹眼科医院（Moorfields Eye Hospital）联合研究的第一阶段成果，该成果将改变眼部疾病的治疗。他们将深度学习应用到眼部疾病的诊断，并为患者与医师提供可解释性的分析图，因而人类能理解深度网络到底是如何诊断眼部疾病的。

目前，该研究结果已在 Nature Medicine 上在线发布，展示了其 AI 系统能够对常规临床实践中的眼球扫描结果进行快速诊断，准确率打破以往记录。它可以准确推荐患有 50 余种危害视力的眼病患者进行转诊治疗，准确程度堪比世界顶级的专家医生。

这只是初步结果，不过它也展示了该系统能够应对常规临床实践中的大部分病人状况。从长期来看，DeepMind 希望这可以帮助医生快速分辨需要紧急治疗的病人，以划分优先级，尽可能地保住病人的视力。




**流程更简化**

目前，眼部护理专家使用光学相干断层扫描（OCT）来帮助诊断眼睛状况。这些 3D 图像能够为眼睛后部提供详细的图像，但是它们很难阅读，需要专家分析才能判别眼睛状况。

分析这些扫描图像所需时间以及专家浏览图像的数量之多（仅摩尔菲尔兹眼科医院每天就有 1000 张此类图像）使得扫描和治疗之间出现了较长的间隔，即使是对于那些需要紧急治疗的人来说。如果这些病人病况突变，比如眼睛后部出血，那么这样的拖延甚至可能导致病人丧失视力。

DeepMind 开发的系统致力于解决这一挑战。它不仅能够在数秒内自动检测出眼疾的特征，还能够优先推荐需要紧急治疗的病人进行转诊治疗。这一即时归类过程大幅降低扫描和治疗之间的时间间隔，帮助患有因糖尿病引起的眼疾和老年黄斑变性的病人避免丧失视力。
[DeepMind Nature01_腾讯视频​v.qq.com![图标](https://pic3.zhimg.com/v2-a772a2982020f0c43d39432a93d041da_180x120.jpg)](https://link.zhihu.com/?target=https%3A//v.qq.com/x/page/r1346glwk50.html)
**可适应技术**

DeepMind 不仅希望这在学术上是一个有趣的结果，他们同时希望该技术能用于实际治疗中。所以 DeepMind 的论文也承载着 AI 在临床实践中所遇到的关键障碍：「黑箱」问题。对于大多数 AI 系统而言，我们很难精确理解为什么算法会作出对应的推荐。这对于需要理解系统推理的临床医生和患者而言是非常大的问题，他们不仅需要知道结果，同时还需要知道如何以及为什么得出这样的结果。

DeepMind 的系统采用了一种新颖的方式来解决该问题，该系统通过在两个不同的神经网络间插入可解释性表征而将它们结合起来。第一个神经网络是分割网络，它能分析 OCT 扫描，从而提供不同类型的眼组织图和观察到的疾病特征，例如出血、病变或其它眼病症状。这种图能帮助眼科专家深入了解系统到底是怎么「思考」的。第二个神经网络为分类网络，它能分析特征图，为临床医生提供诊断意见和转诊推荐。重要的是，该神经网络会将这种推荐表示为百分比，因此临床医生能了解系统对其分析的置信度。
![](https://pic3.zhimg.com/v2-29cc05ec8429191ebf6b85abff5b1556_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='607'></svg>)AI 如何帮助医生检测眼疾
该功能至关重要，因为眼部护理专家通常在确定护理类型和治疗方法时起到关键作用。让专家监督该技术的推荐结果是使该系统实际可用的关键。

此外，该技术可以轻松应用于不同类型的眼部扫描仪，不局限于在摩尔菲尔兹眼科医院训练时所使用的设备类型。这似乎无关紧要，但它意味着该技术可以相对轻松地在全世界推广，大幅提高受益病人的数量。这还能确保该系统在医院或其他临床环境中的 OCT 扫描仪升级换代后依然能够使用。
[Nature Medicine02_腾讯视频​v.qq.com![图标](https://pic3.zhimg.com/v2-a772a2982020f0c43d39432a93d041da_180x120.jpg)](https://link.zhihu.com/?target=https%3A//v.qq.com/x/page/m134654jzvr.html)
该视频展示了用户与 OCT viewer 的互动。




**下一阶段**

尽管 DeepMind 称对此研究进展感到非常骄傲，但是这项初始研究需要被转化为产品，并在实际应用之前先经过严谨的临床试验以及监管部门批准。而 DeepMind 相信，该系统将改变眼部疾病的诊断、治疗和管理方式。

DeepMind 在 Moorfields 的合作伙伴希望 DeepMind 的研究能够帮助他们改善护理过程，减少医生的压力，以及降低成本。因此 DeepMind 也将在未来计划中投入大量的精力。

如果这项技术在临床试验中能被证明其普遍有效性，Moorfields 的临床医生将能在最初的五年内，于他们在英国的所有 30 所医院和社区诊所中免费使用它。这些诊所每年服务 30 万名病人，每天接收 1000 份 OCT 扫描转诊，所有这些工作都能因这项技术得到诊断准确率和速度的提升。

该项目中实现的工作将帮助加速许多其它的 NHS（英国国家医疗服务体系）研究工作。Moorfields 拥有的原始数据集适合医生使用，但不适用于机器学习研究。因此 DeepMind 投入了大量精力对数据集进行清理、整理和标注，以为全世界的眼部疾病研究创建最佳的 AI-ready 数据库。

改进后的数据库由 Moorfields 作为非商业公开资产持有，并且已被医院研究人员用于大范围疾病的九项独立研究中，未来该范围还将扩展。Moorfields 也可以在他们未来的非商业研究项目中使用 DeepMind 的已训练模型。

自 2016 年和 Moorfields 签订协议以来，对于 DeepMind 所有参与该项目的人而言，这项研究是一个相当令人激动的里程碑，并成为医生和技术专家合作潜力的另一个象征。DeepMind 也将继续更新他们的研究成果。




**论文：Clinically applicable deep learning for diagnosis and referral in retinal disease**
![](https://pic3.zhimg.com/v2-966259f40aa1a0b6e9e41e837fea664e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='579'></svg>)
论文地址：[https://www.nature.com/articles/s41591-018-0107-6](https://link.zhihu.com/?target=https%3A//www.nature.com/articles/s41591-018-0107-6)

**摘要：**诊断成像的体量和复杂性比人类专业知识的增长速度更快。人工智能技术在对一些常见疾病的二维照片进行分类方面潜力巨大，且通常依赖于数百万带注释图像的数据库。到目前为止，通过在现实世界的临床方法进行三维诊断扫描还无法媲美专业临床医生的表现。在本文中，我们将一种新的深度学习架构应用于一组临床上异质性的三维光学相干断层扫描，这些扫描来自一家大型眼科医院的患者。在仅接受了 14884 次扫描训练后，我们的转诊建议在一系列威胁视力的视网膜疾病诊断中达到或超过了专家的表现。此外，我们证明了：我们的体系架构产生的组织分割可作为一种独立于设备的表征；当使用来自不同类型设备的组织分割时，可以保持推荐准确性。我们的研究消除了以前存在的大范围临床使用的障碍，也不需要来自现实环境中多种疾病的庞大训练数据。
![](https://pic3.zhimg.com/v2-05749b8fc199312a6247fbcfade525da_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='599'></svg>)
图 1：我们提出的 AI 框架。a. 原始视网膜 OCT 扫描（视网膜黄斑周围 6 × 6 × 2.3 mm³）。b. 深度分割网络，用手动分割的 OCT 扫描图像进行训练。c. 得到的组织分割图。d. 深度分类网络，使用带有确诊和最佳转诊决定的组织图进行训练。e. 预测的诊断概率和转诊建议。
![](https://pic4.zhimg.com/v2-fef8d725726fec1dad6ee53cbc872c33_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='731'></svg>)
图 2：分割网络结果。从 n=224 OCT 扫描中选择的三个二维切片，其中左图为分割测试集、中间为手动分割、右边为自动分割。
![](https://pic2.zhimg.com/v2-2034fff790d6d38514627c13503c802d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='1347'></svg>)
图 3：病人转诊决策的结果。在 n=997 名病人的独立测试集上得到的性能结果（252 名紧急、230 名次紧急、266 名常规、249 名仅观察）。a. 紧急转诊（脉络膜新生血管，CNV）vs. 所有其它转诊的 ROC 曲线图。b. 带有患者编号的混淆矩阵，转诊决策由 DeepMind 框架和两位顶级视网膜专家分别做出。c. 转诊决策的总误差率（1 表示准确）。浅蓝色区域（3.9–7.3%）外的值与框架表现（5.5%）非常不同（95% 的置信区间，使用一个双重精确二项检验）。AUC 为曲线下区域面积。
![](https://pic3.zhimg.com/v2-750f2e5bba62e37d533a3a60bba01dba_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='794'></svg>)
图 4：扩展到新的扫描设备类型。
![](https://pic1.zhimg.com/v2-2c6ba73bae200bc59e38fd32d6d527e0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1029' height='780'></svg>)
图 5：将分割结果可视化为等厚图（thickness map）。
![](https://pic2.zhimg.com/v2-2e098dd94981aff65deaf635d63edd4d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='623'></svg>)
表 1：在两种设备类型上完成的训练扫描数以及获得的性能。
![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)
原文链接：[https://deepmind.com/blog/moorfields-major-milestone/](https://link.zhihu.com/?target=https%3A//deepmind.com/blog/moorfields-major-milestone/)


