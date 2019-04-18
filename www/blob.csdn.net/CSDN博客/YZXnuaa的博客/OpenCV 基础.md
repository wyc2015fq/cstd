# OpenCV 基础 - YZXnuaa的博客 - CSDN博客
2018年01月30日 10:34:03[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：227
## 图像特征：
1.边界
2.角点（兴趣点）
3.斑点（兴趣区域）
角点是图像的一个局部特征，常用的有harris角点，其算法是一种直接基于灰度图像的，稳定性高，尤其对L型角点检测精度高，但由于采用了高斯滤波，运算速度相对较慢，角点信息有丢失和位置偏移的现象，而且角点提取有聚簇现象。具体在OpenCV中的实现就是使用函数cornerHarris。
除了Harris角点检测，还有Shi-Tomasi角点检测，goodFeaturesToTrack角点检测，将找到的点再用FindCornerSubPix()来找出强特征点。也可以自己制作角点检测的函数，需要用到cornerMinEigenVal函数和minMaxLoc函数，最后的特征点选取，判断条件要根据自己的情况编辑。如果对特征点，角点的精度要求更高，可以用cornerSubPix函数将角点定位到子像素。
## [OpenCV仿射变换、投影变换的](http://blog.csdn.net/godenlove007/article/details/9364971)重要函数
estimateRigidTransform()：计算多个二维点对或者图像之间的最优仿射变换矩阵 （2行x3列），H可以是部分自由度，比如各向一致的切变。
getAffineTransform()：计算3个二维点对之间的仿射变换矩阵H（2行x3列），自由度为6.
warpAffine()：对输入图像进行仿射变换
findHomography： 计算多个二维点对之间的最优单映射变换矩阵 H（3行x3列） ，使用最小均方误差或者RANSAC方法 。
getPerspectiveTransform()：计算4个二维点对之间的透射变换矩阵 H（3行x3列）
warpPerspective()： 对输入图像进行透射变换
perspectiveTransform()：对二维或者三维矢量进行透射变换，也就是对输入二维坐标点或者三维坐标点进行投射变换。
estimateAffine3D：计算多个三维点对之间的最优三维仿射变换矩阵H （3行x4列）
transform()：对输入的N维矢量进行变换，可用于进行仿射变换、图像色彩变换.
findFundamentalMat：计算多个点对之间的基矩阵H。
cvStereoCalibrate()：中T类型要求了3*1，对与其他形参float和double都支持
cvStereoRectigy()：只支持double类型
cvStereoRectifyUncalibrated()：立体校正算法Hartley算法效果和F矩阵及图像数量有关，
ps：
【如果用cvStereoCalibrate()函数计算处理的F矩阵效果和Bouguet算法（cvStereoRectigy()）效果一样】
【如果用cvFindFundamentalMat()函数计算F矩阵，没有Bougut算法好】
【用Hartley算法（cvStereoRectifyUncalibrated()）校正时，别忘了实现要用cvUndistortPoints()去除相机畸变，Bouguet算法（cvStereoRectigy()）没有这个要求，实际上它在函数内部校正了相机的畸变。】
## Q&A：
问题1：如何计算3个二维点对之间的仿射变换矩阵？
答：使用getAffineTransform()。
问题2：如何计算多个二维点对之间的仿射变换矩阵（使用误差最小准则 ）？
答：使用estimateRigidTransform()。
问题3：如何计算多个二维点对之间的投影变换矩阵（使用误差最小准则 ）
答：findHomography()。
问题4：如何计算4个二维点对之间的透射变换？
答：使用getPerspectiveTransform()。
问题5：如何计算多个三维点对之间的仿射变换？
答：使用estimateAffine3D。
问题6：如何对输入图像进行仿射变换？
答：使用warpAffine()。
问题7：如何对输入图像进行透射变换？
答：使用perspectiveTransform()。
问题8：如何对输入的二维点对进行仿射变换？
答：使用transform()。
问题9：如何对输入的三维点对进行投影变换？
答：使用perspectiveTransform()。
