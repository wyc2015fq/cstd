# [zt]摄像机标定(Camera calibration)笔记 - Grandyang - 博客园







# [[zt]摄像机标定(Camera calibration)笔记](https://www.cnblogs.com/grandyang/p/4027894.html)








http://www.cnblogs.com/mfryf/archive/2012/03/31/2426324.html



一 作用
建立3D到2D的映射关系，一旦标定后，对于一个摄像机内部参数K(光心焦距变形参数等，简化的情况是只有f错切=0,变比=1，光心位置简单假设为图像中心)，参数已知，那么根据2D投影，就可以估计出R t；
空间3D点所在的线就确定了，根据多视图（多视图可以是运动图像）可以重建3D。
如果场景已知，则可以把场景中的虚拟物体投影到2D图像平面（DLT，只要知道M即可）。或者根据世界坐标与摄像机坐标的相对关系，R,T，直接在Wc位置渲染3D图形，这是AR的应用。
因为是离线的，所以可以用手工定位点的方法。

二 方法
1 Direct linear transformation (DLT) method 
2 A classical
 approach is "Roger Y. Tsai Algorithm".It is a 2-stage algorithm, 
calculating the pose (3D Orientation, and x-axis and y-axis translation)
 in first stage. In second

stage it computes the focal length, distortion coefficients and the z-axis translation. 
3
 Zhengyou Zhang's "a flexible new technique for camera calibration" 
based on a planar chess board. It is based on constrains on homography.


4个空间：世界，摄像机(镜头所在位置)，图像（原点到镜头中心位置距离f），图像仿射空间（最终成像空间），这些空间原点在不同应用场合可以假设重合。
具体说来，前两个空间是R t关系，6个变量搞定；第二到第三空间用相似三角形投影到平面上，如果没有变形，光心在图像中心上，到这里为止其实就够了，K就一个参数。

K是摄像机坐标系到图像平面的转换函数，叫内部参数(intrinsic parameter)。标定就是求上三角矩阵K的5个参数的过程。
用齐次（Homogeneous）坐标表示这个关系: （实际二维点是[u,v]'=[U/W , V/W]'）

       [U]    [a b -u0][-fxc/zc]   [-fa -fb -u0][xc/zc]        [xc/zc]
ub=[V]  =[0 c -v0][-fyc/zc ]= [0   -fc -v0][yc/zc]= K * [yc/zc]
       [W]   [0 0   1][  1    ]       [0   0     1]  [ 1   ]           [ 1   ]

世界坐标系到图像坐标系的转换有平移矢量t和旋转R矩阵组成，R,t是外部参数(extrinsic parameters)，表示摄像机姿态，包含6个自由度,R t 各3个。Xc=[xc yc zc]'= R(Xw-t) ， ' 表示转置。

把两个合起来 
zc*ub=zc*[U V W]'= K*R(Xw-t)

把三维点Xw表示成齐次坐标，Xwb=[Xw,1]',则
       [U] 
ub=[V] = [K*R |-K*R*t][Xw 1]' =M*[Xw 1]' = M* Xwb
       [W]  ([K*R |-K*R*t]也可写成K[R t']串联的形式)

3×4矩阵M称为投影矩阵，这样三维点P到二维平面的投影就由上式的线性变换表达了。摄像机标定就是求K，K是与R,t一起耦合在M中的。

常用的方法是已知场景3D点和图像二维点，求M。如Tsai grid方法。   
根据至少6个对应点，建立方程组:
alpha*[u v 1]' = M * [x y z 1]'
写成2n×12矩阵 l*mij=0的形式(叫Direct Linear Transformation (DLT))：

[P1 | 0 | -u*P1]  [m11]
[0  |P1 | -v*P1]* [m12] = 0.   
[   ...                 ]   ...
[   ...                  ]   [m34]      (矢量Pi表示第i个三维点[Xi,Yi,Zi,1]')
方程解是l'*l的最小eigenvalue对应的eigenvector,|m|2=1，很多书上提到用SVD分解，l=UDV'，V就是特征矢量矩阵。
其实，l'*l=VDU'UDV'=VDDV',V'=inv(V),所以l'*l*V=VD^2，这就是特征矢量的求法。很明显，l的singular vector是l'*l的特征矢量的平方根。

（PCA,K-L变换 
和SVD之间的关系。边肇琪《模式识别》提到人脸识别降维的例子时，讲到如果样本数小于特征维数，要用SVD提取特征向量。在这里协方差矩阵为l'*l，
是12×12的矩阵，l矩阵的短边为9，而且计算M(Homography矩阵)时，点对要经过Normalize,完全可以参照人脸的例子。SVD总是
和最小二乘，伪逆Ax=b-》A'*Ax=A'b-》x=inv(A'*A)*A'*b，联系在一起。）


M=[K*R |-K*R*t]=[A|b],A = K*R,用QR分解（或SVD）将A分解为上三角矩阵K和单位正交阵R.
实际上M只有11个参数（Rt6个，K5个）。如果把摄像头畸变也考虑进去，mij变成16个值,则需要更多的对应点。
如果3D点在一个平面上,l is singular,那么退化成9个参数。

已知场景3D点pattern拍一张照片，点分布在2到3个pattern平面上。
以上参考 milan sonka的Image processing一书。

实际上现在大家都是用张正友的方法，将网格点构成的pattern预先打印在一张纸上,对它拍摄不同方向照片（至少两张）。
网上有matlab calibration包，OpenCV也有对应的实现，可以同时求摄像机的内部参数和外部参数(姿态估计)。
具体用到梯度下降方法，尽管R有3*3=9个数，但只有3个自由度，用rodrigues formula转为3维矢量，计算Jacobi更方便。

具体参考：
1 Rodrigues' Formula
[http://www.cs.berkeley.edu/~ug/slide/pipeline/assignments/as5/rotation.html](http://www.cs.berkeley.edu/%7Eug/slide/pipeline/assignments/as5/rotation.html)
2 OpenCV函数说明
[http://opencv.willowgarage.com/documentation/camera_calibration_and_3d_reconstruction.html](http://opencv.willowgarage.com/documentation/camera_calibration_and_3d_reconstruction.html)
3 Camera Calibration Toolbox for Matlab
[http://www.vision.caltech.edu/bouguetj/calib_doc/](http://www.vision.caltech.edu/bouguetj/calib_doc/)

--------------------------------------------------
如果只有平面4个点，如何计算构造DLT，求Homography矩阵[mij]？

1 左边8×9矩阵A，m为9个数。rank 8, i.e. they have a one-dimensional null-space. 
The solution can be determined from the null-space of A.

svd分解 A=UDV',D为singular valuies，mij的解为V中最小的singular value对应列。

2 退化成一个平面到另一个平面的透视变换

u = (L1*X + L2*Y + L3)/(L7*X + L8*Y + 1) ;
v = (L4*X + L5*X + L6)/(L7*X + L8*Y + 1) ;

--------------------------------------------------
姿态估计:
基本思想是用3D点align到2D点上，使误差最小。已知3D场景点，估计摄像机姿态，或者3D场景点关于摄像机坐标系的相对位置。
比如，marker可以构成世界坐标系，4个点坐标已知。它们相对于摄像机空间的坐标（Xa,Ya,Za）求出，xy轴可以求出，用右手法则z轴可以求出。
这样世界坐标系的3根轴已知，就可以渲染3D场景了。（AR应用）
具体参见Hybrid camera pose estimation combining square fiducials localisation technique and orthogonal iteration algorithm
或 OI算法 都是类似梯度下降的迭代算法，可以利用前一帧的参数作为初值，实时估计R和T。
--------------------------------------------------
重建3D场景：
对于多个未经标定的摄像机，也可以根据在每个图像中的2D点位置重建3D点
x1=P1X
x2=P2X
。。。。
Pi可以由已知场景点实时更新，xi可以根据运动估计更新，这样可以计算X。

具体方法参见 Multiple View Geometry in Computer Vision 一书。





本文引用地址：[http://blog.sciencenet.cn/home.php?mod=space&uid=465130&do=blog&id=365366](http://blog.sciencenet.cn/home.php?mod=space&uid=465130&do=blog&id=365366)













