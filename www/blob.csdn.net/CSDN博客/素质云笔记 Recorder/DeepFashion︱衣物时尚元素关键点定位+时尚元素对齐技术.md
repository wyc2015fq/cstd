
# DeepFashion︱衣物时尚元素关键点定位+时尚元素对齐技术 - 素质云笔记-Recorder... - CSDN博客

2017年03月19日 11:09:18[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：11568标签：[github																](https://so.csdn.net/so/search/s.do?q=github&t=blog)[深度学习																](https://so.csdn.net/so/search/s.do?q=深度学习&t=blog)[时尚																](https://so.csdn.net/so/search/s.do?q=时尚&t=blog)[关键点定位																](https://so.csdn.net/so/search/s.do?q=关键点定位&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=时尚&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=深度学习&t=blog)个人分类：[图像︱相关技术跟踪与商业变现																](https://blog.csdn.net/sinat_26917383/article/category/6818531)
[
																								](https://so.csdn.net/so/search/s.do?q=深度学习&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=github&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=github&t=blog)

香港中文大学最近的研究成果：[Fashion Landmark Detection in the Wild](http://personal.ie.cuhk.edu.hk/~lz013/papers/fashionlandmarks_poster.pdf)
![这里写图片描述](https://img-blog.csdn.net/20170319110236878?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
github地址：[https://github.com/liuziwei7/fashion-landmarks](https://github.com/liuziwei7/fashion-landmarks)
> 这项工作提出了时尚关键点检测或时尚对齐（通过三层级联网络）,预测关键点位置上的时尚物品,如领口的角落,裙摆,袖口，同时实现了时尚衣物检索功能。
.
---一、时尚数据集：Large-scale Fashion (DeepFashion) Database

---网址：

---[http://mmlab.ie.cuhk.edu.hk/projects/DeepFashion.html](http://mmlab.ie.cuhk.edu.hk/projects/DeepFashion.html)

---可扫码看到这一数据集：

![这里写图片描述](https://img-blog.csdn.net/20170319105814005?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---目前有以下几类数据集：

---1. Category and Attribute Prediction Benchmark

![这里写图片描述](https://img-blog.csdn.net/20170319110111751?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---In-shop Clothes Retrieval Benchmark

![这里写图片描述](https://img-blog.csdn.net/20170319110134392?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---Consumer-to-shop Clothes Retrieval Benchmark

![这里写图片描述](https://img-blog.csdn.net/20170319110227440?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---Fashion Landmark Detection Benchmark

![这里写图片描述](https://img-blog.csdn.net/20170319110149803?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---[ ](https://img-blog.csdn.net/20170319110149803?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

---.

---[
](https://img-blog.csdn.net/20170319110149803?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

---二、预训练好的caffemodel

---三个预先训练好的：

---3-stage cascaded CNN models trained on upper-body clothes of Fashion

---Landmark Detection Benchmark (FLD).

---3-stage cascaded CNN models trained on lower-body clothes of Fashion

---Landmark Detection Benchmark (FLD).

---3-stage cascaded CNN models trained on full-body clothes of Fashion

---Landmark Detection Benchmark (FLD).

---.

---三、服装对齐技术

![这里写图片描述](https://img-blog.csdn.net/20170319110511477?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---结果：

![这里写图片描述](https://img-blog.csdn.net/20170319110546741?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---[ ](https://img-blog.csdn.net/20170319110546741?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

---.

---[
](https://img-blog.csdn.net/20170319110546741?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

---四、应用一：

---[用深度学习算法感知你的穿衣风格](https://mp.weixin.qq.com/s?__biz=MzI1MzU2ODEwNg==&mid=2247483835&idx=1&sn=3f478562a74630a3692cd06156672ee6&chksm=e9d334bedea4bda88968cd074e96218b23e442bcc503fe0f581471795b3599f93053fbdfc83b&mpshare=1&scene=1&srcid=0428jSrp4Ua6Qa6qO52CGNes#rd)

---来源于：大数据与多模态计算公众号

---人们常说“所见即所得。”在推荐系统中，视觉信息也扮演着十分重要的角色，尤其是在服装、珠宝、装饰等外观对人们的选择起着重大作用的领域中。因此，研究用户喜好和商品特性的视觉维度变成了一个很重要的任务。

---近年来，服装等商品的搭配、推荐受到了广泛的关注，并在基于视觉的推荐问题中取得了一定的成果。但是，目前工作对于商品的表征，往往是在一个通用的视觉特征空间中，比如CNN (Convolutional Neural Networks)网络的输出层特征。这样的视觉特征表示，对商品的类别比较敏感，却难以建模商品的不同风格。如下图所示，我们根据CNN网络的输出层特征对Amazon数据中的服装商品进行聚类，每一行表示一个类别。可以看到，不同类别的服装（比如，上衣、裙子、裤子、鞋等）被聚类到了一起，而不同风格的服装（比如，正式、休闲、运动等）没有被聚类到一起，甚至男装和女装间的区分度也很低。

---这样的视觉特征表示很难有效地用于推荐系统中，因为相似风格的商品往往会被同一个人同时购买，但在视觉特征空间中却并不相似，这就为提升推荐效果带来了难度。比如，西裤和皮鞋一般是搭配在一起并被同时购买的，但西裤和皮鞋的相似度要远远小于西裤和牛仔裤的相似度。

---根据上面的观察，我们提出了一个假设：一个商品（item）由风格（style）和类别（category）两部分构成，并且满足：

---商品（item）= 风格（style）+ 类别（category）

---基于上面的假设，我们提出了DeepStyle方法，学习用户的喜好和商品的风格属性，并应用于基于视觉的推荐中。

![这里写图片描述](https://img-blog.csdn.net/20170430113030783?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---[ ](https://img-blog.csdn.net/20170430113030783?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

---在DeepStyle模型中，我们将每个商品的图片输入到CNN网络中，其中这个CNN网络在ImageNet上经过了预训练。经过CNN，我们可以得到每个商品的视觉特征向量。然后，根据前文的假设，我们从商品的视觉特征向量中减除了该商品对应类别的隐含表达，进而得到了商品的风格特征向量。

---随后，我们将得到的风格特征向量输入到BPR (Bayesian Personalized Ranking)框架中实现个性化推荐。对每个用户，采样正负商品样本对（正样本表示实际购买了的商品，负样本表示没有购买过的商品），通过优化，尽量扩大该用户和正样本的相似度，减小和负样本的相似度。我们的模型以用户的实际购买记录为监督进行训练，以SGD (Stochastic Gradient Descent)进行参数学习，直至模型收敛。

---还对模型学习得到的商品风格特征向量进行了聚类，如下图所示，每一个方框中的商品属于一个类别：

![这里写图片描述](https://img-blog.csdn.net/20170430113103563?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---[ ](https://img-blog.csdn.net/20170430113103563?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

---可以看到，DeepStyle学到的风格特征可以很好地表征商品的不同风格，有效地挖掘用户的喜好，而在训练过程中完全没有商品风格方面的监督信息。

---.

---延伸一：DeepFashion: Powering Robust Clothes Recognition and Retrieval with Rich Annotations （CVPR 2016）

---来源：

---[基于深度学习的视觉实例搜索研究进展](https://mp.weixin.qq.com/s?__biz=MzI1NTE4NTUwOQ==&mid=2650325339&idx=1&sn=9554c2ba8e7f3177d552a174803f89c1&mpshare=1&scene=1&srcid=0708NDQXeUb5iJbTMaaOJron#rd)

---介绍了衣服识别和搜索，同样是与实例搜索相关的任务，来自于香港中文大学Ziwei Liu等人的工作。首先，本篇文章介绍了一个名为DeepFashion的衣服数据库。该数据库包含超过800K张的衣服图片，50个细粒度类别和1000个属性，并还额外提供衣服的关键点和跨姿态/跨领域的衣服对关系（cross-pose/cross-domain pair correspondences）

---然后为了说明该数据库的效果，作者提出了一种新颖的深度学习网络，FashionNet——通过联合预测衣服的关键点和属性，学习得到更具区分性的特征。该网络的总体框架如下所示：

![这里写图片描述](https://img-blog.csdn.net/20170729122341518?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---[ ](https://img-blog.csdn.net/20170729122341518?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

---FashionNet的前向计算过程总共分为三个阶段：第一个阶段，将一张衣服图片输入到网络中的蓝色分支，去预测衣服的关键点是否可见和位置。第二个阶段，根据在上一步预测的关键点位置，关键点池化层（landmark pooling layer）得到衣服的局部特征。第三个阶段，将“fc6 global”层的全局特征和“fc6 local”的局部特征拼接在一起组成“fc7_fusion”，作为最终的图像特征。FashionNet引入了四种损失函数，并采用一种迭代训练的方式去优化。这些损失分别为：回归损失对应于关键点定位，softmax损失对应于关键点是否可见和衣服类别，交叉熵损失函数对应属性预测和三元组损失函数对应于衣服之间的相似度学习。作者分别从衣服分类，属性预测和衣服搜索这三个方面，将FashionNet与其他方法相比较，都取得了明显更好的效果。

---[
](https://img-blog.csdn.net/20170729122341518?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

---总结：当有足够多的有标注数据时，深度学习可以同时学习图像特征和度量函数。其背后的思想就是根据给定的度量函数，学习特征使得特征在该度量空间下具有最好的判别性。因此端到端的特征学习方法的主要研究方向就是如何构建更好的特征表示形式和损失函数形式。

---[
](https://img-blog.csdn.net/20170729122341518?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

---公众号“素质云笔记”定期更新博客内容：

![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


---[
  ](https://img-blog.csdn.net/20170729122341518?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
