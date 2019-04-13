
# ArcGis Server开发Web GIS新手体验（四）  - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[ArcGis Server开发Web GIS新手体验（四） ](https://www.cnblogs.com/watsonyin/archive/2006/01/18/319424.html)
接（三）
这一节主要贴代码算了，通过代码看一下一些简单功能的实现。偶快要放假了，静不下心来写了，写得也不好。说不定贴代码效果更好。
１、获取所有图层
ESRI.ArcGIS.Server.WebControls.WebMap webmap=Map1.CreateWebMap();
try
{
ESRI.ArcGIS.Carto.IMapDescription descr=webmap.MapDescription;
ddlLayers.Items.Clear();
intid;
for(inti=0;i<=descr.LayerDescriptions.Count-1;i++)
{
id=descr.LayerDescriptions.get_Element(i).ID;
ddlLayers.Items.Add(id.ToString()+","+webmap.LayerNameFromID(id));//ddlLayers是一个DropDownList控件
}
if(ddlLayers.Items.Count>0)
{
ddlLayers.SelectedIndex=0;
}
}
finally
{
webmap.Dispose();
}
２、通过图层的ID获取图层对象
privateIFeatureLayer GetFeatureLayer(intlyid)
{
WebMap webmap=Map1.CreateWebMap();
try
{
ILayer layer=(webmap.MapServerasIMapServerObjects).get_Layer(webmap.DataFrame,lyid);
if(layer==null)
returnnull;
else
{
return(layerasIFeatureLayer);
}
}
finally
{
webmap.Dispose();
}
}
３、新建一个多边形
![](/Images/OutliningIndicators/None.gif)privatevoidMap1_Polygon(objectsender, ESRI.ArcGIS.Server.WebControls.PolygonEventArgs args)
![](/Images/OutliningIndicators/ExpandedBlockStart.gif)![](/Images/OutliningIndicators/ContractedBlock.gif)[  ](/Images/OutliningIndicators/ContractedBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](/Images/OutliningIndicators/InBlock.gif)[   ](/Images/OutliningIndicators/InBlock.gif)if(args.ToolName=="newpolygon")
![](/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](/Images/OutliningIndicators/ContractedSubBlock.gif)[   ](/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](/Images/OutliningIndicators/InBlock.gif)[    IFeatureLayer flayer ](/Images/OutliningIndicators/InBlock.gif)=GetCurFeatureLayer();//获取当前活动图层了函数，这里就不贴了，就是调用GetFeatureLayer(int lyid)函数
![](/Images/OutliningIndicators/InBlock.gif)if(flayer==null)return;
![](/Images/OutliningIndicators/InBlock.gif)[    ](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[    ](/Images/OutliningIndicators/InBlock.gif)if(flayer.FeatureClass.ShapeType!=ESRI.ArcGIS.Geometry.esriGeometryType.esriGeometryPolygon)
![](/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](/Images/OutliningIndicators/ContractedSubBlock.gif)[    ](/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](/Images/OutliningIndicators/InBlock.gif)[     ](/Images/OutliningIndicators/InBlock.gif)stringsc;
![](/Images/OutliningIndicators/InBlock.gif)[     sc ](/Images/OutliningIndicators/InBlock.gif)="<script language=javascript>alert('当前图层何类型不对！')</script>";
![](/Images/OutliningIndicators/InBlock.gif)[     Page.RegisterClientScriptBlock(](/Images/OutliningIndicators/InBlock.gif)"ShapeTypeError",sc);
![](/Images/OutliningIndicators/InBlock.gif)[     ](/Images/OutliningIndicators/InBlock.gif)return;
![](/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[    }](/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[    ](/Images/OutliningIndicators/InBlock.gif)//生成多边形
![](/Images/OutliningIndicators/InBlock.gif)ESRI.ArcGIS.Server.IServerContext context;
![](/Images/OutliningIndicators/InBlock.gif)[    ESRI.ArcGIS.Server.WebControls.WebMap webmap ](/Images/OutliningIndicators/InBlock.gif)=Map1.CreateWebMap();
![](/Images/OutliningIndicators/InBlock.gif)[    webmap.ManageLifetime(flayer);](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[    context ](/Images/OutliningIndicators/InBlock.gif)=webmap.ServerContext;
![](/Images/OutliningIndicators/InBlock.gif)[    ESRI.ArcGIS.Geometry.IPolygon poly ](/Images/OutliningIndicators/InBlock.gif)=context.CreateObject("esriGeometry.Polygon")asESRI.ArcGIS.Geometry.IPolygon;//'new ag.PolygonClass();
![](/Images/OutliningIndicators/InBlock.gif)webmap.ManageLifetime(poly);
![](/Images/OutliningIndicators/InBlock.gif)[    ESRI.ArcGIS.Geometry.IPoint pt;](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[    ESRI.ArcGIS.Geometry.IGeometryCollection ringcol ](/Images/OutliningIndicators/InBlock.gif)=context.CreateObject("esriGeometry.Polygon")asESRI.ArcGIS.Geometry.IGeometryCollection;//new  PolygonClass();
![](/Images/OutliningIndicators/InBlock.gif)webmap.ManageLifetime(ringcol);
![](/Images/OutliningIndicators/InBlock.gif)[    ESRI.ArcGIS.Geometry.IPointCollection ptcol ](/Images/OutliningIndicators/InBlock.gif)=context.CreateObject("esriGeometry.Ring")asESRI.ArcGIS.Geometry.IPointCollection;//new RingClass();
![](/Images/OutliningIndicators/InBlock.gif)webmap.ManageLifetime(ptcol);
![](/Images/OutliningIndicators/InBlock.gif)[    ](/Images/OutliningIndicators/InBlock.gif)objectobj=Type.Missing;
![](/Images/OutliningIndicators/InBlock.gif)[    ](/Images/OutliningIndicators/InBlock.gif)for(inti=0;i<=args.Vectors.Length-1;i++)
![](/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](/Images/OutliningIndicators/ContractedSubBlock.gif)[    ](/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](/Images/OutliningIndicators/InBlock.gif)[     pt ](/Images/OutliningIndicators/InBlock.gif)=webmap.ToMapPoint(args.Vectors[i].X,args.Vectors[i].Y);
![](/Images/OutliningIndicators/InBlock.gif)[     ptcol.AddPoint(pt,](/Images/OutliningIndicators/InBlock.gif)refobj,refobj);
![](/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[    }](/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](/Images/OutliningIndicators/InBlock.gif)[    ringcol.AddGeometry(ptcol ](/Images/OutliningIndicators/InBlock.gif)asIGeometry,refobj,refobj);
![](/Images/OutliningIndicators/InBlock.gif)[    poly ](/Images/OutliningIndicators/InBlock.gif)=ringcolasIPolygon;
![](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[    ](/Images/OutliningIndicators/InBlock.gif)//将多边形写入到图层中
![](/Images/OutliningIndicators/InBlock.gif)ESRI.ArcGIS.Geodatabase.IFeature feature=flayer.FeatureClass.CreateFeature();
![](/Images/OutliningIndicators/InBlock.gif)[    feature.Shape ](/Images/OutliningIndicators/InBlock.gif)=polyasIGeometry;
![](/Images/OutliningIndicators/InBlock.gif)[    feature.Store();](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[    webmap.Refresh();](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[    webmap.Dispose();](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[   }](/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](/Images/OutliningIndicators/ExpandedBlockEnd.gif)[  }](/Images/OutliningIndicators/ExpandedBlockEnd.gif)
![](/Images/OutliningIndicators/None.gif)
![](/Images/OutliningIndicators/None.gif)
[
](/Images/OutliningIndicators/None.gif)
４、矩形选择，获得选择集，并在地图上显示选中的对象。（这个功能花费了偶一天多的时间。）
![](/Images/OutliningIndicators/None.gif)privatevoidMap1_DragRectangle(objectsender, ESRI.ArcGIS.Server.WebControls.ToolEventArgs args)
![](/Images/OutliningIndicators/ExpandedBlockStart.gif)![](/Images/OutliningIndicators/ContractedBlock.gif)[  ](/Images/OutliningIndicators/ContractedBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](/Images/OutliningIndicators/InBlock.gif)[   ](/Images/OutliningIndicators/InBlock.gif)stringstrTool=args.ToolName.ToLower();
![](/Images/OutliningIndicators/InBlock.gif)[   ](/Images/OutliningIndicators/InBlock.gif)if(strTool=="rectsel")
![](/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](/Images/OutliningIndicators/ContractedSubBlock.gif)[   ](/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](/Images/OutliningIndicators/InBlock.gif)[    ](/Images/OutliningIndicators/InBlock.gif)//取得当前层
![](/Images/OutliningIndicators/InBlock.gif)if(ddlLayers.SelectedValue=="")
![](/Images/OutliningIndicators/InBlock.gif)[     ](/Images/OutliningIndicators/InBlock.gif)return;
![](/Images/OutliningIndicators/InBlock.gif)[    IFeatureLayer flayer ](/Images/OutliningIndicators/InBlock.gif)=GetCurFeatureLayer();
![](/Images/OutliningIndicators/InBlock.gif)[    ](/Images/OutliningIndicators/InBlock.gif)if(flayer==null)return;
![](/Images/OutliningIndicators/InBlock.gif)[    ](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[    ](/Images/OutliningIndicators/InBlock.gif)//获得选择集
![](/Images/OutliningIndicators/InBlock.gif)intt1=Environment.TickCount;
![](/Images/OutliningIndicators/InBlock.gif)[    ESRI.ArcGIS.Server.WebControls.WebMap webmap ](/Images/OutliningIndicators/InBlock.gif)=Map1.CreateWebMap();
![](/Images/OutliningIndicators/InBlock.gif)[    ESRI.ArcGIS.Server.IServerContext ctx ](/Images/OutliningIndicators/InBlock.gif)=webmap.ServerContext;
![](/Images/OutliningIndicators/InBlock.gif)[    webmap.ManageLifetime(ctx);](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[    ESRI.ArcGIS.Geodatabase.IWorkspace ws ](/Images/OutliningIndicators/InBlock.gif)=(flayer.FeatureClassasESRI.ArcGIS.Geodatabase.IDataset).Workspace;
![](/Images/OutliningIndicators/InBlock.gif)[    ESRI.ArcGIS.Geometry.IEnvelope env ](/Images/OutliningIndicators/InBlock.gif)=ctx.CreateObject("esriGeometry.Envelope")asESRI.ArcGIS.Geometry.IEnvelope;
![](/Images/OutliningIndicators/InBlock.gif)[    webmap.ManageLifetime(ws);](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[    webmap.ManageLifetime(env);](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[    IPoint pt ](/Images/OutliningIndicators/InBlock.gif)=webmap.ToMapPoint(Convert.ToInt32(Request.Params.Get("maxx")),Convert.ToInt32(Request.Params.Get("maxy")));
![](/Images/OutliningIndicators/InBlock.gif)[    env.XMax ](/Images/OutliningIndicators/InBlock.gif)=pt.X;
![](/Images/OutliningIndicators/InBlock.gif)[    env.YMin ](/Images/OutliningIndicators/InBlock.gif)=pt.Y;
![](/Images/OutliningIndicators/InBlock.gif)[    pt ](/Images/OutliningIndicators/InBlock.gif)=webmap.ToMapPoint(Convert.ToInt32(Request.Params.Get("minx")),Convert.ToInt32(Request.Params.Get("miny")));
![](/Images/OutliningIndicators/InBlock.gif)[    env.XMin](/Images/OutliningIndicators/InBlock.gif)=pt.X;
![](/Images/OutliningIndicators/InBlock.gif)[    env.YMax ](/Images/OutliningIndicators/InBlock.gif)=pt.Y;
![](/Images/OutliningIndicators/InBlock.gif)[    ESRI.ArcGIS.Geodatabase.ISpatialFilter filter ](/Images/OutliningIndicators/InBlock.gif)=ctx.CreateObject("esriGeodatabase.SpatialFilter")asESRI.ArcGIS.Geodatabase.ISpatialFilter;
![](/Images/OutliningIndicators/InBlock.gif)[    webmap.ManageLifetime(filter);](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[    filter.SpatialRel ](/Images/OutliningIndicators/InBlock.gif)=ESRI.ArcGIS.Geodatabase.esriSpatialRelEnum.esriSpatialRelIntersects;
![](/Images/OutliningIndicators/InBlock.gif)[    filter.Geometry ](/Images/OutliningIndicators/InBlock.gif)=envasESRI.ArcGIS.Geometry.IGeometry;
![](/Images/OutliningIndicators/InBlock.gif)[    filter.GeometryField ](/Images/OutliningIndicators/InBlock.gif)=flayer.FeatureClass.ShapeFieldName;
![](/Images/OutliningIndicators/InBlock.gif)[    ESRI.ArcGIS.Geodatabase.ISelectionSet sset ](/Images/OutliningIndicators/InBlock.gif)=flayer.FeatureClass.Select(filter,ESRI.ArcGIS.Geodatabase.esriSelectionType.esriSelectionTypeSnapshot,ESRI.ArcGIS.Geodatabase.esriSelectionOption.esriSelectionOptionNormal,ws);
![](/Images/OutliningIndicators/InBlock.gif)[  　](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[    ](/Images/OutliningIndicators/InBlock.gif)intt2=Environment.TickCount;
![](/Images/OutliningIndicators/InBlock.gif)[    ](/Images/OutliningIndicators/InBlock.gif)intt3=t2-t1;//t3是查询响应的时间，可以用来测试一下性能，呵呵
![](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[    ](/Images/OutliningIndicators/InBlock.gif)//显示选择集
![](/Images/OutliningIndicators/InBlock.gif)intid;
![](/Images/OutliningIndicators/InBlock.gif)[    ESRI.ArcGIS.Geodatabase.IEnumIDs ids;](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[    ids ](/Images/OutliningIndicators/InBlock.gif)=sset.IDs;
![](/Images/OutliningIndicators/InBlock.gif)[    webmap.ManageLifetime(ids);](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[    ids.Reset();](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[    ESRI.ArcGIS.Geodatabase.IFIDSet fidset ](/Images/OutliningIndicators/InBlock.gif)=ctx.CreateObject("esriGeodatabase.FIDSet")asESRI.ArcGIS.Geodatabase.IFIDSet;
![](/Images/OutliningIndicators/InBlock.gif)[    id ](/Images/OutliningIndicators/InBlock.gif)=ids.Next();
![](/Images/OutliningIndicators/InBlock.gif)[    ](/Images/OutliningIndicators/InBlock.gif)while(id>=0)
![](/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](/Images/OutliningIndicators/ContractedSubBlock.gif)[    ](/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](/Images/OutliningIndicators/InBlock.gif)[     fidset.Add(id);](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[     id ](/Images/OutliningIndicators/InBlock.gif)=ids.Next();
![](/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[    }](/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](/Images/OutliningIndicators/InBlock.gif)[    IMapDescription desc ](/Images/OutliningIndicators/InBlock.gif)=webmap.MapDescriptionasIMapDescription;
![](/Images/OutliningIndicators/InBlock.gif)[    webmap.ManageLifetime(desc);](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[    ILayerDescription ldesc ](/Images/OutliningIndicators/InBlock.gif)=desc.LayerDescriptions.get_Element(flyid);
![](/Images/OutliningIndicators/InBlock.gif)[    webmap.ManageLifetime(ldesc);](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[    ldesc.SelectionFeatures ](/Images/OutliningIndicators/InBlock.gif)=fidset;
![](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[    ](/Images/OutliningIndicators/InBlock.gif)//将选择信息保存在session中
![](/Images/OutliningIndicators/InBlock.gif)Session["selection"]=sset;
![](/Images/OutliningIndicators/InBlock.gif)[    Session[](/Images/OutliningIndicators/InBlock.gif)"layerid"]=flyid;
![](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[    webmap.Refresh();](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[    webmap.Dispose();](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[     ](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[   }](/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/ExpandedBlockEnd.gif)[  }](/Images/OutliningIndicators/ExpandedBlockEnd.gif)
![](/Images/OutliningIndicators/None.gif)
![](/Images/OutliningIndicators/None.gif)
[
](/Images/OutliningIndicators/None.gif)
５、删除选中的对象
![](/Images/OutliningIndicators/None.gif)privatevoidDeleteSel()
![](/Images/OutliningIndicators/ExpandedBlockStart.gif)![](/Images/OutliningIndicators/ContractedBlock.gif)[  ](/Images/OutliningIndicators/ContractedBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](/Images/OutliningIndicators/InBlock.gif)[   ](/Images/OutliningIndicators/InBlock.gif)if(Session["layerid"]==null)return;
![](/Images/OutliningIndicators/InBlock.gif)[   ](/Images/OutliningIndicators/InBlock.gif)if(Session["selection"]==null)return;
![](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[   ](/Images/OutliningIndicators/InBlock.gif)intlayerid=(int)Session["layerid"];
![](/Images/OutliningIndicators/InBlock.gif)[   ](/Images/OutliningIndicators/InBlock.gif)if(layerid==-1)return;
![](/Images/OutliningIndicators/InBlock.gif)[   ESRI.ArcGIS.Geodatabase.ISelectionSet sset ](/Images/OutliningIndicators/InBlock.gif)=Session["selection"]asESRI.ArcGIS.Geodatabase.ISelectionSet;
![](/Images/OutliningIndicators/InBlock.gif)[   ](/Images/OutliningIndicators/InBlock.gif)if(sset==null)return;
![](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[   WebMap webmap ](/Images/OutliningIndicators/InBlock.gif)=Map1.CreateWebMap();
![](/Images/OutliningIndicators/InBlock.gif)[   IFeatureLayer layer ](/Images/OutliningIndicators/InBlock.gif)=GetFeatureLayer(layerid);
![](/Images/OutliningIndicators/InBlock.gif)[   ](/Images/OutliningIndicators/InBlock.gif)if(layer==null)return;
![](/Images/OutliningIndicators/InBlock.gif)[   webmap.ManageLifetime(layer);](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[   webmap.ManageLifetime(sset);](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[   ESRI.ArcGIS.Geodatabase.IEnumIDs ids ](/Images/OutliningIndicators/InBlock.gif)=sset.IDs;
![](/Images/OutliningIndicators/InBlock.gif)[   webmap.ManageLifetime(ids);](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[   ids.Reset();](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[   ](/Images/OutliningIndicators/InBlock.gif)intid;
![](/Images/OutliningIndicators/InBlock.gif)[   id ](/Images/OutliningIndicators/InBlock.gif)=ids.Next();
![](/Images/OutliningIndicators/InBlock.gif)[   ESRI.ArcGIS.Geodatabase.IFeature feature;](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[   ESRI.ArcGIS.Server.IServerContext ctx ](/Images/OutliningIndicators/InBlock.gif)=webmap.ServerContext;
![](/Images/OutliningIndicators/InBlock.gif)[   webmap.ManageLifetime(ctx);](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[   ](/Images/OutliningIndicators/InBlock.gif)//将selectionset转化为featurecursor对象
![](/Images/OutliningIndicators/InBlock.gif)ESRI.ArcGIS.Geodatabase.IFeatureCursor fcursor;
![](/Images/OutliningIndicators/InBlock.gif)[   ESRI.ArcGIS.Geodatabase.ICursor cursor;](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[   sset.Search(](/Images/OutliningIndicators/InBlock.gif)null,false,outcursor);
![](/Images/OutliningIndicators/InBlock.gif)[   fcursor ](/Images/OutliningIndicators/InBlock.gif)=cursorasESRI.ArcGIS.Geodatabase.IFeatureCursor;
![](/Images/OutliningIndicators/InBlock.gif)[   ESRI.ArcGIS.esriSystem.ISet pDeleteSet ](/Images/OutliningIndicators/InBlock.gif)=ctx.CreateObject("esriSystem.Set")asESRI.ArcGIS.esriSystem.Set;
![](/Images/OutliningIndicators/InBlock.gif)[   webmap.ManageLifetime(pDeleteSet);](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[   ](/Images/OutliningIndicators/InBlock.gif)//设置ISet对象
![](/Images/OutliningIndicators/InBlock.gif)feature=fcursor.NextFeature();
![](/Images/OutliningIndicators/InBlock.gif)[   ](/Images/OutliningIndicators/InBlock.gif)while(feature!=null)
![](/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](/Images/OutliningIndicators/ContractedSubBlock.gif)[   ](/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](/Images/OutliningIndicators/InBlock.gif)[    pDeleteSet.Add(feature);](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[    feature ](/Images/OutliningIndicators/InBlock.gif)=fcursor.NextFeature();
![](/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[   }](/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[   ESRI.ArcGIS.Geodatabase.IFeatureEdit fedit;](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[   pDeleteSet.Reset();](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[   fedit ](/Images/OutliningIndicators/InBlock.gif)=pDeleteSet.Next()asESRI.ArcGIS.Geodatabase.IFeatureEdit;
![](/Images/OutliningIndicators/InBlock.gif)[   ](/Images/OutliningIndicators/InBlock.gif)while(fedit!=null)
![](/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](/Images/OutliningIndicators/ContractedSubBlock.gif)[   ](/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](/Images/OutliningIndicators/InBlock.gif)[    fedit.DeleteSet(pDeleteSet);](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[    fedit ](/Images/OutliningIndicators/InBlock.gif)=pDeleteSet.Next()asESRI.ArcGIS.Geodatabase.IFeatureEdit;
![](/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)[   }](/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)
![](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[   Session.Remove(](/Images/OutliningIndicators/InBlock.gif)"layerid");
![](/Images/OutliningIndicators/InBlock.gif)[   Session.Remove(](/Images/OutliningIndicators/InBlock.gif)"selection");
![](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[   webmap.Refresh();](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/InBlock.gif)[   webmap.Dispose();](/Images/OutliningIndicators/InBlock.gif)
![](/Images/OutliningIndicators/ExpandedBlockEnd.gif)[  }](/Images/OutliningIndicators/ExpandedBlockEnd.gif)
![](/Images/OutliningIndicators/None.gif)
![](/Images/OutliningIndicators/None.gif)
![](/Images/OutliningIndicators/None.gif)
[
](/Images/OutliningIndicators/None.gif)

[
](/Images/OutliningIndicators/None.gif)
[
](/Images/OutliningIndicators/None.gif)


[

		](/Images/OutliningIndicators/None.gif)posted on2006-01-18 09:47[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=319424)[收藏](#)
[
	](/Images/OutliningIndicators/None.gif)

[
](/Images/OutliningIndicators/None.gif)
[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






[
    
](/Images/OutliningIndicators/None.gif)
Powered by:
博客园
Copyright © 西西吹雪
[
](/Images/OutliningIndicators/InBlock.gif)