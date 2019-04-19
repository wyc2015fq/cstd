# ae GP制作缓冲区分析 - 毛小亮 - 博客园
# [ae GP制作缓冲区分析](https://www.cnblogs.com/xianerwonder/p/4460000.html)
```csharp;gutter
private void barButtonItem1_ItemClick(object sender, DevExpress.XtraBars.ItemClickEventArgs e)
        {
            // Initialize the geoprocessor. 
            Geoprocessor GP = new Geoprocessor();
            //创建一个tool
            ESRI.ArcGIS.AnalysisTools.Buffer bufferTool = new
              ESRI.ArcGIS.AnalysisTools.Buffer();
           //给与必须属性
            bufferTool.in_features = @"D:\win7我的文档-桌面-收藏夹\Desktop\test\roadsBuffer.shp";
            bufferTool.out_feature_class = @"D:\win7我的文档-桌面-收藏夹\Desktop\test\roads.shp";
            bufferTool.buffer_distance_or_field = "distance";
            
            GP.Execute(bufferTool, null);
            MessageBox.Show("OK");
        }
```
