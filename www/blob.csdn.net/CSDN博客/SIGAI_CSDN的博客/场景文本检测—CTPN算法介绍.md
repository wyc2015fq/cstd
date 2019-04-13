
# 场景文本检测—CTPN算法介绍 - SIGAI_CSDN的博客 - CSDN博客
# [SIGAI_CSDN的博客](https://blog.csdn.net/sigai_csdn)


[博客首页](https://blog.csdn.net/SIGAI_CSDN)
[关于我们](https://me.csdn.net/SIGAI_CSDN)

2018年06月25日 13:14:37[SIGAI_csdn](https://me.csdn.net/SIGAI_CSDN)阅读数：1482


本文及其它机器学习、深度学习算法的全面系统讲解可以阅读《机器学习与应用》，清华大学出版社，雷明著，由SIGAI公众号作者倾力打造，自2019年1月出版以来已重印3次。
[书的购买链接](https://link.zhihu.com/?target=https%3A//item.jd.com/12504554.html)
[书的勘误，优化，源代码资源](https://link.zhihu.com/?target=http%3A//www.tensorinfinity.com/paper_78.html)
SIGAI特约作者：沪东三哥 原创声明:本文为SIGAI 原创文章，仅供个人学习使用，未经允许，不得转载，不能用于商业目的。
![](https://img-blog.csdn.net/20180625131118511?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)何为OCR ？
OCR的全称为“Optical Character Recognition” 中文翻译为光学字符识别。它是利用光学技术和计算机技术把印在或写在纸上的文字读取出来，并转换成一种计算机和人都能够理解的形式的过程。
先来看一个简单的OCR的流程：![](https://img-blog.csdn.net/20180625120019504?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)第一步，通过图像信息采集（一般就是相机），得到包含了待识别字符的图像，分析结构。
第二步，运用阈值操作等一些图像处理的方法对待测对象进行去噪和矫正。
第三步，因为文本信息的特殊性质，需要进行行列分割，对单个字符，或者是连续几个字符进行检测。
第四步，将分割后的字符图像导入识别模型中进行处理，进而得到原图中的字符信息。
场景文本识别
对于印刷字体的OCR技术如今已经相当成熟了。腾讯TIM的手机版就自带有图像文字提取的功能，微软的Office Lens的各种扫描功能呢等等，虽然不能说是百分之百正确，但是95%以上的印刷字体识别基本都是可以做到了。所以现在技术上关注的更多的是“场景文本识别”，也就是复杂环境下的字符信息的识别，如下面几张图所示。
![](https://img-blog.csdn.net/20180625120044471?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)对于复杂环境中的字符的识别，主要包括文字检测和文字识别两个步骤，这里介绍的CTPN(Detecting Text inNatural Image with Connectionist Text Proposal Network)方法就是在场景中提取文字的一个效果较好的算法，能将自然环境中的文本信息位置加以检测。
涉及到了图像中位置信息的选择，很容易联想到之前用于目标检测的R-CNN的模型。毕竟CNN（Convolutional Neural Network）在这两年的图像处理上一枝独秀已经“深入人心”。那么把“字符位置”标记成一类，然后直接放入CNN模型处理岂不美哉？不过，现实总不会这么美好，文字的多种情况、字体，以及大面积的文字信息的位置，都对我们直接用R-CNN的方法产生了干扰，让结果产生严重的偏差。应对于此，一类结合CNN优势，专门应对环境文本信息的模型也就因运而生了，CTPN正是其中的佼佼者。
CTPN算法概述
言归正传,那么算法上文本位置的准确界定是怎么做到的呢？
首先，明确待检测的目标的特点，无论是图3还是图4的样例，文字信息的特点在于它是一个序列，是由“字符、字符的一部分、多字符”组成的一个sequence。所以这个目标显然不像我们一般的目标检测中的对象那样是独立和封闭的，所以不妨使用前后关联的序列的方法，比如说RNN (Recurrent Neural Networks),利用前后文的信息来进行文本位置的测定。![](https://img-blog.csdn.net/20180625120105685?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
另外很重要的一点是，作者认为预测文本水平方向的位置比预测竖直方向上的位置要困难得多。所以检测的过程中不妨引入一个类似数学上“微分”的思想，如下图5所示，先检测一个个小的、固定宽度的文本段。在后处理部分再将这些小文本段连接起来，得到文本行。
![](https://img-blog.csdn.net/20180625120132285?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)有了CNN和RNN结合，以及数学上”微分”思想处理文字段这样的奇思妙想之后，接下来就看作者是如何将其实现的了。具体流程图如下，然后分别进行介绍。
过程的图像如下图6：
![](https://img-blog.csdn.net/20180625120148901?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)具体的步骤为：
1. 首先，用VGG16的前5个Conv stage得到feature map，大小为W*H*C
2. 用3*3的滑动窗口在前一步得到的feature map上提取特征，利用这些特征来对多个anchor进行预测,这里anchor定义与之前faster-rcnn中的定义相同，也就是帮我们去界定出目标待选区域。
3. 将上一步得到的特征输入到一个双向的LSTM中，输出W*256的结果，再将这个结果输入到一个512维的全连接层（FC）.
4. 最后通过分类或回归得到的输出主要分为三部分，根据上图从上到下依次为2k vertical coordinates:表示选择框的高度和中心的y轴的坐标；2k scores:表示的是k个anchor的类别信息，说明其是否为字符；k side-refinement表示的是选择框的水平偏移量。本文实验中anchor的水平宽度都是16个像素不变，也就是说我们微分的最小选择框的单位是 “16像素”。
5. 用文本构造的算法，将我们得到的细长的矩形（如下图7），然后将其合并成文本的序列框。
![](https://img-blog.csdn.net/20180625120213451?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**核心方法**
具体的流程已经介绍完毕了，而这些流程中有很多作者提出的实现方法需要特别关注，名称（按照论文）分别是：Detecting Text in Fine-scale proposals（选择出anchor，也就是待选的”矩形微分框“）、Recurrent Connectionist Text Proposals（双向LSTM，利用上下文本信息的RNN过程）、Side-refinement（文本构造，将多个proposal合并成直线）。
**Detecting Text in Fine-scale proposals**：
和faster-rcnn中的RPN的主要区别在于引入了”微分“思想，将我们的的候选区域切成长条形的框来进行处理。k个anchor（也就是k个待选的长条预选区域）的设置如下：宽度都是16像素，高度从11~273像素变化（每次乘以1.4），也就是说k的值设定为10。最后结果对比如下：
![](https://img-blog.csdn.net/2018062512023661?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)本文使用的方法回归出来的y轴坐标结果如下：
![](https://img-blog.csdn.net/20180625120251687?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)其中标记*的![](https://img-blog.csdn.net/20180625120311297?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**Recurrent Connectionist Text Proposals:**
其方法对应的就是之前流程中的”双向LSTM“对应的细节，将前后文的信息用到文本位置的定位当中。其中BLSTM有128个隐含层。输入为3*3*C滑动窗口的feature，输出为每个窗口所对应的256维的特征。简要表示如下：![](https://img-blog.csdn.net/20180625130537948?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)下面是使用RNN和不使用RNN的效果对比图
**Side-refinement：**
先进行文本位置的构造，Side-refinement是最后进行优化的方法。对定位出来的“小矩形框”加以合并和归纳，可以得到需要的文本信息的位置信息。我们最后保留的小矩形框是需要score>0.7的情况，也就是将下图中的红色小矩形框合并，最后生成黄色的大矩形框。
![](https://img-blog.csdn.net/20180625130612199?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
主要的思路为：每两个相近的proposal（也就是候选区）组成一个pair，合并不同的pair直到无法再合并为止。而判断两个proposal,Bi和Bj可以组成一个pair的条件为Bi—>Bj,同时Bj—>Bi;该符号的判定条件见下图。
![](https://img-blog.csdn.net/20180625130642814?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
因为这里规定了回归出来的box的宽度是16个像素，所以会导致一些位置上的误差，这时候就是Side-refinement发挥作用的时候了。定义的式子如下：
![](https://img-blog.csdn.net/20180625130723657?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180625130806367?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180625130824226?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
纵观整个流程，该方法的最大两点也是在于将RNN引入了文本检测之中，同时将待检测的结果利用“微分”的思路来减少误差，使用固定宽度的anchor来检测分割成许多块的proposal.最后合并之后的序列就是我们需要检测的文本区域。CNN和RNN的高效无缝衔接极大提升了精度，实验对比如下表所示：
![](https://img-blog.csdn.net/20180625130929694?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
说明
1. 论文： https://arxiv.org/pdf/1609.03605.pdf
2. 代码：[https://github.com/eragonruan/text-detection-ctpn](https://github.com/eragonruan/text-detection-ctpn)
（搬运，实测运行可靠，是有人根据faster-r-cnn的tensorflow版本进行 改写的，具体的训练和测试请参阅git上的readme）
3. 除流程图外其他配图和表格信息均来自原文 “Detecting Text in Natural Image with Connectionist Text Proposal Network”

参考文献
[1] Ren, S., He,K., Girshick, R., & Sun, J. (2015). Faster R-CNN: towards real-time objectdetection with region proposal networks. International Conference onNeural Information Processing Systems (Vol.39, pp.91-99). MIT Press.
[2] sTian,Z., Huang, W., He, T., He, P., & Qiao, Y. (2016). Detecting Text in Natural Image with Connectionist Text Proposal Network. European Conference on Computer Vision (pp.56-72). Springer, Cham.
[3] Olah C(2015). Understanding LSTM networks.blog, http://colah.github.io/posts/2015-08-Understanding-LSTMs/, August 27, 2015.

推荐文章
[1][机器学习-波澜壮阔40年](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247483705&idx=1&sn=c6e7c4a2e14a2469308b41eb60f155ac&chksm=fdb69caecac115b8712653600e526e99a3f6976fdaa2f6b6a09388fa6f9677ccb57b40c40ae3&scene=21#wechat_redirect)SIGAI2018.4.13.
[2][学好机器学习需要哪些数学知识？](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247483713&idx=1&sn=1e7c81381d16806ac73e15691fe17aec&chksm=fdb69cd6cac115c05f1f90b0407e3f8ae9be8719e454f908074ac0d079885b5c134e2d60fd64&scene=21#wechat_redirect)SIGAI 2018.4.17.
[3][人脸识别算法演化史](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247483726&idx=1&sn=9fef4cc1766ea4258749f8d40cc71a6e&chksm=fdb69cd9cac115cf4eba16081780c3b64c75e1e55a40bf2782783d5c28f00c6f143426e6f0aa&scene=21#wechat_redirect)SIGAI2018.4.20.
[4][基于深度学习的目标检测算法综述](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247483731&idx=1&sn=237c52bc9ddfe65779b73ef8b5507f3c&chksm=fdb69cc4cac115d2ca505e0deb975960a792a0106a5314ffe3052f8e02a75c9fef458fd3aca2&scene=21#wechat_redirect)SIGAI2018.4.24.
[5][卷积神经网络为什么能够称霸计算机视觉领域？](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247483816&idx=1&sn=fc52765b012771d4736c9be4109f910e&chksm=fdb69c3fcac115290020c3dd0d677d987086a031c1bde3429339bb3b5bbc0aa154e76325c225&scene=21#wechat_redirect)SIGAI2018.4.26.
[6][用一张图理解SVM的脉络 ](https://zhuanlan.zhihu.com/p/38459244/htt%3C/u%3Ep://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247483937&idx=1&sn=84a5acf12e96727b13fd7d456c414c12&chksm=fdb69fb6cac116a02dc68d948958ee731a4ae2b6c3d81196822b665224d9dab21d0f2fccb329&scene=21#wechat_redirect)SIGAI2018.4.28.
[7][人脸检测算法综述](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247483950&idx=1&sn=a3a5b7907b2552c233f654a529931776&chksm=fdb69fb9cac116af5dd237cf987e56d12b0d2e54c5c565aab752f3e366c0c45bfefa76f5ed16&scene=21#wechat_redirect)SIGAI2018.5.3.
[8][理解神经网络的激活函数](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247483977&idx=1&sn=401b211bf72bc70f733d6ac90f7352cc&chksm=fdb69fdecac116c81aad9e5adae42142d67f50258106f501af07dc651d2c1473c52fad8678c3&scene=21#wechat_redirect)SIGAI2018.5.5.
[9][深度卷积神经网络演化历史及结构改进脉络-40页长文全面解读](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247484037&idx=1&sn=13ad0d521b6a3578ff031e14950b41f4&chksm=fdb69f12cac11604a42ccb37913c56001a11c65a8d1125c4a9aeba1aed570a751cb400d276b6&scene=21#wechat_redirect)SIGAI2018.5.8.
[10][理解梯度下降法](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247484111&idx=1&sn=4ed4480e849298a0aff828611e18f1a8&chksm=fdb69f58cac1164e844726bd429862eb7b38d22509eb4d1826eb851036460cb7ca5a8de7b9bb&scene=21#wechat_redirect)SIGAI2018.5.11.
[11][循环神经网络综述—语音识别与自然语言处理的利器](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247484310&idx=1&sn=0fc55a2784a894100a1ae64d7dbfa23d&chksm=fdb69e01cac1171758cb021fc8779952e55de41032a66ee5417bd3e826bf703247e243654bd0&scene=21#wechat_redirect)SIGAI2018.5.15
[12][理解凸优化](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247484439&idx=1&sn=4fa8c71ae9cb777d6e97ebd0dd8672e7&chksm=fdb69980cac110960e08c63061e0719a8dc7945606eeef460404dc2eb21b4f5bdb434fb56f92&scene=21#wechat_redirect)SIGAI2018.5.18
[13][【实验】理解SVM的核函数和参数](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247484495&idx=1&sn=4f3a6ce21cdd1a048e402ed05c9ead91&chksm=fdb699d8cac110ce53f4fc5e417e107f839059cb76d3cbf640c6f56620f90f8fb4e7f6ee02f9&scene=21#wechat_redirect)SIGAI 2018.5.22
[14][ 【SIGAI综述】行人检测算法](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247484523&idx=1&sn=ddaa70c4b92f6005d9bbd6b3a3fe4571&chksm=fdb699fccac110ea14e6adeb873a00d6ee86dd4145ddf8e90c9b878b908ac7b7655cfa51dab6&scene=21#wechat_redirect)SIGAI2018.5.25
[15][机器学习在自动驾驶中的应用—以百度阿波罗平台为例](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247484540&idx=1&sn=733332042c31e1e18ad800f7f527893b&chksm=fdb699ebcac110fd6607c1c99bc7ebed1594a8d00bc454b63d7f518191bd72274f7e61ca5187&scene=21#wechat_redirect)(上) SIGAI2018.5.29
[16][理解牛顿法](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247484651&idx=1&sn=a0e4ca5edb868fe3eae9101b71dd7103&chksm=fdb6997ccac1106a61f51fe9f8fd532045cc5d13f6c75c2cbbf1a7c94c58bcdf5f2a6661facd&scene=21#wechat_redirect)SIGAI2018.5.31
[17][【群话题精华】5月集锦—机器学习和深度学习中一些值得思考的问题 ](/u%3Em/s?__biz=MzU4MjQ3MDkwNA==&mid=2247484658&idx=1&sn=f5c9f92c272c75883bf8e6f532559f11&chksm=fdb69965cac11073f49048caef5d7b9129614090a363d9ef7f3d1b9bc59948d2217d2bca7b7b&scene=21#wechat_redirect)SIGAI 2018.6.1
[18][大话Adaboost算法](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247484692&idx=1&sn=9b389aa65208c778dddf17c601afbee1&chksm=fdb69883cac1119593934734e94c3b71aa68de67bda8a946c1f9f9e1209c3b6f0bf18fed99b8&scene=21#wechat_redirect)SIGAI2018.6.2
[ 19][FlowNet到FlowNet2.0：基于卷积神经网络的光流预测算法](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247484711&idx=1&sn=bb7644e101b5924f54d6800b952dc3aa&chksm=fdb698b0cac111a6605f5b9b6f0478bf21a8527cfad2342dbaaf624b4e9dcc43c0d85ae06deb&scene=21#wechat_redirect)SIGAI2018.6.4
[20][理解主成分分析(PCA)](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247484754&idx=1&sn=b2c0d6798f44e13956bb42373e51d18c&chksm=fdb698c5cac111d3e3dca24c50aafbfb61e5b05c5df5b603067bb7edec8db049370b73046b24&scene=21#wechat_redirect)SIGAI2018.6.6
[21][人体骨骼关键点检测综述](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247484784&idx=1&sn=ceafb54203f4e930ae457ad392b9f89c&chksm=fdb698e7cac111f13d8229d7dcc00b4a7305d66de3da1bd41e7ecc1d29bfa7be520d205c53e9&scene=21#wechat_redirect)SIGAI2018.6.8
[22][理解决策树](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247484827&idx=1&sn=043d7d0159baaddfbf92ed78ee5b1124&chksm=fdb6980ccac1111a9faeae7f517fee46a1dfab19612f76ccfe5417487b3f090ab8fc702d18b8&scene=21#wechat_redirect)SIGAI2018.6.11
[23][用一句话总结常用的机器学习算法](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247484859&idx=1&sn=2c4db22fb538953a62a90983e3e1f99d&chksm=fdb6982ccac1113a82e92be325bb07a947d54090274654375f3b50e11e1abd809fb7358bde16&scene=21#wechat_redirect)SIGAI2018.6.13
[24][目标检测算法之YOLO](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247484909&idx=1&sn=c02ee17e5175230ed39ad63e73249f5c&chksm=fdb6987acac1116c0108ec28424baf4ea16ca11d2b13f20d4a825d7b2b82fb8765720ebd1063&scene=21#wechat_redirect)SIGAI2018.6.15
[25][理解过拟合](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247484954&idx=1&sn=c28b7f07c22466e91b1ef90e9dbe3ad1&chksm=fdb69b8dcac1129bc6e78fca1d550e2b18238ad1c240c73b280d4e529f9f93c4626b3ac45ea2&scene=21#wechat_redirect)SIGAI 2018.6.18
[26][理解计算：从√2到AlphaGo ——第1季 从√2谈起](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247484981&idx=1&sn=d3003468b9853851923844812993e060&chksm=fdb69ba2cac112b4dac620d52100ebd033eb679f29340726a67297c4d6980b16c7cc91122028&scene=21#wechat_redirect)SIGAI2018.6.20
原创声明：本文为SIGAI 原创文章，仅供个人学习使用，未经允许，不得转载，不能用于商业目的。
![](https://img-blog.csdn.net/20180625131104415?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

