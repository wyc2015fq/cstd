# 百度开放平台-o2o上门解决方案 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年08月10日 13:57:07[boonya](https://me.csdn.net/boonya)阅读数：771








文章来源：[http://lbsyun.baidu.com/index.php?title=open/o2o1](http://lbsyun.baidu.com/index.php?title=open/o2o1)


# O2O与LBS

![](https://img-blog.csdn.net/20160810135203837)


# [](http://lbsyun.baidu.com/skins/MySkin/resources/iframs/o2o.html#section1)

“

    工作太忙，没有时间打扫屋子 

    不想做饭，也不想出去吃 

  ” 


越来越多的“懒人”需求催生了互联网+上门服务的O2O形式蓬勃发展。上门服务O2O，解决了用户在时间和位置上的众多不便，高频高效地连接起人与服务。作为O2O创业者，不仅需要提供良好的线下服务，还需要有一套稳定好用的在线系统提供给用户使用。而O2O基于地理位置提供到家服务，构建一套成熟的系统，其核心技术便是LBS。该版块介绍了上门服务O2O的通用LBS解决方案，希望能帮助O2O开发者快速清晰地构建系统。
通用LBS解决方案
对一个通用的上门O2O服务流程进行分析，每个流程场景中都用到了哪些LBS相关的功能：
![](https://img-blog.csdn.net/20160810135526824)
![](https://img-blog.csdn.net/20160810135537936)
![](https://img-blog.csdn.net/20160810135549936)

|角色|场景|LBS功能|
|----|----|----|
|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/user1.JPG)用户|寻找周边服务，填写地址并进行下单|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/16.png)定位![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/15.png)数据![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/14.png)地图|
|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/user2.JPG)开发者|根据订单的位置特性，调度最合适的服务人员|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/13.png)分析![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/15.png)数据![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/14.png)地图|
|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/user3.JPG)服务人员|规划路线，前往上门服务地点|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/12.png)轨迹追踪![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/11.png)出行![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/14.png)地图|

各个功能对应到地图开放平台的服务：
|功能|端|服务|简介|相关链接|
|----|----|----|----|----|
|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/16.png)定位|App|定位SDK|实现Android设备的定位功能|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/android.png)[查看文档![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)](http://developer.baidu.com/map/index.php?title=android-locsdk)|
|Web App|IP定位API|根据IP返回对应位置信息|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/pc.png)[查看文档![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)](http://developer.baidu.com/map/index.php?title=webapi/ip-api)| |
|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/15.png)数据检索|App|地图SDK|iOS、Android设备的地图功能开发|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/android.png)[查看文档![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)](http://developer.baidu.com/map/index.php?title=androidsdk/guide/retrieval)![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/ios.png)[查看文档![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)](http://developer.baidu.com/map/index.php?title=iossdk/guide/retrieval)|
|Web App|Place API|根据关键词搜索POI|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/pc.png)[查看文档![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)](http://developer.baidu.com/map/index.php?title=webapi/guide/webservice-placeapi)| |
|逆地理编码API|位置描述，周边POI推荐|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/pc.png)[查看文档![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)](http://developer.baidu.com/map/index.php?title=webapi/guide/webservice-geocoding)| | |
|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/14.png)地图|App|地图SDK||![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/android.png)[查看文档![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)](http://developer.baidu.com/map/index.php?title=androidsdk/guide/basicmap)![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/ios.png)[查看文档![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)](http://developer.baidu.com/map/index.php?title=iossdk/guide/basicmap)|
|Web App|JavaScript API|Web端的地图功能开发|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/pc.png)[查看文档![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)](http://developer.baidu.com/map/index.php?title=jspopular)| |
|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/11.png)路线规划|App|地图SDK||![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/android.png)[查看文档![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)](http://developer.baidu.com/map/index.php?title=androidsdk)![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/ios.png)[查看文档![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)](http://developer.baidu.com/map/index.php?title=iossdk)|
|App|导航SDK|iOS、Android设备的实时导航功能开发|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/android.png)[查看文档![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)](http://developer.baidu.com/map/index.php?title=android-navsdk)![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/ios.png)[查看文档![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)](http://developer.baidu.com/map/index.php?title=ios-navsdk)| |
|Web App|路线交通API|公交、驾车、步行方案的查询检索|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/pc.png)[查看文档![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)](http://developer.baidu.com/map/index.php?title=webapi/direction-api)| |
|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/12.png)轨迹追踪|App|鹰眼轨迹Android/iOS SDK|实现手机端的轨迹追踪与查询|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/android.png)[查看文档![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)](http://developer.baidu.com/map/index.php?title=android-yingyan)![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/ios.png)[查看文档![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)](http://developer.baidu.com/map/index.php?title=ios-yingyan)|
|Web|鹰眼轨迹Web API|Web端实时与历史轨迹查询|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/pc.png)[查看文档![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)](http://developer.baidu.com/map/index.php?title=yingyan)| |
|智能硬件|MTK 2502芯片|用于智能硬件实时轨迹追踪|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/pc.png)[查看文档![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)](http://developer.baidu.com/map/index.php?title=yingyan)| |
|OBD设备|OBD设备|车载OBD设备，用于获取车辆实时轨迹和车况|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/mail.png)联系我们![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)| |
|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/15.png)数据云检索|App|周边雷达|查找周边人、服务、订单|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/android.png)[查看文档![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)](http://developer.baidu.com/map/index.php?title=androidsdk/guide/radar)![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/ios.png)[查看文档![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)](http://developer.baidu.com/map/index.php?title=iossdk/guide/radar)|
|Web App|云检索API|检索LBS云中存储的位置数据|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/pc.png)[查看文档![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)](http://developer.baidu.com/map/index.php?title=lbscloud)| |
|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/11.png)距离计算|服务器端|直线距离计算|计算坐标点之间的直线空间距离|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/android.png)[查看文档![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)](http://developer.baidu.com/map/index.php?title=androidsdk/guide/tool)![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/ios.png)[查看文档![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)](http://developer.baidu.com/map/index.php?title=iossdk/guide/tool)|
|行驶距离计算|计算坐标点间实际行驶距离|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/mail.png)联系我们![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)| | |

深入思考
随着业务的不断增长，开发者需要思考更多的问题来提升产品竞争力，如何通过LBS技术来优化用户体验、提升服务效率，是开发者最为关心的问题之一，对于上门服务O2O开发者，最常见的问题有如下三个：


1、如何帮助服务人员进行高效的路线规划？ 

2、如何进行对订单的监管？

3、如何呈现业务分布情况，并进行地域分析调配？


|问题|服务|简介|相关链接|
|----|----|----|----|
|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/11.png)高效规划路线|多点路线规划|多个目的地/途径点的先后顺序计算|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/mail.png)联系我们![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)|
|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/12.png)管理服务人员|区域计算|计算服务人员点和服务区域面的空间关系|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/mail.png)联系我们![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)|
|鹰眼服务|轨迹管理服务|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/pc.png)[查看文档![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)](http://developer.baidu.com/map/index.php?title=yingyan)| |
|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/14.png)业务分布可视化|云麻点服务|高性能地渲染出业务的分布情况|![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/pc.png)[查看文档![](http://lbsyun.baidu.com/skins/MySkin/resources/img/btn-see.gif)](http://developer.baidu.com/map/jsdemo.htm#g0_4)|

合作案例
![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/edaixi.png)![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/aixianfeng.png)![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/diandao.png)![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/baiduwaimai.jpg)![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/yimixian.jpg)![](http://lbsyun.baidu.com/skins/MySkin/resources/img/o2o/xiaoma.jpg)




