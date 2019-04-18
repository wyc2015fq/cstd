# Python-OpenCV 图像加权混合（滤色， 叠加， 柔光， 强光） - wsp_1138886114的博客 - CSDN博客





2018年09月28日 14:19:37[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：801











### OpenCV 图像混合模式
- [一、图像混合原理](#_1)
- [1.1 滤色模式](#11__2)
- [1.2 叠加模式](#12__12)
- [1.3 柔光模式](#13__26)
- [1.4 强光模式](#14__39)
- [二、线性混合（相加）](#_49)




### 一、图像混合原理

##### 1.1 滤色模式

作用结果和正片叠底刚好相反，它是将**两个颜色的互补色的像素值相乘**，然后除以255得到的最终色的像素值。
- 通常执行滤色模式后的颜色都较浅：

任何颜色和黑色执行滤色，原色不受影响;

任何颜色和白色执行滤色得到的是白色；

而与其他颜色执行滤色会产生漂白的效果。- Screen 滤色
$C=1-(1-A)×(1-B)$ 也可以写成 1-C=(1-A)*(1-B)

该模式和上一个模式刚好相反，**上下层像素的标准色彩值反相后相乘后输出**，输出结果比两者的像素值都将要亮（就好像两台投影机分别对其中一个图层进行投影后，然后投射到同一个屏幕上）。从第二个公式中我们可以看出，如果两个图层反相后，采用Multiply模式混合，则将和对这两个图层采用 Screen模式混合后反相的结果完全一样。
##### 1.2 叠加模式

在保留底色明暗变化的基础上使用“正片叠底”或“滤色”模式，绘图的颜色被叠加到底色上，但保留底色的高光和阴影部分。
- 底色的颜色没有被取代，而是和绘图色混合来体现原图的亮部和暗部。

使用此模式可使底色的图像的饱和度及对比度得到相应的提高，使图像看起来更加鲜亮。
- Overlay 叠加
$B \leqslant 0.5: C=2×A×B$
$B>0.5: C=1-2×(1-A)×(1-B)$

依据下层色彩值的不同，该模式可能是Multiply，也可能是Screen模式。**上层决定了下层中间色调偏移的强度。**- 如果上层为50%灰，则结果将完全为下层像素的值。
- 如果上层比50%灰暗，则下层的中间色调的将向暗地方偏移，

对于上层比50%灰暗，下层中间色调以下的色带变窄（原来为0~2*0.4*0.5，现在为0~2*0.3*0.5），中间色调以上的色带变宽（原来为2*0.4*0.5~1，现在为2*0.3*0.5~1）。反之亦然。
- 如果上层比50%灰亮，则下层的中间色调的将向亮地方偏移。

##### 1.3 柔光模式
- 根据绘图色的明暗程度来决定最终色是变亮还是变暗：

当绘图色比50%的灰要亮时，则 底色图像变亮。

当绘图色比50%的灰要暗时，则底色图像就变暗。

如果绘图色有纯黑色或纯白色，最终色不是黑色或白色，而是稍微变暗或变亮。

如果底色是纯白色或纯黑色，不产生任何效果。此效果与发散的聚光灯照在图像上相似。- Soft Light 柔光（叠加时去除灰色调）
$A \leqslant 0.5: C=(2*A-1)*(B-B*B)+B$
$A>0.5 : C=(2*A-1)*(sqrt(B)-B)+B$

混合色 <=128：结果色 = 基色 + (2 * 混合色 - 255) * (基色 - 基色 * 基色 / 255) / 255；

混合色 >128： 结果色 = 基色 + (2 * 混合色 - 255) * (Sqrt(基色/255)*255 - 基色)/255。

该模式类似上层以Gamma值范围为2.0到0.5的方式来调制下层的色彩值。结果将是一个非常柔和的组合。
##### 1.4 强光模式
- 根据绘图色来决定是执行“正片叠底”还是“滤色”模式。

当绘图色比50%的灰要亮 时，则底色变亮，就执行“滤色”模式一样，这对增加图像的高光非常有帮助；

当绘图色比50%的灰要暗时，则底色变暗，就执行“正片叠底”模式一样，可增加 图像的暗部。

当绘图色是纯白色或黑色时得到的是纯白色和黑色。此效果与耀眼的聚光灯照在图像上相似。- Hard Light 强光
$A\leqslant0.5: C=2*A*B$
$A>0.5: C=1-2*(1-A)*(1-B)$

该模式完全相对应于Overlay模式下，两个图层进行次序交换的情况。如过上层的颜色高于50%灰，则下层越亮，反之越暗。
### 二、线性混合（相加）

简而言之：矩阵表达式为 **dst = src1 * alpha  +  src2 * beta + gamma**
$$g(x) = (1-\alpha)f_0(x)+\alpha f_1(x)$$

```python
```python
"""
cv2.addWeighted(src1, alpha, src2, beta, gamma[, dst[, dtype]]) → dst
	src1  – 输入图片1.
	alpha – 图片1的权重
	src2  – 输入图片2（image.shape & channel 相同）
	beta  – 图片2的权重
	dst   – 目标图片
	gamma – 标量求和.
	dtype – 输出图片数组深度，当两个输入数组具有相同的深度时，
	        可以将dtype设置为-1，这相当于src1..()。
"""
```
```

```python
```python
import cv2
import numpy as np

def img_filter(img):                   #计算图像梯度（高反差像素）
    x=cv2.Sobel(img,cv2.CV_16S,1,0)
    y=cv2.Sobel(img,cv2.CV_16S,0,1)

    absx=cv2.convertScaleAbs(x)
    absy=cv2.convertScaleAbs(y)
    dist=cv2.addWeighted(absx,0.5,absy,0.5,0)
    return dist

def addImage(img1, img2,alpha):
    h, w, _ = img1.shape
    """
        函数要求两张图必须是同一个size
        alpha，beta，gamma可调
    """
    img2 = cv2.resize(img2, (w, h), interpolation=cv2.INTER_AREA)

    beta = 1 - alpha
    gamma = 0
    img_add = cv2.addWeighted(img1, alpha, img2, beta, gamma)
    return img_add


if __name__ == '__main__':
    img1=cv2.imread('R_channel.png',cv2.IMREAD_COLOR)  # 以彩色图的形式读入
    dist_img = img_filter(img1)                        # 执行高通过滤
    for i in range(1,10):                              # 循环执行（不同的alpha）：显示叠加图，写入处理后的图像
        IMG_Add = addImage(img1,dist_img,i*0.1)        # alpha，beta，gamma可调
        cv2.imshow('img_add_'+ str(i), IMG_Add)
        cv2.imwrite('img_add_'+ str(i)+".png", IMG_Add)
        cv2.imshow("img1",img1)
        # cv2.waitKey()
        # cv2.destroyAllWindows()
```
```

![在这里插入图片描述](https://img-blog.csdn.net/2018092914534568?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

参考与鸣谢：

python 图层混合大全：[https://blog.csdn.net/chy555chy/article/details/54016317](https://blog.csdn.net/chy555chy/article/details/54016317)

图层混合原理
[http://blog.csdn.net/matrix_space/article/details/22416241](http://blog.csdn.net/matrix_space/article/details/22416241)
[http://blog.csdn.net/matrix_space/article/details/22425209](http://blog.csdn.net/matrix_space/article/details/22425209)
[http://blog.csdn.net/matrix_space/article/details/22426633](http://blog.csdn.net/matrix_space/article/details/22426633)
[http://blog.csdn.net/matrix_space/article/details/22427285](http://blog.csdn.net/matrix_space/article/details/22427285)
[http://blog.csdn.net/matrix_space/article/details/22488159](http://blog.csdn.net/matrix_space/article/details/22488159)
[http://blog.csdn.net/matrix_space/article/details/22488467](http://blog.csdn.net/matrix_space/article/details/22488467)
[https://blog.csdn.net/matrix_space/article/details/72284090](https://blog.csdn.net/matrix_space/article/details/72284090)










