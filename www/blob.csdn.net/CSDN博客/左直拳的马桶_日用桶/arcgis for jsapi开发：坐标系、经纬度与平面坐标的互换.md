# arcgis for jsapi开发：坐标系、经纬度与平面坐标的互换 - 左直拳的马桶_日用桶 - CSDN博客
2019年03月04日 18:09:19[左直拳](https://me.csdn.net/leftfist)阅读数：170
练武不练功，到老一场空。学习GIS开发，坐标系是绕不过去的，此乃基本概念，一定要搞清楚。某从事WEB GIS开发差不多2年，一直对这个坐标系感到模糊。这两天因为求解一个问题，方始有点认知。
GIS世界里，坐标系有很多，而且描述得十分抽象，让我们这些没有天赋，缺乏空间想象力的傻叉晕头转向。不过，对于arcgis for jsapi开发人员来说，经常接触的大概只有2种坐标系：
**一、4326地理坐标系**
通常，我们说地图上的某个点，有经纬度，往往指的就是这种坐标系。
这种坐标系，属于地理坐标系。因为地球是个球体，所以，这是一种球面坐标系。
全称：GCS_WGS_1984，ID为4326，一般GPS、遥感影像、外业测绘数据等就采用它。当然，还有其他地理坐标系，如beijing54，xian80等等，4326只是其中之一。
```java
var sr4326 = new SpatialReference({wkid: 4326 });
```
**二、102113投影坐标系**
102113，全称：GCS_WGS_1984_web_mercator_auxiliary_sphere，又称为平面直角坐标系，笛卡尔平面坐标系，Google在线地图所采用。
如上所述，4326地理坐标系是球面坐标系，而我们用的2维地图，是平面的，所以一般用的是平面坐标系。球面坐标系如何换算到平面坐标系，采用投影法。投影法有很多，如米勒投影、墨卡托投影、高斯投影，等等。arcgis中，似乎一般就是墨卡托投影。墨卡托这个名字挺好听，有逼格，我喜欢。102113就是墨卡托投影法搞出来的，故而也可以将它称为墨卡托投影坐标系？
所谓的墨卡托投影是这样的：假设有一个圆锥体或圆柱体，刚好将地球包起来（就是切于地球的赤道），然后地球球心会发光，将地球的经纬线投影到圆锥体或圆柱体上面，这样就得到了地球的平面坐标。圆柱可以看作是特殊的圆锥，即顶点在无限远处。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190304165256657.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sZWZ0ZmlzdC5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190304165011883.png)
使用这种方式得到的投影地图在两极会拉长，但经纬度是平行的直线。如图所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190304165019466.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sZWZ0ZmlzdC5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
```
var sr102113 = new SpatialReference({wkid: 102113 });
```
因为我们加载的底图一般都用它，也可以直接引用地图对象的：
```
var sr102113 = map.spatialReference;
```
如果不确定地图对象的坐标系，可以直接看看地图的地图服务：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019030417030739.png)
百度地图用的是102100（GCS_WGS_1984_web_mercator），应该也是一种墨卡托投影坐标系。
**三、坐标系转换**
为啥要转换？
当然要转换了。
一般来说，我们拿到的空间数据，都是经纬度，属于所谓的球面坐标系，而平面地图用的是平面坐标系，不转换是不能使用的。但我平时开发过程中，对这个无感，那是因为api自动做了转换。比如说，我声明一个点对象：
```
var pt = new Point(longitude, latitude, new SpatialReference({wkid: 4326 });//最后一个参数，注明要使用何种坐标系来创建本对象
```
那么，经纬度如何转化为平面坐标系中的点坐标呢？转化出来的点，其单位是什么呢？墨卡托投影，出来的点的坐标，单位是米。所以经纬度换算成平面坐标后，数值都很大：
```
XMin: 7750103.171888566
YMin: 1252344.2714235506
XMax: 1.693722247553803E7
YMax: 7973910.790707026
Spatial Reference: 102113  (3785)
```
上代码：
```
var _f = 6378137, _p = 0.017453292519943;
function lngLatToXY(pt){//经纬度转化为平面坐标
	var lng = pt[0];//经度
	var lat = pt[1];//纬度
	
	if (lat > 89.999999){
		lat = 89.999999;
	}
	else if (lat < -89.999999){
		lat = -89.999999;
	}
	var c = lat * _p;
	x = lng * _p * _f;
	y = _f / 2 * Math.log((1 + Math.sin(c)) / (1 - Math.sin(c)));
	
	return [x,y];
}
```
参考资料中有经纬度和平面坐标的互换代码，好像与我给的代码有点不同。我这个代码是同事给的，他是我司GIS的权威和专家。我没仔细比较，先记录下来。
**四、实际应用**
有辆车，已知当前的坐标、车速和方向，求一段时间后，这辆车预计会到达哪里？
1、这里就要用到坐标转换了。因为经纬度的单位是度分秒，可不是米，假设15分钟后，车子走了10公里，你说它会到哪里？在赤道附近，经纬度一度都约为111公里；但在南北极，经度之间的距离是0！所以要将经纬度先转成平面坐标，再参加运算。
2、本问题是已知圆心（车辆当前位置）、半径（车辆所走的距离）、角度（方向），求圆弧上的点坐标。arcgis地图中，以正北方向为0度，顺时针起算。也就是说，y轴的正方向为0度。有：
圆心坐标(a,b)，半径r的圆的参数方程（根据三角比的定义可以得出）为：(以y轴正方向为0度，设 t 为圆上某一点的度数)
x = a + r＊sin t
y = b + r＊cos t
所以圆上任一一点的坐标为 P(a + r＊sin t, b + r＊cos t)，（其中 t 为圆上某一点的度数）
```
function drawSpeed(car){
	var t = car;
	var sp = 转成平面坐标([t.longitude,t.latitude]);//起点
	var fp = 获得终点的平面坐标(sp,t.speed,t.direction);//终点
	
	var sr102113 = map.spatialReference;
	
	var line ={geometry:{
			"paths":[[sp,fp]],
			"spatialReference":sr102113},
		"symbol":{"color":_cbd,
			"width":1,
			"type":"esriSLS",
			"style":"esriSLSSolid"}
	};
	return new Graphic(line);
}
```
幸甚至哉。
参考资料：
[利用米勒投影法实现经纬度和平面坐标的相互转换](https://blog.csdn.net/qq_32693445/article/details/79597213)
[Web墨卡托坐标与WGS84坐标互转](https://www.cnblogs.com/liongis/archive/2012/06/13/2548061.html)
