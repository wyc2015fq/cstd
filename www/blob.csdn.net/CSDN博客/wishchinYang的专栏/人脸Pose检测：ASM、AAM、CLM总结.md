# 人脸Pose检测：ASM、AAM、CLM总结 - wishchinYang的专栏 - CSDN博客
2016年06月01日 11:55:08[wishchin](https://me.csdn.net/wishchin)阅读数：14689
         人脸的Pose检测可以使用基于位置约束的特征点的方法。人脸特征点定位的目的是在人脸检测的基础上，进一步确定脸部特征点（眼睛、眉毛、鼻子、嘴巴、脸部外轮廓）的位置。定位算法的基本思路是：人脸的纹理特征和各个特征点之间的位置约束结合。经典算法是ASM和AAM。
         一不小心听懂了ASM、AAM、CLM算法，还是记录下来......................
**CLM/AAM/ASM/Snake模型：**
         参考文献：An Introduction to Active Shape Models.  Constrained Local Model for FaceAlignment. Xiaoguang Yan(2011).
         Kass et al [15] introduced Active Contour Models (or ‘snakes’) which are energy minimising curves. In the original formulation the energy has an internal term which aims to impose smoothness on the curve, and an external term which encourages movement
 toward image features. They are particularly useful for locating the outline of general amorphous objects, such as some cells (see Chapter 3, secion 3.1 for the application of a snake to microscope images). However, since no model (other than smoothness) is
 imposed, they are not optimal for locating objects which have a known shape. As the constraints are weak, this can easily converge to incorrect solutions.
        ASM模型起源于snake模型（作为动态边缘分割的snake模型），该方法用一条由n个控制点组成的连续闭合曲线作为snake模型，再用一个能量函数作为匹配度的评价函数，首先将模型设定在目标对象预估位置的周围，再通过不断迭代使能量函数最小化，当内外能量达到平衡时即得到目标对象的边界与特征。
        1989年yuille等人此提出使用参数化的可变形模板来代替snake模型，可变形模板概念的提出为aam的产生奠定了理论基础。
        1995年cootes等人提出的asm算法是aam的直接前身，asm采用参数化的采样形状来构成对象形状模型，并利用pca方法建立描述形状的控制点的运动模型，最后利用一组参数组来控制形状控制点的位置变化从而逼近当前对象的形状，该方法只单纯利用对象的形状，因此准确率不高.
        1998年，cootes等人在asm算法的基础上首先提出aam，与asm的不同之处是他不仅利用了对象的形状信息而且利用了对象的纹理信息。
**Snake模型：**
         参考文章：[图像分割之（五）活动轮廓模型之Snake模型简介](http://blog.csdn.net/zouxy09/article/details/8712287)
自1987年Kass提出Snakes模型以来，各种基于主动轮廓线的图像分割理解和识别方法如雨后春笋般蓬勃发展起来。Snakes模型的基本思想很简单，它以构成一定形状的一些控制点为模板（轮廓线），通过模板自身的弹性形变，与图像局部特征相匹配达到调和，即某种能量函数极小化，完成对图像的分割。再通过对模板的进一步分析而实现图像的理解和识别。
        简单的来讲，SNAKE模型就是一条可变形的参数曲线及相应的能量函数，以最小化能量目标函数为目标，控制参数曲线变形，具有最小能量的闭合曲线就是目标轮廓。
模型的形变受到同时作用在模型上的许多不同的力所控制，每一种力所产生一部分能量，这部分能量表示为活动轮廓模型的能量函数的一个独立的能量项。
         Snake模型首先需要在感兴趣区域的附近给出一条初始曲线，接下来最小化能量泛函，让曲线在图像中发生变形并不断逼近目标轮廓。
        Kass等提出的原始Snakes模型由一组控制点：v(s)=[x(s), y(s)]   s∈[0, 1]组成，这些点首尾以直线相连构成轮廓线。其中x(s)和y(s)分别表示每个控制点在图像中的坐标位置。 s是以傅立叶变换形式描述边界的自变量。在Snakes的控制点上定义能量函数（反映能量与轮廓之间的关系）：
![](https://img-my.csdn.net/uploads/201303/24/1364097621_4887.jpg)
          .................................................
          此外由不同的参数不同的函数值，表示为轮廓能量表达式，通过最小化轮廓能量来控制轮廓的演化，完成图像分割。
          效果：原始的snake模型倾向于生成光滑的边缘，对边缘变化剧烈的图像分割效果不明显....
          其他方法：基于轮廓内外函数差值的图像分割方法
**ASM模型：**
         参考文献：An Introduction to Active Shape Models
         参考文献：[ASM（Active Shape Model）算法介绍](http://blog.csdn.net/carson2005/article/details/8194317)
         算法理论查看论文即可，在此处只列出ASM算法其中的几个关键点，也是当初起疑的地方。
概述：
         ASM是一种基于点分布模型（Point Distribution Model, PDM）的算法。在PDM中，外形相似的物体，例如人脸、人手、心脏、肺部等的几何形状可以通过若干关键特征点（landmarks）的坐标依次串联形成一个形状向量来表示。本文就以人脸为例来介绍该算法的基本原理和方法。首先给出一个标定好68个关键特征点（怎么标的？？）的人脸面部图片，如下所示：
![](https://img-blog.csdn.net/20160601140433325?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
         三维框显示了面部的位姿。ASM在实际应用中，包括了一般的ML训练和搜索两个部分。年级大了，就显得憔悴......................
**ASM模型：训练/学习过程**
        1、建立形状模型
**  搜集N个训练样本**：
                 搜集的图片只要有人脸的面部区域就可以了，暂时不需要考虑尺寸的归一化问题。以便完成人脸区域的ASM训练。
**手动记录每个样本中的k个关键点**：
                  对于训练集中的一个图片而言，你需要手动标记下（68）个关键点的位置坐标信息，并且将它们保存在文本中。最好用程序实现半自动化标定。
**构建训练集的形状向量**：
                 将一幅图片中的K个关键点组成一个形状向量 *X*( *Xi,Y* ), 其中，Xi 为 i 个位置坐标组成的向量，这样所有N个图像便生成N个训练样本，构成了N个形状向量组成的集合S(X |*X*(*Xi,Y* ) ,N )。
**形状归一化**：
                 改步骤的目的在于对前面手动标定的人脸形状进行归一化或者对齐操作，从而消除图片中人脸由于不同角度、距离远近、姿态变换等外界因素造成的非形状干扰，从而使点分布模型更加有效。 一般，使用Procrusts方法进行归一化：把一系列的点分布模型通过适当的平移、旋转和缩放变换，在不改变点分布模型的基础上对齐到一个点分布模型，从而改变原始的 非形状干扰。
                 Procrusts归一化方法一般为使用一个四元数：旋转角度a, 缩放尺度s，水平位移x, 垂直位移y 。可以用一个 4X4 的矩阵来完成转换过程。
                 在ASM模型中，利用Procrusts转换 完成 点分布模型的对齐操作，具体过程为：
                            （1）、将训练集中的所有人脸模型对齐到第一个人脸模型（典型意义，可以自主选择）；
                            （2）、计算平均人脸模型 X0,
                            （3）、将所有人脸模型对齐到平均人脸 X0,
                            （4）、重复（2）（3），直至X0收敛或者到一定时间停止。
**  灰度模型：**
                  方法请参考原始论文
**选择是否PCA：**
                     若考虑到某些特征的影响可以忽略，可以考虑PCA。
          示意图：
![](https://img-blog.csdn.net/20160601140816404?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**模型描述：**
![](https://img-blog.csdn.net/20160601141614813?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                X 为平均脸，b表示一个脸与平均脸的差的参数化，可以描述为图像到参数空间的变化。任意的一个脸可以由平均脸和参数b来进行描述，参数b描述了平均脸到人脸的细微变化。
**ASM模型：搜索/使用过程**
        通过训练样本集得到ASM模型之后即可进行ASM搜索，首先对平均形状进行仿射变换得到一个初始模型：
                X = M( Tp ) + Xc
        用初始模型在新的图像中搜索目标形状，使搜索中的特征点和相对应的特征点最为接近，搜索过程可以通过仿射变换Tp和参数b变化（Xc）完成。
        进行迭代搜索，持续优化Xc，计算最接近模型。
**初始化过程：**
        依据上述描述，目标向量的初始化是由 初始模型在图像中搜索得到，这是个得到近似结果的过程。也可以借由其他的人脸检测算法检测出人脸，再由初始模型按照固定参数起始点进行搜索，得到目标形状向量。
**迭代匹配过程：**
        原始过程描述：
![](https://img-blog.csdn.net/20160601145639032?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
         通过模型由平均脸 y0 寻找 目标向量 Y,  来获得仿射转换参数Tp；
         ---------由 Y 经 Tp的逆 得到 平均脸的接近值 y；
         通过向y 的 切平面 投影得到原始尺度 y1；
         得到模型描述的残差b = y1 - y0 ;再由 b 更新 Y------------
         反复迭代，直到 残差b 小到一定范围...
         结果：得到形状和Pose（x,y,z,旋转）最匹配 平均脸 的结果Y，完成对齐过程，和得到一个收敛的 b来描述结果。
**ASM的搜索过程：**
                持续更新Y：根据灰度模型来更新Y；
![](https://img-blog.csdn.net/20160601154818521?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
               由更新的 Y 获得 y, y1, 更新b 。
**AAM模型：**
参考文献：Active Appearance Models
        ASM是基于统计形状模型的基础上进行的，而AAM则是在ASM的基础上，进一步对**纹理（将人脸图像变形到平均形状而得到的形状无关图像g）进行统计建模**，并将形状和纹理两个统计模型进一步融合为表观模型。
        AAM模型相对于ASM模型的改进为：使用两个统计模型融合 取代 ASM的**灰度模型**。主要对特征点的特征描述子进行了改进，增加了描述子的复杂度和鲁棒性。
**CLM模型：**
         参考链接：[机器学习理论与实战（十六）概率图模型04](http://blog.csdn.net/marvin521/article/details/11489453)[机器学习理论与实战（十六）概率图模型04](http://blog.csdn.net/marvin521/article/details/11489453)
**Patch模型/Local模型：**
         在ASM构建好形状模型之后，我们就可以在检测到的人脸上初始化一个人脸形状模型，接下来的工作就是让每个点在其邻域范围内寻找最佳匹配点。ASM使用灰度来更新Y，是沿着边缘的方向进行块匹配。匹配高错误率导致ASM的性能不是很好。后续各种改进版本大部分做法都是对候选匹配特征点邻域内的块--局部 进行建模，我们统称他们为有约束的局部模型：CLM。
**局部特征提取：**
**建议提取laws特征，比较有普遍适用性**。
![](https://img-blog.csdn.net/20160601165012751?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20160601164813076?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                                  CLM模型                                                                                     特征点（左）其邻域响应图（右）
**优化过程：**
        寻找最小残差b 最后为可以转化为一个二次凸优化问题，作者在论文里有详细的解释。
        ...........................................        
**人脸的Pose检测：**
         人脸的Pose检测需要一个 SolvePNP 的过程，对于固定三维点集模型，找出二维点集对应的位姿。此外，在track时使用点集寻找一个最优的位姿起始，应该给出一个好的起始点。
