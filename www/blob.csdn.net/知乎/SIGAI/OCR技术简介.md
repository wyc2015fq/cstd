# OCR技术简介 - 知乎
# 

本文及其它机器学习、深度学习算法的全面系统讲解可以阅读《机器学习与应用》，清华大学出版社，雷明著，由SIGAI公众号作者倾力打造，自2019年1月出版以来已重印3次。
- [书的购买链接](https://link.zhihu.com/?target=https%3A//item.jd.com/12504554.html)
- [书的勘误，优化，源代码资源](https://link.zhihu.com/?target=http%3A//www.tensorinfinity.com/paper_78.html)

**OCR技术简介**

光学字符识别（Optical Character Recognition, OCR）是指对文本资料的图像文件进行分析识别处理，获取文字及版面信息的过程。亦即将图像中的文字进行识别，并以文本的形式返回。

**OCR的应用场景**

根据识别场景，可大致将OCR分为识别特定场景的专用OCR和识别多种场景的通用OCR。比如现今方兴未艾的证件识别和车牌识别就是专用OCR的典型实例。通用OCR可以用于更复杂的场景，也具有更大的应用潜力。但由于通用图片的场景不固定，文字布局多样，因此难度更高。根据所识别图片的内容，可将场景分为清晰且具有固定模式的简单场景和更为复杂的自然场景。自然场景文本识别的难度极高，原因包括：图片背景极为丰富，经常面临低亮度、低对比度、光照不均、透视变形和残缺遮挡等问题，而且文本的布局可能存在扭曲、褶皱、换向等问题，其中的文字也可能字体多样、字号字重颜色不一的问题。因此自然场景中的文字识别技术，也经常被单列为场景文字识别技术(Scene Text Recognition, STR)，相关内容可回顾往期SigAI的文章。

**OCR的技术路线**

典型的OCR的技术路线如下图所示
![](https://pic2.zhimg.com/v2-78792fa4ad28e64930fb37bad2877845_b.jpg)
其中影响识别准确率的技术瓶颈是文字检测和文本识别，而这两部分也是OCR技术的重中之重。

在传统OCR技术中，图像预处理通常是针对图像的成像问题进行修正。常见的预处理过程包括：几何变换（透视、扭曲、旋转等）、畸变校正、去除模糊、图像增强和光线校正等	

文字检测即检测文本的所在位置和范围及其布局。通常也包括版面分析和文字行检测等。文字检测主要解决的问题是哪里有文字，文字的范围有多大。

文本识别是在文本检测的基础上，对文本内容进行识别，将图像中的文本信息转化为文本信息。文字识别主要解决的问题是每个文字是什么。识别出的文本通常需要再次核对以保证其正确性。文本校正也被认为属于这一环节。而其中当识别的内容是由词库中的词汇组成时，我们称作有词典识别(Lexicon-based)，反之称作无词典识别(Lexicon-free)

**图像预处理**

传统OCR基于数字图像处理和传统机器学习等方法对图像进行处理和特征提取。常用的二值化处理有利于增强简单场景的文本信息，但对于复杂背景二值化的收效甚微。

传统方法上采用HoG对图像进行特征提取，然而HoG对于图像模糊、扭曲等问题鲁棒性很差，对于复杂场景泛化能力不佳。由于深度学习的飞速发展，现在普遍使用基于CNN的神经网络作为特征提取手段。得益于CNN强大的学习能力，配合大量的数据可以增强特征提取的鲁棒性，面临模糊、扭曲、畸变、复杂背景和光线不清等图像问题均可以表现良好的鲁棒性。[1]
![](https://pic4.zhimg.com/v2-37b9d463bb02dade7f2fdd2aa8e08a17_b.jpg)基于CNN(LeNet-5)的文字识别[1]
**文字检测**

对于文字检测任务，很自然地可以想到套用图像检测的方法来框选出图像中的文本区域。常见的一些物体检测方法如下：

**Faster R-CNN** Faster R-CNN采用辅助生成样本的RPN（Region Proposal Networks）网络，将算法结构分为两个部分，先由RPN 网络判断候选框是否为目标，再经分类定位的多任务损失判断目标类型，整个网络流程都能共享卷积神经网络提取的的特征信息，节约计算成本，且解决Fast R-CNN 算法生成正负样本候选框速度慢的问题，同时避免候选框提取过多导致算法准确率下降。对于受限场景的文字检测，Faster R-CNN的表现较为出色。可以通过多次检测确定不同粒度的文本区域。[2]
![](https://pic1.zhimg.com/v2-351f91ee7b463b7908a553f8e45da940_b.jpg)Faster R-CNN的网络结构[2]
**FCN** 相较于Faster R-CNN 算法只能计算ROI pooling 层之前的卷积网络特征参数，R-FCN 算法提出一种位置敏感分布的卷积网络代替ROI pooling 层之后的全连接网络，解决了Faster R-CNN 由于ROI Pooling 层后面的结构需要对每一个样本区域跑一次而耗时比较大的问题，使得特征共享在整个网络内得以实现，解决物体分类要求有平移不变性和物体检测要求有平移变化的矛盾，但是没有考虑到候选区域的全局信息和语义信息。[3]所以当面对自然场景的通用OCR，适于多尺度检测的FCN较之Faster R-CNN有着更好的表现。当采用FCN时，输出的掩膜可以作为前景文字的二值图像进行输出。
![](https://pic3.zhimg.com/v2-327d7d12135a627188da8cef3b39aeb6_b.jpg)FCN的网络结构[3]
但是与其他日常场景的物体检测所不同的是，文字图像的分布更接近于均匀分布而非正态分布，即文字总体的均值图像并不能体现文字这一抽象概念的特征。除此之外，文字的长宽比与物体的长宽比不同，导致候选锚定框不适用；文字的方向仍然不能确定，对非垂直的文字方向表现佳；自然场景中常出现一些结构与文字非常接近，导致假阳性率升高。因此需要对现有模型进行调整。

一种常见的做法是调整候选锚定框，例如

**RRPN** (Rotation Region Proposal Networks)在faster R-CNN的基础上，将垂直的候选锚定框进行旋转满足非垂直文本的检测，这样一来就可以满足非垂直文字的检测需求。[4]
![](https://pic3.zhimg.com/v2-d162cd4687dcb57841dbd002dd5b041a_b.jpg)RRPN的网络结构[4]
**TextBoxes**是基于SSD改进的一个算法。调整了锚定框的长宽比，以适应文字的高长宽比。输出层也利用了利用非标准的卷积核。更适应文字细长的宽高比这一特点。[5]而
**![](https://pic4.zhimg.com/v2-8da48c9a415cea6a60131adec5c68eeb_b.jpg)TextBoxes的网络结构[5]**
**DMPNet** (Deep Matching Prior Network)采用非矩形四边形的候选锚定框进行检测。通过Monte-Carlo方法计算标注区域与矩形候选框和旋转候选框的重合度后重新计算顶点坐标，得到非矩形四边形的顶点坐标。[6]
![](https://pic4.zhimg.com/v2-2d2a1f439f520f97a9e7611e49c2edd3_b.jpg)DMPNet生成候选锚定框的示意图[6]
另一种改进的方法是通过自底向顶的方法，检测细粒度文本后将其连接成更粗粒度的文本

**CTPN**(Connectionist Text Proposal Network)是目前应用最广的文本检测模型之一。其基本假设是单个字符相较于异质化程度更高的文本行更容易被检测，因此先对单个字符进行类似R-CNN的检测。之后又在检测网络中加入了双向LSTM，使检测结果形成序列提供了文本的上下文特征，便可以将多个字符进行合并得到文本行。[7]
![](https://pic3.zhimg.com/v2-ab0e93d6834227bf26098d49eb44d3aa_b.jpg)CTPN的网络结构[7]
**SegLink**则是在SSD的启发下得出的。采用临近连接的方法对上下文进行连接。并且通过将连接参数的学习整合进了神经网络的学习过程，使得模型更容易训练。[8]
![](https://pic2.zhimg.com/v2-c58b797c532a4f57870bf3fddb6b79d9_b.jpg) SegLink的网络结构[8]
有一些研究引入了注意力机制，如下图模型采用Dense Attention模型来对图像的权重进行评估。这样有利于将前景图像和背景图像分离，对于文本内容较之背景图像有着更高的注意力，使检测结果更准确。[9]

**文本识别**

文本识别在传统技术中采用模板匹配的方式进行分类。但是对于文字行，只能通过识别出每一个字符来确定最终文字行从内容。因此可以对文字行进行字符切分，以得到单个文字。这种方式中，过分割-动态规划是最常见的切分方法。由于单个字符可能会由于切分位置的原因产生多个识别结果，例如“如”字在切分不当时会被切分成“女_口”，因此需要对候选字符进行过分割，使其足够破碎，之后通过动态规划合并分割碎片，得到最优组合，这一过程需要人工设计损失函数。还有另一种方法是通过滑动窗口对每一个可能的字符进行匹配，这种方法的准确率依赖于滑动窗口的滑动窗尺寸，如果滑动窗尺寸过大会造成信息丢失，而太小则会使计算力需求大幅增加。

以上的传统方法通过识别每个单字符以实现全文的识别，这一过程导致了上下文信息的丢失，对于单个字符有较高的识别正确率，其条目识别正确率也难以保证。以身份证识别为例，识别18位的身份号的场景下，即使单字符识别正确率高达99%，其条目正确率只能到0.9918=83%，如果切分也存在1%的损失（即正确率99%），条目正确率则只有(0.99*0.99)18=70%。

因此引入上下文的信息，成为了提升条目准确率的关键。从深度学习的角度出发，要引入上下文这样的序列信息，RNN和LSTM等依赖于时序关系的神经网络是最理想的选择。
![](https://pic2.zhimg.com/v2-5074903b9f23f45a749782d1e77a8631_b.jpg)
CNN+Softmax  CNN+RNN+CTC  CNN+RNN+Attention机制

几种典型的文本识别网络结构

常见的一种做法是利用CRNN模型。以CNN特征作为输入，双向LSTM进行序列处理使得文字识别的效率大幅提升，也提升了模型的泛化能力。先由分类方法得到特征图，之后通过CTC对结果进行翻译得到输出结果。[10]
![](https://pic2.zhimg.com/v2-e6ad327b076fb9a7e5c432d041e99071_b.jpg)CRNN的网络结构[10]
另一种方法是引入注意力机制。以CNN特征作为输入，通过注意力模型对RNN的状态和上一状态的注意力权重计算出新一状态的注意力权重。之后将CNN特征和权重输入RNN，通过编码和解码得到结果。[11]
![](https://pic4.zhimg.com/v2-63c369028fb3a804e01710ffb7c2fe97_b.jpg)Attention OCR的网络结构[11]
**端到端的OCR**

与检测-识别的多阶段OCR不同，深度学习使端到端的OCR成为可能，将文本的检测和识别统一到同一个工作流中。目前比较受到瞩目的一种端到端框架叫做FOTS(Fast Oriented Text Spotting)。FOTS的检测任务和识别任务共享卷积特征图。一方面利用卷积特征进行检测，另一方面引入了RoIRotate，一种用于提取定向文本区域的算符。得到文本候选特征后，将其输入到RNN编码器和CTC解码器中进行识别。同时，由于所有算符都是可微的，因此端到端的网络训练成为可能。由于简化了工作流，网络可以在极低运算开销下进行验证，达到实时速度。[12]
![](https://pic4.zhimg.com/v2-1a70fc6302106741e0884c3a8ed5db37_b.jpg)FOTS的总体结构[12]


**总结**

尽管基于深度学习的OCR表现相较于传统方法更为出色，但是深度学习技术仍需要在OCR领域进行特化，而其中的关键正式传统OCR方法的精髓。因此我们仍需要从传统方法中汲取经验，使其与深度学习有机结合进一步提升OCR的性能表现。另一方面，作为深度学习的推动力，数据起到了至关重要的作用，因此收集广泛而优质的数据也是现阶段OCR性能的重要举措之一。	

**参考文献**

 [1]	Lécun Y, Bottou L, Bengio Y, et al. Gradient-based learning applied to document recognition[J]. Proceedings of the IEEE, 1998,86(11):2278-2324.

 [2]	Ren S, Girshick R, Girshick R, et al. Faster R-CNN: Towards Real-Time Object Detection with Region Proposal Networks[J]. IEEE Trans Pattern Anal Mach Intell, 2015,39(6):1137-1149.

 [3]	Lin T Y, Dollar P, Girshick R, et al. Feature Pyramid Networks for Object Detection[J]. 2016:936-944.

 [4]	Ma J, Shao W, Ye H, et al. Arbitrary-Oriented Scene Text Detection via Rotation Proposals[J]. IEEE Transactions on Multimedia, 2017,PP(99):1.

 [5]	Liao M, Shi B, Bai X, et al. TextBoxes: A Fast Text Detector with a Single Deep Neural Network[J]. 2016.

 [6]	Liu Y, Jin L. Deep Matching Prior Network: Toward Tighter Multi-oriented Text Detection[C]//: IEEE Conference on Computer Vision and Pattern Recognition, 2017.

 [7]	Tian Z, Huang W, He T, et al. Detecting Text in Natural Image with Connectionist Text Proposal Network[C]//: European Conference on Computer Vision, 2016.

 [8]	Shi B, Bai X, Belongie S. Detecting Oriented Text in Natural Images by Linking Segments[J]. 2017:3482-3490.

 [9]	Gao Y, Chen Y, Wang J, et al. Reading Scene Text with Attention Convolutional Sequence Modeling[J]. 2017.

[10]	Shi B, Bai X, Yao C. An End-to-End Trainable Neural Network for Image-based Sequence Recognition and Its Application to Scene Text Recognition[J]. IEEE Transactions on Pattern Analysis & Machine Intelligence, 2016,39(11):2298-2304.

[11]	Wojna Z, Gorban A N, Lee D S, et al. Attention-Based Extraction of Structured Information from Street View Imagery[J]. 2017:844-850.

[12]	Liu X, Liang D, Yan S, et al. FOTS: Fast Oriented Text Spotting with a Unified Network[J]. 2018.

推荐阅读

[1][机器学习-波澜壮阔40年](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483705%26idx%3D1%26sn%3Dc6e7c4a2e14a2469308b41eb60f155ac%26chksm%3Dfdb69caecac115b8712653600e526e99a3f6976fdaa2f6b6a09388fa6f9677ccb57b40c40ae3%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0413.

[2][学好机器学习需要哪些数学知识？](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483713%26idx%3D1%26sn%3D1e7c81381d16806ac73e15691fe17aec%26chksm%3Dfdb69cd6cac115c05f1f90b0407e3f8ae9be8719e454f908074ac0d079885b5c134e2d60fd64%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0417.

[3] [人脸识别算法演化史](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483726%26idx%3D1%26sn%3D9fef4cc1766ea4258749f8d40cc71a6e%26chksm%3Dfdb69cd9cac115cf4eba16081780c3b64c75e1e55a40bf2782783d5c28f00c6f143426e6f0aa%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0420.

[4][基于深度学习的目标检测算法综述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483731%26idx%3D1%26sn%3D237c52bc9ddfe65779b73ef8b5507f3c%26chksm%3Dfdb69cc4cac115d2ca505e0deb975960a792a0106a5314ffe3052f8e02a75c9fef458fd3aca2%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0424.

[5][卷积神经网络为什么能够称霸计算机视觉领域？](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483816%26idx%3D1%26sn%3Dfc52765b012771d4736c9be4109f910e%26chksm%3Dfdb69c3fcac115290020c3dd0d677d987086a031c1bde3429339bb3b5bbc0aa154e76325c225%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0426.

[6] [用一张图理解SVM的脉络](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483937%26idx%3D1%26sn%3D84a5acf12e96727b13fd7d456c414c12%26chksm%3Dfdb69fb6cac116a02dc68d948958ee731a4ae2b6c3d81196822b665224d9dab21d0f2fccb329%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0428.

[7] [人脸检测算法综述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483950%26idx%3D1%26sn%3Da3a5b7907b2552c233f654a529931776%26chksm%3Dfdb69fb9cac116af5dd237cf987e56d12b0d2e54c5c565aab752f3e366c0c45bfefa76f5ed16%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0503.

[8] [理解神经网络的激活函数](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483977%26idx%3D1%26sn%3D401b211bf72bc70f733d6ac90f7352cc%26chksm%3Dfdb69fdecac116c81aad9e5adae42142d67f50258106f501af07dc651d2c1473c52fad8678c3%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI2018.5.5.

[9] [深度卷积神经网络演化历史及结构改进脉络-40页长文全面解读](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484037%26idx%3D1%26sn%3D13ad0d521b6a3578ff031e14950b41f4%26chksm%3Dfdb69f12cac11604a42ccb37913c56001a11c65a8d1125c4a9aeba1aed570a751cb400d276b6%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0508.

[10] [理解梯度下降法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484111%26idx%3D1%26sn%3D4ed4480e849298a0aff828611e18f1a8%26chksm%3Dfdb69f58cac1164e844726bd429862eb7b38d22509eb4d1826eb851036460cb7ca5a8de7b9bb%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0511.

[11] [循环神经网络综述—语音识别与自然语言处理的利器](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484310%26idx%3D1%26sn%3D0fc55a2784a894100a1ae64d7dbfa23d%26chksm%3Dfdb69e01cac1171758cb021fc8779952e55de41032a66ee5417bd3e826bf703247e243654bd0%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0515

[12] [理解凸优化](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484439%26idx%3D1%26sn%3D4fa8c71ae9cb777d6e97ebd0dd8672e7%26chksm%3Dfdb69980cac110960e08c63061e0719a8dc7945606eeef460404dc2eb21b4f5bdb434fb56f92%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】 SIGAI0518

[13] [【实验】理解SVM的核函数和参数](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484495%26idx%3D1%26sn%3D4f3a6ce21cdd1a048e402ed05c9ead91%26chksm%3Dfdb699d8cac110ce53f4fc5e417e107f839059cb76d3cbf640c6f56620f90f8fb4e7f6ee02f9%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0522

[14][【SIGAI综述】行人检测算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484523%26idx%3D1%26sn%3Dddaa70c4b92f6005d9bbd6b3a3fe4571%26chksm%3Dfdb699fccac110ea14e6adeb873a00d6ee86dd4145ddf8e90c9b878b908ac7b7655cfa51dab6%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0525

[15] [机器学习在自动驾驶中的应用—以百度阿波罗平台为例(上)](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484540%26idx%3D1%26sn%3D733332042c31e1e18ad800f7f527893b%26chksm%3Dfdb699ebcac110fd6607c1c99bc7ebed1594a8d00bc454b63d7f518191bd72274f7e61ca5187%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0529

[16][理解牛顿法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484651%26idx%3D1%26sn%3Da0e4ca5edb868fe3eae9101b71dd7103%26chksm%3Dfdb6997ccac1106a61f51fe9f8fd532045cc5d13f6c75c2cbbf1a7c94c58bcdf5f2a6661facd%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0531

[17] [【群话题精华】5月集锦—机器学习和深度学习中一些值得思考的问题](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484658%26idx%3D1%26sn%3Df5c9f92c272c75883bf8e6f532559f11%26chksm%3Dfdb69965cac11073f49048caef5d7b9129614090a363d9ef7f3d1b9bc59948d2217d2bca7b7b%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI 0601

[18] [大话Adaboost算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484692%26idx%3D1%26sn%3D9b389aa65208c778dddf17c601afbee1%26chksm%3Dfdb69883cac1119593934734e94c3b71aa68de67bda8a946c1f9f9e1209c3b6f0bf18fed99b8%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0602

[19] [FlowNet到FlowNet2.0：基于卷积神经网络的光流预测算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484711%26idx%3D1%26sn%3Dbb7644e101b5924f54d6800b952dc3aa%26chksm%3Dfdb698b0cac111a6605f5b9b6f0478bf21a8527cfad2342dbaaf624b4e9dcc43c0d85ae06deb%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0604

[20] [理解主成分分析(PCA)](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484754%26idx%3D1%26sn%3Db2c0d6798f44e13956bb42373e51d18c%26chksm%3Dfdb698c5cac111d3e3dca24c50aafbfb61e5b05c5df5b603067bb7edec8db049370b73046b24%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0606

[21] [人体骨骼关键点检测综述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484784%26idx%3D1%26sn%3Dceafb54203f4e930ae457ad392b9f89c%26chksm%3Dfdb698e7cac111f13d8229d7dcc00b4a7305d66de3da1bd41e7ecc1d29bfa7be520d205c53e9%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0608

[22][理解决策树](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484827%26idx%3D1%26sn%3D043d7d0159baaddfbf92ed78ee5b1124%26chksm%3Dfdb6980ccac1111a9faeae7f517fee46a1dfab19612f76ccfe5417487b3f090ab8fc702d18b8%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0611

[23] [用一句话总结常用的机器学习算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484859%26idx%3D1%26sn%3D2c4db22fb538953a62a90983e3e1f99d%26chksm%3Dfdb6982ccac1113a82e92be325bb07a947d54090274654375f3b50e11e1abd809fb7358bde16%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0611

[24] [目标检测算法之YOLO](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484909%26idx%3D1%26sn%3Dc02ee17e5175230ed39ad63e73249f5c%26chksm%3Dfdb6987acac1116c0108ec28424baf4ea16ca11d2b13f20d4a825d7b2b82fb8765720ebd1063%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0615

[25] [理解过拟合](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484954%26idx%3D1%26sn%3Dc28b7f07c22466e91b1ef90e9dbe3ad1%26chksm%3Dfdb69b8dcac1129bc6e78fca1d550e2b18238ad1c240c73b280d4e529f9f93c4626b3ac45ea2%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0618

[26][理解计算：从√2到AlphaGo ——第1季 从√2谈起](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484981%26idx%3D1%26sn%3Dd3003468b9853851923844812993e060%26chksm%3Dfdb69ba2cac112b4dac620d52100ebd033eb679f29340726a67297c4d6980b16c7cc91122028%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0620

[27] [场景文本检测——CTPN算法介绍](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485005%26idx%3D1%26sn%3D0d4fb43b8db2a8046c64a9cfcbf3f478%26chksm%3Dfdb69bdacac112cce05c8b735b4f8b1ccf2348bea55a30af2055fc328958bb8f1ffd0f819bd2%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0622

[28] [卷积神经网络的压缩和加速](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485042%26idx%3D1%26sn%3Dcdcf8d4b07acf64c7a6f5f7c1a731a12%26chksm%3Dfdb69be5cac112f377766984afb87313c1e1c58d94c80005f0f6f6af61ee5a4bd1bf6c6157b6%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0625

[29] [k近邻算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485074%26idx%3D1%26sn%3D0ebf1bf8f49e9c46075fe3803d04c95d%26chksm%3Dfdb69b05cac112132d280c70af3923ca4c3cccfa5fcd8628b79d4b246b3b2decbc80a180abb3%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0627

[30][自然场景文本检测识别技术综述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485142%26idx%3D1%26sn%3Dc0e01da30eb5e750be453eabe4be2bf4%26chksm%3Dfdb69b41cac11257ae22c7dac395e9651dab628fc35dd6d3c02d9566a8c7f5f2b56353d58a64%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0627

[31] [理解计算：从√2到AlphaGo ——第2季 神经计算的历史背景](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485155%26idx%3D1%26sn%3D990cc7400751c36e9fef0a261e6add2a%26chksm%3Dfdb69b74cac112628bdae14c6435120f6fece20dae9bf7b1ffc8b8b25e5496a24160feca0a72%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0704

[32] [机器学习算法地图](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485306%26idx%3D1%26sn%3Dfc8cc8de313bdb61dcd39c1dedb240a4%26chksm%3Dfdb69aedcac113fb4b18c74248a313536ded50bade0e66b26f332ab247b148519da71ff2a3c0%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0706

[33] [反向传播算法推导-全连接神经网络](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485446%26idx%3D1%26sn%3D57d7d866443810c20c4ea2c6ee8018cc%26chksm%3Dfdb69591cac11c8773638b396abe43c0161e4d339f0fa845e54326be3e8c4933a3b6a2713dae%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0709

[34] [生成式对抗网络模型综述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485551%26idx%3D1%26sn%3D213f48c4e55bee688cf0731097bb832c%26chksm%3Dfdb695f8cac11ceef3ef246c54d811dd64d8cc45fc75488c374c7aa95f72c1abfb55555ef0b7%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0709.

[35][怎样成为一名优秀的算法工程师](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485570%26idx%3D1%26sn%3D5e71437a6d3ddf6d05144fc99e7633cc%26chksm%3Dfdb69515cac11c030cf713ec85293b7ad3bbe0f20d43fc2729cc976ff628aabf636834ccd904%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0711.

[36] [理解计算：从根号2到AlphaGo——第三季 神经网络的数学模型](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485592%26idx%3D1%26sn%3D1c5236972402ea8cb168161bc41e8e7e%26chksm%3Dfdb6950fcac11c19ad047e7cb9ced96447a85b41e21b10789a86ae4a211e4fb2ca1f911a7fc5%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0716

[37][【技术短文】人脸检测算法之S3FD](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485609%26idx%3D1%26sn%3Dd9068aecfbf150b40103210de538fea9%26chksm%3Dfdb6953ecac11c28361435306a7a09632ea79000abf1bf626e50afb3cda48eb3e47b96c6e7cd%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0716

[38] [基于深度负相关学习的人群计数方法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485617%26idx%3D1%26sn%3D7955bfefc8e4b28221ae5247812f8235%26chksm%3Dfdb69526cac11c30e1051edc4378d7dfdedf46925236dbe33e7912b4bea882e515f074eee4c9%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0718

[39] [流形学习概述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485668%26idx%3D1%26sn%3Df70547fc671d164e28fddcea6473524d%26chksm%3Dfdb69573cac11c65ee9fc98ac8fad093282a3d244748e7c88541c133ac55a32cb07472dc80e0%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0723

[40] [关于感受野的总结](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485700%26idx%3D1%26sn%3Dc0425495fe0ae9cb2120dbcb246f49b1%26chksm%3Dfdb69493cac11d8542f7a8e662a7ecdeece4fd2270c71504023e8b58128575d1e4fdadf60cf5%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0723

[41] [随机森林概述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485718%26idx%3D1%26sn%3Dc05c217af81173ae2c1301cbda5f7131%26chksm%3Dfdb69481cac11d975d86ff2e280371963d04b5709dfa0a9e874d637b7cf3844cad12be584094%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0725

[42] [基于内容的图像检索技术综述——传统经典方法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485741%26idx%3D1%26sn%3Df8217e523d54842daaa4be38fa1792eb%26chksm%3Dfdb694bacac11dacfd8d7e4882166e2774c4685c043379ce0d2044e99c3b3c8b0140480bbf8e%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0727

[43] [神经网络的激活函数总结](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485762%26idx%3D1%26sn%3De1e9fc75b92999177d3c61c655b0e06e%26chksm%3Dfdb694d5cac11dc37dac1a7ce32150836d66f0012f35a7e04e3dceaf626b8453dc39ee80172b%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0730

[44] [机器学习和深度学习中值得弄清楚的一些问题](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485811%26idx%3D1%26sn%3Da87082f0e47b01bb8c22443ba5b1139c%26chksm%3Dfdb694e4cac11df20ea1deb8b55cf297ad44e48a4c7ca45cfce387284143403fcd342ac98ec0%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0802

[45] [基于深度神经网络的自动问答系统概述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485842%26idx%3D1%26sn%3Dd7485054d6e93225b6ac0c77f8706cf7%26chksm%3Dfdb69405cac11d1355b84f692c2cbe49a3852a10e074b6941c95618598caea6ed64103c4ee4c%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0803

[46] [反向传播算法推导——卷积神经网络](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485971%26idx%3D1%26sn%3D632e3c135ead5f0ac730eee5b6a05647%26chksm%3Dfdb69784cac11e9258f67312fa2798e9c8210b8f77741a3d7bab4c1ccfa1c1f66632183f4f26%26token%3D1469111007%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0806

[47] [机器学习与深度学习核心知识点总结 写在校园招聘即将开始时](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486105%26idx%3D1%26sn%3Dd0b33e7e23b0e2fc442bd6b3e2a9d952%26chksm%3Dfdb6970ecac11e18085ea36f3b654028b2d4ba33a0cdc89c4ea25ac81570969f95f84c6939ac%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取 码】SIGAI0808

[48] [理解Spatial Transformer Networks](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486133%26idx%3D1%26sn%3D31c64e83511ad89929609dbbb0286890%26chksm%3Dfdb69722cac11e34da58fc2c907e277b1c3153a483ce44e9aaf2c3ed468386d315a9b606be40%26token%3D283993194%26lang%3Dzh_CN%23rd)【获取码】SIGAI0810

[49][AI时代大点兵-国内外知名AI公司2018年最新盘点](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486140%26idx%3D1%26sn%3D6157951b026199422becca8863f18a17%26chksm%3Dfdb6972bcac11e3d7427847df818bd6cc7893a261daa0babe0b11bd01a403dc4f36e2b45650e%26token%3D283993194%26lang%3Dzh_CN%23rd)【获取码】SIGAI0813

[50] [理解计算：从√2到AlphaGo ——第2季 神经计算的历史背景](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486202%26idx%3D1%26sn%3Dbf66be1e71272be42508e557ed93acbf%26chksm%3Dfdb6976dcac11e7b9d0f878233e8d9907825e57851e7a095f1be3a23abc9327370a77f4e2c03%26token%3D283993194%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0815

[51] [基于内容的图像检索技术综述--CNN方法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486268%26idx%3D1%26sn%3Ddff53feb3d78ea7698f70bede08b3b19%26chksm%3Dfdb696abcac11fbdcde5f4acc72d34c14119a21234b9e6cd0c1886b85c330e7f77d6e3da9122%26token%3D283993194%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0817

[52][文本表示简介](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486289%26idx%3D1%26sn%3Da312c951c943ad104c722e6c681823b6%26chksm%3Dfdb696c6cac11fd01224a68b9d67fcf0043ff2de0ec67f7bfd98bacf691c8eaf221cbca9b4d6%26token%3D1485183944%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0820

[53][机器学习中的最优化算法总结](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486354%26idx%3D2%26sn%3D1afd5f272536b6771b5e1e224e8414ec%26chksm%3Dfdb69605cac11f13f2a16c8748e333045e99497dc03ca9f4741723204e7d8348e969375379ae%26token%3D1580317399%26lang%3Dzh_CN%23rd)【获取码】SIGAI0822

[54][【AI就业面面观】如何选择适合自己的舞台？](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486371%26idx%3D1%26sn%3D955286243fe23ff58d5a090c166d83f6%26chksm%3Dfdb69634cac11f2251793b40432b5429828f5e72411e2ae48c1ae282117f2d1b4067c14fc889%26token%3D1383761413%26lang%3Dzh_CN%23rd)【获取码】SIGAI0823

[55][浓缩就是精华-SIGAI机器学习蓝宝书](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486392%26idx%3D1%26sn%3D81cc65e42418bf846167ab80ae1dd4f4%26chksm%3Dfdb6962fcac11f39d3c62bc06e8a0708a24a024e53aef38e1bd716dcd1433fa89a0c0fe422df%26token%3D867271861%26lang%3Dzh_CN%23rd)【获取码】SIGAI0824

[56][DenseNet详解](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486433%26idx%3D2%26sn%3D9858164d290b60c03081ee0b04d57a76%26chksm%3Dfdb69676cac11f60f48b9bce3017e2776767f3f9c2cc980e03c07dabc076a9ce8da4b906760d%26token%3D1199765642%26lang%3Dzh_CN%23rd)【获取码】SIGAI0827

[57][AI时代大点兵国内外知名AI公司2018年最新盘点【完整版】](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486478%26idx%3D2%26sn%3Dc322cadb649d85f4a6a45b104fd3955b%26chksm%3Dfdb69199cac1188f1524564763b78e1b5651aec56c2a4ef5a15b1eab5fad47aa9b99024aa75c%26token%3D1389180520%26lang%3Dzh_CN%23rd)【获取码】SIGAI0829

[58][理解Adaboost算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/cgi-bin/appmsg%3Ft%3Dmedia/appmsg_edit%26action%3Dedit%26type%3D10%26appmsgid%3D100002827%26isMul%3D1%26token%3D558765803%26lang%3Dzh_CN)【获取码】SIGAI0831

[59][深入浅出聚类算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486514%26idx%3D1%26sn%3D0eecb0fbb2fcfcb70aa0054a39ae7552%26chksm%3Dfdb691a5cac118b30e7e7df814fe44b4aa892df7817867391f64c981b4c646ccb2fdf9e8bbc5%26token%3D1980490660%26lang%3Dzh_CN%23rd)【获取码】SIGAI0903

[60][机器学习发展历史回顾](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486528%26idx%3D1%26sn%3D6bfd99475bc9d4850a113464ad25b129%26chksm%3Dfdb691d7cac118c1d020418dae8d024833a6360b6733cf1bec8582221b188af7c42750dff72a%26token%3D1980490660%26lang%3Dzh_CN%23rd)【获取码】SIGAI0905

[61] [网络表征学习综述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486560%26idx%3D1%26sn%3Dce723401f12859a497f78cc716fecc5c%26chksm%3Dfdb691f7cac118e10dfc6abc8fcc3c50ecfe80503bc1ecc4f0a62d3579acfc2b05440b9b5827%26token%3D1508419138%26lang%3Dzh_CN%23rd)【获取码】SIGAI0907

[62] [视觉多目标跟踪算法综述(上)](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486576%26idx%3D1%26sn%3D5f8e0e08ffcca70d42e370f548eefca2%26chksm%3Dfdb691e7cac118f1d6f0ec25582f0f845b48af27795a52d34352e02a58e74b822919a9026fbb%26token%3D1508419138%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0910

[63] [计算机视觉技术self-attention最新进展](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/cgi-bin/appmsg%3Ft%3Dmedia/appmsg_edit%26action%3Dedit%26type%3D10%26appmsgid%3D100002942%26isMul%3D1%26token%3D2089597586%26lang%3Dzh_CN) 【获取码】SIGAI0912 

[64] [理解Logistic回归](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486643%26idx%3D1%26sn%3Dbd1523c0c26d16a0fe59e0e5f89a09f4%26chksm%3Dfdb69124cac118324c852a5c2055e101d37b555b740a182e66ff3b005c9573027c503787fd92%26token%3D577589374%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0914

[65] [机器学习中的目标函数总结](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486696%26idx%3D1%26sn%3D69125413f53373598f86f5f437989e9d%26chksm%3Dfdb6917fcac11869c8963e9f70dd755743058363c1c49896e8a8f44f9eadacdc6d25b1186e3b%26token%3D577589374%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0917

[66] [人脸识别中的活体检测算法综述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486721%26idx%3D1%26sn%3Df0e5b2b0165e391c0d5adc4ce253f2f6%26chksm%3Dfdb69096cac119807aa9d190c62e18815937bc9751c0fbb09a183f8c936f44e570299e3cac9b%26token%3D936136024%26lang%3Dzh_CN%23rd)【获取码】SIGAI0919

[67] [机器学习与深度学习常见面试题（上）](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486727%26idx%3D1%26sn%3D7ac1be7d04fb2a8f5f56ad2f5a83a7b4%26chksm%3Dfdb69090cac119867ba99a5725d97e4a822ae578219dd7f404c81a114aa3467c43cb5c9812fe%26token%3D1122832922%26lang%3Dzh_CN%23rd)【获取码】SIGAI0921

[68] [浅谈动作识别TSN，TRN，ECO](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486742%26idx%3D1%26sn%3D9bac663898037746f656f9034d73d409%26chksm%3Dfdb69081cac11997a8ade63261ecbcafd67238a5e9c9b0527fd4800c8d61c34b12d2d0c21f1e%26token%3D1201469219%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0924

原创声明：本文为 SIGAI 原创文章，仅供个人学习使用，未经允许，不能用于商业目的

