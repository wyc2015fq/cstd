# Skyline画多边形，标图标，点击弹对话框 - 左直拳的马桶_日用桶 - CSDN博客
2017年10月30日 19:29:45[左直拳](https://me.csdn.net/leftfist)阅读数：1120
在SkyLine里面，画一个多边形，然后点击这个多边形，弹出一个提示框，显示该多边形的相关信息，咋写呢？
![这里写图片描述](https://img-blog.csdn.net/20171030192817167?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
步骤：
> 
1、在地图上画多边形，数据无非就是一些点，比二维多了一个高度的坐标  
  2、在该多边形上面标上一个图标，比如一张水滴图标 
  3、将该多边形绑定定一个消息框
具体上代码：
1、画多边形
```
var cVerticesArray = [
      -122.415025, 37.76059,  10,
      -122.415868, 37.760546, 11,
      -122.415922, 37.761244, 12,
      -122.415592, 37.761254, 13,
      -122.415557, 37.760973, 14,
      -122.415081, 37.76099,  15,
      ];
var cRing = sgworld.Creator.GeometryCreator.CreateLinearRingGeometry(cVerticesArray);
var cPolygonGeometry = sgworld.Creator.GeometryCreator.CreatePolygonGeometry(cRing, null);
var nLineColor = 0xFF00FF00; 
var nFillColor = 0x7FFF0000; 
var eAltitudeTypeCode = 3; 
var pg = sgworld.Creator.CreatePolygon(cPolygonGeometry, nLineColor, nFillColor, eAltitudeTypeCode, groupId, 
"描述");//var groupId = sgworld.ProjectTree.CreateGroup("分组名称", "父节点，为空则为根节点")
```
2、在该多边形上标上图标
```
var pos = pg.Position.Copy();
var imageLabelStyle = sgworld.Creator.CreateLabelStyle();
imageLabelStyle.PivotAlignment = "Bottom,Center";
var iconUrl = "http://localhost/test.web/locate.png";//切记，图片的路径务必为完整的、绝对路径，相对路径不行，可能skyline只是一个activeX控件，比较弱的缘故吧
sgworld.Creator.CreateImageLabel(pos, iconUrl, imageLabelStyle, groupId, "icon_1");
```
3、为多边形绑定一个弹出框
```java
function bindMessage() {
    var caption = "标题";
    var src = "";
    var left = 0;
    var top = 0;
    var timeout = -1;
    var mess = sgworld.Creator.CreatePopupMessage(caption, caption, left, top, 300, 200, timeout);
    mess.InnerHtml = "<div>Hello World!</div>"; 
    mess.ShowCaption = true;
    return mess.ID;
}
pg.Message.MessageID = bindMessage();//pg为该多边形，参见代码1
```
如此，则点击出对话框矣！
