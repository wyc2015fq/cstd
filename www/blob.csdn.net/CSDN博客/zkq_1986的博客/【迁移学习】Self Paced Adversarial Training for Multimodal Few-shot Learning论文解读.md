# 【迁移学习】Self Paced Adversarial Training for Multimodal Few-shot Learning论文解读 - zkq_1986的博客 - CSDN博客





2018年12月03日 15:41:33[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：51
个人分类：[神经网络](https://blog.csdn.net/zkq_1986/article/category/6338867)








- paper：Self Paced Adversarial Training for Multimodal Few-shot Learning

20181127 WACV-19 Self Paced Adversarial Training for Multimodal Few-shot Learning.
- 方法
- 源域训练GAN网络

利用源域数据训练GAN网络。具体地，GAN网络设成stackGAN网络，所谓stackGAN网络是指，用l个生成器G，来逐步生成分辨率高的图像。形式化的stackGAN如下：

G1 –> image1 -> G2 -> image2 -> … ->image l-1 -> Gl -> image l
- 目标域训练GAN网络

将stackGAN中判别器D的最后一层网络神经元个数从原来的类别个数改为新的类别个数，从而产生新的判别器D’。
- 引入文本来辅助分类

每张图像用10个左右的文字描述，用这些文字生成图像，通过判别器D’进行每个类别下对应生成图像的排序，取排序最前的图像作为可信图像，放入真实图像中，一起来训练G和D’。

![](https://img-blog.csdnimg.cn/2018120315404478.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2,size_16,color_FFFFFF,t_70)
- 实验
- 数据来源

CUB with bird data and Oxford-102 containing flower data.

CUB数据，包含11,788 images of 200 different bird species，每个类别月30张图像。
- 参数设置

During representation learning, we train a StackGAN for 900 epochs. Similar to [38], we use Adam [10] for optimization. We set the learning rate _ to 2 _ 10􀀀4 and the batch size to 24 for both G and D. In the initialization phase for self-paced learning, we construct D0 by replacing the last layer of D by a linear softmax layer. The resulting network is then optimized using the cross-entropy loss function and a SGD optimizer with learning rate _ = 10􀀀3 and momentum 0:5. Batch size is set to 32 and training proceeds for 20 epochs. Self-paced learning of D0 continues to use the same settings (i.e. SGD with _ = 10􀀀3 and momentum 0:5, minimizing a cross-entropy loss). Additionally, Adam’s learning rate for G is reduced to 2_10􀀀5. In every iteration we choose exactly one generated image.
- 实验结果

![](https://img-blog.csdnimg.cn/20181203154004406.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181203153937288.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2,size_16,color_FFFFFF,t_70)

Finetuning为原始的GAN网络和简单D网络的迁移，属于meta-learning，可以学习到源域与目标域共有的特征。

SPL-D’G为作者提出的模型

SPL表示self paced learning，翻译为自步数更新，意思是说用生成的可信的图像扩充到原有图像中，来训练GAN网络。





