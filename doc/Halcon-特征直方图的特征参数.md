# Halcon-特征直方图的特征参数

2015年06月24日 22:04:57 [龙舞天涯](https://me.csdn.net/u011135902) 阅读数：3416

## **select_shape**

```Halcon
basic
'area' 面积
'row' 行
'column' 列
'width' 宽
'height' 高
'row1' 左上角的行坐标
'column1' 左上角的列坐标
'row2' 右下角的行坐标
'column2' 右下角的列坐标
'circularity' 圆度 （C' = F / (max^2 * pi)   C = min(1,C') F为区域面积，max为最大半径）【circularity】
'compactness' 紧密度（C' = L^2 / (4 F pi) C = max(1,C') L为区域轮廓周长，F为区域面积）【compactness】
'contlength' 区域轮廓总长度【contlength】
'convexity' 凸面（C = Fo / Fc Fc为凸出区域，Fo为原始区域）【convexity】
'rectangularity' 垂直度(计算矩形与归一化矩形的区域差异)【rectangularity】
'ra' 等效椭圆的长轴【elliptic_axis】
'rb' 等效椭圆的短轴
'phi' 等效椭圆的角度
'anisometry' 偏心率（Anisometry = Ra / Rb，Ra, Rb为长轴和短轴，A为面积）【eccentricity】
'bulkiness' 偏心率（Bulkiness = pi Ra Rb / A）
'struct_factor' 偏心率（StructureFactor = Anisometry * Bulkiness - 1）
shape
'outer_radius' 外接圆半径【smallest_circle】
'inner_radius' 内切圆半径【inner_circle】
'inner_width' 内切矩形的宽【inner_rectangle1】
'inner_height' 内切矩形的高
'max_diameter' 轮廓区域点之间的最大距离
'dist_mean' 圆周到中心的平均距离（Distance  = sum(||p - pi||) / F   p为区域中心，pi为轮廓像素，F为轮廓区域）【roundness】
'dist_deviation' 中心到圆周的距离偏差（Sigma^2   = sum(||p - pi|| - Distance)^2 / F）
'roundness' 圆度（Roundness = 1 - Sigma / Distance）
'num_sides' 多边形边的条数（Sides = 1.4111 *(Distance/Sigma)^0.4724）
'orientation' 区域的方向
'connect_num' 组件的数目（connection，ount_obj）【connect_and_holes】
'holes_num' 孔洞数目
'area_holes' 孔洞区域【area_holes】
'euler_number' 孔洞的欧拉数目（euler_number 组件数与孔洞数之差）
'rect2_phi' 外切矩形的方向
'rect2_len1' 外切矩形的半长轴
'rect2_len2' 外切矩形的半短轴
moments
'moments_m11' 几何矩_惯性积（Mij = SUM ( (Z0 - Z)^i (S0 - S)^j )Z0 ，S0 为中心坐标）【moments_region_2nd】
'moments_m20' 行二阶矩
'moments_m02' 列二阶矩
'moments_ia' 主惯性轴（h = (M20 + M02) / 2   Ia = h + sqrt ( h^2 - M20 x M02 + M11^2) ）
'moments_ib' 其他惯性轴（Ib = h - sqrt ( h^2 - M20 x M02 + M11^2)）
'moments_m11_invar' 按比例缩小的矩_惯性积（Mij = 1/F^2 * SUM ( (Z0 - Z)^i (S0 - S)^j )）【moments_region_2nd_invar】
'moments_m20_invar' 二阶矩
'moments_m02_invar' 二阶矩
'moments_phi1' 按比例缩小的相对力矩_二阶矩（Mpq = SUM Z( xi, yi) (xi - x)^p (yi - y)^q）【moments_region_2nd_rel_invar】
'moments_phi2' 二阶矩
'moments_m21' 计算移不变中心力矩_三阶矩【moments_region_3rd】
'moments_m12' 三阶矩
'moments_m03' 三阶矩
'moments_m30' 三阶矩
'moments_m21_invar' 计算尺度不变力矩_三阶矩【moments_region_central】
'moments_m12_invar' 三阶矩
'moments_m03_invar' 三阶矩
'moments_m30_invar' 三阶矩
'moments_i1' 计算中心矩_二阶矩【 moments_region_central】
'moments_i2' 二阶矩
'moments_i3' 二阶矩
'moments_i4' 三阶矩
'moments_psi1' 计算旋转线性移动不变矩_二阶矩【moments_region_central_invar】
'moments_psi2' 二阶矩
'moments_psi3' 二阶矩
'moments_psi4' 二阶矩
```

## **gray_features**

```halcon
basic
'area' 灰度值区域
'row' 重心行索引
'column' 重心列索引
'ra' 灰度值区域长轴【elliptic_axis_gray】
'rb' 灰度值区域短轴
'phi' 灰度值区域方向
'min' 最小灰度值【min_max_gray】
'max' 最大灰度值
texture
'mean' 平均灰度值【intensity】
'deviation' 灰度值偏差
'plane_deviation' 逼近像平面的灰度值偏差【plane_deviation】
'anisotropy' 非均向性【entropy_gray】
'entropy' 熵
'fuzzy_entropy' 模糊熵【fuzzy_perimeter】
'fuzzy_perimeter' 模糊周长
moments
'moments_row' 混合矩（行）【 moments_gray_plane】
'moments_column' 混合矩（列）
'alpha' 近似平面参数
'beta' 近似平面参数
（ 说明：moments_gray_plane计算灰度矩和近似平面
MRow  = sum((r-r')*(Image(r,c)-Mean))/F^2
MCol  = sum((c-c')*(Image(r,c)-Mean))/F^2
Alpha = (MRow*F*m02-m11*MCol*F)/(m20*m02-m11^2)
Beta  = (m20*MCol*F-MRow*F*m11)/(m20*m02-m11^2)
F为一个面，r', c' 中心，m11, m20,  m02为归一化矩阵
Image'(r,c) = Alpha*(r-r')+Beta*(c-c')+Mean ）
```

## **select_shape_xld**

```
basic
'area' 轮廓区域【area_center_xld】
'row' 重心行
'column' 重心列
'width' 轮廓宽
'height' 轮廓高
'row1' rectangle1左上角行坐标【smallest_rectangle1_xld】
'column1' rectangle1左上角列坐标
'row2' rectangle1右下角行坐标
'column2' rectangle1右下角列坐标
shape
'circularity' 圆度【circularity_xld】
'compactness' 紧密度【compactness_xld】
'contlength' 轮廓总长度【length_xld】
'convexity' 凸面【convexity_xld】
'ra' 等效椭圆的长轴【elliptic_axis_xld】
'rb' 等效椭圆的短轴
'phi' 等效椭圆的角度
'anisometry' 偏心率【eccentricity_xld】
'bulkiness' 偏心率
'struct_factor' 偏心率
'outer_radius' 外接圆半径【smallest_circle_xld】
'max_diameter' 轮廓区域点之间的最大距离【diameter_xld】
'orientation' 轮廓区域的方向【orientation_xld】
'rect2_phi' rectangle2方向【smallest_rectangle2_xld】
'rect2_len1' rectangle2长半轴
'rect2_len2' rectangle2短半轴
points
'area_points' 轮廓点区域【area_center_points_xld】
'row_points' 轮廓点重心行
'column_points' 轮廓点重心列
'ra_points' 轮廓点等效椭圆的长轴【elliptic_axis_points_xld】
'rb_points' 轮廓点等效椭圆的短轴
'phi_points' 轮廓点等效椭圆的的角度
'anisometry_points' 轮廓点等效椭圆的偏心率【eccentricity_points_xld】
'orientation_points' 轮廓点区域的方向【 orientation_points_xld】
moments
'moments_m11' 几何矩_惯性积【moments_region_2nd】
'moments_m20' 行二阶矩
'moments_m02' 列二阶矩
'moments_m11_points' 【moments_points_xld】
'moments_m20_points'
'moments_m02_points'
```

