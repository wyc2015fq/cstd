# DEM数据下载和导入的方法汇总 - 战斗蜗牛的专栏 - CSDN博客





2012年12月24日 16:27:28[vbskj](https://me.csdn.net/vbskj)阅读数：10843








1）30m的DEM数据

ftp://edcftp.cr.usgs.gov/data/gtopo30/global/





2）SRTM数据下载

http://srtm.csi.cgiar.org/index.asp





3）国家科学数据平台

http://datamirror.csdb.cn/












[http://glcf.umiacs.umd.edu/data/](http://glcf.umiacs.umd.edu/data/)


[http://datamirror.csdb.cn/](http://datamirror.csdb.cn/)[国内镜像
 单线程普速1MB/S左右]
[http://glovis.usgs.gov/](http://glovis.usgs.gov/)【美国地质调查局 速度很慢30KB/S以下，比CSDB多】

全球各国shape数据下载，包括矢量要素、dem数据、遥感图片，免费，精度不知。[http://biogeo.berkeley.edu/bgm/gdata.php](http://biogeo.berkeley.edu/bgm/gdata.php)

全球各国eoo格式数据下载，包括矢量要素、dem数据、遥感图片，需付费，也有部分类型数据免费，精度不知道。[http://data.geocomm.com/](http://data.geocomm.com/)
       1.公开的DEM数据SRTM3
         SRTM数据主要是由美国太空总署(NASA)和国防部国家测绘局(NIMA)联合测量的，SRTM 的全称是Shuttle Radar Topography Mission，即航天飞机雷达地形测绘使命，2000年2月 11日上午11时44分，美国“奋进”号航天飞机在佛罗里达州卡那维拉尔角的航天发射中心 发射升空，“奋进”号上搭载的SRTM系统共计进行了222小时23分钟的数据采集工作，获取 北纬60度至南纬56度之间，面积超过1.19亿平方公里的9.8万亿字节的雷达影像数据，覆盖
 全球陆地表面的80%以上，该计划共耗资3.64亿美元，获取的雷达影像数据经过两年多的处 理，制成了数字地形高程模型，该测量数据覆盖中国全境。 SRTM数据每经纬度方格提供一个文件，精度有1 arc-second和3 arc-seconds两种，称 作SRTM1和SRTM3,或者称作30M和90M数据，SRTM1的文件里面包含3601*3601个采样点的高度 数据，SRTM3的文件里面包含1201*1201个采样点的高度数据。目前能够免费获取中国境内的 SRTM3文件，是90米的数据，每个90米的数据点是由9个30米的数据点算术平均得来的。 

         SRTM数据下载的地址是：[http://srtm.csi.cgiar.org/SELECTION/inputCoord.asp](http://srtm.csi.cgiar.org/SELECTION/inputCoord.asp)。我国境内的数
 据在Eurasia目录下，每经纬度方格一个文件，文件命名方法是X1X2X3X4.hgt.zip，X1是N或 S表示南北，X2是下方纬度数，X3是E或W表示东西，X4是左方经度数。 Documentation目录下有这些数据的介绍。 

http://dds.cr.usgs.gov/srtm/version2_1/SRTM3/






       2.公开的三波段遥感合成图像GeoCover
         GeoCover是使用LANDSAT5卫星在1990年左右采集的数据合成的，LANDSAT5是美国太空总 署(NASA)于1984年3月1日发射的地球观测卫星。LANDSAT5 TM(Thematic Mapper)有7个波段： 

波段1:0.45–0.52um蓝绿波段 波段2:0.52- 0.60绿色波段 

波段3:0.63 - 0.69红色波段 波段4:0.76-0.90um近红外波段 

波段7:2.08-2.35um短红外波段 

         其中波段1-5和7为可见光及近红外波段，分辨率为30米，波段6为热红外光分辨率为120 米。LANSAT5每景扫描耗时约26.31秒，覆盖185公里×170公里，扫描影像重叠率超过10%。 GeoCover使用了742波段分别作为红色、绿色和蓝色进行了假彩色合成，也许如果使用1、 2、3三种可见光波段合成的话会使我们看起来更舒服一些，不过我们现在能拿到的只能是这 种742的合成假彩，通过观察发现其中的绿色通道对地形有较好的反应，实际操作中可以将这 种假彩图像反相取其中的绿色通道来作为地形底图。GeoCover图像文件使用WGS84
 UTM坐标， 采用TIFF或MrSID格式分发，图像中每个像素28.5M-30M,一般以28.5M计算。 GeoCover的下载地址是：[https://zulu.ssc.nasa.gov/mrsid](https://zulu.ssc.nasa.gov/mrsid)。可从全球地图页面上选择 

你需要的区域下载。GeoCover文件每个在东西方向跨越一个UTM分区，在南北方向跨越5个纬 度，东西提供50KM的富裕，南北提供1KM的富裕。文件命名方法是X1-X2-X3.tar，X1是N或S表 示南北，X2是UTM带数，X3下方经度数。 
         3.其他渠道 
         NIMA地名数据库:可以提供大多数国家的地名英文拼写及坐标,包含中国境内大约6万个地方, 可以在以下地址下载中国地名库:[ftp://ftp.nga.mil/pub/gns](http://www.cnblogs.com/GIS_zhou/admin/ftp://ftp.nga.mil/pub/gns);data/cn.zip。 
         SPACEIMAGING 1M分辨率卫星图像预览:虽然只能免费得到预览图像，但其分辨率也已经不算 太低了，并且可以随预览图提供图像的一些数据信息，非常方便将图像用工具软件进行较正， 下载地址:[http://www.spaceimaging.com/default.htm](http://www.spaceimaging.com/default.htm);
       国家基础地理信息系统:国家基础地理信息系统全国1:400万数据库全部数据均可浏览。其中， 中国国界、省界、地市级以上居民地、三级以上河流、主要公路和主要铁路等数据可以自由下载。 

       下载地址为：[http://nfgis.nsdi.gov.cn/nfgis/chinese/c](http://nfgis.nsdi.gov.cn/nfgis/chinese/c);xz.htm。可以在这里预先浏览供下
 载的数据内容有： （1）国界 国界与省界 地市级以上居民地 一级河流 三级以上河流 主要公路 主要铁路 （2）地级以上境界 县级以上境界 县级以上居民地 四级以上河流 五级以上河流 

       从公开发行的地图上获取数据信息:可以直接从某些电子地图提取到不错的数据，也可以从 纸介地图上测量或数字化某些信息，目前正在进行这方面的试验，已取得一定量数据。




