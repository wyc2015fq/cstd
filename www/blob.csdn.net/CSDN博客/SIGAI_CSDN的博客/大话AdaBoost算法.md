
# 大话AdaBoost算法 - SIGAI_CSDN的博客 - CSDN博客
# [SIGAI_CSDN的博客](https://blog.csdn.net/sigai_csdn)


[博客首页](https://blog.csdn.net/SIGAI_CSDN)
[关于我们](https://me.csdn.net/SIGAI_CSDN)

2018年06月12日 14:55:44[SIGAI_csdn](https://me.csdn.net/SIGAI_CSDN)阅读数：288


本文及其它机器学习、深度学习算法的全面系统讲解可以阅读《机器学习与应用》，清华大学出版社，雷明著，由SIGAI公众号作者倾力打造，自2019年1月出版以来已重印3次。
[书的购买链接](https://link.zhihu.com/?target=https%3A//item.jd.com/12504554.html)
[书的勘误，优化，源代码资源](https://link.zhihu.com/?target=http%3A//www.tensorinfinity.com/paper_78.html)
原创声明：本文为 SIGAI 原创文章，仅供个人学习使用，未经允许，不能用于商业目的。

### 算法的诞生
AI 39年（公元1995年），扁鹊成立了一家专治某疑难杂症的医院，经过半年的精心筹备，硬件设施已全部到位，只缺经验丰富的医生前来坐诊。找几个猎头打听了一下，乖乖，请一个资深专家（总监头衔的），一年的工资就得250万。这恐怕还不够去知名搜索引擎投放广告！
穷则思变，扁鹊院长想来想去，找到了两个天才的顾问，名叫Freund和Schapire，想请他们出出主意，怎样用较低的成本解决医生的问题。这两位老兄想到了同一个点子：
三个臭皮匠，赛过诸葛亮
我们玩人海战术！不如去医学院招一批应届生，给他们训练一段时间然后上岗，组成一个会诊小组，一起来给病人看病，集体决策。扁鹊很快招来了8个年轻的新手：
赵大夫，钱大夫，孙大夫，李大夫，周大夫，吴大夫，郑大夫，王大夫
![](https://img-blog.csdn.net/20180612142424910?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
怎么训练这些新人呢？两个顾问设计出了一套培训方案：
用大量的病例让这些新手依次学习，每个大夫自己琢磨怎样诊断，学习结束后统计一下每个人在这些病例上的诊断准确率
训练时，前面的大夫误诊的病例，后面的大夫要重点学习研究，所谓查缺补漏
训练结束之后，给每个大夫打分，如果一个大夫对病例学习的越好，也就是说在这些学习的病例集上诊断的准确率越高，他在后面诊断病人时的话语权就越大
扁鹊院长跑到其他医院买回了1万个病例，这些病例是这样的：
|血糖
|白细胞
|...
|...
|阴性/阳性
|
|...
|...
|...
|...
|...
|
|...
|...
|...
|...
|...
|
|...
|...
|...
|...
|...
|
|...
|...
|...
|...
|...
|
|...
|...
|...
|...
|...
|
接下来培训过程开始了。首先接受培训的是赵大夫，经过学习总结，他摸索出了一套诊断规则，这套规则表现很不错，至少在学习用的病例集上，达到了70%的诊断准确率。学习完成之后，他给每一条病例调整了权重，被他误诊的病例，权重增大，诊断正确的病例，权重调小，以便于后面的医生有重点的学习。
接下来让钱大夫学习，他同样学习这些病例，但重点要关注被赵大夫误诊的那些病例，经过一番训练，钱大夫达到了75%的准确率。学习完之后，他也调整了这些病例的权重，被他误诊的病例，加大权重，否则减小权重。
后面的过程和前面类似，依次训练孙大夫，李大夫，周大夫，吴大夫，郑大夫，王大夫，每个大夫在学习的时候重点关注被前面的大夫误诊的病例，学习完之后调整每条病例的权重。这样到最后，王大夫对前面这些大夫都误诊的病例特别擅长，专攻这些情况的疑难杂症！
学习期结束之后，扁鹊院长汇总出了这8个大夫的诊断准确率：
|赵大夫
|70%
|
|钱大夫
|75%
|
|孙大夫
|60%
|
|李大夫
|55%
|
|周大夫
|55%
|
|吴大夫
|60%
|
|郑大夫
|75%
|
|王大夫
|60%
|
当所有大夫都培训完成之后，就可以让他们一起坐堂问诊了。Freund和Schapire设计出了这样一套诊断规则：来一个病人之后，8个大夫一起诊断，然后投票。如果一个大夫之前在学习时的诊断准确率为p，他在投票时的话语权是：
![](https://img-blog.csdn.net/20180612142528761?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
后面Freund和Schapire会解释这个规则的来由。按照这个计算规则，8个大夫的话语权为：
|赵大夫
|0.424
|
|钱大夫
|0.550
|
|孙大夫
|0.203
|
|李大夫
|0.100
|
|周大夫
|0.100
|
|吴大夫
|0.203
|
|郑大夫
|0.550
|
|王大夫
|0.203
|
这样诊断结果的计算方法为，先汇总整合8个大夫的诊断结果：
![](https://img-blog.csdn.net/20180612142619238?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在这里对病人的诊断结果有两种可能，阳性和阴性，我们量化表示，+1表示阳性，-1表示阴性。
最后的诊断结果是：如果上面计算出来的s值大于0，则认为是阳性，否则为阴性。
第一个病人来了，8个大夫一番诊断之后，各自给出了结果：
|赵大夫
|阳性
|
|钱大夫
|阳性
|
|孙大夫
|阳性
|
|李大夫
|阴性
|
|周大夫
|阳性
|
|吴大夫
|阴性
|
|郑大夫
|阳性
|
|王大夫
|阳性
|
现在是投票集体决策的时候了。投票值为：
![](https://img-blog.csdn.net/20180612143041159?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
按照规则，这个病人被判定为阳性。医院运营了3个月，效果出奇的好，扁鹊院长统计了一下，诊断准确率居然高达95%，不比一个资深老专家差！每个医生一年的工资10万，8个医生总共才80万，这比请一个资深专家要便宜170万，太划算了！
这次成功之后，Freund和Schapire决定把这种方法推广到其他行业，用于解决一些实际问题。这些行业要解决的问题都有一个特点：要做出一个决策，这个决策有两种可能，例如银行要根据客户的收入、负债情况、信用记录等信息决定给客户贷款还是不贷款；人脸识别公司要判断一张图像是人脸还是不是人脸。这就是机器学习中的二分类问题，给定一个样本数据，判断这个样本的类别。对于上面的疾病诊断来说，样本数据就是病人的各项检查数据，类别是阴性和阳性。两位天才给这种方法取了一个名字：
AdaBoost算法
就这样，机器学习算法家族中的一个年轻小伙伴诞生了，没有想到，他后来在很多应用中都大显身手而被载入史册。（本故事纯属虚构）

### 集成学习
AdaBoost算法是一种集成学习（ensemble learning）方法。集成学习是机器学习中的一类方法，它对多个机器学习模型进行组合形成一个精度更高的模型，参与组合的模型称为弱学习器（weak learner）。在预测时使用这些弱学习器模型联合起来进行预测；训练时需要用训练样本集依次训练出这些弱学习器。典型的集成学习算法是随机森林和boosting算法，而AdaBoost算法是boosting算法的一种实现版本。

### 强分类器与弱分类器
AdaBoost算法的全称是自适应boosting（Adaptive Boosting），是一种用于二分类问题的算法，它用弱分类器的线性组合来构造强分类器，可以看成是前面介绍的新手大夫会诊例子的抽象。
弱分类器的性能不用太好，仅比随机猜测强，依靠它们可以构造出一个非常准确的强分类器。强分类器的计算公式为：
![](https://img-blog.csdn.net/20180612142932693?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
其中x是输入向量，是强分类器，是弱分类器，是弱分类器的权重，T为弱分类器的数量，弱分类器的输出值为+1或-1，分别对应正样本和负样本。这里的弱分类器就是上面的8位医生，强分类器代表他们的集体决策。弱分类器的结果就是前面例子中每个医生的诊断结果，如果一个医生的判定结果为+1，表示病人为阳性，-1则为阴性。
分类时的判定规则为：
![](https://img-blog.csdn.net/20180612143320372?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
其中sgn是符号函数，是机器学习中经常会使用的一个函数，定义为：
![](https://img-blog.csdn.net/2018061214345786?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
强分类器的输出值也为+1或-1，同样对应于正样本和负样本，也就是医生集体会诊的结果。这里的强分类器模型就是这就是上面的诊断规则的抽象化。弱分类器和它们的权重通过训练算法得到，后面我们会介绍。之所以叫弱分类器，是因为它们的精度不用太高，对于二分类问题，只要保证准确率大于0.5即可，即比随机猜测强，随机猜测也有50%的准确率。在上面的例子中，即使每个医生的准确率只有60%-75%，集体决策的准确率仍然能够达到95%以上。这种看似简单的组合，能够带来算法精度上的大幅度提升。

### 训练算法
下面来看AdaBoost算法的模型是怎么训练出来的，这是训练8位医生过程的抽象。算法依次训练每一个弱分类器，并确定它们的权重值。
在这里，训练样本带有权重值，初始时所有样本的权重相等，在训练过程中，被前面的弱分类器错分的样本会加大权重，反之会减小权重，这样接下来的弱分类器会更加关注这些难分的样本。弱分类器的权重值根据它的准确率构造，精度越高的弱分类器权重越大。
给定L个训练样本![](https://img-blog.csdn.net/20180612143559397?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)，其中![](https://img-blog.csdn.net/20180612143640234?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)是特征向量，![](https://img-blog.csdn.net/20180612143732193?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)为类别标签，其值为+1或-1。训练算法的流程为：
![](https://img-blog.csdn.net/20180612144110555?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
最后得到强分类器：
![](https://img-blog.csdn.net/2018061214531461?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
根据计算公式，错误率低的弱分类器权重大，它是准确率的增函数。沿用医生集体会诊的例子，如果在之前的诊断中医生的技术更好，对病人情况的判断更准确，那么可以加大他在此次会诊时说话的分量即权重。弱分类器在训练样本集上的错误率计算公式为：
![](https://img-blog.csdn.net/20180612145341499?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在这里考虑了样本权重值。因为可以保证在训练集上弱分类器的正确率大于0.5，所以有：
![](https://img-blog.csdn.net/20180612144423284?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
因此弱分类器的权重大于0。弱分类器的错误率小于0.5是能保证的，如果准确率小于0.5，只需要将弱分类器的输出反号即可。对于被弱分类器正确分类的样本，有：
![](https://img-blog.csdn.net/20180612144438523?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
对于被弱分类器错误分类的样本，有：
![](https://img-blog.csdn.net/20180612144809969?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
因此样本权重更新公式可以简化为：
![](https://img-blog.csdn.net/20180612144825830?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
由于：
![](https://img-blog.csdn.net/20180612144839832?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
它可以进一步可以简化成：
![](https://img-blog.csdn.net/2018061214485222?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
被上一个弱分类器错误分类的样本权重会增大，正确分类的样本权重减小，训练下一个弱分类器时算法会关注在上一轮中被错分的样本。这对应于每个医生在学习之后对病例权重的调整。给样本加权重是有必要的，如果样本没有权重，每个弱分类器的训练样本是相同的，训练出来的弱分类器也是一样的，这样训练多个弱分类器没有意义。AdaBoost算法的原则是：
关注之前被错分的样本，准确率高的弱分类器有更大的权重。
上面的算法中并没有说明弱分类器是什么样的，具体实现时应该选择什么样的分类器作为弱分类器？在实际应用时一般用深度很小的决策树，在后面将会详细介绍。强分类器是弱分类器的线性组合，如果弱分类器是线性函数，无论怎样组合，强分类器都是线性的，因此应该选择非线性的分类器做弱分类器。至此，我们介绍了AdaBoost算法的基本原理与训练过程，在后面的文章中，我们会介绍这种算法的理论依据，以及其他版本的实现，在现实问题中的应用。
原创声明：本文为[SIGAI](https://mp.weixin.qq.com/s/G9gW2ghTII57jGmXSIaf7w)原创文章，仅供个人学习使用，未经允许，不能用于商业目的。
推荐阅读
[1][机器学习-波澜壮阔40年](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247483705&idx=1&sn=c6e7c4a2e14a2469308b41eb60f155ac&chksm=fdb69caecac115b8712653600e526e99a3f6976fdaa2f6b6a09388fa6f9677ccb57b40c40ae3&scene=21%22%20%5Cl%20%22wechat_redirect%22%20%5Ct%20%22_blank)SIGAI 2018.4.13.
[2][学好机器学习需要哪些数学知识？](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247483713&idx=1&sn=1e7c81381d16806ac73e15691fe17aec&chksm=fdb69cd6cac115c05f1f90b0407e3f8ae9be8719e454f908074ac0d079885b5c134e2d60fd64&scene=21)SIGAI 2018.4.17.
[3][人脸识别算法演化史](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247483726&idx=1&sn=9fef4cc1766ea4258749f8d40cc71a6e&chksm=fdb69cd9cac115cf4eba16081780c3b64c75e1e55a40bf2782783d5c28f00c6f143426e6f0aa&s)SIGAI 2018.4.20.
[4][基于深度学习的目标检测算法综述](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247483731&idx=1&sn=237c52bc9ddfe65779b73ef8b5507f3c&chksm=fdb69cc4cac115d2ca505e0deb975960a792a0106a5314ffe3052f8)SIGAI 2018.4.24.
[5][卷积神经网络为什么能够称霸计算机视觉领域？](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247483816&idx=1&sn=fc52765b012771d4736c9be4109f910e&chksm=fdb69c3fcac115290020c3dd0d)SIGAI 2018.4.26.
[6][用一张图理解SVM的脉络 ](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247483937&idx=1&sn=84a5acf12e96727b13fd7d456c414c1)SIGAI 2018.4.28.
[7][人脸检测算法综述](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247483950&idx=1&sn=a3a5b7)SIGAI 2018.5.3.
[8][理解神经网络的激活函数](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=224748)SIGAI 2018.5.5.
[9][深度卷积神经网络演化历史及结构改进脉络-40页长文全面解读](http://mp.weixin.qq.com/s?__biz=MzU4)SIGAI 2018.5.8.
[10][理解梯度下降法](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247484111&idx=1&sn=4ed4480e849298a0aff828611e18f1a8&chksm=fdb69f58cac1164e844726bd429862eb7b38d22509eb4d1826eb851036460cb7ca5a8de7b9bb&scene=21%22%20%5Cl%20%22wechat_redirect%22%20%5Ct%20%22_blank)SIGAI 2018.5.11
[11][循环神经网络综述—语音识别与自然语言处理的利器](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247484310&idx=1&sn=0fc55a2784a894100a1ae64d7dbfa23d&chksm=fdb69e01cac1171758cb021fc8779952e55de41032a66ee5417bd3e826bf703247e243654bd0&scene=21)SIGAI 2018.5.15
[12][理解凸优化](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247484439&idx=1&sn=4fa8c71ae9cb777d6e97ebd0dd8672e7&chksm=fdb69980cac110960e08c63061e0719a8dc7945606eeef460404dc2eb21b4f5bdb434fb56f92&scene=21%22%20%5Cl%20%22wechat_redirect%22%20%5Ct%20%22_blank)SIGAI 2018.5.18
[13][【实验】理解SVM的核函数和参数](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247484495&idx=1&sn=4f3a6ce21cdd1a048e402ed05c9ead91&chksm=fdb699d8cac110ce53f4fc5e417e107f839059cb76d3cbf640c6f56620f90f8fb4e7f6ee02f9&scene=21)SIGAI 2018.5.22
[14][【SIGAI综述】 行人检测算法](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247484523&idx=1&sn=ddaa70c4b92f6005d9bbd6b3a3fe4571&chksm=fdb699fccac110ea14e6adeb873a00d6ee86dd4145ddf8e90c9b878b908ac7b7655cfa51dab6&scene=21)SIGAI 2018.5.25
[15][机器学习在自动驾驶中的应用—以百度阿波罗平台为例](https://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247484540&idx=1&sn=733332042c31e1e18ad800f7f527893b&chksm=fdb699ebcac110fd6607c1c99bc7ebed1594a8d00bc454b63d7f518191bd72274f7e61ca5187%22%20%5Cl%20%22rd)（上） SIGAI 2018.5.29
[16][理解牛顿法](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247484651&idx=1&sn=a0e4ca5edb868fe3eae9101b71dd7103&chksm=fdb6997ccac1106a61f51fe9f8fd532045cc5d13f6c75c2cbbf1a7c94c58bcdf5f2a6661facd&scene=21%22%20%5Cl%20%22wechat_redirect%22%20%5Ct%20%22_blank)SIGAI 2018.5.31
[17][【群话题精华】5月集锦—机器学习和深度学习中一些值得思考的问题 ](http://mp.weixin.qq.com/s?__biz=MzU4MjQ3MDkwNA==&mid=2247484658&idx=1&sn=f5c9f92c272c75883bf8e6f532559f11&chksm=fdb69965cac11073f49048caef5d7b9129614090a363d9ef7f3d1b9bc59948d2217d2bca7b7b&scene=21%22%20%5Cl%20%22wechat_redirect%22%20%5Ct%20%22_blank)SIGAI 2018.6.1
![](https://img-blog.csdn.net/20180612144928663?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





