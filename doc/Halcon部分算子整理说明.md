# Halcon部分算子整理说明

2017年12月13日 11:18:27 [文学长](https://me.csdn.net/qq_24007601) 阅读数：3904



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/qq_24007601/article/details/78789805

### dev_close_window()

关闭当前激活的窗口

 

### read_image( : Image : FileName : )

读取图像，可以支持多种格式，比如TIFF，PNG，JPEG-XR，JPEG-2000等，还支持一次性读取多个图像。

Image：输出，读取完后在halcon所存放的变量名

FileName：图片路径，可以是多个路径，可以是绝对路径或者相对路径，还可以省略扩展名

 

示例：

\* Reading an image:

 read_image(Image,'mreut')

 

\* Reading 3 images into an image array:

 read_image(Images,['ic0','ic1','ic2'])

### stop()

停止程序（等待用户继续运行）

 

### get_image_size(Image : : : Width, Height)

获取图像的尺寸

Image：要获取尺寸的图像

Width：输出，图像的宽度

Height：输出，图像的高度

 

### dev_open_window( : : Row, Column, Width, Height,Background : WindowHandle)

打开一个新的图像窗口

Row：图像窗口左上角的起始行，默认0。（好像没什么用）

Column:图像窗口左上角的起始列，默认0.（好像没什么用）

Width：图像窗口的宽度，默认256

Height：图像窗口的高度，默认256

Background:新窗口的背景颜色，默认黑色（black）

WindowHandle：窗口的识别Id

 

### dev_display(Object : : : )

将图像显示到当前的图像窗口上

Object：要显示的图像对象

 

### dev_set_draw( : : DrawMode : )

设置Region的显示形式

DrawMode：区域的显示形式，默认'fill'，可选'fill','margin'，fill表示显示实心区域，margin表示只显示区域的外边界

 

### dev_set_color( : : ColorName : )

设置输出颜色

ColorName：颜色名称，默认’white’，可选值（格式）：'white','black', 'gray', 'red', 'green', 'blue', '#003075', '#e53019', '#ffb529'

 

### disp_message( : : WindowHandle, String, CoordSystem, Row,Column, Color, Box : )

输出一段文字信息

WindowHandle：要显示文字的窗口handle

String：要显示的文字信息，会显示在一个行里

CoordSystem：使用的坐标系，默认window，可选'window','image'

Row，Column：文字坐标，默认12

Color：文字颜色，默认'black'，可选'','black', 'blue', 'yellow', 'red', 'green', 'cyan', 'magenta', 'forest green','lime green', 'coral', 'slate blue'

Box：是否包含在一个背景框内，默认'true'，可选'true','false'

 

## 基础语法

### If(‘condition’) … else … endif

条件判断

### While(‘condition’) … endwhile

While循环

 

### for Index := ‘start‘ to ‘max’ by ‘step’ … endfor

for循环

# 图像处理部分

## 基础操作

## 图像转化

### convert_image_type(Image : ImageConverted : NewType : )

转换图像类型

Image : 要转化的图像

ImageConverted : 输出，转化后的图像

NewType :要转化的图像类型，详见Halcon的图像像素类型

 

### decompose3(MultiChannelImage : Image1, Image2, Image3 : :)

把一个RGB图像转化为3个单通道的图像

MultiChannelImage：输入的多通道图像（应该是3通道？）

Image1, Image2, Image3:输出，转化后的单通道图像，1是red，2是green，3是blue

 

### rgb1_to_gray(RGBImage : GrayImage : : )

把一张RGB图像转化为灰度图像

RGBImage:输入的RGB图像

GrayImage：输出，得到的灰度图像

## 腐蚀膨胀

### gen_disc_se( : SE : Type, Width, Height, Smax : )

创建一个椭圆形结构元素，用于图像的腐蚀膨胀

SE：输出，生成后的结构元素，图像类型

Type：结构元素的图像像素类型，默认是’byte’，可选：’byte’，’uint2’，’real’，详见Halcon的图像像素类型

 

### gray_erosion(Image, SE : ImageErosion : : )

使用结构元素对图像做腐蚀操作（结构元素可以是gen_disc_se的输出）

Image：要做腐蚀操作的图像

SE：结构元素

ImageErosion：输出，腐蚀后的图像

 

### gray_dilation(Image, SE : ImageDilation : : )

使用结构元素对图像做膨胀操作（结构元素可以是gen_disc_se的输出）

Image：要做膨胀操作的图像

SE：结构元素

ImageDilation：输出，膨胀后的图像

 

# 区域处理部分

## 基础操作

### threshold(Image :Region : MinGray, MaxGray : )

将图像根据灰度值二值化

Image：需要进行二值化的图像

Region：输出，二值化后的结果区域

MinGray：最小灰度值，默认128

MaxGray：最大灰度值，默认255，必须大于MinGray

 

### connection(Region : ConnectedRegions : : )

计算出区域中连接的部分

Region：要计算的区域

ConnectedRegions：输出，计算后的Region数组，相连的部分将被划分为一个区域，会以不同颜色加以区分

 

### select_shape(Regions : SelectedRegions : Features,Operation, Min, Max : )

从一个区域数组中选择出符合某特征条件的区域

Regions：输入的区域数组

SelectedRegions：输出，选出的符合某些特征条件的区域数组

Features：条件特征，详见区域特征说明

Operation：对于符合特征的区域的连接操作，可以是And或者Or

Min：特征的最小值

Max：特征的最大值

 

示例：

read_image(Image,'monkey')

threshold(Image,S1,160,255)

connection(S1,S2)

select_shape(S2,Eyes,['area','anisometry'],'and',[500,1.0],[50000,1.7])

disp_region(Eyes,WindowHandle)

 

### difference(Region, Sub : RegionDifference : : )

计算两个区域的差

Region：需要处理的区域

Sub：被减去的区域

RegionDifference：输出，计算后的结果。

 

示例：

\* provides the region X without the pointsin Y

difference(X,Y,RegionDifference)

 

 

 

 

 

 

 

 

### union1(Region : RegionUnion : : )

联合有连接性质的区域

Region：输入的区域（比如connection过的区域数组）

RegionUnion：输出，合并后的区域

 

### union2(Region1, Region2 : RegionUnion : : )

把两个区域合并成一个区域

Region1：要合并的第一个区域

Region2：要合并的第二个区域

RegionUnion：输出，合并后的区域

 

### complement(Region : RegionComplement : : )

计算一个区域的补（一般指全图像区域减去该区域）

Region：要计算的区域

RegionComplement：输出，计算后的区域

 

## 阈值分割

### threshold(Image : Region : MinGray, MaxGray : )

详见：threshold

 

### auto_threshold(Image : Regions : Sigma : )

自动阈值分割

 

 

 

### bin_threshold(Image : Region : : )

用于提取背景为白色，且前后北京较为分明；自动选取sigma值进行告诉光滑处理，光滑直到只有一个最小值；例如提取白纸黑字，可以用此算子；

mage : 输入的图像

Region：输出，分割后的区域

 

### char_threshold(Image, HistoRegion : Characters : Sigma,Percent : Threshold)

阈值分割提取字符

Image：输入的图像

HistoRegion : 要提取字符所在的区域

Characters : 输出，提取得到的字符区域

Sigma：高斯光滑因子

Percent : 灰度直方图中的灰度值差的百分比

Threshold：得到的用于阈值处理的阈值

 

示例：

read_image (Image, 'letters')

char_threshold (Image, Image, Seg, 0.0,5.0, Threshold)

connection (Seg, Connected)

 

### dual_threshold(Image : RegionCrossings : MinSize, MinGray,Threshold : )

应用于分隔符号图像的阈值处理。通常伴随着如Diff_of_gauss，sub_image等这样的算子使用；就像前后两帧做差得到图像（差帧法），像素值有正值有负值。

Image : 输入的图像

RegionCrossings : 输出，获得的区域

MinSize:输出的区域的最小面积

MinGray:　区域的最小灰度绝对值

Threshold:　分割值

 

## 腐蚀膨胀

### erosion_circle(Region : RegionErosion : Radius : )

使用圆心结构元素对图像做腐蚀操作

Region：要做腐蚀操作的区域（也可以是区域数组）

RegionDilation：输出，腐蚀后的区域（或区域数组）

Radius：腐蚀操作的圆形结构的半径

 

### dilation_circle(Region : RegionDilation : Radius : )

使用圆形结构对区域做膨胀操作

Region：要做膨胀操作的区域（也可以是区域数组）

RegionDilation：输出，膨胀后的区域（或区域数组）

Radius：膨胀操作的圆形结构的半径

 

### erosion_rectangle1(Region : RegionErosion : Width, Height: )

使用矩形结构元素对图像做腐蚀操作

Region：要做腐蚀操作的区域（也可以是区域数组）

RegionDilation：输出，腐蚀后的区域（或区域数组）

Width，Height：腐蚀操作的矩形结构的宽和高

 

### dilation_rectangle1(Region : RegionDilation : Width,Height : )

使用矩形结构元素对图像做膨胀操作

Region：要做腐蚀操作的区域（也可以是区域数组）

RegionDilation：输出，膨胀后的区域（或区域数组）

Width，Height：膨胀操作的矩形结构的宽和高

 

## 生成区域

### gen_rectangle2( : Rectangle : Row, Column, Phi, Length1,Length2 : )

生成与坐标成任意角度的矩形

Rectangle : 输出，生成后的矩形

Row，Column：用于生成的矩形的中心坐标

Phi：生成矩形与水平轴的角度，以弧度为单位

Length1, Length2 :生成矩形的长宽的一半

 

 

# 亚像素轮廓（XLD）部分

## 基础操作

### threshold_sub_pix(Image : Border : Threshold : )

提取图像的XLD

Image：要提取xld的图像（单通道，如果为多通道只提取第一通道）

Border：输出，得到的XLD

Threshold：提取XLD的阈值，灰度值以阈值为分界提取XLD

### gen_contour_region_xld(Regions : Contours : Mode : )

从区域（区域数组）生成亚像素轮廓

Regions：要生成亚像素轮廓的区域（或区域数组）

Contours：输出，生成后的亚像素轮廓

Mode：生成方式，默认’border’，可输入范围区域：'border','border_holes', 'center'

 

### select_shape_xld(XLD : SelectedXLD : Features, Operation,Min, Max : )

选择特定形状特征要求的XLD轮廓或多边形

XLD：输入，要提取的XLD

SelectedXLD：输出，提取得到的XLD

Features：提取XLD的特征依据，默认为’area’，详见特征依据说明

Operation：特征之间的逻辑关系，默认’and’

Min，Max：特征值的要求范围

 

### select_contours_xld(Contours : SelectedContours : Feature,Min1, Max1, Min2, Max2 : )

选择多种特征要求的XLD轮廓（如长度开闭等，不支持多边形）

Contours：要提取的XLD轮廓

SelectedContours：输出，提取得到的XLD轮廓

Feature：提取的特征依据，详见特征依据说明

Min1， Max，Min2，Max2：特征值的要求范围

 

 

### area_center_xld(XLD : : : Area, Row, Column, PointOrder)

获取封闭的亚像素轮廓（或轮廓数组）的面积和重心坐标

XLD：输入的亚像素轮廓（或轮廓数组）

Area：输出，亚像素轮廓的面积

Row：输出，轮廓的重心的row坐标

Column:输出，轮廓的重心的column坐标

PointOrder: 输出，沿边界点顺序，正向或者反向

 

## 转换操作

### shape_trans_xld(XLD : XLDTrans : Type : )

将目标轮廓（或轮廓数组）转化成其他形状

XLD：要转化的XLD（或XLD数组）

XLDTrans：输出，转化后的XLD（或数组）

Type：转化的类型，默认'convex'，详见XLD轮廓的转化类型说明

## 分割

### segment_contours_xld(Contours : ContoursSplit : Mode,SmoothCont, MaxLineDist1, MaxLineDist2 : )

把原XLD分割成：“直线”，“直线和圆”，“直线和椭圆”

Contours：需要被分割的轮廓（或数组）

ContoursSplit：输出，分割后的轮廓数组

Mode：分割模式，默认lines_circles，可选'lines','lines_circles', 'lines_ellipses'

SmoothCont：用于平滑轮廓的点数量

MaxLineDist1：轮廓到近似线的最大距离（第一次迭代）

MaxLineDist2：轮廓到近似线的最大距离（第二次迭代）

## 拟合操作（未完）

### fit_line_contour_xld(Contours : : Algorithm, MaxNumPoints,ClippingEndPoints, Iterations, ClippingFactor : RowBegin, ColBegin, RowEnd,ColEnd, Nr, Nc, Dist)

根据已知轮廓拟合直线，获得的是直线的参数

Contours：输入的轮廓

Algorithm：拟合算法，默认tukey，可选'regression','huber', 'tukey', 'gauss', 'drop'，详见轮廓拟合算法说明轮廓拟合算法说明

MaxNumPoints：用于计算的轮廓点的最大数目（-1为所有点），默认-1

ClippingEndPoints：起始和结尾上忽略的点数，默认0

Iterations：迭代最大次数（不用于回归？）

ClippingFactor：离群值的剪切因子，值越小忽略的离群值越多

RowBegin，ColBegin：拟合后得到的直线开始点坐标

RowEnd，ColEnd：拟合后得到的直线结束点坐标

Nr，Nc：直线的法向量

Dist:原点到直线的距离

 

### fit_circle_contour_xld(Contours : : Algorithm,MaxNumPoints, MaxClosureDist, ClippingEndPoints, Iterations, ClippingFactor :Row, Column, Radius, StartPhi, EndPhi, PointOrder)

根据已知轮廓来拟合圆，并获得圆参数

Contours：输入的轮廓

Algorithm：拟合算法，默认algebraic，可选' 'algebraic','ahuber', 'atukey', 'geometric', 'geohuber', 'geotukey'，详见轮廓拟合算法说明轮廓拟合算法说明

MaxNumPoints：用于计算的轮廓点的最大数目（-1为所有点），默认-1

MaxClosureDist:小于这个值被认为是轮廓封闭的，区分拟合圆和圆弧

ClippingEndPoints：轮廓开始点和结束点参加与拟合点的个数（起始和结尾上忽略的点数？），默认0

Iterations：迭代次数

ClippingFactor：离群值的剪切因子，值越小忽略的离群值越多

Row, Column：圆心坐标

Radius：圆半径

StartPhi, EndPhi：圆弧开始角度，结束角度

PointOrder：边界点的次序

 

### fit_ellipse_contour_xld(Contours : : Algorithm,MaxNumPoints, MaxClosureDist, ClippingEndPoints, VossTabSize, Iterations,ClippingFactor : Row, Column, Phi, Radius1, Radius2, StartPhi, EndPhi,PointOrder)

拟合椭圆

 

### fit_rectangle2_contour_xld(Contours : : Algorithm,MaxNumPoints, MaxClosureDist, ClippingEndPoints, Iterations, ClippingFactor :Row, Column, Phi, Length1, Length2, PointOrder)

拟合矩形

 

 

# 测量

## 基础操作

### gen_measure_rectangle2( : : Row, Column, Phi, Length1,Length2, Width, Height, Interpolation : MeasureHandle)

创建一个任意方向的矩形测量句柄（区域）

Row, Column：矩形中心点坐标

Phi：矩形主轴方向与水平轴夹角

Length1：矩形宽度一半

Length2:矩形高度一半

Width, Height:处理图像的宽，高

Interpolation:插值方法

MeasureHandle：测量句柄

 

 

 

 

# 附:

## Halcon的图像像素类型

Halcon中的像素类型有如下(首字母应当为小写)：

Int1

Int2

Uint2

Int4

Int8

Byte:最常见的灰度图像，8位，一个字节，0~255

Real

Direction

Cyclic

Complex

## 特征依据说明

以下是一些可选的条件特征说明（代码中首字母应该为小写）：

Area：区域大小，应该是以像素为单位

Row：区域中心的row坐标

Column：区域中心的column坐标

Width：区域的宽度

Height：区域的高度

Row1：区域的起始row坐标（顶部）

Column1：区域的起始column坐标（左侧）

Row2：区域的最终row坐标（底部）

Column2：区域的最终column坐标(右侧)

Circularity：区域的圆度

Compactness：区域的紧密度

Contlength：区域外轮廓的总长度

Convexity：区域的凸性

Rectangularity：区域的垂直度

Ra：等效椭圆的长轴半径

Rb：等效椭圆的短轴半径

Phi：等效椭圆的方向

Anisometry：椭圆参数，Ra/Rb长轴与短轴的比值

Bulkiness：椭圆参数，蓬松度π*Ra*Rb/A

struct_factor：椭圆参数，Anisometry*Bulkiness-1

outer_radius：最小外接圆半径

inner_radius：最大内接圆半径

inner_width：最大内接矩形宽度

inner_height：最大内接矩形高度

dist_mean：区域边界到中心的平均距离

dist_deviation：区域边界到中心距离的偏差

roundness：圆度，与circularity计算方法不同           

num_sides：多边形边数       

connect_num：连通数           

holes_num：区域内洞数       

area_holes：所有洞的面积           

max_diameter：最大直径     

orientate:区域方向         

euler_number：欧拉数，即连通数和洞数的差          

rect2_phi：最小外接矩形的方向          

rect2_len1：最小外接矩形长度的一半？？

rect2_len2：   最小外接矩形宽度的一半    

moments_m11：几何矩        

moments_m20：几何矩        

moments_m02：几何矩        

moments_ia：几何矩    

moments_ib：几何矩    

moments_m11_invar：几何矩      

moments_m20_invar：几何矩      

moments_m02_invar：几何矩      

moments_phi1：几何矩         

moments_phi2：几何矩         

moments_m21：几何矩        

moments_m12：几何矩        

moments_m03：几何矩        

moments_m30：几何矩        

moments_m21_invar：几何矩      

moments_m12_invar：几何矩      

moments_m03_invar：几何矩      

moments_m30_invar：几何矩      

moments_i1：几何矩    

moments_i2：几何矩    

moments_i3：几何矩    

moments_i4：几何矩    

moments_psi1：几何矩         

moments_psi2：几何矩         

moments_psi3：几何矩         

moments_psi4：几何矩

 

## 区域的xld生成方式说明

border：边界像素的外边框将被用来生成边界

border_holes：除了外边界，还包含内边界

center：边界像素的中心将被用来生成边界