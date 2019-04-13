
# 人脸识别——Patch-based Features - 小花生的博客 - CSDN博客


2018年11月11日 16:22:22[Peanut_范](https://me.csdn.net/u013841196)阅读数：214



###### 《Fully Associative Patch-based 1-to-N Matcher for Face Recognition》
2018，Lingfeng Zhang and Ioannis A. Kakadiaris
### 1.引言：
本文通过学习不同人脸块之间的相关性，提出了基于完全关联图像块的签名匹配算法（FAPSM），使得每个图像块的局部匹配身份有助于所有图像块的全局匹配身份，来提高人脸识别性能。
**所提出的匹配器由三个步骤组成。**
**首先**，基于签名，计算每个图像块的局部匹配标识和相应的匹配分数。
**然后**，学习一个完全关联的权重矩阵，以获得所有图像块的全局匹配标识和分数。
**最后**，利用L1-正则化加权对每个图像块的全局匹配标识进行组合，得到最终的匹配标识。
实验结果表明，所提出的匹配器比现有的UR2D系统具有更好的性能。在UHDB31数据集和IJB-A数据集上，RANK-1精度分别提高了3%和0.55%。
给定probe签名和gallery签名列表，首先基于补丁签名计算每个图像块的局部匹配标识和分数。然后，学习一个完全关联的权重矩阵来更新每个补丁的局部匹配标识和分数，并获得全局匹配标识和分数。术语“fully associative（完全关联的）”来源于每个补丁的局部匹配标识对于所有补丁的全局匹配标识的贡献。这样，如果一个补丁被遮挡或者不匹配，其匹配分数将通过使用完全关联的权重矩阵来降低。同时，将提高正确的匹配补丁。此外，L1-正则化加权用于组合每个补丁的全局匹配身份，而不是直接求出每个补丁的相似性得分。
**基于补丁的匹配器的过程：**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111155422346.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
**本文的贡献在于通过引入两种技术来改进人脸识别签名匹配：**
（1）通过提出一个完全关联模型来主动地关联贴片相关性，以学习不同面部贴片之间匹配结果的相关性。基于核技巧（kernel trick）学习完全联想权重矩阵。
（2）通过引入L1-正则化加权法结合所有块的全局匹配结果，提高最终匹配精度。
### 2.Signature:
给定输入人脸图像，UR2D的管道如下：face detection, landmark detection, pose estimation, 3D reconstruction, texture lifting, signature generation, and signature matching. 面部texture lifting是一种将像素值从原始2D图像映射到UV map的技术。利用3D模型和Z-Buffer技术估计每个像素的遮挡状态，并生成遮挡掩模。
在UR2D中，可以提取两类签名:姿态鲁棒脸部签名（PRFS）和深姿态鲁棒脸部签名（DPRFS）。
**PRFS和DPRFS都是基于补丁的签名。**
**在PRFS，**面部纹理图像和遮挡掩模图像首先被划分为64个非重叠的局部贴片。然后，在每个局部贴片上提取判别的DFD特征。此外，计算自遮挡编码。
**在DPRFS中，**首先将人脸纹理图像和遮挡掩模图像分成八个部分重叠的局部斑块。由于表情的变化，口部区域被忽略了。
Signature由两部分组成：特征矩阵和遮挡编码。
**基于DPRFS的签名生成过程：**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111155528620.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
### 3.Fully associative patch-based matcher
让$I^{g}$和$I^{p}$代表一对gallery和probe图像，特征矩阵和遮挡编码被表示为：$E^{g}$，$E^{p}$和$O^{g}$，$O^{p}$。特征是基于补丁的，只有非遮挡补丁有助于S，**签名匹配分数S计算为：**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111160417745.png)
其中，k表示非遮挡贴片的数目对。
然后，将整个gallery的最大得分的身份作为1-N匹配结果返回。
### 4.局部匹配与全局匹配的关系
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111160458299.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111160522272.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
具体细节，参考原论文，没细看。
[
](https://img-blog.csdnimg.cn/20181111160522272.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
### 5.实验结果：
[
](https://img-blog.csdnimg.cn/20181111160522272.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111160551493.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
### 总结：
本文提出了一种基于补丁的1-to-N特征匹配人脸识别方法，用于学习不同人脸补丁之间的相关性。学习权重矩阵来更新每个补丁的局部匹配标识，并获得全局标识。将所有贴片的全局恒等式结合起来，得到最终的匹配标识。
---

###### 注：博众家之所长，集群英之荟萃。

[
](https://img-blog.csdnimg.cn/20181111160522272.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
