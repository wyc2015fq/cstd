# 空间变换网络（Spatial Transform Networks，STN） - wydbyxr的博客 - CSDN博客
2018年11月28日 10:16:33[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：153
# 空间变换网络（Spatial Transform Networks，STN）
  该网络不需要关键点的标定，能够根据分类或者其它任务自适应地将数据进行空间变换和对齐（包括平移、缩放、旋转以及其它几何变换等）。在输入数据在空间差异较大的情况下，这个网络可以加在现有的卷积网络中，提高分类的准确性。
  STN 计算较快，几乎没有增加原有网络模型的训练时间。由于它能够在训练过程中，学习到与任务相关的空间变换参数，因此能够进一步最小化网络的损失函数。STN 不只可以用在输入的图像层，也可以加入卷积层或者其它层之后。
## 总流程
  STN 主要可以分为三个部分：
  1）localisation network.用来计算空间变换的参数θ。
  2) grid generator. 得到input map U∈RH×W×C到 output map 各位置的V∈RH′×W′×C对应关系 Tθ。
  3) sampler. 根据input map U 和 对应关系 Tθ，生成最终的output map。
  流程图如图所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181128101255905.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
## 1）Localisation Network
  它的作用就是通过一个子网络（全连接或者卷积网，再加一个回归层），生成空间变换的参数θ。
  θ 的形式可以多样，如需实现2D仿射变换，θ 就是一个6维（2x3）向量的输出。
## 2）Parameterised Sampling Grid
  假设U (不局限于输入图片，也可以是其它层输出的feature map)每个像素的坐标为(x_si,y_si)， V 的每个像素坐标为(x_ti,y_ti)，空间变换函数 Tθ 为仿射变换函数，那么 (x_si,y_si) 和 (x_ti,y_ti) 的对应关系可以写为：
  (xsi，ysi)=Tθ(Gi)=Aθ(xti，yti)	
  当然，Aθ也可以有其它形式，如3D仿射变换，透射变换等。
## 3）Differentiable Image Sampling
  在计算得到 Tθ 后，就可以由以下公式 U 得到 V 了（省略推导公式若干，只放上最终形式）：
  Vci=∑nH∑mW U_cnm max(0,1−|xsi−m|)max(0,1−|ysi−n|)
