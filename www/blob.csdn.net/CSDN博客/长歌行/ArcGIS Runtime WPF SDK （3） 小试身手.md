# ArcGIS Runtime WPF SDK （3） 小试身手 - 长歌行 - CSDN博客





2017年09月20日 17:34:16[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：457
所属专栏：[Esri技术谈](https://blog.csdn.net/column/details/17488.html)









[本节代码](https://github.com/bentan2013/try-with-arcgis-rt-wpf/tree/%E5%B0%8F%E8%AF%95%E8%BA%AB%E6%89%8B)

在上一节中，了解了，WPF API的各个命名空间的作用，本节内容包括，创建一个地图，加载一个要素图层，并支持点选查询。

效果如下： 
![这里写图片描述](https://img-blog.csdn.net/20170920172550653?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 底图
`var myMap = new Map(Basemap.CreateTopographic());`
### 要素图层

```
// Create Uri for the feature service
Uri featureServiceUri = new Uri(            "http://sampleserver6.arcgisonline.com/arcgis/rest/services/DamageAssessment/FeatureServer/0");
// Initialize feature table using a url to feature server url
var featureTable = new ServiceFeatureTable(featureServiceUri);
// Initialize a new feature layer based on the feature table
_featureLayer = new FeatureLayer(featureTable);
await _featureLayer.LoadAsync();
// Check for the load status. If the layer is loaded then add it to map
if (_featureLayer.LoadStatus == LoadStatus.Loaded)
{
        // Add the feature layer to the map
        myMap.OperationalLayers.Add(_featureLayer);
}
```

### 支持点选

```
// Set the selection color for feature layer
_featureLayer.SelectionColor = Colors.Cyan;
// Set the selection width
_featureLayer.SelectionWidth = 3;
// Add tap event handler for mapview
MyMapView.GeoViewTapped += OnMapViewTapped;

 pivate async void OnMapViewTapped(object sender, GeoViewInputEventArgs e)
 {
     try
     {
         // Define the selection tolerance (half the marker symbol size so that any click on the symbol will select the feature)
         double tolerance = 14;

         // Convert the tolerance to map units
         double mapTolerance = tolerance * MyMapView.UnitsPerPixel;

         // Define the envelope around the tap location for selecting features
         var selectionEnvelope = new Envelope(e.Location.X - mapTolerance, e.Location.Y - mapTolerance, e.Location.X + mapTolerance,
             e.Location.Y + mapTolerance, MyMapView.Map.SpatialReference);

         // Define the query parameters for selecting features
         var queryParams = new QueryParameters();

         // Set the geometry to selection envelope for selection by geometry
         queryParams.Geometry = selectionEnvelope;

         // Select the features based on query parameters defined above
         await _featureLayer.SelectFeaturesAsync(queryParams, Esri.ArcGISRuntime.Mapping.SelectionMode.New);
     }
     catch (Exception ex)
     {
         MessageBox.Show("Sample error", ex.ToString());
     }
 }
```

### 弹出窗

在xaml中加入

```xml
<Grid>
   <esri:MapView x:Name="MyMapView" />
    <Border Name="myPopup" Width="200"
                          Height="100"
                          Margin="0,0,100,200">
        <!--<Path x:Name="myPopupPath">
            <Path.Data>
                <PathGeometry x:Name="myPopupPathGeometry" Figures="{Binding FiguresPopupPathGeometry}"/>
            </Path.Data>
        </Path>-->
        <TextBlock Background="White" Foreground="Blue">Test</TextBlock>
    </Border>
</Grid>
```

在cs中加入相应代码，控制popup位置，以及显示内容

```
bool hasResult = false;
foreach(var r in _queryResult)
{
    hasResult = true;
    Feature f = r as Feature;
    string attribute = "";
    foreach(var a in f.Attributes)
    {
        attribute = attribute + a.ToString() + "\n";
    }

    MapPoint p = f.Geometry as MapPoint;

    Point sp = MyMapView.LocationToScreen(p);
    TextBlock popupText = new TextBlock();
    popupText.Background = Brushes.LightBlue;
    popupText.Foreground = Brushes.Blue;
    popupText.Text = attribute;
    myPopup.Child = popupText;
    myPopup.Width = 200;
    myPopup.Height = 100;
    var b = MyMapView.Margin;

    myPopup.Margin = new Thickness(sp.X, sp.Y, MyMapView.ActualWidth - 200 - sp.X, MyMapView.ActualHeight - 100 - sp.Y);
}

if(!hasResult)
{
    myPopup.Width = myPopup.Height = 0;
}
```




