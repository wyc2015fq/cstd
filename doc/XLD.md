# XLD

 

## halcon中什么是xld?

　　xld(eXtended Line Descriptions) 扩展的线性描述，它不是基于像素的，人们称它是亚像素，只不过比像素更精确罢了，可以精确到像素内部的一种描述。

　　而image和region这些数据结构都是像素级别的。

　　亚像素精度数据可以通过亚像素阈值分割或者亚像素边缘提取来获得，这些处理得到的结果就是用亚像素轮廓来表示。

　　上面提到的亚像素轮廓其实在halcon中指的就是xld，它可以表示直线或多边形，是一组有序的控制点集合，控制点顺序用来说明彼此相连的关系。

　　当然亚像素轮廓可以分为两种：

　　1.闭轮廓：首尾相交

　　2.开轮廓：首尾不相交

　　我们可以通过如下算子得到xld:

　　threshold_sub_pix(Image:Boreder:Threshold:):提取xld并不是沿着像素与像素边界交界的地方，而是halcon经过某种计算得出的位置（一种描述）。其中Threshold为提取xld的阈值，灰度值以阈值为界提取xld。

　　xld的特征部分和region有很多相同的部分：

　　1.基础特征：xld面积，中心，宽高，左上角及右下角坐标。

　　2.形状特征：圆度，紧密度，长度，矩形度，凸性，偏心率，蓬松度（bulkiness)，外接圆半径，外接矩形的方向及两边长度等。

　　3.云点特征：云点面积，中心，等效椭圆半轴及角度，云点方向等。

　　4.几何特征：二阶矩等。

 

 

　　选取特定xld:

　　使用算子：

　　select_shape_xld(XLD:SelectedXLD:Features,Operation,Min,Max:)

　　作用：选择特定形状特征要求的xld轮廓或多边形

　　XLD：要提取的XLD

　　SelectedXLD：提取到的XLD

　　Features：提取XLD的特征依据

　　Operation：特征之间的逻辑关系（and or)

　　Min，Max：特征值的要求范围

 

　　Select_Contours_xld(Contours:SelectedContours:Features,Min,Max,Min2,Max2:)

　　作用：选择多种特征要求的XLD轮廓 （如长度  开闭  方向 等特征，不支持多边形）

　　Contours：要提取的XLD轮廓

　　SelectedContours：提取到的XLD轮廓

　　Features：提取XLD轮廓的依据（筛选）

　　Min,Max,Min2,Max2：特征值的范围

　　

　　area_center_xld（XLD:::Area,Row,Column,PointOrder)

　　作用：计算xld的面积和中心位置

 

　　下面来看看XLD的合并操作：

　　union_collinear_contours_xld：合并在同一直线的XLD

　　union_cocircular_contours_xld：合并在同圆的XLD

　　union_adjacent_contours_xld：合并邻近的XLD

　　这些算子以后会详细讲解。

 

　　再来看看XLD的合并操作：

　　把XLD转换成其他规则形状的XLD，相应说明及算子和region的很类似：

　　1.convex hull

　　2.outer_circle

　　3.retangle1

　　4.retangle2

　　这四种类型转换使用同一个算子：

　　shape_trans_xld（XLD:XLDTrans:Type:)

　　作用：转换XLD的形状，其中Type为转换的类型如上面的四种类型

 

　　接下来看看XLD的分割操作：

　　算子：

　　segment_contours_xld（Contours:ContoursSplit:Mode,SmoothCont,Max,LineDist1,MaxLineDist2:)

　　具体操作以后再说。

 

　　然后就是XLD的拟合操作：（fit_***_contour_xld)

　　具体算子如下：

　　fit_line_contour_xld：拟合直线

　　fit_circle_contour_xld:拟合圆

　　fit_ellipse_contour_xld：拟合椭圆

　　fit_retangle2_contour_xld：拟合矩形 