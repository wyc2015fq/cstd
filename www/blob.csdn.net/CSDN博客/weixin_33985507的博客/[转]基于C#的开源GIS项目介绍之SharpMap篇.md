# [转]基于C#的开源GIS项目介绍之SharpMap篇 - weixin_33985507的博客 - CSDN博客
2015年12月31日 20:52:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：14
我是一个刚毕业的GIS本科毕业生，目前在杭州从事GIS软件应用开发。在项目开发中总感觉自己的编程水平还不够，于是想找些开源GIS小项目来研究研究，借以提高自己的编程能力和项目开发能力。在网上搜了一下“GIS开源”发现还不少，下面是一个介绍GIS开源项目的链接：
[http://www.yuanma.org/data/2008/0526/article_3048.htm](http://www.yuanma.org/data/2008/0526/article_3048.htm)
里面介绍了基于各种编程语言的GIS开源项目，并列出了各自的特点和官网链接。
由于在学校时候学的一直都是C#和Visual Studio开发平台，虽然不敢说自己精通.NET和C#，但好歹也有2年的C#开发经验了，所以我毫不犹豫的选择了SharpMap作为我的第一个开源GIS研究对象。
我也是刚学，所以不敢发表过多评论，也没有什么心得。
下面就简单介绍一下两个基于C#的开源GIS项目和一个C#版本的OpenGL，即
1.SharpMap
官方网站：[http://sharpmap.codeplex.com/](http://sharpmap.codeplex.com/)
2.WorldWind
官方网站：[http://worldwind.arc.nasa.gov/index.html](http://worldwind.arc.nasa.gov/index.html)
源码下载及学习网站：[http://worldwindcentral.com/wiki/Main_page](http://worldwindcentral.com/wiki/Main_page)
3.CsGL
官方网站：[http://csgl.sourceforge.net/](http://csgl.sourceforge.net/)
### SharpMap
#### 一、简介
我就不多说了，百度百科就讲的很好。现概括如下：
SharpMap是一个基于.net 2.0使用C#开发的Map渲染类库，可以渲染各类GIS数据，可应用于桌面和Web程序。
目前支持B/S及C/S两种方式的DLL调用，支持地图渲染效果。稳定版本为0.9，最新版本为2.0，代码行数近10000行。
官方网站：[http://sharpmap.codeplex.com/](http://sharpmap.codeplex.com/)
它的优点有：
1、占用资源较少，响应比较快。在对于项目中如果只需要简单的地图功能的话，是一个比 较好的选择 。
2、它是基于.NET 2.0环境下开发的，对于.NET环境支持较好。
3、使用简单，只要在.NET项目中引用响应的dll文件即可，没有复杂的安装步骤。
支持的数据格式和功能：
PostGreSQL/PostGIS，ESRI Shapefile
支持WMS layers
支持ECW 和 JPEG2000 栅格数据格式
Windows Forms 控件，可以移动和缩放
通过HttpHandler支持ASP.net程序
点、线、多边形、多点、多线和多多边形等几何类型
几何集合（GeometryCollections）等OpenGIS Simple Features Specification
可通过Data Providers（增加数据类型支持）、Layer Types（增加层类型）和Geometry Types等扩展
图形使用GDI+渲染，支持anti-aliased等  
### 二、截图
为了做到有图有真相，并且我们从小就喜欢看图，一张图抵过千言万语，所以我还是贴几张图吧
![](https://images2015.cnblogs.com/blog/494924/201512/494924-20151231204821370-916888836.gif)
![](https://images2015.cnblogs.com/blog/494924/201512/494924-20151231204830557-783530209.gif)
下面是我自己做的一个Demo，很简单就读了一下shapefile，不过这只是一个开始。
![](https://images2015.cnblogs.com/blog/494924/201512/494924-20151231204854604-155928588.gif)
更多图片更多了解请看官网 [http://sharpmap.codeplex.com/](http://sharpmap.codeplex.com/)
### 三、学习资源
1.最好的资源在官网
[http://sharpmap.codeplex.com/](http://sharpmap.codeplex.com/)
2.Sharpmap介绍及源码分析
[http://www.cnblogs.com/maweifeng/archive/2006/02/24/337181.html](http://www.cnblogs.com/maweifeng/archive/2006/02/24/337181.html)
3.SharpMap深度分析
[http://www.cnblogs.com/maweifeng/archive/2006/03/06/344082.html#1865196](http://www.cnblogs.com/maweifeng/archive/2006/03/06/344082.html#1865196)
4.基于.NET 2.0的GIS开源项目SharpMap分析手记
[http://blog.csdn.net/suen/article/details/1420455](http://blog.csdn.net/suen/article/details/1420455)
这是我第一次在CSDN上写博文，一直都不敢写，主要是水平还不高。不过现在觉得无所谓了，博文不仅仅是给别人看的，还可以作为自己学习的总结和记录。
未完待续！
参考链接：
官网：http://sharpmap.codeplex.com/SourceControl/latest
[基于C#的开源GIS项目介绍之SharpMap篇](http://blog.csdn.net/ewyetc/article/details/6665553)
## [SharpMap介绍及源码分析](http://www.cnblogs.com/maweifeng/archive/2006/02/24/337181.html)
资源下载：
[http://yun.baidu.com/s/1VAL5s](http://yun.baidu.com/s/1VAL5s)
