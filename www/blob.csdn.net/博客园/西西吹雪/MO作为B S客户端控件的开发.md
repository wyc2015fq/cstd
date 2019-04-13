
# MO作为B-S客户端控件的开发 - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[MO作为B/S客户端控件的开发](https://www.cnblogs.com/watsonyin/archive/2005/05/08/150809.html)
开发GIS与OA一体化系统时，OA中经常需要使用图形的浏览、查询等功能，这在OA系统采用C/S模式的情况下，并不困难，但在采用B/S架构时，却往往不知从何下手。一般的做法时采用ArcIMS等软件作为服务器端软件进行开发，但这些软件需要额外购买，很不划算。其实我们将MO作为一个ActiveX控件插入到网页中，用JavaScript进行控制，也可以实现非常强大的功能。
下面举一个例子，可以实现加载图层，放大，缩小和平移功能。
<html>
<head>
<meta http-equiv="Content-Language" content="zh-cn">
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<meta name="GENERATOR" content="Microsoft FrontPage 4.0">
<meta name="ProgId" content="FrontPage.Editor.Document">
<title>New Page 1</title>
</head>
<body>
<SCRIPT language=JavaScript>
var MapState = 0;
function LoadLayers()
{
Map1.ScrollBars = false;
var dc = new ActiveXObject("MapObjects2.DataConnection")
dc.Server = "sde81:server";
dc.DataBase = "esri_sde";
dc.User="sa";
dc.Password = "sa";
if (dc.connect() == false)
{
return;
}
var gs;
gs = dc.FindGeoDataset("sde.dbo.zd\#1.shape.polygons");
if (gs == null) return;
var ly = new ActiveXObject("MapObjects2.MapLayer");
ly.GeoDataset = gs;
Map1.Layers.Add(ly);
Map1.Refresh();
}
</script>
<script language = "JavaScript" for = "Map1" event = "MouseDown(Button,Shift,X,Y)">
if (MapState == 0)
{
Map1.Extent = Map1.TrackRectangle();
}
else if(MapState ==1)
{
var rect;
rect=Map1.Extent();
rect.ScaleRectangle(1.3);
Map1.Extent = rect;
}
else if(MapState == 2)
{
Map1.Pan();
}
</script>
<script language = "JavaScript" for = "Map1" event = "MouseMove(Button,Shift,X,Y)">
if (MapState == 0)
{
Map1.MousePointer = 51;
}
else if(MapState ==1)
{
Map1.MousePointer = 52;
}
else if(MapState == 2)
{
Map1.MousePointer = 53;
}
</script>
<p>
<object classid="clsid:9BD6A64B-CE75-11D1-AF04-204C4F4F5020" id="Map1" width="572" height="452">
<param name="_Version" value="131072">
<param name="_ExtentX" value="15134">
<param name="_ExtentY" value="11959">
<param name="_StockProps" value="225">
<param name="BackColor" value="16777215">
<param name="BorderStyle" value="1">
<param name="Enabled" value="1">
<param name="Appearance" value="0">
<param name="ScrollBars" value="1">
<param name="RefreshCount" value="10000">
<param name="CancelAction" value="1">
<param name="WindowMode" value="0">
<param name="MinWidth" value="0">
<param name="MousePointer" value="0">
<param name="BorderStyle" value="1">
<param name="BackColor" value="16777215">
<param name="FullRedrawOnPan" value="0">
</object>
</p>
<p>
<input type="button" value="LoadLayer" name="B3"  onclick = "LoadLayers()">
<input type="button" value="Zoom In" name="B5"  onclick = "javscript:MapState=0">
<input type="button" value="Zoom Out" name="B6"  onclick = "javscript:MapState=1">
<input type="button" value="Pan" name="B4"  onclick = "javscript:MapState=2">
</p>
<p>

</body>
</html>





posted on2005-05-08 14:37[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=150809)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
