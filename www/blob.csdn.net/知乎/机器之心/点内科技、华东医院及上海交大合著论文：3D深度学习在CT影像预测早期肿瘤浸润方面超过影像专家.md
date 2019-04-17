# 点内科技、华东医院及上海交大合著论文：3D深度学习在CT影像预测早期肿瘤浸润方面超过影像专家 - 知乎
# 



机器之心发布，来源：点内科技。

> 本研究利用高效的、多任务的 3D 卷积神经网络 DenseSharp，同时进行分类和分割，旨在研究从 CT 影像预测早期肿瘤病理浸润深度学习和放射专家的准确度。
![](https://pic4.zhimg.com/v2-59ae2ef574bbbab9665ea3f4b1a099cb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='817' height='246'></svg>)
点内科技、复旦大学附属华东医院「张国桢肺微小结节诊治中心」和上海交通大学「SJTU-UCLA 机器感知与推理联合研究中心」组成的联合研究团队共同合作的科研成果「3D Deep Learning from CT Scans Predicts Tumor Invasiveness of Subcentimeter Pulmonary Adenocarcinomas」发表于美国癌症研究协会（American Association for Cancer Research，AACR）会刊《Cancer Research》期刊上，华东医院李铭教授为本篇论文的通讯作者，赵伟博士和杨健程博士为共同第一作者。

《Cancer Research》创办于 1916 年，为国际肿瘤研究领域引用率最高的权威期刊之一，主要发表包括基础研究、临床前及临床、肿瘤预防及生物治疗在内的肿瘤学原创研究论文和综述文章，具有很高的国际影响力，2017 年影响因子高达 9.13。这一成果的发布是 AI 医疗领域重要的里程碑，意味着机器学习工具开始在医疗实践中产生重要影响，该研究能帮助医生选择早期肺癌的治疗方法，将有利于推动精准医疗。




**3D 卷积神经网络 DenseSharp 在医学影像领域的应用**

论文「3D Deep Learning from CT Scans Predicts Tumor Invasiveness of Subcentimeter Pulmonary Adenocarcinomas」于 2018 年 10 月 2 日在线发表，文章利用深度学习的方法对像素级标注的亚厘米肺腺癌 CT 数据和其病理结果标注进行训练，并通过多任务的卷积神经网络对亚厘米肺腺癌的浸润风险程度进行自动术前预测。通过建立医疗影像上的 taskonomy（任务谱），逐步使医疗影像的研究脱离西西弗斯式的悲剧（Sisyphean challenge），合理的任务配置将会极大降低模型的学习难度、迁移泛化能力、稳定性和可靠性。该模型基于 3D DenseNets，配合多任务学习，是参数高效（parameter-efficient）的 3D 卷积神经网络。训练完成后，模型只需要常规的 CT 数据，不需要肺结节分割、大小以及任何预先定义的信息。我们的多任务学习模型显著优于单任务模型，并且 3D 模型也显著优于其 2D 变种。为推进可复现的人工智能研究，基于 Keras 的模型代码开源（Apache-2.0 License）在 [https://github.com/duducheng/DenseSharp](https://link.zhihu.com/?target=https%3A//github.com/duducheng/DenseSharp)。

在 128 例测试集上，多任务深度学习模型预测的结果优于 4 位放射科医生（两位高年资医师和两位低年资医生）的评价结果；我们训练的多任务深度学习模型在区分浸润/非浸润两分类的准确率达到了 78.8%（AUC），区分 IAC/非 IAC（0 期/I 期）两分类的准确率达到了 88.0%（AUC），区分 AAH-AIS/MIA/IAC 三分类的准确率达到了 63.3%（F1）。需要说明的是，本文纳入的亚厘米肺结节大部分为肺磨玻璃结节，这种类型的结节，特别是亚厘米磨玻璃结节，在 CT 图像上由于传统的恶性征象较少出现，浸润前病变和浸润性病变影像表现重叠较高等特征，诊断十分困难，在三分类的诊断上，高年资医师的诊断正确率也只有 56.6%，而我们的深度学习的准确率可达到 63.3%，由此可见深度学习在处理这类问题时的优势与前景。本文从构思到发表，经历了数据采集、像素级标注、数据处理、模型开发训练、模型测试、公共数据集申请、下载、标注、测试、论文的攥写、修改、同行评阅、修回等过程，点内的联合研究团队只用了不到 9 个月时间便完成了算法开发测试及论文发表工作。



![](https://pic3.zhimg.com/v2-690d471d44f5cebf6662049141b9a152_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='689' height='386'></svg>)
参考链接：

[http://cancerres.aacrjournals.org/content/early/2018/10/02/0008-5472.CAN-18-0696](https://link.zhihu.com/?target=http%3A//cancerres.aacrjournals.org/content/early/2018/10/02/0008-5472.CAN-18-0696)


