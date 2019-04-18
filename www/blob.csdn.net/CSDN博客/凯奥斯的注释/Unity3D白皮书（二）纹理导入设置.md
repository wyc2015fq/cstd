# Unity3D白皮书（二）纹理导入设置 - 凯奥斯的注释 - CSDN博客





2017年08月07日 14:12:28[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：3648
所属专栏：[Unity3D白皮书](https://blog.csdn.net/column/details/13147.html)









# Unity3D中，纹理占据着很重要的位置，包括模型的贴图，UI的精灵等都需要使用纹理。而如何导入纹理，该怎样设置，变成了一个很重要的问题。

# 前一篇[Unity3D白皮书（一）模型导入设置](http://blog.csdn.net/ecidevilin/article/details/76616690)介绍了Unity3D中的模型导入设置，本文继续介绍U3D中纹理的导入设置。

（注：U3D版本5.6.1f1）

![](https://img-blog.csdn.net/20170807135134925)


如图所示，U3D中纹理的类型分为以上8种（Advanced这种类型已经被淹没在历史的尘埃里了）。



Default：默认最常用的纹理类型，大部分导入参数都可以访问。



Normal map：法线贴图。

Editor GUI and Legacy GUI：编辑器GUI和传统GUI。

Sprite(2D and UI)：精灵，用于2D对象和UGUI贴图。

Cursor：自定义光标。
Cookie：场景光的Cookie。



Lightmap：光照贴图，将贴图编码成特定的格式，并对纹理数据进行后处理。

SingleChannel：单通道。





我们首先看默认类型的设置：

![](https://img-blog.csdn.net/20170807135425315)




Texture Shape：纹理形状，2D或立方体。（详情参考[TextureImporter](https://docs.unity3d.com/Manual/class-TextureImporter.html)的Texture Shape部分）

sRGB(Color Texture)：指定纹理是否保存在[Gamma](https://www.zhihu.com/question/27467127)空间。

Alpha Source： α通道来源。

- None：强制无α通道。
- Input Texture Alpha：使用纹理自带的α通道。
- From Gray Scale：使用纹理RGB通道的均值来生成α通道。

Alpha Is Transparency：α通道是透明度。





Non Power of 2：NPOT的处理方式。

Read/Write Enabled：读写开关，非必要不开启，否则会增加一倍的内存。

Generate Mip Maps：生成[Mip Maps](https://baike.baidu.com/item/Mipmap/3722136)，会增加33%的内存。一般用于模型纹理，UI、天空盒等纹理不需要开启。

Border Mip Maps：防止低阶的Mip Map的色彩值溢出边界，一般用于光照Cookie。

Mip Map Filtering：过滤算法，Box和[Kaiser](https://en.wikipedia.org/wiki/Kaiser_window)。

Fadeout Mip Maps：根据层阶使Mip Map慢慢变灰，一般用于细节贴图（DetailMaps）。


Wrap Mode：循环（平铺）模式，当UV<0或>1时，如何采样。

- Repeat：重复。
- Clamp：固定。


Filter Mode：[滤波模式](http://smartblack.iteye.com/blog/762948)，当被3D变换拉伸时，纹理如何插值。

- Point(nofilter)：点（无过滤），马赛克。
- Bilinear：双线性，模糊。
- Trilinear：三线性，在Bilinear的基础上对不同的Mip层阶进行模糊（插值）。


AnisoLevel：各项异性滤波（Anisotropic Filtering ）等级，增加纹理在大倾角视角的质量，对底板和地表纹理效果很好。


Normal Map类型的纹理设置：


![](https://img-blog.csdn.net/20170807140050703)


与Default相比增加了一下设置：



Create from Grayscale：从灰度高度图（Heightmap）创建。

Bumpiness：崎岖度。

Filtering：滤波算法。

- Smooth：平滑，标准[前向](https://baike.baidu.com/item/%E5%89%8D%E5%90%91%E5%B7%AE%E5%88%86/600877)[差分](https://baike.baidu.com/item/%E5%89%8D%E5%90%91%E5%B7%AE%E5%88%86/600877)算法。
- Sharp：尖锐，[Sobel](https://baike.baidu.com/item/Sobel%E7%AE%97%E5%AD%90/11000092)滤波器。

Sprite类型的纹理设置：



![](https://img-blog.csdn.net/20170807140239489)


与Default相比增加了一下设置：



Sprite Mode：精灵模式。

- Single：单图。
- Multiple：多图。
- Polygon：多边形，在[SpriteEditor](https://docs.unity3d.com/560/Documentation/Manual/SpriteEditor.html)里使用多边形裁剪精灵。




PackingTag：指定图集。

PixelsPer Unit：每单位像素数，在世界场景中，每单位距离有多少个像素。

Mesh Type：网格类型（Polygon模式无此属性）。

- FullRect：矩形。
- Tight：紧凑的，根据Alpha通道生成Mesh。（2DObject）


ExtrudeEdge：拉伸边缘。

Pivot：轴心（仅Single），精灵内部坐标的原点。


Cookie类型的纹理设置：

![](https://img-blog.csdn.net/20170807140431603)


与Default相比增加了一下设置：



Light Type：光照类型。

- Spotlight：聚光灯，形状必须为2D。
- Directional：平行光，形状必须为2D。
- Point：点光源，形状必须为立方体。





最后，在这些纹理设置之下，还有针对不同平台的压缩设置：

![](https://img-blog.csdn.net/20170807140627616)




Max Size：最大尺寸。

Compression：压缩质量。

Format：压缩格式。

Use Crunch Compression：紧凑压缩。

­Compressor Quality：压缩质量。



