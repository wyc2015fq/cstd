# osgearth earth文件规范-符号参考 - L_Andy的专栏 - CSDN博客

2016年02月26日 12:10:54[卡哥](https://me.csdn.net/L_Andy)阅读数：1238



符号参考

osgEarth用样式表渲染要素和注记。

本文档列出了可在样式表中使用的所有符号属性。不是每个符号是适用于每种情况；这只是一个主列表。

跳转到符号：

•[Geometry](http://docs.osgearth.org/en/latest/references/symbology.html?highlight=feature%20symbol#geometry)

 •[Altitude](http://docs.osgearth.org/en/latest/references/symbology.html?highlight=feature%20symbol#altitude)

 •[Extrusion](http://docs.osgearth.org/en/latest/references/symbology.html?highlight=feature%20symbol#extrusion)

 •[Icon](http://docs.osgearth.org/en/latest/references/symbology.html?highlight=feature%20symbol#icon)

 •[Model](http://docs.osgearth.org/en/latest/references/symbology.html?highlight=feature%20symbol#model)

 •[Render](http://docs.osgearth.org/en/latest/references/symbology.html?highlight=feature%20symbol#render)

 •[Skin](http://docs.osgearth.org/en/latest/references/symbology.html?highlight=feature%20symbol#skin)

 •[Text](http://docs.osgearth.org/en/latest/references/symbology.html?highlight=feature%20symbol#text)

**开发人员注意:**

在SDK 中，符号在osgEarth::Symbology命名空间中，每个符号类是以类似AltitudeSymbol这样的形式。下面的属性就是他们出现在earth文件中；在
 SDK中，可以以LineSymbol::strokeWidth()等形式通过存取器获得属性。

值类型

这些是基本值类型。在此页的符号表中，各个属性包括其说明后的括号中的值类型。

float：浮点数

带单位的float：带单位指示符的浮点数，例如
 20px 的浮点数 （20 像素） 或 10 米 （10 米）

HTML_Color：十六进制格式的颜色字符串，如HTML所使用的方式如。像#RRGGBB或
 #RRGGBBAA 的格式。（示例： #FFCC007F）

integer：整形数据

numeric_expr：可解析为一个数的表达式
 （简单或JavaScript）

string：简单的文本字符串

string_expr：可解析为一个文本字符串的表达式
 （简单或JacaScript)

uri_string：

用于表示资源位置 （如 URL 或文件路径）的字符串，Uri可以是绝对或相对 ；相对 Uri 始终是所处位置的引用，即，请求该资源的实体。（例如，earth文件内的相对URI
 将是earth文件本身的所处位置。

## 几何图形

基本几何符号（SDK：LineSymbol， PolygonSymbol， PointSymbol） 控制矢量数据的颜色和样式。

|属性|说明|值|
|----|----|----|
|fill|为多边形填充颜色。|HTML 颜色|
|stroke|线颜色 （或多边形的轮廓颜色，如果存在fill参数）|HTML 颜色|
|stroke-width|线宽|带单位的float|
|stroke-min-pixel|最低渲染宽度 ；防止线变薄比以像素为单位的该值更小仅适用于当stroke-width不是以像素为单位时|float(像素)|
|strok-tessellation|细分线的次数|integer|
|stroke-linejoin|加入 polygonized 线样式。仅适用与stroke-width是在世界单位 (而不是以像素为单位)|miter，round|
|stroke-linecap|Polygonized 线帽样式。仅适用与stroke-width是在世界单位 (而不是以像素为单位)|square，flat，round|
|stroke-rounding-ratio|用于联接和caps为round，也是圆角上分辨率。值是到角线段长度线宽度的比例。|float(0.4)|
|stroke-stipple-pattern|点模式下的位掩码。每个设置的位模式中表示像"On"。|integer(65535)|
|stroke-stipple-factor|点画像素宽度的线的因素。数重复描模式中的每一位|integer(1)|
|point-fill|填充颜色的点。|HTML 颜色|
|point-size|GL 几何图形上点的尺寸|float（1.0）|

## 高程

海拔高度符号（SDK：AltitudeSymbol） 控制与它的位置根据地形功能交互。

|属性|说明|
|----|----|
|altitude-clamping|控制地形跟随行为。none:不使用固定terrain: 固定地形并减去Z值relative:固定地形并加上Z值absolute:要素的Z包含其Z值的绝对值.|
|altitude-technique|当altitude-clamping设置为terrain时，请选择一个贴合地形技术:map:几何图形紧贴到地图上的高程数据drape:用投影纹理紧贴几何图形地图上的高程数据gpu:在GPU上把几何图形紧贴到地形上scene:重新固定几何图形到新分页的图块上 (仅适用于注记)|
|altitude-binding|当altitude-technique为map时在Granularity上采样：vertex:固定每个顶点centroid:只固定每个要素的质心|
|altitude-resolution|当altitude-technique为map时采样高程数据使用的分辨率(float)|
|altitude-offset|几何图形的 Z 值的垂直偏移量|
|altitude-scale|几何图形 Z值的比例因子|


提示： 您可以也使用快捷方式激活表面附着或 GPU固定；设置altitude-clamping为terrain-drape或terrain-gpu。

## 挤出

挤压符号（SDK：ExtrusionSymbol） 指示 osgEarth 源矢量数据 ； 从创建拉伸几何挤出一个 2D 向量变成 3D形状。注：挤压属性的简单的存在将使挤压。

|属性|说明|
|----|----|
|extrusion-height|拉伸矢量数据到多远（numeric-expr）|
|extrusion-flatten|是否强制所有挤出到相同的 Z 值 (bool) 顶点。例如，如果您挤出多边形为 3D 建筑物，将此设置为true将强制屋顶为平的即使基底地形不是平的。(bool)|
|extrusion-wall-gradient|与 3D 形状的基底上挤出的几何图形的填充颜色属性（fill）的值相乘的因子。这将导致3D 形状底部比顶部更暗形成好的效果。（float [0..1] ； 试试 0.75）|
|extrusion-wall-style|相同的样式表中osgEarth 应适用于挤出形状的墙壁上的另一种风格的名称。（string）|
|extrusion-roof-style|相同的样式表中osgEarth 应适用于屋顶挤出形状的另一种风格的名称。（string）|

## 皮肤

皮肤符号（SDK：SkinSymbol） 适用于几何图形，适用时的纹理映射。（此刻这仅适用于挤压几何)。

|属性|说明|
|----|----|
|skin-library|包含皮肤的资源库的名称|
|skin-tags|字符串的集（以空格分隔多个资源标记。在选择要应用的纹理外观时，osgEarth 将通过这些标记限制皮肤的选择。如果您省略此属性，则默认选中所有皮肤。例如，如果你挤出的建筑物，你可能只想要纹理作用于building标签上。（字符串）|
|skin-tiled|当设置为true，osgEarth 只考虑选择一个有其tiled属性设置为true的皮肤。tiled属性表示皮肤可能用作重复纹理。(boolean)|
|skin-object-height|数值表达式解析为该要素的实际高度 （以米为单位）。osgEarth 将使用此值来缩小适应那样的高度的皮肤的选择范围（即，皮肤将贴在其值落在皮肤的最小/最大对象高度范围之间位置上。(numeric-expr)|
|skin-min-object-height|告诉 osgEarth，只使用物体最小高度不大于或等于此值的皮肤。(numeric-expr)|
|skin-max-object-height|告诉 osgEarth，只使用其物体最大高度是小于或等于此值的皮肤。(numeric-expr)|
|skin-random-seed|筛选操作完成后 （osgEarth 根据以上的属性，将确定适当皮肤要从中选择的最小集并随机选择其中之一。通过设置此种子值，您可以确保每次您运行应用时产生相同的"随机"选择。（integer）|

##  图标

图标符号（SDK：IconSymbol） 描述 2D 图标的外观。图标用于不同的东西，最常见的是：

•指向模型替换-几何替换图标

 •地方批注

|属性|说明|
|----|----|
|icon|图标图像的 URI。（uri-字符串）|
|icon-library|包含图标 （可选）资源库的名称|
|icon-placement|用于模型替换，描述osgEarth 应如何用图标替换几何形状：vertex:用图标替换几何图形上的每一个顶点interval:沿着几何图形按照icon-density属性有规律间隔地放置图标random:将随机内几何，图标放置根据图标密度属性。将随机内几何，图标放置根据图标密度属性。centroid:将单个图标放置在几何的质心。|
|icon-density|icon-placement 设置是interval 或random属性的设置是提示 osgEarth 应放置多少图标实例。单位就是"unit per km"(对于线性数据而言） 或者"units per square km"（对于对于多边形数据而言）。(float)|
|icon-scale|通过这一数值缩放图标 (float)|
|icon-heading|沿其中轴线 （float、度） 旋转图标|
|icon-declutter|激活此图标为decluttering 。osgEarth 将尝试自动显示或隐藏物体让他们不会在屏幕上重叠。(布尔值)|
|icon-align|设置icon与锚点相对的位置，合法的值是具有“水平-竖直”格式的，如下所示：•left-top •left-center •left-bottom •center-top •center-center •center-bottom •right-top •right-center •right-bottom|
|icon-random-seed|用于随机放置操作，设置此种子以便每次您运行应用程序该随机化是可重复的（integer）|
|icon-occlusion-cull|是否剔除被遮挡的图标，让它们在视线受地形阻挡时不显示|
|icon-occlusion-cull-altitude|当查看器达到这个高度时开始剔除视线受地形阻挡的图标|

## 模型

模型符号（SDK：ModelSymbol） 描述了外部的 3D 模型。像图标，模型，通常用于：

•指向模型替换-替换的 3D 模型的几何形状

 •模型注释

|属性|说明|
|----|----|
|model|3D 模型 （uri-字符串） 的 URI。此或模型库属性，但不是能同时使用。|
|model-library|资源库包含模型的名称。模型属性，但不是能同时使用此或。|
|model-placement|为模型代替，描述如何 osgEarth 应替换几何模型：vertex:替换模型在几何中的每个顶点。interval:将模型放在沿几何，定期根据模型密度属性。random:根据模型密度属性发生随机内的几何模型。centroid:一个单一的模型几何的质心在的地方。|
|model-density|model-placement 设置是interval 或random属性的设置是提示 osgEarth 应放置多少模型实例。单位就是"unit per km"(对于线性数据而言） 或者"units per square km"（对于对于多边形数据而言）。(float)|
|model-scale|按这个值沿所有轴 缩放模型 (float)|
|model-heading|作其 Z 轴正方向上的 旋转（float、degrees）|
|icon-random-seed|用于随机放置操作，设置此种子以便每次您运行应用程序 随机化是可重复的（integer）|


## 渲染器

渲染器的符号（SDK： RenderSymbol） 适用于一般的 OpenGL 渲染设置，以及一些osgEarth 特定的设置，并不特定于任何其他符号类型。

|属性|说明|
|----|----|
|render-depth-test|启用或禁用 GL 的深度测试。(boolean)|
|render-lighting|启用或禁用 GL 照明。(boolean)|
|render-depth-offset|启用或禁用深度偏移。深度偏移是 修改片段的深度值，模拟该对象在渲染过程中离查看器近或比实际的更远了的GPU 技术。它是一种减少 z-fighting的机制。(boolean)|
|render-depth-offset-min-bias|设置深度偏移 的最小修正（到查看器距离的偏移量）。通常可以设置此属性 ，所有其他的都将被自动设置。（float、 米）|
|render-depth-offset-max-bias|设置深度偏移的最小修正（到查看器距离的偏移量） 。|
|render-depth-offset-min-range|设置(到查看器的距离)范围的最小深度的修正。修正值浮动在指定范围内的最小和最大值之间。|
|render-depth-offset-max-range|设置(到查看器的距离)范围的最大深度的修正。修正值浮动在指定范围内的最小和最大值之间。|

## 文本

文本符号（SDK：TextSymbol） 控制的存在性和外观的文本标签。

|属性|说明|
|----|----|
|text-fill|前景颜色的文本 （HTML 颜色）|
|text-size|文本的大小（float、 像素为单位）|
|text-font|要使用 （依赖于系统） 的字体的名称。在Windows 例如，使用"arialbd"设置 Arial 粗体显示。|
|text-halo|文本的轮廓颜色 ，没有轮廓就一并省略此属性。（HTML 颜色）|
|text-halo-offset|轮廓厚度 （float、 像素为单位）|
|text-align|相对于其锚点的文本字符串的对齐方式：•left-top •left-center •left-bottom •left-base-line •left-bottom-base-line •center-top •center-center •center-bottom •center-base-line •center-bottom-base-line •right-top •right-center •right-bottom •right-base-line •right-bottom-base-line •base-line|
|text-layout|文本的布局：•ltr •rtl •vertical|
|text-content|要显示 的实际文本字符串（string-expr)|
|text-encoding|字符编码的文本内容：•utf-8 •utf-16 •utf-32 •ascii|
|text-declutter|激活此文本为decluttering 。osgEarth 将尝试自动显示或隐藏物体让他们不会在屏幕上重叠。(布尔值)|
|text-occlusion-cull|是否剔除被遮挡的文本，让它们在视线受地形阻挡时不显示|
|text-occlusion-cull-altitude|当查看器达到这个高度时开始剔除视线受地形阻挡的文本|

[](http://blog.csdn.net/giantchen547792075/article/details/15318863#)[](http://blog.csdn.net/giantchen547792075/article/details/15318863#)[](http://blog.csdn.net/giantchen547792075/article/details/15318863#)[](http://blog.csdn.net/giantchen547792075/article/details/15318863#)[](http://blog.csdn.net/giantchen547792075/article/details/15318863#)[](http://blog.csdn.net/giantchen547792075/article/details/15318863#)

