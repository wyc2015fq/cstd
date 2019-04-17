# Windows Mobile开源地图Guidebee开发方案 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年07月30日 15:32:39[boonya](https://me.csdn.net/boonya)阅读数：1572标签：[windows mobile																[github																[地图																[导航																[开源代码](https://so.csdn.net/so/search/s.do?q=开源代码&t=blog)
个人分类：[C#](https://blog.csdn.net/boonya/article/category/1776321)





   前言：当我第一次接触到Guidebee的时候就被其裸露的’美肌‘（应用UI截图）所深深吸引，这是Windows 平台下落日余烬的斜晖，灿烂而美丽。当我对Windows Mobile 平台地图导航应用展开搜索时，出现了很多地图如openstreetmap、sharpmap等等，但是它们并没能让我感觉有多么高大上，于是Guidebee映入眼帘，而且生动形象地展现在世人面前，我知道这就是我想要的。感谢James
 Shen对开源社区的无私奉献，让我等屌丝顺利逆袭！Guidebee学习地址：[http://blog.csdn.net/mapdigit/article/details/7729883](http://blog.csdn.net/mapdigit/article/details/7729883)

#### 1. 开源地图站点

Guide Bee GitHub 地址 为[https://github.com/guidebee/](https://github.com/guidebee/) 
 下
- 
guidebeemap      Java库源码
- 
guidebeemapdonnet  .Net 库源码
- 
guidebeemaptutorial  教程及示例应用代码
- 
maptiledownloader    离线地图下载应用代码

#### 2.地图平台特性

![](http://www.imobilebbs.com/wordpress/wp-content/uploads/2011/01/featurs.png)


#### 3.Windows Mobile开发

     3.1、在此处下载Guidebee v2.1 ,地址：[http://blog.csdn.net/mapdigit/article/details/7729883](http://blog.csdn.net/mapdigit/article/details/7729883)





     3.2、在Github上面下载guidebeemapdotnet源码，编译【DotNetMapDigitLib】下的示例，生成*.dll文件，如下图：

![](https://img-blog.csdn.net/20140730151403315?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





3.3、将开源代码的GIS部分打包替换原V2.1的GIS引用




注：此处的License去掉，删去v2.1中License验证的内容。另外提醒大家，站点上的DotNetPocketStreet代码有点点问题，按照我上面所说的就可以轻松开始Guidebee之旅。


#### 4.Windows Mobile效果图

![](https://img-blog.csdn.net/20140730152525051?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20140730152602460?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

注：后面一张图是放大一级后的图片。](https://so.csdn.net/so/search/s.do?q=导航&t=blog)](https://so.csdn.net/so/search/s.do?q=地图&t=blog)](https://so.csdn.net/so/search/s.do?q=github&t=blog)](https://so.csdn.net/so/search/s.do?q=windows mobile&t=blog)




