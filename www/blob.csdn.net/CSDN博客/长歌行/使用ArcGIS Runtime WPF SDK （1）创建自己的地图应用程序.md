# 使用ArcGIS Runtime WPF SDK （1）创建自己的地图应用程序 - 长歌行 - CSDN博客





2017年09月15日 14:06:35[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：848
所属专栏：[Esri技术谈](https://blog.csdn.net/column/details/17488.html)









随着ArcGIS Runtime 100.x的发布，可见Runtime系列产品会在Esri的产品体系中扮演越来越重要的角色。本篇主要是编译和编撰官网相关内容。ArcGIS Earth和Drone2Map的推出，使得Runtime系列越发闪耀。本篇主要是编译和编撰官网相关内容。作为ArcGIS Runtime SDK系列开发的第一篇。（但是由于博主的.Net开发水平有限，估计会有很多谬误之处，请见谅）

## 起步

这部分的内容，最好参考[官方文档}([https://developers.arcgis.com/net/latest/wpf/guide/guide-home.htm](https://developers.arcgis.com/net/latest/wpf/guide/guide-home.htm))，会更加具体。

### 安装SDK

ArcGIS Runtime SDK for .NET 包含了developing apps for Windows Presentation Foundation (WPF), Universal Windows Platform (UWP), Xamarin.Android, Xamarin.iOS, and Xamarin Forms的API。

最方便的使用方式是，下载安装Visual Studio的扩展，引用本地或者在线的NuGet包。

**操作系统要求**

*系统版本：*

 Windows 7 SP1 及以上。

*Net Framework版本*
**4.5.2**及以上，越新越好。

*支持的IDE*

Visual Studio 2017 

Visual Studio 2015 Update 3 

Visual Studio Express 2015 for Windows Desktop Update 3 

Visual Studio 2013 Update 5 

Visual Studio Express 2013 for Windows Desktop Update 5 

Visual Studio 2012 Update 4 

Visual Studio Express 2012 for Windows Desktop Update 4
这一章节的开发，我使用的IDE是VS2013，在创建工程的时候，是看不到4.5.2选项的，如下图所示： 
![](https://i.stack.imgur.com/Xsm0x.png)

这个时候，你就需要安装[Microsoft .NET Framework 4.5.2 Developer Pack](http://www.microsoft.com/en-us/download/details.aspx?id=42637)

具体参考[stackoverflow](https://stackoverflow.com/questions/23617900/how-to-select-net-4-5-2-as-a-target-framework-in-visual-studio)
安装完毕，重启VS2013就能看到ArcGIS Runtime WPF 的application了。如下图所示： 
![这里写图片描述](https://img-blog.csdn.net/20170917112631595?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 添加ArcGIS Runtime SDK的引用

只需要添加ArcGIS Runtime API NuGet package并在你的XAML中添加所需的XML命名空间就可以了。这部分的内容在上面下面都会更详细的描述。

### 创建一个WPF的应用程序

使用上图所示的方式就可以创建出一个简易的应用程序了，如下图所示： 
![这里写图片描述](https://img-blog.csdn.net/20170917113226865?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后我们分析下里面的代码：

打开MainWindow.xaml文件，会发现，较之于一般的app，有这么一行： 
`xmlns:esri="http://schemas.esri.com/arcgis/runtime/2013"`

我们可以在xaml中穿件一个MapView，然后和程序中的map绑定一下。 

MainWindow.xaml中有如下代码， 

```
<esri:MapView Map="{Binding Map, Source={StaticResource MapViewModel}}" />
```


MapViewModel.cs中有如下代码：
```
private Map _map = new Map(Basemap.CreateStreets());

        /// <summary>
        /// Gets or sets the map
        /// </summary>
        public Map Map
        {
            get { return _map; }
            set { _map = value; OnPropertyChanged(); }
        }
```

就这样，一个简单的App就创建成功了。后面随着博主的进一步学习，会带来更多精彩内容。

本节代码见[bentan2013/ArcGIS-Runtime-WPF-SDK-Example/tree/init_map](https://github.com/bentan2013/ArcGIS-Runtime-WPF-SDK-Example/tree/init_map)








