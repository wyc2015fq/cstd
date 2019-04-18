# osgearth earth文件规范-Features&Symbology（要素与符号） - L_Andy的专栏 - CSDN博客

2016年02月26日 12:07:52[卡哥](https://me.csdn.net/L_Andy)阅读数：3242



# 前言

osg渲染引擎实现了仿真的基础功能，使用osg可以帮助我们快速实现仿真效果，osgEarth致力于数字地球的仿真实现，现已实现大量地理信息相关的算法，是数字城市仿真、三位地理信息的利器。

**要素与符号**

## 要素是什么

要素是向量几何。与图像和高程数据不同 （这是*光栅*），功能不具有离散的显示分辨率。osgEarth可以呈现任何级别的详细信息的功能。

一个要素是三个组件的组合：
- 矢量几何图形 （点、
 线、 或多边形的集合）
- 属性 （名称/值对的集合）
- 空间参考（描述几何坐标系）

## 创建一个要素图层

osgEarth可以以两种不同的方式呈现的功能：

•图层栅格化为图像

 •镶嵌为一个模型层

### 栅格化

栅格化的要素是最简单的-osgEarth
 将向量"绘制"到一个图像瓦片上，然后在一个正常的图像图层中使用该图像瓦片。

osgEarth 有一个栅格化功能驱动程序： agglite驱动程序。这里是一个把ESRIshapefile图层作为
 栅格化图像呈现的示例：

<modelname="my layer" driver="agglite">

    <features name="states"driver="ogr">

        <url>states.shp</url>

    </features>

    <styles>

        <style type="text/css">

            states {

                stroke:       #ffff00;

                stroke-width: 2.0;

            }

        </style>

    </styles>

</model>

###   镶嵌

镶嵌化的要素经过一个编译过程，将输入的向量变成
 OSG 几何图形 (点、 线、 三角形或osg可以识别的替身 3D
 模型）。主要的要素镶嵌插件是feature_geom驱动程序——您将在大部分的 osgEarth
 的earth文件中演示使用要素数据时看到它。

这里有一个模型层，将呈现一系列的黄色的线条，呈现为 OSG 线几何作为 ESRI 联接：

<modelname="my layer" driver="feature_geom">

    <features name="states"driver="ogr">

        <url>states.shp</url>

    </features>

    <styles>

        <style type="text/css">

            states {

                stroke:       #ffff00;

                stroke-width: 2.0;

            }

        </style>

    </styles>

</model>

### **要素图层的组****件**

正如你可以看到从上面的例子，有几个必要的组件到任何功能层

 •<features>块描述实际要素源
 ；即，osgEarth 应该去哪里找的输入的数据。

     • <styles>块描述 osgEarth如何去渲染要素，即它们的外观在场景中的呈现方式。我们称这个样式表或符号。样式表的构成可以从根本上改变要素数据的外观。

 这两个元素都是必需的。

## 样式

在earth文件中，您可能会看到一个<styles>块，格式如下：

<styles>

    <style type="text/css">

        buildings {

            altitude-clamping: terrain;

            extrusion-height:  15;

            extrusion-flatten: true;

            fill:              #ff7f2f;

        }

    </style>

</styles>

这是一个样式表块。你会发现这里面呈现特征数据，与一个<features>块配对的<model>层。( <features>块定义了实际内容的来源)。

在这种情况下，该<styles>元素具有 CSS 格式的数据。CSS样式块可以容纳多个样式，其中每个都有一个名称。在这种情况下我们只有一种样式：buildings。这种风格告诉几何引擎进行以下操作：

•几何要素固定到地形高程数据 ；

 •拉伸形状到地形 的15 米以上高度 ；

 •拼合拉伸出来的形状的顶部 ；和

 •该形状的颜色橙色。

osgEarth采用"模型/视图"的方法来呈现要素。它分离内容和样式的概念，这与 web
 应用程序使用CSS设计web 内容的样式的方式很相像。

osgEarth得到每个输入的要素并提交到一个设计造型的流程中。输出将完全取决于样式表中的符号的组合。这包括：

•fill和stroke——是否作为线条或多边形绘制数据

 •extrusion——挤压成
 3D形状的 2D 几何

 •substitution——用外部
 3D 模型 （例如，树） 或图标替换几何图形

 •altitude——几何要素与地图上的地形的交互方式

 •text——标签控件

 •rendering——可以使用lighting, blending,和 depth testing

### 样式表

每个要素层需要样式表。样式表作为一个<styles>块在earth文件中出现。下面是一个示例：

<modelname="test" driver="feature_geom">

    <features driver="ogr">

        <geometry>POLYGON( (0 0, 1 0, 11, 0 1) )</geometry>

       <profile>global-geodetic</profile>

    </features>

    <styles>

        <style type="text/css">

            default {

                fill:               #ff7f009f;

                stroke:             #ffffff;

                stroke-width:       2.0;

                altitude-clamping:  terrain;

                altitude-technique: drape;

                render-lighting:    false;

            }

        </style>

    </styles>

</model>

样式表包含一个默认的样式。由于只有一个样式，osgEarth会将它应用到输入的所有要素上。（要将不同的样式应用到不同的要素，使用选择器——下面有更详细的信息。

样式包含一组描述osgEarth应如何呈现几何要素的符号。在这种情况下：

        fill:    绘制实心的多边形，用指定的HTML样式颜色（本例中为橙色）填充。

        stroke:   用白线画出多边形轮廓。

        stroke-width :    轮廓线宽度为2个像素。

        altitude_clamp：把多边形固定到地形上。

       altitude_technique:   使用"drape"技术来固定多边形（作为投影纹理）

        render-lighting:   禁用OpenGL多边形上的照明。

这只是可用符号的一个小例子。有关完整列表，请参阅：[符号参考](http://docs.osgearth.org/en/latest/references/symbology.html)（[译文](http://blog.csdn.net/giantchen547792075/article/details/15318863)）。

### 表达式

一些符号属性支持表达式。表达式是使用功能属性值来动态地计算属性简单线中计算。

在表达式中，你访问功能属性值通过将它的名称括在方括号内，像这样：[name]

示例：

mystyle{

    extrusion-height:  [hgt]*0.3048;           - read the "hgt"attribute, and convert it from feet to meters

    altitude-offset:   max([base_offset], 1);  - use the greater of the"base_offset" attribute, and 1.0

    text-content:      "Name: [name]";         - sets the text label to theconcatenation of a literal and an attribute value

}

数值表达式计算器支持基本的算术运算（+、-、 *，/ %)、 一些实用工具函数 （min、 max） 和带括号分组。它也适用于字符串值。但对字符串没有操作符，但您仍可以嵌入属性。

如果简单的表达式还是不够的您可以将JavaScript 代码 的嵌入— — 请参阅一节关于脚本的详细信息。

## 样式选择器

TBD。

## 脚本

TBD。

##  贴地

要素与地形以某种方式进行交互是相当常见的。有此类需求的对象包括：

•贴合着地形轮廓的街道

 •种植在地上的树木

 •专题制图，如基于它的人口的一个国家的面积着色

osgEarth提供了多种贴地的办法，因为没有哪一种方式是适合每种情况的。

### 地图固定(clamp,有好的建议请回复)

地图固定是最简单的方法。在编译要显示的要素时，osgEarth将对地图中的高程层采样，查找地形高度值并将其应用到由此而产生的几何图形要素上。它将沿几何图形测试每个点。

地图固定致使渲染的质量提高，需要权衡的是性能：

•采样地图中的高程数据会使得系统变慢，这要看您选择的分辨率。对于大量的数据来说这是 CPU
 密集的而且是耗时的。

 •采样是准确的并做为几何中的每一点。你可以选择采样每个要素的质心来提高编译速度。

 •根据几何要素的分辨率，您可能需要镶嵌 您的数据，实现更好的质量。

 •与其他方法相比渲染质量是很好的。

您可以在样式表中激活地图固定如下：

altitude-clamping:   terrain;        // terrain-following on

altitude-technique:  map;            // clamp features to the map data

altitude-resolution:0.005;          // [optional] resolutionof map data to

### **表面附着****drape**

附着是叠加编译的几何图形到地形表面的过程，很像在不平整的表面上“铺”一条毯子的过程。osgEarth做这样一件事情，将要素呈现为一个纹理
 (RTT) 然后将纹理投影到地形上。

表面附着有其优点和缺点：

•表面附着完美将要素贴合到地形上这样就不要考虑分辨率或镶嵌。

•在呈现线条或多边形的边时，你可能会得到交错的工件。被投影的纹理的大小是有限的，要覆盖越大的面积，影响的分辨率就越小了。这意味着表面附着功能对于多边形来说比线更有用。

 •意外的混合可能会导致从是由于太多使用表面附着的透明几何图形相互压盖造成的。

您可以像这样激活表面附着：

altitude-clamping:   terrain;        // terrain-following on

altitude-technique:  drape;          // drape features with a projectivetexture

### **GUP****固定**

GPU 固定实现近似地形跟随使用 GPU着色器。它使用一种两相技术： 首先它要使用深度现场取样固定到地形皮肤的每个顶点的顶点着色器 ；第二，它适用于深度偏移算法在片段渲染器来减轻 z 战斗。

GPU 固定也有它的取舍：

•它是非常适合的行 （或甚至三角的线），但较少的多边形因为它需要要做很好的近似固定tessellate 的多边形的内部。

 •它是快速、完全地在运行时，会发生和充分利用了 GPU 的并行处理。

 •没有锯齿状边缘效果，如在悬垂。

设置 GPU 固定像这样：

altitude-clamping:   terrain;        // terrain-following on

altitude-technique:  gpu;            // clamp and offset feature data onthe

## 渲染大数据集

功能数据加载到 osgEarth的最简单方法是像这样：

<modelname="shapes">

   <features name="data"driver="ogr">

      <url>data.shp</url>

   </features>

   <styles>

      data {

          fill: #ffff00;

      }

   </styles>

</model>

我们只是加载每个要素中的联接和他们所有黄色。

这工作正常每一点— — 在其中 osgEarth （和 OSG） 成为重载具有太多几何形状的点。即使与优化采用了osgEarth 的几何编译器，一个足够大的数据集可以耗尽系统资源。

解决办法，是功能平铺和分页。这里是如何配置它。

### **要素布局**

功能显示布局激活分页和瓷砖的特征数据。让我们修改前面的示例：

<modelname="shapes">

   <features name="data"driver="ogr">

      <url>data.shp</url>

   </features>

   <layout>

      <tile_size_factor>15.0</tile_size_factor>

       <level name="only"max_range="100000"/>

   </layout>

   <styles>

      data {

          fill: #ffff00;

      }

   </styles>

</model>

<layout>元素的存在激活页面。这意味着而不是加载和编译在加载时，将加载功能的数据，一次在后台编译应用程序已启动。之前特征数据显示在现场，其复杂程度不同，可能会有延迟。

<level>元素内的平铺的布局玩偶和明细数据的级别的存在。如果您省略级别，数据仍将在后台加载，但是它将加载一次。与一个或多个的水平，osgEarth将分手成瓷砖在一个或多个级别的明细数据特征数据和单独页中的那些拼贴。下面的更多。

### **层次**

每个级别描述该级别的详细程度。这就是在这个详细程度的级别下渲染瓦片时摄像机的范围
 ( min_range和max_range） 。但每个图块有多大？这是基于平铺范围的因素计算的.

Tile_range_factor根据max_range的LOD 的大小确定瓦片的大小。平铺范围因素是半径的倍数，倍数受LOD
 的max_range的影响。换句话说：

tileradius = max range / tile size factor.

默认瓷砖范围因素是15.0.

结果就是：如果你想要更大的瓦片，减小的平铺范围因素。对于范围较小的瓦片，增加的平铺范围因素。

你为什么关心瓦片大小？因为您的数据的密度会影响在每个图块中几何图形的数量。而且由于OSG（OpenGL，实际上）受益于能够发送大量的类似的几何图形到显卡上，调整图块大小有助于提高性能和数据吞吐量。不幸的是osgEarth没有办法提前知道到底什么样的瓦片大小将是"最好的"；所以，你有机会使用此设置来调整。

### **多层次和选择器的使用**

您可以有任意数目的层次——但请牢记它与地形图像不同，要素数据并不构成在其中更高 LOD 取代较低的四叉树。相反，要素级别是相互独立的。因此，如果您想要再放大一点时绘制更多详细的信息，你要使用选择器来决定每一步您想要绘制的。

这有一个示例。说我们绘图的道路。我们有道路类型存储在一个称为roadtype的属性中的shapefile文件。

<layout>

   <tile_size_factor>15.0</tile_size_factor>

   <crop_features>true</crop_features>

    <level name="highway"max_range="100000">

       <selectorclass="highway">

          <query>

             <expr>roadtype ='A'</expr>

          </query>

       </selector>

    </level>

    <level name="street"max_range="10000">

       <selectorclass="street">

          <query>

             <expr>roadtype ='B'</expr>

          </query>

       </selector>

    </level>

</layout>

<styles>

   highway {

       stroke:       #ffff00;

       stroke-width: 2.0;

   }

   street {

       stroke:       #ffffff7f;

       stroke-width: 1.0;

   }

</styles>




