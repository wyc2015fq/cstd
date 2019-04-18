# MRI图像处理：VBM原理和步骤 - happyhorizon的算法天空 - CSDN博客
2019年02月23日 19:00:10[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：303
VBM是voxel-based morphometry的缩写， 是对被试之间灰质体素粒度统计分析。VBM可以得到人群中volume和gyrification的不同，对clinical score进行相关性分析，VBM不仅仅可以用于结构数据（T1），还可以用在fmri和PET数据上，分析由于结构变化带来的影响。
VBM在研究中非常常见，除了VBM以外，还有一些类似的方法，例如基于表面的灰质厚度分析方法（surface-based thickness analysis）， 张量分解形态学分析.
VBM分析中，不需要任何的先验知识，是一种全脑的无偏分析方法，并且可以自动运行，运算速度非常快。
说了VBM这么多优点，那么到底VBM怎么做？
## VBM的罪与罚
VBM要做的是不同被试之间的灰质比较，那么首先就需要将这些被试图像从各自所在的个体空间配准到标准空间，使其可以比较。
但是要提到配准，这里似乎有个权衡（trade-off），如果非线性配准不够，可能会没有一致性，导致图像之间不好比较，但是如果非线性配准得太好，这么有丧失了数据间的差异。
但是，如何避免上述这个似乎矛盾的trade-off需求？ 如何找到合适的临界值，使得数据在具备可比性的前提下，尽量保留差异呢？
## 优化方案
Good在2001年的文章中提出了如下的优化方案：
1） 首先，用bet命令去颅骨，fast命令进行成分分割，得到被试的灰质图像（注意，这时并没有做过任何配准）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190223164715413.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=,size_16,color_FFFFFF,t_70)
2）在patients和controls两个组中挑选数量相同的被试作为样本集合，将每个被试的灰质图像依次配准到标准模版上，包括了线性配准和非线性配准。
注意在挑选时一定要从两个组中选择完全相同的被试个数，不要去掉特别显著的（与其他sub显著不同的被试）
线性配准包括了是线性坐标变换和仿射变换，包括了坐标旋转、平移、整体以及局部的缩放。非线性配准是局部的非线性形变，具体来说，就是将一个变形场应用在了原始图像上，对原始图像进行局部任意的变换（基于B样条曲线拟合）。
3）Modulation。这一步是优化VBM的关键。modulation就是说利用非线性配准时候生成的变形场（warp）文件，对配准后的灰质组织的volume进行补偿。在warp文件中，保存了所有体素上volume的压缩/放大的信息，将这些信息（主要是warp数据中的一阶现行jacobian矩阵）应用在配准后的灰质上，在不改变灰质图像标准空间坐标的情况下，改变灰质的volume的intense，这样就既保证了被试数据在空间上的一致性可以比较，同时也保证了个体灰质的差异性。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190223181508412.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=,size_16,color_FFFFFF,t_70)
具体而言，由于warp在进行非线性配准的时候，改变了体素之间的空间位置，从一个特定的slice来看，这个slice上体素之间的距离改变带来了面积的改变，从三维角度就是带来了体积的改变。例如如下的说明图片，图中最初黄色区域部分的面积是1/3，用warp进行非线性变换后，原本没有表情的脸变成了笑脸，同时，黄色的区域部分面积变成了1. 但是我们知道，这部分最初的面积只有1/3而已。如果将这个区域的数据看左灰质，那么这部分实际上的灰质volume只有1/3而已。将这个信息应用在配准后的数据上，就可以还原出原来的体积情况。在右侧的图片中，就表现为“密度”的变化。
4） 对图像进行空间平滑，这样做可以提高信噪比。当然，平滑核的宽度选取非常重要，一般情况下，5-10mm都是不错的选择。当平滑核宽度越大，局部信息就会丢失的越多，结果中某些比较小的统计上显著的区域就会消失。
经过上述的处理后，原始在个体空间中的图像就配准到了标准空间，具备了统计分析所需要的可比性，但是也丢失了原始数据中包含的部分生物信息。面对不同的具体问题，可以参考Ridgway 2008年发表在neuroimage上关于VBM的文章。
对于如何用FSL做VBM的纵向变化研究（longitudinal），可以参考Douaud 2009年发表在Brain上的文章。
纵向研究方法和软件对比，可以看Thomas2009年发表在NeuroImage上的文章。
## study-specific模版步骤
用FSL做VBM的过程，整体来说可以总结为：
数据准备：patient组和control组数据查看，选择。
step1：去颅骨，bet命令
step2：大脑成分分割，segmentation命令
step3：配准到MNI模版的灰质上，flirt+fnirt，保留warp和Jacobian文件。
step4：modulation，将Jacobian文件应用于配准的subject图像上，保存原始图像上灰质的volume信息
step5：空间平滑，选择合适的高斯平滑核宽度进行平滑，提高数据信噪比。
step6：平均，生成study-specific模版。
step7：将所有的个体灰质图像再次配准到study-specific的模版上
Step8 ：非参数统计，对比patient组和control组的差异显著性。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019022318582698.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=,size_16,color_FFFFFF,t_70)
对于上述的处理流程，FSL提供了封装的处理命令：`fslvbm_1_bet`, `fslvbm_2_template`和`fslvbm_3_proc` 和`randomise`, 其中：
fslvbm_1_bet实现上面的step1~2
fslvbm_2_template实现了step3~6
fslvbm_3_proc是step7
randomise则是step8
但是这个过程并不是固定的，不可修改的，在具体的处理过程中，可以根据要处理的数据情况，自己写脚本实现step1～7的任何一步，也可以和fslvbm_*_*的任何一个脚本配合，最后用randomise实现统计分析就可以。
