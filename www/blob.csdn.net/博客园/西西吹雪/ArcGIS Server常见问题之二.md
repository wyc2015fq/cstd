
# ArcGIS Server常见问题之二 - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[ArcGIS Server常见问题之二](https://www.cnblogs.com/watsonyin/archive/2006/03/29/361647.html)
同样摘自ESRI公司网站。

内容摘要|
本指导描述了如何配置Windows XP SP2防火墙，使得ArcGIS Server能正常工作。为了完成下面
的过程，必须安装Windows XP Service Pack 2，下面的步骤需要为ArcGIS Server配置Windows
防火墙。|
过程描述|
当升级的Windows XP SP2，将安装Windows防火墙。其缺省的设置使得Windows阻止所有从ArcGIS
Server的连接。解决这个问题，需要在Windows防火墙设置中打开80端口，135端口，ArcSOM.exe
和ArcSOC.exe。
1、启动Windows防火墙。开始 > 设置 > 控制面板 > Windows防火墙。缺省情况下，防火墙是启
用的，这是推荐的设置。
2、点击“例外”选项卡。
3、点击“添加端口”。
4、添加下面的信息：
名称：Web Port(http)
端口号：80
类型：TCP
点击“确定”。
5、添加135端口，点击“添加端口”。
6、添加下面的信息：
名称：DCOM(ArcGIS Server)
端口号：135
类型：TCP
点击“确定”。
7、点击“添加程序”将ArcSOM.exe添加到例外。
8、点击“浏览”，并浏览到：
<ArcGIS Install Directory>\bin\ArcSOM.exe,例如
C:\Program Files\ArcGIS\bin\ArcSOM.exe
点击“确定”。
9、点击“添加程序”将ArcSOC.exe添加到例外。
10、点击“浏览”，并浏览到：
<ArcGIS Install Directory>\bin\ArcSOC.exe,例如
C:\Program Files\ArcGIS\bin\ArcSOC.exe
点击“确定”。
11、这些端口和程序入口现在将显示在程序和服务列表中。确保其之前的Check框被选中。
12、关闭Windows防火墙。|
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
内容摘要|
我们使用ArcGIS Server .net ADF来开发应用程序的时候，使用Map WebControl的话，一般都是通过设置控件的属性来设定Host以及MapServerObject。那如何在程序运行过程中来动态的改变这些属性，以使得地图的内容发生相应的变化。下面是c\#的代码：|
过程描述|
'Map1是地图控件
Map1.Host = "ServerName";
Map1.ServerObject = "ServerObjectName";
WebMap webMap = Map1.CreateWebMap();
IMapServer mapServer = webMap.MapServer;
IMapServerObjects mapServerObjects = mapServer as IMapServerObjects;
mapServerObjects.RefreshServerObjects();
webMap.Refresh ();|
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
内容摘要|
提供的指导描述了如何创建一个简单的ArcGIS Server ASP.NET网页。为了诊断，尽可能使用最小的应用，这样有助于测试ArcGIS Server和.NET 应用开发框架(ADF)是否配置适当且工作正常。|
过程描述|
在运行这个例子前，在网络服务器上必须安装好.NET ADF，ArcGIS Server已经安装并正常运行，达到所有的系统要求。
1.创建一个ASP.NET应用目录。可以按照下面步骤创建:
A. 在您的目录<drive>:\inetpub\wwwroot下创建一个目录。
B. 使用IIS管理器查看新建的目录的属性。
C. 在目录标签上，单击创建按钮，创建一个ASP.NET应用。
2.把下面的代码复制到文本编辑器中，存为*.aspx文件。然后对它做如下编辑:
A. 把esri:map标签中的Host属性改为运行着ArcGIS Server SOM(Server Object Manager)的机器。
B. 把ServerObject改为一个运行着的MapServer服务器对象。记住服务器对象的名称区分大小写。'World'和'world'不一样。
<%@ Register TagPrefix="esri" Namespace="ESRI.ArcGIS.Server.WebControls"
Assembly="ESRI.ArcGIS.Server.WebControls, Version=9.0.0.2, Culture=neutral, PublicKeyToken=8fc3cc631e44ad86" %>
<html>
<head>
</head>
<body>
<form id="WebForm" method="post" runat="server">
<esri:map id="TheMap" UseMIMEData="True" Width="400px" Height="400px"
style="LEFT: 200px; POSITION: relative; TOP: 30px" runat="server"
BorderStyle="Solid" BorderColor="200,170,35" BorderWidth="10px"
ServerObject="PoliticalEarthquakes" Host="tao" AutoFirstDraw="True">
</esri:map>
</form>
</body>
</html>
3.在ASP.NET应用目录下创建一个web.config文件。下面显示了一个web.config文件的例子。
使用ArcGIS Server机器上的agsusers组中的帐户配置这个文件。
把用户名和密码用明文写在web.config文件中，可能不是存储这个信息的安全方法。微软提供了加密机制。这个例子只是用来诊断的。
<?xml version="1.0" encoding="utf-8" ?>
<configuration>
<system.web>
<!-- Session state must be enabled for the map control -->
<sessionState mode="InProc" />
<identity impersonate="true" userName="SomeDomain\SomeAcct" password="SomePassword" />
</system.web>
</configuration>
4.在浏览器中输入http://servername/virtualdirectory/myapp.aspx 来测试这个应用程序。
如果ArcGIS Server已经安装，.NET ADF工作正常，那么将显示一幅地图。
5.完成测试后，删除目录中的文件，- 特别是包含明文密码的web.config文件。|

－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
内容摘要|
虽然说ArcGIS Server的开发技术当中最重要的技术是ArcOBjects。但是和在C/S模式下使用的ArcObjects还是会有一些差别，一个很重要的区别就是不能使用New方法来创建ArcObjects。|
过程描述|
1 web应用调用的是远程的Ao组件，这些Ao对象都运行在服务器ArcSoc.exe进程中，由服务器上下文(server context)统一来创建(IServerContext::CreateObject),并服务器上下文来统一的管理和操作，比如Ao对象之间的交互，释放等。
2 ArcGIS Server是一个可分布式部署的软件，GIS Server和Web应用可以部署在不同的机子上，而web应用的机子上只需要安装ADF运行包，只有ArcObjects的代理，不安装ArcObjects本身，因此web 应用没有能力来创建本地的ArcObjects对象，这也是不能使用New方式来创建ArcObjects的原因。|
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
内容摘要|
使用ArcGIS Server开发的web应用程序，有时候会需要把分析结果以graphics的方式临时添加到地图上，但是graphics会遮盖住下面的标注。设透明也无效。|
过程描述|
解决办法：
打开ArcMap,把Serverobject中用到的地图文档(*.mxd)打开，
打开Labeling工具条，点击label weight ranking工具，
在弹出的label weight ranking对话框上，把<default>的feature weight设为none，
停止并启动Serverobject。|

－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
内容摘要|
在设计阶段，使用Impersonation控件的属性页面设置用户、密码、域或机器的Identity属性的时候出错。|
过程描述|
出错原因是微软的.Net框架认证在操作系统上没有必要的权限。
处理方法：
1、打开控制面板
2、打开管理工具页面
3、本地打开安全设置
4、打开本地策略
5、选择用户权利指派
6、选择“以操作系统方式操作”
7、添加“agsadmin”和“agsusers”两个组
8、确认本地策略设置复选框处于选定状态
9、确定退出界面
10、重新启动计算机|
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
内容摘要|
下面的文章讲的是怎么样更新一个使用ESRI.net web controls和ArcGIS Server 9.0写的web应用，使得它能支持Arcgis Server 9.1版本。
这里讲的是ArcGIS Server 9.0的应用指的是在安装了全部Service Packs的基础上的。|
过程描述|
使用下面的过程在9.1中来修改已有的9.0的web应用。
这些步骤对于仅仅把ArcGIS Server 9.0的应用运行在安装了9.1的机器上时候是不必要的。
1 在Microsoft Visual Studio .NET中打开web应用。
2 在HTML 视图中，用下面的语句替换已有的 @Register 语句。
<%@ Register TagPrefix="esri" Namespace="ESRI.ArcGIS.Server.WebControls" Assembly="ESRI.ArcGIS.Server.WebControls, Version=9.1.0.722, Culture=neutral, PublicKeyToken=8fc3cc631e44ad86" %>
注意："Version="属性值必须和上面语句中的一样。
3 重新编译并保存应用。|
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
内容摘要|
很多线划图数据进ArcSDE库中都会有些对象转换不进去。这些对象有很大一部分是由于数据本身有自相交的情况。如果这些线划图数据只是用来做底图，而不需要做对象分析。则可以采用数据简单化操作后再转换入ArcSDE中。具体操作如下例子：|
过程描述|
Private Sub SimplifyPolyLine(pPolyline As esriGeometry.Polyline)
'通过QI取得线对象的拓扑操作接口
Dim pTopologicalOperator As esriGeometry.ITopologicalOperator
Set pTopologicalOperator = pPolyline
'执行简单化操作
pTopologicalOperator.Simplify
'简单化操作完了的数据可以转换入ArcSDE中了
'下面代码用来展示简单化后的线数据的每个部分
Dim pGeometryCollection As esriGeometry.IGeometryCollection
Set pGeometryCollection = pPolyline
Dim i As Long
For i = 0 To pGeometryCollection.GeometryCount - 1
Dim TempPolyline As esriGeometry.IGeometryCollection
Dim TempPath As esriGeometry.IPath
Set TempPolyline = New esriGeometry.Polyline
Set TempPath = pGeometryCollection.Geometry(i)
TempPolyline.AddGeometry TempPath
MapControl1.FlashShape TempPolyline
Set TempPath = Nothing
Set TempPolyline = Nothing
Next i
End Sub
备注：点、线、面都可以执行简单化操作。可以根据不同的业务需求来使用这些功能。|
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
内容摘要|
实现多个实体的联合，通过这种联合可以与其他几何实体进行空间关系判断，请看如下实例|
过程描述|
''''''''''''''''''''''''''''''''''''''''''''''''
dim I as interger
Dim SelectFeatureLineArr is Iarray
Set SelectFeatureLineArr = new Array
‘加入实体到array中，之后
Dim pTmpGeom As IGeometry
Dim pGeom As IGeometry
Dim pOutputGeometry As IGeometry
Dim pTopoOperator As ItopologicalOperator
Dim lFeature As iFeature
For i = 0 To SelectFeatureLineArr.Count - 1
Set lFeature = SelectFeatureLineArr.Element(i)
Set pGeom = lFeature.ShapeCopy
If i = 0 Then ' if its the first feature
Set pTmpGeom = pGeom
Set pOutputGeometry = pTmpGeom
Else ' merge the geometry of the features
Set pTopoOperator = pTmpGeom
Set pOutputGeometry = pTopoOperator.Union(pGeom)
Set pTmpGeom = pOutputGeometry
End If
Next i
|
------------------------------------------------------------------------------------------------------------------
内容摘要|
提供了判断几何学之间空间关系的成员、方法。 其对如下产品有效：
ArcGIS Engine, ArcGIS Desktop, and ArcGIS Server.|
过程描述|
描述
判断两个空间实体之间在几何上是否有关联并返回 bool变量，对于一些关联的存在判断有限制(要求输入相同的尺寸)。 大部份的预先定义表示关系的操作员是互斥的。当使用时，关联操作是用来决定两个几何空间实体之间存在的特定的关系。
Description
Contains 包含指出是否这几何实体包含另一个几何实体
Crosses 交叉指出两个实体是否交叉.
Disjoint 脱节指出两个实体在几何上是不是没有共同的点.，来判断它们之间是否是横断关系
Equals 相等指出两个实体是否相同并定义了相同的点
Overlaps 输入的实体是否 重叠
Relation 两实体之间是否有定义关联存在.
Touches 相触与，指出两实体之间是否有相触与的关系.
Within 判断一个实体是否被另一个实体所包含
使用VB实例
Dim pRelOp As IrelationalOperator
Dim oneFeature As iFeature
Dim twoFeature As iFeature
Set pRelOp = oneFeature.ShapeCopy
If pRelOp.Touches(twoFeature.Shape) Then
End if|
-----------------------------------------------------------------------------------------------------------------
内容摘要|
图层渲染是GIS应用之中十分常用的功能，所有基于ArcOjects组件的ArcGIS软件产品(如: ArcGIS Desktop, ArcGIS Engine, ArcGIS Server), 还有ArcIMS和MapObjects(Windows Edition & Java Edition)都提供了各种实用的渲染方法，详细列举如下:
ArcOjects提供了SimpleRenderer, UniqueValueMapRenderer, ClassBreaksRenderer, DotDensityRenderer, ProportionalSymbolRenderer, ChartRenderer;
ArcIMS提供了SimpleRenderer, SimpleLabelRenderer, ValueMapRenderer, ValueMapLableRenderer, ScaleDependentRenderer, GroupRenderer;
MapObjects Windows Edition提供了ValueMapRenderer, ClassBreaksRenderer, DotDensityRenderer, LabelRenderer, EventRenderer, ZRenderer, GroupRenderer;
MapObjects JavaEdition提供了SimpleRenderer, ValueMapRenderer, LabelRenderer, ScaleDependentRenderer, GroupRenderer;
跟一般的渲染功能有所不同，在ArcIMS和MapObjects的两个版本中都提供的GroupRender它本身并不利用任何图层的属性来对图层进行渲染，而是通过借用其他Renderer的功能，把它们组合起来，实现对同一个图层进行不同类型的渲染效果的同时展现，比如对一个包含了人口总数量，男性人口数量和女性人口数量的世界地图图层(面状)先利用人口总数量属性现实分类渲染的效果，同时利用男性人口数量和女性人口数量属性用直棒图选然展现出两个数值比较的效果。
可惜的是ArcObjects中偏偏没有实现这种渲染功能。不过基于COM技术的ArcObjects所具有的可扩展性为我们自己去实现"GroupRenderer"提供了可能。|
过程描述|
ArcObjects中，图层的渲染效果是通过实例化一个实现了某个渲染接口(ISimpleRenderer, IUniqueValueMapRenderer, IClassBreaksRenderer, IDotDensityRenderer, IProportionalSymbolRenderer, IChartRenderer之一)的CoClass类，然后作为该渲染接口类型的变量赋给图层(IGeoFeatureLayer)的Renderer属性来实现的。这个Renderer属性是IFeatureRenderer接口类型，IFeatureRenderer中定义了一个叫做Draw的方法，其实ArcObjects中各种各样的渲染效果就是通过这个Draw方法画出来的，因为各个渲染效果CoClass都实现了IFeatureRenderer接口。所以要实现"GroupRenderer"效果，只要实现了IFeatureRenderer接口就可以了。以下提供VB和ArcGIS Server Java API的例程供大家参考：
[VB6: CustomGroupRenderer.cls]
Option Explicit
Implements IFeatureRenderer
Private m_Renderers As Collection
Private Sub Class_Initialize()
On Error GoTo ErrHand
Set m_Renderers = New Collection
GoTo EndProc
ErrHand:
MsgBox "Class Initialize" & Err.Description
EndProc:
Exit Sub
End Sub
Private Function IFeatureRenderer_CanRender(ByVal featClass As esriGeoDatabase.IFeatureClass, ByVal Display As esriDisplay.IDisplay) As Boolean
On Error GoTo ErrHand
If Not featClass.ShapeType = esriGeometryNull Then
IFeatureRenderer_CanRender = True
Else
IFeatureRenderer_CanRender = False
End If
GoTo EndProc
ErrHand:
MsgBox "CanRender" & Err.Description
EndProc:
Exit Function
End Function
Private Sub IFeatureRenderer_Draw(ByVal Cursor As esriGeoDatabase.IFeatureCursor, _
ByVal drawPhase As esriSystem.esriDrawPhase, _
ByVal Display As esriDisplay.IDisplay, _
ByVal trackCancel As esriSystem.ITrackCancel)
On Error GoTo ErrHand
Dim i As Integer
For i = 1 To m_Renderers.Count
Dim r As IFeatureRenderer
Set r = m_Renderers.Item(i)
If (TypeOf r Is ISimpleRenderer) And (drawPhase = esriDPGeography) Then
r.Draw Cursor, drawPhase, Display, trackCancel
ElseIf (TypeOf r Is IUniqueValueRenderer) And (drawPhase = esriDPGeography) Then
r.Draw Cursor, drawPhase, Display, trackCancel
ElseIf (TypeOf r Is IClassBreaksRenderer) And (drawPhase = esriDPGeography) Then
r.Draw Cursor, drawPhase, Display, trackCancel
ElseIf (TypeOf r Is IDotDensityRenderer) And (drawPhase = esriDPGeography) Then
r.Draw Cursor, drawPhase, Display, trackCancel
ElseIf (TypeOf r Is IProportionalSymbolRenderer) And (drawPhase = esriDPAnnotation) Then
r.Draw Cursor, drawPhase, Display, trackCancel
ElseIf (TypeOf r Is IChartRenderer) And (drawPhase = esriDPAnnotation) Then
r.Draw Cursor, drawPhase, Display, trackCancel
End If
Next i
GoTo EndProc
ErrHand:
MsgBox "Draw" & Err.Description
EndProc:
Exit Sub
End Sub
Private Sub IFeatureRenderer_PrepareFilter(ByVal pFeatClass As esriGeoDatabase.IFeatureClass, ByVal QueryFilter As esriGeoDatabase.IQueryFilter)
Dim i As Integer
For i = 1 To m_Renderers.Count
Dim r As IFeatureRenderer
Set r = m_Renderers.Item(i)
r.PrepareFilter pFeatClass, QueryFilter
Next i
End Sub
Private Property Set IFeatureRenderer_ExclusionSet(ByVal pIDSet As esriCarto.IFeatureIDSet)
End Property
Private Property Get IFeatureRenderer_RenderPhase(ByVal drawPhase As esriSystem.esriDrawPhase) As Boolean
On Error GoTo ErrHand
IFeatureRenderer_RenderPhase = True
Exit Property
ErrHand:
MsgBox "Get RenderPhase" & Err.Description
End Property
Private Property Get IFeatureRenderer_SymbolByFeature(ByVal Feature As esriGeoDatabase.IFeature) As esriDisplay.ISymbol
On Error GoTo ErrHand
Dim pSym As ISymbol
Set pSym = m_LegendGroup.Class(0).Symbol
Set IFeatureRenderer_SymbolByFeature = pSym
GoTo EndProc
ErrHand:
MsgBox Err.Description
EndProc:
Set pSym = Nothing
Exit Property
End Property
Public Sub AddRenderer(ByVal renderer As esriCarto.IFeatureRenderer)
m_Renderers.Add renderer
End Sub
==========================================================================================================================================================
[ArcGIS Server Java API]
//IGroupRenderer.java
package com.esri.arcgis.samples.carto.renderers;

import com.linar.jintegra.AutomationException;
import java.io.IOException;
import java.io.Serializable;
public interface IGroupRenderer extends Serializable {
public static final int IID27AC33C1_506B_41F6_B2F0_D6F163CB7699 = 1;
public static final int xxDummy = 0;
public static final String IID = "27AC33C1-506B-41F6-B2F0-D6F163CB7699";
public void addRenderer(Object iRenderer) throws IOException, AutomationException;
}
//CustomGroupRenderer.java
package com.esri.arcgis.samples.carto.renderers;
import java.io.IOException;
import java.util.ArrayList;
import com.esri.arcgis.server.*;
import com.esri.arcgis.carto.*;
import com.esri.arcgis.display.*;
import com.esri.arcgis.geodatabase.*;
import com.esri.arcgis.system.*;
import com.esri.arcgis.geometry.esriGeometryType;
import com.esri.arcgis.system.esriDrawPhase;
import com.linar.jintegra.AutomationException;
public class CustomGroupRenderer implements IFeatureRenderer, IGroupRenderer {
private ILegendGroup pLegendGroup;
private IServerContext serverContext;
private ArrayList renderers = new ArrayList();
public CustomGroupRenderer(IServerContext sc) {
try {
serverContext = sc;
pLegendGroup = new ILegendGroupProxy(serverContext.createObject(LegendGroup.getClsid()));
ILegendClass pLegendClass = new ILegendClassProxy(sc.createObject(LegendClass.getClsid()));
pLegendGroup.addClass(pLegendClass);
pLegendGroup.setVisible(true);
pLegendGroup.setEditable(true);
} catch (AutomationException e) {
System.out.println("AutomationException " + e);
} catch (IOException e) {
System.out.println("IOException " + e);
}
}
public boolean canRender(IFeatureClass iFeatureClass, IDisplay iDisplay) throws
IOException, AutomationException {
if (iFeatureClass.getShapeType() == esriGeometryType.esriGeometryNull) {
return true;
} else {
return false;
}
}
public void prepareFilter(IFeatureClass iFeatureClass,
IQueryFilter iQueryFilter) throws IOException,
AutomationException {
if (renderers.size() > 0) {
for (int i = 0; i < renderers.size(); i++) {
IFeatureRenderer pFR = new IFeatureRendererProxy(renderers.get(i));
pFR.prepareFilter(iFeatureClass, iQueryFilter);
}
}
}
public void draw(IFeatureCursor iFeatureCursor, int drawPhase,
IDisplay iDisplay, ITrackCancel iTrackCancel) throws
IOException, AutomationException {
if (renderers.size() > 0) {
for (int i = 0; i < renderers.size(); i++) {
Object obj = renderers.get(i);
if (obj instanceof ISimpleRenderer && drawPhase == esriDrawPhase.esriDPGeography) {
IFeatureRenderer pFR = new IFeatureRendererProxy(obj);
pFR.draw(iFeatureCursor, drawPhase, iDisplay, iTrackCancel);
}
if (obj instanceof IUniqueValueRenderer && drawPhase == esriDrawPhase.esriDPGeography) {
IFeatureRenderer pFR = new IFeatureRendererProxy(obj);
pFR.draw(iFeatureCursor, drawPhase, iDisplay, iTrackCancel);
}
if (obj instanceof IClassBreaksRenderer && drawPhase == esriDrawPhase.esriDPGeography) {
IFeatureRenderer pFR = new IFeatureRendererProxy(obj);
pFR.draw(iFeatureCursor, drawPhase, iDisplay, iTrackCancel);
}
if (obj instanceof IDotDensityRenderer && drawPhase == esriDrawPhase.esriDPGeography) {
IFeatureRenderer pFR = new IFeatureRendererProxy(obj);
pFR.draw(iFeatureCursor, drawPhase, iDisplay, iTrackCancel);
}
if (obj instanceof IProportionalSymbolRenderer && drawPhase == esriDrawPhase.esriDPAnnotation) {
IFeatureRenderer pFR = new IFeatureRendererProxy(obj);
pFR.draw(iFeatureCursor, drawPhase, iDisplay, iTrackCancel);
}
if (obj instanceof IChartRenderer && drawPhase == esriDrawPhase.esriDPAnnotation) {
IFeatureRenderer pFR = new IFeatureRendererProxy(obj);
pFR.draw(iFeatureCursor, drawPhase, iDisplay, iTrackCancel);
}
}
}
}
public ISymbol getSymbolByFeature(IFeature iFeature) throws IOException,
AutomationException {
ISymbol pSym = pLegendGroup.esri_getClass(0).getSymbol();
return pSym;
}
public boolean isRenderPhase(int drawPhase) throws IOException,
AutomationException {
return true;
}
public void setExclusionSetByRef(IFeatureIDSet iFeatureIDSet) throws
IOException, AutomationException {
}
public void addRenderer(Object iRenderer) throws
IOException, AutomationException {
renderers.add(iRenderer);
}
}
如果想要在TOC控件上动态的展现出渲染的效果，还需实现ILegendInfo接口，这里不做详细说明。|





posted on2006-03-29 12:13[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=361647)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
