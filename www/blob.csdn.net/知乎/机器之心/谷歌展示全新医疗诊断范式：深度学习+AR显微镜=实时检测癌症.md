# 谷歌展示全新医疗诊断范式：深度学习+AR显微镜=实时检测癌症 - 知乎
# 



选自Google Research Blog，作者：Martin Stumpe、Craig Mermel，机器之心编译。

> 今天，谷歌在美国癌症研究协会（AACR）年会上发表演讲，介绍癌症检测的新研究论文。他们开发了一种增强现实显微镜（ARM）原型平台，可实时检测癌症。该平台具备相当大的灵活性，适用于多种显微镜、任务类型的配置。

近期，深度学习在医疗领域的应用（包括眼科、皮肤科、放射科和病理科（pathology））显示了极大的前景，提高全世界高质量医疗的准确率和可用性。谷歌也发布了[研究结果](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650723875%26idx%3D2%26sn%3D4ed962e9e7db1e86550d65833c9cc539%26chksm%3D871b125db06c9b4b9b0d221a17b9a890a4f9707351a10bf33ade8af7bf5b13a3a181a14a380e%26scene%3D21%23wechat_redirect)（Detecting Cancer Metastases on Gigapixel Pathology Images），展示卷积神经网络能够检测乳腺癌在淋巴结中的转移，准确率堪比训练有素的病理医生。但是，由于使用复式光学显微镜得到直接组织可视化仍然是病理医生诊断疾病的主要手段，因此深度学习在病理科的传播和采用的关键阻碍是对微观组织数字表征的依赖。

今天，谷歌在美国癌症研究协会（AACR）年会上发表演讲，介绍论文《An Augmented Reality Microscope for Real­time Automated Detection of Cancer》（under review）。演讲介绍了一种增强现实显微镜（ARM）原型平台，有望帮助加速和促进世界各地病理医生对深度学习工具的采用。该平台包含改良光学显微镜，可以实时地将图像分析和机器学习算法结果直接呈现。重要的是，ARM 平台可以利用低价、易获取的组件进行改造，以适应世界各地医院、诊所的现有光学显微镜，且无需分析组织的整张幻灯片数字图像。

论文地址：[https://drive.google.com/file/d/1WRBCqJItaGly-9PDSMlwQ5Ldhc8lB0lf/view](https://link.zhihu.com/?target=https%3A//drive.google.com/file/d/1WRBCqJItaGly-9PDSMlwQ5Ldhc8lB0lf/view)

现代计算组件和深度学习模型（如建立在 TensorFlow 之上的模型）使得该平台可以运行多种预训练模型。在传统的模拟显微镜（analog microscope）中，用户通过目镜观察样本。机器学习算法可以实时地将它的输出投影回光学路径上。这种数字投影在视觉上是叠加在样本的原始（模拟）图像上的，以帮助观察者定位或量化感兴趣的特征。重要的是，其计算和视觉反馈的更新是很快的，目前的实现以大约每秒 10 帧的速率运行，因此随着用户通过移动幻灯片和/或改变放大倍数来扫描组织，模型输出可以无缝地进行更新。
![](https://pic1.zhimg.com/v2-1e47b6dcac86899285c1b77686a47dc0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='266'></svg>)
左：ARM 的概述图。数字摄像头捕捉和用户相同的视野，并将图像传递给附加的计算单元，以运行机器学习模型进行实时推断。结果将被反馈给自定义的 AR 显示器，该显示器和目镜串联，可将模型输出投影到和幻灯片相同的平面上。右：设备原型，已被改造成典型的临床应用级别光学显微镜。 




原则上，ARM 可以提供多种视觉反馈，包括文本、箭头、等高线、热图或动画，并且可以运行多种不同的机器学习算法来解决不同的问题，如目标检测、量化或分类。

作为 ARM 的潜在效用的展示，谷歌将其用于运行两种不同的癌症检测算法：一个在淋巴结样本上检测乳腺癌转移，另一个在前列腺切除术样本上检测前列腺癌。这些模型可以在 4-40 倍的放大率上运行，给定模型的输出结果通过绿线描绘的检测到的肿瘤区域进行展示。这些轮廓可以帮助病理医生集中注意力到感兴趣的区域上，而不需要遮挡潜在的肿瘤细胞外观。
![](https://pic1.zhimg.com/v2-2f6a5fdebce3c73d90287fda909bfc54_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='342'></svg>)
通过 ARM 目镜观察到的图像示例。这些图像展示了淋巴结癌细胞转移示例，显微镜物镜分别是 4x、10x、20x 和 40x。 




尽管两个癌症模型都基于光学配置明显不同的完整幻灯片扫描仪所提供的图像进行训练，但是这些模型在 ARM 上的效果非常好，且无需额外的再训练。例如，淋巴结癌细胞转移模型的曲线下面积（AUC）为 0.98，在 ARM 上运行的前列腺癌模型的 AUC 是 0.96，只比 WSI 上获取的性能稍有下降。谷歌认为这些模型的性能可以通过在 ARM 上直接获取的图像上进行额外训练而得到提升。

谷歌认为 ARM 可以对世界医疗状况产生巨大影响，尤其是发展中国家的传染病诊断，如肺结核、疟疾等。此外，即使对于采用了数字化病理诊断工作流程的医院，ARM 也可与数字化工作流程结合使用，数字化工作流程中扫描仪仍然面临很大挑战或者仍然需要快速周转（如细胞学检查、荧光成像或术中冰冻切片）。当然，光学显微镜在病理科之外的很多方面也被证明有效，谷歌认为 ARM 可以得到大范围的应用，如医疗、生命科学研究和材料科学。谷歌将继续探索 ARM，帮助加速机器学习技术对世界的积极影响。
![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)
原文链接：[https://research.googleblog.com/2018/04/an-augmented-reality-microscope.html](https://link.zhihu.com/?target=https%3A//research.googleblog.com/2018/04/an-augmented-reality-microscope.html)


