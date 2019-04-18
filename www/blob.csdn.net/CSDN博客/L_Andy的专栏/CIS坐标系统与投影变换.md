# CIS坐标系统与投影变换 - L_Andy的专栏 - CSDN博客

2014年03月06日 09:41:58[卡哥](https://me.csdn.net/L_Andy)阅读数：15993
个人分类：[地理信息系统](https://blog.csdn.net/L_Andy/article/category/2035345)



坐标系统与投影变换

本文共可分为如下几个部分组成：
地球椭球体(Ellipsoid)
大地基准面（Geodetic datum）

投影坐标系统（Projected Coordinate Systems ）

GIS处理的是空间信息，而所有对空间信息的量算都是基于某个坐标系统的，因此GIS中坐标系统的定义是GIS系统的基础，正确理解GIS中的坐标系统就变得尤为重要。坐标系统又可分为两大类：地理坐标系统、投影坐标系统。本文就对坐标系和投影及其在ArcGIS桌面产品中的应用做一些简单的论述。

    GIS中的坐标系定义由基准面和地图投影两组参数确定，而基准面的定义则由特定椭球体及其对应的转换参数确定，因此欲正确定义GIS系统坐标系，首先必须弄清地球椭球体(Ellipsoid)、大地基准面(Datum)及地图投影(Projection)三者的基本概念及它们之间的关系。

地球椭球体(Ellipsoid)
众所周知我们的地球表面是一个凸凹不平的表面，而对于地球测量而言，地表是一个无法用数学公式表达的曲面，这样的曲面不能作为测量和制图的基准面。假想一个扁率极小的椭圆，绕大地球体短轴旋转所形成的规则椭球体称之为地球椭球体。地球椭球体表面是一个规则的数学表面，可以用数学公式表达，所以在测量和制图中就用它替代地球的自然表面。因此就有了地球椭球体的概念。

    地球椭球体有长半径和短半径之分，长半径(a)即赤道半径，短半径(b)即极半径。f=（a-b）/a为椭球体的扁率，表示椭球体的扁平程度。由此可见，地球椭球体的形状和大小取决于a、b、f。因此，a、b、f被称为地球椭球体的三要素。

       ArcGIS(ArcInfo)桌面软件中提供了30种地球椭球体模型；常见的地球椭球体数据见下表：

![](https://img-blog.csdn.net/20140306095238937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

对地球椭球体而言，其围绕旋转的轴叫地轴。地轴的北端称为地球的北极，南端称为南极；过地心与地轴垂直的平面与椭球面的交线是一个圆，这就是地球的赤道；过英国格林威治天文台旧址和地轴的平面与椭球面的交线称为本初子午线。以地球的北极、南极、赤道和本初子午线等作为基本要素，即可构成地球椭球面的地理坐标系统(A*geographic coordinate system* (GCS) uses a threedimensional spherical surface to definelocations on the earth.A GCS includes an angular unit of measure, a primemeridian,

and a datum (based on a spheroid).)。可以看出地理坐标系统是球面坐标系统，以经度/纬度（通常以十进制度或度分秒(DMS)的形式）来表示地面点位的位置。
地理坐标系统以本初子午线为基准（向东，向西各分了1800）之东为东经其值为正，之西为西经其值为负；以赤道为基准（向南、向北各分了900）之北为北纬其值为正，之南为南纬其值为负。

![](https://img-blog.csdn.net/20140306095335890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20140306095345281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

大地基准面（Geodetic datum）
大地基准面（Geodetic datum），设计用为最密合部份或全部大地水准面的[数学](http://baike.baidu.com/view/1284.htm)模式。它由椭球体本身及椭球体和地表上一点视为原点间之关系来定义。此关系能以6个量来定义，通常（但非必然）是大地[纬度](http://baike.baidu.com/view/1298.htm)、大地[经度](http://baike.baidu.com/view/93982.htm)、原点高度、原点[垂线](http://baike.baidu.com/view/200817.htm)偏差之两分量及原点至某点的大地方位角。

    让我们先抛开测绘学上这个晦涩难懂的概念，看看GIS系统中的基准面是如何定义的，GIS中的基准面通过当地基准面向WGS1984的转换7参数来定义，转换通过相似变换方法实现，具体算法可参考科学出版社1999年出版的《城市地理信息系统标准化指南》第76至86页。假设Xg、Yg、Zg表示WGS84地心坐标系的三坐标轴，Xt、Yt、Zt表示当地坐标系的三坐标轴，那么自定义基准面的7参数分别为：三个平移参数ΔX、ΔY、ΔZ表示两坐标原点的平移值；三个旋转参数εx、εy、εz表示当地坐标系旋转至与地心坐标系平行时，分别绕Xt、Yt、Zt的旋转角；最后是比例校正因子，用于调整椭球大小。

    那么现在让我们把地球椭球体和基准面结合起来看，在此我们把地球比做是“马铃薯”，表面凸凹不平，而地球椭球体就好比一个“鸭蛋”，那么按照我们前面的定义，基准面就定义了怎样拿这个“鸭蛋”去逼近“马铃薯”某一个区域的表面，X、Y、Z轴进行一定的偏移，并各自旋转一定的角度，大小不适当的时候就缩放一下“鸭蛋”，那么通过如上的处理必定可以达到很好的逼近地球某一区域的表面。

    因此，从这一点上也可以很好的理解，每个国家或地区均有各自的基准面，我们通常称谓的北京54坐标系、西安80坐标系实际上指的是我国的两个大地基准面。我国参照前苏联从1953年起采用克拉索夫斯基(Krassovsky)椭球体建立了我国的北京54坐标系，1978年采用国际大地测量协会推荐的1975地球椭球体（IAG75）建立了我国新的大地坐标系--西安80坐标系，目前大地测量基本上仍以北京54坐标系作为参照，北京54与西安80坐标之间的转换可查阅国家测绘局公布的对照表。WGS1984基准面采用WGS84椭球体，它是一地心坐标系，即以地心作为椭球体中心，目前GPS测量数据多以WGS1984为基准。

    克拉索夫斯基(Krassovsky)、1975地球椭球体（IAG75）、WGS1984椭球体的参数可以参考常见的地球椭球体数据表。
椭球体与基准面之间的关系是一对多的关系，也就是基准面是在椭球体基础上建立的，但椭球体不能代表基准面，同样的椭球体能定义不同的基准面。地球椭球体和基准面之间的关系以及基准面是如何结合地球椭球体从而实现来逼近地球表面的可以通过下图一目了然。

![](https://img-blog.csdn.net/20140306095358375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

---------------------------ArcGIS中坐标系统的一些补充----------------------------------

空间参考

空间参考（Spatial Reference）是GIS数据的骨骼框架，能够将我们的数据定位到相应的位置，为地图中的每一点提供准确的坐标。 在同一个地图上显示的地图数据的空间参考必须是一致的，如果两个图层的空间参考不一致，往往会导致两幅地图无法正确拼合，因此开发一个GIS系统时，为数据选择正确的空间参考非常重要。

    在 ArcGIS 中，每个数据集都具有一个坐标系，该坐标系用于将数据集与通用坐标框架（如地图）内的其他地理数据图层集成。通过坐标系可在地图中集成数据集，以及执行各种集成的分析操作，例如叠加不同的源和坐标系中的数据图层。

![](https://img-blog.csdn.net/20140306095426921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

大地水准面

大地水准面是由静止海水面并向大陆延伸所形成的不规则的封闭曲面。它是重力等位面，即物体沿该面运动时，重力不做功（如水在这个面上是不会流动的）。因为地球的质量并非在各个点均匀分布，因此重力的方向也会相应发生变化，所以大地水准面的形状是不规则的，如下图：

![](https://img-blog.csdn.net/20140306095438703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

[](http://photo.blog.sina.com.cn/showpic.html#blogid=a9091a330101aaxz&url=http://s8.sinaimg.cn/orignal/a9091a33gd8e217001bc7)

地球椭球体

由定义可以知大地水准面的形状也是不规则的，仍不能用简单的数学公式表示，为了测量成果的计算和制图的需要，人们选用一个同大地水准面相近的可以用数学方法来表达的椭球体来代替，简称地球椭球体，它是一个规则的曲面，是测量和制图的基础，因地球椭球体是人们选定的跟大地水准面很接近的规则的曲面，所以地球椭球体就可以有多个，地球椭球体是用长半轴、短半轴和扁率来表示的。

基准面

基准面是在特定区域内与地球表面极为吻合的椭球体。椭球体表面上的点与地球表面上的特定位置相匹配，也就是对椭球体进行定位，该点也被称作基准面的原点。原点的坐标是固定的，所有其他点由其计算获得。

基准面的坐标系原点往往距地心有一定偏移(有的也在地心，如WGS1984)，如西安80的基准面和北京54的基准面.因为椭球体通过定位以便能更好的拟合不同的地区，所以同一个椭球体可以拟合好几个基准面.因为原点不同，所以不同的基准面上，同一个点的坐标是不相同的，这点我们应该清楚。

----------------------------------回归原文，继续介绍----------------------------------------------

投影坐标系统（Projected Coordinate Systems ）

地球椭球体表面也是个曲面，而我们日常生活中的地图及量测空间通常是二维平面，因此在地图制图和线性量测时首先要考虑把曲面转化成平面。由于球面上任何一点的位置是用地理坐标（λ，φ）表示的，而平面上的点的位置是用直角坐标（x，у）或极坐标（r，θ）表示的，所以要想将地球表面上的点转移到平面上，必须采用一定的方法来确定地理坐标与平面直角坐标或极坐标之间的关系。这种在球面和平面之间建立点与点之间函数关系的数学方法，就是地图投影方法。

接下来首先让我们来看看ArcGIS产品中对于北京54投影坐标系统的定义参数：

Projection: Gauss_Kruger

Parameters:

False_Easting: 500000.000000

False_Northing: 0.000000

Central_Meridian: 117.000000

Scale_Factor: 1.000000

Latitude_Of_Origin: 0.000000

Linear Unit: Meter (1.000000)

Geographic Coordinate System:

Name: GCS_Beijing_1954

Alias:

Abbreviation:

Remarks:

Angular Unit: Degree (0.017453292519943299)

Prime Meridian: Greenwich (0.000000000000000000)

Datum: D_Beijing_1954

Spheroid: Krasovsky_1940

Semimajor Axis: 6378245.000000000000000000

Semiminor Axis: 6356863.018773047300000000

Inverse Flattening: 298.300000000000010000

从参数中可以看出，每一个投影坐标系统都必定会有Geographic Coordinate System（地理坐标系统）。那么我们从这一角度上解释一下投影和投影所需要的必要条件：将球面坐标转化为平面坐标的过程便是投影过程；投影所需要的必要条件是：第一、任何一种投影都必须基于一个椭球（地球椭球体），第二、将球面坐标转换为平面坐标的过程（投影算法）。简单的说投影坐标系是地理坐标系+投影过程。

让我们从透视法（地图投影方法的一种）角度来直观的理解投影，如下图：

![](https://img-blog.csdn.net/20140306095452625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

几何透视法是利用透视的关系，将地球体面上的点投影到投影面（借助的几何面）上的一种投影方法。如假设地球按比例缩小成一个透明的地球仪般的球体，在其球心或球面、球外安置一个光源，将球面上的经纬线投影到球外的一个投影平面上。

投影既然是一种数学变换方法，那么任何一种投影都存在一定的变形，因此可以按照变形性质将投影方法如下分类：等角投影（Conformal  Projection）、等积投影（EqualArea Projection）、等距投影（Equidistant Projection）、等方位投影（True-direction Projection）四种。每种投影根据其名称就可以知道其方法保证了数据的那些几何属性，在实际应用过程中应根据需求来选取某种投影。

如果按照投影的构成方法分类又可分为方位、圆柱、圆锥投影三种，在上述三种投影中由于几何面与球面的关系位置不同，又分为正轴、横轴和斜轴三种。

![](https://img-blog.csdn.net/20140306095511625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

正、横、斜圆柱投影示意图

![](https://img-blog.csdn.net/20140306095519296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

正、横、斜圆锥投影示意图

![](https://img-blog.csdn.net/20140306095527218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

正、横、斜方位投影示意图

![](https://img-blog.csdn.net/20140306095539656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

接下来我们来看看我们国家通常采用的投影——高斯—克吕格（Gauss-Kruger)投影，是一种“等角横切圆柱投影”。德国数学家、物理学家、天文学家高斯（Carl
 Friedrich Gauss，1777一1855）于十九世纪二十年代拟定，后经德国大地测量学家克吕格（JohannesKruger，1857～1928）于1912年对投影公式加以补充，故名。设想用一个圆柱横切于球面上投影带的中央经线，按照投影带中央经线投影为直线且长度不变和赤道投影为直线的条件，将中央经线两侧一定经差范围内的球面正形投影于圆柱面。然后将圆柱面沿过南北极的母线剪开展平，即获高斯-克吕格投影平面。高斯—克吕格投影后，除中央经线和赤道为直线外，其他经线均为对称于中央经线的曲线。高斯—克吕格投影没有角度变形，在长度和面积上变形也很小，中央经线无变形，自中央经线向投影带边缘，变形逐渐增加，变形最大处在投影带内赤道的两端。

![](https://img-blog.csdn.net/20140306095552015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20140306095603140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

按一定经差将地球椭球面划分成若干投影带,这是高斯投影中限制长度变形的最有效方法。分带时既要控制长度变形使其不大于测图误差，又要使带数不致过多以减少换带计算工作，据此原则将地球椭球面沿子午线划分成经差相等的瓜瓣形地带,以便分带投影。通常按经差6度或3度分为六度带或三度带。六度带自0度子午线起每隔经差6度自西向东分带，带号依次编为第
 1、2…60带。三度带是在六度带的基础上分成的，它的中央子午线与六度带的中央子午线和分带子午线重合，即自1.5度子午线起每隔经差3度自西向东分带，带号依次编为三度带第 1、2…120带。我国的经度范围西起73°东至135°，可分成六度带十一个，各带中央经线依次为75°、81°、87°、……、117°、123°、129°、135°，或三度带二十二个。
我国大于等于50万的大中比例尺地形图多采用六度带高斯—克吕格投影，三度带高斯—克吕格投影多用于大比例尺1：1万测图，如城建坐标多采用三度带的高斯—克吕格投影。高斯—克吕格投影按分带方法各自进行投影，故各带坐标成独立系统。以中央经线（L0）投影为纵轴X, 赤道投影为横轴Y，两轴交点即为各带的坐标原点。为了避免横坐标出现负值，高斯—克吕格投影北半球投影中规定将坐标纵轴西移500公里当作起始轴。由于高斯—克吕格投影每一个投影带的坐标都是对本带坐标原点的相对值，所以各带的坐标完全相同，为了区别某一坐标系统属于哪一带，通常在横轴坐标前加上带号，如(4231898m,21655933m)，其中21即为带号。高斯—克吕格投影及分带示意图如下：

![](https://img-blog.csdn.net/20140306095615796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

我们再来看看ArcGIS中对我们国家经常采用北京54和西安80坐标系统是怎么样描述的，在ArcMap或是ArcCatalog中选择系统预定义的北京54和西安80坐标系统。

在${ArcGISHome}\Coordinate Systems\CoordinateSystems\Projected Coordinate Systems\Gauss Kruger\Beijing 1954目录中，我们可以看到四种不同的命名方式：
 Beijing 1954 3 Degree GK CM 75E.prj
   Beijing 1954 3 Degree GK Zone25.prj

    Beijing 1954 GK Zone 13.prj

    Beijing 1954 GK Zone 13N.prj
对它们的说明分别如下：
三度分带法的北京54坐标系，中央经线在东75度的分带坐标，横坐标前不加带号
三度分带法的北京54坐标系，中央经线在东75度的分带坐标，横坐标前加带号
六度分带法的北京54坐标系，分带号为13，横坐标前加带号
六度分带法的北京54坐标系，分带号为13，横坐标前不加带号

在Coordinate Systems\Projected Coordinate Systems\GaussKruger\Xian 1980目录中，文件命名方式又有所变化：
 Xian 1980 3 Degree GK CM 75E.prj
   Xian 1980 3 Degree GK Zone25.prj

    Xian 1980 GK CM 75E.prj

    Xian 1980 GK Zone 13.prj
西安80坐标文件的命名方式、含义和北京54前两个坐标相同，但没有出现“带号+N”这种形式，为什么没有采用统一的命名方式？让人看了的确有些费解，大家在应用过程中需要特别注意一下。

