# 基于方向梯度直方图(HOG)的人脸检测 - 墨流觞的博客 - CSDN博客





2018年09月12日 16:32:09[墨氲](https://me.csdn.net/dss_dssssd)阅读数：695








# 基于方向梯度直方图(HOG)的人脸检测

论文：[Histograms of Oriented Gradients for Human Detection, CVPR 2005](http://lear.inrialpes.fr/pubs/2005/DT05/hog_cvpr2005.pdf)

惯例，分享歌词一句：

> 
《化鹤归》 

  醉举杯饮星河 人间胜事且勾勒 

  来日悠长 此间你我 终能把 岁月雕琢
## 每个像素点的梯度计算公式：
- $G_x(x, y) = H(x+1, y) - H(x-1, y)$
- $G_y(x, y) = H(x, y+1) - H(x, y-1)$
- 幅度: $G(x, y) = \sqrt{{G_x(x, y)}^2 + {G_y(x, y)}^2}$
- 方向：$\alpha (x, y) = tan^{-1} \frac {G_x(x, y)} {G_y(x, y)}$

## 算法流程

![](./流程图.png)![这里写图片描述](https://img-blog.csdn.net/20180912162645123?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](./图片解释.jpg)![这里写图片描述](https://img-blog.csdn.net/20180912162655862?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

1. 归一化图像包括先灰度化然后标准化Gamma空间来减小光照因素的影响，Gamma压缩公式为： $I(x, y) = I(x, y)^{gammma}$，比如gamma可以取0.2 

2. 计算每一个像素的梯度 

3. 以$8*8$个像素单元为最小结构单元cell,计算0~360度方向上的权重投影,后面介绍一个计算的例子，将 

360划分为9个区间，按照每个像素梯度的幅度给梯度方向的所在区间赋值。最后每个cell形成一个$9 *  1$的向量- 将4个cell组成一个block（$16*16$）,在这注意，每个cell会被不同的block共享，来通过对重叠cell的归一 

化，消除局部光照的变化以及前景-背景对比度的变化。这样最后每个block会形成$16 * 1$的向量
- 最后将目标的所有block向量串联形成该目标的特征向量。假设目标窗口为$64 * 128$, 则最后的特征向量 

长度为 $(4 * 8 * 16）*(1) =512 * 1$ 。注意第4步中由于cell的共享问题导致最后一个cell会以不同的结果 

出现在最后的特征值中。- 利用得到的(特征向量, 目标）制成训练集，来训练SVM等分类器
- 从图像上确定检测窗比如64*128，重复以上步骤，将得到的特征向量送给训练好的分类器来检测人脸

### cell中权重投影，即 $9 * 1$ 向量的计算实例

### 梯度对于图像影响的例子

注意：图像的原点是图片的左上角，x轴是水平的，y轴是垂直的 
![](x_y.jpg)![这里写图片描述](https://img-blog.csdn.net/20180912162741918?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
从上面的图像中可以看到x轴方向的梯度主要凸显了垂直方向的线条，y轴方向的梯度凸显了水平方向的梯度，梯度幅值凸显了像素值有剧烈变化的地方。

### cell权重投影计算
- 在每个像素点，都有一个幅值(magnitude)和方向，对于有颜色的图片，会在三个channel上都计算梯度。那么相应的幅值就是三个channel上最大的幅值，角度(方向)是最大幅值所对应的角。
- 分割cell 在这一步，选取的$64 * 128$的检测窗口会被分割成$8 * 8$大小的网格每个网格都会计算一个梯度直方图。每个像素有两个值(幅值magnitude,三个channel取最大的magnitude和方向direction，)，加起来就是$8 * 8 * 2=128$，然后将这128个数用一个9个条目的直方图来表示成9个数的数组。用直方图来表示一个patch更加抗噪，一个gradient可能会有噪音，但是用直方图来表示后就不会对噪音那么敏感了。

![](./分割.jpg)![这里写图片描述](https://img-blog.csdn.net/20180912162757558?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)- 计算cell中每个元素的梯度（方向和幅值）

![](./计算.jpg)![这里写图片描述](https://img-blog.csdn.net/20180912162806917?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
中间: 一个网格用箭头表示梯度 右边: 这个网格用数字表示的梯度 

中间这个图的箭头是梯度的方向，长度是梯度的大小，可以发现箭头的指向方向是像素强度都变化方向，幅值 

是强度变化的大小。 
注意，**右边的梯度方向矩阵中可以看到角度是0-180度，不是0-360度，这种被称之 

为”无符号”梯度(“unsigned” gradients)因为一个梯度和它的负数是用同一个数字表示的，也就是说一个梯度 

的箭头以及它旋转180度之后的箭头方向被认为是一样的。那为什么不用0-360度的表示呢？在实践中发现在 

某些检测任务中，unsigned gradients比signed gradients效果更好。一些HOG的实现中可以让你指定 

signed gradients。** 

4. 为$8 * 8$的网格创建直方图，直方图包含了9个bin来对应0,20,40,…160这些角度。 bin 就是 认为直方图中每 

一个数据条目.
![](./88.jpg)![这里写图片描述](https://img-blog.csdn.net/2018091216281919?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上面这张图解释了这个过程。我们用了上一张图里面的那个网格的梯度幅值和方向。根据方向选择用哪个bin, 

根据幅值来确定这个bin的大小。先来看蓝色圈圈出来的像素点，它的角度是80，幅值是2，所以它在第五个bin 

里面加了2，再来看红色的圈圈出来的像素点，它的角度是$10^。$，幅值是4，刚好介于$0^。-20^。$的中间 

(正好一半)，所以把幅值一分为二地放到$0^。$和$20^。$两个bin里面去。
![](./165.jpg)![这里写图片描述](https://img-blog.csdn.net/20180912162833525?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

对于在$160^。-180^。$之间的角度，由于$0^。$和$180^。$是一样的，所以当像素的角度为$165^。$的时 

候，要把幅值按照比例放到$0^。$和$160^。$的bin里面去。 

计算公式为: 


$value_{bin_{after}}= \frac{direction - bin_{fre}}{bin_{after} - bin_{fre}} * magnitude$


$value_{bin_{fre}} =\frac{bin_{after} - direction }{bin_{after} - bin_{fre}} * magnitude$
注意：**与距离成反比的**

比如： 


$63.75 = (180 - 165) / 20 * 85$


$21.25 = (165 - 160) / 20 * 85$
按照上面的规则，将这8*8的cell里面所有的像素点都分别加到这9个bin里面去，就构建了一个9-bin的直方图： 
![](./9bin.jpg)![这里写图片描述](https://img-blog.csdn.net/20180912162851297?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

下面是人脸特征向量的图示化结果：
![](./11.png)![这里写图片描述](https://img-blog.csdn.net/20180912162900675?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
左图是通过SVM后得到的人脸模式图，右图是检测窗口检测到的特征向量

上述就是计算过程，算法中还有很多计算和处理细节，具体细致的内容可以参考文末的参考引用

## 参考资料
- [https://www.learnopencv.com/histogram-of-oriented-gradients/](https://www.learnopencv.com/histogram-of-oriented-gradients/)
- [上面网站的中文翻译版本](https://zhuanlan.zhihu.com/p/33059421)
- [https://blog.csdn.net/zhazhiqiang/article/details/21047207](https://blog.csdn.net/zhazhiqiang/article/details/21047207)
- [https://medium.com/@ageitgey/machine-learning-is-fun-part-4-modern-face-recognition-with-deeplearning-c3cffc121d78](https://medium.com/@ageitgey/machine-learning-is-fun-part-4-modern-face-recognition-with-deeplearning-c3cffc121d78)

















