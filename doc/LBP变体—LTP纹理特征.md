# LBP变体—LTP纹理特征

2018年08月09日 10:29:18 [God_6838](https://me.csdn.net/God_68) 阅读数：681



如今，纹理分析和分类已成为计算机视觉和图像处理的重要领域之一。 它们在许多应用中发挥着至关重要的作用，如视觉对象识别和检测[1,2]，人体探测器[3]，物体跟踪[4]，行人分类[5]，图像检索[6,7] 和人脸识别 [8,9]。

Ojala等人提出了局部二值模式 local binary pattern（LBP）算子 [10]用于旋转不变纹理分类。LBP已被修改并适用于多种应用，如人脸识别[8,9]和图像检索[7]。 LBP提取算法包含两个主要步骤，即阈值处理步骤和编码步骤。这在图（a）中示出。在阈值处理步骤中，将每个模式中的所有相邻像素值与其模式的中心像素的值进行比较，以将它们的值转换为二进制值（0或1）。此步骤有助于获取有关局部二进制差异的信息。然后在编码步骤中，对从阈值处理步骤获得的二进制数进行编码并转换成十进制数以表征结构模式。最初，Ojala等人 [11]通过计算特定局部图案的中心像素的灰度级与其邻居之间的绝对差异，使用纹理直方图来表示纹理图像。然后，作者通过使用中心像素的灰度级与其局部模式的邻居之间的差异的符号而不是幅值来提出LBP算子[10]。由Ojala等人提出的LBP [10]已成为许多计算机视觉研究人员的研究方向。这是因为它能够区分微观结构，如边缘，线条和斑点。研究人员旨在增加纹理特征提取的区分特性，以实现令人印象深刻的旋转不变纹理分类。因此，已经有许多基于LBP的变体用于旋转不变纹理分类。 

![img](https://img-blog.csdn.net/20180809100153736?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0dvZF82OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

下面我们介绍LBP及LTP！！！  

## 1.1 LBP概述

如图（a）所示，通过将中心像素的强度值与其邻居的强度值进行比较来计算中心像素的LBP算子。该过程可以用数学表达如下：

![img](https://img-blog.csdn.net/20180809100447716?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0dvZF82OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中![i_{c}](https://private.codecogs.com/gif.latex?i_%7Bc%7D)和![i_{p}](https://private.codecogs.com/gif.latex?i_%7Bp%7D) (p=0,1,2……P-1)分别表示半径圆上的中心像素的灰度值和相邻像素的灰度值，R是半径，P是相邻像素的数量。而![LBP_{P,R}^{ri}](https://private.codecogs.com/gif.latex?LBP_%7BP%2CR%7D%5E%7Bri%7D)和![LBP_{P,R}^{riu2}](https://private.codecogs.com/gif.latex?LBP_%7BP%2CR%7D%5E%7Briu2%7D)分别是LBP的旋转不变量和LBP的均匀旋转不变量。

完成上述LBP运算符的编码步骤后，![LBP_{P,R}^{ri}](https://private.codecogs.com/gif.latex?LBP_%7BP%2CR%7D%5E%7Bri%7D)和![LBP_{P,R}^{riu2}](https://private.codecogs.com/gif.latex?LBP_%7BP%2CR%7D%5E%7Briu2%7D)可以根据以下等式构建直方图： 

![img](https://img-blog.csdn.net/20180809101351165?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0dvZF82OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 其中K是最大LBP模式值。

## **1.2 LTP概述**

Tan和Triggs [12]提出了一种新的纹理算子 local ternary pattern (LTP)，它对噪声更加鲁棒。基于LBP扩展到三值编码（-1,0,1）。图（b）显示了LTP运算符的示例。

![img](https://img-blog.csdn.net/20180809101845319?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0dvZF82OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

LTP的数学表达式可以描述如下：

![img](https://img-blog.csdn.net/20180809101725542?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0dvZF82OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中![i_{c}](https://private.codecogs.com/gif.latex?i_%7Bc%7D), ![i_{p}](https://private.codecogs.com/gif.latex?i_%7Bp%7D) (p=0,1,2……P-1),R和P分别表示半径圆上的中心像素的灰度值和相邻像素的灰度值，R是半径，P是相邻像素的数量； κ是阈值。在阈值处理步骤之后，如图（b）所示构造和编码上部模式和下部模式。

 

# 参考文献：

1. J. Xiao, J. Hays, K. A. Ehinger, A. Oliva, and A. Torralba, “SUN database: large-scale scene recognition from abbey to zoo,” in *Proceedings of the IEEE Computer Society Conference on Computer Vision and Pattern Recognition (CVPR '10)*, pp. 3485–3492, June 2010. [View at Publisher](https://doi.org/10.1109%2FCVPR.2010.5539970) ·[View at Google Scholar](http://scholar.google.com/scholar?q=https://doi.org/10.1109%2FCVPR.2010.5539970) · [View at Scopus](http://www.scopus.com/scopus/inward/record.url?eid=2-s2.0-77955988947&partnerID=K84CvKBR&rel=3.0.0&md5=71171a0561804a758cd73f9c9b7ad0fe)
2. Y. Lee, D. K. Han, and H. Ko, “Reinforced adaboost learning for object detection with local pattern representations,” *The Scientific World Journal*, vol. 2013, Article ID 153465, 14 pages, 2013.[View at Publisher](https://doi.org/10.1155%2F2013%2F153465) · [View at Google Scholar](http://scholar.google.com/scholar_lookup?title=Reinforced+adaboost+learning+for+object+detection+with+local+pattern+representations&author=Y.+Lee&author=D.+K.+Han&author=H.+Ko&publication_year=2013)
3. X. Wang, T. Han, and S. Yan, “An HOG-LBP human detector with partial occlusion handling,” in *Proceedings of the 12th IEEE International Conference of Computer Vision*, pp. 32–39, 2009.
4. V. Takala and M. Pietikäinen, “Multi-object tracking using color, texture and motion,” in *Proceedings of the IEEE Computer Society Conference on Computer Vision and Pattern Recognition (CVPR '07)*, pp. 1–7, June 2007. [View at Publisher](https://doi.org/10.1109%2FCVPR.2007.383506) · [View at Google Scholar](http://scholar.google.com/scholar?q=https://doi.org/10.1109%2FCVPR.2007.383506) · [View at Scopus](http://www.scopus.com/scopus/inward/record.url?eid=2-s2.0-34948816600&partnerID=K84CvKBR&rel=3.0.0&md5=d3fe71091c2b94c9ac8f2a26534e8bbc)
5. M. Enzweiler and D. M. Gavrila, “A multilevel mixture-of-experts framework for pedestrian classification,” *IEEE Transactions on Image Processing*, vol. 20, no. 10, pp. 2967–2979, 2011.[View at Publisher](https://doi.org/10.1109%2FTIP.2011.2142006) · [View at Google Scholar](http://scholar.google.com/scholar_lookup?title=A+multilevel+mixture-of-experts+framework+for+pedestrian+classification&author=M.+Enzweiler&author=D.+M.+Gavrila&publication_year=2011) · [View at Scopus](http://www.scopus.com/scopus/inward/record.url?eid=2-s2.0-79960822863&partnerID=K84CvKBR&rel=3.0.0&md5=155438def7b8f676f7663bfa28e399da)
6. F. Qiao, C. Wang, X. Zhang, and H. Wang, “Large scale near-duplicate celebrity web images retrieval using visual and textual features,” *The Scientific World Journal*, vol. 2013, Article ID 795408, 11 pages, 2013. [View at Publisher](https://doi.org/10.1155%2F2013%2F795408) · [View at Google Scholar](http://scholar.google.com/scholar_lookup?title=Large+scale+near-duplicate+celebrity+web+images+retrieval+using+visual+and+textual+features&author=F.+Qiao&author=C.+Wang&author=X.+Zhang&author=H.+Wang&publication_year=2013)
7. S. Murala, R. P. Maheshwari, and R. Balasubramanian, “Local tetra patterns: a new feature descriptor for content-based image retrieval,” *IEEE Transactions on Image Processing*, vol. 21, no. 5, pp. 2874–2886, 2012. [View at Publisher](https://doi.org/10.1109%2FTIP.2012.2188809) · [View at Google Scholar](http://scholar.google.com/scholar_lookup?title=Local+tetra+patterns%3A+a+new+feature+descriptor+for+content-based+image+retrieval&author=S.+Murala&author=R.+P.+Maheshwari&author=R.+Balasubramanian&publication_year=2012) · [View at Scopus](http://www.scopus.com/scopus/inward/record.url?eid=2-s2.0-84860129600&partnerID=K84CvKBR&rel=3.0.0&md5=68816b35874ba5951eb933ed39c502e9)
8. B. Zhang, Y. Gao, S. Zhao, and J. Liu, “Local derivative pattern versus local binary pattern: face recognition with high-order local pattern descriptor,” *IEEE Transactions on Image Processing*, vol. 19, no. 2, pp. 533–544, 2010. [View at Publisher](https://doi.org/10.1109%2FTIP.2009.2035882) · [View at Google Scholar](http://scholar.google.com/scholar_lookup?title=Local+derivative+pattern+versus+local+binary+pattern%3A+face+recognition+with+high-order+local+pattern+descriptor&author=B.+Zhang&author=Y.+Gao&author=S.+Zhao&author=J.+Liu&publication_year=2010) · [View at Scopus](http://www.scopus.com/scopus/inward/record.url?eid=2-s2.0-76849093371&partnerID=K84CvKBR&rel=3.0.0&md5=cb76d1ec0e8aa47c4cafcba1de4ca8a7)
9. J. Y. Choi, Y. M. Ro, and K. N. Plataniotis, “Color local texture features for color face recognition,” *IEEE Transactions on Image Processing*, vol. 21, no. 3, pp. 1366–1380, 2012. [View at Publisher](https://doi.org/10.1109%2FTIP.2011.2168413) ·[View at Google Scholar](http://scholar.google.com/scholar_lookup?title=Color+local+texture+features+for+color+face+recognition&author=J.+Y.+Choi&author=Y.+M.+Ro&author=K.+N.+Plataniotis&publication_year=2012) · [View at Scopus](http://www.scopus.com/scopus/inward/record.url?eid=2-s2.0-84857296458&partnerID=K84CvKBR&rel=3.0.0&md5=996fae35c82939c2c299e59349d9b2a5)
10. T. Ojala, M. Pietikäinen, and T. Mäenpää, “Multiresolution gray-scale and rotation invariant texture classification with local binary patterns,” *IEEE Transactions on Pattern Analysis and Machine Intelligence*, vol. 24, no. 7, pp. 971–987, 2002. [View at Publisher](https://doi.org/10.1109%2FTPAMI.2002.1017623) · [View at Google Scholar](http://scholar.google.com/scholar_lookup?title=Multiresolution+gray-scale+and+rotation+invariant+texture+classification+with+local+binary+patterns&author=T.+Ojala&author=M.+Pietik%C3%A4inen&author=T.+M%C3%A4enp%C3%A4%C3%A4&publication_year=2002) · [View at Scopus](http://www.scopus.com/scopus/inward/record.url?eid=2-s2.0-0036647193&partnerID=K84CvKBR&rel=3.0.0&md5=6a80e1ad60391e4e170af8450e7b6957)
11. T. Ojala, M. Pietikäinen, and D. Harwood, “A comparative study of texture measures with classification based on feature distributions,” *Pattern Recognition*, vol. 29, no. 1, pp. 51–59, 1996. [View at Publisher](https://doi.org/10.1016%2F0031-3203(95)00067-4) · [View at Google Scholar](http://scholar.google.com/scholar_lookup?title=A+comparative+study+of+texture+measures+with+classification+based+on+feature+distributions&author=T.+Ojala&author=M.+Pietik%C3%A4inen&author=D.+Harwood&publication_year=1996) · [View at Scopus](http://www.scopus.com/scopus/inward/record.url?eid=2-s2.0-0029669420&partnerID=K84CvKBR&rel=3.0.0&md5=505c6acd0f8abaa13b166548a144ad1e)
12. X. Tan and B. Triggs, “Enhanced local texture feature sets for face recognition under difficult lighting conditions,” *IEEE Transactions on Image Processing*, vol. 19, no. 6, pp. 1635–1650, 2010. [View at Publisher](https://doi.org/10.1109%2FTIP.2010.2042645) · [View at Google Scholar](http://scholar.google.com/scholar_lookup?title=Enhanced+local+texture+feature+sets+for+face+recognition+under+difficult+lighting+conditions&author=X.+Tan&author=B.+Triggs&publication_year=2010) · [View at Scopus](http://www.scopus.com/scopus/inward/record.url?eid=2-s2.0-77952634397&partnerID=K84CvKBR&rel=3.0.0&md5=d8e93abe6ba8c3573e7e54a0be3ce53d)

 