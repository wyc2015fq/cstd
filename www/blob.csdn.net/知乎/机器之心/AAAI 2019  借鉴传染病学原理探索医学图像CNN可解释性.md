# AAAI 2019 | 借鉴传染病学原理探索医学图像CNN可解释性 - 知乎
# 



机器之心发布，作者：牛钰浩、谷林（共同一作）, 陆峰（通讯作者） 等 。

> 虽然深度学习在医疗图像的病变检测上取得了巨大成功，然而，现有的方法几乎都将 CNN 作为黑盒使用，没有进一步揭示做出分类的依据。CNN 缺乏「可解释性」严重制约了其在医疗等涉及公众安全的关键领域应用。 北京航空航天大学联合日本国立信息学研究所以及中南大学湘雅医院，湘雅二医院对这一问题展开探索。参考传染病学金标准：「科赫法则」，本文提供了一种解释医疗图像深度学习的方法并发表在 AAAI 2019。通过了 5 位眼科医师双盲测试，本方法表现出了令人满意的性能。

回顾医学的历史，病菌感染曾一度困扰着人们：致病微生物也是看不见、摸不着的。微生物学鼻祖之一的罗伯特·科赫提出了一套科学验证方法——科赫法则（Koch's postulates），用于将某种病变与特定的病原体建立联系。这一方法随后成为传染病病原学鉴定的金标准。科赫也因此获得了 1905 年的诺贝尔生理学或医学奖。

这套准则分为四条：

1.	每一例患病动物体内都可以分离到该病菌。

2.	该病菌可以在体外培养数代。

3.	培养了数代的细菌可以通过接种，使健康的实验动物患上同样的疾病。

4.	被接种的动物中可以分离到同样的病菌。

自然界的微生物种类是天文数字，这其中仅有一小部分能够感染人类，而对人类致病的就更微乎其微了，科赫法则成为确定这些病原体的金标准并成为现代医学基础之一。检测医疗图像的 CNN 网络中也往往包含几十万乃至上百万参数。受此方法启发，可以使用类似的方法论来确定极少量的关键参数并研究 CNN 病变检测器的决策依据。

论文 Pathological Evidence Exploration in Deep Retinal Image Diagnosis 以糖尿病视网膜病变为例，展示了 CNN 检测器的决策依据，增进了医生与 AI 之间的相互理解。

**论文：Pathological Evidence Exploration in Deep Retinal Image Diagnosis**
![](https://pic4.zhimg.com/v2-24d77f35eb5aad3e3f0e60e2e14619cf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='858' height='194'></svg>)
论文链接： [https://arxiv.org/pdf/1812.02640.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1812.02640.pdf)
![](https://pic4.zhimg.com/v2-0f3439428f0271b5ac97052e916043a7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='851' height='339'></svg>)图 1：受柯赫准则启发，本论文对病变证据进行发掘、提取、重现
具体来说，本文首先在经典 CNN 病变检测算法中提取病理描述符，并利用一种 CNN 的可视化方法来定位病变（图 1.a）；模仿科赫法则中在无数微生物中分离病原体这一过程，本方法从百万级神经元中定位到数千维直接参与疾病诊断的参数并提取出描述符（图 1.b）；科赫法则接着将分离的微生物接种到健康生物体并观察产生的疾病症状，同样，本方法训练一个基于 GAN 的生成器，通过输入病理描述符中来合成眼底图（图 1.c, d），如果眼底图复现了相应的病变，就可以确认其与病理描述符的直接关系。

**病理描述符的提取**

本文以糖尿病视网膜病变（Diabetic Retinopathy，简称 DR）为例。全球范围内，有超过 4 亿糖尿病患者。根据 2016 年美国的统计，约有 40%-45% 的患者会发展出 DR 的症状。这种病变通常根据视网膜眼底图来进行诊断。DR 患者的眼底图上会出现不同程度的微动脉瘤、出血、渗出等病变。
![](https://pic1.zhimg.com/v2-bd15dc20a349bf72c08d090d2f64ce08_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='719' height='454'></svg>)图 2：DR 患者眼底图中的病变
Kaggle 于 2016 年举行了 DR 眼底图检测的竞赛。本文使用的冠军算法中的一种：o_O(Antony 2016)。仿造 Zeiler 和 Fergus 的 CNN 可视化方法，我们在检测器后接上了一个激活网络，用对称的结构来还原每个卷积层中被激活的神经元。
![](https://pic3.zhimg.com/v2-2106acb91657862a34888caca544d4de_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='868' height='263'></svg>)图 3：病理描述符的提取
具体来讲，为每一个卷积层构造一个转置卷积层，为每一个池化层构造一个反池化层，激活网络和检测网络共享权重。使用这样的网络架构，激活网络某一层的结果是从 #9 层的神经元激活结果翻译回来的，一定程度上代表了这一层的神经元对 #9 的影响。

图 4 将输入的眼底病变图和输出的激活图进行放大对比。输入的病变图经过一层层卷积，保留了病理特征，丢弃了血管等正常生理特征。由结果对病变的选择性，可以窥见 CNN 做出决策的证据。虽然在训练 CNN 检测器时仅仅使用了「病变图-严重程度」这样的图像级别标注，它仍能学到各种病变的特征，并且主动关注这些区域。
![](https://pic4.zhimg.com/v2-8f8b47f57fce9202a6262303ddf099ff_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='853' height='654'></svg>)图 4：输入的病变图和输出的激活图
接下来，根据病变参考图的特征层和激活层，定义一个病理描述符，用于代表每一个病变的信息。首先，对最后的激活图其进行阈值处理，得到二值掩码图，用于指示病变的位置信息。然后，通过寻找最大连通区域，可以得到每个病变的边界框。这一步操作即可将不同的病变分离开来。最后，使用边界框，在特征网络和激活网络的中间层中截取出一些特征块、激活块，将它们作为病理描述符（图 3）。这些病理描述符编码了完整的病变特征，可以为病变眼底图的合成提供关于病变的所有信息。




**病变眼底图的合成**

柯赫准则中，将不可见的纯化病菌注射到健康动物体内，可以使其患上相应的疾病。类似地，本文使用 GAN 来重现病理描述符中的特征、说明 DR 检测器提取出的病理描述符确实包含了病理信息。
![](https://pic3.zhimg.com/v2-d3a9f0fb477b8557490217fddcc5c122_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='888' height='421'></svg>)图 5：病变眼底图合成网络架构*图 5：病变眼底图合成网络架构*
本节的任务是训练一个合成病变图的生成器。生成器以血管分割图
![](https://pic1.zhimg.com/v2-0dc3e02750a2888873f364e4cfbc9ad4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='38' height='36'></svg>)
、噪声向量
![](https://pic1.zhimg.com/v2-f3b61a59e22b34c9920f587715b08064_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='36' height='31'></svg>)
为输入，通过一个 U-Net (Ronneberger, Fischer, and Brox 2015) 结构来合成出包含特定病变的眼底图。

训练生成器的时候，使用的数据集是正常眼底图
![](https://pic1.zhimg.com/v2-563821f98232d7591d6441461a7dfae4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='38' height='33'></svg>)
和它的血管分割图
![](https://pic1.zhimg.com/v2-0dc3e02750a2888873f364e4cfbc9ad4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='38' height='36'></svg>)
。而病变信息，全部来自于之前提取出的病理描述符。计算 Loss 时，设置了多个子网络来达到不同的目标。




**子网络 1：判别器网络**

判别器子网络判别一张图片是真实图
![](https://pic1.zhimg.com/v2-563821f98232d7591d6441461a7dfae4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='38' height='33'></svg>)
还是合成图。进而计算出「生成对抗 Loss」，使生成器
![](https://pic4.zhimg.com/v2-fd3283b875fcdbe0a7f6625c64f9975f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='43' height='38'></svg>)
和判别器
![](https://pic3.zhimg.com/v2-09e68796997d2de4310a37c0fc8ca192_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='43' height='41'></svg>)
相互竞争，增加合成眼底图的真实性。对于生成器、判别器，有 
![](https://pic2.zhimg.com/v2-3d3897b1809c96b840b7d54508c30ead_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='855' height='52'></svg>)
。

**子网络 2：视网膜细节网络**

视网膜细节网络使用 VGG 网络来提取健康眼底图
![](https://pic1.zhimg.com/v2-563821f98232d7591d6441461a7dfae4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='38' height='33'></svg>)
与合成眼底图
![](https://pic2.zhimg.com/v2-44be400cd0c67c26a1d3e826ccbca469_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='28' height='36'></svg>)
的特征，做差得到「视网膜细节 Loss」，让合成图像的特征与其接近，保证合成图片的生理细节。用
![](https://pic3.zhimg.com/v2-a824d4c486f8ae16c57203e2eb53275a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='40' height='40'></svg>)
来代表 VGG 特征提取函数，则视网膜细节 Loss 为 
![](https://pic1.zhimg.com/v2-ef4e8f969048e579441e43f8db648c90_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='397' height='46'></svg>)
。




**子网络 3：DR 检测网络**

DR 检测子网络用来计算最重要的「病理 Loss」，使合成的眼底图中出现病变。

计算病理 Loss 的时候，将合成的病变图输入到 DR 检测器网络中，从想要合成病变的地方截取出特征块，与病理描述符中的相应特征块进行对比、做差即可得到合成病变与真实病变的差异。利用病理 Loss，能够让合成图在特征层的与病变相关的神经元激活，从而保证病变的复现。

使用
![](https://pic4.zhimg.com/v2-0edc02d843916890eca69c18c6c63993_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='28' height='28'></svg>)
代表合成图的特征块，用
![](https://pic1.zhimg.com/v2-ba9f6a9b9395bc77c0a540406606a8b4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='31' height='31'></svg>)
代表描述符中的特征块，则病理 Loss 为
![](https://pic4.zhimg.com/v2-749d9bbd40b6c5a58dee0b92fd4f6bd3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='523' height='52'></svg>)
。 计算过程中使用了 Gram 矩阵来保留样式而非内容：
![](https://pic1.zhimg.com/v2-a0cdcd253c72041831cf8bc870aa6734_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='436' height='52'></svg>)
。

**合成结果**

在测试阶段，文章使用了 DRIVE 数据集 (Staal et al. 2004) 作为正常眼底图，使用 STARE (Hoover, Kouznetsova, and Goldbaum 2000) 和 Kaggle 中的图像作为病变参考图，进行了多组实验。

图 6 是使用不同的病变参考图来合成出的结果。根据专业眼科医生的评估，前两列的微动脉瘤合成较好，第三列合成的激光瘢痕与参考图一致，不过第四列合成的硬性渗出有些欠真实。 
![](https://pic3.zhimg.com/v2-2de5a58ddab9480e68477d02ce748c7e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='846' height='815'></svg>)图 6：不同病变的合成效果
为了进一步说明合成结果的先进性，我们将本文方法与 Zhao 等人的 Tub-sGAN 生成的图片随机混合，并组织了 5 位专业的眼科医生，对其进行双盲评测。对于每幅合成图像，医生为其进行三个评分：1. 眼底图的真实性，2. 合成病变的真实性，3. DR 的严重程度（评分范围是 1-10，得分越高越好）。最后收集到对 560 组评分，其平均值如表 1 所示。本文方法在合成的真实性上有明显优势。 
![](https://pic3.zhimg.com/v2-299f2433113f28bcaba1b98a6a3b422e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='608' height='194'></svg>)
病理描述符针对每个病变单独存储，这种设计使其应用变得容易操控。对病变的筛选、复制均可以通过对病理描述符的操作来实现，并可以进一步做到数量控制、合成病变的位置控制。具体操控效果如图 7 所示。 
![](https://pic1.zhimg.com/v2-a307e4b034ea2af4ceb8408cd0f78314_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='853' height='392'></svg>)图 7：对病变位置和数量的操控![](https://pic4.zhimg.com/v2-87da9365e379267cbd40b1de678663cf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='654' height='421'></svg>)图 8：病变数量增加时，DR 检测器给出的严重程度也会相应提升
本文的生理细节 Loss，选用 VGG 来进行高层特征的计算，而非直接使用 L1 Loss，能够正确的保留视盘边界等生理细节。与 Zhao 等人的 Tub-sGAN 不同，本文的病理 Loss 作用在病变局部，不会影响生理细节的正常合成（图 9）。 
![](https://pic2.zhimg.com/v2-a4d556688ccd5270c7d08bf0981ed8c1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='836' height='447'></svg>)图 9：生理细节保留对比
总体来看，本文的合成方法仅仅使用了参考图中的病变特征，忽略了参考图的整体色调、外观。若在此合成方法之后再增加一个 Deep Photo Style Transfer (Luan et al. 2017) 风格迁移，则会更进一步增加参考图与合成图在外观上的相似程度（图 10）。
![](https://pic4.zhimg.com/v2-6fbf5871953c69a2dd3e457c13fd4a13_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='858' height='499'></svg>)图 10：增加风格迁移后处理以后的效果
**总结**

本文提出了通过激活神经元来检测和编码病变特征的方法，该过程与决策证据紧密相关。

本文使用一种基于 GAN 的方法来将病理描述符可视化，合成带有特定病变的眼底图，且病变合成的方法具有一定的可操控性。合成的结果得到了医生的认可。

文章为 CNN 的可解释性提供了一种研究思路，可激发更一般的探索。


