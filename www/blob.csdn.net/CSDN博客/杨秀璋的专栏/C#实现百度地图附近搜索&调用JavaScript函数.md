
# C\#实现百度地图附近搜索&调用JavaScript函数 - 杨秀璋的专栏 - CSDN博客

2016年05月19日 01:56:10[Eastmount](https://me.csdn.net/Eastmount)阅读数：7300所属专栏：[C\# 系统应用知识](https://blog.csdn.net/column/details/eastmount-xtyy.html)



前一篇文章“[C\#调用百度地图API入门&解决BMap未定义问题](http://blog.csdn.net/eastmount/article/details/51380996)”讲述了如何通过C\#调用百度API显示地图，并且如何解决BMap未定义的问题。这篇文章主要更加详细的介绍百度地图的一些功能，包括附近搜索、城市搜索、路线规划、添加覆盖物等等。
希望文章对你有所帮助！如果文章中有不足之处，还请海涵~
百度官方文档：[http://developer.baidu.com/map/jsmobile.htm](http://developer.baidu.com/map/jsmobile.htm)
官方DEMO例：[http://developer.baidu.com/map/jsdemo-mobile.htm](http://developer.baidu.com/map/jsdemo-mobile.htm#a1_1)
源码下载地址：[http://download.csdn.net/detail/eastmount/9524565](http://download.csdn.net/detail/eastmount/9524565)
目录：
一. C\#显示百度地图
二. C\#调用JavaScript中函数
三. C\#显示百度地图附近搜索路线推荐
四. JavaScript百度地图代码详解

## 一. C\#显示百度地图
使用V2012创建WinForm工程，界面如下图所示：
![](https://img-blog.csdn.net/20160519001735546)
双击界面空白处，生出Form1_Load()函数，载入界面时显示百度图片。代码如下：
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
        //载入加载地图
        private void Form1_Load(object sender, EventArgs e)
        {
            //本地文件 MapWinForms\bin\Debug
            string url = Application.StartupPath + "\\BaiMapJS.html";
            string file = "file:///E:\\WinFormBaiduMap\\test.html";
            webBrowser1.ScriptErrorsSuppressed = true;   //屏蔽js相关错误
            webBrowser1.Navigate(url);                   //导航显示本地HTML文件
        }
    }
}
```
本地工程路径"MapWinForms\bin\Debug"下存在JavaScript文件。其中申请百度地图AK密钥的方法参考前一篇文章，其中BaiMapJS.html代码如下：
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
    <script type="text/javascript" src="http://api.map.baidu.com/api?v=2.0&ak=ui7X2CSu3ITjojARL4tILi5W1i9gkDVS"></script>
    <title>地图官网展示效果</title>  
</head>  
<body>    
    <div id="allmap"></div>  
</body>  
</html>  
<script type="text/javascript">  
	// 百度地图API功能  
	var map = new BMap.Map("allmap");            // 创建Map实例  
	var point = new BMap.Point(116.404, 39.915); // 创建点坐标  
	map.centerAndZoom(point, 15);                // 初始化地图,设置中心点坐标和地图级别  
	map.enableScrollWheelZoom();                 // 启用滚轮放大缩小
	map.addControl(new BMap.ZoomControl());      // 添加地图缩放控件 
	map.addControl(new BMap.ScaleControl());     // 添加比例尺控件
</script>
```
运行结果如下图所示，JavaScript代码中建议使用你自己的AK。
同时解决BMap未定义参考[前文](http://blog.csdn.net/eastmount/article/details/51380996)。
![](https://img-blog.csdn.net/20160519002145302)
PS：写到此处，C\#通过WebBrowser浏览器控件加载本地JS的方法已经实现。请不要推荐我的界面配色，从大一老师就说我的配色像HW。整篇文章的思路非常简单，感觉存在取巧的地方。当然Android也同样可以通过WebView控件实现。Android参考：
[http://bbs.csdn.net/topics/390776139](http://bbs.csdn.net/topics/390776139)
[http://www.cnblogs.com/tinyphp/p/3858997.html](http://www.cnblogs.com/tinyphp/p/3858997.html)
[http://blog.csdn.net/beyond0525/article/details/9374301](http://blog.csdn.net/beyond0525/article/details/9374301)


## 二. C\#调用JavaScript中函数
这部分主要讲述C\#调用HTML中JavaScript函数的过程，其中包括传递参数和无参数的情况。如下图所示，下拉菜单选择贵阳，然后城市定位结果如下。
![](https://img-blog.csdn.net/20160519002021201)
其中"贵阳"就是传递的参数，C\#中对应的代码是：

```python
//城市搜索
private void button1_Click(object sender, EventArgs e)
{
    //C#调用JS函数
    string city;
    if (comboBox1.SelectedIndex == -1)
    {
        city = "北京";
    }
    else 
    {
        city = comboBox1.SelectedItem.ToString();
    }
    webBrowser1.Document.InvokeScript("LoadMap", new object[] { city });
}
```
city为下拉菜单选择的结果，如果未选择则定位"北京"。调用JS的函数核心代码：
**webBrowser1.Document.InvokeScript("LoadMap", new object[] { city });**
其中InvokeScript中两个参数，"LoadMap"为html中的JavaScript函数，第二个为传递的参数city。如果无参数，则写"new object[] { }"即可。
BaiMapJS.html中函数如下：
```python
// 定位城市
function LoadMap(city) {                       
	map.centerAndZoom(city, 12);                 // 初始化地图,设置中心点坐标和地图级别 15更详细
	map.enableScrollWheelZoom();                 // 启用滚轮放大缩小
	map.addControl(new BMap.ZoomControl());      // 添加地图缩放控件 
	map.addControl(new BMap.ScaleControl());     // 添加比例尺控件
}
```
参考资料：
[http://bbs.csdn.net/topics/390935011](http://bbs.csdn.net/topics/390935011)
[http://www.sufeinet.com/thread-7897-1-1.html](http://www.sufeinet.com/thread-7897-1-1.html)

## 三. C\#实现百度地图附近搜索路线推荐
完整C\#代码如下：

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
        //载入加载地图
        private void Form1_Load(object sender, EventArgs e)
        {
            //本地文件 MapWinForms\bin\Debug
            string url = Application.StartupPath + "\\BaiMapJS.html";
            string file = <a target=_blank href="file:///E:\\WinFormBaiduMap\\test.html">file:///E:\\WinFormBaiduMap\\test.html</a>;
            webBrowser1.ScriptErrorsSuppressed = true;   //屏蔽js相关错误
            webBrowser1.Navigate(url);                   //导航显示本地HTML文件
        }
        //城市搜索
        private void button1_Click(object sender, EventArgs e)
        {
            /**
                * 参考资料：
                * http://bbs.csdn.net/topics/390935011
                * http://www.sufeinet.com/thread-7897-1-1.html
                */
            //C#调用JS函数
            string city;
            if (comboBox1.SelectedIndex == -1)
            {
                city = "北京";
            }
            else 
            {
                city = comboBox1.SelectedItem.ToString();
            }
            webBrowser1.Document.InvokeScript("LoadMap", new object[] { city });
        }
        //车间搜索
        private void button2_Click(object sender, EventArgs e)
        {
            string city;
            if (comboBox1.SelectedIndex == -1)
            {
                city = "北京";
            }
            else
            {
                city = comboBox1.SelectedItem.ToString();
            }
            webBrowser1.Document.InvokeScript("SearchCarMap", new object[] { city });
        }
        //附近热点搜索
        private void button3_Click(object sender, EventArgs e)
        {
            string city;
            if (comboBox1.SelectedIndex == -1)
            {
                city = "北京";
            }
            else
            {
                city = comboBox1.SelectedItem.ToString();
            }
            string key;
            if (comboBox2.SelectedIndex == -1)
            {
                key = "学校";
            }
            else
            {
                key = comboBox2.SelectedItem.ToString();
            }
            webBrowser1.Document.InvokeScript("SearchNearbyMap", new object[] { city, key });
        }
        //当前位置定位
        private void button4_Click(object sender, EventArgs e)
        {
            webBrowser1.Document.InvokeScript("LocateMyPlcae", new object[] { });
        }
        //附近餐馆
        private void button5_Click(object sender, EventArgs e)
        {
            webBrowser1.Document.InvokeScript("GetResMyPlace", new object[] { });
        }
        //导航
        private void button6_Click(object sender, EventArgs e)
        {
            webBrowser1.Document.InvokeScript("RouteMyPlace", new object[] { });
        }
    }
}
```
其中JavaScript代码如下，即BaiMapJS.html文件：
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
    <script type="text/javascript" src="http://api.map.baidu.com/api?v=2.0&ak=ui7X2CSu3ITjojARL4tILi5W1i9gkDVS"></script>
    <title>地图官网展示效果</title>  
</head>  
<body>    
    <div id="allmap"></div>  
</body>  
</html>  
<script type="text/javascript">  
	// 百度地图API功能  
	var map = new BMap.Map("allmap");            // 创建Map实例  
	var point = new BMap.Point(116.404, 39.915); // 创建点坐标  
	map.centerAndZoom(point, 15);                // 初始化地图,设置中心点坐标和地图级别  
	map.enableScrollWheelZoom();                 // 启用滚轮放大缩小
	map.addControl(new BMap.ZoomControl());      // 添加地图缩放控件 
	map.addControl(new BMap.ScaleControl());     // 添加比例尺控件
	
	// 定位城市
	function LoadMap(city) {                       
		map.centerAndZoom(city, 12); // 初始化地图,设置中心点坐标和地图级别 15更详细
		map.enableScrollWheelZoom();                 // 启用滚轮放大缩小
		map.addControl(new BMap.ZoomControl());      // 添加地图缩放控件 
		map.addControl(new BMap.ScaleControl());     // 添加比例尺控件
	}	
	// 根据中心点关键字周边搜索
	function SearchNearbyMap(city, key) {
		map.centerAndZoom(city, 12);                 
		var point = new BMap.Point(104.083, 30.686); // 默认中心点
		var marker = new BMap.Marker(point);         // 计划调用自定义
		
		//清除覆盖物再加载，否则多次点击不断添加
		//map.removeOverlay(Overlay);                // 清除指定覆盖物
        map.clearOverlays();                         // 清除全部
		var local = new BMap.LocalSearch(map, {
			renderOptions:{map: map, autoViewport:true}
		});
		local.searchNearby(key, city);
		
		map.addControl(new BMap.NavigationControl()); //左上角控件
		map.enableScrollWheelZoom();                 // 启用滚轮放大缩小
		map.addControl(new BMap.ZoomControl());      // 添加地图缩放控件 
		map.addControl(new BMap.ScaleControl());     // 添加比例尺控件
	}
	
	// 根据中心点关键字周边搜索 停车场
	function SearchCarMap(city) {
		map.centerAndZoom(city, 12);                 
        map.clearOverlays();                         // 清除全部覆盖物再加载，否则多次点击不断添加
		var local = new BMap.LocalSearch(map, {
			renderOptions:{map: map, autoViewport:true}
		});
		local.searchNearby("停车场", city);
		
		map.addControl(new BMap.NavigationControl()); //左上角控件
		map.enableScrollWheelZoom();                 // 启用滚轮放大缩小
		map.addControl(new BMap.ZoomControl());      // 添加地图缩放控件 
		map.addControl(new BMap.ScaleControl());     // 添加比例尺控件
	}
    // 当前位置定位 北理 人民大学  
    function LocateMyPlcae() {
    	map.clearOverlays(); 
    	var pointA_A = 116.32377;
    	var pointA_B = 39.967031;
    	var pointA = new BMap.Point(pointA_A, pointA_B);  
    	var pointB_A = 116.34964;
    	var pointB_B = 39.957990;
    	var pointB = new BMap.Point(pointB_A, pointB_B); 
    	var pointC = new BMap.Point((pointA_A+pointB_A)/2, 
    						(pointA_B+pointB_B)/2); 
    	map.centerAndZoom(pointC, 15);
    	
		var markerA = new BMap.Marker(pointA);
		var markerB = new BMap.Marker(pointB); 
		// 将标注添加到地图中
		//map.addOverlay(markerA);                      
		//map.addOverlay(markerB);  
		// 创建标注1 
		addMarker(pointA, "北京理工大学", "地址：北京理工大学11号学生公寓");          
        // 创建标注2 
    	addMarker(pointB, "北京交通大学", "地址：北京交通大学第一教学楼");          
  
    	// 开始测距 定义折线
    	var polyline = new BMap.Polyline([pointA, pointB], { strokeColor: "orange", strokeWeight: 6, strokeOpacity: 0.5 });   
    	map.addOverlay(polyline);                       // 添加折线到地图上 
    	// 监听标注事件 获取两点距离,保留小数点后两位  
    	polyline.addEventListener("click", function () {    //          
        	alert('两地距离是：' + 
        		(map.getDistance(pointA, pointB)).toFixed(2) + ' 米。');   
    	});
		map.enableScrollWheelZoom();                 // 启用滚轮放大缩小
		map.addControl(new BMap.ZoomControl());      // 添加地图缩放控件 
		map.addControl(new BMap.ScaleControl());     // 添加比例尺控件
		map.addControl(new BMap.NavigationControl());
		map.addControl(new BMap.OverviewMapControl());
    }
    function GetResMyPlace() {
    	map.clearOverlays(); 
    	var pointA_A = 116.32377;
    	var pointA_B = 39.967031;
    	var pointA = new BMap.Point(pointA_A, pointA_B);  
    	var pointB_A = 116.34964;
    	var pointB_B = 39.957990;
    	var pointB = new BMap.Point(pointB_A, pointB_B); 
    	var pointC = new BMap.Point((pointA_A+pointB_A)/2, 
    						(pointA_B+pointB_B)/2); 
    	map.centerAndZoom(pointC, 15);
    	map.enableScrollWheelZoom();                 // 启用滚轮放大缩小
    	
    	var markerA = new BMap.Marker(pointA);
		var markerB = new BMap.Marker(pointB);  
		// 创建标注1 
		addMarker(pointA, "北京理工大学", "地址：北京理工大学11号学生公寓");          
        // 创建标注2 
    	addMarker(pointB, "北京交通大学", "地址：北京交通大学第一教学楼");          
  
    	// 开始测距 定义折线        
    	var polyline = new BMap.Polyline([pointA, pointB], { strokeColor: "orange", strokeWeight: 6, strokeOpacity: 0.5 });    
    	map.addOverlay(polyline);                       // 添加折线到地图上 
    	// 监听标注事件 获取两点距离,保留小数点后两位  
    	polyline.addEventListener("click", function () {    //          
        	alert('两地距离是：' + 
        		(map.getDistance(pointA, pointB)).toFixed(2) + ' 米。');   
    	});  
    	/*
    	var local = new BMap.LocalSearch(map, {
	  		renderOptions:{map: map}
		});
    	// 自己规定范围
		var bs = new BMap.Bounds(pointA, pointB);
		local.searchInBounds("餐馆", bs);
		*/
		var local = new BMap.LocalSearch(map, {
			renderOptions:{map: map}
		});
		local.search("餐馆");
		
		map.addControl(new BMap.ZoomControl());      // 添加地图缩放控件 
		map.addControl(new BMap.ScaleControl());     // 添加比例尺控件
		map.addControl(new BMap.NavigationControl());
		map.addControl(new BMap.OverviewMapControl());
    }
    // 导航
    function RouteMyPlace() {
    	map.clearOverlays(); 
    	var pointA_A = 116.32377;
    	var pointA_B = 39.967031;
    	var pointA = new BMap.Point(pointA_A, pointA_B);  
    	var pointB_A = 116.34964;
    	var pointB_B = 39.957990;
    	var pointB = new BMap.Point(pointB_A, pointB_B); 
    	var pointC = new BMap.Point((pointA_A+pointB_A)/2, 
    						(pointA_B+pointB_B)/2); 
    	map.centerAndZoom(pointC, 15);
    	map.enableScrollWheelZoom();                 // 启用滚轮放大缩小
    	
    	var markerA = new BMap.Marker(pointA);
		var markerB = new BMap.Marker(pointB);  
    	map.addOverlay(markerA);
    	map.addOverlay(markerB);
		markerA.addEventListener("click", function(){
			/*start|end：（必选）
			{name:string,latlng:Lnglat}
			opts:
			mode：导航模式，固定为
			BMAP_MODE_TRANSIT、BMAP_MODE_DRIVING、
			BMAP_MODE_WALKING、BMAP_MODE_NAVIGATION
			分别表示公交、驾车、步行和导航，（必选）
			region：城市名或县名  给定region认为起点和终点都在同一城市，除非单独给定起点或终点城市
			origin_region/destination_region：同上
			*/
			var start = {
				 name:"北京理工大学"
			}
			var end = {
				name:"北京交通大学"
			}
			var opts = {
				mode:BMAP_MODE_DRIVING,
				region:"北京"
			}
			var ss = new BMap.RouteSearch();
			ss.routeCall(start,end,opts);
		});
		map.addControl(new BMap.ZoomControl());      // 添加地图缩放控件 
		map.addControl(new BMap.ScaleControl());     // 添加比例尺控件
		map.addControl(new BMap.NavigationControl());
		map.addControl(new BMap.OverviewMapControl());
    }
    // 自定义创建标注  
    function addMarker(point, a, b) {  
        var marker = new BMap.Marker(point);  
        map.addOverlay(marker);  
        marker.addEventListener("click", function () {    // 监听标注事件    
            var opts = {         // 创建信息窗口  
                width: 250,      // 信息窗口宽度      
                height: 100,     // 信息窗口高度      
                title: a         // 信息窗口标题     
            }  
            var infoWindow = new BMap.InfoWindow(b, opts);  // 创建信息窗口对象      
            map.openInfoWindow(infoWindow, point);          // 打开信息窗口                 
        });  
    }
</script>
```

## 四. JavaScript百度地图代码详解
这部分主要详细讲解代码过程及运行截图。
**1.加载百度地图**
var map = new BMap.Map("allmap");// 创建Map实例
var point = new BMap.Point(116.404, 39.915);// 创建点坐标
map.centerAndZoom(point, 15);// 初始化地图,设置中心点坐标和地图级别
map.enableScrollWheelZoom();// 启用滚轮放大缩小
map.addControl(new BMap.ZoomControl());// 添加地图缩放控件
map.addControl(new BMap.ScaleControl());// 添加比例尺控件
运行结果如下图所示：
![](https://img-blog.csdn.net/20160519002229944)
它可以通过鼠标滚轮缩放地图，同时点击如"北京理工大学"(我母校)可以查看详细信息。
**2.定位城市**
主要通过函数map.centerAndZoom("南京", 12)实现，其中12表示地图显示大小级别。
function LoadMap(city) {
map.centerAndZoom(city, 12);
map.enableScrollWheelZoom();
map.addControl(new BMap.ZoomControl());//
 添加地图缩放控件
map.addControl(new BMap.ScaleControl());//
 添加比例尺控件
}
其中运行截图如上面第二部分的"贵阳"，这里不在介绍。
**3.根据中心点关键字周边搜索**
主要参考：http://developer.baidu.com/map/jsdemo-mobile.htm\#i1_2
function SearchNearbyMap(city,
 key) {
map.centerAndZoom(city, 12);
var point = new BMap.Point(104.083, 30.686);// 默认中心点
var marker = new BMap.Marker(point);
// 清除覆盖物再加载，否则多次点击不断添加
//map.removeOverlay(Overlay);                // 清除指定覆盖物
map.clearOverlays();// 清除全部
// 根据中心点关键字周边搜索核心代码
var local = new BMap.LocalSearch(map, {
renderOptions:{map: map, autoViewport:true}
});
local.searchNearby(key, city);
map.addControl(new BMap.NavigationControl());//左上角控件
map.enableScrollWheelZoom();
map.addControl(new BMap.ZoomControl());
map.addControl(new BMap.ScaleControl());
}
运行结果如下图所示，热点我自定义包括：学校、景区、KTV、加油站、餐馆、旅店等。
![](https://img-blog.csdn.net/20160519003532738)
**4.根据中心点周边搜索停车场**
主要调用local.searchNearby("停车场", city)即可实现，注意添加覆盖物前清除原有覆盖物。
function SearchCarMap(city) {
map.centerAndZoom(city, 12);
map.clearOverlays();
var local = new BMap.LocalSearch(map, {
renderOptions:{map: map, autoViewport:true}
});
local.searchNearby("停车场", city);
map.addControl(new BMap.NavigationControl());
map.enableScrollWheelZoom();
map.addControl(new BMap.ZoomControl());
map.addControl(new BMap.ScaleControl());
}
运行结果如下图所示：
![](https://img-blog.csdn.net/20160519004129063)
**5.当前位置定位北理工到北交大**
定义当前位置使用自定义函数添加覆盖物及[信息框](http://developer.baidu.com/map/jsdemo-mobile.htm#d0_1)显示"北理地址"。
// 自定义创建标注
function addMarker(point, a, b) {
var marker = new BMap.Marker(point);
map.addOverlay(marker);
marker.addEventListener("click", function () {// 监听标注事件
var opts = {// 创建信息窗口
width: 250,// 信息窗口宽度
height: 100,// 信息窗口高度
title: a// 信息窗口标题
}
// 创建信息窗口对象
var infoWindow = new BMap.InfoWindow(b, opts);
map.openInfoWindow(infoWindow, point);// 打开信息窗口
});
}
运行结果如下图所示：
![](https://img-blog.csdn.net/20160519004220564)
你可以看到在北理和北交之间绘制一条直线，同时点击直线返回距离。通过获取北理PointA的经纬度，北交PointB的经纬度，然后显示添加两个覆盖物，地图中心设置为两个学校的中心点PointC。
// 定位北理和北交核心代码：
function LocateMyPlcae() {
map.clearOverlays();
var pointA_A = 116.32377;
var pointA_B = 39.967031;
var pointA = new BMap.Point(pointA_A, pointA_B);
var pointB_A = 116.34964;
var pointB_B = 39.957990;
var pointB = new BMap.Point(pointB_A, pointB_B);
var pointC = new BMap.Point((pointA_A+pointB_A)/2,
(pointA_B+pointB_B)/2);
map.centerAndZoom(pointC, 15);
var markerA = new BMap.Marker(pointA);
var markerB = new BMap.Marker(pointB);
// 将标注添加到地图中 官方方法
//map.addOverlay(markerA);
//map.addOverlay(markerB);
// 创建标注1 自定义方法
addMarker(pointA, "北京理工大学", "地址：北京理工大学11号学生公寓");
// 创建标注2自定义方法
addMarker(pointB, "北京交通大学", "地址：北京交通大学第一教学楼");
// 开始测距定义折线
var polyline = new BMap.Polyline([pointA, pointB],
{ strokeColor: "orange", strokeWeight: 6, strokeOpacity: 0.5 });
// 添加折线到地图上
map.addOverlay(polyline);
// 监听标注事件 获取两点距离,保留小数点后两位
polyline.addEventListener("click", function () {
alert('两地距离是：' +
(map.getDistance(pointA, pointB)).toFixed(2) + ' 米。');
});
map.enableScrollWheelZoom();// 启用滚轮放大缩小
map.addControl(new BMap.ZoomControl());// 添加地图缩放控件
map.addControl(new BMap.ScaleControl());// 添加比例尺控件
map.addControl(new BMap.NavigationControl());
map.addControl(new BMap.OverviewMapControl());
}
运行结果如下所示：
![](https://img-blog.csdn.net/20160519004327815)

**6.定位北理工大北交大附近餐馆**
对应JavaScript代码中GetResMyPlace()函数，在5的基础上增加代码：
var local = new BMap.LocalSearch(map, {
renderOptions:{map: map}
});
local.search("餐馆");
运行结果如下图所示：
![](https://img-blog.csdn.net/20160519004402488)

**7.百度地图路线导航**
百度地图导航主要是调用浏览器进行，需要点击北京理工大学，选为起点，然后会推荐路线。核心代码如下所示：
// 导航
function RouteMyPlace() {
map.clearOverlays();
var pointA_A = 116.32377;
var pointA_B = 39.967031;
var pointA = new BMap.Point(pointA_A, pointA_B);
var pointB_A = 116.34964;
var pointB_B = 39.957990;
var pointB = new BMap.Point(pointB_A, pointB_B);
var pointC = new BMap.Point((pointA_A+pointB_A)/2,
(pointA_B+pointB_B)/2);
map.centerAndZoom(pointC, 15);
map.enableScrollWheelZoom();
var markerA = new BMap.Marker(pointA);
var markerB = new BMap.Marker(pointB);
map.addOverlay(markerA);// map方法添加覆盖物
map.addOverlay(markerB);
markerA.addEventListener("click", function(){
/*start|end：（必选）
{name:string, latlng:Lnglat}
opts:
mode：导航模式，固定为
BMAP_MODE_TRANSIT、BMAP_MODE_DRIVING、
BMAP_MODE_WALKING、BMAP_MODE_NAVIGATION
分别表示公交、驾车、步行和导航，（必选）
region：城市或县名 给定region时，认为起点和终点都在同一城市，
除非单独给定起点或终点的城市
origin_region/destination_region：同上
*/
var start = {
name:"北京理工大学"
}
var end = {
name:"北京交通大学"
}
var opts = {
mode:BMAP_MODE_DRIVING,
region:"北京"
}
var ss = new BMap.RouteSearch();
ss.routeCall(start,end,opts);
});
map.addControl(new BMap.ZoomControl());
map.addControl(new BMap.ScaleControl());
map.addControl(new BMap.NavigationControl());
map.addControl(new BMap.OverviewMapControl());
}

![](https://img-blog.csdn.net/20160519004439926)
其中推荐路线如下图所示，你自己也可以做些相关研究。

![](https://img-blog.csdn.net/20160519004506083)
﻿﻿
PS：最后希望文章对你有所帮助！这篇文章主要是一些在线笔记和入门介绍，非常基础的内容，详细知识参考百度地图官方文档。
虽然Android百度地图我也研究过，但是由于网络问题、Android环境我不太熟悉（电脑太卡啦），随意导致做了一些C\#调用WebBrowser浏览器控件加载本地的JS文件的技巧，当然Android也可以通过WebView控件实现。(By:Eastmount 2016-05-15 深夜2点半[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))
﻿﻿
﻿﻿

