
# 人脸识别——脸部属性辅助（属性引导triplet loss） - 小花生的博客 - CSDN博客


2018年11月20日 23:31:06[Peanut_范](https://me.csdn.net/u013841196)阅读数：338



###### 《Deep Attribute Guided Representation for Heterogeneous Face Recognition》
2018，Decheng Liu et al. AGTL
## 1.引言：
**异构人脸识别（Heterogeneous face recognition:HFR）**：人脸的纹理和空间结构差异大，不同的传感器（相机）和认知机制。
**动机：**人类认知机制的激励，我们自然地利用显式不变语义信息（人脸属性）来帮助解决不同模态之间的差距。
本文提出了一种新的**基于深度属性引导表示的异构人脸识别方法（DAG-HFR）**，不需要手动标记属性。采用深度卷积网络将不同情况下的人脸图像直接映射到一个紧凑的公共空间，其中距离意味着pairs对的相似性。针对端到端HFR网络，**设计了属性引导triplet loss（AGTL）**，可以有效地消除错误检测属性的缺陷。大量的实验证明：在多种异构的场景（例如视觉图像(VIS)、脸部草图(由手或软件生成)、身份证照片(嵌入卡中)、近红外图像(通过近红外设备捕获)），该方法与现有技术相比具有更好的性能。
针对异构人脸识别，自然地，我们的目标是集成人脸属性鉴别信息，以解决异构人脸识别中不同模式之间的巨大差距。
**已经存在的解决异构人脸识别的方法主要分为3个类别：**
**1）Feature descriptor based methods**
直接提取用于识别的模态不变特征。
**2）Synthesis based methods**
将一种模态中的图像变换为另一种模态，使得这些图像在均匀场景中产生，然后可以直接利用传统的均匀人脸识别方法。
**3）Common space projection based methods.**
尝试将异质人脸图像投影到潜在公共空间中，其中probe图像和gallery图像可以直接匹配。
本文提出了一种新的面向异构人脸识别的深层属性引导的表示方法（DAG-HFR）。直接整合了不同人脸的属性和身份之间的关系。提出了属性导引三重损失（AGTL）来消除错误属性预测的负面影响。我们手动选择一些判别面部属性来提高识别性能。
**论文的主要贡献：**
1）本文提出了一种深度属性引导表示的异构人脸识别，该方法能有效地集成人脸属性判别信息，在训练阶段无需人工属性标注即可自动检测出照片图像的人脸属性。
2）属性引导的三重损失是由身份和面部属性的内在关系设计的，可以减少不正确检测到的人脸属性的负面影响，使表情更具辨别力。
3）实验结果表明，与现有的HFR方法相比，所提出的方法具有更好的性能。特征维度仅有128维。
## 2.DAG-HFR
为了便于表达，本文以**人脸合成草图-照片识别**为典型例子来描述所提出的方法。
由于异构人脸图像源有限，借鉴triplet loss的方法来构建probe sketches 和 gallery photos进行人脸识别，提升模型的泛化性能。
本文使用预先训练好的多任务结构属性评估网络来检测三元组人脸照片的人脸属性。
**本文设计了一种新的属性导引triplet loss：**
考虑 sketch-face pairs {${(s_{1},p_{1}),(s_{2},p_{2}),...,(s_{N},p_{N})}$}，N是数量。
attribute guided triplet loss定义为：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120231418397.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
**其中：**$Φ$是度量函数，f表示使用L2范数归一化后的特征，$s_{i}^{attri}\in \left \{ (0,1),(1,0)) \right \}$
意味着probe sketch固有的两种属性，m是一个margin（区间），g表示输入photo图像的两种属性的相似性得分（见下图）。
目的将sketch$s_{i}$检测出的不同属性的负样本$p_{i}^{n}$推到比负样本$p_{i}^{n}$检测出的相同属性更远的位置。因此，在HFR中可以利用更具区分性的属性引导信息。
超参数λ平衡了属性可辨别性影响的重要性，实验结果表明，AGTL不仅能够有效地综合属性信息，而且能够消除错误检测属性的负面影响。
假设我们有M个面部属性，并且异构人脸训练图像p_{i}的m个属性的分别表示为$a_{i}^{m}$。
Attribute Evaluation Network的损失为：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120232159321.png)
使用cross entropy loss作为二分类的损失函数。
注意，该预先训练的属性评估网络仅用于为输入面部照片提供属性相似性得分，这在训练阶段不需要更新。
**结构框架：**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120232241390.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
## 3.模型训练和测试
网络结构参考ResNet，这三个通道共享相同的参数，以确保异构人脸图像能够映射到公共空间中，从而有效地桥接不同模态之间的差距。
本文使用CelebA数据集对这个多属性评估网络进行了预训练，CelebA数据集包含202，599张人脸图像，每张图像都标有40个人脸属性，仅针对四个选定属性 (Eyeglasses, Male,No beard and Young)进行训练。
异构人脸识别数据集：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120232313990.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
## 实验结果评估：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120232346532.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
**1.Influence of parameter margin m：**
评估了E - PRIP复合数据集中间隔m对识别准确率的影响。
m设置为{0, 0.05, 0.1, 0.15, 0.2}，λ=0.07，m在0.1时，rank50达到了做好的表现。
**2.Influence of parameter λ：**
评估了E - PRIP复合数据集中权重λ对识别准确率的影响。
λ设置为0.01, 0.03, 0.05, 0.07, 0.09，m固定为0.1，λ=0.07时rank50达到最好的效果。
**3.Influence of the attributes evaluation performance**
在这个实验中，我们手动标记训练人脸照片属性和使用预训练模型来进行属性预测，结果十分接近。提出的AGTL的准确率都超过了原始的triplet loss。
**4.Discussions on the fusion of different attributes**
在这一部分，选择了四种有区别性的二元属性(眼镜、男性、无胡须、年轻)来探索不同属性融合方法的有效性。
多个测试集验证不同方法的有效性：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120232428564.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120232440190.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
**总结：**提出了一种新的基于深度属性引导表示的异构人脸识别方法（DAG-HFR），在triplet loss中margin加入了属性约束和相应的权重系数λ。**本文m=0.1，λ=0.07。**
[
](https://img-blog.csdnimg.cn/20181120232440190.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
---

###### 注：博众家之所长，集群英之荟萃。
[
						](https://img-blog.csdnimg.cn/20181120232440190.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
[
	](https://img-blog.csdnimg.cn/20181120232440190.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
[
  ](https://img-blog.csdnimg.cn/20181120232428564.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)