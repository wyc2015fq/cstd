# ae 地理坐标与投影坐标转换  [转] - 毛小亮 - 博客园
# [ae 地理坐标与投影坐标转换  [转]](https://www.cnblogs.com/xianerwonder/p/4451152.html)
转载地址：http://blog.163.com/lai_xiao_hui/blog/static/123037324201151443221942/
代码是将WGS84地理坐标转换为WGS84UTM投影坐标，如果要转换为西安80或北京54坐标，则参数是：
北京54坐标：
             地理坐标枚举值为：esriSRGeoCSType.esriSRGeoCS_Beijing1954
             投影坐标枚举值为：esriSRProjCSType.esriSRProjCS_Beijing1954GK_13……
西安80坐标：
            地理坐标枚举值为：esriSRGeoCS3Type.esriSRGeoCS_Xian1980
            投影坐标枚举值为：esriSRProjCS4Type.esriSRProjCS_Xian1980_3_Degree_GK_CM_102E……
public class WGS84UTM
    {
        /// <summary>
        /// 从地理坐标转换到投影坐标
        /// </summary>
        /// <param name="longitude">经度</param>
        /// <param name="latitude">纬度，南半球为负数</param>
        /// <param name="x">X</param>
        /// <param name="y">Y</param>
        /// <param name="zone">带区（1-60，从-180到+180，6度带）</param>
        public static void GeoToPrj(double longitude, double latitude, out double x, out double y, out int zone)
        {
            ISpatialReferenceFactory pSpatialReferenceFactory = new SpatialReferenceEnvironmentClass();
            IGeographicCoordinateSystem pGeoCoordSys = pSpatialReferenceFactory.CreateGeographicCoordinateSystem((int)esriSRGeoCSType.**esriSRGeoCS_WGS1984**);
            int startprjnum = 32601;
            if (latitude < 0) startprjnum = 32701;
            zone = (int)Math.Round(((longitude + 3) / 6)) + 30;
            int prjnum = startprjnum + zone - 1;
            IProjectedCoordinateSystem pPrjCoordSys = pSpatialReferenceFactory.CreateProjectedCoordinateSystem(**prjnum**);
            IPoint pt = new PointClass();
            pt.PutCoords(longitude, latitude);
            IGeometry geo = (IGeometry)pt;
            geo.SpatialReference = pGeoCoordSys;
            geo.Project(pPrjCoordSys);
            x = pt.X;
            y = pt.Y;
            if (latitude < 0)
                y = 0 - y;
        }
        /// <summary>
        /// 从投影坐标转换到地理坐标
        /// </summary>
        /// <param name="x">X</param>
        /// <param name="y">Y，南半球为负数</param>
        /// <param name="zone">带区（1-60，从-180到+180，6度带）</param>
        /// <param name="longitude">经度</param>
        /// <param name="latitude">纬度</param>
        public static void PrjToGeo(double x, double y, int zone, out double longitude, out double latitude)
        {
            ISpatialReferenceFactory pSpatialReferenceFactory = new SpatialReferenceEnvironmentClass();
            IGeographicCoordinateSystem pGeoCoordSys = pSpatialReferenceFactory.CreateGeographicCoordinateSystem((int)esriSRGeoCSType.**esriSRGeoCS_WGS1984**);
            int startprjnum = 32601;
            bool South = false;
            if(y < 0) 
            {
                South = true;
                y = 0 - y;
            }
            if (South) startprjnum = 32701;
            int prjnum = startprjnum + zone - 1;
            IProjectedCoordinateSystem pPrjCoordSys = pSpatialReferenceFactory.CreateProjectedCoordinateSystem(**prjnum**);
            IPoint pt = new PointClass();
            pt.PutCoords(x, y);
            IGeometry geo = pt as IGeometry;
            geo.SpatialReference = pPrjCoordSys;
            geo.Project(pGeoCoordSys);
            longitude = pt.X;
            latitude = pt.Y;
            if (South) latitude = 0 - latitude;
        }
    }

