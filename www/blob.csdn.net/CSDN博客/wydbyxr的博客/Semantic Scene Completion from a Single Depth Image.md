# Semantic Scene Completion from a Single Depth Image - wydbyxr的博客 - CSDN博客
2017年11月23日 20:56:18[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：870
所属专栏：[深度学习--最新论文](https://blog.csdn.net/column/details/23683.html)
Semantic Scene Completion from a Single Depth Image
Princeton University(普林斯顿大学)
1.论文简介
1.1摘要
论文解决的问题是semantic scene completion（语义场景补全），之前的方法是将分为两个问题：1.场景补全（scene completion），也就是预测体积占用，一般是用体素 （voxel）表示重建后三维图像；2.语义标记（semantic labeling），将每个体素标记上语义信息。本论文认为上述的两部分是耦合的，可以视为一个任务，采用了名为SSCNet的end-to-end的三维卷积神经网络解决。所解决的问题如下图。
```
这里介绍本文如何解决semantic scene completion，也是分为两方面：
```
1.我们如何有效地捕捉上下文来自三维体积数据的信息，其中信号是稀疏且缺乏高频细节。
方法：使用3D dilation-based context module，它扩大了感受野。
2.既然目前的RGB-D数据只提供可见表面的注释，我们如何获得完整体积的训练数据？场景级别的注释？
方法：提出SUNCG数据集。它是手动创建的大型数据集，该数据集中的图像是有着dense object labels的合成三维场景。
```
主要思想包括：1.使用了dilation-based 3D context module来扩大感受野。2.使用联合模型（joint model），该模型的结果优于孤立地处理每个任务的方法，是端到端的。
主要贡献包括：1. 提出SSCNet，该网络的输入是单幅的深度图像，输出包括了体素和语义标签。2.提出了一个新的数据集SUNCG，该数据集是带有dense注释的合成三维场景数据集。
另外，3D相关的算法都是建立在相机模型的基础之上，分单目（即单视角）和双目（即多视角，multi-view），而本论文是single-view。本论文可以应用于robot的有关领域。
```
1.2相关研究
	RGB-D数据集：这种数据集只获取“可见表面”的语义标签，不考虑完整的形状。因此不能直接用其进行场景完成，或预测超出可见表面的标签。见上图a）
	形状补全（Shape completion）：过去大多是单个物体的补全，而当丢失的区域很大的时候，它的效果不好。见上图b）
	三维模型拟合（3D model fitting）：这种方法可以完成SSC，实质就是检索并拟合实例级（instance-level）的模型。缺点是模型组太小就不能泛化，太大就产生了检索和匹配的问题，用边缘盒逼近（bounding box approximation）又会导致输出的几何细节不好。举例：在深度图像中找椅子，该方法就是用一个椅子的模型去拟合椅子。
	体素空间推理（Voxel space reasoning）：这种方法可以完成SSC，不过它将特征提取和上下文语义（context）分开处理了。这种方法首先使用预定义的特征，然后将特征学习与context建模。对于long-range contextual，CRF-based（Conditional Random Field）编码的代价太高。而我们用大的感受野能解决这个问题。
2.Semantic scene completion network
训练阶段有两输入：voxel级的标签（voxelize the full 3D scenes）和自制数据集（也是深度图）。
测试阶段有一输入：RGB-D相机拍摄的深度图。
```
如图，processing pipeline有三个阶段：1.对输入的深度图进行三维编码；2.进入网络；3.得到体素空间的概率分布和摄像机视图中的所有体素的类别。下面具体介绍1和2。
```
2.1 Volumetric data encoding
我们使用截断符号距离函数（TSDF ，Truncated Signed Distance Function) ，编码后每个体素存储与其最近的表面的距离值。当然，我们对标准的TSDF进行了改进，提出了更适合我们论文的flipped TSDF。TSDF点云融合算法将当前帧的3D点云融合到现有模型中。
先介绍一下TSDF。它是KinectFusion（kinectfusion是微软研究院利用kinect进行三维重建的项目，kinect是3D成像传感器）中提出的Volumetric Integration（体积积分）的方式。
```
举个例子：
网格模型中值的大小代表网格离重建好的表面的距离，如上图表示的是重建的一个人的脸（红线表示重建的表面，示意图给出的二维信息，实际是三维的），重建好的表面到相机一侧都是正值，另一侧都是负值，网格点离重建好的表面距离越远绝对值越大，在网格模型中从正到负的穿越点表示重建好的场景的表面。虽然可能没有准确的零值，但是可以根据正负值插值求出零值点，所以最后物体表面的分辨率将会超过晶格的分辨率。有一个参数max truncation，它表示选取的截断范围。
我们对其进行改进。分为两个方面：
1.消除视角依赖。代替原来找在摄像机的视线内的最近的表面点，我们这里是找离全观测表面的最近的表面点，如下图（c）。we choose to compute the distance to the closest point anywhere on the full observed surface.
2.消除empty space边缘的强烈的梯度。
如上图，（a）是表面；（b）是投影TSDF，是计算的是相对于相机的距离，因此有视角依赖；（c）视角依赖变少，但在物体内部体素和空体素的边界处中表现出强烈的梯度（灰色圆圈处）；（d）是我们改进的，可以看出只在表面附近才有很强的梯度。
我们改进后，公式为 ，其中d为原始的TSDF值。
```
2.2 Network architecture
2.2.1 输入的体素
假设我们三维空间的尺寸是（4.8m，2.88m，4.8m），设定网格尺寸0.02m，截断值为0.24m，我们会得到一个三维的输入（240*144*240）。
1
2
3
4
5	    vox_unit: 0.02
vox_margin: 0.24
vox_size: 240
vox_size: 144
vox_size: 240
2.2.2 扩大感受野
由于高频信号的缺乏，3D图像的context信息十分重要。我们把2D-dilated卷积核改进为3D-dilated，扩大了感受野的区域。如下图，比较了各种方法的感受野大小（和体素大小）。
带颜色的部分是可感受的区域。
2.2.3 Multi-scale context aggregation
为了聚集不同的尺度上的信息，我们在最后添加了一个concat层，然后再把这个组合特征映射通过两个尺寸为1的卷积层。
实际上，这是扩张卷积带了的好处。concat层收集的map尺寸一致，但由于感受野不同，故代表了不同的尺度信息。
2.2.4 Data balancing
由于三维数据的稀疏性，空体素（empty voxels）与占领体素（occupied voxels）的比例大约是9:1。对此我们进行采样，空体素与占领体素的采样比是2:1。另外我们不选取以下体素：Voxels in free space, outside the field of view, or outside the room are ignored。对应摘要中的图，occupied voxels 表示为observed surface和occluded，empty voxels表示为observed free。
2.2.5 voxel-wise softmax
网络结构的损失函数是  。其中，  是单个体素的Softmax损失， 是真实的标签， 是该体素预测正确的概率， 与上面的2.2.4有关（基于采样算法，为0或者为1）。
- 
Synthesizing training data
目前的RGB-D数据库缺乏体素级的注释，故我们提出了一种新的大规模合成三维场景数据集SUNCG，并且我们的训练数据也来自其中。数据库中提供了合成的深度图像和真实的容积（depth images andvolumetric ground truth）。
使用lanner5d平台（一个在线的室内设计的界面）手工地创造了45622张不同的场景。we have 49 884 valid floors, with contain 404 058 rooms，and 5 697 217 object instances from 2644 unique object meshes covering 84 categories.
如果满足以下要求，则视图被认为是有效的。共有三个标准：1.有效深度区域（深度范围是1米到8米）占图像区域的70%以上；2.除了墙壁、天花板和地板之外，还有两个以上的物体类别；3. 除了墙壁，天花板，地板外的物体面积大于图像区域的30%以上。另外，为了减少数据冗余，从每个房间最多挑选五幅图像。
打标签时，设定体素为2cm，我们把物体的包围盒（bounding box）转换到场景体素网格的所有每个体素中去，然后计算到最近的物体体素的距离，如果距离小于对象体素2cm，这个场景的体素将被标记为这个对象类别。
- 
Evaluation
4.1 所使用的数据集
训练阶段，我们有两个训练集可选择。一个是真实的图像集NYU，另一个是合成的图像集SUNCG。
测试阶段，我们也有两个test set可选择。一个是NYU test set，另一个是The SUNCG test set（consists of 500 depth images rendered from 184 scenes that are not in the training set）。
4.2 评价指标
我们使用三个指标：体素级的IoU、recall以及precision。
对于语义任务，每个物体类别的 遮挡的体素和可见的体素之间的IoU；
For the semantic scene completion task, we evaluate the IoU of each object classes on both the observed and occluded voxels.
对于场景补全任务，我们把非空物体作为一个类别，IoU只是计算遮挡体素和非遮挡体素之间的比例。
For the scene completion task, we treat all non-empty object class as one category and evaluate IoU of the binary predictions on occluded voxels.
以下是实验的结果：
```
此外，由于我们的方法不需要模型拟合步骤，所以每张图像比2015的一篇论文 要快上7s（我们只需要120s）。
下面是主观效果：
```
4.3 各种论证
这里不细说，见论文。
	Does scene completion help in recognizing objects? 有
	Does synthetic data help?  有
	Does multi-scale aggregation help?  有
	Do different encodings matter（TSDF）? 有
	Is data balancing necessary? 必须
4.4 缺点Limitations.
第一，诸如“Windows”之类缺少深度的对象很难处理。这也会导致几何上类似对象之间的混淆。
第二，由于GPU内存约束，我们的网络输出分辨率低于输入体素。这会导致几何细节的丢失和小目标的丢失。
4.5 实际训练
batch_size为1还是会内存溢出，故改了corp_size。在4000多次迭代后，完全看不到收敛的迹象。
1
2
3
4
5
6
7
8
9
I0727 15:36:20.038151 57976 sgd_solver.cpp:106] Iteration 4611, lr = 0.01
I0727 15:36:22.555456 57976 solver.cpp:228] Iteration 4612, loss = 0.953568
I0727 15:36:22.555518 57976 solver.cpp:244]     Train net output #0: loss = 1.00309 (* 1 = 1.00309 loss)
I0727 15:36:23.082343 57976 sgd_solver.cpp:106] Iteration 4612, lr = 0.01
I0727 15:36:25.603811 57976 solver.cpp:228] Iteration 4613, loss = 0.627275
I0727 15:36:25.603873 57976 solver.cpp:244]     Train net output #0: loss = 0.941985 (* 1 = 0.941985 loss)
I0727 15:36:26.334817 57976 sgd_solver.cpp:106] Iteration 4613, lr = 0.01
I0727 15:36:29.312858 57976 solver.cpp:228] Iteration 4614, loss = 1.19364
I0727 15:36:29.312980 57976 solver.cpp:244]     Train net output #0: loss = 1.82576 (* 1 = 1.82576 loss)
附录
关于三维卷积
3D卷积源自视频处理，目前找到最早的是2013的一篇关于行为理解的论文 。通过在CNNs的卷积层进行3D卷积，以捕捉在时间和空间维度都具有区分性的特征（本论文中是捕捉3D空间的特征）。
例如下图，某一个输出位置的值是通过卷积上一层的三个连续的map的同一个位置的局部感受野得到的。
```
3D卷积可以理解为为想领的3幅图用3个不同卷积核进行卷积，并把卷积结果相加。可以这么说，通过这种3幅图之间的卷积，网络提取了时间之间某种的相关性。
在keras中，有3Dconv。当使用该层作为第一层时，需要提供input_shape参数。例如input_shape = (3,10,128,128)代表对10帧128*128的彩色RGB图像进行卷积。输入应为形如（samples，channels，input_dim1，input_dim2, input_dim3）的5D张量。
在我们的网络模型的具体代码中，我们可以看出，padding、kernel_size、stride以及dilation都变成三维的了，其中，任何不为1的dilation均与任何不为1的strides均不兼容。
扩张3D卷积		普通3D卷积
```
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26	layer {
name: “conv3_8”
type: “Convolution”
bottom: “conv3_7”
top: “conv3_8”
convolution_param {
num_output: 64
pad: 2
pad: 2
pad: 2
kernel_size: 3
kernel_size: 3
kernel_size: 3
weight_filler {
type: “xavier”
}
bias_filler {
type: “constant”
}
engine: CAFFE
axis: 1
dilation: 2
dilation: 2
dilation: 2
}
}	1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24	layer {
name: “conv1_1”
type: “Convolution”
bottom: “data”
top: “conv1_1”
convolution_param {
num_output: 16
pad: 3
pad: 3
pad: 3
kernel_size: 7
kernel_size: 7
kernel_size: 7
stride: 2
weight_filler {
type: “xavier”
}
bias_filler {
type: “constant”
}
engine: CUDNN
axis: 1
}
}
```
另外，有两个conv不是3D卷积，即支路是的两个。它们的pad为0，kernel_size为1，stride为1。
本论文中的pooling不是3D的，而是2D的。其类型是MAX，kernel_size为2，stride为2。当然，存在3D-pooling，其输入和输出是形如（samples, channels, len_pool_dim1, len_pool_dim2, len_pool_dim3）的5D张量。
```
其他
网络模型为3.6M，速度10s左右。主要是input很大（240*144*240）
参考文献
	Song S, Yu F, Zeng A, et al. Semantic Scene Completion from a Single Depth Image[J]. 2016.
	朱笑笑, 曹其新, 杨扬,等. 一种改进的KinectFusion三维重构算法[J]. 机器人, 2014, 36(2):129-136.
	[http://blog.csdn.net/xiaohu50/article/details/51592503](http://blog.csdn.net/xiaohu50/article/details/51592503)
	[http://blog.csdn.net/fuxingyin/article/details/51417822](http://blog.csdn.net/fuxingyin/article/details/51417822)
	[http://blog.csdn.net/baidu_17313961/article/details/52862416](http://blog.csdn.net/baidu_17313961/article/details/52862416)
