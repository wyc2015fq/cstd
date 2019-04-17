# 医学影像比赛及资源 summary - 别说话写代码的博客 - CSDN博客





2018年10月19日 20:10:57[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：789








https://luna16.grand-challenge.org/download/  -> downloaded on centos-purley-no101-1



前列腺 （prostate）

Dataset :

https://promise12.grand-challenge.org/details/ -> downloaded on self notebook

MRI format

Code: VNet

tools:

    Evaluation of prostate segmentation algorithms for MRI The PROMISE12.pdf

    Computer-aided diagnosis of prostate cancer with MRI.pdf

    A survey of prostate segmentation methodologies.pdf

脑部相关的

Deep Learning for Brain MRI Segmentation: State of the Art and Future Directions笔记

https://blog.csdn.net/sunyao_123/article/details/73927653

为了评定一个新的开发的深度学习方法的效果，有必要将它与现在最好的方法作比较。这里提到很多数据集，[https://www.nitrc.org/projects/msseg](https://www.nitrc.org/projects/msseg)，brain MRI are Brain Tumor Segmentation (BRATS), Ischemic Stroke Lesion Segmentation(ISLES), Mild Traumatic Brain Injury Outcome Prediction(mTOP), Multiple Sclerosis Segmentation (MSSEG), Neonatal Brain Segmentation (NeoBrainS12), and MR Brain Image Segmentation (MRBrainS)。

Brats 这个脑肿瘤图像分割挑战联合MICCAI会议，自从2012年开始每年举办，为了评估现在最好的脑部肿瘤分割方法，并且比较不同方法。为此，很多的数据集被公开，有5类label：脑部健康组织，坏死区，水肿区，肿瘤的加强和非加强区。并且训练集每年都在增长。最近的Brats 2015–2016比赛中训练集包含220个高等级子集和54个低等级子集，测试集包含53个混合子集。所有的数据集被校准为同样的解剖模板，并且被插值为1 mm 3的分辨率。每个数据集包含增强前T1和增强后T1，T2，T2磁共振成像液体衰减反转恢复序列MRI体素。联合配准，头骨分离，标注的训练集，算法的评价结果可以通过Virtual Skeleton Database ([https://www.virtualskeleton.ch/](https://www.virtualskeleton.ch/))来获取.

Isles 这个挑战被组织来评估，在精确MRI扫描图像中，中风病变及临床结果预测。提供了包含大量的精确中风样例和相关临床参数的MRI扫描。联合的被标记的真实数据是最终损伤的区域（任务一），用了3到9个月的跟踪扫描来人工标记，和表示残疾度的临床mRM得分（任务二）。在ISLES2016比赛中，35个训练集和40个测试集通过SMIR平台公开。([https://www.smir.ch/ISLES/Start2016](https://www.smir.ch/ISLES/Start2016)). 亚急性缺血性卒中病变分割的获胜者的算法结果为0. 59±0.31（骰子相似性系数，DSC）和37.88±30.06（豪斯多夫距离，HD）。

mTOP 这个挑战要求算法找到健康组织和外伤性脑损伤（TBI）病人的差异，并且使用非监督方法将给定的数据分为明显不同的类。开源MRI数据在[https://tbichallenge.wordpress.com/data](https://tbichallenge.wordpress.com/data)下载。

MSSEG 这个挑战的目的是从MS数据的参赛者中评定最好的最新的分割方法，为此他们评估了在一个在多中心临床数据库（4个数据中心的38个病人，为1.5T或者3T的图像，每个病人被7个专家手动标记）上的损伤区域检测（多少个病变区被检测出）和分割（被勾出的损伤区精确度如何）。除了这个经典的评估外，他们提供了一个共同的基础设施来评价算法，比如运行时间和自动化度的比较。数据可以从[https://portal.fli-iam.irisa.fr/msseg-challenge/data](https://portal.fli-iam.irisa.fr/msseg-challenge/data)下载。

NeoBrainS12 这个比赛的的目的是，通过使用脑部T1和T2的MRI图像，来比较新生脑组织分割算法和对应大小的测量。在以下结构比较：皮质和中央灰质，无髓有髓白质，脑干和小脑，脑室和脑外间隙脑脊液。训练数据包括两个30周到40周大小的婴儿的T1和T2MR图像。测试集包括5个婴儿的T1和T2 MRI图像。数据和算法的评估结果已经被提交，可以从[http://neobrains12.isi.uu.nl/](http://neobrains12.isi.uu.nl/)下载。

MRBrainS 这个评估架构的目的是比较脑部多序列（T1加权，T1加权反转恢复，磁共振成像液体衰减反转恢复序列，FLAIR）3T MRI图像，灰质，白质，脑脊髓液的分割算法。训练集包括5个手动分割的脑部MRI图像，测试集包括15份MRI图像。数据可以从[http://mrbrains13.isi.uu.nl](http://mrbrains13.isi.uu.nl/)下载。在这个数据集上的获胜者的算法的结果（骰子相似系数，DSC）：灰质86.15%，白质89.46%，脑脊髓液84.25%。



Brats2017, 面向GLIOMAS

Dataset https://www.med.upenn.edu/sbia/brats2017/data.html

Nii format

preprocessing: https://github.com/ANTsX/ANTs

Tools:https://www.med.upenn.edu/cbica/captk/

code:

    Unet: [https://ai.intel.com/biomedical-image-segmentation-u-net](https://ai.intel.com/biomedical-image-segmentation-u-net/)[/](https://ai.intel.com/biomedical-image-segmentation-u-net/)

    3D-Unet: [https](https://github.com/ellisdg/3DUnetCNN)[://](https://github.com/ellisdg/3DUnetCNN)[github.com/ellisdg/3DUnetCNN](https://github.com/ellisdg/3DUnetCNN)

Paper:

    Brain Tumor Segmentation with Deep Neural Networks_1505.03540.pdf

    MICCAI_BraTS_2017_proceedings_shortPapers.pdf  --> all algorithm summary

    The Multimodal Brain Tumor Image Segmentation Benchmark.pdf --> good introducing about Brats dataset

    Advancing The Cancer Genome Atlas glioma MRI collections with expert segmentation labels and radiomic features.pdf

T1, T1c, T2, FLAIR ???



https://camelyon17.grand-challenge.org/

淋巴癌, 乳腺癌

Dataset:

Code:

Tools:



乳腺癌

http://www.bioimaging2015.ineb.up.pt/dataset.html

Dataset:

Code:

Tools:

Paper:

    Classification of breast cancer histology using CNN.pdf





宫颈癌TCT

Dataset:

Code:

Tools:

Paper:







肺结节

https://www.jianshu.com/p/9c1facf70b01

Dataset:

[LIDC-IDRI](https://link.jianshu.com/?t=https%3A%2F%2Fwiki.cancerimagingarchive.net%2Fdisplay%2FPublic%2FLIDC-IDRI)

Code:

Tools:

Paper:



数据预处理

CT

MRI

脑部相关

http://www.360doc.com/content/18/0119/22/42715024_723486417.shtml

The image datasets used in the study all share the following four MRI contrasts (Fig. 2).

1) T1: T1-weighted, native image, sagittal or axial 2D acquisitions, with 1–6 mm slice thickness.

2) T1c: T1-weighted, contrast-enhanced (Gadolinium)

image, with 3D acquisition and 1 mm isotropic voxel size

for most patients.

3) T2: T2-weighted image, axial 2D acquisition, with 2–6 mm

slice thickness.

4) FLAIR: T2-weighted FLAIR image, axial, coronal, or

sagittal 2D acquisitions, 2–6 mm slice thickness.



病理切片？？

      通常可以得到各种scale的图片，一般医生使用20x-40x的分辨率做图像分析







https://blog.csdn.net/zxllll8898/article/details/76040426

https://blog.csdn.net/u013635029/article/details/72957944

https://blog.csdn.net/a8039974/article/details/77964764




作者：lail3344

链接：https://www.jianshu.com/p/9778658da808

來源：简书

简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。



