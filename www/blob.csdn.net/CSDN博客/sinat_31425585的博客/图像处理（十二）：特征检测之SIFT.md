# 图像处理（十二）：特征检测之SIFT - sinat_31425585的博客 - CSDN博客
2018年09月21日 16:23:21[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：101
    SIFT（Scale-invariant Feature Transform）是由David Lowe 提出的一种具有平移、缩放、旋转等不变性的局部特征提取方法。
**    1、高斯金字塔**
          人与物体之间距离由近及远过程中，物体在视网膜上对应成像规律为：尺寸由大到小，物体轮廓由清晰到模糊。通过建立高斯金字塔来模拟这样一种人眼成像规律，来保障检测到的特征点具备尺度不变性。
         构建过程：
        （1）高斯模糊：模拟物体轮廓由清晰到模糊这一规律；
        （2）下采样：模拟物体尺寸由大到小这一规律。
**    2、寻找极值（局部极值，极大值和极小值）**
          2002年Mikolajczyk在大量试验后发现，尺度归一化的高斯拉普拉斯函数的极值同样也能产生稳定的图像特征。Lowe采用更高效的高斯差分算子代替拉普拉斯算子进行极值检测：
![](https://img-blog.csdn.net/20180617141246549?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
    实际计算时，将高斯金字塔中，同一组的相邻层相减即可得到高斯差分图像：
![](https://img-blog.csdn.net/20180617141624572?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
    因此，假设高斯金字塔每层有S+3个图像，高斯差分金字塔（DOG）每层就只有S+2个图像。同时，高斯差分函数存在以下几个特点：
    （1）不同滤波图像相减后，仅保留频率位于两个滤波级别间的边缘；
    （2）滤除掉噪声的同时，保留了局部偏差；
    （3）这种方式实际上是一个带通滤波器。
    这里可以这样理解，一般采用高斯函数做平滑处理，平滑处理的意思就是将边缘地方给打磨一下，使得部分边缘被磨平，这样图像就会变得更加平滑。高斯差分其实就是计算经过打磨前后，被磨平的边缘部分，这部分边缘处于打磨前后两个滤波级别间的边缘，高斯差分的方式就相当于一个带通滤波器。
    极值检测过程：验证中间检测点是否为同尺度的8个相邻点和上下相邻尺度对应9×2个点中的极值点，以确保在尺度空间和二维图像空间都检测到极值点。 
![](https://img-blog.csdn.net/20180617143204711?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
    利用DOG方法找到的极值点不仅包含了斑点（blob-like）和角点（corner-like），还包含了具有较强响应的边缘（edges），边缘不利于寻找图像与图像间的对应关系，因此需要做一个极大值抑制。
    SIFT采用的判别准则是基于Hessian矩阵特征值的比：
![](https://img-blog.csdn.net/2018061714424327?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
    其中，H表达式如下：
![](https://img-blog.csdn.net/20180617144137272?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**    3、特征点描述**
    为了使特征点具有缩放不变性，采用特征点邻域内像素点的梯度方向分布特性来进行描述，描述的过程中，不仅仅是对当前特征点对应像素位置进行描述，还有对其周围中心环绕的近邻范围内像素进行描述，这样才能使得该特征点更具备判别性。
    首先，对于每个特征点，我们希望通过位置（location）、尺度（scale）和方向（orientation）三个信息进行描述，现在已知位置和尺度信息，方向可以利用梯度进行计算求解。
    梯度分量计算：
![\left\{\begin{matrix} dx= f(x+1,y)-f(x-1,y)&direction\ of \ x \\ dy=f(x,y+1)-f(x,y-1)&direction\ of\ y \end{matrix}\right.](https://private.codecogs.com/gif.latex?%5Cleft%5C%7B%5Cbegin%7Bmatrix%7D%20dx%3D%20f%28x&plus;1%2Cy%29-f%28x-1%2Cy%29%26direction%5C%20of%20%5C%20x%20%5C%5C%20dy%3Df%28x%2Cy&plus;1%29-f%28x%2Cy-1%29%26direction%5C%20of%5C%20y%20%5Cend%7Bmatrix%7D%5Cright.)
    梯度和方向计算：
![\left\{\begin{matrix} gradient= \sqrt{dx^{2}+dy^{2}}\\ \theta=atan(\frac{dy}{dx})\end{matrix}\right.](https://private.codecogs.com/gif.latex?%5Cleft%5C%7B%5Cbegin%7Bmatrix%7D%20gradient%3D%20%5Csqrt%7Bdx%5E%7B2%7D&plus;dy%5E%7B2%7D%7D%5C%5C%20%5Ctheta%3Datan%28%5Cfrac%7Bdy%7D%7Bdx%7D%29%5Cend%7Bmatrix%7D%5Cright.)
    要实现特征对于旋转（由观测角度、观测位置等因素造成的）的不变性，就必须保证对于同一个特征点的描述具有一定不变性，也即是保证对特征点描述的一致性。那么怎么保证？很简单，我们每次对特征点进行描述时，都从当前特征点所在邻域区块内主方向开始统计，主方向其实就是这个区域内大部分像素所处的梯度方向，这里采用直方图进行统计，然后将统计结果的峰值作为当前特征点的主方向。
~~~未完待续~~~~
    参考资料：
[1] [https://www.youtube.com/watch?v=U0wqePj4Mx0](https://www.youtube.com/watch?v=U0wqePj4Mx0)
[2] [https://www.youtube.com/watch?v=oT9c_LlFBqs](https://www.youtube.com/watch?v=oT9c_LlFBqs)
