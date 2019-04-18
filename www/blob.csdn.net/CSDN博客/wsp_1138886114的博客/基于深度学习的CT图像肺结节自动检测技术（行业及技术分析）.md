# 基于深度学习的CT图像肺结节自动检测技术（行业及技术分析） - wsp_1138886114的博客 - CSDN博客





2018年08月22日 16:27:20[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：4058









- - - [一、行业现状](#一行业现状)
- [二、项目流程—技术应用：](#二项目流程技术应用)- - [2.1 项目流程](#21-项目流程)
- [2.2 技术应用](#22-技术应用)


- [三、CT图像](#三ct图像)- - [3.1 CT图像的格式 (DICOM)](#31-ct图像的格式-dicom)


- [四、图片分割模型](#四图片分割模型)- - [4.1 关键技术 – 基于CNN的分割网络](#41-关键技术-基于cnn的分割网络)
- [4.2 关键技术：U-net](#42-关键技术u-net)


- [五、三维卷积神经网络](#五三维卷积神经网络)





### 一、行业现状

根据相关研究表明：中国每年医学影像增速达到了30%，而放射科医生的年增长率仅为4.1%, 远远低于影像数据的增长，需求缺口不断加大，医生数量的不足导致工作量繁重，超负荷工作也会导致误诊率和漏诊率提高。 
|$~~~~~~~~~~~~~~~$|传统方式    $~~~~~~~~~~~~~~~$|人工智能  $~~~~~~~~~~~~~~~$|
|----|----|----|
|阅片方式|医生逐张査看，凭借经验进行判断。|机器完成初步筛选、判断.交由医生完成最后判断。|
|阅片时间|医生査看一套PET/CT影像需要10分钟以上，且需要反复观看确认。|人工智能能够快速完成初筛，交由医生进行判断，能够大幅缩短医生阅片时间。|
|准确率|个体差异较大。医生阅片能力的高低严重依赖个人经验。一旦医生长时间阅片会产生疲劳，导致准确率下降。|全面性。一张图片医生会根据经验挑重点可疑区域来现察.而机器可以完整地观察整张切片而无遗漏；   稳定性。机器不需要休息，不会受到疲劳状态影响。其诊断结果能保持完全的客观、稳定和复现。|

![这里写图片描述](https://img-blog.csdn.net/20180819193944981?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180819194034998?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 二、项目流程—技术应用：

![这里写图片描述](https://img-blog.csdn.net/20180819194725536?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 2.1 项目流程
- 数据预处理： 

 将医学影像（DICOM格式）进行转换，变为通用图像格式（PNG），以便用于深度学习模型训练。
- 数据增强(Data augmentation)：使用图像变换算法扩增数据集，提高模型的泛化能力。
- 训练图像分割模型（U-net）： 

 使用图像分割(segmentation)算法检测图像中所有可能是肺结节的区域，生成候选集。
- 训练三维卷积神经网络（3D-CNN）模型： 

 使用一种3D-CNN算法对上一步骤生成的结果进行分类，剔除假阳性的候选，保留真正的结节。
- 模型串联： 

   将两种模型进行串联，完整打通整个肺结节检测的流程，实现对输入的胸部CT图像进行肺结节检测。 

##### 2.2 技术应用

Python图像处理库的使用: `opencv , scikit-image`

Python医学影像处理库的使用: `pydicom , SimpleITK`

Python深度学习框架的使用: `Keras, Tensorflow`
### 三、CT图像

##### 3.1 CT图像的格式 (DICOM)

DICOM（Digital Imaging and Communications in Medicine）即医学数字成像和通信，是医学图像和相关信息的国际标准（ISO 12052）。它定义了质量能满足临床需要的可用于数据交换的医学图像格式。 

其它非标准格式：.mhd，.nii.gz 

常见CT图像的规格是512*512像素，宽度和高度分别是512个像素点 

![这里写图片描述](https://img-blog.csdn.net/2018081919555867?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/2018081920022650?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


### 四、图片分割模型

##### 4.1 关键技术 – 基于CNN的分割网络

![这里写图片描述](https://img-blog.csdn.net/20180819200657647?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 4.2 关键技术：U-net

![这里写图片描述](https://img-blog.csdn.net/20180819202025225?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 五、三维卷积神经网络

![这里写图片描述](https://img-blog.csdn.net/20180819202423337?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180819202908235?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





