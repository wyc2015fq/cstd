# FeatureLayer，FeatureDataset，FeatureClass，Feature的概念 - 毛小亮 - 博客园
# [FeatureLayer，FeatureDataset，FeatureClass，Feature的概念](https://www.cnblogs.com/xianerwonder/p/4230129.html)
刚学AE，其中很多概念都模糊不清。经过一段时间的摸索总结，对FeatureLayer，FeatureDataset，FeatureClass，Feature几个概念有了一点认识。拿出来分享一下，有错误的请多多指教，共同进步.
******FeatureLayer***:*** 要素图层，承载要素数据。几个重要的接口如下：
**IFeatureLayer**接口，用于管理要素图层的数据源，即要素类（FeatureClass）。具体察看IFeatureLayer.DataSourceType条目。  
                             IFeatureLayer::FeatureClass属性可以返回要素图层使用的要素类。
                             IFeatureLayer::search方法，返回一个ICursor对象，仅仅指向一个要素指针。
**IFeatureSelection**接口负责管理一个图层中的要素的选择集的方法和属性。
                            IFeatureSelection：SelectFeatures使用一个过滤器把符合要求的要素放入图层的选择中（IEnumFeature）。
**ILayerFields**接口，可以直接获得一个要素图层的要素类字段结构。
***FeatureDataset***: 先看官方解释：A FeatureDataset is a container for feature classes that share the same spatial refrence.
                          要素数据集对象在GeoDatabase是一个简单的要素类的容器，在这个容器内放置着相同空间参考的要素类
***FeatureClass***：（要素类）是用来存储空间数据的对象类，是ObjectClass的扩展。在一个要素类中的所有的要素都使用同样的字段结构，要素类与表，对象类最大的区别是它有一个几何字段，即Shape字段，用于存储要素的几何信息，这样使得用户可以在地图上看到一个要素的形状和位置。在地图上，要素类是离散对象，”小”对象(small objects)表示为点要素，”长”对象表示为线要素，”大”对象表示为多边形要素.(不要老往Shapefile上面想)，不包含Raster。
**IFields**接口，字段集可以通过ITable：：Fields获取（IFeatureClass.Fields）IFields有2个属性，其中一个是Field(index),通过传入不同的字段索引值可以返回某个字段。两个方法，一个是FindField，用字段名作为字段的索引。IField接口是字段对象的主要接口，可以个从这个接口中得到字段的主要属性，如名称、别名，字段的值域，字段的类型等。
***Feature：***是要素类中一条记录，主要接口是IFeature，定义了要素对象特有的属性。IFeature ：：get_Value（i）可以获得索引值为i的要素的属性值。通过循环就可以获得所有属性值。
还是图最清楚：
![](https://images.cnblogs.com/cnblogs_com/secenes/feature.jpg)

