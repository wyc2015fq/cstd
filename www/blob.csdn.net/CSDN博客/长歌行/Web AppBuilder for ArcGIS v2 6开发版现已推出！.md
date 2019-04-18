# Web AppBuilder for ArcGIS v2.6开发版现已推出！ - 长歌行 - CSDN博客





2017年10月15日 16:25:23[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：323
所属专栏：[Esri技术谈](https://blog.csdn.net/column/details/17488.html)









[原文地址](https://blogs.esri.com/esri/arcgis/2017/10/12/web-appbuilder-for-arcgis-v2-6-developer-edition-now-available/)

by Law on October 12, 2017

FYI：ArcGIS v2.6开发版的Web AppBuilder现在可从[ArcGIS开发人员网站](https://developers.arcgis.com/web-appbuilder/)下载。

![这里写图片描述](https://blogs.esri.com/esri/arcgis/files/2017/10/WAB_DEv26.png)

此版本包括最近在上个月在ArcGIS Online中添加到嵌入式Web AppBuilder for ArcGIS的新功能。请参阅此博客文章了解更多详情[What’s New in Web AppBuilder for ArcGIS (September 2017).](https://blogs.esri.com/esri/arcgis/2017/09/21/whats-new-in-web-appbuilder-for-arcgis-september-2017/)

在SDK端，添加了以下类和方法：

[LayerNode](http://developers.arcgis.com/web-appbuilder/api-reference/layernode.htm) - 此类是ArcGIS API for JavaScript中的层对象的抽象。它包含对父层或子层的引用，并允许您使用统一接口访问层，而不管根层，子层或层类型（如FeatureLayer，ArcGISDynamicMapServiceLayer，KMLLayer等）。它替代已被弃用的LayerInfo类。

[LayerStructure](http://developers.arcgis.com/web-appbuilder/api-reference/layerstructure.htm) - 此类反映当前地图中操作层的结构。它可以用于遍历和操作Layer Layer中的每个层和子层作为LayerNode进行抽象。它替代已被弃用的LayerInfos类。

[Utils](http://developers.arcgis.com/web-appbuilder/api-reference/utils.htm) - getDisplayValueForCodedValueOrSubtype方法获取编码值和子类型字段的最佳显示值。



