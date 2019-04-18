# OpenCV—Python 导向滤波 - wsp_1138886114的博客 - CSDN博客





2018年11月19日 15:23:13[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：339








### 一、导向滤波原理

导向滤波是使用导向图像作为滤波内容图像，在导向图像上实现局部线性函数表达，实现各种不同的线性变换，输出变形之后的导向滤波图像。根据需要，导向图像可以跟输入图像不同或者一致。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018121119363475.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)
##### 公式及推导

假设 $I$ 是导向图像、$p$ 是输入图像、$q$ 是导向滤波输出图像；**导向滤波**是作为局部线性模型描述 **导向图像 $I$** 与 **输出图像 $q$** 之间的关系。

对于任意像素 $\rm I$ 来说，$\omega_k$窗口下的线性变换可以表述如下：
$q_i = a_k I_i + b_k$ 其中 $(a_k,b_k)$是窗口 $\omega_k$ 范围内的参数常量。

为了寻找线性相关性，窗口 $\omega_k$ 定义的损失函数为：
$$E(a_k,b_k) = \sum_{i \in \omega_k} ((a_k I_i + b_k-p_i)^2+\epsilon a_k^2)$$

其中：
$\omega_k$是对 $a_k$ 值过大时侯的正则化补偿。
$\varepsilon a_k^2$ 是抑制 $a_k$ 值过大的。
$\varepsilon$ 是调整图的模糊程度与边缘检测精度的参数。

如果导向图 $\rm I$ 没有边缘信息，输出均值模糊结果；

如果导向图 $\rm I$ 包含边缘信息，边缘信息则迁移到输出图像中实现边缘保留滤波；

上述的损失函数可以被看成一个线性回归问题，其中两个参数的求解如下：- 
$$a_k = \frac{\frac{1}{\omega_k}\sum_{i \in \omega_k} I_iP_i-\mu_k \bar{p}_k}{\sigma_k^2 + \epsilon}$$

- 
$b_k = \bar{p}_k - a_k\mu_k$

- 
$\mu_k$ 与 $\sigma_k^2$ 是导向图在 $\omega_k$ 窗口大小均值与方差

- 
$|\omega|$表示窗口内像素总数

- 
$\bar{p}_k = \frac{1}{|\omega|}\sum_{i \in \omega_k}p_i$ 在 $\omega_k$ 窗口内输入图像像素均值


使用线性相关参数 $(a_k,b_k)$，滤波输出图像就可以通过$q_i = a_k I_i + b_k$ 线性模型得到。

针对不同的窗口大小我们就会得到不同的 $q_i$ 值，所以通过它的均值作为**最终的输出结果**：
$$q_i = \frac{1}{\omega_k}\sum_{i \in \omega_k}(a_k I_i + b_k) = \bar{a}_iI_i+\bar{b}_i$$
**最终导向滤波公式**为：
$\bar{a}_，\bar{b}_i$ 是所有像素点 $i$ 上重叠窗口相关因子的均值。

导向滤波算法实现的一般步骤为：
- 读取导向图像 $\rm I$ 与 输入图像 $\rm P$
- 积分图计算 $\rm I$ 的均值与方差、输入图像 $\rm P$的均值、 $\rm I$ 与 $\rm P$的乘积 $\rm IP$
- 计算线性相关因子 $\rm a$ 与 $\rm b$
$a=(IP-I_{mean}P_{mean})/(I_{Var} +\epsilon )$
$b=P_{mean}-\textrm{a}I_{mean}$- 计算a与b的均值
- 使用均值得到导向滤波结果 $Q = a_{mean}*I+b_{mean}$

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181119152102748.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)

##### 导向滤波最常用四个功能是：
- 边缘保留滤波
- 图像去噪声
- 图像边缘羽化
- 图像增强（对比度）

```python
import numpy as np
import cv2

def guideFilter(I, p, winSize, eps):

    mean_I = cv2.blur(I, winSize)      # I的均值平滑
    mean_p = cv2.blur(p, winSize)      # p的均值平滑

    mean_II = cv2.blur(I * I, winSize) # I*I的均值平滑
    mean_Ip = cv2.blur(I * p, winSize) # I*p的均值平滑

    var_I = mean_II - mean_I * mean_I  # 方差
    cov_Ip = mean_Ip - mean_I * mean_p # 协方差

    a = cov_Ip / (var_I + eps)         # 相关因子a
    b = mean_p - a * mean_I            # 相关因子b

    mean_a = cv2.blur(a, winSize)      # 对a进行均值平滑
    mean_b = cv2.blur(b, winSize)      # 对b进行均值平滑

    q = mean_a * I + mean_b
    return q


if __name__ == '__main__':
    eps = 0.01
    winSize = (5,5)
    image = cv2.imread(r'./5921.png', cv2.IMREAD_ANYCOLOR)
    image = cv2.resize(image, None,fx=0.7, fy=0.7, interpolation=cv2.INTER_CUBIC)
    I = image/255.0        #将图像归一化
    p =I
    guideFilter_img = guideFilter(I, p, winSize, eps)

    # 保存导向滤波结果
    guideFilter_img  = guideFilter_img  * 255
    guideFilter_img [guideFilter_img  > 255] = 255
    guideFilter_img  = np.round(guideFilter_img )
    guideFilter_img  = guideFilter_img.astype(np.uint8)
    cv2.imshow("image",image)
    cv2.imshow("winSize_5", guideFilter_img )
    cv2.waitKey(0)
    cv2.destroyAllWindows()
```

当 $I=p$ 时，导向滤波就变成了边缘保持的滤波操作，此时原来求出的$a$ 和 $b$ 的表达式就变成了：
$$a_k=\frac{σ_k^2}{σ_k^2+ϵ}$$

$$b_k=(1−a_k)μ_k$$

考虑两种情况：
- 情况1：高方差区域，即表示图像 $I$ 在窗口 $w_k$ 中变化比较大，此时我们有$σ_k^2>>ϵ$，于是有$a_k≈1$和$b_k≈0$。
- 情况2：平滑区域（方差不大），即图像 $I$ 在窗口 $w_k$ 中基本保持固定，此时有$σ_k^2<<ϵ$，于是有$a_k≈0$和$b_k≈μ_k$ 。

也就是说在方差比较大的区域，保持值不变，在平滑区域，使用临近像素平均（也就退化为普通均值滤波）。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211194120264.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)

### 快速导向滤波

通过下采样减少像素点，计算 $\rm mean_a$ & $\rm mean_b$ 后进行上采样恢复到原有的尺寸大小。

假设缩放比例为s,那么缩小后像素点的个数为$N/s^2，$那么时间复杂度变为$O(N/s^2)$
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181119152303197.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)
```python
import cv2
import numpy as np

def guideFilter(I, p, winSize, eps, s):
    # 输入图像的高、宽
    h, w = I.shape[:2]

    # 缩小图像
    size = (int(round(w * s)), int(round(h * s)))
    small_I = cv2.resize(I, size, interpolation=cv2.INTER_CUBIC)
    small_p = cv2.resize(I, size, interpolation=cv2.INTER_CUBIC)

    # 缩小滑动窗口
    X = winSize[0]
    small_winSize = (int(round(X * s)), int(round(X * s)))

    # I的均值平滑 p的均值平滑
    mean_small_I = cv2.blur(small_I, small_winSize)
    mean_small_p = cv2.blur(small_p, small_winSize)

    # I*I和I*p的均值平滑
    mean_small_II = cv2.blur(small_I * small_I, small_winSize)
    mean_small_Ip = cv2.blur(small_I * small_p, small_winSize)

    # 方差、协方差
    var_small_I = mean_small_II - mean_small_I * mean_small_I
    cov_small_Ip = mean_small_Ip - mean_small_I * mean_small_p

    small_a = cov_small_Ip / (var_small_I + eps)
    small_b = mean_small_p - small_a * mean_small_I

    # 对a、b进行均值平滑
    mean_small_a = cv2.blur(small_a, small_winSize)
    mean_small_b = cv2.blur(small_b, small_winSize)

    # 放大
    size1 = (w, h)
    mean_a = cv2.resize(mean_small_a, size1, interpolation=cv2.INTER_LINEAR)
    mean_b = cv2.resize(mean_small_b, size1, interpolation=cv2.INTER_LINEAR)

    q = mean_a * I + mean_b

    return q
    
if __name__ == '__main__':
    eps = 0.01
    winSize = (16,16)       #类似卷积核（数字越大，磨皮效果越好）
    image = cv2.imread(r'./5921.png', cv2.IMREAD_ANYCOLOR)
    image = cv2.resize(image,None,fx=0.8,fy=0.8,interpolation=cv2.INTER_CUBIC)
    I = image/255.0       #将图像归一化
    p =I
    s = 3 #步长
    guideFilter_img = guideFilter(I, p, winSize, eps,s)

    # 保存导向滤波结果
    guideFilter_img = guideFilter_img  * 255         #(0,1)->(0,255)
    guideFilter_img[guideFilter_img  > 255] = 255    #防止像素溢出
    guideFilter_img = np.round(guideFilter_img )
    guideFilter_img = guideFilter_img.astype(np.uint8)
    cv2.imshow("image",image)
    cv2.imshow("winSize_16", guideFilter_img )
    cv2.waitKey(0)
    cv2.destroyAllWindows()

输出效果图如上：winSize_16
```

为了写这篇博客，查了许久的资料，可气的是，那些都是只写了一半的代码，如今代码运行通了，与诸位共勉。

鸣谢
[https://blog.csdn.net/qq_40755643/article/details/83831071](https://blog.csdn.net/qq_40755643/article/details/83831071)

特别鸣谢
[http://blog.sina.com.cn/s/blog_734f70550102wof3.html](http://blog.sina.com.cn/s/blog_734f70550102wof3.html)
[https://blog.csdn.net/baimafujinji/article/details/74750283](https://blog.csdn.net/baimafujinji/article/details/74750283)








