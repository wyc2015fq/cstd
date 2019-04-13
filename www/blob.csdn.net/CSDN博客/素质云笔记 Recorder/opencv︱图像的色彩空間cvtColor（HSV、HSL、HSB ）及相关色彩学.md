
# opencv︱图像的色彩空間cvtColor（HSV、HSL、HSB ）及相关色彩学 - 素质云笔记-Recorder... - CSDN博客

2017年04月27日 17:20:03[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：12319



# 一、色彩空间理论
几种色彩空间：
## 1、RGB
Red  红色，0-255
Green 绿色，0-255
Blue 蓝色，0-255
![这里写图片描述](https://img-blog.csdn.net/20170427170331802?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 2、HSI
HSI色彩空间可以用一个圆锥空间模型来描述。用这种 描述HIS色彩空间的圆锥模型相当复杂，但确能把色调、亮度和色饱和度的变化情形表现得很清楚。
通常把色调和饱和度通称为色度，用来表示颜色的类别与深浅程度。由于人的视觉对亮度的敏感 程度远强于对颜色浓淡的敏感程度，为了便于色彩处理和识别，人的视觉系统经常采用HSI色彩空间， 它比RGB色彩空间更符合人的视觉特性。
色相 (Hue)：指物体传导或反射的波长。更常见的是以颜色如红色，橘色或绿色来辨识，取 0 到 360 度的数值来衡量。
饱和度 (Saturation)：又称色度，是指色彩的强度或纯度。饱和度代表灰色与色调的比例，并以 0% (灰色) 到 100% (完全饱和) 来衡量。
亮度 (Intensity)：是指颜色的相对明暗度，通常以 0% (黑色) 到 100% (白色) 的百分比来衡量。
## 3、HSL
HSL 表示 hue（色相）、saturation（饱和度）、lightness（亮度）
HSL 在概念上表示了一个双圆锥体和圆球体（白色在上顶点，黑色在下顶点，最大横切面的圆心是半程灰色）
色相決定顏色的本質，像我們說紅、澄、黃就是指一種色相，
飽和度是指顏色的深淺比例，顏色越深飽和度越高，白色所占比例越高，飽和度越低，
亮度表示顏色的明暗程度，數值越大越亮
## 4、HSV
HSV 表示 hue、saturation、value(色调)
HSV（色相，饱和度，明度）在概念上可以被认为是颜色的倒圆锥体（黑点在下顶点，白色在上底面圆心）
## 5、HSB
HSB 表示 hue、saturation、brightness（明度）。
## 6、YCrCb
Y 是亮度(Luminance)，Cb、Cr 是色差(chrominance)，Cb是藍色色差 ，Cr是紅色色差，雖然RGB與YCbCr都為三個通道無法節省儲存空間，但實際上可利用人類視覺對亮度比較敏感，而對彩度比較不敏感的特質來減少內存，也就是減少Cb、Cr的取樣個數。取樣格式有4：2：0、4：2：2、4：4：4三種，4：2：0格式代表每2×2的4個像素中，Y 會對每個像素取樣，而色差 CbCr 僅會在第一行兩個像素的中間取樣，因此8位元情況下，原本每個像素需要3byte儲存空間，4個像素需要12個byte，現在只要6個byte，減少了一半的內存，現今像Jpeg、Mpeg4等影像格式，都是利用YCbCr去壓縮的。
## 7、CIE XYZ
在CIE XYZ色彩空間中，有一組X、Y和Z的值，對應於紅色、綠色和藍色，並不是真的紅藍綠，而是使用匹配函數來計算出來。
![这里写图片描述](http://monkeycoding.com/wp-content/uploads/2015/12/colorSpace_3.jpg)
## 8、CIE Lab
Lab色彩空間是顏色-對立空間，帶有維度L表示亮度，a和b表示顏色對立維度，基於非線性壓縮的CIE XYZ色彩空間坐標，Lab顏色被設計來接近人類視覺，致力於感知均勻性，L分量密切匹配人類亮度感知，因此可以被用來通過修改a和b分量的輸出色階來做精確的顏色平衡，或使用L分量來調整亮度對比。當我們計算8位元或16位元圖時，R、G、B都轉成0到1之間的浮點數，比如R=R/(R+G+B)，接著用下列公式進行轉換，分別得到L、a、b的值。
## 7、之间的关系
HSL 和 HSV 二者都把颜色描述在圆柱体内的点，这个圆柱的中心轴取值为自底部的黑色到顶部的白色而在它们中间是的灰色，绕这个轴的角度对应于“色相”，到这个轴的距离对应于“饱和度”，而沿着这个轴的距离对应于“亮度”，“色调”或“明度”。
HSL 和 HSV 是设备依赖的 RGB 的简单变换，(h, s, l) 或 (h, s, v) 三元组定义的颜色依赖于所使用的特定红色、绿色和蓝色“加法原色”
![这里写图片描述](https://img-blog.csdn.net/20160526142337832?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)[ ](https://img-blog.csdn.net/20160526142337832?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![这里写图片描述](https://img-blog.csdn.net/20160526142345584?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
.
---二、opencv实现

---cv2

---.cvtColor

---(image, cv2

---.COLOR

---_BGR2HLS)
cv2

---.cvtColor

---(image, cv2

---.COLOR

---_BGR2HSV)

---用这个函数把图像从RGB转到HSV颜色空间，注意是BGR2HSV不是RGB2HSV

---因为OpenCV 默认的颜色空间是 BGR，类似于RGB，但不是RGB。

---HSV效果图

![这里写图片描述](https://img-blog.csdn.net/20170427171811294?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---cvtColor还包含了

---[官方网址](http://docs.opencv.org/2.4/modules/imgproc/doc/miscellaneous_transformations.html)

---：

---RGB \leftrightarrow GRAY ( CV_BGR2GRAY, CV_RGB2GRAY, CV_GRAY2BGR, CV_GRAY2RGB )

---RGB \leftrightarrow CIE XYZ.Rec 709 with D65 white point ( CV_BGR2XYZ, CV_RGB2XYZ, CV_XYZ2BGR, CV_XYZ2RGB )

---RGB \leftrightarrow YCrCb JPEG (or YCC) ( CV_BGR2YCrCb, CV_RGB2YCrCb, CV_YCrCb2BGR, CV_YCrCb2RGB )

---RGB \leftrightarrow HSV ( CV_BGR2HSV, CV_RGB2HSV, CV_HSV2BGR, CV_HSV2RGB )

---RGB \leftrightarrow HLS ( CV_BGR2HLS, CV_RGB2HLS, CV_HLS2BGR, CV_HLS2RGB ).

---RGB \leftrightarrow CIE L*a*b* ( CV_BGR2Lab, CV_RGB2Lab, CV_Lab2BGR, CV_Lab2RGB ).

---RGB \leftrightarrow CIE L*u*v* ( CV_BGR2Luv, CV_RGB2Luv, CV_Luv2BGR, CV_Luv2RGB ).

---.

---参考文献：

---[【OpenCV】HSV颜色识别-HSV基本颜色分量范围](http://blog.csdn.net/taily_duan/article/details/51506776)

---[色彩空間轉換(cvtColor)](http://monkeycoding.com/?p=690#BGR_YCrCb)

---[【OpenCV】颜色空间RGB与HSV(HSL)的转换](http://lib.csdn.net/article/opencv/22778)

---.

---延伸一：

---[用谷歌新开源的deeplearnJS预测互补颜色](https://mp.weixin.qq.com/s?__biz=MzA3MDgyMjMwMA==&mid=2649933411&idx=1&sn=2efc2e0492b62326829559d1c1fa7070&chksm=87309690b0471f86e4c8ed055837b6f5e3b4caa72344cab4aba650b6b073c4a950c4176c436d&mpshare=1&scene=1&srcid=0817QKrNuOxxZ5sgAJ5egeFh#rd)

---来源： 设计Mix科技实验室O自在园

---deeplearnJS简介：deeplearn.js是用于机器学习的开源WebGL加速JavaScript库。

---deeplearn.js让您可以在浏览器中训练神经网络，或运行预先训练好的模型。它提供了与TensorFlow一致的数据流图（data flow graphs）的API，及用于数值计算的函数库。

---预测效果：

---第一个颜色为随机输入的颜色originColor；

---第二个为神经网络预测的互补颜色predictedColor；

---第三个为使用算法计算的互补颜色computeComplementaryColor，作为标准答案；

---我们可以对比下，第二个跟第三个之间的差异。

![这里写图片描述](https://img-blog.csdn.net/20170909224322384?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

