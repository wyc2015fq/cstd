# Neuromation新研究：用卷积神经网络进行儿童骨龄评估 - 知乎
# 



**选自Medium，作者：Tete Xiao、Yingcheng Liu、，Bolei Zhou、Yuning Jiang、Jian Sun。**

> 近日，Neuromation 团队在 Medium 上撰文介绍其最新研究成果：利用卷积神经网络（CNN）评估儿童骨龄，这一自动骨龄评估系统可以得到与放射科专家相似或更好的结果。该团队评估了手骨不同区域，发现仅对掌骨和近端指骨进行评估，得到的结果与对整个手骨进行评估的结果相差无几。为了克服放射图像的质量和多样性问题，该团队引入了严格的清理和标准化过程，以增强模型的鲁棒性和准确率，提升骨龄评估的准确率、提高结果的可复现性以及临床医生的效率。

Alexander 的论文《Pediatric Bone Age Assessment Using Deep Convolutional Neural Networks》（Vladimir Iglovikov、Alexander Kalinin 以及 Alexey Shvets 联合完成的一项工作）已经发表在第 4 届 Deep Learning in Medical Image Analysis（DLMIA 2018）Workshop 上。这不是 Neuromation 在医学影像方面发表的第一篇论文，但这是一个深入了解该研究细节的好机会。与我们之前发表的关于医疗概念规范化的文章（[https://medium.com/neuromation-io-blog/neuromation-research-medical-concept-normalization-in-social-media-posts-f06504200cec](https://link.zhihu.com/?target=https%3A//medium.com/neuromation-io-blog/neuromation-research-medical-concept-normalization-in-social-media-posts-f06504200cec)）相似，这是一件严肃且相当复杂的事。
![](https://pic3.zhimg.com/v2-2d4794d9aa8aa6ebab8bc443ad69225e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='947' height='526'></svg>)



**骨龄反映了你的年龄：骨龄评估**

骨龄，基本上就是指你的骨头看起来多大了。随着孩子的成长，他的骨骼也在逐渐成长。这就意味着你可以通过观察儿童的骨骼，根据具备此类骨骼的儿童的平均年龄来了解这个孩子多大了。这时候你可能会想这是不是一篇考古学方面的文章：毕竟活着的小孩应该并不怎么拍 X 光。

这么想是对的，但是也不对。正常发育的话，儿童的骨龄应该在生理年龄（日历年龄）的上下 10% 区间内波动。但也有例外，例如，孩子青春期的生长突增可能会影响骨龄。因此，如果骨龄比生理年龄大几年的话，该儿童就会提前停止生长，而如果骨骼比较「年轻」的话，则可能会出现延迟的生长突增。此外，在给定身高和骨龄的情况下，你可以准确预测这个孩子的成年身高，这也可以派上一些用场：如果你的孩子喜欢篮球，你可能想知道他未来身高能否超过 7 英尺。

还有一些例外情况：骨龄和生理年龄严重不匹配是预示着身体出现问题，如成长障碍和内分泌问题等。骨龄只能告诉医生患者在特定时间时骨骼的相对成熟度，还要和其他临床指标结合才能区分出是正常生长还是生长相对提前或迟缓。连续的骨龄读数可以说明儿童的发育方向或治疗进展。通过评估骨龄，儿科医生可以诊断儿童发育过程中的内分泌和代谢紊乱问题，如骨发育不良或受营养、代谢或其他未知因素影响的生长缺陷，这可能会损害骨骺或骨质成熟。在这种生长迟缓情况下，骨龄和身高可能会有相同程度的延迟，但是经过治疗，此类儿童仍可达到正常成年人的身高。

综上所述，儿科医生常常会用 X 光照射儿童手部来评估其骨龄……所以自动化是一个很大的问题。




**读手骨：从手掌和手腕评估骨龄**

骨骼成熟度主要是通过骨骺中次级骨化中心的发育和骨化程度来评估的。几十年来，评估骨骼成熟度通常是根据对手骨和手腕的骨骼发育情况进行可视化评估来进行的。以下是放射科医生在查看手部 X 光片时看到的内容：
![](https://pic1.zhimg.com/v2-2601de2e48b34451e90be68a1c54ad04_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='605' height='766'></svg>)
现在评估骨龄的两种最常用的技术是 Greulich-Pyle 法和 Tanner-Whitehouse（TW2）法。这两种方法都是根据成熟度指标，用左手手掌和手腕的放射图像评估骨骼成熟度，即管状骨的骨骺从骨化的最早阶段到它们和骨干融合的过程中放射影像呈现出的变化，或者是扁骨在变成成年人骨骼形状之前在放射影像上呈现出来的变化……别担心，我们在此之前也没听过这些。我们将这一过程展示在下图中：
![](https://pic4.zhimg.com/v2-d7fea8d722bef74b3ed21de72ce3fc2b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='691' height='369'></svg>)



像 GP 或 TW2 这样的传统评估骨骼成熟度的技术耗时较久，从一定程度上说也太过主观，因此一些高级的放射科医生也不总是认可这些结果。因此，使用计算机辅助诊断系统提升骨龄评估的准确率、提高结果的可复现性以及临床医生的效率就变得很诱人了。

近期，一些基于深度学习的方法已经证明在解决生物医学领域的多个问题时性能优于传统的机器学习方法。而在医学成像领域，卷积神经网络已经成功应用于糖尿病视网膜病变筛查、乳腺癌组织学成像分析、骨骼疾病预测以及一些其他问题中。如果你想深入了解这些应用，请参阅我们之前的文章（[https://medium.com/neuromation-io-blog/artificial-intelligence-technologies-could-help-solve-the-global-healthcare-crisis-441a51ac45b8](https://link.zhihu.com/?target=https%3A//medium.com/neuromation-io-blog/artificial-intelligence-technologies-could-help-solve-the-global-healthcare-crisis-441a51ac45b8)）。

因此我们试着将现代深度神经网络架构应用于骨龄评估。下文将描述用全自动深度学习方法解决骨龄评估问题的完整过程，我们所用的数据来自儿科骨龄估计挑战赛（[http://rsnachallenges.cloudapp.net/competitions/4](https://link.zhihu.com/?target=http%3A//rsnachallenges.cloudapp.net/competitions/4)），该比赛由北美放射学会（RSNA）举办。尽管我们的主要目标是要获得尽可能高的准确率，但还是要保持系统的鲁棒性，克服不同医疗中心的不同硬件所生成 X 光片的质量问题和多样性问题。




**数据**

数据集来自于举办了儿科骨龄挑战赛 2017 的 RSNA。他们从斯坦福儿童医院和科罗拉多儿童医院获取了放射图像，这些图像是在不同时间不同条件下用不同设备拍出来的。专业的儿科放射医师对这些图像进行注释，他们将这些图像与 Greulich 和 Pyle 所著《手部骨骼发育放射图解》进行比较，并在报告中记录骨龄。大赛组委会从这些报告中提取了骨龄信息，并将其作为模型训练的真值。

放射图像在比例、方向、曝光等方面各不相同，而且通常会带有特定的标记。完整的 RSNA 数据集包含 12,611 个训练图像、1,425 个验证图像以及 200 个测试图像。显而易见测试集特别小，且在开发阶段我们并不知道其标签，因此我们从训练集中取出 1000 张放射图像，在这些图像上对模型进行测试。

训练数据包含 5778 名女性和 6833 名男性的放射图像。他们的年龄从 1 个月到 228 个月不等，受试者多为 5 到 15 岁的儿童：
![](https://pic1.zhimg.com/v2-3ff46047d1dba156fce976d8bb588d80_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='519'></svg>)



**预处理 I：分割和对比**

我们工作的关键贡献之一是进行了严格的预处理。为了防止模型因图像伪影学习到错误信息，我们首先通过图像分割来移除图像背景。

我们用 U-Net 深度架构（[https://arxiv.org/abs/1505.04597](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1505.04597)）进行图像分割。自 2015 年 U-Net 出现以来，它就成为分割任务中的主力。它由一个获取上下文信息的收缩路径（contracting path）和一个用于精准定位的对称扩张路径（symmetric expanding path）组成；因为本文主题不是 U-Net，我们简单地展示一下其架构即可，细节请参阅论文。
![](https://pic4.zhimg.com/v2-f0d54b1661a3419db18f4e09e846bff3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='713'></svg>)
我们还在训练过程中使用了批归一化使函数收敛。我们的算法使用的是广义损失函数（generalized loss function）
![](https://pic4.zhimg.com/v2-f64cf0d5b16706e464ce90fc04d5b697_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='230' height='48'></svg>)
式中 H 是标准的二进制交叉熵损失函数
![](https://pic1.zhimg.com/v2-4b603773888975e8d516f25fed3de2d8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='544' height='85'></svg>)
，式中 y_i 是像素 i 的真值，
![](https://pic1.zhimg.com/v2-03faa63509972d6feffe2ded4ffcd0a8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='70' height='57'></svg>)
是像素的预测概率，J 是 Jaccard 系数的可微泛化：
![](https://pic3.zhimg.com/v2-30f71e4a806fc61eea24cbdf5ca948fa_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='244' height='90'></svg>)
。

我们通过移除小的无关的连接组件和均衡对比度完成了分割步骤。下图是我们进行预处理的流程：
![](https://pic1.zhimg.com/v2-f053f67076d721c753c10c111a0b5034_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='416'></svg>)
如你所见，放射图像的质量和对比度有了显著的提升。你可以到这里停止预处理，用我们的预处理方法和标准技术（如缩放和旋转）来增强训练集，然后训练一个标准的卷积神经网络用于分类或回归。我们试了这种方法，尽管结果不如我们的最终模型，但是也很令人满意。

之前的 GP 和 TW 方法关注的是手骨的特定部位（包括指骨、掌骨和腕骨）。我们决定试着用这些信息，针对几个解析度较高的特定区域分别训练模型，再评估和比较它们的性能。为了正确定位这些区域，我们要将所有图的大小和位置转换成一样的，也就是说将它们放在相同的坐标空间中，该过程通常被称为图像配准（image registration）。




**预处理 II：基于特征点进行图像配准**

我们的图像配准策略很简单：检测手骨中几个特征点的坐标，然后计算仿射变换参数（缩放、旋转、平移和反射），使图像与标准位置一致。

为了创建特征点模型的训练集，我们用 VGG 图像注释器（VIA）手动标注了 800 张放射影像。我们选择了三个特征点：第三根手指远端指骨的顶端，拇指远端指骨的顶端以及头状骨的中心。我们将特征点的像素坐标作为回归模型的训练目标。

再说一次，特征点模型是受一系列 VGG 族模型启发、通过深度卷积神经网络实现的回归模型。VGG 模块由两个带有指数线性单元激活、批归一化和最大池化的卷积层组成。下图是其架构：
![](https://pic3.zhimg.com/v2-6b733c9196e2876d4263f3a51ab544da_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='786' height='340'></svg>)
该模型使用均方误差（MSE）损失和 Adam 优化器训练得到：
![](https://pic3.zhimg.com/v2-3af354978dec33e31b44fcf61aad2756_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='288' height='90'></svg>)
为了提升泛化性能，我们对输入应用了包括旋转、平移和缩放在内的标准增强。模型输出 6 个坐标，即 3 个特征点中每个特征点有 2 个坐标。

找到特征点后，我们就可以计算所有放射影像的仿射变换（缩放、旋转、平移）。我们的目标是在保持图像纵横比的情况下，将其调整到统一的位置：

1. 中指顶端水平对齐，在图像上边缘以下约 100 个像素处。

2. 头状骨水平对齐，在图像下边缘以上约 480 个像素处。

通常用左手的放射影像评估骨龄，但有时候数据集中的图像会呈镜像。为了检测这些图像并对其进行适当调整，我们使用拇指作为特征点。

来看一个图像配准模型起作用的例子。如你所见，手骨成功地旋转到我们希望的标准位置了：
![](https://pic1.zhimg.com/v2-414d488b1345844d7414c2eefc81e2d8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='661'></svg>)
此处展示了更多经过完整预处理的样本。分割、归一化以及配准结果见第四行：
![](https://pic2.zhimg.com/v2-6c7d8e9e6346e0a489028fc400658eed_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1078' height='660'></svg>)



**骨龄评估模型**

遵循 Gilsanz 和 Ratib 的《Hand Bone Age: a Digital Atlas of Skeletal Maturity》，我们从经过配准的放射影像中选择了三个特定区域，针对每一个区域训练了一个单独的模型：

1. 完整的手骨；

2. 腕骨；

3. 掌骨和近端指骨。




下图是真实放射影像中的上述区域以及相对应的分割样本：
![](https://pic2.zhimg.com/v2-03fa7c6d37894a4cf3ab2441523f2741_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='1040'></svg>)![](https://pic3.zhimg.com/v2-749bf69d439810dafbd8301a9cd55b56_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='409'></svg>)
卷积神经网络常用于分类任务，但是骨龄评估本质上是一个回归问题：我们必须要预测年龄，而年龄是一个连续的变量。因此，我们需要比较两种 CNN 架构设置——回归和分类。我们分别实现了二者，它们的参数和训练方法类似，区别只在于最后两层。

我们的第一个模型是自定义的 VGG 架构，其输出结果是回归结果。网络堆叠了 6 个卷积核大小分别为 32、64、128、256、384 的卷积模块，接着是两个有 2048 个神经元的全连接层和一个输出层（见下图）。输入的大小取决于需要预测的图像区域。为了更好地进行泛化，我们在全连接层之前应用了 dropout 层。我们还重新调整了回归目标也就是骨龄的范围，将其定在 [-1,1]。为了避免过拟合，我们还使用缩放、旋转和位移等方式增强训练数据。该网络是用 Adam 优化器通过最小化平均绝对误差（MAE）训练得到的：
![](https://pic2.zhimg.com/v2-d4672ab8cd65b69af190f8e2593d4445_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='281' height='90'></svg>)
第二个模型是分类模型，该模型和回归模型很像，只是最后两层不同。一个主要的区别是分配给每个骨龄的类别不同。在数据集中，骨龄是以月为单位的，所以我们可以将其视为 240 类，将倒数第二层变成有 240 个输出的 softmax 层。这一层输出的是概率的向量，这里的类别概率真值范围在 [0,1] 区间内。最后一层中，概率向量与不同骨龄的向量 [1,…,239,240] 相乘。因此，该模型会输出骨龄的期望值。我们用和回归模型同样的训练方法训练了这个模型。

下图是分类模型的架构，除了多了 softmax 层和分箱层（binning layer）外，其他和回归模型相同：
![](https://pic4.zhimg.com/v2-98316bd5d18149fc7c7ca693c1220eaf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='827' height='194'></svg>)



**结果**

我们从训练集中取出 1000 张放射图像作为验证集来评估模型。遵循 GP 和 TW 方法将性别考虑在内，对每个空间区域我们都针对性别训练了性别特定模型，并与在整个数据集上训练得到的不区分性别的模型做了比较。以下是结果总结：
![](https://pic4.zhimg.com/v2-c4d5c207174696974a2f25a2b89090bf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='851' height='584'></svg>)
结果证明在输入中添加性别可以将模型准确率平均提高 1.4 个月。最左边这一列表示针对两个性别的回归模型的表现。掌骨和近端指骨区域（区域 C）的 MAE 为 8.42 个月，整个手骨（区域 A）的 MAE 为 8.08 个月。对这三个区域进行线性集成可以将准确率提高到 7.52 个月（表中最后一行）。

性别特定的回归模型（第二列和第三列）将针对男性的模型的 MAE 改善为 6.30 个月，将针对女性的模型的 MAE 改善到 6.49。需要注意的是，就女性而言，掌骨和近端指骨区域的 MAE 为 6.79 个月，甚至比对整个手骨进行预测的结果还准确，后者的 MAE 仅为 7.12 个月。

性别特定的分类模型（第四列和第五列）的结果比回归模型的结果略好，它们的 MAE 分别为 6.16 个月和 6.39 个月（最后一行）。

最后，在第六列，我们将所有性别特定的模型（分类和回归）进行集成。在验证集上，集成模型得到的准确率为 6.10 个月，这也是当前最佳的结果，无论从骨龄评估挑战赛还是从实际应用来看，这都是一个很好的结果。




**结论**

本文介绍了如何开发可以准确评估骨骼成熟度的自动骨龄评估系统，该系统可以得到与放射科专家相似或更好的结果。我们评估了手骨不同区域，发现可以仅对掌骨和近端指骨进行评估，得到的结果与对整个手骨进行评估的结果相差无几。为了克服放射图像的质量和多样性问题，我们引入了严格的清理和标准化过程，以显著增强模型的鲁棒性和准确率。

我们的模型有极大的潜力部署在临床环境中，帮助临床医生实时判断骨龄。这可以保证在患病儿童出现成长障碍时能进行及时的诊断和治疗。
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*
原文地址：[https://medium.com/neuromation-io-blog/neuromation-research-pediatric-bone-age-assessment-with-convolutional-neural-networks-ad539ffb671f](https://link.zhihu.com/?target=https%3A//medium.com/neuromation-io-blog/neuromation-research-pediatric-bone-age-assessment-with-convolutional-neural-networks-ad539ffb671f)


