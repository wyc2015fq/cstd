# 图像审核  Java SDK文档

# 简介

Hi，您好，欢迎使用百度图像审核服务。

本文档主要针对Java开发者，描述百度图像审核接口服务的相关技术内容。如果您对文档内容有任何疑问，可以通过以下几种方式联系我们：

- 在百度云控制台内[提交工单](http://ticket.bce.baidu.com/#/ticket/create)，咨询问题类型请选择**人工智能服务**；
- 如有疑问，进入[AI社区交流](http://ai.baidu.com/forum/topic/list/172)：http://ai.baidu.com/forum/topic/list/172

## 接口能力

| 接口名称             | 接口能力简要描述                                             |
| :------------------- | :----------------------------------------------------------- |
| 图像审核             | 为用户提供色情识别、暴恐识别、政治敏感人物识别、广告识别、图像垃圾文本识别（反作弊）、恶心图像识别等一系列图像识别接口的一站式服务调用，并且支持用户在控制台中自定义配置所有接口的报警阈值和疑似区间，上传自定义文本黑库和敏感人物名单等。 |
| GIF色情识别          | 识别GIF图片的色情程度，返回图片的色情、性感、正常分值。      |
| 图像审核组合服务接口 | 提供一个API接口，同时调用多个模型服务。支持通用文字识别、网络图片文字识别、色情识别、暴恐识别、政治敏感识别、广告检测、恶心图像识别、图像质量检测一系列模型服务的调用。可通过入参灵活指定需要调用的模型服务，传入图片或图片URL，返回指定模型的识别结果。相比于图像审核接口，本接口只是对结果进行了简单的组合，json原样输出。 |
| 用户头像审核         | 通过人脸检测、文字识别、色情识别、暴恐识别、政治敏感识别、公众人物检测等多个维度，识别图片是否合规，直接返回审核结果，并支持自定义黑词库和审核规则配置。 |

## 版本更新记录

| 上线日期   | 版本号 | 更新内容                                                     |
| :--------- | :----- | :----------------------------------------------------------- |
| 2018.4.3   | 4.2.0  | 图像审核更名内容审核；新增文本审核接口                       |
| 2017.12.22 | 4.0.0  | 接口统一升级                                                 |
| 2017.10.18 | 3.2.1  | 使用proxy问题修复                                            |
| 2017.8.25  | 3.0.0  | 更新sdk打包方式：所有AI服务集成一个SDK                       |
| 2017.7.14  | 2.0.2  | 更新sdk打包方式                                              |
| 2017.6.16  | 2.0.1  | 新增头像审核、组合审核接口                                   |
| 2017.5.25  | 2.0.0  | 更名为图像审核SDK，新增暴恐识别接口，色情识别增加返回值结论包装 |
| 2017.4.13  | 1.3.2  | AI SDK同步版本更新                                           |
| 2017.3.23  | 1.3    | 新增GIF色情图像识别接口，兼容Android环境                     |
| 2017.3.2   | 1.2    | 上线对图片参数要求限制，增加设置超时接口                     |
| 2017.1.20  | 1.1    | 对部分云用户调用不成功的错误修复                             |
| 2017.1.6   | 1.0    | 初始版本，上线黄反接口                                       |

# 快速入门

## 安装ImageCensor Java SDK

**ImageCensor Java SDK目录结构**

```
com.baidu.aip
       ├── auth                                //签名相关类
       ├── http                                //Http通信相关类
       ├── client                              //公用类
       ├── exception                           //exception类
       ├── imagecensor
       │       └── AipImageCensor           //AipImageCensor类
       └── util                                //工具类
```

**支持 JAVA版本：1.7+**

**查看源码** Java SDK代码现已公开，您可以查看代码、或者在License范围内修改和编译SDK以适配您的环境。 github链接：https://github.com/Baidu-AIP/java-sdk

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

其中，`version`为版本号，添加完成后，用户就可以在工程中使用ImageCensor Java SDK。

## 新建AipImageCensor

AipImageCensor是图像审核的Java客户端，为使用图像审核的开发人员提供了一系列的交互方法。

用户可以参考如下代码新建一个AipImageCensor,初始化完成后建议**单例使用**,避免重复获取access_token：

```
public class Sample {
    //设置APPID/AK/SK
    public static final String APP_ID = "你的 App ID";
    public static final String API_KEY = "你的 Api Key";
    public static final String SECRET_KEY = "你的 Secret Key";

    public static void main(String[] args) {
        // 初始化一个AipImageCensor
        AipImageCensor client = new AipImageCensor(APP_ID, API_KEY, SECRET_KEY);

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
        JSONObject res = client.antiPorn(path);
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

## 配置AipFace

如果用户需要配置AipFace的一些细节参数，可以在构造AipFace之后调用接口设置参数，目前只支持以下参数：

| 接口                         | 说明                                                         |
| :--------------------------- | :----------------------------------------------------------- |
| setConnectionTimeoutInMillis | 建立连接的超时时间（单位：毫秒）                             |
| setSocketTimeoutInMillis     | 通过打开的连接传输数据的超时时间（单位：毫秒）               |
| setHttpProxy                 | 设置http代理服务器                                           |
| setSocketProxy               | 设置socket代理服务器 （http和socket类型代理服务器只能二选一） |

# 接口说明

## 色情识别

请调用替代的图像审核接口

## 暴恐识别

请调用替代的图像审核接口

## 图像审核接口

为用户提供色情识别、暴恐识别、政治敏感人物识别、广告识别、图像垃圾文本识别（反作弊）、恶心图像识别等一系列图像识别接口的一站式服务调用，并且支持用户在控制台中自定义配置所有接口的报警阈值和疑似区间，上传自定义文本黑库和敏感人物名单等。相比于组合服务接口，本接口除了支持自定义配置外，还对返回结果进行了总体的包装，按照用户在控制台中配置的规则直接返回是否合规，如果不合规则指出具体不合规的内容。

```
public void sample(AipContentCensor client) {
    // 参数为本地图片路径
    String path = "test.jpg";
    JSONObject response = client.imageCensorUserDefined(path, EimgType.FILE, null);
    System.out.println(response.toString());

	// 参数为url
	String url = "http://testurl";
    response = client.imageCensorUserDefined(url, EimgType.URL, null);
    System.out.println(response.toString());

    // 参数为本地图片文件二进制数组
    byte[] file = readImageFile(imagePath);
    response = client.imageCensorUserDefined(file, null);
    System.out.println(response.toString());
}
```

**请求参数详情**

| 参数   | 类型   | 是否必须 | 说明                                                         |
| :----- | :----- | :------- | :----------------------------------------------------------- |
| image  | string | N        | 待审核图片Base64编码字符串，以图片文件形式请求时必填。不能与imgUrl并存。 |
| imgUrl | string | N        | 网图URL地址，以网图形式请求，图片Url需要做UrlEncode。不能与image并存。 |

**返回数据参数详情**

| 字段         | 类型     | 是否必须 | 说明                                                         |
| :----------- | :------- | :------- | :----------------------------------------------------------- |
| log_id       | Long     | 是       | 请求唯一id                                                   |
| error_code   | uint64   | 否       | 错误提示码，失败才返回，成功不返回                           |
| error_msg    | string   | 否       | 错误提示信息，失败才返回，成功不返回                         |
| conclusion   | string   | 否       | 审核结果，成功才返回，失败不返回。可取值1.合规,2.疑似，3.不合规 |
| data         | object[] | 否       | 审核项详细信息，响应成功并且conclusion为疑似或不合规时才返回，响应失败或conclusion为合规是不返回。 |
| +error_code  | uint64   | 否       | 内层错误提示码，底层服务失败才返回，成功不返回               |
| +error_msg   | string   | 否       | 内层错误提示信息，底层服务失败才返回，成功不返回             |
| +type        | Integer  | 否       | 审核类型，1：色情、2：性感、3：暴恐、4:恶心、5：水印码、6：二维码、7：条形码、8：政治人物、9：敏感词、10：自定义敏感词 |
| +msg         | String   | 否       | 不合规项描述信息                                             |
| +probability | double   | 否       | 不合规项置信度                                               |
| +stars       | object[] | 否       | 政治人物列表数组，只有政治任务审核不通过才有                 |
| +words       | string   | 否       | 审核不通过敏感词，只有敏感词审核不通过才有                   |

**返回示例**

```
// 失败返回示例
{
    "log_id": 149319909347709,
    "error_code": 282800,
    "error_msg":"configId param is error or null"
}
// 成功返回示例-合规
{
    "log_id": 123456789,
    "conclusion": "合规"
}
// 成功返回示例-不合规
{
    "log_id": 123456789,
    "conclusion": "不合规",
    "data": [
        {
            "msg": "存在色情内容",
            "probability": 0.94308,
            "type": 1
        },
        {
            "msg": "存在性感内容",
            "probability": 0.94308,
            "type": 2
        },
        {
            "msg": "存在暴恐内容",
            "probability": 0.94308,
            "type": 3
        },
        {
        "msg": "存在恶心内容",
        "probability": 0.9688154,
        "type": 4
    },
        {
            "msg": "存在政治敏感内容",
            "stars": [
                {
                    "probability": 0.94308,
                    "name": "习近平"
                },
                {
                    "probability": 0.44308,
                    "name": "彭丽媛"
                }
            ],
            "type": 8
        },
        {
            "msg": "存在二维码内容",
            "probability": 0.94308,
            "type": 6
        },
        {
            "msg": "存在水印码内容",
            "probability": 0.94308,
            "type": 5
        },
        {
            "msg": "存在条形码内容",
            "probability": 0.94308,
            "type": 7
        },
        {
            "msg": "包含联系方式",
            "probability": 0.94308,
            "words": "包含联系方式",
            "type": 8
        }
    ]
}
```

## GIF色情图像识别

图片接受类型支持本地图片路径字符串，图片文件二进制数组，此接口只支持gif识别，若非gif接口，请使用色情识别接口。接口会对图片中每一帧进行识别，并返回所有检测结果中色情值最大的为结果。

```
public void antiPornGif(AipContentCensor client) {
    // 参数为本地图片路径
    String imagePath = "porn.gif";
    JSONObject response = client.antiPornGif(imagePath);
    System.out.println(response.toString());

    // 参数为本地图片文件二进制数组
    byte[] file = readImageFile(imagePath);  // 此函数仅为示例
    JSONObject response = client.antiPornGif(file);
    System.out.println(response.toString());
}
```

**GIF色情图像识别 请求参数详情**

| 参数  | 类型          | 描述                                               | 是否必须 |
| :---- | :------------ | :------------------------------------------------- | :------- |
| image | string/byte[] | 图像数据，支持本地图像文件路径，图像文件二进制数组 | 是       |

**GIF色情图像识别 访问限制**

| 检查项          | 限制条件              |
| :-------------- | :-------------------- |
| 图片格式        | gif                   |
| 每帧编码后大小  | < 4M                  |
| 帧数            | 不超过50              |
| GIF图片整体大小 | base64编码后不超过20M |

**GIF色情图像识别 返回数据参数详情**

| 参数                   | 类型   | 描述                                           |
| :--------------------- | :----- | :--------------------------------------------- |
| log_id                 | number | 请求标识码，随机数，唯一                       |
| conclusion             | string | 色情识别分类结论                               |
| confidence_coefficient | string | “conclusion”结论的置信度，值为"确定"或"不确定" |
| frame_count            | number | gif总帧数                                      |
| porn_probability       | number | 色情识别置信度                                 |
| result_num             | number | 返回结果数目，即：result数组中元素个数         |
| result                 | array  | 结果数组，每项内容对应一个分类维度的结果       |
| +class_name            | string | 分类结果名称，示例：色情                       |
| +probability           | number | 分类结果置信度，示例：0.89471650123596         |

**返回示例**

```
{
    "log_id": 1744190292,
    "frame_count": 9,
    "porn_probability":0.41608,
    "conclusion": "性感",
    "confidence_coefficient": "确定",
    "result_num": 3,
    "result": [
        {
            "class_name": "色情",
            "probability": 0.41608
        },
        {
            "class_name": "性感",
            "probability": 0.249851
        },
        {
            "class_name": "正常",
            "probability": 0.334069
        }
    ]
}
```

## 头像审核

**代码示例**

```
public void faceAudit(AipContentCensor client) {
    // 参数为本地图片路径
    String path1 = "test.jpg";
    String path2 = "test2.jpg";
    JSONObject response = client.faceAudit(Arrays.asList(path1, path2), EimgType.FILE, 1L);
    System.out.println(response.toString());

    // 参数为本地图片文件二进制数组
    byte[] file = readImageFile(imagePath);
    byte[][] data = new byte[1][];
    data[0] = file;
    JSONObject response = client.faceAudit(data, 1L);
    System.out.println(response.toString());
}
```

**请求参数**

| 参数     | 类型   | 描述                              | 是否必须 |
| :------- | :----- | :-------------------------------- | :------- |
| images   | mixed  | 待审核图片或者图片URL, 支持数组。 | 是       |
| configId | number | 审核配置组ID，不传则使用默认配置  | 否       |

**返回参数**

| 参数         | 类型   | 描述                                                         |
| :----------- | :----- | :----------------------------------------------------------- |
| error_code   | number | 错误码，错误才返回，成功不返回，具体错误码请参考下文         |
| error_msg    | string | 错误提示，错误才返回，成功不返回                             |
| result       | object | 识别结果集合，成功才返回，默认[]                             |
| +res_code    | number | 业务校验结果 0：校验通过，1：校验不通过                      |
| +res_msg     | object | 未校验通过的项,[]描述的是未校验通过的规则，具体参考 result中的res_msg业务错误码定义 |
| +error_code  | number | 底层服务调用异常码，正常不返回，异常才返回                   |
| +error_msg   | string | 底层服务调用提示信息，正常不返回，异常才返回                 |
| +data        | object | 识别详细结果数据，默认[]，data中的key含义： ocr:文字识别 face:人脸识别 star：明星人脸 public：公众人物 politicians:政治人物 antiporn：色情识别 terror：暴恐识别 |
| ++ocr        | object | 文字识别服务返回结果。请参照 [通用文字识别接口文档](http://ai.baidu.com/tech/ocr/general) |
| ++face       | object | 人脸识别服务返回结果。请参照[人脸检测接口文档](http://ai.baidu.com/tech/face/detect) |
| ++antiporn   | object | 色情识别返回结果。请参照[色情识别接口文档](http://ai.baidu.com/tech/imagecensoring/antiporn) |
| ++terror     | object | 暴恐识别返回结果。请参照[暴恐识别接口文档](http://ai.baidu.com/tech/imagecensoring/antiterror) |
| ++public     | object | 公众人物服务返回结果。 请参照下文，公众人物识别说明。        |
| ++politician | object | 政治人物返回结果。请参照下文，政治人物识别说明               |
| log_id       | number | 请求标识码。                                                 |

**返回示例**

```
{
   "log_id": 149319909347709,
    "result": [
        {
            "res_code": 0,
            "res_msg": [
                2001,
                2002,
                2003
            ],
            "error_code": 0,
            "error_msg":"iinternal request failed",
            "data": {
                "ocr": {
                    "log_id": 2471272194,
                    "words_result_num": 2,
                    "words_result": [
                        {
                            "words": " TSINGTAO"
                        },
                        {
                            "words": "青島睥酒"
                        }
                    ]
                },
                "face": {
                    "result_num": 1,
                    "result": [
                        {
                            "location": {
                                "left": 90,
                                "top": 92,
                                "width": 111,
                                "height": 99
                            },
                            "face_probability": 1,
                            "rotation_angle": 6,
                            "yaw": 11.61234664917,
                            "pitch": -0.30852827429771,
                            "roll": 8.8044967651367,
                            "landmark": [
                                {
                                    "x": 105,
                                    "y": 110
                                }
                            ],
                            "landmark72": [
                                {
                                    "x": 88,
                                    "y": 109
                                }
                            ],
                            "gender": "male",
                            "gender_probability": 0.99358034133911,
                            "glasses": 0,
                            "glasses_probability": 0.99991309642792,
                            "race": "yellow",
                            "race_probability": 0.99960690736771,
                            "qualities": {
                                "occlusion": {
                                    "left_eye": 0.000085282314103097,
                                    "right_eye": 0.00001094374601962,
                                    "nose": 3.2677664307812e-7,
                                    "mouth": 2.6582130940866e-10,
                                    "left_cheek": 8.752236624332e-8,
                                    "right_cheek": 1.0212766454742e-7,
                                    "chin": 4.2632994357028e-10
                                },
                                "blur": 4.5613666312237e-41,
                                "illumination": 0,
                                "completeness": 0,
                                "type": {
                                    "human": 0.98398965597153,
                                    "cartoon": 0.016010366380215
                                }
                            }
                        }
                    ],
                    "log_id": 2418894422
                },
                "antiporn": [
                    {
                        "class_name": "色情",
                        "probability": 0.014619
                    },
                    {
                        "class_name": "正常",
                        "probability": 0.171783
                    },
                    {
                        "class_name": "性感",
                        "probability": 0.813598
                    }
                ]
            }
        }
    ]
}
```

**政治人物识别说明**

错误返回值：

| 参数       | 类型   | 必须 | 说明                                                 |
| :--------- | :----- | :--- | :--------------------------------------------------- |
| error_code | number | 是   | 错误码，参考错误码表错误码说明。只在异常响应中出现   |
| error_msg  | string | 是   | 错误信息，参考错误码表错误码说明。只在异常响应中出现 |
| log_id     | number | 是   | 请求标识码，随机数，唯一                             |

正确返回值：

| 参数               | 类型   | 是否必须 | 说明                                                       |
| :----------------- | :----- | :------- | :--------------------------------------------------------- |
| log_id             | number | 是       | 日志id                                                     |
| include_politician | string | 是       | 是否包含政治人物，取值为"是"或"否"                         |
| result_confidence  | string | 是       | 对于include_politician字段的置信度，取值为“确定”或“不确定” |
| result_num         | number | 是       | 实际检测到人脸数目                                         |
| result             | array  | 是       |                                                            |
| +location          | object | 是       | 人脸在输入图片中的位置                                     |
| ++left             | number | 是       | 人脸区域离左边界的距离                                     |
| ++top              | number | 是       | 人脸区域离上边界的距离                                     |
| ++width            | number | 是       | 人脸区域的宽度                                             |
| ++height           | number | 是       | 人脸区域的高度                                             |
| +stars             | array  | 是       | 公众人物数组                                               |
| ++name             | string | 是       | 姓名                                                       |
| ++star_id          | string | 是       | 人物id,全局唯一                                            |
| ++probability      | number | 是       | 相似度，取值范围[0, 1]                                     |

返回值样例：

```
{
    "log_id": 3268660173,
    "include_politician": "是",
    "result_confidence": "确定",
    "result_num": 1,
    "result": [
        {
            "location": {
                "left": 132,
                "top": 168,
                "width": 238,
                "height": 223
            },
            "stars": [
                {
                    "name": "习近平",
                    "star_id": "515617",
                    "probability": 0.9750030040741
                }
            ]
        }
    ]
}
```

**公众人物识别说明**

错误返回值：

| 参数       | 类型   | 必须 | 说明                                                 |
| :--------- | :----- | :--- | :--------------------------------------------------- |
| error_code | number | 是   | 错误码，参考错误码表错误码说明。只在异常响应中出现   |
| error_msg  | string | 是   | 错误信息，参考错误码表错误码说明。只在异常响应中出现 |
| log_id     | number | 是   | 请求标识码，随机数，唯一                             |

正确返回值：

| 参数          | 类型   | 是否必须 | 说明                                   |
| :------------ | :----- | :------- | :------------------------------------- |
| result_num    | number | 是       | 实际检测到人脸数目(不大于max_face_num) |
| result        | array  | 是       | 识别出的结果数据                       |
| +location     | object | 是       | 人脸在输入图片中的位置                 |
| ++left        | number | 是       | 人脸区域离左边界的距离                 |
| ++top         | number | 是       | 人脸区域离上边界的距离                 |
| ++width       | number | 是       | 人脸区域的宽度                         |
| ++height      | number | 是       | 人脸区域的高度                         |
| +stars        | array  | 是       | 公众人物数组                           |
| ++name        | string | 是       | 姓名                                   |
| ++star_id     | string | 是       | 人物id，全局唯一                       |
| ++probability | number | 是       | 相似度，[0, 1]                         |
| log_id        | number | 是       | 请求标识码，随机数，唯一               |

返回值样例：

```
{
    "log_id": 3268660173,
    "result_num": 1,
    "result": [
        {
            "location": {
                "left": 132,
                "top": 168,
                "width": 238,
                "height": 223
            },
            "stars": [
                {
                    "name": "习近平",
                    "star_id": "515617",
                    "probability": 0.9750030040741
                }
            ]
        }
    ]
}
```

## 组合审核

**代码示例**

```
public void imageCensorComb(AipContentCensor client) {
    // 参数为本地图片路径
    String path = "test.jpg";
    JSONObject response = client.imageCensorComb(path, EimgType.FILE, Arrays.asList("antiporn", "terror", "disgust"), null);
    System.out.println(response.toString());

	// 参数为url
	String url = "http://testurl";
    JSONObject response = client.imageCensorComb(url, EimgType.URL, Arrays.asList("antiporn", "terror", "disgust"), null);
    System.out.println(response.toString());

    // 参数为本地图片文件二进制数组
    byte[] file = readImageFile(imagePath);
    JSONObject response = client.imageCensorComb(file, Arrays.asList("antiporn", "terror", "disgust"), null);
    System.out.println(response.toString());
}
```

**请求参数**

| 参数      | 类型   | 描述                                                         | 是否必须 |
| :-------- | :----- | :----------------------------------------------------------- | :------- |
| image     | string | 待审核图片或者URL。                                          | 是       |
| scenes    | array  | 服务类型。说明： key为要调用的服务类型，取值如下： 1、ocr：通用文字识别 2、public：公众人物识别 3、politician：政治人物识别 4、antiporn：色情识别 5、terror：暴恐识别。 6、webimage：网图OCR识别 7、disgust:恶心图 8、watermark:水印、二维码 | 是       |
| sceneConf | array  | jsonObject。 说明： key为要调用的服务类型，取值如下： 1、ocr：通用文字识别 2、public：公众人物识别 3、politician：政治人物识别 4、antiporn：色情识别 5、terror：暴恐识别。 6、webimage：网图OCR识别 7、disgust:恶心图 8、watermark:水印、二维码 value为各底层服务参数，请参照百度AI官网具体接口请参数 | 否       |

**sceneConf示例**

```
{
   "ocr":{
      "detect_direction":"false",
      "recognize_granularity":"big",
      "language_type":"CHN_ENG"
   }
}
```

**返回参数**

| 参数        | 类型   | 描述                                                         |
| :---------- | :----- | :----------------------------------------------------------- |
| result      | object | 返回结果json串，其内包含用户要调用的各个子服务的返回结果。   |
| +ocr        | object | 文字识别服务返回结果。请参照 [通用文字识别接口文档](http://ai.baidu.com/tech/ocr/general) |
| +face       | object | 人脸识别服务返回结果。请参照[人脸检测接口文档](http://ai.baidu.com/tech/face/detect) |
| +antiporn   | object | 色情识别返回结果。请参照[色情识别接口文档](http://ai.baidu.com/tech/imagecensoring/antiporn) |
| +terror     | object | 暴恐识别返回结果。请参照[暴恐识别接口文档](http://ai.baidu.com/tech/imagecensoring/antiterror) |
| +webimage   | object | 网络图片文字识别结果。请参照[网络图片文字识别接口文档](http://ai.baidu.com/tech/ocr/webimage) |
| +politician | object | 政治人物返回结果。                                           |
| +disgust    | object | 恶心图识别结果。                                             |
| +watermark  | object | 水印二维码识别返回结果。                                     |
| log_id      | number | 请求标识码。                                                 |

**返回示例**

```
{
    "result": {
        "antiporn": {
            "result": [
                {
                    "probability": 0.000126,
                    "class_name": "色情"
                },
                {
                    "probability": 0.000185,
                    "class_name": "性感"
                },
                {
                    "probability": 0.999689,
                    "class_name": "正常"
                }
            ],
            "log_id": 2476940655,
            "result_num": 3
        },
        "webimage": {
            "log_id": 2477072892,
            "words_result": [
                {
                    "words": "若心是走失的"
                },
                {
                    "words": "就连是坐看也像在流浪"
                }
            ],
            "words_result_num": 2,
            "direction": 0
        },
        "ocr": {
            "log_id": 2476966151,
            "words_result": [
                {
                    "words": "若心是表关自的"
                },
                {
                    "words": "규카坐看也在流"
                }
            ],
            "words_result_num": 2,
            "language": -1,
            "direction": 0
        }
    },
    "log_id": 149510767081507
}
```

**恶心图识别说明**

请求参数:

```
无额外参数
```

错误返回值：

| 参数       | 类型   | 必须 | 说明                                                 |
| :--------- | :----- | :--- | :--------------------------------------------------- |
| error_code | number | 是   | 错误码，参考错误码表错误码说明。只在异常响应中出现   |
| error_msg  | string | 是   | 错误信息，参考错误码表错误码说明。只在异常响应中出现 |
| log_id     | number | 是   | 请求标识码，随机数，唯一                             |

正确返回值：

| 参数   | 类型   | 是否必须 | 说明                     |
| :----- | :----- | :------- | :----------------------- |
| log_id | number | 是       | 请求标识码，随机数，唯一 |
| result | number | 是       | 得分                     |

返回值样例：

```
{
    "result": 0.000017522097550682,
    "log_id": 2239381760
}
```

**水印二维码识别说明**

请求参数:

```
无额外参数
```

错误返回值：

| 参数       | 类型   | 必须 | 说明                                                 |
| :--------- | :----- | :--- | :--------------------------------------------------- |
| error_code | number | 是   | 错误码，参考错误码表错误码说明。只在异常响应中出现   |
| error_msg  | string | 是   | 错误信息，参考错误码表错误码说明。只在异常响应中出现 |
| log_id     | number | 是   | 请求标识码，随机数，唯一                             |

正确返回值：

| 参数         | 类型   | 是否必须 | 说明                                                   |
| :----------- | :----- | :------- | :----------------------------------------------------- |
| result_num   | number | 是       | 返回结果数目，即：result数组中元素个数                 |
| result       | array  | 是       | 返回结果数组，每一项为一个检测出的结果                 |
| +probability | number | 是       | 分类结果置信度[0-1.0]                                  |
| +type        | string | 是       | 返回结果的类型 (watermark、bar code、QR code)          |
| +location    | object | 是       | 位置信息（左起像素位置、上起像素位置、像素宽、像素高） |
| ++left       | number | 是       | 信息区域离左边界的距离                                 |
| ++top        | number | 是       | 信息区域离上边界的距离                                 |
| ++width      | number | 是       | 信息区域的宽度                                         |
| ++height     | number | 是       | 信息区域的高度                                         |
| log_id       | number | 是       | 请求标识码，随机数，唯一                               |

返回值样例：

```
{
    "log_id": 2959788741,
    "result_num": 1,
    "result": [
        {
            "probability": 0.50555914640427,
            "location": {
                "left": 407,
                "top": 181,
                "width": 369,
                "height": 104
            },
            "type": "watermark"
        }
    ]
}
```

## 图像审核反馈接口

新增效果反馈接口，用于用户反馈模型的效果，用户必须至少反馈一个 true/false 来表示对该结果是否满意，同时可选择反馈详细的评价。

```
public void report(AipContentCensor client) {
    String path = "test.jpg";
    String content = "";
    try {
        byte[] imgData = Util.readFileByBytes(path);
        content = Base64Util.encode(imgData);
    } catch (IOException e) {
        e.printStackTrace();
    }
    JSONArray arr = new JSONArray();
    JSONObject obj = new JSONObject();
    obj.put("api_url", "https://aip.baidubce.com/rest/2.0/antiporn/v1/detect");
    obj.put("image", content);
    obj.put("correct", 1);
    obj.put("level", 1);
    arr.put(obj);
    JSONObject res = imageCensor.report(arr);
    System.out.println(res.toString(2));
```

**请求参数详情**

| 参数名    | 取值               | 是否必选                                | 说明                                                         |
| :-------- | :----------------- | :-------------------------------------- | :----------------------------------------------------------- |
| api_url   | 与数据库中取值一致 | Y                                       | 所调用的API                                                  |
| correct   | int                | Y                                       | 结果是否正确，正确：1，错误：0                               |
| image     | string             | N(image/image_md5/log_id三选一)         | 图像BASE64编码数据                                           |
| image_md5 | string             | N(image/image_md5/log_id三选一)         | 原请求的图像的MD5值                                          |
| log_id    | long               | N(image/image_md5/log_id三选一)         | 原请求返回的log_id                                           |
| level     | int                | N（该字段仅适用于色情识别结果反馈）     | 填写您认为的正确结果，色情：1，性感：2，正常：3，难以判断：4 |
| watermark | int                | N（该字段仅适用于广告检测结果反馈）     | 不包含水印：0， 包含：1                                      |
| qr_code   | int                | N（该字段仅适用于广告检测结果反馈）     | 不包含二维码：0， 包含：1                                    |
| bar_code  | int                | N（该字段仅适用于广告检测结果反馈）     | 不包含条形码：0， 包含：1                                    |
| clarity   | int                | N（该字段仅适用于图像质量检测结果反馈） | 0: 不清晰, 1: 清晰                                           |
| aesthetic | int                | N（该字段仅适用于图像质量检测结果反馈） | 0: 不美观，1：美观                                           |
| extra     | string             | N                                       | 备注，扩展字段                                               |

**返回示例**

```
{
  "log_id": 234232235,
  "result": [
    {
      "error_code": 0,
      //成功
      "error_msg": ""
    },
    {
      "error_code": 282004,
      // image_logid 或者 image_md5 或者 image 不存在
      // 三者必选其一
      "error_msg": "params error"
    },
    {
      "error_code": 0,
      "error_msg": ""
    }
  ]
}
```

## 文本审核

运用业界领先的深度学习技术，判断一段文本内容是否符合网络发文规范，实现自动化、智能化的文本审核。审核内容目前分为5大方向：色情文本、政治敏感、恶意推广、网络暴恐、低俗辱骂。v2正式版支持审核文本的黑白名单配置，且可通过调整阈值控制审核的松紧度标准，大幅度满足个性化文本内容审核的需求，详细配置说明详见AI官网社区说明帖。

```
public void sample(AipContentCensor client) {
    
    String content = "今天天气真好";
    JSONObject response = client.antiSpam(content, null);
    System.out.println(response.toString());
}
```

**文本审核 请求参数详情**

| 参数    | 类型   | 描述                        | 是否必须 |
| :------ | :----- | :-------------------------- | :------- |
| content | string | 待审核文本，不超过20000字节 | 是       |

**文本审核 返回数据参数详情**

| 参数     | 类型   | 描述                                                         |
| :------- | :----- | :----------------------------------------------------------- |
| logid    | number | 日志id，用于错误排查                                         |
| result   | object | 包含审核结果详情                                             |
| +spam    | number | 请求中是否包含违禁，0表示非违禁，1表示违禁，2表示建议人工复审 |
| +fail    | array  | 审核失败的类别列表与详情                                     |
| +pass    | array  | 正确调用生成的唯一标识码，用于问题定位                       |
| ++labels | array  | 请求中的违禁类型集合，一个或多个，审核通过则为空             |
| ++score  | number | 违禁检测分，范围0~1，数值从低到高代表风险程度的高低          |
| ++hit    | array  | 命中的违禁词集合，可能为空                                   |

**返回示例**

```
{
  "result": {
    "pass": [
      {
        "score": 0,
        "hit": [],
        "label": 1
      },
      {
        "score": 0.014999999664724,
        "hit": [],
        "label": 2
      },
      {
        "score": 0.23199999332428,
        "hit": [],
        "label": 3
      },
      {
        "score": 0.202097,
        "hit": [],
        "label": 4
      },
      {
        "score": 0.0089999996125698,
        "hit": [],
        "label": 5
      }
    ],
    "review": [],
    "reject": [],
    "spam": 0
  },
  "log_id": 3434243158103078465
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
| 216015 | module closed                           | 模块关闭                                                     |
| 216100 | invalid param                           | 非法参数                                                     |
| 216101 | not enough param                        | 参数数量不够                                                 |
| 216102 | service not support                     | 业务不支持                                                   |
| 216103 | param too long                          | 参数太长                                                     |
| 216110 | appid not exist                         | APP ID不存在                                                 |
| 216111 | invalid userid                          | 非法用户ID                                                   |
| 216200 | empty image                             | 空的图片                                                     |
| 216201 | image format error                      | 图片格式错误                                                 |
| 216202 | image size error                        | 图片大小错误                                                 |
| 216300 | db error                                | DB错误                                                       |
| 216400 | backend error                           | 后端系统错误                                                 |
| 216401 | internal error                          | 内部错误                                                     |
| 216500 | unknown error                           | 未知错误                                                     |
| 282000 | logic internal error                    | 业务逻辑层内部错误                                           |
| 282001 | logic backend error                     | 业务逻辑层后端服务错误                                       |
| 282002 | input encoding error                    | 请求参数编码错误                                             |
| 282100 | image transcode error                   | 图片压缩转码错误                                             |
| 282202 | antiporn detect timeout                 | 检测超时                                                     |
| 282203 | image frame size error                  | gif单帧大小超限                                              |
| 282204 | image frames limit error                | gif总帧数超限                                                |
| 282205 | image fromat must gif                   | 图片格式错误                                                 |
| 282801 | image and imgUrl are empty              | image和imgUrl均为空                                          |
| 282802 | either image and imgUrl has value       | image和imgUrl只能有一个有值                                  |
| 282804 | download image error                    | 图片下载失败                                                 |
| 282805 | internal request failed                 | 调用底层服务异常                                             |
| 282806 | image length or width error             | 图片宽高异常                                                 |