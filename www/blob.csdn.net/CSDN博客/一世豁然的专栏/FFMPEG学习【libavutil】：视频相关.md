# FFMPEG学习【libavutil】：视频相关 - 一世豁然的专栏 - CSDN博客





2017年07月24日 09:42:44[一世豁然](https://me.csdn.net/Explorer_day)阅读数：315








一、显示变换矩阵函数

显示变换矩阵指定应适用于视频帧以进行正确预处理的仿射变换。


它与存储在ISO / IEC 14496-12容器格式中的矩阵兼容。


数据是一个3x3矩阵，表示为9元素数组：




```cpp
| a b u |
(a, b, u, c, d, v, x, y, w) -> | c d v |
                               | x y w |
```
所有数字以原始字节顺序存储，除了u，v和w之外的16.16个定点值，存储为2.30个定点值。

变换将源（预转换）帧中的点（p，q）映射到目标（后变换）帧中的点（p'，q'），如下所示：



```cpp
| a b u |
(p, q, 1) . | c d v | = z * (p', q', 1)
            | x y w |
```
转换也可以更明确地写在组件中，如下所示：



```cpp
p' = (a * p + c * q + x) / z;
q' = (b * p + d * q + y) / z;
z  =  u * p + v * q + w
```




一）、函数


|double|[av_display_rotation_get](http://ffmpeg.org/doxygen/trunk/group__lavu__video__display.html#gaac2ea94d3f66496c758349450b5b0217) (const [int32_t](http://ffmpeg.org/doxygen/trunk/libswresample_2audioconvert_8c.html#afd12020da5a235dfcf0c3c748fb5baed) matrix[9])|
|----|----|

提取转换矩阵的旋转分量。

参数：matrix：转换矩阵

返回：变形使逆时针旋转框架的角度（以度为单位）。 如果矩阵是单数，角度将在[-180.0,180.0]或NaN的范围内。

注意：浮点数本质上是不准确的，所以建议在使用前将返回值舍入到最接近的整数。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_display_rotation_set](http://ffmpeg.org/doxygen/trunk/group__lavu__video__display.html#ga5964303bfe085ad33683bc2454768d4a) ([int32_t](http://ffmpeg.org/doxygen/trunk/libswresample_2audioconvert_8c.html#afd12020da5a235dfcf0c3c748fb5baed) matrix[9], double angle)|
|----|----|

初始化描述纯逆时针旋转指定角度（以度为单位）的变换矩阵。

参数：matrix：分配的变换矩阵（将被该函数完全覆盖）

angle：旋转角度以度为单位。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_display_matrix_flip](http://ffmpeg.org/doxygen/trunk/group__lavu__video__display.html#gadd236b7ced34637aa9f0a9bceaeac5c3) ([int32_t](http://ffmpeg.org/doxygen/trunk/libswresample_2audioconvert_8c.html#afd12020da5a235dfcf0c3c748fb5baed) matrix[9], [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) hflip, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) vflip)|
|----|----|

水平和/或垂直地翻转输入矩阵。

参数：matrix：一个分配的变换矩阵

hflip：矩阵是否应水平翻转

vflip：矩阵是否应垂直翻转







二、球形视频映射

一个球面视频文件包含需要映射到球体上的曲面。


注意：结构必须分配av_spherical_alloc（），其大小不是公共ABI的一部分。


根据帧的转换方式，在映射和显示视频之前，需要应用不同的失真变换或表面重构功能。





一）、数据结构

此结构描述如何处理球形视频，概述有关投影，初始布局和任何其他视图修改器的信息。






|struct|[AVSphericalMapping](http://ffmpeg.org/doxygen/trunk/structAVSphericalMapping.html){enum AVSphericalProjection projection；//投影类型。uint32_t padding；//从每个立方体面的边缘进行焊接的像素数。**初始方位**在视频帧映射到场后，场中描述了应用于球体的附加旋转。球体围绕观察者转动，观察者保持静止。 变换的顺序总是偏航，其次是俯仰，最后是滚动。坐标系与OpenGL中定义的坐标系匹配，其中向量向量（z）离开屏幕，并且等效于R = r_y（yaw）* r_x（pitch）* r_z（roll）的旋转矩阵。正偏航将观看者前面的球体部分向右旋转。 正音调将观看者前方的球体部分向上旋转。 正面滚动将观看者前面的球体部分倾斜到观看者的右侧。这些值被导出为16.16个固定点。见这个等角投影为例：```Yaw    -180           0           180  90 +-------------+-------------+  180     |             |             |                  upP    |             |             |                 y|    forwardi    |             ^             |                  |   /zt  0 +-------------X-------------+    0 Roll        |  /c    |             |             |                  | /h    |             |             |                 0|/_____right     |             |             |                        x -90 +-------------+-------------+ -180X - the default camera center^ - the default up vector```int32_t yaw；//  向上旋转[-180,180]。int32_t pitch；//绕右矢量旋转[-90,90]。int32_t roll；//绕前向矢量旋转[-180,180]。**边界矩形**这些字段指示当前图块的位置，以及相对于原始曲面映射的位置。 它们导出为0.32个固定点，并可以通过av_spherical_bounds（）转换为经典像素值。```+----------------+----------+|                |bound_top ||            +--------+     || bound_left |tile    |     |+<---------->|        |<--->+bound_right|            +--------+     ||                |          ||    bound_bottom|          |+----------------+----------+```如果需要，可以通过将当前流或帧大小添加到相关边界来导出原始视频表面尺寸，如以下示例所示：```cpporiginal_width  = tile->width  + bound_left + bound_right;original_height = tile->height + bound_top  + bound_bottom;```注意：这些值仅适用于平铺等角投影类型（AV_SPHERICAL_EQUIRECTANGULAR_TILE），并且在所有其他情况下都应该被忽略。uint32_t bound_left；//  距离左边缘的距离。uint32_t bound_top；// 距离上边缘的距离。uint32_t bound_right；//距离右边缘的距离。uint32_t bound_bottom；// 与底边距离。}|
|----|----|





二）、枚举


|enum|[AVSphericalProjection](http://ffmpeg.org/doxygen/trunk/group__lavu__video__spherical.html#ga689eeb8cf9b453f3619f3cf626e03a08) { [AV_SPHERICAL_EQUIRECTANGULAR](http://ffmpeg.org/doxygen/trunk/group__lavu__video__spherical.html#gga689eeb8cf9b453f3619f3cf626e03a08a55efbae149ee7a6e327a66be7061d146), [AV_SPHERICAL_CUBEMAP](http://ffmpeg.org/doxygen/trunk/group__lavu__video__spherical.html#gga689eeb8cf9b453f3619f3cf626e03a08aa0a8bc866354ecb619493db502c4d103), [AV_SPHERICAL_EQUIRECTANGULAR_TILE](http://ffmpeg.org/doxygen/trunk/group__lavu__video__spherical.html#gga689eeb8cf9b453f3619f3cf626e03a08a768257247d4090b959ee1678f67bc9d3) }|
|----|----|





三）、函数


|[AVSphericalMapping](http://ffmpeg.org/doxygen/trunk/structAVSphericalMapping.html) *|[av_spherical_alloc](http://ffmpeg.org/doxygen/trunk/group__lavu__video__spherical.html#gab366393b8836ac987df51546183386ea) (size_t *[size](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e))|
|----|----|

分配AVSphericalVideo结构，并将其字段初始化为默认值。

返回：新分配的结构或NULL失败





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_spherical_tile_bounds](http://ffmpeg.org/doxygen/trunk/group__lavu__video__spherical.html#ga9c9a5f996f41bd0fe6f4c9908a70f98e) (const [AVSphericalMapping](http://ffmpeg.org/doxygen/trunk/structAVSphericalMapping.html) *[map](http://ffmpeg.org/doxygen/trunk/hwcontext__vdpau_8c.html#a2996cc16b36e13c9010705bad768e866), size_t [width](http://ffmpeg.org/doxygen/trunk/tests_2utils_8c.html#a2474a5474cbff19523a51eb1de01cda4), size_t [height](http://ffmpeg.org/doxygen/trunk/tests_2utils_8c.html#ad12fc34ce789bce6c8a05d8a17138534), size_t *left, size_t *top, size_t *right, size_t *bottom)|
|----|----|

将AVSphericalVideo的边界字段从0.32固定点转换为像素。

参数：map:从AVSphericalVideo映射读取绑定值。

   width:当前帧或流的宽度。

   height:当前帧或流的高度。

   left:从左边缘的像素。

   top:上边缘的像素。

   right:像素从右边缘。

   bottom:像素从底边。





|const char *|[av_spherical_projection_name](http://ffmpeg.org/doxygen/trunk/group__lavu__video__spherical.html#ga80ddcd4147c89a228fd7e4cc8d50158e) (enum [AVSphericalProjection](http://ffmpeg.org/doxygen/trunk/group__lavu__video__spherical.html#ga689eeb8cf9b453f3619f3cf626e03a08) projection)|
|----|----|

提供给定的AVSphericalProjection的可读名称。

参数：projection：输入AVSphericalProjection。

返回：AVSphericalProjection的名称，或“未知”。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_spherical_from_name](http://ffmpeg.org/doxygen/trunk/group__lavu__video__spherical.html#ga539173e8b22176710e1e0b728e4bd22f) (const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c))|
|----|----|

获取AVSphericalProjection形式的人类可读的名称。

参数：name：输入字符串。

返回：AVSphericalProjection值，如果没有找到则为-1。







三、Stereo3D类型和功能

立体视频文件由嵌入在单个帧中的多个视图组成，通常描述场景的两个视图。





一）、数据结构

立体声3D类型：此结构描述了如何在单个视频表面中打包两个视频，并根据需要提供其他信息。






|struct|[AVStereo3D](http://ffmpeg.org/doxygen/trunk/structAVStereo3D.html){enum AVStereo3DType type；//视频中的视图如何打包。int flags；//有关框架包装的附加信息。}|
|----|----|


二）、宏


|#define|[AV_STEREO3D_FLAG_INVERT](http://ffmpeg.org/doxygen/trunk/group__lavu__video__stereo3d.html#ga5cca86880e9be72c89bca1c7a41eec99)   (1 << 0)|
|----|----|

倒置视图，右/底表示左视图。




三）、枚举


|enum|[AVStereo3DType](http://ffmpeg.org/doxygen/trunk/group__lavu__video__stereo3d.html#ga12fe9672eeaeb8980960faa96d2fcee6) { [AV_STEREO3D_2D](http://ffmpeg.org/doxygen/trunk/group__lavu__video__stereo3d.html#gga12fe9672eeaeb8980960faa96d2fcee6a276701c16077443c9526dd28da8001a7), [AV_STEREO3D_SIDEBYSIDE](http://ffmpeg.org/doxygen/trunk/group__lavu__video__stereo3d.html#gga12fe9672eeaeb8980960faa96d2fcee6aaa1ee993fc27f99269c536c51b0b5f3c), [AV_STEREO3D_TOPBOTTOM](http://ffmpeg.org/doxygen/trunk/group__lavu__video__stereo3d.html#gga12fe9672eeaeb8980960faa96d2fcee6a4cf21029b6e5aa28bf53fbd9740443c9), [AV_STEREO3D_FRAMESEQUENCE](http://ffmpeg.org/doxygen/trunk/group__lavu__video__stereo3d.html#gga12fe9672eeaeb8980960faa96d2fcee6aa23e3e6b40107100e62fa16ea536aa07), [AV_STEREO3D_CHECKERBOARD](http://ffmpeg.org/doxygen/trunk/group__lavu__video__stereo3d.html#gga12fe9672eeaeb8980960faa96d2fcee6a29a4a89f56c46999741e394e2019b221), [AV_STEREO3D_SIDEBYSIDE_QUINCUNX](http://ffmpeg.org/doxygen/trunk/group__lavu__video__stereo3d.html#gga12fe9672eeaeb8980960faa96d2fcee6a6176782f4b7b21776faf6738baa520e2), [AV_STEREO3D_LINES](http://ffmpeg.org/doxygen/trunk/group__lavu__video__stereo3d.html#gga12fe9672eeaeb8980960faa96d2fcee6a23d62b9a67d4a99d745eb9bebcfd2b3e), [AV_STEREO3D_COLUMNS](http://ffmpeg.org/doxygen/trunk/group__lavu__video__stereo3d.html#gga12fe9672eeaeb8980960faa96d2fcee6a1ea7966fe2696b13348a3cf0544e3caa)}|
|----|----|





四）、函数


|[AVStereo3D](http://ffmpeg.org/doxygen/trunk/structAVStereo3D.html) *|[av_stereo3d_alloc](http://ffmpeg.org/doxygen/trunk/group__lavu__video__stereo3d.html#ga4c647cd938e0919c6b8cd31244343561) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

分配AVStereo3D结构并将其字段设置为默认值。

结果可以使用av_freep（）来释放。


返回：AVStereo3D填充默认值或失败时为NULL。





|[AVStereo3D](http://ffmpeg.org/doxygen/trunk/structAVStereo3D.html) *|[av_stereo3d_create_side_data](http://ffmpeg.org/doxygen/trunk/group__lavu__video__stereo3d.html#ga9401b2d2cbd0c0988b1cdde6c88f6e6b) ([AVFrame](http://ffmpeg.org/doxygen/trunk/structAVFrame.html) *[frame](http://ffmpeg.org/doxygen/trunk/extract__mvs_8c.html#ad7d33d579a8d4241a5e643e39287a209))|
|----|----|

分配完整的AVFrameSideData并将其添加到框架中。

参数：frame：添加哪一侧数据的帧。

返回：AVStereo3D结构由调用者填充。





|const char *|[av_stereo3d_type_name](http://ffmpeg.org/doxygen/trunk/group__lavu__video__stereo3d.html#gabc39f81856b3559a7ba099a1bcd38303) (unsigned [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[type](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a7d05960f4f1c1b11f3177dc963a45d86))|
|----|----|

提供一个给定的stereo3d类型的人类可读的名称。

参数：type：输入stereo3d类型值。

返回：stereo3d值的名称，或“未知”。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_stereo3d_from_name](http://ffmpeg.org/doxygen/trunk/group__lavu__video__stereo3d.html#ga6a48f4c80b275d74d1e4ba760802a070) (const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c))|
|----|----|

获取AVStereo3DType形式可读的名称。

参数：name：输入字符

返回：AVStereo3DType值，如果没有找到则为-1。



