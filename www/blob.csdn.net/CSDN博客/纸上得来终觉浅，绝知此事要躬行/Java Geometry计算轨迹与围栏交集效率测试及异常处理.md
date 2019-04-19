# Java Geometry计算轨迹与围栏交集效率测试及异常处理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客
2019年04月17日 18:49:02[boonya](https://me.csdn.net/boonya)阅读数：7标签：[Java																[Geometry																[轨迹																[围栏																[交集](https://so.csdn.net/so/search/s.do?q=交集&t=blog)](https://so.csdn.net/so/search/s.do?q=围栏&t=blog)](https://so.csdn.net/so/search/s.do?q=轨迹&t=blog)](https://so.csdn.net/so/search/s.do?q=Geometry&t=blog)](https://so.csdn.net/so/search/s.do?q=Java&t=blog)
个人分类：[Java																[LBS位置服务](https://blog.csdn.net/boonya/article/category/6358231)](https://blog.csdn.net/boonya/article/category/878447)
**本文涉及异常问题比较普遍：**[https://cn.bing.com/search?q=found+non-noded+intersection+between&qs=n&form=QBRE&sp=-1&pq=found+non-noded+intersection+between&sc=0-36&sk=&cvid=B72C36C5CD9B45E392758021640A8E06](https://cn.bing.com/search?q=found+non-noded+intersection+between&qs=n&form=QBRE&sp=-1&pq=found+non-noded+intersection+between&sc=0-36&sk=&cvid=B72C36C5CD9B45E392758021640A8E06)
经测试，是由于使用方法不当造成的。
**目录**
[Geometry计算方法](#Geometry%E8%AE%A1%E7%AE%97%E6%96%B9%E6%B3%95)
[轨迹点与围栏关系计算逻辑](#%E8%BD%A8%E8%BF%B9%E7%82%B9%E4%B8%8E%E5%9B%B4%E6%A0%8F%E5%85%B3%E7%B3%BB%E8%AE%A1%E7%AE%97%E9%80%BB%E8%BE%91)
[轨迹在围栏内部](#%E8%BD%A8%E8%BF%B9%E5%9C%A8%E5%9B%B4%E6%A0%8F%E5%86%85%E9%83%A8)
[轨迹在围栏外部](#%E8%BD%A8%E8%BF%B9%E5%9C%A8%E5%9B%B4%E6%A0%8F%E5%A4%96%E9%83%A8)
[轨迹里程计算效率测试](#%E8%BD%A8%E8%BF%B9%E9%87%8C%E7%A8%8B%E8%AE%A1%E7%AE%97%E6%95%88%E7%8E%87%E6%B5%8B%E8%AF%95)
[Geometry轨迹围栏交集计算效率](#Geometry%E8%BD%A8%E8%BF%B9%E5%9B%B4%E6%A0%8F%E4%BA%A4%E9%9B%86%E8%AE%A1%E7%AE%97%E6%95%88%E7%8E%87)
[Geometry两个之间距离计算效率](#Geometry%E4%B8%A4%E4%B8%AA%E4%B9%8B%E9%97%B4%E8%B7%9D%E7%A6%BB%E8%AE%A1%E7%AE%97%E6%95%88%E7%8E%87)
[异常关注](#%E5%BC%82%E5%B8%B8%E5%85%B3%E6%B3%A8)
[调用方式](#%E8%B0%83%E7%94%A8%E6%96%B9%E5%BC%8F)
[正常情况](#%E6%AD%A3%E5%B8%B8%E6%83%85%E5%86%B5)
[异常情况](#%E5%BC%82%E5%B8%B8%E6%83%85%E5%86%B5)
[异常分析](#%E5%BC%82%E5%B8%B8%E5%88%86%E6%9E%90)
[避免异常](#%E9%81%BF%E5%85%8D%E5%BC%82%E5%B8%B8)
[Github项目](#Github%E9%A1%B9%E7%9B%AE)
# **Geometry计算方法**
## **轨迹点与围栏关系计算逻辑**
将轨迹分成两部分进行运算，即在围栏的内部和外部，笼统地都可以用geometry求交集。
### **轨迹在围栏内部**
![](https://img-blog.csdnimg.cn/20190417184459475.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
几何关系：包含&相交。
**计算步骤：**
- 判断是否包含
- 判断是否相交
- 前两步都为真，则计算轨迹里程等于围栏内运动里程，围栏外里程为0。
### **轨迹在围栏外部**
![](https://img-blog.csdnimg.cn/20190417184516535.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
几何关系：非包含&不相交。
**计算步骤：**
- 判断是否包含
- 判断是否相交
- 前两步都为真，则计算轨迹里程等于围栏外运动里程，围栏内里程为0。
## **轨迹里程计算效率测试**
### **Geometry轨迹围栏交集计算效率**
此种计算方式需要做两个维度的计算
- 轨迹本身的里程
- 交叉里程（围栏内里程）
**轨迹点的计算时间复杂度： T(n)=O(f(n-1));**
**轨迹与围栏交集里程时间复杂度： T(n)=O(f(n));**
|**轨迹点个数**|**耗时(ms)**|**平均耗时(ms)**|
|----|----|----|
|100|485,24,28,33,43,26,70,34,52,39,|83|
|300|545,38,34,30,33,27,25,42,32,33,|83|
|500|570,40,34,43,32,29,49,42,28,27,|89|
|1000|580,41,33,31,30,37,30,28,27,28,|86|
|2000|641,38,41,52,34,51,42,40,29,28,|99|
|3000|633,46,53,38,29,31,32,58,25,23,|96|
|5000|619,53,57,33,39,76,32,38,25,30,|100|
|8000|660,46,45,64,33,35,35,49,40,32,|103|
|10000|695,62,60,60,33,47,43,50,31,28,|110|
### **Geometry两个之间距离计算效率**
此种方式只需要计算轨迹的里程，计算过程中区分围栏内里程和围栏外里程。
**轨迹点的计算时间复杂度： T(n)=O(f(n-1));**
|**轨迹点个数**|**耗时(ms)**|**平均耗时(ms)**|
|----|----|----|
|100|363,24,33,14,16,17,14,14,11,10,|51|
|300|449,55,41,33,26,20,20,13,30,17,|70|
|500|451,52,31,38,36,34,27,28,25,32,|75|
|1000|687,72,65,62,53,75,48,47,62,464,|163|
|2000|636,100,95,105,81,97,101,244,81,111,|165|
|3000|695,128,136,116,190,145,154,95,170,156,|198|
|5000|802,301,326,222,270,210,251,497,206,268,|335|
|8000|1075,464,437,497,487,489,527,494,490,362,|532|
|10000|1107,1148,587,683,534,632,639,580,644,741,|729|
注：这里耗时体现在每次都对点是否在围栏里面进行判断。
# 异常关注
## 调用方式
错误的方式：
![](https://img-blog.csdnimg.cn/20190417183414130.png)
正确使用方式：
![](https://img-blog.csdnimg.cn/20190417183421726.png)
注：下面产生异常的方式主要是方法调用产生的异常，导致geometry与geometry之间判断相交产生冲突。
## **正常情况**
**正常有无交集**
LINESTRING (116.25219844906536 39.906786278366184, 116.25244821464976 39.90685660540906,.......)
POLYGON ((116.25535777758334 39.915633025102814, 116.2574767832826 39.92280683703119, 116.26090814827204 39.931584334357126, 116.26145755480688 39.94115959034832, 116.26261596265554 39.94975783123877, 116.25535777758334 39.915633025102814))
lineStringContainsLineString=true
**polygonContainsPolygon=true**
================>开始....
线的长度:0.7536681662645706
线轨迹的实际里程(米):63327.0
面的面积(平方米):493080.36165995616
两个空间对象最近的距离:0.003595942642908063
两个几何对象的交集:LINESTRING EMPTY
围栏内里程(米):0.0
围栏外里程(米):63327.0
================>耗时641ms,points=1000
================>开始....
线的长度:0.7536681662645706
线轨迹的实际里程(米):63327.0
面的面积(平方米):493080.36165995616
两个空间对象最近的距离:0.003595942642908063
两个几何对象的交集:LINESTRING EMPTY
围栏内里程(米):0.0
围栏外里程(米):63327.0
================>耗时102ms,points=1000
================>开始....
线的长度:0.7536681662645706
线轨迹的实际里程(米):63327.0
面的面积(平方米):493080.36165995616
两个空间对象最近的距离:0.003595942642908063
两个几何对象的交集:LINESTRING EMPTY
围栏内里程(米):0.0
围栏外里程(米):63327.0
================>耗时111ms,points=1000
**正常有交集**
**lineStringContainsLineString=true**
**polygonContainsPolygon=true**
================>开始....
线的长度:0.7621779140342029
线轨迹的实际里程(米):62687.0
面的面积(平方米):808894.4476752883
两个空间对象最近的距离:0.0
两个几何对象的交集:MULTILINESTRING ((116.25688221662699 39.914395421882276, 116.25758584647397 39.91484391938795, 116.25822427104575 39.91560473167323, 116.25860369811619 39.91625944161212, 116.2587542555323 39.917155419248814, 116.25934281853155 39.91740573376146, 116.26017238150571 39.91837305892461, 116.26047989801104 39.918917913968706), (116.26066125935785 39.91908979976905, 116.26139721203687 39.91909890945397, 116.261493614772 39.91969117292497, 116.26209510309556 39.919864426634085, 116.26267162465133 39.920768643283466, 116.26283447817538 39.92087285535724, 116.26320152504029 39.92117941999886, 116.26371154759461 39.92198072026618))
围栏内里程(米):860.0
围栏外里程(米):61827.0
================>耗时529ms,points=1000
================>开始....
线的长度:0.7621779140342029
线轨迹的实际里程(米):62687.0
面的面积(平方米):808894.4476752883
两个空间对象最近的距离:0.0
两个几何对象的交集:MULTILINESTRING ((116.25688221662699 39.914395421882276, 116.25758584647397 39.91484391938795, 116.25822427104575 39.91560473167323, 116.25860369811619 39.91625944161212, 116.2587542555323 39.917155419248814, 116.25934281853155 39.91740573376146, 116.26017238150571 39.91837305892461, 116.26047989801104 39.918917913968706), (116.26066125935785 39.91908979976905, 116.26139721203687 39.91909890945397, 116.261493614772 39.91969117292497, 116.26209510309556 39.919864426634085, 116.26267162465133 39.920768643283466, 116.26283447817538 39.92087285535724, 116.26320152504029 39.92117941999886, 116.26371154759461 39.92198072026618))
围栏内里程(米):860.0
围栏外里程(米):61827.0
================>耗时97ms,points=1000
================>开始....
线的长度:0.7621779140342029
线轨迹的实际里程(米):62687.0
面的面积(平方米):808894.4476752883
两个空间对象最近的距离:0.0
两个几何对象的交集:MULTILINESTRING ((116.25688221662699 39.914395421882276, 116.25758584647397 39.91484391938795, 116.25822427104575 39.91560473167323, 116.25860369811619 39.91625944161212, 116.2587542555323 39.917155419248814, 116.25934281853155 39.91740573376146, 116.26017238150571 39.91837305892461, 116.26047989801104 39.918917913968706), (116.26066125935785 39.91908979976905, 116.26139721203687 39.91909890945397, 116.261493614772 39.91969117292497, 116.26209510309556 39.919864426634085, 116.26267162465133 39.920768643283466, 116.26283447817538 39.92087285535724, 116.26320152504029 39.92117941999886, 116.26371154759461 39.92198072026618))
围栏内里程(米):860.0
围栏外里程(米):61827.0
================>耗时85ms,points=1000
## **异常情况**
由于模拟数据生成的随机性（不排除用户设置的围栏有交叉情况），会发生如下异常：
LINESTRING (116.25233147795146 39.90671657422486, 116.25329975573385 ..........................................
, 116.75523501649404 40.38680842299548, 116.75542104290876 40.38752711749191, 116.75557794481281 40.3880272337106, 116.7563739582496 40.38839729795829)
POLYGON ((116.25705727212875 39.907554807329966, 116.26239238833828 39.9171876545588, 116.2723211320915 39.92447819980439, 116.27886972069327 39.924660627590114, 116.27984327323371 39.93156232014855, 116.25705727212875 39.907554807329966))
lineStringContainsLineString=true
**polygonContainsPolygon=false**
================>开始....
com.vividsolutions.jts.geom.TopologyException: found non-noded intersection between LINESTRING ( 116.2723211320915 39.92447819980439, 116.27886972069327 39.924660627590114 ) and LINESTRING ( 116.27984327323371 39.93156232014855, 116.25705727212875 39.907554807329966 ) [ (116.27314128058926, 39.924501047151125, NaN) ]
at com.vividsolutions.jts.noding.FastNodingValidator.checkValid(FastNodingValidator.java:130)
at com.vividsolutions.jts.geomgraph.EdgeNodingValidator.checkValid(EdgeNodingValidator.java:94)
at com.vividsolutions.jts.geomgraph.EdgeNodingValidator.checkValid(EdgeNodingValidator.java:59)
at com.vividsolutions.jts.operation.overlay.OverlayOp.computeOverlay(OverlayOp.java:170)
at com.vividsolutions.jts.operation.overlay.OverlayOp.getResultGeometry(OverlayOp.java:127)
at com.vividsolutions.jts.operation.overlay.OverlayOp.overlayOp(OverlayOp.java:66)
at com.vividsolutions.jts.operation.overlay.snap.SnapIfNeededOverlayOp.getResultGeometry(SnapIfNeededOverlayOp.java:96)
at com.vividsolutions.jts.operation.overlay.snap.SnapIfNeededOverlayOp.overlayOp(SnapIfNeededOverlayOp.java:58)
at com.vividsolutions.jts.geom.Geometry.intersection(Geometry.java:1342)
at com.forestar.geometry.utils.GeometryUtil.intersectionGeo(GeometryUtil.java:122)
at test.GeometryTest.run(GeometryTest.java:108)
================>开始....
at test.GeometryTest.runLoop(GeometryTest.java:144)
at test.GeometryTest.main(GeometryTest.java:155)
com.vividsolutions.jts.geom.TopologyException: found non-noded intersection between LINESTRING ( 116.2723211320915 39.92447819980439, 116.27886972069327 39.924660627590114 ) and LINESTRING ( 116.27984327323371 39.93156232014855, 116.25705727212875 39.907554807329966 ) [ (116.27314128058926, 39.924501047151125, NaN) ]
at com.vividsolutions.jts.noding.FastNodingValidator.checkValid(FastNodingValidator.java:130)
at com.vividsolutions.jts.geomgraph.EdgeNodingValidator.checkValid(EdgeNodingValidator.java:94)
at com.vividsolutions.jts.geomgraph.EdgeNodingValidator.checkValid(EdgeNodingValidator.java:59)
at com.vividsolutions.jts.operation.overlay.OverlayOp.computeOverlay(OverlayOp.java:170)
at com.vividsolutions.jts.operation.overlay.OverlayOp.getResultGeometry(OverlayOp.java:127)
at com.vividsolutions.jts.operation.overlay.OverlayOp.overlayOp(OverlayOp.java:66)
at com.vividsolutions.jts.operation.overlay.snap.SnapIfNeededOverlayOp.getResultGeometry(SnapIfNeededOverlayOp.java:96)
at com.vividsolutions.jts.operation.overlay.snap.SnapIfNeededOverlayOp.overlayOp(SnapIfNeededOverlayOp.java:58)
at com.vividsolutions.jts.geom.Geometry.intersection(Geometry.java:1342)
at com.forestar.geometry.utils.GeometryUtil.intersectionGeo(GeometryUtil.java:122)
at test.GeometryTest.run(GeometryTest.java:108)
at test.GeometryTest.runLoop(GeometryTest.java:144)
at test.GeometryTest.main(GeometryTest.java:155)
================>开始....
com.vividsolutions.jts.geom.TopologyException: found non-noded intersection between LINESTRING ( 116.2723211320915 39.92447819980439, 116.27886972069327 39.924660627590114 ) and LINESTRING ( 116.27984327323371 39.93156232014855, 116.25705727212875 39.907554807329966 ) [ (116.27314128058926, 39.924501047151125, NaN) ]
at com.vividsolutions.jts.noding.FastNodingValidator.checkValid(FastNodingValidator.java:130)
at com.vividsolutions.jts.geomgraph.EdgeNodingValidator.checkValid(EdgeNodingValidator.java:94)
at com.vividsolutions.jts.geomgraph.EdgeNodingValidator.checkValid(EdgeNodingValidator.java:59)
at com.vividsolutions.jts.operation.overlay.OverlayOp.computeOverlay(OverlayOp.java:170)
at com.vividsolutions.jts.operation.overlay.OverlayOp.getResultGeometry(OverlayOp.java:127)
at com.vividsolutions.jts.operation.overlay.OverlayOp.overlayOp(OverlayOp.java:66)
at com.vividsolutions.jts.operation.overlay.snap.SnapIfNeededOverlayOp.getResultGeometry(SnapIfNeededOverlayOp.java:96)
at com.vividsolutions.jts.operation.overlay.snap.SnapIfNeededOverlayOp.overlayOp(SnapIfNeededOverlayOp.java:58)
at com.vividsolutions.jts.geom.Geometry.intersection(Geometry.java:1342)
at com.forestar.geometry.utils.GeometryUtil.intersectionGeo(GeometryUtil.java:122)
at test.GeometryTest.run(GeometryTest.java:108)
at test.GeometryTest.runLoop(GeometryTest.java:144)
at test.GeometryTest.main(GeometryTest.java:155)
## **异常分析**
在上面的测试中，异常情况下多边形不能被其本身所包含。
正常情况:
lineStringContainsLineString=true
**polygonContainsPolygon=****true**
异常情况:
lineStringContainsLineString=true
**polygonContainsPolygon=false**
可以在求交集之前做一次自身的包含计算，以避免异常的产生。
异常产生的提示是：发现LINESTRING之间的非节点交叉点
![](https://img-blog.csdnimg.cn/20190417183844753.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
可以看到类似8字形的多边形，其交叉点并没有实际的点存在，这种图像就会产生异常。
![](https://img-blog.csdnimg.cn/20190417183857655.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
## **避免异常**
最简单的方式就是合理的使用Geometry的方法，也就不存在异常问题了。
**计算步骤：**
- 判定多边形（围栏）是否对本身包含
- 如果为假，则直接通过判断轨迹点是否在围栏内部进行分段计算。
注：分段计算是将轨迹区别运算，分别统计围栏内外的轨迹，效率不高，Geometry求交集方式的效率最高。
# Github项目
[https://github.com/SunflowersOfJava/geo-gis-utils](https://github.com/SunflowersOfJava/geo-gis-utils)
