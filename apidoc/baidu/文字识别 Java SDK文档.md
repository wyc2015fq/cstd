# 文字识别 Java SDK文档

# 简介

Hi，您好，欢迎使用百度文字识别服务。

本文档主要针对Java开发者，描述百度文字识别接口服务的相关技术内容。如果您对文档内容有任何疑问，可以通过以下几种方式联系我们：

- 在百度云控制台内[提交工单](http://ticket.bce.baidu.com/#/ticket/create)，咨询问题类型请选择**人工智能服务**；
- 如有疑问，进入[AI社区交流](http://ai.baidu.com/forum/topic/list/164)：http://ai.baidu.com/forum/topic/list/164

## 接口能力

| 接口名称                       | 接口能力简要描述                                             |
| :----------------------------- | :----------------------------------------------------------- |
| 通用文字识别                   | 识别图片中的文字信息                                         |
| 通用文字识别（高精度版）       | 更高精度地识别图片中的文字信息                               |
| 通用文字识别（含位置信息版）   | 识别图片中的文字信息（包含文字区域的坐标信息）               |
| 通用文字识别（高精度含位置版） | 更高精度地识别图片中的文字信息（包含文字区域的坐标信息）     |
| 通用文字识别（含生僻字版）     | 识别图片中的文字信息（包含对常见字和生僻字的识别）           |
| 网络图片文字识别               | 识别一些网络上背景复杂，特殊字体的文字                       |
| 身份证识别                     | 识别身份证正反面的文字信息                                   |
| 银行卡识别                     | 识别银行卡的卡号并返回发卡行和卡片性质信息                   |
| 驾驶证识别                     | 识别机动车驾驶证所有关键字段                                 |
| 行驶证识别                     | 识别机动车行驶证所有关键字段                                 |
| 车牌识别                       | 对小客车的车牌进行识别                                       |
| 营业执照识别                   | 对营业执照进行识别                                           |
| 表格文字识别                   | 自动识别表格线及表格内容，结构化输出表头、表尾及每个单元格的文字内容 |
| 通用票据识别                   | 对各类票据图片（医疗票据，保险保单等）进行文字识别，并返回文字在图片中的位置信息 |
| 自定义模板文字识别             | 自定义模板文字识别可以通过自助的模板制作，建立起键值的对应关系，一步完成非结构化到结构化的转换，实现自动化的数据录入 |

## 版本更新记录

| 上线日期   | 版本号 | 更新内容                                                     |
| :--------- | :----- | :----------------------------------------------------------- |
| 2018.11.9  | 4.8.0  | 新增火车票、出租车票识别接口                                 |
| 2018.4.28  | 4.3.0  | 新增ocr邀测接口                                              |
| 2018.4.3   | 4.2.0  | 新增表格识别同步接口                                         |
| 2018.1.11  | 4.1.0  | 新增自定义模板ocr接口                                        |
| 2017.12.22 | 4.0.0  | 接口统一升级                                                 |
| 2017.10.18 | 3.2.1  | 使用proxy问题修复                                            |
| 2017.8.25  | 3.0.0  | 新增营业执照识别接口，更新sdk打包方式：所有AI服务集成一个SDK |
| 2017.8.10  | 1.3.9  | 新增票据识别接口                                             |
| 2017.7.28  | 1.3.8  | 新增通用文字识别高精度版（包括带位置信息版），通用文字识别、通用文字识别（带位置信息版）、通用文字识别（生僻字版）支持参数为图片url |
| 2017.7.14  | 1.3.7  | 新增车牌识别接口，更新sdk打包方式                            |
| 2017.6.30  | 1.3.6  | 新增表格识别系列接口                                         |
| 2017.6.16  | 1.3.5  | 新增驾驶证、行驶证识别接口                                   |
| 2017.4.13  | 1.3.2  | 新增通用文字识别（含生僻字版）和网图识别接口                 |
| 2017.3.31  | 1.3.1  | 新增通用文字识别（含位置信息版）                             |
| 2017.3.23  | 1.3    | 兼容Android环境                                              |
| 2017.3.2   | 1.2    | 上线对图片参数要求限制，增加设置超时接口                     |
| 2017.1.20  | 1.1    | 对部分云用户调用不成功的错误修复                             |
| 2017.1.6   | 1.0    | 初始版本，上线身份证识别、银行卡识别和通用文字识别接口       |

# 快速入门

## 安装OCR Java SDK

**OCR Java SDK目录结构**

```
com.baidu.aip
       ├── auth                                //签名相关类
       ├── http                                //Http通信相关类
       ├── client                              //公用类
       ├── exception                           //exception类
       ├── ocr
       │       └── AipOcr           //AipOcr类
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

其中，`version`为版本号，添加完成后，用户就可以在工程中使用OCR Java SDK。

## 新建AipOcr

AipOcr是Optical Character Recognition的Java客户端，为使用Optical Character Recognition的开发人员提供了一系列的交互方法。

用户可以参考如下代码新建一个AipOcr,初始化完成后建议**单例使用**,避免重复获取access_token：

```
public class Sample {
    //设置APPID/AK/SK
    public static final String APP_ID = "你的 App ID";
    public static final String API_KEY = "你的 Api Key";
    public static final String SECRET_KEY = "你的 Secret Key";

    public static void main(String[] args) {
        // 初始化一个AipOcr
        AipOcr client = new AipOcr(APP_ID, API_KEY, SECRET_KEY);

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
        JSONObject res = client.basicGeneral(path, new HashMap<String, String>());
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

## 配置AipOcr

如果用户需要配置AipOcr的一些细节参数，可以在构造AipOcr之后调用接口设置参数，目前只支持以下参数：

| 接口                         | 说明                                                         |
| :--------------------------- | :----------------------------------------------------------- |
| setConnectionTimeoutInMillis | 建立连接的超时时间（单位：毫秒）                             |
| setSocketTimeoutInMillis     | 通过打开的连接传输数据的超时时间（单位：毫秒）               |
| setHttpProxy                 | 设置http代理服务器                                           |
| setSocketProxy               | 设置socket代理服务器 （http和socket类型代理服务器只能二选一） |

# 接口说明

## 通用文字识别

用户向服务请求识别某张图中的所有文字

```
public void sample(AipOcr client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("language_type", "CHN_ENG");
    options.put("detect_direction", "true");
    options.put("detect_language", "true");
    options.put("probability", "true");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.basicGeneral(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.basicGeneral(file, options);
    System.out.println(res.toString(2));
    
    // 通用文字识别, 图片参数为远程url图片
    JSONObject res = client.basicGeneralUrl(url, options);
    System.out.println(res.toString(2));

}
```

**通用文字识别 请求参数详情**

| 参数名称         | 是否必选 | 类型   | 可选值范围                                  | 默认值  | 说明                                                         |
| :--------------- | :------- | :----- | :------------------------------------------ | :------ | :----------------------------------------------------------- |
| image            | 是       | mixed  |                                             |         | 本地图片路径或者图片二进制数据                               |
| url              | 是       | String |                                             |         | 图片完整URL，URL长度不超过1024字节，URL对应的图片base64编码后大小不超过4M，最短边至少15px，最长边最大4096px,支持jpg/png/bmp格式，当image字段存在时url字段失效 |
| language_type    | 否       | String | CHN_ENG ENG POR FRE GER ITA SPA RUS JAP KOR | CHN_ENG | 识别语言类型，默认为CHN_ENG。可选值包括： - CHN_ENG：中英文混合； - ENG：英文； - POR：葡萄牙语； - FRE：法语； - GER：德语； - ITA：意大利语； - SPA：西班牙语； - RUS：俄语； - JAP：日语； - KOR：韩语； |
| detect_direction | 否       | String | true false                                  | false   | 是否检测图像朝向，默认不检测，即：false。朝向是指输入图像是正常方向、逆时针旋转90/180/270度。可选值包括: - true：检测朝向； - false：不检测朝向。 |
| detect_language  | 否       | String | true false                                  | false   | 是否检测语言，默认不检测。当前支持（中文、英语、日语、韩语） |
| probability      | 否       | String | true false                                  |         | 是否返回识别结果中每一行的置信度                             |

**通用文字识别 返回数据参数详情**

| 字段             | 必选 | 类型   | 说明                                                         |
| :--------------- | :--- | :----- | :----------------------------------------------------------- |
| direction        | 否   | number | 图像方向，当detect_direction=true时存在。 - -1:未定义， - 0:正向， - 1: 逆时针90度， - 2:逆时针180度， - 3:逆时针270度 |
| log_id           | 是   | number | 唯一的log id，用于问题定位                                   |
| words_result_num | 是   | number | 识别结果数，表示words_result的元素个数                       |
| words_result     | 是   | array  | 定位和识别结果数组                                           |
| +words           | 否   | string | 识别结果字符串                                               |
| probability      | 否   | object | 行置信度信息；如果输入参数 probability = true 则输出         |
| +average         | 否   | number | 行置信度平均值                                               |
| +variance        | 否   | number | 行置信度方差                                                 |
| +min             | 否   | number | 行置信度最小值                                               |

**通用文字识别 返回示例**

```
{
"log_id": 2471272194,
"words_result_num": 2,
"words_result":
    [
        {"words": " TSINGTAO"},
        {"words": "青島睥酒"}
    ]
}
```

## 通用文字识别（高精度版）

用户向服务请求识别某张图中的所有文字，相对于通用文字识别该产品精度更高，但是识别耗时会稍长。

```
public void sample(AipOcr client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("detect_direction", "true");
    options.put("probability", "true");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.basicAccurateGeneral(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.basicAccurateGeneral(file, options);
    System.out.println(res.toString(2));
}
```

**通用文字识别（高精度版） 请求参数详情**

| 参数名称         | 是否必选 | 类型   | 可选值范围 | 默认值 | 说明                                                         |
| :--------------- | :------- | :----- | :--------- | :----- | :----------------------------------------------------------- |
| image            | 是       | mixed  |            |        | 本地图片路径或者图片二进制数据                               |
| detect_direction | 否       | String | true false | false  | 是否检测图像朝向，默认不检测，即：false。朝向是指输入图像是正常方向、逆时针旋转90/180/270度。可选值包括: - true：检测朝向； - false：不检测朝向。 |
| probability      | 否       | String | true false |        | 是否返回识别结果中每一行的置信度                             |

**通用文字识别（高精度版） 返回数据参数详情**

| 字段             | 必选 | 类型   | 说明                                                         |
| :--------------- | :--- | :----- | :----------------------------------------------------------- |
| direction        | 否   | number | 图像方向，当detect_direction=true时存在。 - -1:未定义， - 0:正向， - 1: 逆时针90度， - 2:逆时针180度， - 3:逆时针270度 |
| log_id           | 是   | number | 唯一的log id，用于问题定位                                   |
| words_result_num | 是   | number | 识别结果数，表示words_result的元素个数                       |
| words_result     | 是   | array  | 定位和识别结果数组                                           |
| +words           | 否   | string | 识别结果字符串                                               |
| probability      | 否   | object | 行置信度信息；如果输入参数 probability = true 则输出         |
| +average         | 否   | number | 行置信度平均值                                               |
| +variance        | 否   | number | 行置信度方差                                                 |
| +min             | 否   | number | 行置信度最小值                                               |

**通用文字识别（高精度版） 返回示例**

**参考通用文字识别返回示例**

## 通用文字识别（含位置信息版）

用户向服务请求识别某张图中的所有文字，并返回文字在图中的位置信息。

```
public void sample(AipOcr client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("recognize_granularity", "big");
    options.put("language_type", "CHN_ENG");
    options.put("detect_direction", "true");
    options.put("detect_language", "true");
    options.put("vertexes_location", "true");
    options.put("probability", "true");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.general(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.general(file, options);
    System.out.println(res.toString(2));
    
    // 通用文字识别（含位置信息版）, 图片参数为远程url图片
    JSONObject res = client.generalUrl(url, options);
    System.out.println(res.toString(2));

}
```

**通用文字识别（含位置信息版） 请求参数详情**

| 参数名称              | 是否必选 | 类型   | 可选值范围                                    | 默认值  | 说明                                                         |
| :-------------------- | :------- | :----- | :-------------------------------------------- | :------ | :----------------------------------------------------------- |
| image                 | 是       | mixed  |                                               |         | 本地图片路径或者图片二进制数据                               |
| url                   | 是       | String |                                               |         | 图片完整URL，URL长度不超过1024字节，URL对应的图片base64编码后大小不超过4M，最短边至少15px，最长边最大4096px,支持jpg/png/bmp格式，当image字段存在时url字段失效 |
| recognize_granularity | 否       | String | big - 不定位单字符位置 small - 定位单字符位置 | small   | 是否定位单字符位置，big：不定位单字符位置，默认值；small：定位单字符位置 |
| language_type         | 否       | String | CHN_ENG ENG POR FRE GER ITA SPA RUS JAP KOR   | CHN_ENG | 识别语言类型，默认为CHN_ENG。可选值包括： - CHN_ENG：中英文混合； - ENG：英文； - POR：葡萄牙语； - FRE：法语； - GER：德语； - ITA：意大利语； - SPA：西班牙语； - RUS：俄语； - JAP：日语； - KOR：韩语； |
| detect_direction      | 否       | String | true false                                    | false   | 是否检测图像朝向，默认不检测，即：false。朝向是指输入图像是正常方向、逆时针旋转90/180/270度。可选值包括: - true：检测朝向； - false：不检测朝向。 |
| detect_language       | 否       | String | true false                                    | false   | 是否检测语言，默认不检测。当前支持（中文、英语、日语、韩语） |
| vertexes_location     | 否       | String | true false                                    | false   | 是否返回文字外接多边形顶点位置，不支持单字位置。默认为false  |
| probability           | 否       | String | true false                                    |         | 是否返回识别结果中每一行的置信度                             |

**通用文字识别（含位置信息版） 返回数据参数详情**

| 字段               | 必选 | 类型   | 说明                                                         |
| :----------------- | :--- | :----- | :----------------------------------------------------------- |
| direction          | 否   | number | 图像方向，当detect_direction=true时存在。 - -1:未定义， - 0:正向， - 1: 逆时针90度， - 2:逆时针180度， - 3:逆时针270度 |
| log_id             | 是   | number | 唯一的log id，用于问题定位                                   |
| words_result       | 是   | array  | 定位和识别结果数组                                           |
| words_result_num   | 是   | number | 识别结果数，表示words_result的元素个数                       |
| +vertexes_location | 否   | array  | 当前为四个顶点: 左上，右上，右下，左下。当vertexes_location=true时存在 |
| ++x                | 是   | number | 水平坐标（坐标0点为左上角）                                  |
| ++y                | 是   | number | 垂直坐标（坐标0点为左上角）                                  |
| +location          | 是   | array  | 位置数组（坐标0点为左上角）                                  |
| ++left             | 是   | number | 表示定位位置的长方形左上顶点的水平坐标                       |
| ++top              | 是   | number | 表示定位位置的长方形左上顶点的垂直坐标                       |
| ++width            | 是   | number | 表示定位位置的长方形的宽度                                   |
| ++height           | 是   | number | 表示定位位置的长方形的高度                                   |
| +words             | 否   | number | 识别结果字符串                                               |
| +chars             | 否   | array  | 单字符结果，recognize_granularity=small时存在                |
| ++location         | 是   | array  | 位置数组（坐标0点为左上角）                                  |
| +++left            | 是   | number | 表示定位位置的长方形左上顶点的水平坐标                       |
| +++top             | 是   | number | 表示定位位置的长方形左上顶点的垂直坐标                       |
| +++width           | 是   | number | 表示定位定位位置的长方形的宽度                               |
| +++height          | 是   | number | 表示位置的长方形的高度                                       |
| ++char             | 是   | string | 单字符识别结果                                               |
| probability        | 否   | object | 行置信度信息；如果输入参数 probability = true 则输出         |
| + average          | 否   | number | 行置信度平均值                                               |
| + variance         | 否   | number | 行置信度方差                                                 |
| + min              | 否   | number | 行置信度最小值                                               |

**通用文字识别（含位置信息版） 返回示例**

```
{
"log_id": 3523983603,
"direction": 0, //detect_direction=true时存在
"words_result_num": 2,
"words_result": [
    {
        "location": {
            "left": 35,
            "top": 53,
            "width": 193,
            "height": 109
        },
        "words": "感动",
        "chars": [    //recognize_granularity=small时存在
            {
                "location": {
                    "left": 56,
                    "top": 65,
                    "width": 69,
                    "height": 88
                },
                "char": "感"
            },
            {
                "location": {
                    "left": 140,
                    "top": 65,
                    "width": 70,
                    "height": 88
                },
                "char": "动"
            }
        ]
    }
    ...
]
}
```

## 通用文字识别（含位置高精度版）

用户向服务请求识别某张图中的所有文字，并返回文字在图片中的坐标信息，相对于通用文字识别（含位置信息版）该产品精度更高，但是识别耗时会稍长。

```
public void sample(AipOcr client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("recognize_granularity", "big");
    options.put("detect_direction", "true");
    options.put("vertexes_location", "true");
    options.put("probability", "true");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.accurateGeneral(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.accurateGeneral(file, options);
    System.out.println(res.toString(2));
}
```

**通用文字识别（含位置高精度版） 请求参数详情**

| 参数名称              | 是否必选 | 类型   | 可选值范围                                    | 默认值 | 说明                                                         |
| :-------------------- | :------- | :----- | :-------------------------------------------- | :----- | :----------------------------------------------------------- |
| image                 | 是       | mixed  |                                               |        | 本地图片路径或者图片二进制数据                               |
| recognize_granularity | 否       | String | big - 不定位单字符位置 small - 定位单字符位置 | small  | 是否定位单字符位置，big：不定位单字符位置，默认值；small：定位单字符位置 |
| detect_direction      | 否       | String | true false                                    | false  | 是否检测图像朝向，默认不检测，即：false。朝向是指输入图像是正常方向、逆时针旋转90/180/270度。可选值包括: - true：检测朝向； - false：不检测朝向。 |
| vertexes_location     | 否       | String | true false                                    | false  | 是否返回文字外接多边形顶点位置，不支持单字位置。默认为false  |
| probability           | 否       | String | true false                                    |        | 是否返回识别结果中每一行的置信度                             |

**通用文字识别（含位置高精度版） 返回数据参数详情**

| 字段               | 必选 | 类型   | 说明                                                         |
| :----------------- | :--- | :----- | :----------------------------------------------------------- |
| direction          | 否   | number | 图像方向，当detect_direction=true时存在。 - -1:未定义， - 0:正向， - 1: 逆时针90度， - 2:逆时针180度， - 3:逆时针270度 |
| log_id             | 是   | number | 唯一的log id，用于问题定位                                   |
| words_result       | 是   | array  | 定位和识别结果数组                                           |
| words_result_num   | 是   | number | 识别结果数，表示words_result的元素个数                       |
| +vertexes_location | 否   | array  | 当前为四个顶点: 左上，右上，右下，左下。当vertexes_location=true时存在 |
| ++x                | 是   | number | 水平坐标（坐标0点为左上角）                                  |
| ++y                | 是   | number | 垂直坐标（坐标0点为左上角）                                  |
| +location          | 是   | array  | 位置数组（坐标0点为左上角）                                  |
| ++left             | 是   | number | 表示定位位置的长方形左上顶点的水平坐标                       |
| ++top              | 是   | number | 表示定位位置的长方形左上顶点的垂直坐标                       |
| ++width            | 是   | number | 表示定位位置的长方形的宽度                                   |
| ++height           | 是   | number | 表示定位位置的长方形的高度                                   |
| +words             | 否   | number | 识别结果字符串                                               |
| +chars             | 否   | array  | 单字符结果，recognize_granularity=small时存在                |
| ++location         | 是   | array  | 位置数组（坐标0点为左上角）                                  |
| +++left            | 是   | number | 表示定位位置的长方形左上顶点的水平坐标                       |
| +++top             | 是   | number | 表示定位位置的长方形左上顶点的垂直坐标                       |
| +++width           | 是   | number | 表示定位定位位置的长方形的宽度                               |
| +++height          | 是   | number | 表示位置的长方形的高度                                       |
| ++char             | 是   | string | 单字符识别结果                                               |
| probability        | 否   | object | 行置信度信息；如果输入参数 probability = true 则输出         |
| + average          | 否   | number | 行置信度平均值                                               |
| + variance         | 否   | number | 行置信度方差                                                 |
| + min              | 否   | number | 行置信度最小值                                               |

**通用文字识别（含位置高精度版） 返回示例**

```
{
"log_id": 3523983603,
"direction": 0, //detect_direction=true时存在
"words_result_num": 2,
"words_result": [
    {
        "location": {
            "left": 35,
            "top": 53,
            "width": 193,
            "height": 109
        },
        "words": "感动",
        "chars": [    //recognize_granularity=small时存在
            {
                "location": {
                    "left": 56,
                    "top": 65,
                    "width": 69,
                    "height": 88
                },
                "char": "感"
            },
            {
                "location": {
                    "left": 140,
                    "top": 65,
                    "width": 70,
                    "height": 88
                },
                "char": "动"
            }
        ]
    }
    ...
]
}
```

## 通用文字识别（含生僻字版）

某些场景中，图片中的中文不光有常用字，还包含了生僻字，这时用户需要对该图进行文字识别，应使用通用文字识别（含生僻字版）。

```
public void sample(AipOcr client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("language_type", "CHN_ENG");
    options.put("detect_direction", "true");
    options.put("detect_language", "true");
    options.put("probability", "true");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.enhancedGeneral(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.enhancedGeneral(file, options);
    System.out.println(res.toString(2));
    
    // 通用文字识别（含生僻字版）, 图片参数为远程url图片
    JSONObject res = client.enhancedGeneralUrl(url, options);
    System.out.println(res.toString(2));

}
```

**通用文字识别（含生僻字版） 请求参数详情**

| 参数名称         | 是否必选 | 类型   | 可选值范围                                  | 默认值  | 说明                                                         |
| :--------------- | :------- | :----- | :------------------------------------------ | :------ | :----------------------------------------------------------- |
| image            | 是       | mixed  |                                             |         | 本地图片路径或者图片二进制数据                               |
| url              | 是       | String |                                             |         | 图片完整URL，URL长度不超过1024字节，URL对应的图片base64编码后大小不超过4M，最短边至少15px，最长边最大4096px,支持jpg/png/bmp格式，当image字段存在时url字段失效 |
| language_type    | 否       | String | CHN_ENG ENG POR FRE GER ITA SPA RUS JAP KOR | CHN_ENG | 识别语言类型，默认为CHN_ENG。可选值包括： - CHN_ENG：中英文混合； - ENG：英文； - POR：葡萄牙语； - FRE：法语； - GER：德语； - ITA：意大利语； - SPA：西班牙语； - RUS：俄语； - JAP：日语； - KOR：韩语； |
| detect_direction | 否       | String | true false                                  | false   | 是否检测图像朝向，默认不检测，即：false。朝向是指输入图像是正常方向、逆时针旋转90/180/270度。可选值包括: - true：检测朝向； - false：不检测朝向。 |
| detect_language  | 否       | String | true false                                  | false   | 是否检测语言，默认不检测。当前支持（中文、英语、日语、韩语） |
| probability      | 否       | String | true false                                  |         | 是否返回识别结果中每一行的置信度                             |

**通用文字识别（含生僻字版） 返回数据参数详情**

| 字段             | 是否必选 | 类型    | 说明                                                         |
| :--------------- | :------- | :------ | :----------------------------------------------------------- |
| direction        | 否       | int32   | 图像方向，当detect_direction=true时存在。 - -1:未定义， - 0:正向， - 1: 逆时针90度， - 2:逆时针180度， - 3:逆时针270度 |
| log_id           | 是       | uint64  | 唯一的log id，用于问题定位                                   |
| words_result     | 是       | array() | 识别结果数组                                                 |
| words_result_num | 是       | uint32  | 识别结果数，表示words_result的元素个数                       |
| +words           | 否       | string  | 识别结果字符串                                               |
| probability      | 否       | object  | 识别结果中每一行的置信度值，包含average：行置信度平均值，variance：行置信度方差，min：行置信度最小值 |
| + average        | 否       | number  | 行置信度平均值                                               |
| + variance       | 否       | number  | 行置信度方差                                                 |
| + min            | 否       | number  | 行置信度最小值                                               |

**通用文字识别（含生僻字版） 返回示例**

```
{
"log_id": 2471272194,
"words_result_num": 2,
"words_result":
    [
        {"words": " TSINGTAO"},
        {"words": "青島睥酒"}
    ]
}
```

## 网络图片文字识别

用户向服务请求识别一些网络上背景复杂，特殊字体的文字。

```
public void sample(AipOcr client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("detect_direction", "true");
    options.put("detect_language", "true");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.webImage(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.webImage(file, options);
    System.out.println(res.toString(2));
    
    // 网络图片文字识别, 图片参数为远程url图片
    JSONObject res = client.webImageUrl(url, options);
    System.out.println(res.toString(2));

}
```

**网络图片文字识别 请求参数详情**

| 参数名称         | 是否必选 | 类型   | 可选值范围 | 默认值 | 说明                                                         |
| :--------------- | :------- | :----- | :--------- | :----- | :----------------------------------------------------------- |
| image            | 是       | mixed  |            |        | 本地图片路径或者图片二进制数据                               |
| url              | 是       | String |            |        | 图片完整URL，URL长度不超过1024字节，URL对应的图片base64编码后大小不超过4M，最短边至少15px，最长边最大4096px,支持jpg/png/bmp格式，当image字段存在时url字段失效 |
| detect_direction | 否       | String | true false | false  | 是否检测图像朝向，默认不检测，即：false。朝向是指输入图像是正常方向、逆时针旋转90/180/270度。可选值包括: - true：检测朝向； - false：不检测朝向。 |
| detect_language  | 否       | String | true false | false  | 是否检测语言，默认不检测。当前支持（中文、英语、日语、韩语） |

**网络图片文字识别 返回数据参数详情**

| 字段             | 是否必选 | 类型    | 说明                                                         |
| :--------------- | :------- | :------ | :----------------------------------------------------------- |
| direction        | 否       | number  | 图像方向，当detect_direction=true时存在。 - -1:未定义， - 0:正向， - 1: 逆时针90度， - 2:逆时针180度， - 3:逆时针270度 |
| log_id           | 是       | number  | 唯一的log id，用于问题定位                                   |
| words_result     | 是       | array() | 识别结果数组                                                 |
| words_result_num | 是       | number  | 识别结果数，表示words_result的元素个数                       |
| +words           | 否       | string  | 识别结果字符串                                               |
| probability      | 否       | object  | 识别结果中每一行的置信度值，包含average：行置信度平均值，variance：行置信度方差，min：行置信度最小值 |
| + average        | 否       | number  | 行置信度平均值                                               |
| + variance       | 否       | number  | 行置信度方差                                                 |
| + min            | 否       | number  | 行置信度最小值                                               |

**网络图片文字识别 返回示例**

```
{
"log_id": 2471272194,
"words_result_num": 2,
"words_result":
    [
        {"words": " TSINGTAO"},
        {"words": "青島睥酒"}
    ]
}
```

## 身份证识别

用户向服务请求识别身份证，身份证识别包括正面和背面。

```
public void sample(AipOcr client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("detect_direction", "true");
    options.put("detect_risk", "false");
    
    String idCardSide = "back";
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.idcard(image, idCardSide, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.idcard(file, idCardSide, options);
    System.out.println(res.toString(2));
}
```

**身份证识别 请求参数详情**

| 参数名称         | 是否必选 | 类型   | 可选值范围                                           | 默认值 | 说明                                                         |
| :--------------- | :------- | :----- | :--------------------------------------------------- | :----- | :----------------------------------------------------------- |
| image            | 是       | mixed  |                                                      |        | 本地图片路径或者图片二进制数据                               |
| id_card_side     | 是       | String | front - 身份证含照片的一面 back - 身份证带国徽的一面 |        | front：身份证含照片的一面；back：身份证带国徽的一面          |
| detect_direction | 否       | String | true false                                           | false  | 是否检测图像朝向，默认不检测，即：false。朝向是指输入图像是正常方向、逆时针旋转90/180/270度。可选值包括: - true：检测朝向； - false：不检测朝向。 |
| detect_risk      | 否       | String | true - 开启 false - 不开启                           |        | 是否开启身份证风险类型(身份证复印件、临时身份证、身份证翻拍、修改过的身份证)功能，默认不开启，即：false。可选值:true-开启；false-不开启 |

**身份证识别 返回数据参数详情**

| 字段             | 是否必选 | 类型          | 说明                                                         |
| :--------------- | :------- | :------------ | :----------------------------------------------------------- |
| direction        | 否       | number        | 图像方向，当detect_direction=true时存在。 - -1:未定义， - 0:正向， - 1: 逆时针90度， - 2:逆时针180度， - 3:逆时针270度 |
| image_status     | 是       | string        | normal-识别正常 reversed_side-未摆正身份证 non_idcard-上传的图片中不包含身份证 blurred-身份证模糊 over_exposure-身份证关键字段反光或过曝 unknown-未知状态 |
| risk_type        | 否       | string        | 输入参数 detect_risk = true 时，则返回该字段识别身份证类型: normal-正常身份证；copy-复印件；temporary-临时身份证；screen-翻拍；unknow-其他未知情况 |
| edit_tool        | 否       | string        | 如果参数 detect_risk = true 时，则返回此字段。如果检测身份证被编辑过，该字段指定编辑软件名称，如:Adobe Photoshop CC 2014 (Macintosh),如果没有被编辑过则返回值无此参数 |
| log_id           | 是       | number        | 唯一的log id，用于问题定位                                   |
| words_result     | 是       | array(object) | 定位和识别结果数组                                           |
| words_result_num | 是       | number        | 识别结果数，表示words_result的元素个数                       |
| +location        | 是       | array(object) | 位置数组（坐标0点为左上角）                                  |
| ++left           | 是       | number        | 表示定位位置的长方形左上顶点的水平坐标                       |
| ++top            | 是       | number        | 表示定位位置的长方形左上顶点的垂直坐标                       |
| ++width          | 是       | number        | 表示定位位置的长方形的宽度                                   |
| ++height         | 是       | number        | 表示定位位置的长方形的高度                                   |
| +words           | 否       | string        | 识别结果字符串                                               |

**身份证识别 返回示例**

```
{
    "log_id": 2648325511,
    "direction": 0,
    "image_status": "normal",
    "idcard_type": "normal",
    "edit_tool": "Adobe Photoshop CS3 Windows",
    "words_result": {
        "住址": {
            "location": {
                "left": 267,
                "top": 453,
                "width": 459,
                "height": 99
            },
            "words": "南京市江宁区弘景大道3889号"
        },
        "公民身份号码": {
            "location": {
                "left": 443,
                "top": 681,
                "width": 589,
                "height": 45
            },
            "words": "330881199904173914"
        },
        "出生": {
            "location": {
                "left": 270,
                "top": 355,
                "width": 357,
                "height": 45
            },
            "words": "19990417"
        },
        "姓名": {
            "location": {
                "left": 267,
                "top": 176,
                "width": 152,
                "height": 50
            },
            "words": "伍云龙"
        },
        "性别": {
            "location": {
                "left": 269,
                "top": 262,
                "width": 33,
                "height": 52
            },
            "words": "男"
        },
        "民族": {
            "location": {
                "left": 492,
                "top": 279,
                "width": 30,
                "height": 37
            },
            "words": "汉"
        }
    },
    "words_result_num": 6
}
```

## 银行卡识别

识别银行卡并返回卡号和发卡行。

```
public void sample(AipOcr client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.bankcard(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.bankcard(file, options);
    System.out.println(res.toString(2));
}
```

**银行卡识别 请求参数详情**

| 参数名称 | 是否必选 | 类型  | 说明                           |
| :------- | :------- | :---- | :----------------------------- |
| image    | 是       | mixed | 本地图片路径或者图片二进制数据 |

**银行卡识别 返回数据参数详情**

| 参数              | 类型   | 是否必须 | 说明                                         |
| :---------------- | :----- | :------- | :------------------------------------------- |
| log_id            | number | 是       | 请求标识码，随机数，唯一。                   |
| result            | object | 是       | 返回结果                                     |
| +bank_card_number | string | 是       | 银行卡卡号                                   |
| +bank_name        | string | 是       | 银行名，不能识别时为空                       |
| +bank_card_type   | number | 是       | 银行卡类型，0:不能识别; 1: 借记卡; 2: 信用卡 |

**银行卡识别 返回示例**

```
{
    "log_id": 1447188951,
    "result": {
        "bank_card_number": "622500000000000",
        "bank_name": "招商银行",
        "bank_card_type": 1
    }
}
```

## 驾驶证识别

对机动车驾驶证所有关键字段进行识别

```
public void sample(AipOcr client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("detect_direction", "true");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.drivingLicense(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.drivingLicense(file, options);
    System.out.println(res.toString(2));
}
```

**驾驶证识别 请求参数详情**

| 参数名称         | 是否必选 | 类型   | 可选值范围 | 默认值 | 说明                                                         |
| :--------------- | :------- | :----- | :--------- | :----- | :----------------------------------------------------------- |
| image            | 是       | mixed  |            |        | 本地图片路径或者图片二进制数据                               |
| detect_direction | 否       | String | true false | false  | 是否检测图像朝向，默认不检测，即：false。朝向是指输入图像是正常方向、逆时针旋转90/180/270度。可选值包括: - true：检测朝向； - false：不检测朝向。 |

**驾驶证识别 返回数据参数详情**

| 字段             | 是否必选 | 类型          | 说明                                   |
| :--------------- | :------- | :------------ | :------------------------------------- |
| log_id           | 是       | number        | 唯一的log id，用于问题定位             |
| words_result_num | 是       | number        | 识别结果数，表示words_result的元素个数 |
| words_result     | 是       | array(object) | 识别结果数组                           |
| +words           | 否       | string        | 识别结果字符串                         |

**驾驶证识别 返回示例**

```
{
  "errno": 0,
  "msg": "success",
  "data": {
    "words_result_num": 10,
    "words_result": {
      "证号": {
        "words": "3208231999053090"
      },
      "有效期限": {
        "words": "6年"
      },
      "准驾车型": {
        "words": "B2"
      },
      "有效起始日期": {
        "words": "20101125"
      },
      "住址": {
        "words": "江苏省南通市海门镇秀山新城"
      },
      "姓名": {
        "words": "小欧欧"
      },
      "国籍": {
        "words": "中国"
      },
      "出生日期": {
        "words": "19990530"
      },
      "性别": {
        "words": "男"
      },
      "初次领证日期": {
        "words": "20100125"
      }
    }
  }
}
```

## 行驶证识别

对机动车行驶证正本所有关键字段进行识别

```
public void sample(AipOcr client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("detect_direction", "true");
    options.put("accuracy", "normal");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.vehicleLicense(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.vehicleLicense(file, options);
    System.out.println(res.toString(2));
}
```

**行驶证识别 请求参数详情**

| 参数名称         | 是否必选 | 类型   | 可选值范围            | 默认值 | 说明                                                         |
| :--------------- | :------- | :----- | :-------------------- | :----- | :----------------------------------------------------------- |
| image            | 是       | mixed  |                       |        | 本地图片路径或者图片二进制数据                               |
| detect_direction | 否       | String | true false            | false  | 是否检测图像朝向，默认不检测，即：false。朝向是指输入图像是正常方向、逆时针旋转90/180/270度。可选值包括: - true：检测朝向； - false：不检测朝向。 |
| accuracy         | 否       | String | normal - 使用快速服务 |        | normal 使用快速服务，1200ms左右时延；缺省或其它值使用高精度服务，1600ms左右时延 |

**行驶证识别 返回数据参数详情**

| 字段             | 必选 | 类型          | 说明                                   |
| :--------------- | :--- | :------------ | :------------------------------------- |
| log_id           | 是   | number        | 唯一的log id，用于问题定位             |
| words_result_num | 是   | number        | 识别结果数，表示words_result的元素个数 |
| words_result     | 是   | array(object) | 识别结果数组                           |
| +words           | 否   | string        | 识别结果字符串                         |

**行驶证识别 返回示例**

```
{
  "errno": 0,
  "msg": "success",
  "data": {
    "words_result_num": 10,
    "words_result": {
      "品牌型号": {
        "words": "保时捷GT37182RUCRE"
      },
      "发证日期": {
        "words": "20160104"
      },
      "使用性质": {
        "words": "非营运"
      },
      "发动机号码": {
        "words": "20832"
      },
      "号牌号码": {
        "words": "苏A001"
      },
      "所有人": {
        "words": "圆圆"
      },
      "住址": {
        "words": "南京市江宁区弘景大道"
      },
      "注册日期": {
        "words": "20160104"
      },
      "车辆识别代号": {
        "words": "HCE58"
      },
      "车辆类型": {
        "words": "小型轿车"
      }
    }
  }
}
```

## 车牌识别

识别机动车车牌，并返回签发地和号牌。

```
public void sample(AipOcr client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("multi_detect", "true");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.plateLicense(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.plateLicense(file, options);
    System.out.println(res.toString(2));
}
```

**车牌识别 请求参数详情**

| 参数名称     | 是否必选 | 类型   | 可选值范围 | 默认值 | 说明                                                         |
| :----------- | :------- | :----- | :--------- | :----- | :----------------------------------------------------------- |
| image        | 是       | mixed  |            |        | 本地图片路径或者图片二进制数据                               |
| multi_detect | 否       | String | true false | false  | 是否检测多张车牌，默认为false，当置为true的时候可以对一张图片内的多张车牌进行识别 |

**车牌识别 返回数据参数详情**

| 参数   | 类型   | 是否必须 | 说明                       |
| :----- | :----- | :------- | :------------------------- |
| log_id | uint64 | 是       | 请求标识码，随机数，唯一。 |
| Color  | string | 是       | 车牌颜色                   |
| number | string | 是       | 车牌号码                   |

**车牌识别 返回示例**

```
{
    "log_id": 3583925545,
    "words_result": {
        "color": "blue",
        "number": "苏HS7766"
    }
}
```

## 营业执照识别

识别营业执照，并返回关键字段的值，包括单位名称、法人、地址、有效期、证件编号、社会信用代码等。

```
public void sample(AipOcr client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.businessLicense(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.businessLicense(file, options);
    System.out.println(res.toString(2));
}
```

**营业执照识别 请求参数详情**

| 参数名称 | 是否必选 | 类型  | 说明                           |
| :------- | :------- | :---- | :----------------------------- |
| image    | 是       | mixed | 本地图片路径或者图片二进制数据 |

**营业执照识别 返回数据参数详情**

| 参数             | 是否必须 | 类型          | 说明                                   |
| :--------------- | :------- | :------------ | :------------------------------------- |
| log_id           | 是       | number        | 请求标识码，随机数，唯一。             |
| words_result_num | 是       | number        | 识别结果数，表示words_result的元素个数 |
| words_result     | 是       | array(object) | 识别结果数组                           |
| left             | 是       | number        | 表示定位位置的长方形左上顶点的水平坐标 |
| top              | 是       | number        | 表示定位位置的长方形左上顶点的垂直坐标 |
| width            | 是       | number        | 表示定位位置的长方形的宽度             |
| height           | 是       | number        | 表示定位位置的长方形的高度             |
| words            | 否       | string        | 识别结果字符串                         |

**营业执照识别 返回示例**

```
{
    "log_id": 490058765,
    "words_result": {
        "单位名称": {
            "location": {
                "left": 500,
                "top": 479,
                "width": 618,
                "height": 54
            },
            "words": "袁氏财团有限公司"
        },
        "法人": {
            "location": {
                "left": 938,
                "top": 557,
                "width": 94,
                "height": 46
            },
            "words": "袁运筹"
        },
        "地址": {
            "location": {
                "left": 503,
                "top": 644,
                "width": 574,
                "height": 57
            },
            "words": "江苏省南京市中山东路19号"
        },
        "有效期": {
            "location": {
                "left": 779,
                "top": 1108,
                "width": 271,
                "height": 49
            },
            "words": "2015年02月12日"
        },
        "证件编号": {
            "location": {
                "left": 1219,
                "top": 357,
                "width": 466,
                "height": 39
            },
            "words": "苏餐证字(2019)第666602666661号"
        },
        "社会信用代码": {
            "location": {
                "left": 0,
                "top": 0,
                "width": 0,
                "height": 0
            },
            "words": "无"
        }
    },
    "words_result_num": 6
}
```

## 通用票据识别

用户向服务请求识别医疗票据、发票、的士票、保险保单等票据类图片中的所有文字，并返回文字在图中的位置信息。

```
public void sample(AipOcr client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("recognize_granularity", "big");
    options.put("probability", "true");
    options.put("accuracy", "normal");
    options.put("detect_direction", "true");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.receipt(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.receipt(file, options);
    System.out.println(res.toString(2));
}
```

**通用票据识别 请求参数详情**

| 参数名称              | 是否必选 | 类型   | 可选值范围                                    | 默认值 | 说明                                                         |
| :-------------------- | :------- | :----- | :-------------------------------------------- | :----- | :----------------------------------------------------------- |
| image                 | 是       | mixed  |                                               |        | 本地图片路径或者图片二进制数据                               |
| recognize_granularity | 否       | String | big - 不定位单字符位置 small - 定位单字符位置 | small  | 是否定位单字符位置，big：不定位单字符位置，默认值；small：定位单字符位置 |
| probability           | 否       | String | true false                                    |        | 是否返回识别结果中每一行的置信度                             |
| accuracy              | 否       | String | normal - 使用快速服务                         |        | normal 使用快速服务，1200ms左右时延；缺省或其它值使用高精度服务，1600ms左右时延 |
| detect_direction      | 否       | String | true false                                    | false  | 是否检测图像朝向，默认不检测，即：false。朝向是指输入图像是正常方向、逆时针旋转90/180/270度。可选值包括: - true：检测朝向； - false：不检测朝向。 |

**通用票据识别 返回数据参数详情**

| 字段             | 是否必选 | 类型    | 说明                                                         |
| :--------------- | :------- | :------ | :----------------------------------------------------------- |
| log_id           | 是       | number  | 唯一的log id，用于问题定位                                   |
| words_result_num | 是       | number  | 识别结果数，表示words_result的元素个数                       |
| words_result     | 是       | array() | 定位和识别结果数组                                           |
| location         | 是       | object  | 位置数组（坐标0点为左上角）                                  |
| left             | 是       | number  | 表示定位位置的长方形左上顶点的水平坐标                       |
| top              | 是       | number  | 表示定位位置的长方形左上顶点的垂直坐标                       |
| width            | 是       | number  | 表示定位位置的长方形的宽度                                   |
| height           | 是       | number  | 表示定位位置的长方形的高度                                   |
| words            | 是       | string  | 识别结果字符串                                               |
| chars            | 否       | array() | 单字符结果，recognize_granularity=small时存在                |
| location         | 是       | array() | 位置数组（坐标0点为左上角）                                  |
| left             | 是       | number  | 表示定位位置的长方形左上顶点的水平坐标                       |
| top              | 是       | number  | 表示定位位置的长方形左上顶点的垂直坐标                       |
| width            | 是       | number  | 表示定位定位位置的长方形的宽度                               |
| height           | 是       | number  | 表示位置的长方形的高度                                       |
| char             | 是       | string  | 单字符识别结果                                               |
| probability      | 否       | object  | 识别结果中每一行的置信度值，包含average：行置信度平均值，variance：行置信度方差，min：行置信度最小值 |

**通用票据识别 返回示例**

```
{
    "log_id": 2661573626,
    "words_result": [
        {
            "location": {
                "left": 10,
                "top": 3,
                "width": 121,
                "height": 24
            },
            "words": "姓名:小明明",
            "chars": [
                {
                    "location": {
                        "left": 16,
                        "top": 6,
                        "width": 17,
                        "height": 20
                    },
                    "char": "姓"
                }
                ...
            ]
        },
        {
            "location": {
                "left": 212,
                "top": 3,
                "width": 738,
                "height": 24
            },
            "words": "卡号/病案号:105353990标本编号:150139071送检科室:血液透析门诊病房",
            "chars": [
                {
                    "location": {
                        "left": 218,
                        "top": 6,
                        "width": 18,
                        "height": 21
                    },
                    "char": "卡"
                }
                ...
            ]
        }
    ],
    "words_result_num": 2
}
```

## 火车票识别

支持对大陆火车票的车票号、始发站、目的站、车次、日期、票价、席别、姓名进行结构化识别

```
public void sample(AipOcr client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.trainTicket(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.trainTicket(file, options);
    System.out.println(res.toString(2));
}
```

**火车票识别 请求参数详情**

| 参数名称 | 是否必选 | 类型  | 说明                           |
| :------- | :------- | :---- | :----------------------------- |
| image    | 是       | mixed | 本地图片路径或者图片二进制数据 |

**火车票识别 返回数据参数详情**

| 参数                | 类型   | 是否必须 | 说明                       |
| :------------------ | :----- | :------- | :------------------------- |
| log_id              | uint64 | 是       | 请求标识码，随机数，唯一。 |
| ticket_num          | string | 是       | 车票号                     |
| starting_station    | string | 是       | 始发站                     |
| train_num           | string | 是       | 车次号                     |
| destination_station | string | 是       | 到达站                     |
| date                | string | 是       | 出发日期                   |
| ticket_rates        | string | 是       | 车票金额                   |
| seat_category       | string | 是       | 席别                       |
| name                | string | 是       | 乘客姓名                   |

**火车票识别 返回示例**

```
{
  {
  "errno":"0",
  "logid":"55481588484",
  "date":"2017年12月11日",
  "destination_station":"嘉善南站",
  "name":"刘虎",
  "seat_category":"二等座",
  "starting_station":"上海虹桥站",
  "ticket_num":"Z13N025800",
  "ticket_rates":"￥24.0元",
  "train_num":"D3125"
  }
}
```

## 出租车票识别

针对出租车票（现支持北京）的发票号码、发票代码、车号、日期、时间、金额进行结构化识别

```
public void sample(AipOcr client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.taxiReceipt(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.taxiReceipt(file, options);
    System.out.println(res.toString(2));
}
```

**出租车票识别 请求参数详情**

| 参数名称 | 是否必选 | 类型  | 说明                           |
| :------- | :------- | :---- | :----------------------------- |
| image    | 是       | mixed | 本地图片路径或者图片二进制数据 |

**出租车票识别 返回数据参数详情**

| 参数             | 类型   | 是否必须 | 说明                       |
| :--------------- | :----- | :------- | :------------------------- |
| log_id           | uint64 | 是       | 请求标识码，随机数，唯一。 |
| words_result_num | uint32 | 是       |                            |
| Date             | string | 是       | 日期                       |
| Fare             | string | 是       | 实付金额                   |
| InvoiceCode      | string | 是       | 发票代号                   |
| InvoiceNum       | string | 是       | 发票号码                   |
| TaxiNum          | string | 是       | 车牌号                     |
| Time             | string | 是       | 上下车时间                 |

**出租车票识别 返回示例**

```
{
  "log_id":2034039896,
  "words_result_num":6,
  "words_result":
  {
    "Date":"2017-11-26",
    "Fare":"\u00a567.60",
    "InvoiceCode":"111001681009",
    "InvoiceNum":"90769610",
    "TaxiNum":"BV2062",
    "Time":"20:42-21:07"
  }
}
```

## 表格文字识别同步接口

自动识别表格线及表格内容，结构化输出表头、表尾及每个单元格的文字内容。

```
public void sample(AipOcr client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.form(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.form(file, options);
    System.out.println(res.toString(2));
}
```

**表格文字识别同步接口 请求参数详情**

| 参数名称 | 是否必选 | 类型  | 说明                           |
| :------- | :------- | :---- | :----------------------------- |
| image    | 是       | mixed | 本地图片路径或者图片二进制数据 |

**表格文字识别同步接口 返回数据参数详情**

| 字段             | 是否必选 | 类型          | 说明                       |
| :--------------- | :------- | :------------ | :------------------------- |
| log_id           | 是       | long          | 唯一的log id，用于问题定位 |
| forms_result_num | 是       | number        |                            |
| forms_result     | 是       | array(object) | 识别结果                   |

**表格文字识别同步接口 返回示例**

```
{
    "log_id": 3445697108,
    "forms_result_num": 1,
    "forms_result": [
        {
            "body": [
                {
                    "column": 0,
                    "probability": 0.99855202436447,
                    "row": 0,
                    "vertexes_location": [
                        {
                            "x": -2,
                            "y": 260
                        },
                        {
                            "x": 21,
                            "y": 244
                        },
                        {
                            "x": 35,
                            "y": 266
                        },
                        {
                            "x": 12,
                            "y": 282
                        }
                    ],
                    "words": "目"
                },
                {
                    "column": 3,
                    "probability": 0.99960500001907,
                    "row": 5,
                    "vertexes_location": [
                        {
                            "x": 603,
                            "y": 52
                        },
                        {
                            "x": 634,
                            "y": 32
                        },
                        {
                            "x": 646,
                            "y": 50
                        },
                        {
                            "x": 615,
                            "y": 71
                        }
                    ],
                    "words": "66"
                },
                {
                    "column": 3,
                    "probability": 0.99756097793579,
                    "row": 6,
                    "vertexes_location": [
                        {
                            "x": 634,
                            "y": 73
                        },
                        {
                            "x": 648,
                            "y": 63
                        },
                        {
                            "x": 657,
                            "y": 77
                        },
                        {
                            "x": 643,
                            "y": 86
                        }
                    ],
                    "words": "4"
                },
                {
                    "column": 3,
                    "probability": 0.96489900350571,
                    "row": 10,
                    "vertexes_location": [
                        {
                            "x": 699,
                            "y": 178
                        },
                        {
                            "x": 717,
                            "y": 167
                        },
                        {
                            "x": 727,
                            "y": 183
                        },
                        {
                            "x": 710,
                            "y": 194
                        }
                    ],
                    "words": "3,"
                },
                {
                    "column": 3,
                    "probability": 0.99809801578522,
                    "row": 14,
                    "vertexes_location": [
                        {
                            "x": 751,
                            "y": 296
                        },
                        {
                            "x": 786,
                            "y": 273
                        },
                        {
                            "x": 797,
                            "y": 289
                        },
                        {
                            "x": 761,
                            "y": 312
                        }
                    ],
                    "words": "206"
                }
            ],
            "footer": [
                {
                    "column": 0,
                    "probability": 0.99853301048279,
                    "row": 0,
                    "vertexes_location": [
                        {
                            "x": 605,
                            "y": 698
                        },
                        {
                            "x": 632,
                            "y": 680
                        },
                        {
                            "x": 643,
                            "y": 696
                        },
                        {
                            "x": 616,
                            "y": 714
                        }
                    ],
                    "words": "22"
                }
            ],
            "header": [
                {
                    "column": 0,
                    "probability": 0.94802802801132,
                    "row": 0,
                    "vertexes_location": [
                        {
                            "x": 183,
                            "y": 96
                        },
                        {
                            "x": 286,
                            "y": 29
                        },
                        {
                            "x": 301,
                            "y": 52
                        },
                        {
                            "x": 199,
                            "y": 120
                        }
                    ],
                    "words": "29月"
                }
            ],
            "vertexes_location": [
                {
                    "x": -154,
                    "y": 286
                },
                {
                    "x": 512,
                    "y": -153
                },
                {
                    "x": 953,
                    "y": 513
                },
                {
                    "x": 286,
                    "y": 953
                }
            ]
        }
    ]
}
```

## 表格文字识别

自动识别表格线及表格内容，结构化输出表头、表尾及每个单元格的文字内容。表格文字识别接口为异步接口，分为两个API：提交请求接口、获取结果接口。

```
public void sample(AipOcr client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.tableRecognitionAsync(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.tableRecognitionAsync(file, options);
    System.out.println(res.toString(2));
}
```

**表格文字识别 请求参数详情**

| 参数名称 | 是否必选 | 类型  | 说明                           |
| :------- | :------- | :---- | :----------------------------- |
| image    | 是       | mixed | 本地图片路径或者图片二进制数据 |

**表格文字识别 返回数据参数详情**

| 字段        | 是否必选 | 类型   | 说明                                                     |
| :---------- | :------- | :----- | :------------------------------------------------------- |
| log_id      | 是       | long   | 唯一的log id，用于问题定位                               |
| result      | 是       | list   | 返回的结果列表                                           |
| +request_id | 是       | string | 该请求生成的request_id，后续使用该request_id获取识别结果 |

**表格文字识别 返回示例**

```
{
    "result" : [
        {
            "request_id" : "1234_6789"
        }
    ],
    "log_id":149689853984104
}
```

失败应答示例（详细的错误码说明见本文档底部）：

```
{
    "log_id": 149319909347709,
    "error_code": 282000
    "error_msg":"internal error"
}
```

## 表格识别结果

获取表格文字识别结果

```
public void sample(AipOcr client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("result_type", "json");
    
    String requestId = "23454320-23255";
    
    // 表格识别结果
    JSONObject res = client.tableResultGet(requestId, options);
    System.out.println(res.toString(2));

}
```

**表格识别结果 请求参数详情**

| 参数名称    | 是否必选 | 类型   | 可选值范围 | 默认值 | 说明                                                         |
| :---------- | :------- | :----- | :--------- | :----- | :----------------------------------------------------------- |
| request_id  | 是       | String |            |        | 发送表格文字识别请求时返回的request id                       |
| result_type | 否       | String | json excel | excel  | 期望获取结果的类型，取值为“excel”时返回xls文件的地址，取值为“json”时返回json格式的字符串,默认为”excel” |

**表格识别结果 返回数据参数详情**

| 字段         | 是否必选 | 类型   | 说明                                                         |
| :----------- | :------- | :----- | :----------------------------------------------------------- |
| log_id       | 是       | long   | 唯一的log id，用于问题定位                                   |
| result       | 是       | object | 返回的结果                                                   |
| +result_data | 是       | string | 识别结果字符串，如果request_type是excel，则返回excel的文件下载地址，如果request_type是json，则返回json格式的字符串 |
| +percent     | 是       | int    | 表格识别进度（百分比）                                       |
| +request_id  | 是       | string | 该图片对应请求的request_id                                   |
| +ret_code    | 是       | int    | 识别状态，1：任务未开始，2：进行中,3:已完成                  |
| +ret_msg     | 是       | string | 识别状态信息，任务未开始，进行中,已完成                      |

**表格识别结果 返回示例**

成功应答示例：

```
{
    "result" : {
        "result_data" : "",
        "persent":100,
        "request_id": "149691317905102",
        "ret_code": 3
        "ret_msg": "已完成",
    },
    "log_id":149689853984104
}
```

当request_type为excel时，result_data格式样例为：

```
{
    "file_url":"https://ai.baidu.com/file/xxxfffddd"
}
```

当request_type为json时，result_data格式样例为：

```
{
    "form_num": 1,
    "forms": [
        {
            "header": [
                {
                "row": [
                    1
                ],
                "column": [
                    1,
                    2
                ],
                "word": "表头信息1",
            }
        ],
        "footer": [
            {
                "row": [
                    1
                ],
                "column": [
                    1,
                    2
                ],
                "word": "表尾信息1",
            }
        ],
        "body": [
            {
                "row": [
                    1
                ],
                "column": [
                    1,
                    2
                ],
                "word": "单元格文字",
            }
        ]
    }
]
}
```

其中各个参数的说明(json方式返回结果时)：

| 字段     | 是否必选 | 类型   | 说明                                   |
| :------- | :------- | :----- | :------------------------------------- |
| form_num | 是       | int    | 表格数量（可能一张图片中包含多个表格） |
| forms    | 是       | list   | 表格内容信息的列表                     |
| +header  | 是       | list   | 每个表格中，表头数据的相关信息         |
| +footer  | 是       | list   | 表尾的相关信息                         |
| +body    | 是       | list   | 表格主体部分的数据                     |
| ++row    | 是       | list   | 该单元格占据的行号                     |
| ++column | 是       | list   | 该单元格占据的列号                     |
| ++word   | 是       | string | 该单元格中的文字信息                   |

失败应答示例（详细的错误码说明见本文档底部）：

```
{
    "log_id": 149319909347709,
    "error_code": 282000
    "error_msg":"internal error"
}
```

## 增值税发票识别

此接口需要您在页面中提交合作咨询开通权限】 识别并结构化返回增值税发票的各个字段及其对应值，包含了发票基础信息9项，货物相关信息12项，购买方/销售方的名称、识别号、地址电话、开户行及账号，共29项结构化字段。

```
public void sample(AipOcr client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.vatInvoice(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.vatInvoice(file, options);
    System.out.println(res.toString(2));
}
```

**增值税发票识别 请求参数详情**

| 参数名称 | 是否必选 | 类型  | 说明                           |
| :------- | :------- | :---- | :----------------------------- |
| image    | 是       | mixed | 本地图片路径或者图片二进制数据 |

**增值税发票识别 返回数据参数详情**

| 字段                 | 是否必选 | 类型     | 说明                                   |
| :------------------- | :------- | :------- | :------------------------------------- |
| log_id               | 是       | uint64   | 唯一的log id，用于问题定位             |
| words_result_num     | 是       | uint32   | 识别结果数，表示words_result的元素个数 |
| words_result         | 是       | array()  | 识别结果数组                           |
| InvoiceType          | 是       | string   | 发票种类名称                           |
| InvoiceCode          | 是       | uint32   | 发票代码                               |
| InvoiceNum           | 是       | uint32   | 发票号码                               |
| InvoiceDate          | 是       | uint32   | 开票日期                               |
| TotalAmount          | 是       | uint32   | 合计金额                               |
| TotalTax             | 是       | string   | 合计税额                               |
| AmountInFiguers      | 是       | array()  | 价税合计(小写)                         |
| AmountInWords        | 是       | object   | 价税合计(大写)                         |
| SellerName           | 是       | uint32   | 销售方名称                             |
| SellerRegisterNum    | 是       | uint32   | 销售方纳税人识别号                     |
| PurchaserName        | 是       | uint32   | 购方名称                               |
| PurchaserRegisterNum | 是       | uint32   | 购方纳税人识别号                       |
| CommodityName        | 是       | object[] | 货物名称                               |
| -row                 | 是       | uint32   | 行号                                   |
| -word                | 是       | string   | 内容                                   |
| CommodityType        | 是       | object[] | 规格型号                               |
| -row                 | 是       | uint32   | 行号                                   |
| -word                | 是       | string   | 内容                                   |
| CommodityUnit        | 是       | object[] | 单位                                   |
| -row                 | 是       | uint32   | 行号                                   |
| -word                | 是       | string   | 内容                                   |
| CommodityNum         | 是       | object[] | 数量                                   |
| -row                 | 是       | uint32   | 行号                                   |
| -word                | 是       | string   | 内容                                   |
| CommodityPrice       | 是       | object[] | 单价                                   |
| -row                 | 是       | uint32   | 行号                                   |
| -word                | 是       | string   | 内容                                   |
| CommodityAmount      | 是       | object[] | 金额                                   |
| -row                 | 是       | uint32   | 行号                                   |
| -word                | 是       | string   | 内容                                   |
| CommodityTaxRate     | 是       | object[] | 税率                                   |
| -row                 | 是       | uint32   | 行号                                   |
| -word                | 是       | string   | 内容                                   |
| CommodityTax         | 是       | object[] | 税额                                   |
| -row                 | 是       | uint32   | 行号                                   |
| -word                | 是       | string   | 内容                                   |

**增值税发票识别 返回示例**

```
{
  "log_id": "5425496231209218858",
  "words_result_num": 29,
  "words_result": {
    "InvoiceNum": "14641426",
    "SellerName": "上海易火广告传媒有限公司",
    "CommodityTaxRate": [
      {
        "word": "6%",
        "row": "1"
      }
    ],
    "SellerBank": "中国银行南翔支行446863841354",
    "Checker": ":沈园园",
    "TotalAmount": "94339.62",
    "CommodityAmount": [
      {
        "word": "94339.62",
        "row": "1"
      }
    ],
    "InvoiceDate": "2016年06月02日",
    "CommodityTax": [
      {
        "word": "5660.38",
        "row": "1"
      }
    ],
    "PurchaserName": "百度时代网络技术(北京)有限公司",
    "CommodityNum": [
      {
        "word": "",
        "row": "1"
      }
    ],
    "PurchaserBank": "招商银行北京分行大屯路支行8661820285100030",
    "Remarks": "告传",
    "Password": "074/45781873408>/6>8>65*887676033/51+<5415>9/32--852>1+29<65>641-5>66<500>87/*-34<943359034>716905113*4242>",
    "SellerAddress": ":嘉定区胜辛南路500号15幢1161室55033753",
    "PurchaserAddress": "北京市海淀区东北旺西路8号中关村软件园17号楼二属A2010-59108001",
    "InvoiceCode": "3100153130",
    "CommodityUnit": [
      {
        "word": "",
        "row": "1"
      }
    ],
    "Payee": ":徐蓉",
    "PurchaserRegisterNum": "110108787751579",
    "CommodityPrice": [
      {
        "word": "",
        "row": "1"
      }
    ],
    "NoteDrawer": "沈园园",
    "AmountInWords": "壹拾万圆整",
    "AmountInFiguers": "100000.00",
    "TotalTax": "5660.38",
    "InvoiceType": "专用发票",
    "SellerRegisterNum": "913101140659591751",
    "CommodityName": [
      {
        "word": "信息服务费",
        "row": "1"
      }
    ],
    "CommodityType": [
      {
        "word": "",
        "row": "1"
      }
    ]
  }
}
```

## 二维码识别

【此接口需要您在[页面](http://ai.baidu.com/tech/ocr)中提交合作咨询开通权限识别条形码、二维码中包含的URL或其他信息内容

```
public void sample(AipOcr client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.qrcode(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.qrcode(file, options);
    System.out.println(res.toString(2));
}
```

**二维码识别 请求参数详情**

| 参数名称 | 是否必选 | 类型  | 说明                           |
| :------- | :------- | :---- | :----------------------------- |
| image    | 是       | mixed | 本地图片路径或者图片二进制数据 |

**二维码识别 返回数据参数详情**

| 字段             | 是否必选 | 类型    | 说明                                                         |
| :--------------- | :------- | :------ | :----------------------------------------------------------- |
| log_id           | 是       | uint64  | 唯一的log id，用于问题定位                                   |
| codes_result_num | 是       | uint32  | 识别结果数，表示codes_result的元素个数                       |
| codes_result     | 是       | array() | 定位和识别结果数组                                           |
| -type            | 是       | string  | 识别码类型条码类型包括：9种条形码（UPC_A、UPC_E、EAN_13、EAN_8、CODE_39、CODE_93、CODE_128、ITF、CODABAR），4种二维码（QR_CODE、DATA_MATRIX、AZTEC、PDF_417） |
| -text            | 是       | string  | 条形码识别内容,暂时只限于识别中英文结果                      |

**二维码识别 返回示例**

```
示例1:
{
    "log_id": 863402790,
    "codes_result": [
        {
            "type": "QR_CODE",
            "text": [
                "中国",
                "北京"
            ]
        }
    ],
    "codes_result_num": 1
}
示例2（多个图的情况）:
{
    "log_id": 1508509437,
    "codes_result": [
        {
            "type": "QR_CODE",
            "text": [
                "HTTP://Q8R.HK/YELZ0"
            ]
        },
        {
            "type": "PDF_417",
            "text": [
                "PDF417偼丄TL-30偱撉傒庢傝壜擻偱偡丅"
            ]
        },
        {
            "type": "CODABAR",
            "text": [
                "000800"
            ]
        },
        {
            "type": "CODE_39",
            "text": [
                "1234567890"
            ]
        },
        {
            "type": "AZTEC",
            "text": [
                "www.tec-it.com"
            ]
        },
        {
            "type": "DATA_MATRIX",
            "text": [
                "Wikipedia, the free encyclopedia"
            ]
        },
        {
            "type": "CODE_93",
            "text": [
                "123456789"
            ]
        },
        {
            "type": "CODE_128",
            "text": [
                "50090500019191"
            ]
        },
        {
            "type": "EAN_8",
            "text": [
                "12345670"
            ]
        },
        {
            "type": "EAN_13",
            "text": [
                "6901234567892"
            ]
        },
        {
            "type": "UPC_E",
            "text": [
                "01234565"
            ]
        }
    ],
    "codes_result_num": 11
}
```

## 数字识别

【此接口需要您在[页面](http://ai.baidu.com/tech/ocr)中提交合作咨询开通权限】对图像中的阿拉伯数字进行识别提取，适用于快递单号、手机号、充值码提取等场景

```
public void sample(AipOcr client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("recognize_granularity", "big");
    options.put("detect_direction", "true");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.numbers(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.numbers(file, options);
    System.out.println(res.toString(2));
}
```

**数字识别 请求参数详情**

| 参数名称              | 是否必选 | 类型   | 可选值范围                                    | 默认值 | 说明                                                         |
| :-------------------- | :------- | :----- | :-------------------------------------------- | :----- | :----------------------------------------------------------- |
| image                 | 是       | mixed  |                                               |        | 本地图片路径或者图片二进制数据                               |
| recognize_granularity | 否       | String | big - 不定位单字符位置 small - 定位单字符位置 | small  | 是否定位单字符位置，big：不定位单字符位置，默认值；small：定位单字符位置 |
| detect_direction      | 否       | String | true false                                    | false  | 是否检测图像朝向，默认不检测，即：false。朝向是指输入图像是正常方向、逆时针旋转90/180/270度。可选值包括: - true：检测朝向； - false：不检测朝向。 |

**数字识别 返回数据参数详情**

| 字段             | 是否必选 | 类型    | 说明                                          |
| :--------------- | :------- | :------ | :-------------------------------------------- |
| log_id           | 是       | uint64  | 唯一的log id，用于问题定位                    |
| words_result_num | 是       | uint32  | 识别结果数，表示words_result的元素个数        |
| words_result     | 是       | array() | 定位和识别结果数组                            |
| location         | 是       | object  | 位置数组（坐标0点为左上角）                   |
| left             | 是       | uint32  | 表示定位位置的长方形左上顶点的水平坐标        |
| top              | 是       | uint32  | 表示定位位置的长方形左上顶点的垂直坐标        |
| width            | 是       | uint32  | 表示定位位置的长方形的宽度                    |
| height           | 是       | uint32  | 表示定位位置的长方形的高度                    |
| words            | 是       | string  | 识别结果字符串                                |
| chars            | 否       | array() | 单字符结果，recognize_granularity=small时存在 |
| location         | 是       | array() | 位置数组（坐标0点为左上角）                   |
| left             | 是       | uint32  | 表示定位位置的长方形左上顶点的水平坐标        |
| top              | 是       | uint32  | 表示定位位置的长方形左上顶点的垂直坐标        |
| width            | 是       | uint32  | 表示定位定位位置的长方形的宽度                |
| height           | 是       | uint32  | 表示位置的长方形的高度                        |
| char             | 是       | string  | 单字符识别结果                                |

**数字识别 返回示例**

**参考通用文字识别（含位置信息版）**

## 彩票识别

【此接口需要您在[页面](http://ai.baidu.com/tech/ocr)中提交合作咨询开通权限】对大乐透、双色球彩票进行识别，并返回识别结果

```
public void sample(AipOcr client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("recognize_granularity", "big");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.lottery(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.lottery(file, options);
    System.out.println(res.toString(2));
}
```

**彩票识别 请求参数详情**

| 参数名称              | 是否必选 | 类型   | 可选值范围                                    | 默认值 | 说明                                                         |
| :-------------------- | :------- | :----- | :-------------------------------------------- | :----- | :----------------------------------------------------------- |
| image                 | 是       | mixed  |                                               |        | 本地图片路径或者图片二进制数据                               |
| recognize_granularity | 否       | String | big - 不定位单字符位置 small - 定位单字符位置 | small  | 是否定位单字符位置，big：不定位单字符位置，默认值；small：定位单字符位置 |

**彩票识别 返回数据参数详情**

| 字段             | 是否必选 | 类型    | 说明                                          |
| :--------------- | :------- | :------ | :-------------------------------------------- |
| log_id           | 是       | uint64  | 唯一的log id，用于问题定位                    |
| words_result_num | 是       | uint32  | 识别结果数，表示words_result的元素个数        |
| words_result     | 是       | array() | 定位和识别结果数组                            |
| location         | 是       | object  | 位置数组（坐标0点为左上角）                   |
| left             | 是       | uint32  | 表示定位位置的长方形左上顶点的水平坐标        |
| top              | 是       | uint32  | 表示定位位置的长方形左上顶点的垂直坐标        |
| width            | 是       | uint32  | 表示定位位置的长方形的宽度                    |
| height           | 是       | uint32  | 表示定位位置的长方形的高度                    |
| words            | 是       | string  | 识别结果字符串                                |
| chars            | 否       | array() | 单字符结果，recognize_granularity=small时存在 |
| location         | 是       | array() | 位置数组（坐标0点为左上角）                   |
| left             | 是       | uint32  | 表示定位位置的长方形左上顶点的水平坐标        |
| top              | 是       | uint32  | 表示定位位置的长方形左上顶点的垂直坐标        |
| width            | 是       | uint32  | 表示定位定位位置的长方形的宽度                |
| height           | 是       | uint32  | 表示位置的长方形的高度                        |
| char             | 是       | string  | 单字符识别结果                                |

**彩票识别 返回示例**

**参见通用文字识别返回示例**

## 护照识别

支持对中国大陆居民护照的资料页进行结构化识别，包含国家码、姓名、性别、护照号、出生日期、签发日期、有效期至、签发地点。

```
public void sample(AipOcr client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.passport(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.passport(file, options);
    System.out.println(res.toString(2));
}
```

**护照识别 请求参数详情**

| 参数名称 | 是否必选 | 类型  | 说明                           |
| :------- | :------- | :---- | :----------------------------- |
| image    | 是       | mixed | 本地图片路径或者图片二进制数据 |

**护照识别 返回数据参数详情**

| 字段             | 是否必选 | 类型    | 说明                                   |
| :--------------- | :------- | :------ | :------------------------------------- |
| log_id           | 是       | uint64  | 唯一的log id，用于问题定位             |
| words_result_num | 是       | uint32  | 识别结果数，表示words_result的元素个数 |
| words_result     | 是       | array() | 定位和识别结果数组                     |
| -location        | 是       | uint32  | 水平坐标                               |
| -words           | 是       | string  | 识别内容                               |

**护照识别 返回示例**

```
{
    "log_id": 6599970624325432166,
    "words_result_num": 8,
    "words_result": {
        "国家码": {
            "location": {
                "width": 127,
                "top": 200,
                "height": 35,
                "left": 947
            },
            "words": "CHN"
        },
        "护照签发地点": {
            "location": {
                "width": 340,
                "top": 803,
                "height": 48,
                "left": 663
            },
            "words": "江苏/JIANGSU"
        },
        "有效期至": {
            "location": {
                "width": 417,
                "top": 793,
                "height": 47,
                "left": 1312
            },
            "words": "078月/AUG2024"
        },
        "护照号码": {
            "location": {
                "width": 407,
                "top": 209,
                "height": 59,
                "left": 1366
            },
            "words": "E21202282"
        },
        "姓名": {
            "location": {
                "width": 243,
                "top": 327,
                "height": 56,
                "left": 663
            },
            "words": "袁运筹"
        },
        "签发日期": {
            "location": {
                "width": 388,
                "top": 659,
                "height": 46,
                "left": 1302
            },
            "words": "08月/AUG2014"
        },
        "生日": {
            "location": {
                "width": 327,
                "top": 528,
                "height": 49,
                "left": 1289
            },
            "words": "24NOV1990"
        },
        "性别": {
            "location": {
                "width": 102,
                "top": 541,
                "height": 45,
                "left": 656
            },
            "words": "男/M"
        }
    }
}
```

## 名片识别

【此接口需要您在[页面](http://ai.baidu.com/tech/ocr)中提交合作咨询开通权限】提供对各类名片的结构化识别功能，提取姓名、邮编、邮箱、电话、网址、地址、手机号字段

```
public void sample(AipOcr client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.businessCard(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.businessCard(file, options);
    System.out.println(res.toString(2));
}
```

**名片识别 请求参数详情**

| 参数名称 | 是否必选 | 类型  | 说明                           |
| :------- | :------- | :---- | :----------------------------- |
| image    | 是       | mixed | 本地图片路径或者图片二进制数据 |

**名片识别 返回数据参数详情**

| 字段             | 是否必选 | 类型    | 说明                                   |
| :--------------- | :------- | :------ | :------------------------------------- |
| log_id           | 是       | uint64  | 唯一的log id，用于问题定位             |
| words_result_num | 是       | uint32  | 识别结果数，表示words_result的元素个数 |
| words_result     | 是       | array() | 定位和识别结果数组                     |

**名片识别 返回示例**

```
{
    "errno": "0",
    "logid": "14815156840",
    "words_result_num": 7
    "words_result": {
            "ADDR": ["中国·北京东城区朝阳门北大街4848号"],
            "FAX": ["010-818480043"],
            "MOBILE": ["18284584483"],
            "NAME": ["圆圈圈"],
            "PC": ["100010"],
            "URL": ["www.baidu.comwww.baidu.com"],
            "TEL": ["010-89184841"]
        ...
    }
}
```

## 手写文字识别

对手写的中文汉字、数字进行识别

```
public void sample(AipOcr client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("recognize_granularity", "big");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.handwriting(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.handwriting(file, options);
    System.out.println(res.toString(2));
}
```

**手写文字识别 请求参数详情**

| 参数名称              | 是否必选 | 类型   | 可选值范围                                    | 默认值 | 说明                                                         |
| :-------------------- | :------- | :----- | :-------------------------------------------- | :----- | :----------------------------------------------------------- |
| image                 | 是       | mixed  |                                               |        | 本地图片路径或者图片二进制数据                               |
| recognize_granularity | 否       | String | big - 不定位单字符位置 small - 定位单字符位置 | small  | 是否定位单字符位置，big：不定位单字符位置，默认值；small：定位单字符位置 |

**手写文字识别 返回数据参数详情**

| 字段             | 是否必选 | 类型    | 说明                                                         |
| :--------------- | :------- | :------ | :----------------------------------------------------------- |
| log_id           | 是       | uint64  | 唯一的log id，用于问题定位                                   |
| words_result_num | 是       | uint32  | 识别结果数，表示words_result的元素个数                       |
| words_result     | 是       | array() | 定位和识别结果数组                                           |
| location         | 是       | object  | 位置数组（坐标0点为左上角）                                  |
| left             | 是       | uint32  | 表示定位位置的长方形左上顶点的水平坐标                       |
| top              | 是       | uint32  | 表示定位位置的长方形左上顶点的垂直坐标                       |
| width            | 是       | uint32  | 表示定位位置的长方形的宽度                                   |
| height           | 是       | uint32  | 表示定位位置的长方形的高度                                   |
| words            | 是       | string  | 识别结果字符串                                               |
| chars            | 否       | array() | 单字符结果，recognize_granularity=small时存在                |
| location         | 是       | array() | 位置数组（坐标0点为左上角）                                  |
| left             | 是       | uint32  | 表示定位位置的长方形左上顶点的水平坐标                       |
| top              | 是       | uint32  | 表示定位位置的长方形左上顶点的垂直坐标                       |
| width            | 是       | uint32  | 表示定位定位位置的长方形的宽度                               |
| height           | 是       | uint32  | 表示位置的长方形的高度                                       |
| char             | 是       | string  | 单字符识别结果                                               |
| probability      | 否       | object  | 识别结果中每一行的置信度值，包含average：行置信度平均值，variance：行置信度方差，min：行置信度最小值 |

**手写文字识别 返回示例**

```
{
    "log_id": 620759800,
    "words_result": [
        {
            "location": {
                "left": 56,
                "top": 0,
                "width": 21,
                "height": 210
            },
            "words": "3"
        }
    ],
    "words_result_num": 1
}
```

## 自定义模板文字识别

自定义模板文字识别，是针对百度官方没有推出相应的模板，但是当用户需要对某一类卡证/票据（如房产证、军官证、火车票等）进行结构化的提取内容时，可以使用该产品快速制作模板，进行识别。

```
public void sample(AipOcr client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("templateSign", "Nsdax2424asaAS791823112");
    options.put("classifierId", "31232");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.custom(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.custom(file, options);
    System.out.println(res.toString(2));
}
```

**自定义模板文字识别 请求参数详情**

| 参数名称     | 是否必选 | 类型   | 说明                                                         |
| :----------- | :------- | :----- | :----------------------------------------------------------- |
| image        | 是       | mixed  | 本地图片路径或者图片二进制数据                               |
| templateSign | 否       | String | 您在自定义文字识别平台制作的模板的ID                         |
| classifierId | 否       | String | 分类器Id。这个参数和templateSign至少存在一个，优先使用templateSign。存在templateSign时，表示使用指定模板；如果没有templateSign而有classifierId，表示使用分类器去判断使用哪个模板 |

**自定义模板文字识别 返回数据参数详情**

| 字段       | 是否必选   | 类型           | 说明                                                      |
| :--------- | :--------- | :------------- | :-------------------------------------------------------- |
| error_code | number     | number         | 0代表成功，如果有错误码返回可以参考下方错误码列表排查问题 |
| error_msg  | 是         | string         | 具体的失败信息，可以参考下方错误码列表排查问题            |
| data       | jsonObject | 识别返回的结果 |                                                           |

**自定义模板文字识别 返回示例**

```
{
    "isStructured": true,
    "ret": [
        {
            "charset": [
                {
                    "rect": {
                        "top": 183,
                        "left": 72,
                        "width": 14,
                        "height": 28
                    },
                    "word": "5"
                },
                {
                    "rect": {
                        "top": 183,
                        "left": 90,
                        "width": 14,
                        "height": 28
                    },
                    "word": "4"
                },
                {
                    "rect": {
                        "top": 183,
                        "left": 103,
                        "width": 15,
                        "height": 28
                    },
                    "word": "."
                },
                {
                    "rect": {
                        "top": 183,
                        "left": 116,
                        "width": 14,
                        "height": 28
                    },
                    "word": "5"
                },
                {
                    "rect": {
                        "top": 183,
                        "left": 133,
                        "width": 19,
                        "height": 28
                    },
                    "word": "元"
                }
            ],
            "word_name": "票价",
            "word": "54.5元"
        },
        {
            "charset": [
                {
                    "rect": {
                        "top": 144,
                        "left": 35,
                        "width": 14,
                        "height": 28
                    },
                    "word": "2"
                },
                {
                    "rect": {
                        "top": 144,
                        "left": 53,
                        "width": 14,
                        "height": 28
                    },
                    "word": "0"
                },
                {
                    "rect": {
                        "top": 144,
                        "left": 79,
                        "width": 14,
                        "height": 28
                    },
                    "word": "1"
                },
                {
                    "rect": {
                        "top": 144,
                        "left": 97,
                        "width": 14,
                        "height": 28
                    },
                    "word": "7"
                }
            ]
    ]
}
```

## 表格识别轮询接口

**代码示例**

调用表格识别请求，获取请求id之后轮询调用表格识别获取结果的接口

```
public void tableRecognition(AipOcr client) {
    //异步接口

    //使用封装的同步轮询接口
    JSONObject jsonres = client.tableRecognizeToJson(file, 20000);
    System.out.println(jsonres.toString(2));

    JSONObject excelres = client.tableRecognizeToExcelUrl(file, 20000);
    System.out.println(excelres.toString(2));

}
```

**请求参数**

| 参数               | 类型   | 描述                                            | 是否必须 |
| :----------------- | :----- | :---------------------------------------------- | :------- |
| imgPath/imgData    | byte[] | 图像文件路径或二进制数据                        | 是       |
| timeoutMiliseconds | long   | 最长等待时间，超时将返回错误, 一般任务在20s完成 | 是       |

**返回参数**

与**表格识别结果**接口返回相同

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
| 282000 | internal error                          | 服务器内部错误，如果您使用的是高精度接口，报这个错误码的原因可能是您上传的图片中文字过多，识别超时导致的，建议您对图片进行切割后再识别，其他情况请再次请求， 如果持续出现此类错误，请通过QQ群（631977213）或工单联系技术支持团队。 |
| 216100 | invalid param                           | 请求中包含非法参数，请检查后重新尝试                         |
| 216101 | not enough param                        | 缺少必须的参数，请检查参数是否有遗漏                         |
| 216102 | service not support                     | 请求了不支持的服务，请检查调用的url                          |
| 216103 | param too long                          | 请求中某些参数过长，请检查后重新尝试                         |
| 216110 | appid not exist                         | appid不存在，请重新核对信息是否为后台应用列表中的appid       |
| 216200 | empty image                             | 图片为空，请检查后重新尝试                                   |
| 216201 | image format error                      | 上传的图片格式错误，现阶段我们支持的图片格式为：PNG、JPG、JPEG、BMP，请进行转码或更换图片 |
| 216202 | image size error                        | 上传的图片大小错误，现阶段我们支持的图片大小为：base64编码后小于4M，分辨率不高于4096*4096，请重新上传图片 |
| 216630 | recognize error                         | 识别错误，请再次请求，如果持续出现此类错误，请通过QQ群（631977213）或工单联系技术支持团队。 |
| 216631 | recognize bank card error               | 识别银行卡错误，出现此问题的原因一般为：您上传的图片非银行卡正面，上传了异形卡的图片或上传的银行卡正品图片不完整 |
| 216633 | recognize idcard error                  | 识别身份证错误，出现此问题的原因一般为：您上传了非身份证图片或您上传的身份证图片不完整 |
| 216634 | detect error                            | 检测错误，请再次请求，如果持续出现此类错误，请通过QQ群（631977213）或工单联系技术支持团队。 |
| 282003 | missing parameters: {参数名}            | 请求参数缺失                                                 |
| 282005 | batch  processing error                 | 处理批量任务时发生部分或全部错误，请根据具体错误码排查       |
| 282006 | batch task  limit reached               | 批量任务处理数量超出限制，请将任务数量减少到10或10以下       |
| 282110 | urls not exit                           | URL参数不存在，请核对URL后再次提交                           |
| 282111 | url format illegal                      | URL格式非法，请检查url格式是否符合相应接口的入参要求         |
| 282112 | url download timeout                    | url下载超时，请检查url对应的图床/图片无法下载或链路状况不好，您可以重新尝试一下，如果多次尝试后仍不行，建议更换图片地址 |
| 282113 | url response invalid                    | URL返回无效参数                                              |
| 282114 | url size error                          | URL长度超过1024字节或为0                                     |
| 282808 | request id: xxxxx not exist             | request id xxxxx 不存在                                      |
| 282809 | result type error                       | 返回结果请求错误（不属于excel或json）                        |
| 282810 | image recognize error                   | 图像识别错误                                                 |