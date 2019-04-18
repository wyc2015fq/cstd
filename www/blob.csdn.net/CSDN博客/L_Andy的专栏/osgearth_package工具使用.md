# osgearth_package工具使用 - L_Andy的专栏 - CSDN博客

2015年11月11日 14:52:06[卡哥](https://me.csdn.net/L_Andy)阅读数：1661


# 1、osgearth_package工具介绍

     osgearth源码工程中包含了osgearth_package工具，此工具用于切割DEM、DOM并形成金字塔。还可指定生成tms.xml。

# 2、DEM数据获取

## 2.1、数据源

[SRTM Data Search](http://srtm.csi.cgiar.org/SELECTION/inputCoord.asp)

## 2.2、获取方式

    此处将世界切割为多个正方形小格，每个格子代表一个区域的高程。

    在地图上选择想要下载的地点数据，“Click Hear to Begin Search"，进入下载页面。

# 3、数据切割

## 3.1、工具介绍

      此工具用于切割DEM、DOM并形成金字塔。

## 3.2、命令行

     eg1:osgearth_package gdal_my.earth --tms--max-level 20 --out g:\oren\testAir\tms\

     eg2:osgearth_package --tms G:\shanghai\earthfile\test1.earth --max-level 6 --ext png --out G:\shanghai\earthfile\tms\

    参数说明：

     gdal_my.earth：该earth文件中包含了被切割数据的描述。最好仅仅包含被切割数据的描述。

     --tms:切割完成后，生成tms.xml，用于tms的发布。

     --max-level 20 :最高切割多少级别。这里描述最高切割20级，也就是0~19。

     --out :生成的结果存放路径。

## 3.3、.earth文件描述

**[html]**[view
 plain](http://blog.csdn.net/liminled/article/details/18261953#)[copy](http://blog.csdn.net/liminled/article/details/18261953#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/176883/fork)

- <mapname="Feature Geometry Demo"type="projected"version="2">
- <heightfieldname = "fj"driver = "gdal"> // 最终数据的目录名就叫fj  
- <url>..\srtm3</url>
- <tile_size>32</tile_size>
- </heightfield>
- </map>

## 3.4、结果介绍

     最终的生成结果包含了被切割的数据以及tms.xml，方便发布成tms服务。

# 4、tms服务发布

## 4.1、发布工具

      apache tomcat。

## 4.2、发布方式

      直接生成的数据存放在tomcat的webapp目录下，最好将结果放在一个目录中，便于查找、索引。

      再点击“startup.bat”，能够通过浏览器查看到tms.xml就成功。

## 4.3、本机模拟tms并访问数据

      将切割生成的fj目录，拷贝到apache tomcat的webapp目录下，启动apache tomcat的startup.bat，在ie中访问http://localhost:8080/fj/tms.xml，可以看到tms.xml内容即可。然后在.earth文件中配置

      <elevation name="ReadyMap.org - Elevation" driver="tms">

        <url>http://localhost:8080/fj/tms.xml</url>

    </elevation>

