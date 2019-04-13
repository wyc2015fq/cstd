
# C\#调用百度地图API入门&解决BMap未定义问题 - 杨秀璋的专栏 - CSDN博客

2016年05月15日 02:44:38[Eastmount](https://me.csdn.net/Eastmount)阅读数：27827所属专栏：[C\# 系统应用知识](https://blog.csdn.net/column/details/eastmount-xtyy.html)



本文主要是最近帮助好友研究JavaScript的百度地图API，同时显示到C\# Winform界面。同时遇到了BMap未定义的错误（BMap is not defined）及解决方法。以前写过基于Android的百度地图，其实原理都差不多，希望文章对你有所帮助吧！

## 一. C\#显示百度地图
使用C\# Winform显示百度地图主要包括两个步骤：
1.调用百度地图API，通过申请的密钥AK访问JavaScript版本地图；
2.再在C\#中webBrowser浏览器控件中访问JS文件，显示地图到窗口。
百度官方文档：[http://developer.baidu.com/map/jsmobile.htm](http://developer.baidu.com/map/jsmobile.htm)
百度申请密钥：[http://lbsyun.baidu.com/apiconsole/key](http://lbsyun.baidu.com/apiconsole/key)
首先，申请密钥如下图所示：

![](https://img-blog.csdn.net/20160515005315759)

密钥申请成功，如下图AK所示，通过它即可显示JavaScript百度地图。它包括Android端、浏览器端、服务器等应用。
![](https://img-blog.csdn.net/20160515005405951)
百度地图Hello World：[http://developer.baidu.com/map/jsdemo-mobile.htm\#a1_1](http://developer.baidu.com/map/jsdemo-mobile.htm#a1_1)
详细代码如下所示，需要填写你的密钥，本地新建一个a1_1.html即可。

```python
<!DOCTYPE html>
<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
	<meta name="viewport" content="initial-scale=1.0, user-scalable=no" />
	<style type="text/css">
		body, html,#allmap {width: 100%;height: 100%;overflow: hidden;margin:0;}
		#golist {display: none;}
		@media (max-device-width: 780px){#golist{display: block !important;}}
	</style>
	<script type="text/javascript" src="http://api.map.baidu.com/api?type=quick&ak=您的密钥&v=1.0"></script>
	<title>地图官网展示效果</title>
</head>
<body>
	<a id="golist" href="../demolist.htm">返回demo列表页</a>
	<div id="allmap"></div>
</body>
</html>
<script type="text/javascript">
	// 百度地图API功能
	var map = new BMap.Map("allmap");            // 创建Map实例
	var point = new BMap.Point(116.404, 39.915); // 创建点坐标
	map.centerAndZoom(point,15);                 // 初始化地图,设置中心点坐标和地图级别。
	map.addControl(new BMap.ZoomControl());      //添加地图缩放控件
</script>
```
通过Chrome浏览器打开a1_1.html如下图所示：
![](https://img-blog.csdn.net/20160515011642674)
然后打开VS2012新建Winform项目，主要界面是通过WebBrowser。如下图所示：
![](https://img-blog.csdn.net/20160515012840772)
WebBrowser是一个.NET 控件类，在.NET Framework 2.0 版中新增。WebBrowser 类使用户可以在窗体中导航网页。
其中最常见的webBrowser1.Navigate(textBox1.Text.Trim());即可访问URL各类网站。
这里也是通过该控件来显示本地JavaScript网页。核心代码如下：
```python
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
//新添加命名空间  
using System.Net;
using System.IO;
using System.Security.Permissions;  //交互JS
namespace MapWinForms
{
    [PermissionSet(SecurityAction.Demand, Name = "FullTrust")]
    [System.Runtime.InteropServices.ComVisibleAttribute(true)]
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        private void button1_Click(object sender, EventArgs e)
        {
            //本地文件 MapWinForms\bin\Debug
            string url = Application.StartupPath + "\\a1_1.html";
            textBox1.Text = url;
            string file = "file:///E:\\WinFormBaiduMap\\a1_1.html";
            //屏蔽js相关错误
            webBrowser1.ScriptErrorsSuppressed = true; 
            //导航显示本地HTML文件
            webBrowser1.Navigate(url);
        }
    }
}
```
运行结果如下图所示：
![](https://img-blog.csdn.net/20160515015146998)
下一篇文章会详细介绍JavaScript一些百度地图常用的函数即应用。
下面是推荐一些相关的优秀博客：
[C\#调用百度地图API经验分享（四） 期待秋天的叶](http://blog.csdn.net/kkkkkxiaofei/article/details/8663377)
[基于百度地图API的WinForm地图 秦元培](http://blog.csdn.net/qinyuanpei/article/details/9270831)
[C\#百度地图控件BMap.Net 秦元培](http://blog.csdn.net/qinyuanpei/article/details/10728479)
[C\# Form调用百度地图api攻略及常见问题](http://www.cnblogs.com/dyllove98/archive/2013/07/30/3225824.html)
[百度地图js小结 java_xiaobin](http://blog.csdn.net/java_xiaobin/article/details/25718361)
[http://www.cnblogs.com/ljmin/archive/2012/07/27/2612413.html](http://www.cnblogs.com/ljmin/archive/2012/07/27/2612413.html)


## 二. 解决BMap未定义问题
在使用C\# WebBrowser控件显示百度地图时，通常会报错BMap未定义。
报错：BMap.Map("container") BMap未定义 BMap is not defined

![](https://img-blog.csdn.net/20160515020208213)
**未解决方法：**
网上看到很多资料，但是都没有解决。包括：
1.需要将密钥验证的js一定要放在head标签内；
2.C\#忽略JS错误：webBrowser1.ScriptErrorsSuppressed = true;
3.在html文件head中添加如下代码：
```python
<script type="text/javascript" src="http://api.map.baidu.com/library/DistanceTool/1.2/src/DistanceTool_min.js"></script>
<script type="text/javascript" src="http://api.map.baidu.com/library/DrawingManager/1.4/src/DrawingManager_min.js"></script>
<script type="text/javascript" src="http://api.map.baidu.com/library/SearchInfoWindow/1.4/src/SearchInfoWindow_min.js"></script>
```
官方解决方法（没有作用）：
[http://bbs.lbsyun.baidu.com/forum.php?mod=viewthread&tid=4530](http://bbs.lbsyun.baidu.com/forum.php?mod=viewthread&tid=4530)
[http://bbs.lbsyun.baidu.com/forum.php?mod=viewthread&tid=3062](http://bbs.lbsyun.baidu.com/forum.php?mod=viewthread&tid=3062)
[http://bbs.lbsyun.baidu.com/forum.php?mod=viewthread&tid=88488](http://bbs.lbsyun.baidu.com/forum.php?mod=viewthread&tid=88488)
[http://bbs.lbsyun.baidu.com/forum.php?mod=viewthread&page=1&tid=88543](http://bbs.lbsyun.baidu.com/forum.php?mod=viewthread&page=1&tid=88543)
[http://blog.csdn.net/zhujianli1314/article/details/45696259](http://blog.csdn.net/zhujianli1314/article/details/45696259)
[https://segmentfault.com/q/1010000004531211](https://segmentfault.com/q/1010000004531211)
[HTTPS调用百度地图API提示: BMap未定义](http://blog.sina.com.cn/s/blog_7191ed320102vhgz.html)
后来因为我使用WebBrowser控件显示http://www.baidu.com都不能访问，因为它都是微软开发的软件，我怀疑该控件是调用IE浏览器显示的。
修复如下：工具=》Internet选项=》高级=》重置=》重启电脑。同时可以在安全中，设置启用加载脚本，混合等方法。
![](https://img-blog.csdn.net/20160515021511184)
修复前确实我自己的IE浏览器不能使用了，对IE浏览器进行了修复后，C\#程序确实能通过WebBrowser访问网页了。自己以为是这个原因，后来在加载JS时仍然报错：BMap未定义。
**最终解决方法：**
该方法只保证能正确调用，但是具体设置密钥错误的原因我也不清楚。
将head中调用密钥的代码修改为不需要密钥的：
**<script type="text/javascript"**
**src="http://api.map.baidu.com/api?key=&amp;v=1.1&services=true">**
**</script>**
替代的代码：
**<script type="text/javascript"**
**src="http://api.map.baidu.com/api?type=quick&ak=您的密钥&v=1.0">**
**</script>**
或者使用下面的代码也是能在C\#中正常显示的：（AK为我的应用）
**<script type="text/javascript"**
**src="http://api.map.baidu.com/api?v=1.0&ak=ui7X2CSu3ITjojARL4tILi5W1i9gkDVS">**
**</script>**

## 三. 备份一段JS百度地图代码
下面是一段代码备份：
```python
<!DOCTYPE html>
<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
	<meta name="viewport" content="initial-scale=1.0, user-scalable=no" />
        <script type="text/javascript" src="http://api.map.baidu.com/api?v=2.0&ak=ui7X2CSu3ITjojARL4tILi5W1i9gkDVS"></script>
	<style type="text/css">
		body, html,#allmap {width: 100%;height: 100%;overflow: hidden;margin:0;font-family:"微软雅黑";}
	</style>
	<title>百度地图API测试</title>
</head>
<body>
	<div id="allmap"></div>
</body>
</html>
<script type="text/javascript">    
	var map = new BMap.Map("allmap");                     // 创建Map实例
	var pointA = new BMap.Point(113.92983, 22.509397);    // 创建点坐标   地址 蛇口沃尔玛	
	var pointB = new BMap.Point(113.942129, 22.522642);   //地址 海岸城
	map.centerAndZoom(pointA, 15);
	map.enableScrollWheelZoom();                          //启用滚轮放大缩小
	map.addControl(new BMap.NavigationControl());
	map.addControl(new BMap.ScaleControl());
	map.addControl(new BMap.OverviewMapControl());
	//map.setCurrentCity("北京"); // 设置当前城市信息
	/*
         *  给地图添加监控 获取当前点击经纬度
	    map.addEventListener("click", function(e){
	        alert(e.point.lng + ", " + e.point.lat); 
	    });
	 */
	addMarker(pointA, "蛇口沃尔玛", "地址：工业大道和东滨路交汇处<br/>电话：(0755)26816711");           // 创建标注1  
	addMarker(pointB, "海岸城购物中心", "地址：文心五路33号海岸城东座212号<br/>电话：(0755)86125888");  // 创建标注2  
	//开始测距		
	var polyline = new BMap.Polyline([pointA, pointB], { strokeColor: "orange", strokeWeight: 6, strokeOpacity: 0.5 });  //定义折线
	map.addOverlay(polyline);     //添加折线到地图上
	polyline.addEventListener("click", function () {    //监听标注事件		
	    alert('从蛇口沃尔玛到海岸城购物中心的距离是：' + (map.getDistance(pointA, pointB)).toFixed(2) + ' 米。');  //获取两点距离,保留小数点后两位
	});
	//获取驾车路线
	//var driving = new BMap.DrivingRoute(map, { renderOptions: { map: map, autoViewport: true} });
	//driving.search(pointA, pointB);
	
	// 编写自定义函数,创建标注
	function addMarker(point, a, b) {
	    var marker = new BMap.Marker(point);
	    map.addOverlay(marker);
	    //map.removeOverlay(marker);                      // 将标注从地图中移除
	    //marker.setAnimation(BMAP_ANIMATION_BOUNCE);     // 跳动的动画
	    marker.addEventListener("click", function () {    // 监听标注事件  
	        var opts = {         // 创建信息窗口
	            width: 250,      // 信息窗口宽度    
	            height: 100,     // 信息窗口高度    
	            title: a         // 信息窗口标题   
	        }
	        var infoWindow = new BMap.InfoWindow(b, opts);  // 创建信息窗口对象    
	        map.openInfoWindow(infoWindow, point);          // 打开信息窗口			
	        //alert("marker的位置是" + p.lng + "," + p.lat);   
	    });
	}
</script>
```
运行结果如下图所示：
![](https://img-blog.csdn.net/20160515023817769)
参考：
[http://blog.csdn.net/smartsmile2012/article/details/47754163](http://blog.csdn.net/smartsmile2012/article/details/47754163)
[http://www.sufeinet.com/thread-7897-1-1.html](http://www.sufeinet.com/thread-7897-1-1.html)
PS：最后希望文章对你有所帮助！这篇文章主要是一些在线笔记和入门介绍，非常基础的内容，详细知识参考百度地图官方文档。
(By:Eastmount 2016-05-15 深夜2点半[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))



