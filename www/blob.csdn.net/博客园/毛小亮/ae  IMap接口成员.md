# ae   IMap接口成员 - 毛小亮 - 博客园
# [ae   IMap接口成员](https://www.cnblogs.com/xianerwonder/p/4310888.html)
使用IMap接口显示各种数据源的数据。
IMap接口的成员
ActiveGraphicsLayer：活动图形图层，如果没有将创建一个基本memory graphics layer。
AddLayer：向地图中添加单个图层。
AddLayers：向地图中添加多个图层。
AddMapSurround：向地图中添加map surround。
AnnotationEngine：地图使用的注记（标签）引擎。
AreaOfInterest：地图的感兴趣区域。
Barriers：barriers列表和标签权重。
BasicGraphicsLayer：基本图形层。
ClearLayers：从地图中移除所有图层。
ClearMapSurrounds：从地图中移除所有旁注。
ClearSelection：清除地图选择。
ClipBorder：ClipGeometry周围一个可选边界。
ClipGeometry：地图图层中一个要剪切的形状。
ComputeDistance：计算地图上两个点的距离并返回结果。
CreateMapSurround：创建并初始化一个map surround。从样式集中指定一个可选样式。
DelayDrawing：暂停绘制。
DelayEvents：将操作按批组合来减少通知。
DeletLayer：从map中删除图层。
DeleteMapSurround：从地图中删除map surround。
Description：地图描述。
DistanceUnits：地图的距离单位。
Expanded：指出地图是否可以扩张。
FeatureSelection：地图中选择的要素。
GetPageSize：获得地图的页面大小。
IsFramed：指出地图是否在一个框架内而不是在整个窗口中绘制。
Layer：给定序号的layer。
LayerCount：地图中图层的数目。
Layers：以uid表明类型的地图图层。如果recursive是true，将以layers的形式返回layers。
MapScale：数字形式的地图比例尺。
MapSurroundCount：与地图相关的旁注数量。
MapUnits：地图单位。
MoveLayer：将一个图层移到其他位置。
Name：地图名称。
RecalcFullExtent：重新计算全幅范围。
ReferenceScale：分数形式的地图参考比例尺。
SelectByShape:用一个几何形状和选择环境（可选）在地图中选择要素。
SelectFeature：选择一个要素。
SelectionCount：选择的要素的数目。
SetPageSize：设置地图（可选）的页面大小。
SpatialReference：地图的空间参考。
SpatialReferenceLocked：指出是否允许改变空间参考。
UseSymbolLevels:指出是否使用symbol levels绘制地图。
AnnotationEngine：map使用的annotion（label）引擎。
AreaOfInterest：地图的感兴趣区域。
类Map实现接口IMap。
 每个地图文件包含至少一个Map对象。但每次只有一个Map能够获得焦点，这个Map称作focus map（焦点地图）。使用IMxDocument可以访问可以访问文档中的所有地图对象；IMxDocument::FocusMap返回当前具有焦点的地图引用。IMxDocument：Maps返回指向所有地图对象的引用（IMaps）。一个地图文档可以包含任意数目的Map对象--数据视图中只显示焦点地图。
 地图对象，管理一系列图层对象。每个图层有一个空间参考。一个空间参考定义一个分辨率（1/精度）和一个坐标系统。地图坐标系统自动设置为第一个图层的坐标系统，根据所有的图层范围计算分辨率。
几种常用属性和方法：
IMap.AddLayer Method(向map中添加layer)
[Visual Basic.NET] Public Sub AddLayer(ByVal Layer as ILayer)
[C#]        public void AddLayer(Ilayer Layer)
 AddLayer方法向地图中添加图层。使用LayerCount属性获得地图中图层的总数。如果map没有设置坐标系统，AddLayer自动设置地图的SpatialReference属性。当地图的空间参考属性设置后，地图的MapUnits和DistanceUnits属性也就设置了。AddLayer同时也设置layer的空间参考。
IMap.DeleteLayer Method(从Map中删除图层）
[Visual Basic.NET] Public Sub DeleteLayer(ByVal Layer as ILayer)
[C#]        public void DeleteLayer(ILayer Layer)
 如果一个图层在TOC窗口中被选中（IMxDocument::SelectedLayer)，可以将其传递到DeleteLayer方法中。当删除一个引用要素类的图层时，如FeatureLayer，执行IDataLayer2::Disconnect能清楚对要素类的引用。
IMap.FeatureSelection Property（IMap.FeatureSelection属性）
[Visual Basic.NET] Public Property FeatureSelection as Iselection
[C#]        public ISelection FeatureSelection{get;set;}
 ArcMap有两个不同的选择集，feature selection和element selection。两种不同的对象表示了这些选择并且都实现了ISelection接口。feature selection对象还实现了IEnumFeature接口，element selection还实现了IEnumElement。ISelection接口用于clipboard类型的操作，IEnum接口针对集合中的项目。
 当你从IMap中访问FeatureSelection时，返回一个ISelection。当你通过IActiveView::Selection选择时，既可以是元素选择，也可以是要素选择，取决于哪一个处于活动状态--一次只能有一个处于活动状态。使用要素选择确保引用到正确的选择。
 IEnumFeature在所有要素图层上工作。每个FeatureLayer有一个ISelectionSet和IEnumFeature。IEnumFeature工作在所有FeatureLayer上，不能用它遍历某一个图层的要素。
 注意，只有shape字段能确保用于选择。IMap::FeatureSelection属性用于绘制地图选择，而不是访问要素属性。在shapefiles、coverage和geodatabases中要注意。如果想遍历地图选择执行一个操作，最好访问每个图层的选择集而不是整个地图的。
IMap.Layer Property（IMap.Layer属性）
[Visual Basic.NET] Public Function get_Layer(ByVal Index As Integer) As ILayer
[C#]        Public ILayer get_Layer(int Index);
 Maps是由各种数据源中的无数图层组成。使用Layer属性来访问地图的特定图层。图层集从0开始。使用Layer属性和LayerCount属性在地图中遍历各个图层。
 Layer属性返回一个ILayer引用。常用if(player is IFeatureLayer)进行判断。例如，不要认为地图中的所有图层都是FeatureLayers。实现ILayer和IDataLayer接口的图层是有效图层。
 每个地图也有一个basic graphics layer，但并不包含在这个属性返回的图层集中。通过BasicGraphicsLayer属性可以获得对这个图层的引用。
IMap.SelectByShape Method
  通过几何图形和选择环境（可选）选择要素
[Visual Basic.NET] Public Sub SelectByShape(ByVal Shape As IGeometry,ByVal env As ISelectionEnviroment,Byval justOne as Boolean)
[C#]        public void SelectByShape(IGeometry Shape,ISelectionEnvironment env,bool justOne);
 SelectByShape在所有IFeatureLayer中搜索与输入图形相交的要素，只有IFeatureLayer::Selectable属性设为TRUE时才可搜索该图层。
 ISelectionEnvironment决定选择结果。例如，创建一个新的选择或向已存在的选择集中添加要素。通过IMxApplication::SelectionEnviroment使用应用程序的选择环境。
 justOne变量控制找到一个要素后是否停止搜索。
 搜索完成后调用IActiveViewEvents::SelectionChanged通知选择已改变。

