# 图像识别 Java SDK文档

# 简介

Hi，您好，欢迎使用百度图像识别API服务。

本文档主要针对API开发者，描述百度图像识别接口服务的相关技术内容。如果您对文档内容有任何疑问，可以通过以下几种方式联系我们：

- 在百度云控制台内[提交工单](http://ticket.bce.baidu.com/#/ticket/create)，咨询问题类型请选择**人工智能服务**
- 如有疑问，进入**[AI社区交流](http://ai.baidu.com/forum/topic/list/171) **:<http://ai.baidu.com/forum/topic/list/171>
- 具有免费调用额度的接口，超过每天的免费额度后会返回错误码：17，错误信息：Open api daily request limit reached（每天流量超限额），**未上线计费的接口，可通过QQ群（659268104）联系群管、或提交工单手动提额**；已上线计费的接口，请直接在控制台开通计费，调用量不受限制，按调用量阶梯计费

## 接口能力

| 产品功能       | 接口名称           | 接口能力简要描述                                             |
| :------------- | :----------------- | :----------------------------------------------------------- |
| 通用图像分析   | 图像主体检测       | 识别图像中的主体具体坐标位置。                               |
|                | 通用物体和场景识别 | 识别图片中的场景及物体标签，支持8w+标签类型。                |
| 细粒度图像识别 | 菜品识别           | 检测用户上传的菜品图片，返回具体的菜名、卡路里、置信度信息。 |
|                | 车型识别           | 检测用户上传的车辆图片，识别所属车型，包括车辆品牌及具体型号、颜色、年份、位置信息。 |
|                | logo商标识别       | 识别图片中包含的商品LOGO信息，返回LOGO品牌名称、在图片中的位置、置信度。 |
|                | 动物识别           | 检测用户上传的动物图片，返回动物名称、置信度信息。           |
|                | 植物识别           | 检测用户上传的植物图片，返回植物名称、置信度信息。           |

| 接口名称           | 图片编码后大小限额                                           |
| :----------------- | :----------------------------------------------------------- |
| 通用物体和场景识别 | base64编码后大小不超过4M，最短边至少30px，最长边最大4096px，长宽比3：1以内 |
| 菜品识别           | base64编码后大小不超过4M，最短边至少15px，最长边最大4096px，长宽比3：1以内 |
| 车型识别           | base64编码后大小不超过4M，最短边至少15px，最长边最大4096px，长宽比3：1以内 |
| logo商标识别       | base64编码后大小不超过4M，最短边至少15px，最长边最大4096px，长宽比3：1以内 |
| 图像主体检测       | base64编码后大小不超过4M，最短边至少15px，最长边最大4096px，长宽比3：1以内 |
| 动物识别           | base64编码后大小不超过4M，最短边至少15px，最长边最大4096px，长宽比3：1以内 |
| 植物识别           | base64编码后大小不超过4M，最短边至少15px，最长边最大4096px，长宽比3：1以内 |

## 版本更新记录

| 上线日期   | 版本号 | 更新内容               |
| :--------- | :----- | :--------------------- |
| 2018.12.7  | 4.9.0  | 新增地标识别接口       |
| 2018.4.3   | 4.2.0  | 新增通用场景识别接口   |
| 2017.12.22 | 4.0.0  | 接口统一升级           |
| 2017.10.18 | 3.2.1  | 使用proxy问题修复      |
| 2017.9.27  | 3.2.0  | 新增动物、植物识别接口 |
| 2017.9.7   | 3.1.0  | 新增图像识别服务       |

# 快速入门

## 安装ImageClassify Java SDK

**ImageClassify Java SDK目录结构**

```
com.baidu.aip
       ├── auth                                //签名相关类
       ├── http                                //Http通信相关类
       ├── client                              //公用类
       ├── exception                           //exception类
       ├── imageclassify
       │       └── AipImageClassify           //AipImageClassify类
       └── util                                //工具类
```

**支持 JAVA版本：1.7+**

**查看源码**

Java SDK代码现已公开，您可以查看代码、或者在License范围内修改和编译SDK以适配您的环境。 github链接：https://github.com/Baidu-AIP/java-sdk

**使用maven依赖**：

添加以下依赖即可。其中版本号可在[maven官网](http://search.maven.org/#search%7Cga%7C1%7Caip)查询

```
<dependency>
    <groupId>com.baidu.aip</groupId>
    <artifactId>java-sdk</artifactId>
    <version>${version}</version>
</dependency>
```

**直接使用JAR包步骤如下**：

1.在[官方网站](http://ai.baidu.com/sdk)下载Java SDK压缩工具包。

2.将下载的`aip-java-sdk-version.zip`解压后，复制到工程文件夹中。

3.在Eclipse右键“工程 -> Properties -> Java Build Path -> Add JARs”。

4.添加SDK工具包`aip-java-sdk-version.jar`和第三方依赖工具包`json-20160810.jar``log4j-1.2.17.jar`。

其中，`version`为版本号，添加完成后，用户就可以在工程中使用ImageClassify Java SDK。

## 新建AipImageClassifyClient

AipImageClassifyClient是图像识别的Java客户端，为使用图像识别的开发人员提供了一系列的交互方法。

用户可以参考如下代码新建一个AipImageClassifyClient,初始化完成后建议**单例使用**,避免重复获取access_token：

```
public class Sample {
    //设置APPID/AK/SK
    public static final String APP_ID = "你的 App ID";
    public static final String API_KEY = "你的 Api Key";
    public static final String SECRET_KEY = "你的 Secret Key";

    public static void main(String[] args) {
        // 初始化一个AipImageClassifyClient
        AipImageClassifyClient client = new AipImageClassifyClient(APP_ID, API_KEY, SECRET_KEY);

        // 可选：设置网络连接参数
        client.setConnectionTimeoutInMillis(2000);
        client.setSocketTimeoutInMillis(60000);

        // 可选：设置代理服务器地址, http和socket二选一，或者均不设置
        client.setHttpProxy("proxy_host", proxy_port);  // 设置http代理
        client.setSocketProxy("proxy_host", proxy_port);  // 设置socket代理

        // 可选：设置log4j日志输出格式，若不设置，则使用默认配置
        // 也可以直接通过jvm启动参数设置此环境变量
        System.setProperty("aip.log4j.conf", "path/to/your/log4j.properties");

        // 调用接口
        String path = "test.jpg";
        JSONObject res = client.objectDetect(path, new HashMap<String, String>());
        System.out.println(res.toString(2));
        
    }
}
```

其中示例的log4j.properties文件内容如下：

```
#可以设置级别：debug>info>error
#debug：显示debug、info、error
#info：显示info、error
#error：只error
log4j.rootLogger=debug,appender1
#log4j.rootLogger=info,appender1
#log4j.rootLogger=error,appender1

#输出到控制台
log4j.appender.appender1=org.apache.log4j.ConsoleAppender
#样式为TTCCLayout
log4j.appender.appender1.layout=org.apache.log4j.PatternLayout

#自定义样式
# %r 时间 0
# %t 方法名 main
# %p 优先级 DEBUG/INFO/ERROR
# %c 所属类的全名(包括包名)
# %l 发生的位置，在某个类的某行
# %m 输出代码中指定的讯息，如log(message)中的message
# %n 输出一个换行

log4j.appender.appender1.layout.ConversionPattern=[%d{yy/MM/dd HH:mm:ss:SSS}][%t][%p] -%l %m%n
```

在上面代码中，常量`APP_ID`在百度云控制台中创建，常量`API_KEY`与`SECRET_KEY`是在创建完毕应用后，系统分配给用户的，均为字符串，用于标识用户，为访问做签名验证，可在AI服务控制台中的**应用列表**中查看。

**注意**：如您以前是百度云的老用户，其中`API_KEY`对应百度云的“Access Key ID”，`SECRET_KEY`对应百度云的“Access Key Secret”。

## 配置AipImageClassify

如果用户需要配置AipImageClassify的一些细节参数，可以在构造AipImageClassify之后调用接口设置参数，目前只支持以下参数：

| 接口                         | 说明                                                         |
| :--------------------------- | :----------------------------------------------------------- |
| setConnectionTimeoutInMillis | 建立连接的超时时间（单位：毫秒）                             |
| setSocketTimeoutInMillis     | 通过打开的连接传输数据的超时时间（单位：毫秒）               |
| setHttpProxy                 | 设置http代理服务器                                           |
| setSocketProxy               | 设置socket代理服务器 （http和socket类型代理服务器只能二选一） |

# 接口说明

## 通用物体识别

该请求用于通用物体及场景识别，即对于输入的一张图片（可正常解码，且长宽比适宜），输出图片中的多个物体及场景标签。

```
public void sample(AipImageClassify client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("baike_num", "5");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.advancedGeneral(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.advancedGeneral(file, options);
    System.out.println(res.toString(2));
}
```

**通用物体识别 请求参数详情**

| 参数名称  | 是否必选 | 类型   | 默认值 | 说明                             |
| :-------- | :------- | :----- | :----- | :------------------------------- |
| image     | 是       | mixed  |        | 本地图片路径或者图片二进制数据   |
| baike_num | 否       | String | 0      | 返回百科信息的结果数，默认不返回 |

**通用物体识别 返回数据参数详情**

| 字段          | 是否必选 | 类型          | 说明                                                      |
| :------------ | :------- | :------------ | :-------------------------------------------------------- |
| log_id        | 是       | uint64        | 唯一的log id，用于问题定位                                |
| result_num    | 是       | unit32        | 返回结果数目，及result数组中的元素个数                    |
| result        | 是       | arrry(object) | 标签结果数组                                              |
| +keyword      | 是       | string        | 图片中的物体或场景名称                                    |
| +score        | 是       | float         | 置信度，0-1                                               |
| +root         | 是       | string        | 识别结果的上层标签，有部分钱币、动漫、烟酒等tag无上层标签 |
| +baike_info   | object   | 否            | 对应识别结果的百科词条名称                                |
| ++baike_url   | string   | 否            | 对应识别结果百度百科页面链接                              |
| ++image_url   | string   | 否            | 对应识别结果百科图片链接                                  |
| ++description | string   | 否            | 对应识别结果百科内容描述                                  |

**通用物体识别 返回示例**

```
{
  "log_id": 327863200205075661,
  "result_num": 5,
  "result": [{
    "score": 0.967622,
    "root": "公众人物",
    "baike_info": {
      "baike_url": "http://baike.baidu.com/item/%E6%96%B0%E5%9E%A3%E7%BB%93%E8%A1%A3/8035884",
      "image_url": "http://imgsrc.baidu.com/baike/pic/item/91ef76c6a7efce1b27893518a451f3deb58f6546.jpg",
      "description": "新垣结衣(Aragaki Yui)，1988年6月11日出生于冲绳县那霸市。日本女演员、歌手、模特。毕业于日出高中。2001年，参加《nicola》模特比赛并获得最优秀奖。2005年，因出演现代剧《涩谷15》而作为演员出道。2006年，参演校园剧《我的老大，我的英雄》；同年，她还出版了个人首本写真集《水漾青春》。2007年，她从日出高校毕业后开始专注于演艺发展，并发表个人首张音乐专辑《天空》；同年，新垣结衣还主演了爱情片《恋空》，而她也凭借该片获得了多个电影新人奖项。2010年，主演爱情片《花水木》。2011年，主演都市剧《全开女孩》。2012年，相继参演现代剧《Legal High》、剧情片《剧场版新参者：麒麟之翼》。2013年，主演都市剧《飞翔情报室》。2014年，她主演了剧情片《黎明的沙耶》。2016年，主演爱情喜剧《逃避虽可耻但有用》，并凭借该剧获得了多个电视剧女主角奖项。2017年，主演爱情片《恋爱回旋》，凭借该片获得第60届蓝丝带奖最佳女主角；同年11月，她还凭借医疗剧《Code Blue 3》获得第94届日剧学院赏最佳女配角。"
    },
    "keyword": "新垣结衣"
  },
  {
    "score": 0.716067,
    "root": "人物-人物特写",
    "keyword": "头发"
  },
  {
    "score": 0.421281,
    "root": "商品-穿戴",
    "keyword": "围巾"
  },
  {
    "score": 0.22347,
    "root": "商品-五金",
    "keyword": "拉链"
  },
  {
    "score": 0.028031,
    "root": "商品-穿戴",
    "keyword": "脖套"
  }]
}
```

## 菜品识别

该请求用于菜品识别。即对于输入的一张图片（可正常解码，且长宽比适宜），输出图片的菜品名称、卡路里信息、置信度。

```
public void sample(AipImageClassify client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("top_num", "3");
    options.put("filter_threshold", "0.7");
    options.put("baike_num", "5");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.dishDetect(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.dishDetect(file, options);
    System.out.println(res.toString(2));
}
```

**菜品识别 请求参数详情**

| 参数名称         | 是否必选 | 类型   | 默认值 | 说明                                                  |
| :--------------- | :------- | :----- | :----- | :---------------------------------------------------- |
| image            | 是       | mixed  |        | 本地图片路径或者图片二进制数据                        |
| top_num          | 否       | String |        | 返回预测得分top结果数，默认为5                        |
| filter_threshold | 否       | String |        | 默认0.95，可以通过该参数调节识别效果，降低非菜识别率. |
| baike_num        | 否       | String | 0      | 返回百科信息的结果数，默认不返回                      |

**菜品识别 返回数据参数详情**

| 字段          | 是否必选 | 类型    | 说明                                   |
| :------------ | :------- | :------ | :------------------------------------- |
| log_id        | 是       | uint64  | 唯一的log id，用于问题定位             |
| result_num    | 否       | unit32  | 返回结果数目，及result数组中的元素个数 |
| result        | 否       | array() | 菜品识别结果数组                       |
| +name         | 否       | string  | 菜名，示例：鱼香肉丝                   |
| +calorie      | 否       | float   | 卡路里，每100g的卡路里含量             |
| +probability  | 否       | float   | 识别结果中每一行的置信度值，0-1        |
| +baike_info   | object   | 否      | 对应识别结果的百科词条名称             |
| ++baike_url   | string   | 否      | 对应识别结果百度百科页面链接           |
| ++image_url   | string   | 否      | 对应识别结果百科图片链接               |
| ++description | string   | 否      | 对应识别结果百科内容描述               |

**菜品识别 返回示例**

```
{
  "log_id": 7357081719365269362,
  "result_num": 5,
  "result": [
  {
    "calorie": "119",
    "has_calorie": true,
    "name": "酸汤鱼",
    "probability": "0.396031"
    "baike_info": {
      "baike_url": "http://baike.baidu.com/item/%E9%85%B8%E6%B1%A4%E9%B1%BC/1754055",
      "description": "酸汤鱼，是黔桂湘交界地区的一道侗族名菜，与侗族相邻的苗、水、瑶等少数民族也有相似菜肴，但其中以贵州侗族酸汤鱼最为有名，据考证此菜肴最早源于黎平县雷洞镇牙双一带。制作原料主要有鱼肉、酸汤、山仓子等香料。成菜后，略带酸味、幽香沁人、鲜嫩爽口开胃，是贵州“黔系”菜肴的代表作之一。这道菜通常先自制酸汤，之后将活鱼去掉内脏，入酸汤煮制。"
    }
  },
  {
    "calorie": "38",
    "has_calorie": true,
    "name": "原味黑鱼煲",
    "probability": "0.265432",

  },
  {
    "calorie": "144",
    "has_calorie": true,
    "name": "椒鱼片",
    "probability": "0.0998993"
  },
  {
    "calorie": "98",
    "has_calorie": true,
    "name": "酸菜鱼",
    "probability": "0.0701917"
  },
  {
    "calorie": "257.65",
    "has_calorie": true,
    "name": "柠檬鱼",
    "probability": "0.0471465"
  }]
}
```

## 车辆识别

该请求用于检测一张车辆图片的具体车型。即对于输入的一张图片（可正常解码，且长宽比适宜），输出图片的车辆品牌及型号。

```
public void sample(AipImageClassify client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("top_num", "3");
    options.put("baike_num", "5");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.carDetect(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.carDetect(file, options);
    System.out.println(res.toString(2));
}
```

**车辆识别 请求参数详情**

| 参数名称  | 是否必选 | 类型   | 默认值 | 说明                             |
| :-------- | :------- | :----- | :----- | :------------------------------- |
| image     | 是       | mixed  |        | 本地图片路径或者图片二进制数据   |
| top_num   | 否       | String |        | 返回预测得分top结果数，默认为5   |
| baike_num | 否       | String | 0      | 返回百科信息的结果数，默认不返回 |

**车辆识别 返回数据参数详情**

| 字段            | 是否必选 | 类型         | 说明                         |
| :-------------- | :------- | :----------- | :--------------------------- |
| log_id          | 否       | uint64       | 唯一的log id，用于问题定位   |
| color_result    | 是       | string       | 颜色                         |
| result          | 否       | car-result() | 车型识别结果数组             |
| +name           | 否       | string       | 车型名称，示例：宝马x6       |
| +score          | 否       | double       | 置信度，示例：0.5321         |
| +year           | 否       | string       | 年份                         |
| +baike_info     | object   | 否           | 对应识别结果的百科词条名称   |
| ++baike_url     | string   | 否           | 对应识别结果百度百科页面链接 |
| ++image_url     | string   | 否           | 对应识别结果百科图片链接     |
| ++description   | string   | 否           | 对应识别结果百科内容描述     |
| location_result | 否       | string       | 车在图片中的位置信息         |

**车辆识别 返回示例**

```
{
  "log_id": 4086212218842203806,
  "location_result": {
    "width": 447,
    "top": 226,
    "height": 209,
    "left": 188
  },
  "result": [{
    "baike_info": {
      "baike_url": "http://baike.baidu.com/item/%E5%B8%83%E5%8A%A0%E8%BF%AAChiron/20419512",
      "description": "布加迪Chiron是法国跑车品牌布加迪出品的豪华超跑车。配置四涡轮增压发动机，420 公里每小时，有23种颜色的选择，售价高达260万美元。"
    },
    "score": 0.98793351650238,
    "name": "布加迪Chiron",
    "year": "无年份信息"
  },
  {
    "score": 0.0021970034576952,
    "name": "奥迪RS5",
    "year": "2011-2017"
  },
  {
    "score": 0.0021096928976476,
    "name": "奥迪RS4",
    "year": "无年份信息"
  },
  {
    "score": 0.0015581247862428,
    "name": "奥迪RS7",
    "year": "2014-2016"
  },
  {
    "score": 0.00082337751518935,
    "name": "布加迪威航",
    "year": "2004-2015"
  }],
  "color_result": "颜色无法识别"
}
```

## logo商标识别

该请求用于检测和识别图片中的品牌LOGO信息。即对于输入的一张图片（可正常解码，且长宽比适宜），输出图片中LOGO的名称、位置和置信度。当效果欠佳时，可以建立子库（在[控制台](https://console.bce.baidu.com/ai/#/ai/imagerecognition/overview/index)创建应用并申请建库）并通过调用logo入口接口完成自定义logo入库，提高识别效果。

```
public void sample(AipImageClassify client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("custom_lib", "false");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.logoSearch(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.logoSearch(file, options);
    System.out.println(res.toString(2));
}
```

**logo商标识别 请求参数详情**

| 参数名称   | 是否必选 | 类型   | 可选值范围                                                | 默认值 | 说明                                                         |
| :--------- | :------- | :----- | :-------------------------------------------------------- | :----- | :----------------------------------------------------------- |
| image      | 是       | mixed  |                                                           |        | 本地图片路径或者图片二进制数据                               |
| custom_lib | 否       | String | true - 返回自定义库 false - 返回自定义库+默认库的识别结果 | false  | 是否只使用自定义logo库的结果，默认false：返回自定义库+默认库的识别结果 |

**logo商标识别 返回数据参数详情**

| 参数         | 类型   | 是否必须 | 说明                                                         | 示例                                           |
| :----------- | :----- | :------- | :----------------------------------------------------------- | :--------------------------------------------- |
| log_id       | number | 是       | 请求标识码，随机数，唯一                                     | 507499361                                      |
| result_num   | number | 是       | 返回结果数目，即：result数组中元素个数                       | 2                                              |
| result       | array  | 是       | 返回结果数组，每一项为一个识别出的logo                       | -                                              |
| +location    | object | 是       | 位置信息（左起像素位置、上起像素位置、像素宽、像素高）       | {"left": 100,"top":100,"width":10,"height":10} |
| ++left       | number | 是       | 左起像素位置                                                 | 100                                            |
| ++top        | number | 是       | 上起像素位置                                                 | 100                                            |
| ++width      | number | 是       | 像素宽                                                       | 100                                            |
| ++height     | number | 是       | 像素高                                                       | 100                                            |
| +name        | string | 是       | 识别的品牌名称                                               | 京东                                           |
| +probability | number | 是       | 分类结果置信度（0--1.0）                                     | 0.8                                            |
| +type        | number | 是       | type=0为1千种高优商标识别结果;type=1为2万类logo库的结果；其它type为自定义logo库结果 | 1 （2万类的结果）                              |

**logo商标识别 返回示例**

```
{
  "log_id": 843411868,
  "result_num": 1,
  "result": [
    {
      "type": 0,
      "name": "科颜氏",
      "probability": 0.99998807907104,
      "location": {
        "width": 296,
        "top": 20,
        "height": 128,
        "left": 23
      }
    }
  ]
}
```

## logo商标识别—添加

使用入库接口请先在[控制台](https://console.bce.baidu.com/ai/#/ai/imagerecognition/overview/index)创建应用并申请建库，建库成功后方可正常使用。

```
public void sample(AipImageClassify client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    
    String brief = "{\"name\": \"宝马\",\"code\":\"666\"}";
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.logoAdd(image, brief, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.logoAdd(file, brief, options);
    System.out.println(res.toString(2));
}
```

**logo商标识别—添加 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 说明                                                         |
| :------- | :------- | :----- | :----------------------------------------------------------- |
| image    | 是       | mixed  | 本地图片路径或者图片二进制数据                               |
| brief    | 是       | String | brief，检索时带回。此处要传对应的name与code字段，name长度小于100B，code长度小于150B |

**logo商标识别—添加 返回数据参数详情**

| 参数      | 类型   | 是否必须 | 说明                     | 示例      |
| :-------- | :----- | :------- | :----------------------- | :-------- |
| log_id    | number | 是       | 请求标识码，随机数，唯一 | 507499361 |
| cont_sign | string | 是       | 输入图片签名，可用于删除 | -         |

**logo商标识别—添加 返回示例**

```
{
  "log_id": 972934653,
  "cont_sign": "217113248,419422301"
}
```

## logo商标识别—删除

使用删除接口请先在[控制台](https://console.bce.baidu.com/ai/#/ai/imagerecognition/overview/index)创建应用并申请建库，建库成功后先调用入库接口完成logo图片入库，删除接口用户在已入库的logo图片中删除图片。

```
public void sample(AipImageClassify client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.logoDeleteByImage(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.logoDeleteByImage(file, options);
    System.out.println(res.toString(2));
    
    // 删除logo商标，传入参数为图片签名
    JSONObject res = client.logoDeleteBySign(contSign, options);
    System.out.println(res.toString(2));

}
```

**logo商标识别—删除 请求参数详情**

| 参数名称  | 是否必选 | 类型   | 说明                                       |
| :-------- | :------- | :----- | :----------------------------------------- |
| image     | 是       | mixed  | 本地图片路径或者图片二进制数据             |
| cont_sign | 是       | String | 图片签名（和image二选一，image优先级更高） |

**logo商标识别—删除 返回数据参数详情**

| 参数   | 类型   | 是否必须 | 说明                     | 示例      |
| :----- | :----- | :------- | :----------------------- | :-------- |
| log_id | number | 是       | 请求标识码，随机数，唯一 | 507499361 |

**logo商标识别—删除 返回示例**

```
{
  "log_id": 50488417
}
```

## 动物识别

该请求用于识别一张图片。即对于输入的一张图片（可正常解码，且长宽比适宜），输出动物识别结果

```
public void sample(AipImageClassify client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("top_num", "3");
    options.put("baike_num", "5");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.animalDetect(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.animalDetect(file, options);
    System.out.println(res.toString(2));
}
```

**动物识别 请求参数详情**

| 参数名称  | 是否必选 | 类型   | 默认值 | 说明                             |
| :-------- | :------- | :----- | :----- | :------------------------------- |
| image     | 是       | mixed  |        | 本地图片路径或者图片二进制数据   |
| top_num   | 否       | String | 6      | 返回预测得分top结果数，默认为6   |
| baike_num | 否       | String | 0      | 返回百科信息的结果数，默认不返回 |

**动物识别 返回数据参数详情**

| 参数          | 类型          | 是否必须 | 说明                         |
| :------------ | :------------ | :------- | :--------------------------- |
| log_id        | uint64        | 是       | 唯一的log id，用于问题定位   |
| result        | arrry(object) | 是       | 识别结果数组                 |
| +name         | string        | 是       | 动物名称，示例：蒙古马       |
| +score        | uint32        | 是       | 置信度，示例：0.5321         |
| +baike_info   | object        | 否       | 对应识别结果的百科词条名称   |
| ++baike_url   | string        | 否       | 对应识别结果百度百科页面链接 |
| ++image_url   | string        | 否       | 对应识别结果百科图片链接     |
| ++description | string        | 否       | 对应识别结果百科内容描述     |

**动物识别 返回示例**

```
{
  "log_id": 7392482912853822863,
  "result": [{
    "score": "0.993811",
    "name": "叉角羚",
    "baike_info": {
      "baike_url": "http://baike.baidu.com/item/%E5%8F%89%E8%A7%92%E7%BE%9A/801703",
      "description": "叉角羚(学名：Antilocapra americana)：在角的中部角鞘有向前伸的分枝，故名。体型中等，体长1-1.5米，尾长7.5-10厘米，肩高81-104厘米，成体重36-60千克，雌体比雄体小；背面为红褐色，颈部有黑色鬃毛，腹部和臀部为白色，颊面部和颈部两侧有黑色块斑；毛被下面为绒毛，上覆以粗糙、质脆的长毛，由于某些皮肤肌的作用，能使其毛被呈不同角度，以利于保暖或散热。植食。叉角羚奔跑速度非常快，最高时速达100千米。一次跳跃可达3.5-6米。善游泳。夏季组成小群活动，冬季则集结成上百只的大群。为寻找食物和水源，一年中常进行几次迁移。性机警，视觉敏锐，能看到数千米外的物体。遇险时，臀部的白色毛能立起，向同伴告警。分布于北美洲。"
    }
  },
  {
    "score": "0.000289439",
    "name": "印度羚"
  },
  {
    "score": "0.000186248",
    "name": "藏羚羊"
  },
  {
    "score": "0.000147176",
    "name": "跳羚"
  },
  {
    "score": "0.000134434",
    "name": "驯鹿"
  },
  {
    "score": "9.86555e-05",
    "name": "高鼻羚羊"
  }]
}
```

## 植物识别

该请求用于识别一张图片。即对于输入的一张图片（可正常解码，且长宽比适宜），输出植物识别结果。

```
public void sample(AipImageClassify client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("baike_num", "5");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.plantDetect(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.plantDetect(file, options);
    System.out.println(res.toString(2));
}
```

**植物识别 请求参数详情**

| 参数名称  | 是否必选 | 类型   | 默认值 | 说明                             |
| :-------- | :------- | :----- | :----- | :------------------------------- |
| image     | 是       | mixed  |        | 本地图片路径或者图片二进制数据   |
| baike_num | 否       | String | 0      | 返回百科信息的结果数，默认不返回 |

**植物识别 返回数据参数详情**

| 参数          | 类型          | 是否必须 | 说明                         |
| :------------ | :------------ | :------- | :--------------------------- |
| log_id        | uint64        | 是       | 唯一的log id，用于问题定位   |
| result        | arrry(object) | 是       | 植物识别结果数组             |
| +name         | string        | 是       | 植物名称，示例：吉娃莲       |
| +score        | uint32        | 是       | 置信度，示例：0.5321         |
| +baike_info   | object        | 否       | 对应识别结果的百科词条名称   |
| ++baike_url   | string        | 否       | 对应识别结果百度百科页面链接 |
| ++image_url   | string        | 否       | 对应识别结果百科图片链接     |
| ++description | string        | 否       | 对应识别结果百科内容描述     |

**植物识别 返回示例**

```
{
  "log_id": 1705495792822072357,
  "result": [{
    "score": 0.99979120492935,
    "name": "莲",
    "baike_info": {
      "baike_url": "http://baike.baidu.com/item/%E8%8E%B2/2717141",
      "description": "莲(Nelumbo nucifera)，又称荷、荷花、莲花、芙蕖、鞭蓉、水芙蓉、水芝、水芸、水旦、水华等，溪客、玉环是其雅称，未开的花蕾称菡萏，已开的花朵称鞭蕖，莲科，属多年生水生宿根草本植物，其地下茎称藕，能食用，叶入药，莲子为上乘补品，花可供观赏。是我国十大名花之一。是印度的国花。莲，双子叶植物，睡莲科。多年生挺水草本植物。根状茎横走，粗而肥厚，节间膨大，内有纵横通气孔道，节部缢缩。叶基生，挺出水面，盾形，直径30-90cm，波状边缘，上面深绿色，下面浅绿色。叶柄有小刺，长1-2m，挺出水面。花单生，直径10-25cm，椭圆花瓣多数，白色或粉红色；花柄长1-2m。花托在果期膨大，直径5-10cm，海绵质。坚果椭圆形和卵圆形，长1.5-2.0cm，灰褐色。种子卵圆形，长1.2-1.7cm，种皮红棕色。生于池塘、浅湖泊及稻田中。中国南北各省有自生或栽培，经济价值高。人们习惯上称种子为“莲子”、地下茎为“藕”、花托为“莲蓬”、叶为“荷叶”。"
    }
  },
  {
    "score": 0.00015144718054216,
    "name": "红睡莲"
  },
  {
    "score": 1.2172759852547e-05,
    "name": "白睡莲"
  },
  {
    "score": 6.305016540864e-06,
    "name": "延药睡莲"
  },
  {
    "score": 3.6133328649157e-06,
    "name": "华夏慈姑"
  }]
}
```

## 图像主体检测

用户向服务请求检测图像中的主体位置。

```
public void sample(AipImageClassify client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("with_face", "0");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.objectDetect(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.objectDetect(file, options);
    System.out.println(res.toString(2));
}
```

**图像主体检测 请求参数详情**

| 参数名称  | 是否必选 | 类型   | 可选值范围                      | 默认值 | 说明                                                         |
| :-------- | :------- | :----- | :------------------------------ | :----- | :----------------------------------------------------------- |
| image     | 是       | mixed  |                                 |        | 本地图片路径或者图片二进制数据                               |
| with_face | 否       | String | 0 - 不带人脸区域 1 - 带人脸区域 | 1      | 如果检测主体是人，主体区域是否带上人脸部分，0-不带人脸区域，其他-带人脸区域，裁剪类需求推荐带人脸，检索/识别类需求推荐不带人脸。默认取1，带人脸。 |

**图像主体检测 返回数据参数详情**

| 参数    | 类型   | 是否必须 | 说明                                   | 示例      |
| :------ | :----- | :------- | :------------------------------------- | :-------- |
| log_id  | number | 是       | 唯一的log id，用于问题定位             | 507499361 |
| result  | object | 是       | 裁剪结果                               | -         |
| +left   | number | 是       | 表示定位位置的长方形左上顶点的水平坐标 | 50        |
| +top    | number | 是       | 表示定位位置的长方形左上顶点的垂直坐标 | 60        |
| +width  | number | 是       | 表示定位位置的长方形的宽度             | 200       |
| +height | number | 是       | 表示定位位置的长方形的高度             | 200       |

**图像主体检测 返回示例**

```
{
  "log_id": 895582300,
  "result": {
    "width": 486,
    "top": 76,
    "left": 134,
    "height": 394
  }
}
```

## 地标识别

该请求用于识别地标，即对于输入的一张图片（可正常解码，且长宽比适宜），输出图片中的地标识别结果。

```
public void sample(AipImageClassify client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.landmark(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.landmark(file, options);
    System.out.println(res.toString(2));
}
```

**地标识别 请求参数详情**

| 参数名称 | 是否必选 | 类型  | 说明                           |
| :------- | :------- | :---- | :----------------------------- |
| image    | 是       | mixed | 本地图片路径或者图片二进制数据 |

**地标识别 返回数据参数详情**

| 参数      | 类型   | 是否必须 | 说明                             | 示例       |
| :-------- | :----- | :------- | :------------------------------- | :--------- |
| log_id    | number | 是       | 唯一的log id，用于问题定位       | 507499361  |
| result    | object | 是       | 裁剪结果                         | -          |
| +landmark | string | 是       | 地标名称，无法识别则返回空字符串 | 狮身人面像 |

**地标识别 返回示例**

```
{
  "log_id": 3450013152046070669,
  "result":
  {
    "landmark": "狮身人面像"
  }
}
```

# 错误信息

## 错误返回格式

若请求错误，服务器将返回的JSON文本包含以下参数：

- **error_code**：错误码。
- **error_msg**：错误描述信息，帮助理解和解决发生的错误。

## 错误码

**SDK本地检测参数返回的错误码**：

| error_code | error_msg                        | 备注                   |
| :--------- | :------------------------------- | :--------------------- |
| SDK100     | image size error                 | 图片大小超限           |
| SDK101     | image length error               | 图片边长不符合要求     |
| SDK102     | read image file error            | 读取图片文件错误       |
| SDK108     | connection or read data time out | 连接超时或读取数据超时 |
| SDK109     | unsupported image format         | 不支持的图片格式       |

**服务端返回的错误码**

| 错误码 | 错误信息                                | 描述                                                         |
| :----- | :-------------------------------------- | :----------------------------------------------------------- |
| 4      | Open api request limit reached          | 集群超限额                                                   |
| 14     | IAM Certification failed                | IAM鉴权失败，建议用户参照文档自查生成sign的方式是否正确，或换用控制台中ak sk的方式调用 |
| 17     | Open api daily request limit reached    | 每天流量超限额                                               |
| 18     | Open api qps request limit reached      | QPS超限额                                                    |
| 19     | Open api total request limit reached    | 请求总量超限额                                               |
| 100    | Invalid parameter                       | 无效参数                                                     |
| 110    | Access token invalid or no longer valid | Access Token失效                                             |
| 111    | Access token expired                    | Access token过期                                             |
| 282000 | internal error                          | 服务器内部错误，请再次请求， 如果持续出现此类错误，请通过QQ群（659268104）或工单联系技术支持团队。 |
| 216100 | invalid param                           | 请求中包含非法参数，请检查后重新尝试                         |
| 216101 | not enough param                        | 缺少必须的参数，请检查参数是否有遗漏                         |
| 216102 | service not support                     | 请求了不支持的服务，请检查调用的url                          |
| 216103 | param too long                          | 请求中某些参数过长，请检查后重新尝试                         |
| 216110 | appid not exist                         | appid不存在，请重新核对信息是否为后台应用列表中的appid       |
| 216200 | empty image                             | 图片为空，请检查后重新尝试                                   |
| 216201 | image format error                      | 上传的图片格式错误，现阶段我们支持的图片格式为：PNG、JPG、JPEG、BMP，请进行转码或更换图片 |
| 216202 | image size error                        | 上传的图片大小错误，现阶段我们支持的图片大小为：base64编码后小于4M，分辨率不高于4096*4096，请重新上传图片 |
| 216203 | image size error                        | 上传的图片base64编码有误，请校验base64编码方式，并重新上传图片 |
| 216630 | recognize error                         | 识别错误，请再次请求，如果持续出现此类错误，请通过QQ群（659268104）或工单联系技术支持团队。 |
| 216631 | recognize bank card error               | 识别银行卡错误，出现此问题的原因一般为：您上传的图片非银行卡正面，上传了异形卡的图片或上传的银行卡正品图片不完整 |
| 216633 | recognize idcard error                  | 识别身份证错误，出现此问题的原因一般为：您上传了非身份证图片或您上传的身份证图片不完整 |
| 216634 | detect error                            | 检测错误，请再次请求，如果持续出现此类错误，请通过QQ群（659268104）或工单联系技术支持团队。 |
| 282003 | missing parameters: {参数名}            | 请求参数缺失                                                 |
| 282005 | batch  processing error                 | 处理批量任务时发生部分或全部错误，请根据具体错误码排查       |
| 282006 | batch task  limit reached               | 批量任务处理数量超出限制，请将任务数量减少到10或10以下       |
| 282100 | image transcode error                   | 图片压缩转码错误                                             |
| 282101 | image split limit reached               | 长图片切分数量超限                                           |
| 282102 | target detect error                     | 未检测到图片中识别目标                                       |
| 282103 | target recognize error                  | 图片目标识别错误                                             |
| 282114 | url size error                          | URL长度超过1024字节或为0                                     |
| 282808 | request id: xxxxx not exist             | request id xxxxx 不存在                                      |
| 282809 | result type error                       | 返回结果请求错误（不属于excel或json）                        |
| 282810 | image recognize error                   | 图像识别错误                                                 |
| 283300 | Invalid argument                        | 入参格式有误，可检查下图片编码、代码格式是否有误             |
| 336000 | Internal error                          | 服务器内部错误，请再次请求， 如果持续出现此类错误，请通过QQ群（659268104）或工单联系技术支持团队 |
| 336001 | Invalid Argument                        | 入参格式有误，比如缺少必要参数、图片base64编码错误等等，可检查下图片编码、代码格式是否有误。有疑问请通过QQ群（659268104）或工单联系技术支持团队 |