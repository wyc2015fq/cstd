# Look into Person: Self-supervised Structure-sensitive Learning and A New Benchmark for Human Parsing - wxf19940618的博客 - CSDN博客





2018年11月09日 14:50:28[rxdffz](https://me.csdn.net/wxf19940618)阅读数：25








**Look into Person: Self-supervised Structure-sensitive Learning and A New Benchmark for Human Parsing ****论文笔记**

一、提出问题
- 现存的数据集只有有限的图像和标注，缺乏人类外观的多变性；
- 现有的人体解析方法的一个主要缺陷是缺乏对人体姿态的考虑，虽然已经有一些方法将姿态估计引入人体解析问题中，但是姿态估计标注的人体关节与人体解析中的人体关节是不一致的（例如，人体解析中只有在没有任何衣服覆盖的情况下，手臂才被标记为ARM，而姿态标注则与衣服无关），所以使用姿态估计的关节标注去指导人体解析问题是不够准确的。

二、论文贡献
- 提出了一种新的大规模基准测试和评估协议来推进人体解析研究，在19个语义部件标签上提供了50462幅具有像素级标注的图像；
- 通过在基准测试上的实验，对现有的人工解析方法进行了详细的分析，以了解这些方法的优势与不足；
- 提出了一种新的自监督结构敏感学习框架，该框架能够显式地增强解析结果与人的关节结构之间的一致性。在现有的pascal-Person部分数据集上和新的LIP数据集上，本文提出的框架明显优于以前的方法。

三、LIP数据集

![](https://img-blog.csdnimg.cn/20181109144920190.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

图 3-1 LIP数据集实例展示

LIP数据集包括50462张图像，20类别（19个语义人类部分标签和一个背景标签），是目前为止最大的最复杂的人体解析数据集。LIP数据集只关注人体解析问题，包括不同视角、不同背景、各种遮挡、各种外观、各种分辨率的图像数据。

![](https://img-blog.csdnimg.cn/20181109144920114.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

图3-2 LIP数据集与其他同类数据集的对比

标签：LIP数据集中的人体图像是从microsoft coco训练集和验证集中裁剪的。定义了19个人体部件或衣服标签，它们是帽子、头发、太阳镜、上衣、衣服、外套、袜子、裤子、手套、围巾、裙子、连体裤、脸、右臂、左臂、右腿、左腿、右脚、右脚鞋、左鞋，以及背景标签。

数据集分割：数据集中共有50462张图像，其中包括19081张全身图像、13672张上身图像、403张下身图像、3386张头部丢失的图像、2778张后视图图像和21028张有遮挡的图像。经过随机抽取，得到30462个训练图像、10000个验证图像，以及10000个测试图像。

![](https://img-blog.csdnimg.cn/20181109144920133.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

图3-3 19类标签的数量

![](https://img-blog.csdnimg.cn/20181109144920190.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

图 3-4 具有不同遮挡情况的图像数量

四、不同方法在LIP数据集上的表现

![](https://img-blog.csdnimg.cn/20181109144920918.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

图4-1 各种人体解析算法在LIP数据集上的表现

进一步分析了每种方法在以下五个挑战性因素方面的性能：遮挡、全身、上身、头部丢失和后视野(back-view)。在LIP数据集上对不同人体解析算法进行评估，包含4277张包含遮挡的图像，452张全身图像，793张上身图像，112张头部丢失图像和661张背面图像。由图4-2可见在受到不同因素的影响时，性能会有所不同后视野显然是最具挑战性的情况。

![](https://img-blog.csdnimg.cn/20181109144920928.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

图4-2 四种人体解析算法在不同遮挡情况下的表现

由图4-2可以得出结论：Head(或Face)是现有人体解析方法的重要线索。如果头部部分在图像或在back-view中消失，则出现模糊结果的概率会增加。此外，由于存在小尺度部件的存在（如，鞋子，袜子），导致下半身部件解析要比上半生更难。在这种情况下，人体关节结构在人体解析中会起到更重要的作用。

![](https://img-blog.csdnimg.cn/20181109144920920.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

图4-3 各种方法在20类标签下的IOU

如图4-3所示，对于较大区域的标签，如脸、上装、外套和裤子，其效果要比太阳镜、围巾和裙子等小区域标签的效果要好得多。Attention和DeepLabV2, 由于使用了多尺度特性，小标签的性能更好。

五、模型结构

![](https://img-blog.csdnimg.cn/20181109144920954.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

图5-1 SSL框架结构

思想：用各个人体部位的关节点指导人体解析，人体部位关节点是标注的人体区域的中心点，将人体解析的loss和人体关节的loss相结合得到最后的loss。这个思想不引入额外的信息，充分利用人体部位的标注得到关节信息，进而反过来指导人体解析。

给定一个图像I，定义了一个关节结构列表![](https://img-blog.csdnimg.cn/20181109144920770.png)，其中![](https://img-blog.csdnimg.cn/2018110914492126.png)是第i个关节热度图，是根据解析结果计算的得到。同样，对于真实标注图像，关节结构列表为![](https://img-blog.csdnimg.cn/20181109144921187.png)。N是由人体输入图像决定的变量，对于一个完整的身体图像则等于9。对于图像中漏掉的关节，只需将热图置0。关节结构损失函数用L2表示如下：

![](https://img-blog.csdnimg.cn/20181109144921193.png)

最终的结构敏感损失函数即为![](https://img-blog.csdnimg.cn/20181109144921190.png)是关节损失函数和解析分割损失函数的组合，具体如下：

![](https://img-blog.csdnimg.cn/20181109144921788.png)

其中，![](https://img-blog.csdnimg.cn/20181109144921832.png)是根据解析结果计算的逐像素的softmax损失函数。

本文的自我监督学习框架具有良好的适应性和可扩展性，可以被应用到任何网络中，从而有助于从全局的角度整合有关人体关节的丰富高层信息。

六、实验结果

![](https://img-blog.csdnimg.cn/20181109144921366.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181109144921387.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181109144921451.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)



