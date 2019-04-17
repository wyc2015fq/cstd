# 百度提出NCRF：深度学习癌症图像检测效率再提升 - 知乎
# 



选自Baidu Research，作者：Yi Li、Wei Ping，机器之心编译。

> 深度学习在医疗图像疾病检测方向上的发展很快，但目前仍面临着易出现假阳性的问题。近日，百度研究人员提出了 NCRF（神经条件随机场）方法，在提升肿瘤图像准确率的同时也减少了假阳性的出现几率。该研究的论文已被即将在 7 月举行的深度学习医疗图像大会 MIDL 2018 接收。

NCRF 项目已开源：[https://github.com/baidu-research/NCRF](https://link.zhihu.com/?target=https%3A//github.com/baidu-research/NCRF)

目前在医疗领域有很多癌症诊断方法，其中病理学活体检测被认为是最为可信的标准。然而，对病理学切片进行分析并不是一件容易的事，即使对于经验丰富的病理学家而言也是有挑战的事情。一个放大 40 倍的病理切片数字图像通常包含数十亿像素，而在这样大规模的内容里，病理学家有时需要找寻微转移、肿瘤细胞细小群体等早期癌症征兆。这些任务让审查病理切片，而不遗漏任何临床证据成为了一项非常复杂耗时的工作。
![](https://pic3.zhimg.com/v2-111f096bd4017f4697070987f7f364ce_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='485' height='273'></svg>)
随着人工智能的发展，人们已经提出了各种基于深度学习的算法来帮助病理学家有效审查这些切片，并检测癌症转移。由于切片的原始数字图像非常大，大多数算法目前会将图片切割成大量小图片 (patch) 进行处理，如 256×256 像素尺寸的图片——然后训练并使用深度卷积神经网络来对肿瘤细胞和正常细胞进行分类。然而，这种方法有时难以在不知晓周围内容的情况下预测小图片中是否存在肿瘤，特别是在肿瘤/正常区域的边界上，经常会出现假阳性。




图 2 展示了这类方法的困难：
![](https://pic4.zhimg.com/v2-27113fae663ecb80a1ce49264bd07617_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='343' height='343'></svg>)
百度研究人员近日提出了一个新的深度学习算法，该方法不仅分析单个小图片，也将图片四周临近的网格一并输入进行肿瘤细胞分析。就像放大图片，看到更大区域从而做出更有置信度的判断一样。在这里，相邻切片之间的空间相关性通过特定类型的概率图形模型（条件随机场）进行建模。整个深度学习框架可以在 GPU 上进行端到端的训练，无需任何后处理过程。




图 3 展示了该算法的架构：
![](https://pic2.zhimg.com/v2-372f8888584159684a0468bc3f958a0d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='504' height='204'></svg>)
百度研究人员称，通过考虑相邻图片之间的相关性，新算法发生的误报次数少了很多。图 4 展示了在示例肿瘤图片上，新算法与不考虑临近图片的算法之间的预测效果比较。我们可以看到，除了基本的真肿瘤区域以外，新算法的假阳性区域相较基准算法要少很多。
![](https://pic1.zhimg.com/v2-f0b797ef7213a38206d1dd643dbd1584_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='687' height='790'></svg>)
图 4.（a) 原始的完整切片图像；（b) 病理学家注释的图像，白色区域表示癌症转移；（c) 通过之前的算法预测癌症区域，不考虑临近图像块；（d) 通过百度的算法预测癌症区域。




在 Camelyon16 挑战赛测试集上，百度的算法在癌症定位上的得分（FROC）为 0.8096，超越了专业的病理学家（0.7240）和前一个 Camelyon16 挑战赛冠军（0.8074）。此外，百度还在 Github 上开源了此算法，希望能够促进病理分析领域的 AI 研究。

这种全新的癌症检测算法有潜力改进病理切片镜检的效率与准确率。这能使得病理学家更加关注算法重点强调的癌症区域，而不是检查整个切片。不过，要综合评估该算法，还需要在更大数据集上做进一步的临床研究。

更多信息可查看以下论文：

注：在数字病理切片中，在 40 倍的放大下一个像素大概长 0.243 微米。微转移一般定义为一组癌细胞最大直径超过 200 微米，也就是大概 823 像素。

**论文：Cancer Metastasis Detection With Neural Conditional Random Field**
![](https://pic3.zhimg.com/v2-44cfe4a06fa46ea96b0c5488c8881d0e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='586' height='328'></svg>)
论文链接：[https://openreview.net/forum?id=S1aY66iiM](https://link.zhihu.com/?target=https%3A//openreview.net/forum%3Fid%3DS1aY66iiM)

**摘要：**乳腺癌诊断通常要求通过全切片数字化图像（WSI）对淋巴结癌细胞转移进行准确检测。近期深度卷积神经网络的发展使得医疗影像分析取得了极大成功，尤其是在计算病理组织学方面。由于 WSI 非常大，大部分方法都将整张图像切分成很多小的图像块，再对每个图像块分别进行分类。但是，相邻图像块通常具备空间关联，忽视此类关联可能会导致预测结果不一致。本论文提出一种神经条件随机场（neural conditional random field，NCRF）深度学习框架，来检测 WSI 中的癌细胞转移。NCRF 通过一个直接位于 CNN 特征提取器上方的全连接 CRF，来考虑相邻图像块之间的空间关联。整个深度网络可以使用标准反向传播算法，以最小算力进行端到端的训练。CNN 特征提取器也可以从利用 CRF 考虑空间关联中受益。与不考虑空间关联的基线方法相比，NCRF 框架可获取更高视觉质量的图像块预测概率图。我们还展示了在 Camelyon16 数据集上该方法在癌细胞转移检测方面优于基线方法，在测试集上取得了 0.8096 的平均 FROC 分数。
![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)
原文链接：[http://research.baidu.com/Blog/index-view?id=104](https://link.zhihu.com/?target=http%3A//research.baidu.com/Blog/index-view%3Fid%3D104)




