# Shader山下（二十六）Forward渲染路径中的灯光信息 - 凯奥斯的注释 - CSDN博客





2017年12月07日 20:59:51[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：495
所属专栏：[Shader山下](https://blog.csdn.net/column/details/12981.html)









以下摘录自[《Unity 3D ShaderLab开发实战详解》](https://book.douban.com/subject/25892263/)，7.7.1-7.7.2。 

这两类Pass（转注：ForwardBase和ForwardAdd）不能再Camera为VertexLit是被渲染，但可以被渲染路径为Forward和Deferred的Camera渲染。

# 存放光源的第二种方式

在ForwardBase的Pass内，_WorldSpaceLightPos0和_LightColor0只会含有第一个Pixel平行光（转注：Pixel光源即Render Mode为Important的Light，Vertex光源即Render Mode为Not Important的Light）的有效组合。在没有Pixel平行光时，_WorldSpaceLightPos0也可能含有一个Pixel点光源的位置数据，但是并没有有效的_LightColor0数据。在ForwardAdd内，_WorldSpaceLightPos0和_LightColor0只会含有Pixel光源，并且按照强度大小、平行光优先的顺序一次对每一个Pixel光源执行一次。

# 存放光源的第一种方式

在ForwardBase内，当没有ForwardAdd Pass是，unity_4LightPos[X, Y, Z]0（转注：unity_4LightPosX0、unity_4LightPosY0和unity_4LightPosZ0的简写）和unity_LightColor[4]的组合含有点光源的数据，按照Pixel强度的原则排序，但不包含平行光；当有ForwardAdd时，则ForwardBase内unity_4LightPos[X, Y, Z]0只含有Vertex点光源。在ForwardAdd内，unity_4LightPos[X, Y, Z]0不包含有效数据。unity_LightAtten0数据则用在1.0和0.0之间切换，表示对应的unity_4LightPos[X, Y, Z]0是否有效的光源数据（转注：所谓切换应该是在ForwardBase和ForwardAdd 之间切换）。

# 存放光源的第三种方式

unity_LightPosition[4]在ForwardBase和ForwardAdd内从不包含任何有效数据，奇怪的是其x分量总是为1.0，只有在Camera的VertexLit模式切换回Forward和Deferred模式时才会包含一些未及时清除的死数据。 
*（转注：因尊重原作对光源存放方式的定义和叙述顺序，故以上标题略显怪异。）*

# 光源计算

讲了这么多，其实对于Forward渲染路径，你可以简单地使用UnityCG.cginc中的Shade4PointLights函数在ForwardBase中计算Vertex光源的光照。对于Pixel光源，你必须使用_WorldSpaceLightPos0和_LightColor0来计算。



