# K近邻算法 - 知乎
# 

原创声明：本文为 SIGAI 原创文章，仅供个人学习使用，未经允许，不能用于商业目的。 

我们在网上购买水果的时候经常会看到同一种水果会标有几种规格对应不同价格进行售卖，水果分级售卖已经是电商中常见的做法，那么水果分级具体是怎么操作的呢？一种简单的做法是根据水果果径的大小进行划分。今年老李家苹果丰收了，为了能卖个好价钱，老王打算按照果径对苹果进行分级。想法是很好的，但是面对成千上万的苹果这可愁坏了老李。老李的儿子小李是计算机系毕业的，他知道这件事后设计了一个算法，按照老李的要求根据果径大小定义了5个等级

> 70mm左右（<72.5mm）
75mm左右(>=72.5mm&&<77.5mm）
80mm左右(>=77.5mm&&<82.5mm）
85mm左右(>=82.5mm&&<87.5mm）
90mm左右(>=87.5mm）

如下图：
![](https://pic4.zhimg.com/v2-04ee6cc138ebbbf8d7fed4bb5d835c63_b.jpg)
当一个未分级的苹果拿到后可以首先将这个苹果的果径测量出来，然后再和这5个等级的苹果进行对照，假如未分级苹果的果径是82mm则划分为第三个等级，如果是83mm则划分为第二个等级，以此类推。基于这个原则小李发明了一个分级装置，见下图，大大提高了工作效率，很快将老李的问题解决了。
![](https://pic2.zhimg.com/v2-080519652f90fd3f6edbe5e95903ae29_b.jpg)
老李的问题是一个经典的最近邻模板匹配，根据一个已知类别参考模板对未分类的数据进行划分，小李选择的每个类的模板数是一，现实生活中的问题往往会复杂很多，可能需要多个参考模板进行综合决策，当选定的模板数为k的时候就是k近邻算法的思想了，最近邻算法是k近邻算法k=1时的一种特殊情况。

k近邻算法简称kNN算法，由Thomas等人在1967年提出[1]。它基于以下思想：要确定一个样本的类别，可以计算它与所有训练样本的距离，然后找出和该样本最接近的k个样本，统计这些样本的类别进行投票，票数最多的那个类就是分类结果。因为直接比较样本和训练样本的距离，kNN算法也被称为基于实例的算法。

## 基本概念

确定一个样本所属类别的一种最简单的方法是直接比较它和所有训练样本的相似度，然后将其归类的最相似的样本所属的那个类，这是一种模板匹配的思想。下图6.1是使用k近邻思想进行分类的一个例子：
![](https://pic4.zhimg.com/v2-7a5d825c78902ba1f25d343544095e3b_b.jpg)图6.1 k近邻分类示意图
在上图中有红色和绿色两类样本。对于待分类样本即图中的黑色点，我们寻找离该样本最近的一部分训练样本，在图中是以这个矩形样本为圆心的某一圆范围内的所有样本。然后统计这些样本所属的类别，在这里红色点有12个，圆形有2个，因此把这个样本判定为红色这一类。上面的例子是二分类的情况，我们可以推广到多类，k近邻算法天然支持多类分类问题。

## 预测算法

k近邻算法没有求解模型参数的训练过程，参数k由人工指定，它在预测时才会计算待预测样本与训练样本的距离。

对于分类问题，给定L个训练样本 ![(x_{i},y_{i})](https://www.zhihu.com/equation?tex=%28x_%7Bi%7D%2Cy_%7Bi%7D%29) ，其中 ![x_{i}](https://www.zhihu.com/equation?tex=x_%7Bi%7D) 为维特征向量， ![y_{i}](https://www.zhihu.com/equation?tex=y_%7Bi%7D) 为标签值，设定参数k

，假设类型数为c，待分类样本的特征向量为x。预测算法的流程为：

> 1.在训练样本集中找出离x最近的k个样本，假设这些样本的集合为N。
2.统计集合N中每一类样本的个数 ![c_{i},i=1,...,c](https://www.zhihu.com/equation?tex=c_%7Bi%7D%2Ci%3D1%2C...%2Cc) 。
3.最终的分类结果为 ![argmax_{i}c_{i}](https://www.zhihu.com/equation?tex=argmax_%7Bi%7Dc_%7Bi%7D) 。

在这里![argmax_{i}c_{i}](https://www.zhihu.com/equation?tex=argmax_%7Bi%7Dc_%7Bi%7D)表示最大的 ![c_{i}](https://www.zhihu.com/equation?tex=c_%7Bi%7D) 值对应的那个类i。如果k=1，k近邻算法退化成最近邻算法。

k近邻算法实现简单，缺点是当训练样本数大、特征向量维数很高时计算复杂度高。因为每次预测时要计算待预测样本和每一个训练样本的距离，而且要对距离进行排序找到最近的k个样本。我们可以使用高效的部分排序算法，只找出最小的k个数；另外一种加速手段是k-d树实现快速的近邻样本查找。

一个需要解决的问题是参数k的取值。这需要根据问题和数据的特点来确定。在实现时可以考虑样本的权重，即每个样本有不同的投票权重，这称方法称为为带权重的k近邻算法。另外还其他改进措施，如模糊k近邻算法[2]。

kNN算法也可以用于回归问题。假设离测试样本最近的k个训练样本的标签值为 ![y_{i}](https://www.zhihu.com/equation?tex=y_%7Bi%7D) ，则对样本的回归预测输出值为：
![](https://pic2.zhimg.com/v2-610545d2978e90bb86ff5ffbf8065d55_b.jpg)
即所有邻居的标签均值，在这里最近的k个邻居的贡献被认为是相等的。同样的也可以采用带权重的方案。带样本权重的回归预测函数为：
![](https://pic4.zhimg.com/v2-2fbff4f6de27b51b961ac2f026ea3737_b.jpg)
其中 ![w_{i}](https://www.zhihu.com/equation?tex=w_%7Bi%7D) 为第i个样本的权重。权重值可以人工设定，或者用其他方法来确定，例如设置为与距离成反比。

## 距离定义

根据前面的介绍，kNN算法的实现依赖于样本之间的距离值，因此需要定义距离的计算方式。接下来介绍常用的几种距离定义，它们适用于不同特点的数据。

假设两个向量之间的距离为 ![d(x_{i},x_{j})](https://www.zhihu.com/equation?tex=d%28x_%7Bi%7D%2Cx_%7Bj%7D%29) ，这是一个将两个维数相同的向量映射为一个实数的函数。距离函数必须满足以下条件，第一个条件是三角不等式：
![](https://pic1.zhimg.com/v2-cc1baf6c0c2a111538aa922c1b3e0528_b.jpg)
这和我们学习的几何中的三角不等式吻合。第二个条件是非负性，即距离不能是一个负数：
![](https://pic1.zhimg.com/v2-3e87a1084006c8c9876efc039f0a2bb4_b.jpg)
第三个条件是对称性，即A到B的距离和B到A的距离必须相等：
![](https://pic3.zhimg.com/v2-1e0e73ea9898efc5598af17e49f3e65e_b.jpg)
第5个条件是区分性，如果两点间的距离为0，则两个点必须相同：
![](https://pic4.zhimg.com/v2-ca2f82594afacea28d46bf980e0c26a3_b.jpg)
满足上面4个条件的函数都可以用作距离定义。

## **常用距离定义**

常用的有欧氏距离，Mahalanobis距离等。欧氏距离是最常见的距离定义，它就是n维欧氏空间中两点之间的距离。对于 ![R^{n}](https://www.zhihu.com/equation?tex=R%5E%7Bn%7D) 空间中有两个点x和y，它们之间的距离定义为：
![](https://pic3.zhimg.com/v2-a5cdbb06b62be474b16f1f103b3e4406_b.jpg)
这是我们最熟知的距离定义。在使用欧氏距离时应该尽量将特征向量的每个分量归一化，以减少因为特征值的尺度范围不同所带来的干扰。否则数值小的特征分量会被数值大的特征分量淹没。例如，特征向量包含两个分量，分别为身高和肺活量，身高的范围是150-200厘米，肺活量为2000-9000，如果不进行归一化，身高的差异对距离的贡献显然为被肺活量淹没。欧氏距离只是将特征向量看做空间中的点，并没有考虑这些样本特征向量的概率分布规律。

Mahalanobis距离是一种概率意义上的距离，给定两个向量x和y以及矩阵S，它定义为：
![](https://pic4.zhimg.com/v2-01e4458630d0bfd6996b766f2d8566ef_b.jpg)
要保证根号内的值非负，即矩阵S必须是半正定的。这种距离度量的是两个随机向量的相似度。当矩阵S为阶单位矩阵I时，Mahalanobis距离退化为欧氏距离。矩阵可以通过计算训练样本集的协方差矩阵得到，也可以通过训练样本学习得到，优化某一目标函数。

对于矩阵如何确定的问题有不少的研究，代表性的有文献[9-12]，其中文献[9]提出的方法具有很强的指导意义和应用价值。文献[9]指出，kNN算法的精度在很大程度上依赖于所使用的距离度量标准，为此他们提出了一种从带标签的样本集中学习得到距离度量矩阵的方法，称为距离度量学习（Distance Metric Learning），我们将在下一节中介绍。

Bhattacharyya距离定义了两个离散型或连续型概率分布的相似性。对于离散型随机变量的分布，它的定义为：
![](https://pic1.zhimg.com/v2-2d86099680b9e665b8a3c6cafb3a4b48_b.jpg)
其中 ![x_{i},y_{i}](https://www.zhihu.com/equation?tex=x_%7Bi%7D%2Cy_%7Bi%7D) 为两个随机变量取某一值的概率，它们是向量x和y的分量，它们的值必须非负。两个向量越相似，这个距离值越小。

## **距离度量学习**

Mahalanobis距离中的矩阵S可以通过对样本的学习得到，这称为距离度量学习。距离度量学习通过样本集学习到一种线性变换，目前有多种实现。下面我们介绍文献[9]的方法，它使得变换后每个样本的k个最近邻居都和它是同一个类，而不同类型的样本通过一个大的间隔被分开，这和第8章将要介绍的线性判别分析的思想类似。如果原始的样本点为x，变换之后的点为y，在这里要寻找的是如下线性变换：
![](https://pic3.zhimg.com/v2-86ba5b94479d1c37f75b7e0d8244bb66_b.jpg)
其中L为线性变换矩阵。首先定义目标邻居的概念。一个样本的目标邻居是和该样本同类型的样本。我们希望通过学习得到的线性变换让样本最接近的邻居就是它的目标邻居：
![](https://pic4.zhimg.com/v2-7d02d0a39336c764827a33989957e233_b.jpg)
表示训练样本 ![x_{j}](https://www.zhihu.com/equation?tex=x_%7Bj%7D) 是样本 ![x_{i}](https://www.zhihu.com/equation?tex=x_%7Bi%7D) 的目标邻居。这个概念不是对称的，![x_{j}](https://www.zhihu.com/equation?tex=x_%7Bj%7D)是![x_{i}](https://www.zhihu.com/equation?tex=x_%7Bi%7D)的目标邻居不等于

![x_{i}](https://www.zhihu.com/equation?tex=x_%7Bi%7D)是![x_{j}](https://www.zhihu.com/equation?tex=x_%7Bj%7D)的目标邻居。

为了保证kNN算法能准确的分类，任意一个样本的目标邻居样本要比其他类别的样本更接近于该样本。对每个样本，我们可以将目标邻居想象成为这个样本建立起了一个边界，使得和本样本标签值不同的样本无法入侵进来。训练样本集中，侵入这个边界并且和该样本不同标签值的样本称为冒充者（impostors），这里的目标是最小化冒充者的数量。

为了增强kNN分类的泛化性能，要让冒充者离由目标邻居估计出的边界的距离尽可能的远。通过在kNN决策边界周围加上一个大的安全间隔（margin），可以有效的提高算法的鲁棒性。

接下来定义冒充者的概念。对于训练样本![x_{i}](https://www.zhihu.com/equation?tex=x_%7Bi%7D)，其标签值为 ![y_{i}](https://www.zhihu.com/equation?tex=y_%7Bi%7D) ，目标邻居为![x_{j}](https://www.zhihu.com/equation?tex=x_%7Bj%7D)，冒充者是指那些和![x_{i}](https://www.zhihu.com/equation?tex=x_%7Bi%7D)有不同的标签值并且满足如下不等式的样本 ![x_{l}](https://www.zhihu.com/equation?tex=x_%7Bl%7D) ：
![](https://pic4.zhimg.com/v2-e6bae74c86159b64338239e91648fbf7_b.jpg)
其中L为线性变换矩阵，左乘这个矩阵相当于对向量进行线性变换。根据上面的定义，冒充者就是闯入了一个样本的分类间隔区域并且和该样本标签值不同的样本。

训练时优化的损失函数由推损失函数和拉损失函数两部分构成。拉损失函数的作用是让和样本标签相同的样本尽可能与它接近：
![](https://pic2.zhimg.com/v2-24e50b5757f185200760c7f0a39a6891_b.jpg)
推损失函数的作用是把不同类型的样本推开：
![](https://pic1.zhimg.com/v2-0fc23e81b525f1cfaeb57bdbff20b754_b.jpg)
如果 ![y_{i}=y_{i}](https://www.zhihu.com/equation?tex=y_%7Bi%7D%3Dy_%7Bi%7D) ，则 ![y_{ij}=1](https://www.zhihu.com/equation?tex=y_%7Bij%7D%3D1) ，否则![y_{ij}=0](https://www.zhihu.com/equation?tex=y_%7Bij%7D%3D0)。函数 ![[z]_{+}](https://www.zhihu.com/equation?tex=%5Bz%5D_%7B%2B%7D) 定义为：
![](https://pic4.zhimg.com/v2-53cef68ccf7dabce2f04468795dd5a33_b.jpg)
如果两个样本类型相同，则有：
![](https://pic4.zhimg.com/v2-f6e00cbc1d83933e30745fb6d090c23f_b.jpg)
因此推损失函数只对不同类型的样本起作用。总损失函数由这两部分的加权和构成：
![](https://pic2.zhimg.com/v2-a2090db4d4fbf6c2b8b436a76a148df5_b.jpg)
在这里 ![\mu](https://www.zhihu.com/equation?tex=%5Cmu) 是人工设定的参数。求解该最小化问题，就得到了线性变换矩阵。通过这个线性变换，同类样本尽量都成为最近的邻居节点；而不同类型的样本会拉开距离。这会有效的提高kNN算法的分类精度。

## 实验程序

下面用一个例子程序来演示kNN算法的使用，这里我们对2个类进行分类。

![](https://pic2.zhimg.com/v2-13ceb8c41c3137bf4052116cdb07327d_b.jpg)

图6.2  kNN算法的分类效果

在这里分类边界是曲线，证明了kNN算法有非线性分类的能力。以上结果来自[SIGAI](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484784%26idx%3D2%26sn%3Da926e63331c780650e9d714cdd1b4ded%26chksm%3Dfdb698e7cac111f1663d85269cb08240e0fbb8525a23dec65a232df605df3da7f270490ec43b%26scene%3D21%2522%2520%255Cl%2520%2522wechat_redirect)云端实验室，如果你对此感兴趣，可以向[SIGAI](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484784%26idx%3D2%26sn%3Da926e63331c780650e9d714cdd1b4ded%26chksm%3Dfdb698e7cac111f1663d85269cb08240e0fbb8525a23dec65a232df605df3da7f270490ec43b%26scene%3D21%2522%2520%255Cl%2520%2522wechat_redirect)公众号发消息，申请使用。我们的实验室提供了强大的功能，可以帮助大家更容易，深刻的理解各种数学，机器学习，深度学习，以及应用领域的算法。

## 应用

kNN算法简单但却有效，如果能够定义合适的距离度量，它可以取得很好的性能。kNN算法被成功的用于文本分类[5-7]，图像分类[8-11]等模式识别问题。应用kNN算法的关键是构造出合适的特征向量以及确定合适的距离函数。

## 参 考 文 献

[1] Thomas M Cover, Peter E Hart. Nearest neighbor pattern classification. IEEE Transactions on Information Theory, 1967.

[2] James M Keller, Michael R Gray, James Givens. A fuzzy K-nearest neighbor algorithm. systems man and cybernetics, 1985.

[3] Thierry Denoeux. A k-nearest neighbor classification rule based on Dempster-Shafer theory. systems man and cybernetics, 1995

[4] Trevor Hastie, Rolbert Tibshirani. Discriminant adaptive nearest neighbor classification. IEEE Transactions on Pattern Analysis and Machine Intelligence, 1996.

[5] Bruno Trstenjak, Sasa Mikac, Dzenana Donko. KNN with TF-IDF based Framework for Text Categorization. Procedia Engineering, 2014.

[6] J He, Ahhwee Tan, Chew Lim Tan. A Comparative Study on Chinese Text Categorization Methods. pacific rim international conference on artificial intelligence, 2000.

[7] Shengyi Jiang, Guansong Pang, Meiling Wu, Limin Kuang. An improved K-nearest-neighbor algorithm for text categorization. 2012, Expert Systems With Application.

[8] Oren Boiman, Eli Shechtman, Michal Irani. In defense of Nearest-Neighbor based image classification. 2008, computer vision and pattern recognition.

[9] Kilian Q Weinberger, Lawrence K Saul. Distance Metric Learning for Large Margin Nearest Neighbor Classification. 2009, Journal of Machine Learning Research.

[10] S. Belongie, J. Malik, J. Puzicha. Shape matching and obejct recognition using shape contexts. IEEE Transactions on Pattern Analysis and Machine Intelligence, 24(4):509-522, 2002.

[11] P. Y. Simard, Y. LeCun, I. Decker. Efficient pattern recognition using a new transformation distance. In S. Hanson, J. Cowan, and L. Giles, editors, Advances in Neural Information Processing Systems 6, pages 50-58, San Mateo, CA, 1993. Morgan Kaufman.

[12] S. Chopra, R. Hadsell, Y. LeCun. Learning a similarity metric discriminatively, with application to face verification. In Proceedings of the IEEE Conference on Computer Vision and Pattern Recognition (CVPR 2005), pages 349-356, San Diego, CA, 2005.

## 推荐文章

[1]  [机器学习](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483705%26idx%3D1%26sn%3Dc6e7c4a2e14a2469308b41eb60f155ac%26chksm%3Dfdb69caecac115b8712653600e526e99a3f6976fdaa2f6b6a09388fa6f9677ccb57b40c40ae3%26scene%3D21%2522%2520%255Cl%2520%2522wechat_redirect)-波澜壮阔40年 SIGAI 2018.4.13.

[2]  [学好机器学习需要哪些数学知识？](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483713%26idx%3D1%26sn%3D1e7c81381d16806ac73e15691fe17aec%26chksm%3Dfdb69cd6cac115c05f1f90b0407e3f8ae9be8719e454f908074ac0d079885b5c134e2d60fd64%26scene%3D21%2522%2520%255Cl%2520%2522wechat_redirect)SIGAI 2018.4.17.

[3]  [人脸识别算法演化史](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483726%26idx%3D1%26sn%3D9fef4cc1766ea4258749f8d40cc71a6e%26chksm%3Dfdb69cd9cac115cf4eba16081780c3b64c75e1e55a40bf2782783d5c28f00c6f143426e6f0aa%26scene%3D21%2522%2520%255Cl%2520%2522wechat_redirect) SIGAI 2018.4.20.

[4]  [基于深度学习的目标检测算法综述](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483731%26idx%3D1%26sn%3D237c52bc9ddfe65779b73ef8b5507f3c%26chksm%3Dfdb69cc4cac115d2ca505e0deb975960a792a0106a5314ffe3052f8e02a75c9fef458fd3aca2%26scene%3D21%2522%2520%255Cl%2520%2522wechat_redirect) SIGAI 2018.4.24.

[5]  [卷积神经网络为什么能够称霸计算机视觉领域？](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483816%26idx%3D1%26sn%3Dfc52765b012771d4736c9be4109f910e%26chksm%3Dfdb69c3fcac115290020c3dd0d677d987086a031c1bde3429339bb3b5bbc0aa154e76325c225%26scene%3D21%2522%2520%255Cl%2520%2522wechat_redirect)  SIGAI 2018.4.26.

[6] [用一张图理解](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483937%26idx%3D1%26sn%3D84a5acf12e96727b13fd7d456c414c12%26chksm%3Dfdb69fb6cac116a02dc68d948958ee731a4ae2b6c3d81196822b665224d9dab21d0f2fccb329%26scene%3D21%2522%2520%255Cl%2520%2522wechat_redirect)SVM的脉络  SIGAI 2018.4.28.

[7] [人脸检测算法综述](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483950%26idx%3D1%26sn%3Da3a5b7907b2552c233f654a529931776%26chksm%3Dfdb69fb9cac116af5dd237cf987e56d12b0d2e54c5c565aab752f3e366c0c45bfefa76f5ed16%26scene%3D21%2522%2520%255Cl%2520%2522wechat_redirect)  SIGAI 2018.5.3.

[8] [理解神经网络的激活函数](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483977%26idx%3D1%26sn%3D401b211bf72bc70f733d6ac90f7352cc%26chksm%3Dfdb69fdecac116c81aad9e5adae42142d67f50258106f501af07dc651d2c1473c52fad8678c3%26scene%3D21%2522%2520%255Cl%2520%2522wechat_redirect) SIGAI 2018.5.5.

[9] [深度卷积神经网络演化历史及结构改进脉络](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484037%26idx%3D1%26sn%3D13ad0d521b6a3578ff031e14950b41f4%26chksm%3Dfdb69f12cac11604a42ccb37913c56001a11c65a8d1125c4a9aeba1aed570a751cb400d276b6%26scene%3D21%2522%2520%255Cl%2520%2522wechat_redirect)-40页长文全面解读 SIGAI 2018.5.8.

[10] [理解梯度下降法](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484111%26idx%3D1%26sn%3D4ed4480e849298a0aff828611e18f1a8%26chksm%3Dfdb69f58cac1164e844726bd429862eb7b38d22509eb4d1826eb851036460cb7ca5a8de7b9bb%26scene%3D21%2522%2520%255Cl%2520%2522wechat_redirect) SIGAI 2018.5.11.

[11] [循环神经网络综述](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484310%26idx%3D1%26sn%3D0fc55a2784a894100a1ae64d7dbfa23d%26chksm%3Dfdb69e01cac1171758cb021fc8779952e55de41032a66ee5417bd3e826bf703247e243654bd0%26scene%3D21%2522%2520%255Cl%2520%2522wechat_redirect)—语音识别与自然语言处理的利器 SIGAI 2018.5.15

[12] [理解凸优化](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484439%26idx%3D1%26sn%3D4fa8c71ae9cb777d6e97ebd0dd8672e7%26chksm%3Dfdb69980cac110960e08c63061e0719a8dc7945606eeef460404dc2eb21b4f5bdb434fb56f92%26scene%3D21%2522%2520%255Cl%2520%2522wechat_redirect)  SIGAI 2018.5.18

[13][【实验】理解](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484495%26idx%3D1%26sn%3D4f3a6ce21cdd1a048e402ed05c9ead91%26chksm%3Dfdb699d8cac110ce53f4fc5e417e107f839059cb76d3cbf640c6f56620f90f8fb4e7f6ee02f9%26scene%3D21%2522%2520%255Cl%2520%2522wechat_redirect)SVM的核函数和参数 SIGAI 2018.5.22

[14]【SIGAI综述】行人检测算法 SIGAI 2018.5.25

[15] [机器学习在自动驾驶中的应用](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484540%26idx%3D1%26sn%3D733332042c31e1e18ad800f7f527893b%26chksm%3Dfdb699ebcac110fd6607c1c99bc7ebed1594a8d00bc454b63d7f518191bd72274f7e61ca5187%26scene%3D21%2522%2520%255Cl%2520%2522wechat_redirect)—以百度阿波罗平台为例(上) SIGAI 2018.5.29

[16] [理解牛顿法](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484651%26idx%3D1%26sn%3Da0e4ca5edb868fe3eae9101b71dd7103%26chksm%3Dfdb6997ccac1106a61f51fe9f8fd532045cc5d13f6c75c2cbbf1a7c94c58bcdf5f2a6661facd%26scene%3D21%2522%2520%255Cl%2520%2522wechat_redirect) SIGAI 2018.5.31

[17][【群话题精华】](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484658%26idx%3D1%26sn%3Df5c9f92c272c75883bf8e6f532559f11%26chksm%3Dfdb69965cac11073f49048caef5d7b9129614090a363d9ef7f3d1b9bc59948d2217d2bca7b7b%26scene%3D21%2522%2520%255Cl%2520%2522wechat_redirect)5月集锦—机器学习和深度学习中一些值得思考的问题 SIGAI 2018.6.1

[18] [大话](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484692%26idx%3D1%26sn%3D9b389aa65208c778dddf17c601afbee1%26chksm%3Dfdb69883cac1119593934734e94c3b71aa68de67bda8a946c1f9f9e1209c3b6f0bf18fed99b8%26scene%3D21%2522%2520%255Cl%2520%2522wechat_redirect)Adaboost算法 SIGAI 2018.6.2

[ 19]  [FlowNet](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484711%26idx%3D1%26sn%3Dbb7644e101b5924f54d6800b952dc3aa%26chksm%3Dfdb698b0cac111a6605f5b9b6f0478bf21a8527cfad2342dbaaf624b4e9dcc43c0d85ae06deb%26scene%3D21%2522%2520%255Cl%2520%2522wechat_redirect)到FlowNet2.0：基于卷积神经网络的光流预测算法 SIGAI 2018.6.4

[20] [理解主成分分析](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484754%26idx%3D1%26sn%3Db2c0d6798f44e13956bb42373e51d18c%26chksm%3Dfdb698c5cac111d3e3dca24c50aafbfb61e5b05c5df5b603067bb7edec8db049370b73046b24%26scene%3D21%2522%2520%255Cl%2520%2522wechat_redirect)(PCA) SIGAI 2018.6.6

[21] [人体骨骼关键点检测综述](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484784%26idx%3D1%26sn%3Dceafb54203f4e930ae457ad392b9f89c%26chksm%3Dfdb698e7cac111f13d8229d7dcc00b4a7305d66de3da1bd41e7ecc1d29bfa7be520d205c53e9%26scene%3D21%2522%2520%255Cl%2520%2522wechat_redirect)  SIGAI 2018.6.8

[22] [理解决策树](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484827%26idx%3D1%26sn%3D043d7d0159baaddfbf92ed78ee5b1124%26chksm%3Dfdb6980ccac1111a9faeae7f517fee46a1dfab19612f76ccfe5417487b3f090ab8fc702d18b8%26scene%3D21%2522%2520%255Cl%2520%2522wechat_redirect) SIGAI 2018.6.11

[23] [用一句话总结常用的机器学习算法](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484859%26idx%3D1%26sn%3D2c4db22fb538953a62a90983e3e1f99d%26chksm%3Dfdb6982ccac1113a82e92be325bb07a947d54090274654375f3b50e11e1abd809fb7358bde16%26scene%3D21%2522%2520%255Cl%2520%2522wechat_redirect) SIGAI 2018.6.13

[24] [目标检测算法之](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484909%26idx%3D1%26sn%3Dc02ee17e5175230ed39ad63e73249f5c%26chksm%3Dfdb6987acac1116c0108ec28424baf4ea16ca11d2b13f20d4a825d7b2b82fb8765720ebd1063%26scene%3D21%2522%2520%255Cl%2520%2522wechat_redirect)YOLO SIGAI 2018.6.15

[25] [理解过拟合](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484954%26idx%3D1%26sn%3Dc28b7f07c22466e91b1ef90e9dbe3ad1%26chksm%3Dfdb69b8dcac1129bc6e78fca1d550e2b18238ad1c240c73b280d4e529f9f93c4626b3ac45ea2%26scene%3D21%2522%2520%255Cl%2520%2522wechat_redirect) SIGAI 2018.6.18

[26] [理解计算：从](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484981%26idx%3D1%26sn%3Dd3003468b9853851923844812993e060%26chksm%3Dfdb69ba2cac112b4dac620d52100ebd033eb679f29340726a67297c4d6980b16c7cc91122028%26scene%3D21%2522%2520%255Cl%2520%2522wechat_redirect)√2到AlphaGo ——第1季 从√2谈起 SIGAI 2018.6.20

[27] [场景文本检测](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485005%26idx%3D1%26sn%3D0d4fb43b8db2a8046c64a9cfcbf3f478%26chksm%3Dfdb69bdacac112cce05c8b735b4f8b1ccf2348bea55a30af2055fc328958bb8f1ffd0f819bd2%2522%2520%255Cl%2520%2522rd)—CTPN算法介绍 SIGAI 2018.6.22

[28] [卷积神经网络的压缩和加速](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485042%26idx%3D1%26sn%3Dcdcf8d4b07acf64c7a6f5f7c1a731a12%26chksm%3Dfdb69be5cac112f377766984afb87313c1e1c58d94c80005f0f6f6af61ee5a4bd1bf6c6157b6%2522%2520%255Cl%2520%2522rd) SIGAI 2018.6.25

原创声明：本文为 SIGAI 原创文章，仅供个人学习使用，未经允许，不得转载，不能用于商业目的。


