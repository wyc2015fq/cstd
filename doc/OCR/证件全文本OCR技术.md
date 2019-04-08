# 证件全文本OCR技术

2018-5-3 15:04| 发布者: 炼数成金_小数| 查看: 17437| 评论: 0|原作者: 周源|来自: 携程技术中心



**摘要**: 光学字符识别，是指对文本资料的图像文件进行分析识别处理，获取文字及版面信息的过程。摄像头拍摄的图片，大多数是彩色图像，彩色图像所含信息量巨大，对于图片的内容，我们可以简单的分为前景与背景，为了让计算机 ...





简介

周源，携程技术平台研发中心高级研发经理，从事软件开发10余年。2012年加入携程，先后参与支付、营销、客服、用户中心的设计和研发。



本文从计算机视觉的前世今生，到证件全文本OCR的实践，带你了解[人工智能](http://www.dataguru.cn/article-10276-1.html?union_site=innerlink)、计算机视觉、[深度学习](http://www.dataguru.cn/article-9400-1.html?union_site=innerlink)、卷积[神经网络](http://www.dataguru.cn/article-8976-1.html?union_site=innerlink)等技术。无论是计算机视觉的入门者还是从业者，希望都可以有所收获。



## 1、什么是OCR

光学字符识别（英语：Optical Character Recognition, OCR），是指对文本资料的图像文件进行分析识别处理，获取文字及版面信息的过程。



一般的识别过程包括：

图像输入：对于不同的图像格式，有着不同的存储格式，不同的压缩方式，目前有[OpenCV](http://www.dataguru.cn/article-9662-1.html?union_site=innerlink)、CxImage等开源项目。



预处理：主要包括二值化，噪声去除，倾斜校正等。



二值化：摄像头拍摄的图片，大多数是彩色图像，彩色图像所含信息量巨大，对于图片的内容，我们可以简单的分为前景与背景，为了让计算机更快的、更好地识别文字，我们需要先对彩色图进行处理，使图片只剩下前景信息与背景信息，可以简单的定义前景信息为黑色，背景信息为白色，这就是二值化图。



噪声去除：对于不同的文档，我们对噪声的定义可以不同，根据噪声的特征进行去噪，就叫做噪声去除。



倾斜校正：由于一般用户，在拍照文档时，都比较随意，因此拍照出来的图片不可避免的产生倾斜，这就需要文字识别软件进行校正。



版面分析：将文档图片分段落，分行的过程就叫做版面分析。由于实际文档的多样性和复杂性，目前还没有一个固定的，最优的切割模型。



字符切割：由于拍照条件的限制，经常造成字符粘连，断笔，因此极大限制了识别系统的性能。



字符识别：这一研究已经是很早的事情了，比较早有模板匹配，后来以特征提取为主，由于文字的位移，笔画的粗细，断笔，粘连，旋转等因素的影响，极大影响特征的提取的难度。



版面还原：人们希望识别后的文字，仍然像原文档图片那样排列着，段落不变，位置不变，顺序不变地输出到Word文档、PDF文档等，这一过程就叫做版面还原。



后处理、校对：根据特定的语言上下文的关系，对识别结果进行校正，就是后处理。



目前的主流实现是CNN+RNN+CTC和CNN+RNN 基于 Attention的方法。



## 2、携程证件OCR项目

### 2.1 项目目标

根据携程的实际使用场景，使用OCR技术识别身份证、护照、火车票、签证等证件的中文英文及数字文本信息。



### 2.2 主要进展及展望

2016年，实现客户端身份证、护照英文数字的识别。

2017年，实现Offline场景身份证、护照、火车票等中文识别。

2018年，实现APP实时识别身份证、护照、火车票、驾驶证、行驶证、签证等中英文文本。



### 2.3 大概的精度情况

#### 2.3.1.数字英文

误识率<0.5%【线上数据统计】

拒识率~5%

干扰因素包括：曝光、倾斜、远照



#### 2.3.2.中文

1:N有引导（指引导用户将证件放于相机框中）

FAR= 1%【线上数据统计】

拒识率～20%

 

1:N+1  无引导

FAR= 3%【线上数据统计】

拒识率～30%

曝光、图像质量低计入FAR

遮挡计入拒识

考虑外籍证件

考虑少数民族



## 3、关键知识

### 3.1 计算机视觉

#### 3.1.1 什么是计算机视觉

解读w*h*3个0～255之间的数字中蕴含的、人类可理解的内容。

[![img](http://attachbak.dataguru.cn/attachments/portal/201805/03/150017tpztl1hhxurl59m5.jpg)](http://attachbak.dataguru.cn/attachments/portal/201805/03/150017tpztl1hhxurl59m5.jpg)

[![img](http://attachbak.dataguru.cn/attachments/portal/201805/03/150017aepepnussls2eeql.jpg)](http://attachbak.dataguru.cn/attachments/portal/201805/03/150017aepepnussls2eeql.jpg)



[![img](http://attachbak.dataguru.cn/attachments/portal/201805/03/150018eigkl1lnnnnqlqul.jpg)](http://attachbak.dataguru.cn/attachments/portal/201805/03/150018eigkl1lnnnnqlqul.jpg)



#### 3.1.2 人类视觉的启示

感受野：这个位置里适当的刺激能够引起该神经元反应的区域。



[![img](http://attachbak.dataguru.cn/attachments/portal/201805/03/150018ecs3szdeezr0347e.jpg)](http://attachbak.dataguru.cn/attachments/portal/201805/03/150018ecs3szdeezr0347e.jpg)

[![img](http://attachbak.dataguru.cn/attachments/portal/201805/03/150018arcrvc1z5qcbcozc.jpg)](http://attachbak.dataguru.cn/attachments/portal/201805/03/150018arcrvc1z5qcbcozc.jpg)

层级感受野：一个神经细胞看的更远（视野更大）、能处理更负责的任务。



[![img](http://attachbak.dataguru.cn/attachments/portal/201805/03/150020yq13iia131ay6kn3.jpg)](http://attachbak.dataguru.cn/attachments/portal/201805/03/150020yq13iia131ay6kn3.jpg)

神经网络从输入到输出，中间有多个隐藏的层。



[![img](http://attachbak.dataguru.cn/attachments/portal/201805/03/150021u909ih92k9sjv10v.jpg)](http://attachbak.dataguru.cn/attachments/portal/201805/03/150021u909ih92k9sjv10v.jpg)

#### 3.1.3 HSV和灰度图

HSV是一种将RGB色彩模型中的点在圆柱坐标系中的表示法。这两种表示法试图做到比RGB基于笛卡尔坐标系的几何结构更加直观。



色相（H）是色彩的基本属性，就是平常所说的颜色名称，如红色、黄色等。

饱和度（S）是指色彩的纯度，越高色彩越纯，低则逐渐变灰，取0-100%的数值。

明度（V），亮度（L），取0-100%。



[![img](http://attachbak.dataguru.cn/attachments/portal/201805/03/150022hkifk7hwxixkiug7.jpg)](http://attachbak.dataguru.cn/attachments/portal/201805/03/150022hkifk7hwxixkiug7.jpg)

HSV模型通常用于计算机图形应用中。在用户必须选择一个颜色应用于特定图形元素各种应用环境中，经常使用HSV 色轮。在其中，色相表示为圆环；可以使用一个独立的三角形来表示饱和度和明度。典型的，这个三角形的垂直轴指示饱和度，而水平轴表示明度。在这种方式下，选择颜色可以首先在圆环中选择色相，在从三角形中选择想要的饱和度和明度。



[![img](http://attachbak.dataguru.cn/attachments/portal/201805/03/150022k74rp2zprncqreep.jpg)](http://attachbak.dataguru.cn/attachments/portal/201805/03/150022k74rp2zprncqreep.jpg)

图像的灰度化：在计算机领域中，灰度（Grayscale）数字图像是每个像素只有一个采样颜色的图像。这类图像通常显示为从最暗黑色到最亮的白色的灰度，尽管理论上这个采样可以是任何颜色的不同深浅，甚至可以是不同亮度上的不同颜色。



灰度图像与黑白图像不同，在计算机图像领域中黑白图像只有黑白两种颜色，灰度图像在黑色与白色之间还有许多级的颜色深度。但是，在数字图像领域之外，“黑白图像”也表示“灰度图像”，例如灰度的照片通常叫做“黑白照片”。在一些关于数字图像的文章中单色图像等同于灰度图像，在另外一些文章中又等同于黑白图像。





### 3.2 基于卷积神经网络的深度学习模型



[![img](http://attachbak.dataguru.cn/attachments/portal/201805/03/150022v8cn48p2pnf2zd24.jpg)](http://attachbak.dataguru.cn/attachments/portal/201805/03/150022v8cn48p2pnf2zd24.jpg)





[![img](http://attachbak.dataguru.cn/attachments/portal/201805/03/150023ren9e6rlqgglzsny.jpg)](http://attachbak.dataguru.cn/attachments/portal/201805/03/150023ren9e6rlqgglzsny.jpg)[![img](http://attachbak.dataguru.cn/attachments/portal/201805/03/150024h91taai9vvrbae0q.jpg)](http://attachbak.dataguru.cn/attachments/portal/201805/03/150024h91taai9vvrbae0q.jpg)[![img](http://attachbak.dataguru.cn/attachments/portal/201805/03/150024jwwwdg2ty0g9eklt.jpg)](http://attachbak.dataguru.cn/attachments/portal/201805/03/150024jwwwdg2ty0g9eklt.jpg)



[![img](http://attachbak.dataguru.cn/attachments/portal/201805/03/150353sdhd3z3guhb535id.jpg)](http://attachbak.dataguru.cn/attachments/portal/201805/03/150353sdhd3z3guhb535id.jpg)

3.3 二值化和池化

二值化（英语：Thresholding）是图像分割的一种最简单的方法。二值化可以把灰度图像转换成二值图像。把大于某个临界灰度值的像素灰度设为灰度极大值，把小于这个值的像素灰度设为灰度极小值，从而实现二值化。



根据阈值选取的不同，二值化的[算法](http://www.dataguru.cn/article-5747-1.html?union_site=innerlink)分为固定阈值和自适应阈值。比较常用的二值化方法则有：双峰法、P参数法、迭代法和OTSU法等。

[![img](http://attachbak.dataguru.cn/attachments/portal/201805/03/150025tu3vgtivudspqvvh.jpg)](http://attachbak.dataguru.cn/attachments/portal/201805/03/150025tu3vgtivudspqvvh.jpg)

 

## 4、证件OCR的架构及实现

### 4.1 架构图

[![img](http://attachbak.dataguru.cn/attachments/portal/201805/03/150025v7w8y6yl9367w81y.jpg)](http://attachbak.dataguru.cn/attachments/portal/201805/03/150025v7w8y6yl9367w81y.jpg)



### 4.2 应用场景



[![img](http://attachbak.dataguru.cn/attachments/portal/201805/03/150026sheufbzfkyaeq61z.jpg)](http://attachbak.dataguru.cn/attachments/portal/201805/03/150026sheufbzfkyaeq61z.jpg)

[![img](http://attachbak.dataguru.cn/attachments/portal/201805/03/150026chuuowdy4nszsdsu.jpg)](http://attachbak.dataguru.cn/attachments/portal/201805/03/150026chuuowdy4nszsdsu.jpg)

[![img](http://attachbak.dataguru.cn/attachments/portal/201805/03/150026hwwz9nqkwuiph9gp.jpg)](http://attachbak.dataguru.cn/attachments/portal/201805/03/150026hwwz9nqkwuiph9gp.jpg)

### 4.3 拒识检测

以下图为例，当用户将我们待识别区域（即姓名位置）遮挡时，我们会执行拒识处理。

[![img](http://attachbak.dataguru.cn/attachments/portal/201805/03/150027ldbcdke5lc4jf43k.jpg)](http://attachbak.dataguru.cn/attachments/portal/201805/03/150027ldbcdke5lc4jf43k.jpg)



同理如下图，待识别区域发生明显曝光时，我们也会加入拒识处理。

[![img](http://attachbak.dataguru.cn/attachments/portal/201805/03/150027lwuwrep5q1rvp75q.jpg)](http://attachbak.dataguru.cn/attachments/portal/201805/03/150027lwuwrep5q1rvp75q.jpg)



拒识处理使用直方图均衡等技术。



直方图均衡化是图像处理领域中利用图像直方图对对比度进行调整的方法。



这种方法通常用来增加许多图像的全局对比度，尤其是当图像的有用数据的对比度相当接近的时候。通过这种方法，亮度可以更好地在直方图上分布。这样就可以用于增强局部的对比度而不影响整体的对比度，直方图均衡化通过有效地扩展常用的亮度来实现这种功能。



这种方法对于背景和前景都太亮或者太暗的图像非常有用，尤其是可以带来X光图像中更好的骨骼结构显示以及曝光过度或者曝光不足照片中更好的细节。



一个主要优势是它是个相当直观的技术并且是可逆操作，如果已知均衡化函数，那么就可以恢复原始的直方图，并且计算量也不大。一个缺点是它对处理的数据不加选择，它可能会增加背景噪声的对比度并且降低有用信号的对比度。

[![img](http://attachbak.dataguru.cn/attachments/portal/201805/03/150028sd2qm5zjerdohqyy.jpg)](http://attachbak.dataguru.cn/attachments/portal/201805/03/150028sd2qm5zjerdohqyy.jpg)





[![img](http://attachbak.dataguru.cn/attachments/portal/201805/03/150028h01z2oojhco3l3bt.jpg)](http://attachbak.dataguru.cn/attachments/portal/201805/03/150028h01z2oojhco3l3bt.jpg)

拒识处理在客户端或页面前端完成，没有前端的场景则在后端完成。拒识处理还使用二分搜索算法通过大量样本判断目标图片是否拒识。

```python
def binary_search(arr,start,end,hkey):
 ifstart > end:
    return -1
 mid = start + (end - start) / 2
 ifarr[mid] > hkey:
    returnbinary_search(arr, start, mid - 1, hkey)
 ifarr[mid] < hkey:
    returnbinary_search(arr, mid + 1, end, hkey)
 return mid
```





线上版本拒识的精度在98%-99%之间，拒错率在20%以下。



### 4.4 文本检测

文本检测分为有引导和无引导两类，有引导的文本检测，我们使用先验知识（比如人脸，证件边缘等）和大量样本深度学习目标函数定位待识别区域。



而无引导的情况，则完全使用基于Attention的整行识别技术做范文本处理。



人脸识别部分，借鉴了山世光老师的开源项目seetaface/SeetaFaceEngine，并针对我们证件场景做了一些定制开发。



[![img](http://attachbak.dataguru.cn/attachments/portal/201805/03/150029feu3a9h8s8msg1gs.jpg)](http://attachbak.dataguru.cn/attachments/portal/201805/03/150029feu3a9h8s8msg1gs.jpg)

[![img](http://attachbak.dataguru.cn/attachments/portal/201805/03/150029j3l06m96wdsrdrrx.jpg)](http://attachbak.dataguru.cn/attachments/portal/201805/03/150029j3l06m96wdsrdrrx.jpg)



[![img](http://attachbak.dataguru.cn/attachments/portal/201805/03/150030w4z4s1mdtd4m100d.jpg)](http://attachbak.dataguru.cn/attachments/portal/201805/03/150030w4z4s1mdtd4m100d.jpg)

[![img](http://attachbak.dataguru.cn/attachments/portal/201805/03/150030ld9aoyg9mme8bkmd.jpg)](http://attachbak.dataguru.cn/attachments/portal/201805/03/150030ld9aoyg9mme8bkmd.jpg)

### 4.5 文本识别

文本识别部分我们使用灰度投影在切割无关信息（比如少数民族的拼音、外籍护照的发音注释等）、二值化归一化/下采样池化等技术做比较识别。并且引入了HOG、LBP、Haar等特征的权重机制。



```cpp

int* v = NULL;//垂直投影
   int* h = NULL;//水平投影
   CvScalar s,t;//投影时矩阵的元素
   IplImage* pBinaryImg = NULL;//二值化后图像
   IplImage* pVerticImg = NULL;//垂直投影图像
   IplImage* pHorizImg = NULL;//水平投影图像
   int x,y;//图像像素坐标                              
   v=new int[pBinaryImg->width]; 
      h=new int[pBinaryImg->height];
      for(i=0;i<pBinaryImg->width;i++)  
         v[i]=0; 
      for(i=0;i<pBinaryImg->height;i++)  
         h[i]=0;

      for( x=0;x<pBinaryImg->width;x++)     
      {  
         for(y=0;y<pBinaryImg->height;y++)
         {  
           s=cvGet2D(pBinaryImg,y,x);    //t=cvGet2D(paint,y,x);      
            if(s.val[0]==0)   
               v[x]++;    //cvSet2D(paint,y,x,t);        
         }    
      }
      
      for( y=0;y<pBinaryImg->height;y++)
      {  
         for( x=0;x<pBinaryImg->width;x++)
         {   
           s=cvGet2D(pBinaryImg,y,x);    //t=cvGet2D(paint,y,x);      
            if(s.val[0]==0)   
               h[y]++;        
         }    
      }
      pVerticImg = cvCreateImage( cvGetSize(pBinaryImg),8,1 );
      pHorizImg = cvCreateImage( cvGetSize(pBinaryImg),8, 1);
      cvZero(pVerticImg);
      cvZero(pHorizImg);
      for(x=0;x<pBinaryImg->width;x++)
      {  
         for(y=0;y<v[x];y++)
         {   
           t=cvGet2D(pVerticImg,y,x);   //s=cvGet2D(paint,y,x);   //t=cvGet2D(paint,y,x); 
            t.val[0]=255;   
           cvSet2D(pVerticImg,y,x,t);         
         }   
      }
      
      for(y=0;y<pBinaryImg->height;y++)
      {  
         for(x=0;x<h[y];x++)
         {   
           t=cvGet2D(pHorizImg,y,x);   //s=cvGet2D(paint,y,x);   //t=cvGet2D(paint,y,x); 
            t.val[0]=255;   
           cvSet2D(pHorizImg,y,x,t);        
         }    
      }
```





#### 4.5.1 HOG特征

方向梯度直方图（Histogram of Oriented Gradient, HOG）特征是一种在计算机视觉和图像处理中用来进行物体检测的特征描述子。它通过计算和统计图像局部区域的梯度方向直方图来构成特征。Hog特征结合 SVM分类器已经被广泛应用于图像识别中，尤其在行人检测中获得了极大的成功。需要提醒的是，HOG+SVM进行行人检测的方法是法国研究人员Dalal 在2005的CVPR上提出的，而如今虽然有很多行人检测算法不断提出，但基本都是以HOG+SVM的思路为主。



#### 4.5.2 LBP特征

LBP（Local Binary Pattern，局部二值模式）是一种用来描述图像局部纹理特征的算子；具有旋转不变性和灰度不变性等显著的优点。它是首先由T. Ojala, M.Pietikäinen, 和D. Harwood 在1994年提出，用于纹理特征提取。而且，提取的特征是图像的局部的纹理特征。



#### 4.5.3 Haar特征

Haar-like特征最早是由Papageorgiou等应用于人脸表示，Viola和Jones在此基础上，使用3种类型4种形式的特征。



Haar特征分为三类：边缘特征、线性特征、中心特征和对角线特征，组合成特征模板。特征模板内有白色和黑色两种矩形，并定义该模板的特征值为白色矩形像素和减去黑色矩形像素和。Haar特征值反映了图像的灰度变化情况。例如：脸部的一些特征能由矩形特征简单的描述，如：眼睛要比脸颊颜色要深，鼻梁两侧比鼻梁颜色要深，嘴巴比周围颜色要深等。但矩形特征只对一些简单的图形结构，如边缘、线段较敏感，所以只能描述特定走向（水平、垂直、对角）的结构。



### 4.6 一些后处理的思路

在识别主体逻辑外，我们根据实际场景加入了一些后处理逻辑。

比如身份证号验证、护照号验证、汉字权重验证、中文姓氏库验证、中文发音验证等。



## 5.参考资料

1）https://zh.wikipedia.org/wiki/(2018)

2）Learning2See_VAI_Progrss&Trends - 山世光

3）Gradient-Based Learning Applied toDocument Recognition in Proceedings of the IEEE, 1998

4）ImageNetClassification with Deep Convolutional Neural Networks

5）Noteson Convolutional Neural Networks

6）Practical Recommendations for Gradient-BasedTraining of Deep Architectures

7）Stochastic Pooling for Regularization of DeepConvolutional Neural Networks

8）Maxout Networks





