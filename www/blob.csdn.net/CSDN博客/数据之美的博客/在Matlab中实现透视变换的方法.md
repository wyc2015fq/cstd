# 在Matlab中实现透视变换的方法 - 数据之美的博客 - CSDN博客
2017年03月26日 16:29:38[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：1569
个人分类：[matlab																[图像配准](https://blog.csdn.net/oppo62258801/article/category/6800940)](https://blog.csdn.net/oppo62258801/article/category/6798193)
透视变换在两幅图像中进行对应非常有用，看下面的两幅图
第一幅图是源图像，图中有五个点，分别是四个顶点和中间的一个点（对应目标图中的中心点）
![](http://hi.csdn.net/attachment/201106/11/0_13077801097Luu.gif)
第二幅图是目标图，包括与源图像中的五个点对应的五个点
![](http://hi.csdn.net/attachment/201106/11/0_1307780123HCxR.gif)
下面就要用Matlab根据四个顶点求透视变换矩阵，然后将源图像中中间的那个点对应到目标图像中去。
源图像中的点的坐标矩阵为：
B =
     1     1
     1   480
   640    97
   640   384
目标图像中对应的顶点坐标为：
A =
     1     1
     1   480
   640     1
   640   480
用以下方法求得变换矩阵：
TForm = cp2tform(B,A,'projective')
函数返回的TForm是一个结构类型，说明如下：
       ndims_in: 2
      ndims_out: 2
    forward_fcn: @fwd_projective
    inverse_fcn: @inv_projective
          tdata: [1x1 struct]
其中：
ndims_in 和 ndims_out是输入输出数据的维数，因为是平面中坐标的变换，所以都是2位的坐标；
forward_fcn 和 inverse_fcn 分别是正变换和反变换函数。
求得TForm之后，用如下语句进行变换：
round(tformfwd(TForm,[400 240]))
其中[400 240]是在源图像中中间那个点的坐标，tformfwd函数利用刚才的 TForm 进行变换，round再对求得的结果取整，上述语句的输出结果为：
ans =
   320   240
对应到目标图像中正好是图像的中点。
这里的源图像是对目标图像应用透视变换得来的，所以我才知道那个点对应过去是中点，用上述方法可以求得平面内任意一点的对应点坐标。
2011-06-11  16:18:38
