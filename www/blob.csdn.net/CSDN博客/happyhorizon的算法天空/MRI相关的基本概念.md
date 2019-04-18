# MRI相关的基本概念 - happyhorizon的算法天空 - CSDN博客
2019年01月26日 17:43:00[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：164
# 磁共振基础
## 磁共振
磁共振(mageticresonanceMR)；在恒定磁场中的核子（氢质子），在相应的射频脉冲激发后，其电磁能量的吸收和释放，称为磁共振。
## 基本参数: TR, TE
TR(repetitiontime)：又称重复时间。MRI的信号很弱，为提高MR的信噪比，要求重复使用同一种脉冲序列，这个重复激发的间隔时间即称TR。
TE(echedelaytime)：又称回波时间，即射频脉冲放射后到采集回波信号之间的时间。
## 序列(sequence)
指检查中使用的脉冲程序-组合。常用的有自旋回波(SE)，快速自旋回波(FSE)，梯度回波(GE)，翻转恢复序列IR)，平面回波序列(EP)。
## 加权像(weightimage．WI)
为了评判被检测组织的各种参数，通过调节重复时间TR。回波时间TE，可以得到突出某种组织特征参数的图像，此图像称为加权像。
# 空间及坐标系
## K 空间( K space)
K空间的数据分布实际上是图像空间中数据的二维傅立叶变换结果。也就是说，我们的“大脑图像”可以被看作是由一系列频率、相位、方向各异的二维正弦波叠加而成的，而K空间的数据正表示了图像的正弦波组成。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190126110708859.png)
对K空间的数据做二维傅立叶变换, 才能将K空间中的数据变成可读的图像空间中的数据。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190126110805811.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=,size_16,color_FFFFFF,t_70)
## Native Space
Native space就是原始空间，在这个空间中图像的维度、原点、voxel size等都是不同的，不同被试的图像之间不具有可比性，计算出来的任何特征都不能进行统计分析/机器学习。
![native space VS standard space](https://img-blog.csdnimg.cn/20190126110845743.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=,size_16,color_FFFFFF,t_70)
## MNI空间
MNI空间是Montreal Neurological Institute根据一系列正常人脑的磁共振图像而建立的坐标系统。
不同被试的图像在原始空间时是不具备可比性的, 必须对所有被试的图像进行配准标准化到同一个模板上，这样所有被试的维度、原点、voxel size就一样了。使用MNI标准模板，就表示把图像转换至MNI空间了。一般而言MNI模板是最常用的，研究的比较多。标准空间的图像也是指MNI空间的图像。
标准空间中，X代表左/右；Y代表前/后；Z代表上/下。在数据矩阵中，一个特定的体素可以被标记为[Xvox, Yvox, Zvox]，通过这三个维度的坐标就可以确定体素的位置。
![MRI标准空间坐标](https://img-blog.csdnimg.cn/20190126110410915.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=,size_16,color_FFFFFF,t_70)
## Talairach空间
Talairach空间和MNI空间的坐标有对应的关系，很多软件都提供这个功能，比如Mricron、REST等。
Talairach空间主要是为了判别当前坐标在什么结构上，Talairach atlas and Talairach coordinates就是Stereotaxic space。
# MRI数据
## 数据格式
### DICOM格式
DICOM（Digital Imaging and Communications in Medicine）即医学数字成像和通信，是医学图像和相关信息的国际标准（ISO 12052）。它定义了质量能满足临床需要的可用于数据交换的医学图像格式。
### Analyze格式
Analyze格式储存的每组数据组包含2个文件，一个为数据文件，其扩展名为.img，包含二进制的图像资料；另外一个为头文件，扩展名为.hdr，包含图像的元数据。
在fMRI的早期，Analyze格式最常用的格式，但现在逐渐被NIfTI格式所取代。Analyze格式主要不足就是头文件不能真正反映元数据。
### NIfTi 格式
标准NIfTI图像的扩展名是.nii，包含了头文件及图像资料。
### 格式转换
FSL转换数据格式命令为:`fslchfiletype`
```
fslchfiletype
Usage: /home/dl/Software/FSL/bin/fslchfiletype <filetype> <filename> [filename2]
 
  Changes the file type of the image file, or copies to new file
  Valid values of filetype are ANALYZE, NIFTI, NIFTI_PAIR,
                                 ANALYZE_GZ, NIFTI_GZ, NIFTI_PAIR_GZ
```
## 数据类型
### 结构MRI
T1，是所谓的纵向弛豫时间，就是说你把质子磁化弄到z轴负向后，他要花多少时间才能回到初始位置Z轴正向。 T2,是横向弛豫时间，就是说在横向平面产生一个磁化后，他会在多少时间内衰减到零.
总结一下就是:
T1观察解剖结构较好。
T2显示组织病变较好。
### 功能MRI
fMRI往往用于研究大脑的具体功能，扫出来的是BOLD信号。虽然它的空间分辨率比较低，但是时间分辨率很高.
有氧血红蛋白是抗磁性（Diamagnetic）的，脱氧血红蛋白是顺磁性（Paramagnetic）的。fMRI（T2*权重）正是利用了血红蛋白在这两种状态下不同的磁性性质，顺磁性的脱氧血红蛋白可以增强MR的原磁场。当它的含量下降时，BOLD fMRI信号会跟着上升。脱氧血红蛋白上升，会导致信号的下降，因为它会干扰主磁场，导致信号的衰减加速。
参考:
[https://blog.csdn.net/ssw_1990/article/details/51996011](https://blog.csdn.net/ssw_1990/article/details/51996011)
K Space: [http://mriquestions.com/locations-in-k-space.html](http://mriquestions.com/locations-in-k-space.html)
关于T1 T2的区别: [http://blog.sina.com.cn/s/blog_45a7fee80101h6w9.html](http://blog.sina.com.cn/s/blog_45a7fee80101h6w9.html)
