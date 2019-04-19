# 基于LBS的地理位置附近的搜索以及由近及远的排序(MYSQL) - 零度的博客专栏 - CSDN博客
2016年11月07日 16:53:45[零度anngle](https://me.csdn.net/zmx729618)阅读数：3868
                
目前基于LBS地理位置的搜索已经应用非常广了，的确是个很方便的东西。
我们做程序的就是要考虑如何通过这些功能，来做出更符合用户的内容来。
## 1，如何获取位置
例如微信，可以通过发送地理位置来获取到当前用户的经纬度。[查看](http://mp.weixin.qq.com/wiki/index.php?title=%E6%B6%88%E6%81%AF%E6%8E%A5%E5%8F%A3%E6%8C%87%E5%8D%97#.E5.9C.B0.E7.90.86.E4.BD.8D.E7.BD.AE.E6.B6.88.E6.81.AF)
在网页端，可以使用html5获取地理定位。[查看](http://www.w3school.com.cn/html5/html_5_geolocation.asp)
## 2，数据表设计
数据库中要预存自己的位置数据，如何获取数据请查看相关地图api。
字段：十进制数的纬度 latitude FLOAT(10,6) 十进制数的经度 longitude FLOAT(10,6)
假设我们数据库中已经存储大量酒店的位置信息
## 3，根据用户位置查询附近
![](http://images.cnitblog.com/blog2015/106546/201503/211851404374640.jpg)
如上图，假设当前用户所在的位置为坐标O，那么我们要查询附近的酒店，理想的范围应该是以O为原点的圆内，但是我们先不这样做，后面会讲到。
我们先以O为原点，在坐标上以0.3的差值标识了4个位置，其实这样就是一个正方形范围，大致应该符合我们的要求。
那么我们就要从数据库中查询范围在这个正方形之内的所有酒店了。
#### 一，获取到用户的经纬度坐标：
```
`$latitude;$longitude`
```
#### 二，定义一个差值，设置经度和纬度的范围：
```
```
$i
 = 0.3; //差值可自定义，值越大，范围就越大
```
```
$min_latitude
 = $latitude - $i; //纬度最小值
```
```
$max_latitude
 = $latitude + $i; //纬度最大值
```
```
$min_longitude
 = $longitude - $i; //经度最小值
```
```
$max_longitude
 = $longitude + $i; //经度最大值
```
```
#### 三，数据库中查询在这个范围之内的酒店：
```
`SELECT``* ``FROM``table``WHERE``(latitude ``BETWEEN``$min_latitude ``AND``$max_latitude) ``AND``(longitude ``BETWEEN``$min_longitude ``AND``$max_longitude);`
```
这样我们的附近搜索基本完成了
#### 四，以地理位置由远及近的排序：
![](http://images.cnitblog.com/blog2015/106546/201503/211853290008807.jpg)
如果我们要筛选出最近的10个的话，用上面的语句来查询，可能会把稍远的先查出来，后面的就没有机会了，那么我们得做个排序了。
如上图，如果我们要获取E位置和F位置分别距离O点的长度，那么我们就需要计算OE和OF的长度分别为多少，这里我们要用到直角三角形的数学公式：C^2 = A^2 + B^2，知道A和B，那么C的值也就得到了。
注意：请首先在表中建立一个字段d，以作后面缓存距离使用，否则会报错
我们可以根据经纬度的差分别来获取到A和B的值，SQL语句是这样的：
```
`SELECT`
```
*,SQRT(POWER($latitude
 - latitude, 2) + POWER($longitude  - longitude, 2))
```
`AS``d ``FROM``table``WHERE``(latitude ``BETWEEN``$min_latitude ``AND``$max_latitude) ``AND``(longitude ``BETWEEN``$min_longitude ``AND``$max_longitude) ``AND`
```
d
 < $i
```
`ORDER``BY``d ``ASC`
```
LIMIT
 10;
```
```
SQRT(X)：求X的平方根，POWER(X, Y)：求X的Y次方
通过这样一步，我们的范围已经锁定在圆形之内了，并且按照由近及远的方式进行排序（在不考虑效率的情况下）。
