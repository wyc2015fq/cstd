# visual studio MFC 基于google map的应用 - 三少GG - CSDN博客
2012年11月29日 17:26:12[三少GG](https://me.csdn.net/scut1135)阅读数：2327
[https://developers.google.com/maps/](https://developers.google.com/maps/)
https://developers.google.com/maps/documentation/javascript/v2/examples/?hl=zh-cn
**[https://developers.google.com/maps/documentation/javascript/reference?hl=en](https://developers.google.com/maps/documentation/javascript/reference?hl=en)**
/////////////////////////////////////////////
http://blog.chinaunix.net/uid-21658993-id-3051557.html
 以前弄的一个东西和gps有关，当时vc不怎么会，上位机地图功能没实现一直很遗憾。
今天特地在网上学了一下怎样在对话框中嵌入地图。
网上似乎说的很复杂，这里简单讲下。
1、在vs2005下创建一个基于对话框的应用程序。
2、添加webbrower控件，方法如下：
工具----选择工具箱项------COM组件，microsoft brower前面的复选框勾选上。
（添加其他的active X控件也是同样的方法）
3、在对话框中拖入web browser控件，设置适当的大小来显示地图。
4、为web browser控件添加一个control类型的变量m_map。
5、在对话框初始化函数中添加如下代码：
m_map.Navigate(_T("file:///D:/Google-earth.html"), NULL, NULL, NULL, NULL);
其中Google-earth.html文件是事先放在D盘中的文件。
6、Google-earth.html文件是从网上下载的，源代码如下：
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    "[http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd](http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd)">
<html xmlns="[http://www.w3.org/1999/xhtml](http://www.w3.org/1999/xhtml)" xmlns:v="urn:schemas-microsoft-com:vml">
  <head>
    <meta http-equiv="content-type" content="text/html; charset=utf-8"/>
    <title>Google 地图 JavaScript API 示例: 地图标记</title>
    <script src="[http://ditu.google.cn/maps?file=api&v=2&key=ABQIAAAAzr2EBOXUKnm_jVnk0OJI7xSosDVG8KKPE1-m51RBrvYughuyMxQ-i1QfUnH94QxWIa6N4U6MouMmBA&hl=zh-CN](http://ditu.google.cn/maps?file=api&amp;v=2&amp;key=ABQIAAAAzr2EBOXUKnm_jVnk0OJI7xSosDVG8KKPE1-m51RBrvYughuyMxQ-i1QfUnH94QxWIa6N4U6MouMmBA&hl=zh-CN)"
            type="text/javascript"></script>
    <script type="text/javascript">
    function initialize() {
      if (GBrowserIsCompatible()) {
        var map = new GMap2(document.getElementById("map_canvas"));
        map.setCenter(new GLatLng(30.2, 120.2), 14);
     map.addControl(new GLargeMapControl());//大的缩放条
     //map.addControl(new GSmallMapControl());//小的缩放条
        map.addControl(new GMapTypeControl(true));
        // map.addControl(new GMapTypeControl());
        // 在随机位置向地图中添加 10 个标记
        var bounds = map.getBounds();
        var southWest = bounds.getSouthWest();
        var northEast = bounds.getNorthEast();
        var lngSpan = northEast.lng() - southWest.lng();
        var latSpan = northEast.lat() - southWest.lat();
        for (var i = 0; i < 1; i++) {
          var point = new GLatLng(40.2, 120.2);
          map.addOverlay(new GMarker(point));
        }
      }
    }
    </script>
  </head>
  <body onload="initialize()" onunload="GUnload()">
    <div id="map_canvas" style="width: 800px; height: 500px"></div>
  </body>
</html>
7、编译执行，便可看到打开指定坐标的地图了！
效果图如下：
![](http://blog.chinaunix.net/attachment/201112/30/21658993_1325229419D5yr.jpg)
至于更细致的研究，得以后有时间接着再弄。
上传俩地图文件如下：
![](http://blog.chinaunix.net/blog/image/attachicons/zip.gif) google_map.zip 
上传exe文件如下：
![](http://blog.chinaunix.net/blog/image/attachicons/zip.gif) google_map（exe）.zip 
