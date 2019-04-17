# 百度车联网API - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年05月20日 10:20:19[boonya](https://me.csdn.net/boonya)阅读数：3040








API参考地址：[http://lbsyun.baidu.com/index.php?title=car](http://lbsyun.baidu.com/index.php?title=car)









车联网API服务的各种能力后续将合并到Web API服务中。自2015年12月31日起，车联网API服务将不再向新用户提供服务支持。已有用户服务暂不受影响，请已使用了各种车联网API服务的开发者，尽快升级服务到Web API中使用。

    百度地图车联网API是一套适用车载终端应用的开发接口，您可以在C#、C++、Java等应用程序中使用该服务，通过发起HTTP请求方式调用百度地图车联网服务，返回检索后的JSON或XML数据。

    该套API面对普通开发者（如个人、科研机构等）免费开放，商业合作请[发送邮件](mailto:?subject=**%E5%85%AC%E5%8F%B8/%E4%B8%AA%E4%BA%BA%E7%94%B3%E8%AF%B7%E8%BD%A6%E8%81%94%E7%BD%91%E8%BF%9B%E4%B8%80%E6%AD%A5%E5%90%88%E4%BD%9C&body=%E8%AF%B7%E6%82%A8%E5%A1%AB%E5%86%99%E5%A6%82%E4%B8%8B%E9%A1%B9%E7%9B%AE%E7%94%B3%E8%AF%B7%E4%BF%A1%E6%81%AF%EF%BC%9A%0A%E4%BC%81%E4%B8%9A%E5%90%8D%E7%A7%B0/%E5%9B%A2%E9%98%9F%E4%BF%A1%E6%81%AF%EF%BC%9A%0A%E9%A1%B9%E7%9B%AE%E5%90%8D%E7%A7%B0/%E7%AE%80%E4%BB%8B%EF%BC%9A%0A%E7%94%A8%E9%80%94%EF%BC%88%E5%95%86%E7%94%A8/%E8%87%AA%E7%94%A8%EF%BC%89:%E6%B6%89%E5%8F%8A%E8%A3%85%E8%BD%BD%E7%9A%84%E8%BD%A6%E5%9E%8B%E5%8F%8A%E6%95%B0%E9%87%8F%EF%BC%9A%0A%0A%E8%AE%BE%E5%A4%87%E7%9A%84%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F%E5%8F%8A%E7%A1%AC%E4%BB%B6%E5%8F%82%E6%95%B0%EF%BC%9A%0A%E7%94%B3%E8%AF%B7%E9%A1%B9%EF%BC%88%E5%8F%91%E9%80%81%E8%87%B3%E8%BD%A6%E5%8A%9F%E8%83%BD/%E5%8D%87%E7%BA%A7Key%E6%AC%A1%E6%95%B0%EF%BC%89%EF%BC%9A%0A%E6%82%A8%E7%94%B3%E8%AF%B7%E7%9A%84key%EF%BC%9A%0A%0A%E8%AF%B7%E6%8F%90%E4%BE%9B%E5%A6%82%E4%B8%8B%E4%BF%A1%E6%81%AF%EF%BC%8C%E4%BE%BF%E4%BA%8E%E6%88%91%E4%BB%AC%E4%B8%8E%E6%82%A8%E8%81%94%E7%B3%BB%EF%BC%8C%E9%9D%9E%E5%B8%B8%E6%84%9F%E8%B0%A2%E3%80%82%0A%E7%99%BE%E5%BA%A6%E8%B4%A6%E5%8F%B7%EF%BC%9A%0A%E7%9C%9F%E5%AE%9E%E5%A7%93%E5%90%8D%EF%BC%9A%0A%E5%9C%B0%E5%9D%80%EF%BC%9A%0A%E7%94%B5%E8%AF%9D/%E6%89%8B%E6%9C%BA%EF%BC%9A%0A%E5%B8%B8%E7%94%A8%E9%82%AE%E7%AE%B1%EF%BC%9A%0A%E6%89%80%E5%9C%A8%E5%9C%B0%E5%8C%BA%EF%BC%9A%0A%0A%E5%BE%AE%E5%8D%9A/%E7%99%BE%E5%BA%A6HI/QQ/%E5%BE%AE%E4%BF%A1/MSN%EF%BC%88%E8%87%B3%E5%B0%91%E5%A1%AB%E5%86%99%E4%B8%80%E9%A1%B9%EF%BC%89)。使用本套API，您需先申请密钥(key)，通过在线方式调用。每个Key限制5000次/天。若需升级访问次数，请[发送邮件](mailto:?subject=**%E5%85%AC%E5%8F%B8/%E4%B8%AA%E4%BA%BA%E7%94%B3%E8%AF%B7%E8%BD%A6%E8%81%94%E7%BD%91%E8%BF%9B%E4%B8%80%E6%AD%A5%E5%90%88%E4%BD%9C&body=%E8%AF%B7%E6%82%A8%E5%A1%AB%E5%86%99%E5%A6%82%E4%B8%8B%E9%A1%B9%E7%9B%AE%E7%94%B3%E8%AF%B7%E4%BF%A1%E6%81%AF%EF%BC%9A%0A%E4%BC%81%E4%B8%9A%E5%90%8D%E7%A7%B0/%E5%9B%A2%E9%98%9F%E4%BF%A1%E6%81%AF%EF%BC%9A%0A%E9%A1%B9%E7%9B%AE%E5%90%8D%E7%A7%B0/%E7%AE%80%E4%BB%8B%EF%BC%9A%0A%E7%94%A8%E9%80%94%EF%BC%88%E5%95%86%E7%94%A8/%E8%87%AA%E7%94%A8%EF%BC%89:%E6%B6%89%E5%8F%8A%E8%A3%85%E8%BD%BD%E7%9A%84%E8%BD%A6%E5%9E%8B%E5%8F%8A%E6%95%B0%E9%87%8F%EF%BC%9A%0A%0A%E8%AE%BE%E5%A4%87%E7%9A%84%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F%E5%8F%8A%E7%A1%AC%E4%BB%B6%E5%8F%82%E6%95%B0%EF%BC%9A%0A%E7%94%B3%E8%AF%B7%E9%A1%B9%EF%BC%88%E5%8F%91%E9%80%81%E8%87%B3%E8%BD%A6%E5%8A%9F%E8%83%BD/%E5%8D%87%E7%BA%A7Key%E6%AC%A1%E6%95%B0%EF%BC%89%EF%BC%9A%0A%E6%82%A8%E7%94%B3%E8%AF%B7%E7%9A%84key%EF%BC%9A%0A%0A%E8%AF%B7%E6%8F%90%E4%BE%9B%E5%A6%82%E4%B8%8B%E4%BF%A1%E6%81%AF%EF%BC%8C%E4%BE%BF%E4%BA%8E%E6%88%91%E4%BB%AC%E4%B8%8E%E6%82%A8%E8%81%94%E7%B3%BB%EF%BC%8C%E9%9D%9E%E5%B8%B8%E6%84%9F%E8%B0%A2%E3%80%82%0A%E7%99%BE%E5%BA%A6%E8%B4%A6%E5%8F%B7%EF%BC%9A%0A%E7%9C%9F%E5%AE%9E%E5%A7%93%E5%90%8D%EF%BC%9A%0A%E5%9C%B0%E5%9D%80%EF%BC%9A%0A%E7%94%B5%E8%AF%9D/%E6%89%8B%E6%9C%BA%EF%BC%9A%0A%E5%B8%B8%E7%94%A8%E9%82%AE%E7%AE%B1%EF%BC%9A%0A%E6%89%80%E5%9C%A8%E5%9C%B0%E5%8C%BA%EF%BC%9A%0A%0A%E5%BE%AE%E5%8D%9A/%E7%99%BE%E5%BA%A6HI/QQ/%E5%BE%AE%E4%BF%A1/MSN%EF%BC%88%E8%87%B3%E5%B0%91%E5%A1%AB%E5%86%99%E4%B8%80%E9%A1%B9%EF%BC%89)。

    在您使用百度地图车联网API之前，请先阅读百度地图API[使用条款。](http://developer.baidu.com/map/index.php?title=open/law)








发送到车

（需[邮件申请](mailto:?subject=**%E5%85%AC%E5%8F%B8/%E4%B8%AA%E4%BA%BA%E7%94%B3%E8%AF%B7%E8%BD%A6%E8%81%94%E7%BD%91%E8%BF%9B%E4%B8%80%E6%AD%A5%E5%90%88%E4%BD%9C&body=%E8%AF%B7%E6%82%A8%E5%A1%AB%E5%86%99%E5%A6%82%E4%B8%8B%E9%A1%B9%E7%9B%AE%E7%94%B3%E8%AF%B7%E4%BF%A1%E6%81%AF%EF%BC%9A%0A%E4%BC%81%E4%B8%9A%E5%90%8D%E7%A7%B0/%E5%9B%A2%E9%98%9F%E4%BF%A1%E6%81%AF%EF%BC%9A%0A%E9%A1%B9%E7%9B%AE%E5%90%8D%E7%A7%B0/%E7%AE%80%E4%BB%8B%EF%BC%9A%0A%E7%94%A8%E9%80%94%EF%BC%88%E5%95%86%E7%94%A8/%E8%87%AA%E7%94%A8%EF%BC%89:%E6%B6%89%E5%8F%8A%E8%A3%85%E8%BD%BD%E7%9A%84%E8%BD%A6%E5%9E%8B%E5%8F%8A%E6%95%B0%E9%87%8F%EF%BC%9A%0A%0A%E8%AE%BE%E5%A4%87%E7%9A%84%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F%E5%8F%8A%E7%A1%AC%E4%BB%B6%E5%8F%82%E6%95%B0%EF%BC%9A%0A%E7%94%B3%E8%AF%B7%E9%A1%B9%EF%BC%88%E5%8F%91%E9%80%81%E8%87%B3%E8%BD%A6%E5%8A%9F%E8%83%BD/%E5%8D%87%E7%BA%A7Key%E6%AC%A1%E6%95%B0%EF%BC%89%EF%BC%9A%0A%E6%82%A8%E7%94%B3%E8%AF%B7%E7%9A%84key%EF%BC%9A%0A%0A%E8%AF%B7%E6%8F%90%E4%BE%9B%E5%A6%82%E4%B8%8B%E4%BF%A1%E6%81%AF%EF%BC%8C%E4%BE%BF%E4%BA%8E%E6%88%91%E4%BB%AC%E4%B8%8E%E6%82%A8%E8%81%94%E7%B3%BB%EF%BC%8C%E9%9D%9E%E5%B8%B8%E6%84%9F%E8%B0%A2%E3%80%82%0A%E7%99%BE%E5%BA%A6%E8%B4%A6%E5%8F%B7%EF%BC%9A%0A%E7%9C%9F%E5%AE%9E%E5%A7%93%E5%90%8D%EF%BC%9A%0A%E5%9C%B0%E5%9D%80%EF%BC%9A%0A%E7%94%B5%E8%AF%9D/%E6%89%8B%E6%9C%BA%EF%BC%9A%0A%E5%B8%B8%E7%94%A8%E9%82%AE%E7%AE%B1%EF%BC%9A%0A%E6%89%80%E5%9C%A8%E5%9C%B0%E5%8C%BA%EF%BC%9A%0A%0A%E5%BE%AE%E5%8D%9A/%E7%99%BE%E5%BA%A6HI/QQ/%E5%BE%AE%E4%BF%A1/MSN%EF%BC%88%E8%87%B3%E5%B0%91%E5%A1%AB%E5%86%99%E4%B8%80%E9%A1%B9%EF%BC%89)）将百度地图中查找到的相关信息直接发送到车上







兴趣点搜索

根据关键字查询POI信息（百度地图拥有的几千万兴趣点数据）







周边搜索

根据中心点位置、检索半径、关键字查询周边信息。如附近的4S店，加油站等







交通事件查询

根据城市名称或经纬度查询交通事件信息，如施工、禁行等







测距

提供测量多点之间的直线距离







驾车检索

根据起终点，查询城市内或跨城市的驾车路线方案







地理/逆地理编码

坐标和地址信息之间的相互转换功能







途径路段信息

根据起终点查询途径路段的信息，包括主要路段、红绿灯、摄像头等







天气查询

根据经纬度/城市名提供查询天气的结果







旅游线路查询

根据城市名称、城市代码、坐标查询城市 旅游路线







景点详情查询

根据POI 名称、UID查询景点详细信息







热映影片/影讯查询

根据城市名称、城市代码、坐标查询城市热映影片; 根据影片/影院名称查询实时影讯






# 使用场景

![carhome1.jpg](http://developer.baidu.com/map/static/img/carhome1.jpg)




![carhome3.jpg](http://developer.baidu.com/map/static/img/carhome3.jpg)



发送到汽车

该功能可以实现让用户把千万百度地图"点"数据，实时发送至车载地图中，为您提供位置指示、路径导航。"点"可适配多种车载地图，展示无偏差。用户只需登录电脑或者手机端百度地图，搜索到感兴趣的"点"，发送给好友，可随时随地共享您的位置、发起聚餐约会地等；同时，一键发送至车，便能实时更新车载地图数据，解决您的车载地图未及时更新而缺失数据之忧，为您提供贴心路径导航。





![carhome4.jpg](http://developer.baidu.com/map/static/img/carhome4.jpg)



实时查看爱车周边信息

行驶过程中缺油？自驾游时，哪就餐?... 百度地图车联网API为用户提供查找附近加油站，餐馆，娱乐等生活服务信息。同时，在3G和移动互联网时代的今天，车载设备已经具备卫星定位和联网功能，车联网API可以根据用户当前位置及加油站位置，提供最优路径，帮车主节省油耗，时间，直达目的地，真正做到为车主的爱车"加油"！。





![carhome7.png](http://developer.baidu.com/map/static/img/carhome7.png)



最佳旅游路线查询

想寻求一条靠谱的自驾游路线？这里哪儿最火最好玩？哪条旅游路线最实用？让百度地图车联网旅游路线API帮你出谋划策，一日游、两日游、三日游、深度游信息丰富全面，且每条路线都有游客沉淀下来的心得，靠谱实用路线让爱车带着您畅游大江南北。





![carhome8.png](http://developer.baidu.com/map/static/img/carhome8.png)



实时影讯上车

最近什么片最火爆？哪几家影院上映了？有哪几个场次？票价是多少？车联网影讯API推出热映影片查询，影院影讯查询，丰富车主生活，帮助驾车一族便捷掌握第一手影讯信息。





![carhome11.jpg](http://developer.baidu.com/map/static/img/carhome11.jpg)



智能出行

规避城市拥堵，合理规划出行方案，提高出行效率，玩转城市智能交通，结合百度地图实时路况，车联网服务提供道路事件、道路信息、摄像头位置等查询，让车主实时掌握各类交通资讯，方便快捷，实时准确，出行既安全又放心，一路畅行。





![carhome12.jpg](http://developer.baidu.com/map/static/img/carhome12.jpg)



监控定位

百度地图车联网API可以对物流配送、人员定位、快递、出租车、租车，等获取位置坐标后提供详细的地址描述，同时也可以根据车辆、人员当前详细的地址在地图中展示，从而实时把握车辆人员动态，实现基于位置的人和车管理。





![carhome15.jpg](http://developer.baidu.com/map/static/img/carhome15.jpg)



天气查询

为您提供实时全国天气气象信息,让您在爱车中掌握第一手天气信息，帮助您出行规划





![carhome16.jpg](http://developer.baidu.com/map/static/img/carhome16.jpg)



车联网创新

百度地图LBS开放平台开放性即受到业内认可，意与第三方共同挖掘车联网应用的潜能，目前已经成功和福特APPLink平台创新车联应用、丰田蓝牙send2car ，车联网应用，车联网的未来我们邀请你一起谱写。

















