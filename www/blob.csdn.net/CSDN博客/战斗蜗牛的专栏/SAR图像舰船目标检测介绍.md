# SAR图像舰船目标检测介绍 - 战斗蜗牛的专栏 - CSDN博客





2019年03月02日 17:54:47[vbskj](https://me.csdn.net/vbskj)阅读数：981标签：[深度学习																[目标检测](https://so.csdn.net/so/search/s.do?q=目标检测&t=blog)](https://so.csdn.net/so/search/s.do?q=深度学习&t=blog)
个人分类：[Deep Learning继续学习](https://blog.csdn.net/vbskj/article/category/6337222)








作者：SIGAI人工智能平台，出处：SIGAI人工智能平台，公众号：SIGAI

-----------------------------------------



1.SAR舰船目标检测概述

合成孔径雷达(Synthetic Aperture Radar, SAR)具有全天候、全天时、不受天气影响等成像特点，目前已经成为人们对地观测的重要手段之一[1]。因此，利用SAR数据进行目标检测也是图像解译的重要研究方向之一。通过机载和星载SAR，我们能够获得大量的高分辨率SAR海洋图像，舰船目标和舰船的航迹也在这些图像中清晰可见[2]。从SAR图像中检测舰船目标有着广泛的应用前景，在军事领域，对特定目标进行位置检测，有利于战术部署，提高海防预警能力；在民用邻域，对某些偷渡、非法捕鱼船只进行检测，有助于海运的监测与管理。



传统的舰船目标检测所利用的数据是单通道SAR数据，这类数据只包含舰船的强度信息，因此不能全面地反映船只目标与海面杂波间的散射信息差异。相比之下， 多极化SAR(Polarimetric SAR)数据包含了更多的目标散射信息，因而能够更加有效地反映舰船目标与海面杂波间的散射信息差异，所以大多数研究人员已经将研究的热点转向了基于PolSAR图像的舰船目标检测任务中来。



从散射机制的角度来说，海面散射主要以表面散射(也称单次散射)为主，而舰船目标拥有与海面不同的散射机制。复杂的船体结构使得舰船目标可被认为是一个由多种不同“角散射”器构成的集合，因此除单次散射之外，舰船目标还包含二次散射、体散射、螺旋体散射等[3]。图(1)给出了常见船只的散射成分示意图[4]，这些不同的散射机制构成了SAR图像舰船目标检测的基础。总的来说，目前常见的主流SAR舰船目标检测方法可分为以下三大类:
- 
基于背景杂波统计分布的舰船目标检测

- 
基于极化分解的舰船目标检测

- 
基于极化特征的舰船目标检测


![](https://img-blog.csdnimg.cn/20190302174614828.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly96aGFuZ2Zhbi5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

图1.典型的舰船目标散射机制



2. 目标的散射极化表示

2×2的 Sinclair散射矩阵[S]是目前描述目标散射机制的常用矩阵。常见的由线性极化基(H/V)派生出的[S]矩阵被定义为[5]:

![](https://img-blog.csdnimg.cn/20190302174633518.png)

这里SHV代表电磁波的垂直发射，水平接收，[S]矩阵中的其它三个参数也类似定义。因为[S]中的元素是复数元素，所以[S]矩阵还可以被表示为：



![](https://img-blog.csdnimg.cn/2019030217464630.png)

这里，|·|和*Φ*(·)分别代表元素的幅度和相位。如果后向散射满足互易性定理，那么发射和接收天线可以互换，此时的[S]矩阵是对称矩阵，也即SHV=SVH。



事实上，[S]矩阵通常用来描述单一的散射目标，而自然界中的目标通常是以分布式目标存在的[5]。鉴于这个事实，研究人员进一步选用3×3的协方差矩阵[C]或者3×3的相干矩阵[T]来描述这类目标。其中，[C]矩阵被定义为:



![](https://img-blog.csdnimg.cn/20190302174704167.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly96aGFuZ2Zhbi5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

这里，(·)H是共轭转置，<·>代表平均，‘*’代表共轭，k是3维字典式向量，即:

![](https://img-blog.csdnimg.cn/20190302174746916.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly96aGFuZ2Zhbi5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)



3.舰船目标检测方法介绍

3.1. 基于背景杂波统计分布的舰船目标检测

恒虚警率检测算法(Constant False Alarm Rate, CFAR)是目前应用最为广泛的舰船目标检测算法之一,它通过对背景杂波进行统计分布建模来检测舰船目标[6]。所以， CFAR检测器的关键步骤在于对背景杂波的统计建模，一旦概率密度函数fpdf(x)被确定,那么由阈值T产生的虚警率为[6]:

![](https://img-blog.csdnimg.cn/20190302174822547.png)

由于一些方位向模糊和杂波像素也具有与船只类似的散射强度，所以仅仅利用强度分布模型检测船只可能会误检这类目标，因此在考虑目标强度分布λintensity的同时，Leng等人也将目标的空域分布增加到常规的CFAR检测器中，并称之为“Bilateral CFAR”检测器[7]。



在文章[7]中，Leng等人用核密度估计量来确定目标的空域分布。核密度估计量被定义为[8]:

![](https://img-blog.csdnimg.cn/20190302174856114.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly96aGFuZ2Zhbi5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

明显的，式(14)是内x与xj差异的累加。由于船只目标的像素是连续、集中分布的，所以这些像素对应的fh=1(x)值较大，而海杂波像素是分散的，fh=1(x)值较小，故式13可被用来检测舰船目标。



表 1: SAR图像中不同像素类型的xintensity, xspatial和xcombined值

![](https://img-blog.csdnimg.cn/20190302174934966.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly96aGFuZ2Zhbi5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)



![](https://img-blog.csdnimg.cn/20190302174950957.png)

W为窗口的宽度大小。



由于海面上共有4种类型像素，即船只像素，非匀质区域的明亮像素，非匀质区域的黑暗像素和匀质区域的黑暗像素，所以根据强度和空域分布的关系，舰船检测中xcombined会出现如表.1所示的4种情况。可以发现不管在哪种情况下，组合而成的xcombined图像是个高信杂比的图像，所以利用文献[9]中的双模法能够进一步求得阈值T，最终利用标准 CFAR(Standard CFAR)检测器来检测舰船目标。图2给出了整个算法的流程图。



![](https://img-blog.csdnimg.cn/20190302175008327.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly96aGFuZ2Zhbi5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

图 2. Bilateral CFAR检测算法流程图



![](https://img-blog.csdnimg.cn/20190302175022532.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly96aGFuZ2Zhbi5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

图3.数据集. (a) TerraSAR-X: VV; (b) ENVISAT: VV



表 2: 不同算法关于TerraSAR-X和ENVISAT数据集的检测结果

![](https://img-blog.csdnimg.cn/20190302175036826.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly96aGFuZ2Zhbi5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

为了检测Bilateral CFAR检测器的性能，Leng等人选择了2组不同的实验数据集，即TerraSAR-X数据集和ENVISAT数据集。图3分别给出了两组数据集所对应的VV强度图。



表 2 给出了这两组数据集的实验结果。明显的，改进后的Bilateral CFAR检测器性能要优于Standard CFAR检测器的性能。图 4进一步给出了二者的ROC曲线。

![](https://img-blog.csdnimg.cn/20190302175051222.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly96aGFuZ2Zhbi5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

图 4. Bilateral CFAR检测器和Standard CFAR检测器的ROC曲线图对比



3.2. 基于极化分解的舰船目标检测

Sugimoto等人[10]利用船、海不同的散射机制提出了两种不同的舰船目标检测方法。在构造检测器之前，Sugimoto等先将协方差矩阵[C]进行如下的去旋转角度处理:

![](https://img-blog.csdnimg.cn/20190302175106779.png)

这里，是旋转角，[Uθ]是去旋转角矩阵[11]。去旋转角处理的目的是为了进一步提高舰船目标的二次散射，降低体散射。然后在(8)的基础上，Sugimoto利用Yamaguchi 4成分分解方法(简写为 4-CSPD)[12]得到4个散射成分的值，即:单次散射Ps、二次散射Pd (这里称为Optimized Pd )、体散射Pv和螺旋体散射Pc 。



考虑到海面的极化散射机制主要以单次散射为主,所以 Sugimoto提出的第一种检测器是将单次散射Ps从总的能量PT中去除，即用“PT-Ps”去检测船只；第二种方法是直接利用4-CSPD得到的Optimized Pd成分去检测船只。第二种方式是合理的，因为相比于舰船目标，海面的二次散射强度较小。图5给出了这两种方法的流程图。概括来说，这两种算法主要由以下3个步骤构成:

![](https://img-blog.csdnimg.cn/20190302175120619.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly96aGFuZ2Zhbi5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

 图5. 算法检测流程图



![](https://img-blog.csdnimg.cn/20190302175133316.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly96aGFuZ2Zhbi5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

图6.数据集. (a) 数据集1: ALOS-PALSAR的伪彩色图; (b) 数据集2: RADARSAT-2的伪彩色图. 红色:二次散射, 绿色:体散射, 蓝色:表面散射. 白色长方形框代表匀质杂波海域.



Step 1: 对[C]进行去旋转角处理，而后在进行4-CSPD分解，得到参数Ps, Optimized Pd, Pv和Pc。

Step 2: 对各个检测器(“PT - Ps”, Optimized Pd)分别进行背景杂波建模，构造出对应的CFAR检测器。

Step 3: 进行舰船目标检测，输出结果。

在实验部分，Sugimoto等选择了两种不同传感器的全极化数据集(ALOS-PALSAR和RADARSAT-2，如图6所示)来验证这两种算法的检测性能。关于数据集1，Sugimoto在对“PT-Ps”和Optimized Pd图中的长方形背景区域杂波统计分布进行拟合后发现，这两种检测器的背景杂波都符合Gamma分布，如图7所示。在Gamma分布的基础上，通过构建CFAR检测器，图8和图9分别给出了这两种不同算法关于数据集1和2的检测结果。注意检测结果是在设定Pf=1×10-3的情况下获得。



分析图8，可以发现, Optimized Pd的检测性能要优于 “PT-Ps”算法。图9进一步给出了数据集2的检测结果。显然，所有的目标都被“PT-Ps”检测到，所以综合分析图8和图9的结果可以证明“PT-Ps”的鲁棒性要强于Optimized Pd检测器。

![](https://img-blog.csdnimg.cn/20190302175151631.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly96aGFuZ2Zhbi5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

图7. 不同算法的统计分布. (a) “PT-Ps”图像; (b)图(a)中长方形的统计分布; (c) Optimized Pd 图; (d) 图(c)中长方形的统计分布                 ![](https://img-blog.csdnimg.cn/20190302175228767.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly96aGFuZ2Zhbi5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20190302174306193.gif)

图8. 不同算法关于数据集1的检测结果. (a) “PT -Ps”；(b) Optimized Pd 



![](https://img-blog.csdnimg.cn/20190302175240915.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly96aGFuZ2Zhbi5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

图9. 不同算法关于数据集2的检测结果. (a) “PT -Ps”；(b) Optimized Pd 



![](https://img-blog.csdnimg.cn/20190302175254277.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly96aGFuZ2Zhbi5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

图10. SAR图像斜距向和方位向模糊形成图解. (a) 斜距向模糊; (b) 方位向模糊



3.3. 基于极化特征的舰船目标检测

除了常见的杂波虚警之外，由舰船目标自身所导致的方位向模糊也是舰船目标检测中的一个难点问题。舰船的方位向模糊也被称为“鬼船”目标，是由多普勒效应导致的，如图(10)所示[3]。从系统设计的角度来说，太高的PRF(pulse repetition frequency)能够产生重叠的连续返回信号，进而产生斜距向模糊 (如图 10.a 所示)。相反，太低的PRF会导致多普勒频率高于PRF，进而导致方位向模糊的产生 (如图 10.b 所示)。船只的模糊现象在SAR图像斜距向和方位向上的距离ΔxAZ、ΔxRA分别是[3]:

![](https://img-blog.csdnimg.cn/20190302175306101.png)

式中，n是模糊数量，λ是波长, fp是PRF，v是SAR平台速度， fDR是多普勒比，fDC是多普勒中心。



分析式(18.a)可以看出ΔxAZ与波长无关，而在式(18.b)中，当雷达波长较短时，真实目标与其所对应的模糊在斜距向上较近。换句话说，当其它参数不变，波长较长的系统在斜距向上出现模糊的概率较低，这是因为这些模糊在斜距向上已经超出了目标区域[13]。因此，在SAR图像舰船检测中，更多学者将注意力放在方位向模糊去除当中。此外，依靠这些分析，我们也能够发现C波段的SAR图像比L波段的SAR图像更容易出现方位向模糊,如图(11)所示[13]。

![](https://img-blog.csdnimg.cn/20190302175320917.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly96aGFuZ2Zhbi5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

图 11. AIRSAR数据集. (a) L-Band 功率图; (b) C-Band 功率图. 其中A1是目标S1的方位向模糊.注意，这里图像的水平方向是斜距向，垂直方向是方位向.

![](https://img-blog.csdnimg.cn/20190302175334282.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly96aGFuZ2Zhbi5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

图 12. 方位向模糊去除算法流程图.

![](https://img-blog.csdnimg.cn/20190302175349793.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly96aGFuZ2Zhbi5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

图 13. AIRSAR C-Band 儿岛湾附近海域数据集. (a)功率图; (b) HH幅度图; (c) HV幅度图; (d) VV幅度图.注意，这里图像的水平方向是斜距向，垂直方向是方位向.



为了消除方位向模糊的影响，Wang等人[14]将式(7)中的特征值用于舰船检测。这是合理的因为舰船目标拥有复杂的金属结构，所有船只是单次散射、二次散射、去极化散射等散射机制的混合体。由于船只的方位向模糊主要是由船只的单次或者二次散射导致的，所以方位向模糊单次散射和二次散射所对应的特征值λ1和λ2较高，而去极化散射所对应的  

![](https://img-blog.csdnimg.cn/2019030217540653.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly96aGFuZ2Zhbi5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

图 14.各特征值结果图. (a)λ1;  (b)λ2;  (c)λ3.



![](https://img-blog.csdnimg.cn/20190302175420590.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly96aGFuZ2Zhbi5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

图 15.不同算法的检测结果. (a) 本文算法; (b) TP-CFAR;  (c) PWF.

λ3较低，因此特征值分解方法可被用于舰船目标检测[14]。



具体来说，Wang等利用以下4个主要步骤来检测舰船目标:

Step 1: 对相干矩阵[T]进行滤波处理.

Step 2: 分解各个像素的[T]矩阵，得到对应的3个特征值，即，λ1,λ2 ,λ3.

Step 3: 利用λ3计算每个待估像素核中心匀质区域.

Step 4: 确定阈值，输出检测结果。

图(12)给出了具体的算法流程图。



为了验证该算法的有效性，Wang等进一步选择日本儿岛湾附近海域的AIRSAR数据作为实验数据集，图(13)给出了该地区对应的C-Band 功率及各个通道强度图。明显的，A11和A12是目标T1所对应的方位向模糊。



接下来，对该数据集进行特征分解可得如图(14)所示的结果。分析图(14)可以发现，在这3个特征值结果图中，只有(c)图中的舰船目标λ3值远远高于背景杂波和方位向模糊所对应的λ3值。因此，这直接证明了特征值λ3对舰船检测的有效性。图(15)给出了各算法对该数据集的检测结果。可以发现，相比于TP-CFAR和PWF检测器，新的算法在检测到船只的同时也成功将方位向模糊去除。



四 总结

本文对目前SAR图像舰船目标检测方式、方法进行了小结，并对部分流行的算法进行了详细介绍。然而以上所介绍的算法只是SAR图像舰船目标检测算法中的冰山一角，更多的检测方法，如基于深度学习的SAR舰船检测、基于图像其它信息的SAR舰船目标检测，也将会是未来研究的重点。



[1]张澄波. 综合孔径雷达--原理、系统分析与应用. 北京: 科学出版社, 1989.

[2]陈曦，吴涛，阮祥伟. “极化SAR海面船只检测技术的研究进展”, 遥感技术与应用，2009,24(6):841-848。

[3] Velotto D., Soccorsi M., and Lehner S., “Azimuth ambiguities removal for ship detection using full polarimetric X-band SAR data”, IEEE Transactionson Geoscience and Remote Sensing, 2014,52(1):76–88.

[4] Xi. Y. Y., Lang H. T., Tao Y. H. et al., “Four-Component Model-Based Decomposition for Ship Targets Using PolSAR Data”, Remote Sensing, 2017, 9(6):621.

[5]Lee. J. S., and Pottier E., Polarimetric radar imaging: from basics to applications. CRC press, 2009.

[6]Hwang S., and Ouchi K.,” On a novel approach using MLCC and CFAR for the improvement of ship detection by synthetic aperture radar”, IEEE Geosci. Remote Sens. Lett., vol. 7, no. 2, pp. 391–395, May. 2010.

[7]Leng X. G., Ji K. F, Yang K., et al.,” A bilateral CFAR algorithm for ship detection in SAR images”, IEEE Geosci. Remote Sens. Lett., vol. 12, no. 7, pp. 1536–1540, July. 2015.

[8]Wang C., Jiang S., Zhang H., et al.,” Ship detection for high resolution SAR images based on feature analysis”, IEEE Geosci. Remote Sens. Lett., vol. 11, no. 1, pp. 119–123, Jan. 2014.

[9] Sahoo P. K., Soltani S., and Wong A. K. C., ”A survey of thresholding techniques,” Comput. Vis.

  Graph. Image Process., vol. 41, no. 2, pp. 233–260, Feb. 1988.

[10]Sugimoto M., Ouchi K., and Nakamura Y.,” On the novel use of model-based decomposition in SAR polarimetry for target detection on the sea”, Remote Sensing Letters, 2013, 4(9):843-852.

[11] Sugimoto M., Ouchi K., and Nakamura Y., “Four-component scattering power decomposition algorithm with rotation of covariance matrix”, Remote Sensing, 2012, 4:2199-2209.

[12] Yamaguchi, Y., Moriyama, T., Ishido, M. et al, “Four-component scattering model for polarimetric SAR image decomposition”, IEEE Transactions on Geoscience and Remote Sensing, 43, pp. 1699– 1706.

[13] Wei J. J, Li P., Yang J., et al, “A new automatic ship detection method using L-Band Polarimetric SAR imagery“, IEEE Journal of Selected Topics in Applied Earth Observations and Remote Sensing 2014,7 (4): 1383–1393.

[14]Wang C. C., Wang Y., and Liao M. S.,” Removal of azimuth ambiguities and detection of a ship:   using polarimetric airborne C-band SAR images ”, Internal Journal of Remote Sensing, 2012,10(20):3197-3120.




