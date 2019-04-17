# GDAL  APP - 长歌行 - CSDN博客





2012年04月05日 09:47:53[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：1242标签：[dataset																[工具																[dst																[tiff																[多线程																[扩展](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)
个人分类：[图形图像																[GIS/RS/GPS](https://blog.csdn.net/iuhsihsow/article/category/1078364)](https://blog.csdn.net/iuhsihsow/article/category/1065735)





**原文地址：**[GDAL附带工具](http://blog.sina.com.cn/s/blog_6d51719a0100n2o6.html)**作者：**[xxzxlx](http://blog.sina.com.cn/u/1834054042)


**【本文转载于：[http://wiki.woodpecker.org.cn/moin/lilin/gdal-utils](http://wiki.woodpecker.org.cn/moin/lilin/gdal-utils)****，版权属于原创作者】**

[http://blog.163.com/lionyue@126/blog/static/10793071200710120467784/](http://blog.163.com/lionyue@126/blog/static/10793071200710120467784/)




gdal是一个库，所以它附带的程序本身是很容易被忽略的。可是这些程序又是极其有用的。真的，不骗你！



下面是主站中对gdal的附带工具程序的介绍，啥也不说了，翻译吧－－



# 1. GDAL附带工具



## 1.1. GDAL创建了下面的工具程序
- 
[gdalinfo](http://wiki.woodpecker.org.cn/moin/lilin/gdal-utils#gdalinfo) - 统计文件的信息。

- 
[gdal_translate](http://wiki.woodpecker.org.cn/moin/lilin/gdal-utils#gdal_translate) - 拷贝一个可以控制控制输出格式的栅格文件。

- 
[gdaladdo](http://wiki.woodpecker.org.cn/moin/lilin/gdal-utils#gdaladdo) - 为文件添加略缩图。

- 
[gdalwarp](http://wiki.woodpecker.org.cn/moin/lilin/gdal-utils#gdalwarp) - 把一个图像转换到一个新的坐标系统。

- 
[gdalindex](http://wiki.woodpecker.org.cn/moin/) - 为MapServer建立一个栅格索引。

- 
[gdal_contour](http://wiki.woodpecker.org.cn/moin/) - 给DEM创建等高线。

- 
[rgb2pct.py](http://wiki.woodpecker.org.cn/moin/) - 把一个24位的图像转化成8位调色板图像。

- 
[pct2rgb.py](http://wiki.woodpecker.org.cn/moin/) - 把一个8位调色板图像转化成24位位图。

- 
[gdal_merge.py](http://wiki.woodpecker.org.cn/moin/) - 把一系列的图像创建一个快速的地图集。

- 
[gdal_rasterize](http://wiki.woodpecker.org.cn/moin/) - 把一个矢量图层栅格化。

- 
[nearblack](http://wiki.woodpecker.org.cn/moin/) - 转换临近的黑/白边框为精准数值。

- 
[gdal-config](http://wiki.woodpecker.org.cn/moin/) - 获取建立使用gdal的软件需要的参数。




# 2. 创建新的文件


存取一个已存在的文件来读取是一件很容易的事情，只要在命令行中指定文件或者数据集的名字。但是，创建一个文件是一件非常复杂的事情。你可能需要指定创建格式，各种创建参数，以及指定一个坐标系统。在不同的GDAL工具中有许多参数都是差不多的，这里列举出来。

**-of** format
- 选择要创建新的文件的格式。这个格式被指定为类似GTiff（GeoTIFF格式）或者HFA（ERDAS格式）。所有的格式列表可以用--formats参数列出来。只有格式列表“(rw)”可以被写入。 许多工具如果没有指定，默认是创建GeoTIFF文件。文件扩展名不能用来猜测输出格式，如果没有指定文件名，gdal一般不会添加任何扩展名。

**-co** NAME=VALUE
- 许多格式会有一个或者更多的创建参数来控制文件创建的细节。比如GeoTIFF可以用创建参数控制压缩，或者控制是否用分片还是分带来进行存储。


可以使用的创建参数根据格式驱动不同而不同。而一些简单的格式根本就没有创建参数。虽然某个格式可以用"--format <format>"参数列出所有可用的参数列表，但是更详细的信息可以在格式介绍网页中查到。


**-a_srs** SRS
- 有几个工具(gdal_translate,gdalwarp)可以在命令行中通过类似-a_srs(分配输出SRS)、-s_srs(源SRS)、-t_srs(目标SRS)来指定坐标系统 这些工具允许以一系列格式定义坐标系统（SRS就是空间参考系统）。
- 
**NAD27/NAD83/WGS84/WGS72:**这些常见的地理坐标系统可以通过名字来直接使用。

- 
**EPSG:**n坐标系统（投影或者地理坐标）可以通过EPSG码来选择。例如EPSG 27700是英国国家网格。一系列的EPSG坐标系统可以在GDAL数据文件gcs.csv和pcs.csv中找到。

- 
**PROJ.4定义：** 一个PROJ4定义字符串可以用作坐标系统定义。例如“+proj=utm +zone=11 +datum=WGS84”。注意在命令行中要保持Proj4字符串在一起作为一个单独的参数（一般用双引号引起来）。

- 
**OpenGIS 知名文本:** OpenGIS标准定义了一个文本格式来描述坐标系统作为简单要素规范的一个部分。这个格式是gdal中使用的坐标系统的内部工作格式。包含wkt坐标系统描述的文件的文件名可以被用来作为坐标系统参数，或者坐标系统元素本身也可以被用来作为命令行参数（虽然wkt中释放所有引号是很有争议的）。

- 
**ESRI知名文本：**ESRI在他们的ArcGIS产品（ArcGIS .prj文件）中使用了一种经过精简OGC WKT的格式，而且这个格式被用在一个和wkt相似的风格的文件中。但是文件名要被加以ESRI::前缀。比如"ESRI::NAD 1927 StatePlane Wyoming West FIPS 4904.prj"。




# 3. 总的命令行参数


所有的GDAL命令行工具程序都支持下面的“总的”命令行参数。

**--version**
- 登记gdal版本并退出。

**--formats**
- 

列出所有gdal支持的栅格格式（只读和读写的）并退出。格式支持描述如下："ro"是只读驱动，'rw'是读写驱动（比如支持CreateCopy）；"rw+"是读写和更新驱动（比如支持Create）。


**--format format**
- 列出单个格式驱动的细节信息。格式名需要是在--formats 中列出的格式名。比如GTiff。

**--optfile file**
- 读取指定名称的文件并把其中的内容当成参数传入命令行列表。行首以#开头的行将被忽略。多字组成的参数需要用双引号来保正其为单一的参数。

**--config key value**
- 设置配置把指定键设置为某个值，而不必把他们设置为环境变量。一些命令参数键是GDAL_CACHEMAX(用于缓存的内存有多少M)以及GDAL_DATA（gdal的数据路径）。单一的驱动会被其他配置参数影响。

**--debug value**
- 控制调试信息的打印。ON值表示允许调试信息输出，OFF值表示不要输出调试信息。只有在调试信息包含了调试前缀编码中的字符串后其他值才会有选择。

**--help-general**
- 给出一个简短的普通的GDAL命令行参数用法信息然后退出。



# 4. 各工具程序详解



## 4.1. gdalinfo


列出栅格数据集的信息


用法：


gdalinfo [--help-general] [-mm] [-stats] [-nogcp] [-nomd]
        [-mdd domain] * datasetname






gdalinfo程序列出gdal支持的栅格格式的一系列信息。

**-mm**
- 强制计算栅格每个波段的最大最小值

**-stats**
- 牵制计算栅格的统计值

**-nogcp**
- 抑制地面控制点列表打印。这可能对大量的GCP的数据集们来说是十分有用的。比如L1B AVHRR或者hdf4 MODIS数据，这些数据包含了成千上万的地面控制点。

**-nomd**
- 抑制原数据打印，一些数据集可能包含极多的原数据字符串。

**-mdd domain**
- 给出指定域的原数据信息。


gdalinfo会给出如下的信息（如果有的话）：
- 用于存取的文件格式驱动
- 栅格大小(行列数)
- 文件的坐标系统(OGC WKT形式)
- 图像关联到地理的转换参数(当前不包含旋转系数)
- 地理上的边界坐标，如果可能的话还有基于经纬度的完整的地理转换参数（如果是GCPs就没有）
- 地面控制点(GCPs)
- 广义的（包括子栅格的原数据）文件原数据
- 波段数据类型
- 波段光度解析
- 波段瓦片大小
- 波段描述
- 波段最大最小值（已经经过计算的）
- 波段无意义值
- 波段可获得的略缩图分辨率
- 波段单位类型(如：波段的高程是米制还是英制)
- 波段的假颜色列表


例子:
gdalinfo ~/openev/utm.tif 
Driver: GTiff/GeoTIFF
Size is 512, 512
Coordinate System is:
PROJCS["NAD27 / UTM zone 11N",
    GEOGCS["NAD27",
        DATUM["North_American_Datum_1927",
            SPHEROID["Clarke 1866",6378206.4,294.978698213901]],
        PRIMEM["Greenwich",0],
        UNIT["degree",0.0174532925199433]],
    PROJECTION["Transverse_Mercator"],
    PARAMETER["latitude_of_origin",0],
    PARAMETER["central_meridian",-117],
    PARAMETER["scale_factor",0.9996],
    PARAMETER["false_easting",500000],
    PARAMETER["false_northing",0],
    UNIT["metre",1]]
Origin = (440720.000000,3751320.000000)
Pixel Size = (60.000000,-60.000000)
Corner Coordinates:
Upper Left  (  440720.000, 3751320.000) (117d38'28.21"W, 33d54'8.47"N)
Lower Left  (  440720.000, 3720600.000) (117d38'20.79"W, 33d37'31.04"N)
Upper Right (  471440.000, 3751320.000) (117d18'32.07"W, 33d54'13.08"N)
Lower Right (  471440.000, 3720600.000) (117d18'28.50"W, 33d37'35.61"N)
Center      (  456080.000, 3735960.000) (117d28'27.39"W, 33d45'52.46"N)
Band 1 Block=512x16 Type=Byte, ColorInterp=Gray







## 4.2. gdal_translate


不同格式间的栅格数据转换


用法：


gdal_translate [--help-general]
       [-ot {Byte/Int16/UInt16/UInt32/Int32/Float32/Float64/
             CInt16/CInt32/CFloat32/CFloat64}] [-not_strict]
       [-of format] [-b band] [-outsize xsize[%] ysize[%]]
       [-scale [src_min src_max [dst_min dst_max]]]
       [-srcwin xoff yoff xsize ysize] [-projwin ulx uly lrx lry]
       [-a_srs srs_def] [-a_ullr ulx uly lrx lry] [-a_nodata value]
       [-gcp pixel line easting northing]*
       [-mo "META-TAG=VALUE"]* [-quiet] [-sds]
       [-co "NAME=VALUE"]*
       src_dataset dst_dataset





gdal_translate 工具可以用来在不同格式间转换栅格数据。并且在处理过程中暗地里做一些诸如子栅格设置，重采样和象元值集体变化等勾当。

**-ot** type
- 指定输出波段的数据类型

**-not_strict**
- 当转换到输出格式时遇到不匹配或者丢失数据时，宽大处理。

**-of** format
- 

选择输出格式。默认是[GeoTiff](http://wiki.woodpecker.org.cn/moin/GeoTiff)（GTiff）。注意，指定的时候用格式简称。


**-b** band
- 

选择一个波段来输出。波段号从1开始编号。多个 **-b** 参数可以用于选择输出某几个波段或者重新进行波段的排序。


**-outsize** xsize[%] ysize[%]
- 设置输出文件的大小。大小以象元为单位，除非用“%”来标记，这时，表示的是输出为输入图层大小的百分比。

**-scale** [src_min src_max [dst_min dst_max] ]
- 重新组织输入象元的值。将它们从src_min~src_max范围内缩放到dst_min ~ dst_max范围内。如果省略，输出范围将为0~255。输入范围将由源数据自动计算。

**-srcwin** xoff yoff xsize ysize
- 选择一个取值窗口，通过该窗口在原图像中的行列位置来拷贝数值。

**-projwin** ulx uly lrx lry
- 选择一个取值窗口，通过该窗口在原图像中地理坐标范围来拷贝数据（类似srcwin）。

**-a_srs** srs_def
- 把输出文件投影强制指定坐标系。srs_def可以是任何常用的GDAL/OGR格式，WKT，Proj4，EPSG：n 或者一个包含着wkt的文件的文件名。

**-a_ullr** ulx uly lrx lry
- 强制指定输出文件的空间转换边界范围。而不需要和原图像有关系。

**-a_nodata** value
- 分配指定一个无意义值到输出波段。

**-mo** "META-TAG=VALUE"
- 如果可能的话通过设置一个原数据的键来指定输出数据集原数据对应的值。

**-co** "NAME=VALUE"
- 通过一个创建参数来指定输出格式特殊创建要求。多个-co 参数可以组合起来使用。创建参数可以参考个个数据格式本身说明。

**-gcp** pixel line easting northing
- 添加指定地面控制点到输出数据集。这个参数可能被提供多次以提供一系列的GCPs

**-quiet**
- 忽略处理进程监视和其他不是错误的输出。

**-sds**
- 拷贝文件中所有子数据集到各自的输出文件中。通常这个参数用在HDF或者OGDI这样有子数据集的格式中。


src_dataset
- 源栅格数据文件名


dst_dataset
- 目标栅格数据文件民


例子：
gdal_translate -of GTiff -co "TILED=YES" utm.tif utm_tiled.tif







## 4.3. gdaladdo


建立或者重建图像金字塔。


用法：


gdaladdo [-r {nearest,average,average_mp,average_magphase,mode}]
                filename levels







gdaladdo工具可以用于为大多数支持的格式建立或者重建金字塔。可以使用下面几种重采样算法中的一种来进行缩小重采样操作。

**-r** {nearest, average,average_mp, average_magphase,mode}:
- 选择重采样算法。


filename
- 要建立金字塔的文件名。


levels:
- 要建立略缩图的层号的列表。


Mode不是一个确切的实现。而且average_mp不适合使用。Average_magphase在一个mag/phase空间中平均联合数据（译者：这句涉及算法，我不懂这个是什么平均方法）。最临近和平均值法可以用于普通的影像数据。最临近法用一个最临近的邻居(简单象元)重采样，而平均值法计算所有非无意义值的象元的平均值来进行重采样。


选择一个缩放水平如2表示略缩图缩放程度是源图像每个维上分辨率的1/2。如果文件在所选缩放水平上已经存在略缩图，那么那个缩放程度将被重新计算并覆盖写入。


一些格式根本不支持金字塔。许多格式在文件以外以扩展名.ovr存储金字塔，TIFF就是如此。GeoTIFF格式直接把金字塔存储到原有的文件中。


在TIFF中创建金字塔可以通过用COMPRESS_OVERVIEW配置参数进行压缩。所有GeoTIFF支持的压缩方法，可以在这里获得（如： --config COMPRESS_OVERVIEW DEFLATE）。


大多数驱动也支持一个备用的略缩图格式（使用的是Erdas图像格式）。引发这个备用格式使用 USE_RRD=YES 来设置参数。这样做会把GDAL程序创建的金字塔放到一个辅助的.aux文件中使得可以该金字塔可以直接在Erdas中使用或者也可以在ArcGIS中使用。


例子：


在所提供的TIFF文件内部创建金字塔


gdaladdo -r average abc.tif 2 4 8 16






从一个ERDAS.IMG文件中创建一个外部的压缩的金字塔文件。
gdaladdo --config COMPRESS_OVERVIEW DEFLATE erdas.img 2 4 8 16






为给定JPEG文件创建一个Erdas Imagine 格式金字塔
gdaladdo --config USE_RRD YES airphoto.jpg 3 9 27 81







## 4.4. gdalwarp


简单图像重投影和绑定工具。


用法


gdalwarp 
    [-s_srs srs_def] [-t_srs srs_def] [-order n] [-et err_threshold]
    [-te xmin ymin xmax ymax] [-tr xres yres] [-ts width height]
    [-wo "NAME=VALUE"] [-ot Byte/Int16/...] [-wt Byte/Int16]
    [-srcnodata "value [value...]"] [-dstnodata "value [value...]"]
    [-rn] [-rb] [-rc] [-rcs] [-wm memory_in_mb] [-multi] [-q]
    [-of format] [-co "NAME=VALUE"]* srcfile* dstfile







gdalwarp工具是一个图像镶嵌、重投影、和绑定的工具。程序可以重投影到任何支持的投影，而且如果图像("raw" with)控制信息也可以把GCPs和图像存储在一起。

**-s_srs** srs_def
- 

源空间参考集。所有可以使用OGRSpatialReference.[SetFromUserInput](http://wiki.woodpecker.org.cn/moin/SetFromUserInput)()来调用的坐标系统都可以使用。包括EPSG PCS和GCSes（例如EPSG:4296），PROJ.4描述（如上）。或者包含知名文本的以prf为扩展名的文件。


**-t_srs** srs_def
- 目标坐标系统集。（可以参考上面的解释）

**-order** n
- 用于绑定的多项式规则。默认的是选择一个基于GCPs数量的多项式。

**-tps**
- 允许根据已有的GCPs使用薄板内插转换方法。可以用这个来替代-order参数。

**-et** err_threshold
- 转换的错误临界的近似值。（以象元单位- 默认到0.125）

**-te** xmin ymin xmax ymax
- 设置被创建的输出文件的地理边界范围

**-tr** xres yres
- 设置输出文件分辨率（单位以目标地理参考为准）

**-ts** width height
- 设置输出文件大小（以行列多少象元计量）

**-wo** "NAME=VALUE"
- 设置绑定参数。GDALWarpOptions::papszWarpOptions 不会显示所有的参数，多个-wo参数可以并列。

**-ot** type
- 设置输出波段的数据类型。

**-wt** type
- 工作的象元数据类型。包括在源图像和目标图像缓冲中的象元数据类型。

**-rn**
- 用最临近发进行重采样（默认，最快，但是内插质量最差）

**-rb**
- 用双线性法进行重采样

**-rc**
- 用立方体发进行重采样

**-rcs**
- 用立方曲线发进行重采样（最慢的方法）

**-srcnodata** value *[value...]*
- 设置输入波段的无意义值掩膜（如果设置多个值，就可以支持每个波段设一个）。如果有多个值，就需要把他们用双引号括起来，以保持在命令参数中作为单一参数输入。掩膜值不会在内插中处理。

**-dstnodata** value *[value...]*
- 设置输出波段的无意义值掩膜（可以参考上面）。注意，如果可能的化，新的文件将初始化这个值，并记录在输出文件中。

**-wm***memory_in_mb*
- 设置内存总量（以兆字节为单位），如果绑定API允许使用高速缓存。

**-multi**
- 使用多线程绑定实现。多线程将被用来处理打快的图像并同时处理输入输出操作。

**-q**
- 不打印输出。

**-of** format
- 

选择输出格式，默认是[GeoTiff](http://wiki.woodpecker.org.cn/moin/GeoTiff)


**-co***"NAME=VALUE"*
- 通过一个创建参数来控制创建新的文件。多个'-co'可以排列起来使用。参数可用的值可以察看每个驱动。


srcfile
- 源文件名


dstfile
- 目标文件名


如果输出文件已经存在，则在已存在的文件中创建地图集。


例子：


一个带有用经纬度标记的边界控制点的8位[GeoTiff](http://wiki.woodpecker.org.cn/moin/GeoTiff)格式的Spot影像可以通过下面的命令绑定到一个UTM投影上。


gdalwarp -t_srs '+proj=utm +zone=11 +datum=WGS84' raw_spot.tif utm11.tif






一个带有用经纬度标记的边界控制点的两个波段的HDF格式的ASTER影像可以通过下面的命令绑定到UTM投影上。


gdalwarp HDF4_SDS:ASTER_L1B:"pg-PR1B0000-2002031402_100_001":2 pg-PR1B0000-2002031402_100_001_2.tif](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=tiff&t=blog)](https://so.csdn.net/so/search/s.do?q=dst&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=dataset&t=blog)




