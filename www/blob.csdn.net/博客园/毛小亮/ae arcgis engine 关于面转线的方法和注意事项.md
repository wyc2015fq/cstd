# ae arcgis engine 关于面转线的方法和注意事项 - 毛小亮 - 博客园
# [ae arcgis engine 关于面转线的方法和注意事项](https://www.cnblogs.com/xianerwonder/p/4460138.html)
代码很简单，如下
```csharp;gutter
private void barButtonItem1_ItemClick(object sender, DevExpress.XtraBars.ItemClickEventArgs e)
{
// Initialize the geoprocessor.
Geoprocessor GP = new Geoprocessor();
//创建一个tool
FeatureToPolygon featureToPolygon = new FeatureToPolygon();
//给与必须属性
featureToPolygon.in_features = @"D:\win7我的文档-桌面-收藏夹\Desktop\test\roadsBuffer.shp";
featureToPolygon.out_feature_class = @"D:\win7我的文档-桌面-收藏夹\Desktop\test\roads4.shp";
GP.Execute(featureToPolygon, null);
MessageBox.Show("OK");
}
```
需要注意的问题：
1权限问题。我给与的是最高权限：
```csharp;gutter
#region 初始化许可
            esriLicenseStatus licenseStatus = esriLicenseStatus.esriLicenseUnavailable;
            IAoInitialize m_AoInitialize = new AoInitialize();
            licenseStatus = m_AoInitialize.Initialize(esriLicenseProductCode.esriLicenseProductCodeAdvanced);
            //检查扩展模块功能
            licenseStatus = m_AoInitialize.CheckOutExtension(esriLicenseExtensionCode.esriLicenseExtensionCodeRuntimeAdvanced);
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            #endregion
```
2其实上面给了最高权限之后还算不行。关键还看下面的：
在arcmap打开Extension所有项，并且把你的功能在工具箱里跑一遍。嗯，不知道为啥。就可以了
---------------------------------------------------------------------------------------------------------------------------------------------------------------
线转面的时候传参数的时候，命名必须用英文名：
string fromPath = @"D:\win7我的文档-桌面-收藏夹\Desktop\东庄.mdb\JZX";
            string ToPath = @"D:\win7我的文档-桌面-收藏夹\Desktop\东庄.mdb\xxx";
数字是不可以的：
string ToPath = @"D:\win7我的文档-桌面-收藏夹\Desktop\东庄.mdb\2015";
另外覆盖源数据也是不可以的

