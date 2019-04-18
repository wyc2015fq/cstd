# AE 坐标转换_  常见细节 - weixin_33985507的博客 - CSDN博客
2012年05月04日 11:47:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
[http://hi.baidu.com/%D6%DC%B4%BA521/blog/item/16eb3902a54fcb6503088185.html](http://hi.baidu.com/%D6%DC%B4%BA521/blog/item/16eb3902a54fcb6503088185.html)
```
/// <summary>
        /// 从经纬坐标串生成 投影坐标下的线
        /// </summary>
        /// <param name="gcsStr"></param>
        /// <returns></returns>
        public static IPolyline GetLinePCSFromGCSStr(string gcsStr)
        {
            ISpatialReferenceFactory spatialReferenceFactory = new SpatialReferenceEnvironmentClass();
            IProjectedCoordinateSystem pcsSys = spatialReferenceFactory.CreateProjectedCoordinateSystem((int)esriSRProjCS4Type.esriSRProjCS_Xian1980_3_Degree_GK_Zone_39);
            IGeographicCoordinateSystem gcsSys = spatialReferenceFactory.CreateGeographicCoordinateSystem((int)esriSRGeoCS3Type.esriSRGeoCS_Xian1980);
            string[] ar_pts = gcsStr.Split(';');
            object tempType = Type.Missing;
            IPointCollection pPtCollection = new PathClass();
            for (int i = 0; i <= ar_pts.Length - 1; i++)
            {
                IPoint pt = new PointClass();
                pt.X = Convert.ToDouble(ar_pts[i].Split(',')[0]);
                pt.Y = Convert.ToDouble(ar_pts[i].Split(',')[1]);
                IGeometry geoGcs = (IGeometry)pt;
                geoGcs.SpatialReference = gcsSys;
                geoGcs.Project(pcsSys);
                IPoint ptAfter = geoGcs as IPoint;
                pPtCollection.AddPoint(ptAfter, ref tempType, ref tempType);
            }
            IGeometryCollection pGeoCollection = new PolylineClass();
            pGeoCollection.AddGeometry(pPtCollection as IGeometry, ref tempType, ref tempType);
            return pGeoCollection as IPolyline;
        }
```
备注：有坐标,无“度-带”，要确定度带。
解决方式：逐个试。某个“度-带”投影后 基本重合,即是。
常见细节：
(1) PCS: GK --> Xian80 3D  39Z 的featureClass, 使用3DAnalyst ceate TIN. TIN的坐标系统会变成 User_Defined Tranverse Mervator. 
              对比显示featureclass的spatialReference和 TIN的，有此差异.
              显示,两者叠合
