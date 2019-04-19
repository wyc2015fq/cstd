# arcgis engine 调用arcgis server服务 - 毛小亮 - 博客园
# [arcgis engine 调用arcgis server服务](https://www.cnblogs.com/xianerwonder/p/4341038.html)
首先需要添加两个引用：
using ESRI.ArcGIS.GISClient;
using ESRI.ArcGIS.DataSourcesRaster;
```csharp;gutter
/// <summary>
        /// arcgis engine 调用arcgis server服务
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void 符号是否随比例尺变化ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //获得服务对象名称
            IAGSServerObjectName pServerObjectName = GetMapServer("http://services.arcgisonline.com/ArcGIS/services", "ESRI_Imagery_World_2D", false);
            IName pName = (IName)pServerObjectName;
            //访问地图服务
            IAGSServerObject pServerObject = (IAGSServerObject)pName.Open();
            IMapServer pMapServer = (IMapServer)pServerObject;
            ESRI.ArcGIS.Carto.IMapServerLayer pMapServerLayer = new ESRI.ArcGIS.Carto.MapServerLayerClass();
            //连接地图服务
            pMapServerLayer.ServerConnect(pServerObjectName, pMapServer.DefaultMapName);
            //添加数据图层
            axMapControl1.AddLayer(pMapServerLayer as ILayer);
            axMapControl1.Refresh();
        }
        /// <summary>
        /// 获得地图服务的方法，返回IAGSServerObjectName
        /// </summary>
        /// <param name="pHostOrUrl"></param>
        /// <param name="pServiceName"></param>
        /// <param name="pIsLAN"></param>
        /// <returns></returns>
        public IAGSServerObjectName GetMapServer(string pHostOrUrl, string pServiceName, bool pIsLAN)
        {
            //设置连接属性
            IPropertySet pPropertySet = new PropertySetClass();
            if (pIsLAN)
                pPropertySet.SetProperty("machine", pHostOrUrl);
            else
                pPropertySet.SetProperty("url", pHostOrUrl);
            //打开连接
            IAGSServerConnectionFactory pFactory = new AGSServerConnectionFactory();
            //Type factoryType = Type.GetTypeFromProgID(
            //    "esriGISClient.AGSServerConnectionFactory");
            //IAGSServerConnectionFactory agsFactory = (IAGSServerConnectionFactory)
            //    Activator.CreateInstance(factoryType);
            IAGSServerConnection pConnection = pFactory.Open(pPropertySet, 0);
            //Get the image server.
            IAGSEnumServerObjectName pServerObjectNames = pConnection.ServerObjectNames;
            pServerObjectNames.Reset();
            IAGSServerObjectName ServerObjectName = pServerObjectNames.Next();
            while (ServerObjectName != null)
            {
                if ((ServerObjectName.Name.ToLower() == pServiceName.ToLower()) &&
                    (ServerObjectName.Type == "MapServer"))
                {
                    break;
                }
                ServerObjectName = pServerObjectNames.Next();
            }
            //返回对象
            return ServerObjectName;
        }
```
