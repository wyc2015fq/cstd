# mysql空间扩展 VS PostGIS - zhanlijun - 博客园
# [mysql空间扩展 VS PostGIS](https://www.cnblogs.com/LBSer/p/3629149.html)
#### [http://www.cnblogs.com/LBSer/p/3629149.html](http://www.cnblogs.com/LBSer/p/3629149.html)
|**功能**|**Mysql spatial extension **|**PostGIS**|
|----|----|----|
|**空间索引**|仅MyISAM支持R树索引，InnoDB不支持|GIST树索引（R树的变种）|
|**支持的空间类型**|仅二维数据|二维、三维以及曲线|
|**空间操作函数**|有限的空间函数|基本实现OGC标准定义的空间操作函数|
|例：想查找蓝色多边形内的点，mysql空间扩展仅能查出在最小外包矩形（红色框）内的点，而postgis能查出任意多边形内的点。![](https://images0.cnblogs.com/i/522490/201403/272031413278166.png)| | |
|**空间投影**|不支持|支持多种常用投影坐标系|
|例：想查找两点间距离。MySQL Spatial仅能计算欧式空间距离，而PostGIS能计算不同投影坐标系下的真实空间距离| | |
|**事务支持**|不支持|PostGIS提供了一系列的长事务支持，可以有效支持复杂的空间分析功能|
|**查询效率**|加载速度: MySQL > PostGIS (事务) 空间索引的创建: MySQL < PostGIS (diff split algo) 查询: MySQL  PostGIS（不同性质查询结果不一样，各有千秋）| |
|**GIS****系统使用**|使用较少|使用较多，例如openstreetmap的数据库后台就是Postgresql+Postgis|
       小结：如果只做一些简单的GIS或者LBS的应用，MySQL提供的spatial extensions基本能够满足。但如果需要的功能更复杂一些，MySQL spatial extensions提供的功能可能就不够用了，这时Postgresql+postGIS可能将更加合适。

