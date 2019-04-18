# SciPy和Numpy处理能力 - wishchinYang的专栏 - CSDN博客
2014年07月13日 14:24:16[wishchin](https://me.csdn.net/wishchin)阅读数：662
个人分类：[MLandPy](https://blog.csdn.net/wishchin/article/category/1508471)
**1.SciPy和Numpy的处理能力**：
numpy的处理能力包括：
- a powerful N-dimensional array object N维数组；
- advanced array slicing methods (to select array elements)；N维数组的分片方法；
- convenient array reshaping methods；N维数组的变形方法； 
and it even contains 3 libraries with numerical routines: 
- basic linear algebra functions；基本线性代数函数； 
- basic Fourier transforms；基本傅立叶变换； 
- sophisticated random number capabilities；精巧的随机数生成能力；
scipy是科学和工程计算工具。包括处理多维数组，多维数组可以是向量、矩阵、图形（图形图像是像素的二维数组）、表格（一个表格是一个二维数组）；目前能处理的对象有：
- statistics；统计学；
- numeric integration；数值积分；
- special functions；特殊函数；
- integration, ordinarydifferential equation (ODE) solvers；积分和解常微分方程；
- gradient optimization；梯度优化；
- geneticalgorithms；遗传算法；
- parallel programming tools（an expression-to-C++ compilerfor fast execution, and others）；并行编程工具；
在将来会增加下面的计算处理能力（现在已经部分地具备了这些能力）：
- Circuit Analysis (wrapper around Spice?)；电路分析； 
- Micro-Electro Mechanical Systems simulators (MEMs)； 
- Medical image processing；医学图像处理； 
- Neural networks；神经网络； 
- 3-D Visualization via VTK；3D可视化； 
- Financial analysis；金融分析； 
- Economic analysis；经济分析； 
- Hidden Markov Models；隐藏马尔科夫模型；
**2.处理图像** 翻译链接：http://reverland.org/python/2012/11/12/numpyscipy/
原始链接：http://scipy-lectures.github.io/advanced/image_processing/index.html
## 特征提取和分形：
### 边缘检测
合成数据：
```
>>> im = np.zeros((256, 256))
>>> im[64:-64, 64:-64] = 1
>>>
>>> im = ndimage.rotate(im, 15, mode='constant')
>>> im = ndimage.gaussian_filter(im, 8)
```
使用_梯度操作(Sobel)_来找到搞强度的变化：
```
>>> sx = ndimage.sobel(im, axis=0, mode='constant')
>>> sy = ndimage.sobel(im, axis=1, mode='constant')
>>> sob = np.hypot(sx, sy)
```
![sob](http://scipy-lectures.github.com/_images/plot_find_edges_1.png)
[示例源码](http://scipy-lectures.github.com/advanced/image_processing/auto_examples/plot_find_edges.html#example-plot-find-edges-py)
*canny滤镜*
Canny滤镜可以从`skimage`中获取([文档](http://scikit-image.org/docs/dev/api/skimage.filter.html#canny))，但是为了方便我们在这个教程中作为一个_单独模块_导入：
```
>>> #from skimage.filter import canny
>>> #or use module shipped with tutorial
>>> im += 0.1*np.random.random(im.shape)
>>> edges = canny(im, 1, 0.4, 0.2) # not enough smoothing
>>> edges = canny(im, 3, 0.3, 0.2) # better parameters
```
![edge](http://scipy-lectures.github.com/_images/plot_canny_1.png)
[示例源码](http://scipy-lectures.github.com/advanced/image_processing/auto_examples/plot_canny.html#example-plot-canny-py)
需要调整几个参数……过度拟合的风险
### 分割
- 
基于_直方图_的分割(没有空间信息)
```
>>> n = 10
  >>> l = 256
  >>> im = np.zeros((l, l))
  >>> np.random.seed(1)
  >>> points = l*np.random.random((2, n**2))
  >>> im[(points[0]).astype(np.int), (points[1]).astype(np.int)] = 1
  >>> im = ndimage.gaussian_filter(im, sigma=l/(4.*n))
    
  >>> mask = (im > im.mean()).astype(np.float)
  >>> mask += 0.1 * im
  >>> img = mask + 0.2*np.random.randn(*mask.shape)
    
  >>> hist, bin_edges = np.histogram(img, bins=60)
  >>> bin_centers = 0.5*(bin_edges[:-1] + bin_edges[1:])
    
  >>> binary_img = img > 0.5
```
![segmente](http://scipy-lectures.github.com/_images/plot_histo_segmentation_1.png)
