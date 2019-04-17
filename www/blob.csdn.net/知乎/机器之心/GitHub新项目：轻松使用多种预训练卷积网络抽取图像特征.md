# GitHub新项目：轻松使用多种预训练卷积网络抽取图像特征 - 知乎
# 



选自GitHub，机器之心整理。

> 最近 GitHub 有一个非常有意思的项目，它可以使用多种预训练 TensorFLow 模型计算图像特征。对于每一个模型，它们都会输出最后的全连接层，即 AlexNet 的第七个全连接层、VGG_19 的第 8 个全连接层等。这些层级将最终抽取出图像的特征，并能进一步用于图像分类和聚类等。机器之心简要地介绍了该项目，并测试了使用Inception_V1预训练模型抽取图像特征。

项目地址：[https://github.com/cameronfabbri/Compute-Features](https://link.zhihu.com/?target=https%3A//github.com/cameronfabbri/Compute-Features)

这个项目的用法非常简单，我们只需要下载项目中的预训练模型检查点，例如 Inception V1 等。然后再使用如下命令行加载数据与预训练模型就能抽取出图像的特征：


```
tar -xvf inception_v1_2016_08_28.tar.gz
python compute_features.py --data_dir=test_images/ --checkpoint_file=inception_v1.ckpt --model=inception_v1
```


预训练模型的输出将会写入 inception_v1_features.pkl 文件，它包含了一个如下形式的字典：{image_path:feature}。案例 load_features.py 展示了如何使用预计算的特征。例如我们可以使用如下命令行将预抽取的特征用于其它任务：

`python load_features.py features/inception_v1_features.pkl`

有一些模型用上述方法可能并不能加载，例如 inception_v4 有不同的模型检查点和模型定义等。读者可查与具体的模型定义并构建适合于我们自己任务的用法。




**预训练模型**

我们下载所有的预训练模型：[https://drive.google.com/file/d/13pyno-mdbazKs0o4N_Pk8ArDtk1RcE-U/view?usp=sharing](https://link.zhihu.com/?target=https%3A//drive.google.com/file/d/13pyno-mdbazKs0o4N_Pk8ArDtk1RcE-U/view%3Fusp%3Dsharing)

所有模型的独立下载地址在项目中都有展示。这些预训练的卷积神经网络都在 ILSVRC-2012-CLS 图像分类数据集中训练，且它们在 ImageNet 测试集中的 Top-1 和 Top-5 准确度都在下表给出。每个模型都给出了对应的论文和 TF-Sim 写的模型代码，这些模型代码都来自 TensorFLow 中的 models 子项目。

注意 VGG 和 ResNet V1 的参数转换自它们的原始 Caffe 格式，而 Inception 和 ResNet V2 的预训练参数由谷歌内部完成。此外，这些准确度都是在单个图像 crop 下评估的，也有一些学术论文使用多个 crops 在不同规模上有更好的准确度。

在使用预训练模型抽取图像特征的同时，我们还需要了解各个模型的架构与特点，这样才能更有效地利用它们抽取出来的特征向量。如下展示了经典 Inception_V3 模型的一个 Inception 模块，卷积层都是使用 TensorFlow-Slime 实现，因此我们可以在一行中实现一个卷积层的前向传播算法。一般而言，slim.conv2d 有三个参数必填，第一个参数是输入矩阵、第二个是当前卷积层的卷积核数量，最后就是卷积核尺寸。


```
with slim.arg_scope([slim.conv2d, slim.max_pool2d, slim.avg_pool2d],
 stride=1, padding='SAME'):
 # mixed: 35 x 35 x 256.
 end_point = 'Mixed_5b'
 with tf.variable_scope(end_point):
 with tf.variable_scope('Branch_0'):
 branch_0 = slim.conv2d(net, depth(64), [1, 1], scope='Conv2d_0a_1x1')
 with tf.variable_scope('Branch_1'):
 branch_1 = slim.conv2d(net, depth(48), [1, 1], scope='Conv2d_0a_1x1')
 branch_1 = slim.conv2d(branch_1, depth(64), [5, 5],
 scope='Conv2d_0b_5x5')
 with tf.variable_scope('Branch_2'):
 branch_2 = slim.conv2d(net, depth(64), [1, 1], scope='Conv2d_0a_1x1')
 branch_2 = slim.conv2d(branch_2, depth(96), [3, 3],
 scope='Conv2d_0b_3x3')
 branch_2 = slim.conv2d(branch_2, depth(96), [3, 3],
 scope='Conv2d_0c_3x3')
 with tf.variable_scope('Branch_3'):
 branch_3 = slim.avg_pool2d(net, [3, 3], scope='AvgPool_0a_3x3')
 branch_3 = slim.conv2d(branch_3, depth(32), [1, 1],
 scope='Conv2d_0b_1x1')
 net = tf.concat(axis=3, values=[branch_0, branch_1, branch_2, branch_3])
 end_points[end_point] = net
```


以上展示了 Inception_V3 的一个模块，简单而言，该模块就是将四个并行的卷积（branch_0 到 branch_3）分别执行不同的卷积运算，例如 branch_2 是由三个卷积层叠加而成，然后再将 4 份卷积特征图拼接起来作为这一模块的输出。

其它还有很多如特征抽取的效果或模型架构等还需要各位读者自己尝试。

我们测试了 inception_V1 预训练模型，它完成特征计算后会输出一个包含特征的 pkl 文件。我们发现其它如 Inception V3 会报错说权重文件中名为 InceptionV3/AuxLogits/Conv2d_2a_3x3/BatchNorm/beta 的张量没有定义，Inception V4 中会报错 tf.train.Saver() 没有需要保存的变量。
![](https://pic3.zhimg.com/v2-d98761509126f48a39a840f944419e72_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='137'></svg>)
也许这些错误可能是由环境配置或其它问题，但还有很多如特征抽取的效果或模型架构等还需要各位读者自己尝试。
![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)



