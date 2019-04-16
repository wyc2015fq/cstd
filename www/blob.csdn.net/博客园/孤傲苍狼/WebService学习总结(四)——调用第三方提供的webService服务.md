# WebService学习总结(四)——调用第三方提供的webService服务 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [WebService学习总结(四)——调用第三方提供的webService服务](https://www.cnblogs.com/xdp-gacl/p/4260627.html)



　　互联网上面有很多的免费webService服务，我们可以调用这些免费的WebService服务，将一些其他网站的内容信息集成到我们的Web应用中显示，下面就以获取天气预报数据和查询**国内手机号码归属地**为例进行说明。

　　气象中心的管理系统将收集的天气信息并将数据暴露出来(通过WebService Server), 而各大站点的应用就去调用它们得到天气信息并以不同的样式去展示(WebService Client).

## 一、 调用免费的web service获取天气预报信息

### 1.1、**找到提供天气预报信息的****Webservice****的网络地址**

[http://webservice.webxml.com.cn/WebServices/WeatherWS.asmx](http://webservice.webxml.com.cn/WebServices/WeatherWS.asmx) 访问结果如下图所示：

![](https://images0.cnblogs.com/blog/289233/201501/291803534729397.png)

　　找到WebService的wsdl描述，通过URL地址[http://webservice.webxml.com.cn/WebServices/WeatherWS.asmx?wsdl](http://webservice.webxml.com.cn/WebServices/WeatherWS.asmx?wsdl)：如下：

![](https://images0.cnblogs.com/blog/289233/201501/291807011917980.png)

### 1.2、**客户端编码方式访问第三方提供的webService服务**

　　1、借助wsimport命令工具自动生成客户端代码

　　创建一个测试项目，如下：

![](https://images0.cnblogs.com/blog/289233/201501/291812178942002.png)

　　打开命令行窗口，切换到项目的src目录，执行"wsimport -keep [http://webservice.webxml.com.cn/WebServices/WeatherWS.asmx?wsdl](http://webservice.webxml.com.cn/WebServices/WeatherWS.asmx?wsdl)"生成客户端代码，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201501/291816094411874.png)

　　运行结果显示，使用wsimport工具直接生成客户端代码会抛异常, 无法生成客户端代码, 只是因为我们要调用的WebService是用.net写的，这个是Java调用net的webservice都有的问题，这个问题的解决办法如下：
- 将对应的wsdl文档保存到本地

![](https://images0.cnblogs.com/blog/289233/201501/291843172539113.gif)

　　2、修改wsdl文档的部分内容:**将****<s:element ref="s:schema" /><s:any />** 替换成 **<s:any minOccurs="2" maxOccurs="2"/>**

**![](https://images0.cnblogs.com/blog/289233/201501/291851362693798.gif)**

　　再次执行wsimport生成代码，这次使用保存在本地的WeatherWS.wsdl文件来进行生成，如下图所示：

**![](https://images0.cnblogs.com/blog/289233/201501/291833037221243.png)**

　　从执行结果显示，这一次可以正常生成代码了，刷新一下src目录，就可以看到生成的代码了，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201501/291836151125713.png)

2、借助生成的代码编写请求代码

```
1 package me.gacl.ws.client;
 2 
 3 import java.util.List;
 4 
 5 import cn.com.webxml.ArrayOfString;
 6 import cn.com.webxml.WeatherWS;
 7 import cn.com.webxml.WeatherWSSoap;
 8 
 9 public class WeatherWSClient {
10 
11     public static void main(String[] args) {
12         //创建一个WeatherWS工厂
13         WeatherWS factory = new WeatherWS();
14         //根据工厂创建一个WeatherWSSoap对象
15         WeatherWSSoap weatherWSSoap = factory.getWeatherWSSoap();
16         //调用WebService提供的getWeather方法获取南宁市的天气预报情况
17         ArrayOfString weatherInfo = weatherWSSoap.getWeather("南宁", null);
18         List<String> lstWeatherInfo = weatherInfo.getString();
19         //遍历天气预报信息
20         for (String string : lstWeatherInfo) {
21             System.out.println(string);
22             System.out.println("------------------------");
23         }
24     }
25 }
```

　　访问结果如下：

```
广西 南宁
------------------------
南宁
------------------------
2391
------------------------
2015/01/29 19:00:49
------------------------
今日天气实况：暂无实况
------------------------
空气质量：暂无；紫外线强度：最弱
------------------------
太阳镜指数：不需要。白天光线弱不需要佩戴太阳镜。
穿衣指数：较舒适。建议穿薄外套或牛仔裤等服装。
旅游指数：适宜。温度适宜，可尽情享受大自然风光。
运动指数：较适宜。较适宜进行各种户内外运动。
洗车指数：不宜。有雨，雨水和泥水会弄脏爱车。
化妆指数：保湿。请选用中性保湿型霜类化妆品。
感冒指数：较易发。天较凉，增加衣服，注意防护。
空气污染指数：暂无。
紫外线指数：最弱。辐射弱，涂擦SPF8-12防晒护肤品。
舒适度指数：舒适。白天不冷不热，风力不大。

------------------------
1月29日 多云
------------------------
11℃/15℃
------------------------
东北风微风
------------------------
1.gif
------------------------
1.gif
------------------------
1月30日 小雨转阴
------------------------
10℃/15℃
------------------------
东北风微风
------------------------
7.gif
------------------------
2.gif
------------------------
1月31日 多云
------------------------
11℃/13℃
------------------------
东北风微风
------------------------
1.gif
------------------------
1.gif
------------------------
2月1日 多云
------------------------
12℃/17℃
------------------------
东北风微风
------------------------
1.gif
------------------------
1.gif
------------------------
2月2日 阴转多云
------------------------
13℃/18℃
------------------------
东北风微风
------------------------
2.gif
------------------------
1.gif
------------------------
```

　　这样，我们通过生成的Client代码调用了第三方提供的webService服务获取到了南宁市的天气预报信息。

## 二、 调用免费的web service查询**国内手机号码归属地**

### 2.1、找到提供服务的Webservice的网络地址

[http://webservice.webxml.com.cn/WebServices/MobileCodeWS.asmx](http://webservice.webxml.com.cn/WebServices/MobileCodeWS.asmx) 如下所示：

![](https://images0.cnblogs.com/blog/289233/201501/291921125037348.png)

　　找到WebService的wsdl描述信息，通过URL地址[http://webservice.webxml.com.cn/WebServices/MobileCodeWS.asmx?wsdl](http://webservice.webxml.com.cn/WebServices/MobileCodeWS.asmx?wsdl)：如下：

![](https://images0.cnblogs.com/blog/289233/201501/291925001286789.png)

### 2.2、编写客户端访问调用WebService服务

1、创建客户端项目

![](https://images0.cnblogs.com/blog/289233/201501/291928306282032.png)

2、根据WebService的wsdl描述生成客户端代码

　　打开命令行窗口，切换到Phone_Client的src目录，执行wsimport -keep [http://webservice.webxml.com.cn/WebServices/MobileCodeWS.asmx?wsdl](http://webservice.webxml.com.cn/WebServices/MobileCodeWS.asmx?wsdl) 如下图所示：

![](https://images0.cnblogs.com/blog/289233/201501/291933009569318.png)

　　这次可以直接使用wsdl生成代码，不再需要将wsdl文件下载到本地进行修改了，这个WebService应该不是使用.net写的，生成的代码如下：

![](https://images0.cnblogs.com/blog/289233/201501/291935288626070.png)

　　3、借助生成的代码编写请求代码

```
1 package me.gacl.ws.client;
 2 
 3 import cn.com.webxml.MobileCodeWS;
 4 import cn.com.webxml.MobileCodeWSSoap;
 5 
 6 public class PhoneWsClient {
 7 
 8     public static void main(String[] args) {
 9         //创建一个MobileCodeWS工厂
10         MobileCodeWS factory = new MobileCodeWS();
11         //根据工厂创建一个MobileCodeWSSoap对象
12         MobileCodeWSSoap mobileCodeWSSoap = factory.getMobileCodeWSSoap();
13         ////调用WebService提供的getMobileCodeInfo方法查询手机号码的归属地
14         String searchResult = mobileCodeWSSoap.getMobileCodeInfo("15177196635", null);
15         System.out.println(searchResult);
16     }
17 }
```

　　运行结果如下：
![](https://images0.cnblogs.com/blog/289233/201501/291943526446545.png)

　　这样我们调用第三方提供的WebService服务成功查询到了手机号码的归宿地。

　　以上就是如何调用第三方WebService的相关内容。









