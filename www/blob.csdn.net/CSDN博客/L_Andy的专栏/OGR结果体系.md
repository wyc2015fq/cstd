# OGR结果体系 - L_Andy的专栏 - CSDN博客

2015年12月30日 14:36:42[卡哥](https://me.csdn.net/L_Andy)阅读数：2495


在网上看到这篇文章写得非常全面，而且是中文的，就转过来了，学习一下
其中有部分内容是重复的，不过不影响，非常好的资料

主要转自http://blog.csdn.net/liminlu0314?viewmode=contents

# 一、简介

## 1.GDAL

简单地说，GDAL是一个操作各种栅格地理数据格式的库。包括读取、写入、转换、处理各种栅格数据格式（有些特定的格式对一些操作如写入等不支持）。它使用了一个单一的抽象数据模型就支持了大多数的栅格数据（GIS对栅格，矢量，3D数据模型的抽象能力实在令人叹服）。当然除了栅格操作，这个库还同时包括了操作矢量数据的另一个有名的库[ogr](http://ogr.maptools.org/)（ogr这个库另外介绍），这样这个库就同时具备了操作栅格和矢量数据的能力。

最重要的是这个库是跨平台的，开源的！如今这个库对各种数据格式的支持相当强大，几乎所有的GIS、RS所要处理的数据格式类型都包括在内。它的主页最后那些使用了它作为底层数据处理的软件列表中，不仅有[GRASS](http://grass.itc.it/index.html)、[Quantum
 GIS (QGIS)](http://qgis.sourceforge.net/)，更有[Google Earth](http://earth.google.com/)。

学习这个库是十分有必要的。该库在C/C++/python/ruby/VB/java/C#(这个暂时不完全支持)下都能用，而且都一样用。

GDAL(Geospatial Data Abstraction Library)是一个在X/MIT许可协议下的开源栅格空间数据转换库。它利用抽象数据模型来表达所支持的各种文件格式。它还有一系列命令行工具来进行数据转换和处理。

## 2.Ogr

OGR是GDAL项目的一个分支，功能与GDAL类似，只不过它提供对矢量数据的支持。 　　有很多著名的GIS类产品都使用了GDAL/OGR库，包括ESRI的ArgGIS 9.2，Google Earth和跨平台的GRASS GIS系统。 　

OGR提供对矢量数据格式的读写支持，它所支持的文件格式包括：ESRI Shapefiles， S-57， SDTS， PostGIS，Oracle Spatial， Mapinfo mid/mif ， Mapinfo TAB。

### 1)OGR体系结构

OGR包括如下几部分：

Geometry：类Geometry (包括OGRGeometry等类)封装了OpenGIS的矢量数据模型，并提供了一些几何操作，WKB(Well Knows Binary)和WKT(Well Known Text)格式之间的相互转换，以及空间参考系统(投影)。

Spatial Reference：类OGRSpatialReference封装了投影和基准面的定义。

Feature：类OGRFeature封装了一个完整feature的定义，一个完整的feature包括一个geometry和geometry的一系列属性。

Feature Definition：类OGRFeatureDefn里面封装了feature的属性，类型、名称及其默认的空间参考系统等。一个OGRFeatureDefn对象通常与一个层(layer)对应。

Layer：类OGRLayer是一个抽象基类，表示数据源类OGRDataSource里面的一层要素(feature)。

Data Source：类OGRDataSource是一个抽象基类，表示含有OGRLayer对象的一个文件或一个数据库。

Drivers：类OGRSFDriver对应于每一个所支持的矢量文件格式。类OGRSFDriver由类OGRSFDriverRegistrar来注册和管理。

### 2) OGR的Geometry模型

OGR的Geometry模型是建立在OpenGIS的简单要素数据模型之上的。如下图所示：

![](https://img-my.csdn.net/uploads/201204/12/1334236060_4254.png)

图-OGR的Geometry模型关系图

OpenGIS的简单要素数据模型，其关系图如下所示：

![](https://img-my.csdn.net/uploads/201204/12/1334236067_2515.png)

图-OpenGIS的简单要素数据模型

由上面两图的对比，可以清楚的看到，OGR的Geometry模型是严格遵循OpenGIS的简单要素数据规范的。OGR的Geometry模型不仅在继承体系上与OpenGIS的简单要素数据模型一致，在函数接口上也向其靠拢，从基本的获取Geometry对象信息的方法如Dimension( )、GeometryType ( )、SRID ( )、Envelope( )、AsText( )、Boundary( )等到判定空间未知关系的方法如Equals(anotherGeometry:Geometry)、Disjoint(anotherGeometry:Geometry)、Intersects(anotherGeometry:Geometry)、Touches(anotherGeometry:Geometry)等都是符合其标准的。

### 3）OGR的API使用范例

以下C++示例代码展示了使用OGR的API来读取矢量数据。

**[cpp]**[view
 plain](http://blog.csdn.net/rrrrssss00/article/details/7455422#)[copy](http://blog.csdn.net/rrrrssss00/article/details/7455422#)

- #include "ogrsf_frmts.h"
- int main()  
- {      
- //注册所有的文件格式驱动    
- OGRRegisterAll();      
- //打开point.shp文件    
- OGRDataSource *poDS = OGRSFDriverRegistrar::Open( "point.shp", FALSE );      
- //获取点层    
- OGRLayer *poLayer = poDS->GetLayerByName( "point" );      
- OGRFeature *poFeature;      
- //重置该层，确保从层的开始读取数据    
- poLayer->ResetReading();      
- while ( (poFeature = poLayer->GetNextFeature()) != NULL )      
- {          
- //获取该层的属性信息        
- OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();          
- int iField;          
- for ( iField = 0; iField < poFDefn->GetFieldCount(); iField++ )   
- {       
- //获取某一个字段的信息            
- OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( iField );            if( poFieldDefn->GetType() == OFTInteger )                  
- printf( "%d,", poFeature->GetFieldAsInteger( iField ) );          
- }          
- OGRGeometry *poGeometry;          
- //获取feature        
- poGeometry = poFeature->GetGeometryRef();   
- //用wkbFlatten宏把wkbPoint25D类型转换为wkbPoint类型        
- if( poGeometry != NULL             && wkbFlatten(poGeometry->getGeometryType()) == wkbPoint )     
- {              
- OGRPoint *poPoint = (OGRPoint *) poGeometry;              
- printf( "%.3f,%3.f\n", poPoint->getX(), poPoint->getY() );          
- }          
- else
- printf( "no point geometry\n" );          
- //销毁feature        
- OGRFeature::DestroyFeature( poFeature );      
- }      
- //销毁数据源，以便关闭矢量文件    
- OGRDataSource::DestroyDataSource( poDS );  
- }  

利用GDAL/OGR库，可以提供对矢量和栅格文件数据的支持。

