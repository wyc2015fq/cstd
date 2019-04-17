# 英伟达用GAN生成脑瘤图像，训练出AI医生，准确率提升16% - 知乎
# 



> 栗子脑 发自 凹非寺
量子位 出品 | 公众号 QbitAI
![](https://pic2.zhimg.com/v2-960cd0556d2ff8acdb8ee3334231af05_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='340' height='315'></svg>)



辅助医生做**疾病诊断**的AI，已越来越常见。

从前，每只AI要吃下来自放射科的许多许多**图像数据**，学好图中的特征，才可能出师。

数据集越大越好，数据也是越**多样**越好 (样本不能太单一) 。可是相比健康的结果，研究人员**找不到那么多病变的图像**可用。

如今，医学诊断AI依然需要喂食大量的数据，但**未必**用放射科**真实捕捉**的图像了。
![](https://pic4.zhimg.com/v2-a84c21c42ac67a2eb44e63a011022917_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='400'></svg>)
英伟达与一众医疗机构合作，开发了一只医学**GAN**，用来生成脑部**核磁共振** (MRI) 的图像，专攻**脑肿瘤**识别。

他们说，以后就不愁数据集不够大、不够丰富了。

## **诊断能力有提升**

做头部核磁成像的人很多，可大部分检查结果还是**正常**。

用这样的数据集训练出的AI，见不到更多**脑肿瘤**的情况，诊断能力也会被局限。

英伟达的GAN，要生成**病变**的脑部图像。为训练数据集，填补**不正常**的那部分。
![](https://pic4.zhimg.com/v2-6de54663d189b2b9ade5ff01d4f1ecfb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='642'></svg>)
这只AI，是用PyTorch搭起，然后在英伟达DGX平台上训练的。

模型分为三个部分：

一是生成器做**脑部语义分割**，鉴别器判断真假；

二是生成器用**肿瘤语义分割**生成**MRI图像**，鉴别器判断真假；

三是生成器做**肿瘤语义分割**，鉴别器判断真假。
![](https://pic4.zhimg.com/v2-92056b073e045882c6104910b21aa8e3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='596'></svg>)
GAN吃的数据集有两个，一个是阿兹海默神经成像计划 (**ADNI**) ，另一个是多模态脑肿瘤图像分割基准 (**BRATS**) 。其中，BRATS的20%留作**测试集**。

训练完成之后，GAN生成的假MRI图像几可乱真。

用真实MRI图像与合成MRI图像的**混合数据集**训练的诊断AI，正确率达到**80%比**，仅用真实图像训练 (64%) 时，提高许多。

## **欣赏一下数据多样性**

脑部肿瘤的情况有很多种，GAN的生成结果也需要五花八门，才能更好地辅助诊断。
![](https://pic1.zhimg.com/v2-2bc52b966fcdb1e296afad6d2a3f2850_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='586' height='335'></svg>)
论文传送门：

[https://arxiv.org/pdf/1807.10225.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1807.10225.pdf)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态




