# matplotlib2---overview of  plotting Functions: plot, scatter,bar, fill, imshow,contour,arrow,hist... - 墨流觞的博客 - CSDN博客





2018年11月23日 18:36:45[墨氲](https://me.csdn.net/dss_dssssd)阅读数：20标签：[matplotlib](https://so.csdn.net/so/search/s.do?q=matplotlib&t=blog)
个人分类：[matplotlib](https://blog.csdn.net/dss_dssssd/article/category/8401939)

所属专栏：[matplotlib一直在入门](https://blog.csdn.net/column/details/30379.html)









matplotlib有大量的绘图函数，在pyplot文档中有完整的列表，Matplotlib gallery中有大量的绘图实例。

在此文章中先介绍几种最常用的绘图函数，并对其中的一些绘图函数做详细的介绍。

## 1. 一维数据

**plot绘制 线之类函数**

code:[http://nbviewer.jupyter.org/github/matplotlib/AnatomyOfMatplotlib/blob/master/examples/plot_example.py](http://nbviewer.jupyter.org/github/matplotlib/AnatomyOfMatplotlib/blob/master/examples/plot_example.py)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181123181756259.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
**scatter 绘制散点图**

code：
[http://nbviewer.jupyter.org/github/matplotlib/AnatomyOfMatplotlib/blob/master/examples/scatter_example.py](http://nbviewer.jupyter.org/github/matplotlib/AnatomyOfMatplotlib/blob/master/examples/scatter_example.py)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181123181909320.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
**bar 绘制矩形块组成的图像， 各种条形图**

code：[http://nbviewer.jupyter.org/github/matplotlib/AnatomyOfMatplotlib/blob/master/examples/bar_example.py](http://nbviewer.jupyter.org/github/matplotlib/AnatomyOfMatplotlib/blob/master/examples/bar_example.py)
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018112318213088.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
**fill/fill_between 填充多边形**

code:
[http://nbviewer.jupyter.org/github/matplotlib/AnatomyOfMatplotlib/blob/master/examples/fill_example.py](http://nbviewer.jupyter.org/github/matplotlib/AnatomyOfMatplotlib/blob/master/examples/fill_example.py)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181123182335928.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
## 2 . 二维数据：  2维数组或图像

**imshow: 显示图像或颜色映射的数组**

code:
[http://nbviewer.jupyter.org/github/matplotlib/AnatomyOfMatplotlib/blob/master/examples/imshow_example.py](http://nbviewer.jupyter.org/github/matplotlib/AnatomyOfMatplotlib/blob/master/examples/imshow_example.py)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181123182626227.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
**pcolor/pcolormesh: 显示颜色映射的二维数组**

code:
[http://nbviewer.jupyter.org/github/matplotlib/AnatomyOfMatplotlib/blob/master/examples/pcolor_example.py](http://nbviewer.jupyter.org/github/matplotlib/AnatomyOfMatplotlib/blob/master/examples/pcolor_example.py)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181123182750763.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
**contour, contourf, clabel: 用于绘制等值线图， 等高线等  **

code:
[http://nbviewer.jupyter.org/github/matplotlib/AnatomyOfMatplotlib/blob/master/examples/contour_example.py](http://nbviewer.jupyter.org/github/matplotlib/AnatomyOfMatplotlib/blob/master/examples/contour_example.py)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181123183012985.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
## 3. 矢量场图

**arrow/quiver/streamplot： 绘制带箭头的矢量场图**

code:
[http://nbviewer.jupyter.org/github/matplotlib/AnatomyOfMatplotlib/blob/master/examples/vector_example.py](http://nbviewer.jupyter.org/github/matplotlib/AnatomyOfMatplotlib/blob/master/examples/vector_example.py)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181123183156206.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
## 4. 描述数据分布情况

**hist/boxplot/violinplot: 用于描述统计分布的图例**

code
[http://nbviewer.jupyter.org/github/matplotlib/AnatomyOfMatplotlib/blob/master/examples/statistical_example.py](http://nbviewer.jupyter.org/github/matplotlib/AnatomyOfMatplotlib/blob/master/examples/statistical_example.py)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181123183415751.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)











