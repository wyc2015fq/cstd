# Java Web GIS 地理信息系统开发 - youfangyuan - CSDN博客
置顶2016年01月24日 20:03:24[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：8229
Java google地图
![](https://img-blog.csdn.net/20160124200108560?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Java Google卫星图
![](https://img-blog.csdn.net/20160124200141289?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Java Google地形图
![](https://img-blog.csdn.net/20160124200154774?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Java ArcGIS 地图
![](https://img-blog.csdn.net/20160124200209274?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Java 天地图
![](https://img-blog.csdn.net/20160124200216843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
TileSource[] tileSourceArray = { new GoogleTileSource(), new GoogleSatelliteMixTileSource(),
new GoogleTopographicTileSource(), new ArcGISTileSource(), new TianMapTileSource(),
new TianMapSatelliteTileSource(), new TianMapTopographicTileSource(), new AMapTileSource() };
mapComponent.setTileSource(new GoogleTileSource());
