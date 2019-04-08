# 人体分析 Java SDK文档

# 简介

Hi，您好，欢迎使用百度人体分析服务。

本文档主要针对Java开发者，描述百度人体分析接口服务的相关技术内容。如果您对文档内容有任何疑问，可以通过以下几种方式联系我们：

- 在百度云控制台内[提交工单](http://ticket.bce.baidu.com/#/ticket/create)，咨询问题类型请选择**人工智能服务**；
- 如有疑问，进入[AI社区交流](http://ai.baidu.com/forum/topic/list/171)：http://ai.baidu.com/forum/topic/list/171* 具有免费调用额度的接口，超过每天的免费额度后会返回错误码：17，错误信息：Open api daily request limit reached（每天流量超限额），**可通过QQ群（860337848）联系群管、或提交工单手动提额**

## 接口能力

| 接口名称             | 接口能力简要描述                                             |
| :------------------- | :----------------------------------------------------------- |
| 人体关键点识别       | 检测图像中的所有人体并返回每个人体的矩形框位置，精准定位14个核心关键点，包含四肢、脖颈、鼻子等部位，更多关键点持续扩展中；支持多人检测、人体位置重叠、遮挡、背面、侧面、中低空俯拍、大动作等复杂场景 |
| 人体检测与属性识别   | 检测图像中的所有人体并返回每个人体的矩形框位置，识别人体的静态属性和行为，共支持20余种属性，包括：性别、年龄阶段、衣着（含类别/颜色）、是否戴帽子、是否戴眼镜、是否背包、是否使用手机、身体朝向等；支持中低空俯拍视角、人体重叠、遮挡、背面、侧面、动作变化等复杂场景 |
| 人流量统计           | 识别和统计图像当中的人体个数（静态统计，不支持追踪和去重），适用于3米以上的中远距离俯拍，以人头为主要识别目标统计人数，无需正脸、全身照，适应各类人流密集场景；默认识别整图中的人数，支持指定不规则区域的人数统计，同时可输出渲染图片 |
| 手势识别             | 识别图片中的手势类型，返回手势名称、手势矩形框、概率分数，可识别22种手势，支持动态手势识别，适用于手势特效、智能家居手势交互等场景；支持的手势列表：手指、掌心向前、拳头、OK、祈祷、作揖、作别、单手比心、点赞、diss、rock、掌心向上、双手比心（3种）、数字（7种） |
| 人像分割             | 识别人体的轮廓范围，与背景进行分离，适用于拍照背景替换、照片合成、身体特效等场景；输入正常人像图片，返回分割后的二值结果图，返回的二值图像需要进行二次处理才可查看分割效果 |
| 驾驶行为分析         | base64编码后大小不超过4M，最短边至少50px，最长边最大4096px ，建议长宽比3：1以内 |
| 人流量统计（动态版） | 动态人数统计和跟踪，主要适用于低空俯拍、出入口场景，以人体头肩为主要识别目标，核心功能：传入监控视频抓拍图片序列，进行人体追踪，根据目标轨迹判断进出区域行为，进行动态人数统计，返回区域进出人数。 |

## 版本更新记录

| 上线日期  | 版本号 | 更新内容                   |
| :-------- | :----- | :------------------------- |
| 2019.2.22 | 4.10.3 | 新增驾驶行为分析接口       |
| 2019.1.25 | 4.10.2 | 新增人流动态统计接口       |
| 2018.9.29 | 4.7.0  | 新增人像分割、手势识别接口 |
| 2018.6.8  | 4.4.0  | 新增人体分析服务           |

# 快速入门

## 安装BodyAnalysis Java SDK

**BodyAnalysis Java SDK目录结构**

```
com.baidu.aip
       ├── auth                                //签名相关类
       ├── http                                //Http通信相关类
       ├── client                              //公用类
       ├── exception                           //exception类
       ├── bodyanalysis
       │       └── AipBodyAnalysis           //AipBodyAnalysis类
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

4.添加SDK工具包`aip-java-sdk-version.jar`和第三方依赖工具包`json-20160810.jar``slf4j-api-1.7.25.jar` `slf4j-simple-1.7.25.jar（可选）`。

其中，`version`为版本号，添加完成后，用户就可以在工程中使用BodyAnalysis Java SDK。

## 新建AipBodyAnalysis

AipBodyAnalysis是人体分析的Java客户端，为使用人体分析的开发人员提供了一系列的交互方法。

用户可以参考如下代码新建一个AipBodyAnalysis,初始化完成后建议**单例使用**,避免重复获取access_token：

```
public class Sample {
    //设置APPID/AK/SK
    public static final String APP_ID = "你的 App ID";
    public static final String API_KEY = "你的 Api Key";
    public static final String SECRET_KEY = "你的 Secret Key";

    public static void main(String[] args) {
        // 初始化一个AipBodyAnalysis
        AipBodyAnalysis client = new AipBodyAnalysis(APP_ID, API_KEY, SECRET_KEY);

        // 可选：设置网络连接参数
        client.setConnectionTimeoutInMillis(2000);
        client.setSocketTimeoutInMillis(60000);

        // 可选：设置代理服务器地址, http和socket二选一，或者均不设置
        client.setHttpProxy("proxy_host", proxy_port);  // 设置http代理
        client.setSocketProxy("proxy_host", proxy_port);  // 设置socket代理

        // 调用接口
        String path = "test.jpg";
        JSONObject res = client.bodyAnalysis(path, new HashMap<String, String>());
        System.out.println(res.toString(2));
        
    }
}
```

在上面代码中，常量`APP_ID`在百度云控制台中创建，常量`API_KEY`与`SECRET_KEY`是在创建完毕应用后，系统分配给用户的，均为字符串，用于标识用户，为访问做签名验证，可在AI服务控制台中的**应用列表**中查看。

**注意**：如您以前是百度云的老用户，其中`API_KEY`对应百度云的“Access Key ID”，`SECRET_KEY`对应百度云的“Access Key Secret”。

## 配置AipBodyAnalysis

如果用户需要配置AipBodyAnalysis的一些细节参数，可以在构造AipBodyAnalysis之后调用接口设置参数，目前只支持以下参数：

| 接口                         | 说明                                                         |
| :--------------------------- | :----------------------------------------------------------- |
| setConnectionTimeoutInMillis | 建立连接的超时时间（单位：毫秒）                             |
| setSocketTimeoutInMillis     | 通过打开的连接传输数据的超时时间（单位：毫秒）               |
| setHttpProxy                 | 设置http代理服务器                                           |
| setSocketProxy               | 设置socket代理服务器 （http和socket类型代理服务器只能二选一） |

SDK默认使用slf4j-simple包进行日志输出，若用户需要使用自定义日志实现，可去除slf4j-simple依赖包，再额外添加相应的日志实现包即可。 maven去除slf4j-simple依赖包示例：

```
<dependency>
    <groupId>com.baidu.aip</groupId>
    <artifactId>java-sdk</artifactId>
    <version>${version}</version>
    <exclusions>
        <exclusion>
            <groupId>org.slf4j</groupId>
            <artifactId>slf4j-simple</artifactId>
        </exclusion>
    </exclusions>
</dependency>
```

# 接口说明

## 人体关键点识别

对于输入的一张图片（可正常解码，且长宽比适宜），**检测图片中的所有人体，输出每个人体的14个主要关键点，包含四肢、脖颈、鼻子等部位，以及人体的坐标信息和数量**。 支持多人检测、人体位置重叠、遮挡、背面、侧面、中低空俯拍、大动作等复杂场景。

14个关键点的位置：鼻子、脖子、左肩、右肩、左手肘、右手肘、左手腕、右手腕、左髋部、右髋部、左膝、右膝、左脚踝、右脚踝。示意图如下（**接口只输出关键点的坐标信息，不包含连线，如要实现“火柴人”的展现效果，需自行连线**），正在扩展更多关键点，敬请期待。

注：人体的坐标位置是根据关键点的坐标信息推算得出，当图片中人体有一定截断、遮挡时，也能识别出关键点，进而推算出人体的坐标位置。

单人场景：

![img](https://ai.bdstatic.com/file/F0AD8263C52146858AB1A283928E5D38)

多人场景：

![img](https://ai.bdstatic.com/file/B2304153BC5E4C57B6AA831F7CB7078D)

```
public void sample(AipBodyAnalysis client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.bodyAnalysis(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.bodyAnalysis(file, options);
    System.out.println(res.toString(2));
}
```

**人体关键点识别 请求参数详情**

| 参数名称 | 是否必选 | 类型  | 说明                           |
| :------- | :------- | :---- | :----------------------------- |
| image    | 是       | mixed | 本地图片路径或者图片二进制数据 |

**人体关键点识别 返回数据参数详情**

| 字段             | 是否必选 | 类型     | 说明                         |
| :--------------- | :------- | :------- | :--------------------------- |
| person_num       | 是       | uint32   | 人体数目                     |
| person_info      | 是       | object[] | 人体姿态信息                 |
| +body_parts      | 是       | object   | 身体部位信息，包含14个关键点 |
| ++left_ankle     | 是       | object   | 左脚踝                       |
| +++x             | 是       | float    | x坐标                        |
| +++y             | 是       | float    | y坐标                        |
| ++left_elbow     | 是       | object   | 左手肘                       |
| +++x             | 是       | float    | x坐标                        |
| +++y             | 是       | float    | y坐标                        |
| ++left_hip       | 是       | object   | 左髋部                       |
| +++x             | 是       | float    | x坐标                        |
| +++y             | 是       | float    | y坐标                        |
| ++left_knee      | 是       | object   | 左膝                         |
| +++x             | 是       | float    | x坐标                        |
| +++y             | 是       | float    | y坐标                        |
| ++left_shoulder  | 是       | object   | 左肩                         |
| +++x             | 是       | float    | x坐标                        |
| +++y             | 是       | float    | y坐标                        |
| ++left_wrist     | 是       | object   | 左手腕                       |
| +++x             | 是       | float    | x坐标                        |
| +++y             | 是       | float    | y坐标                        |
| ++neck           | 是       | object   | 颈                           |
| +++x             | 是       | float    | x坐标                        |
| +++y             | 是       | float    | y坐标                        |
| ++nose           | 是       | object   | 鼻子                         |
| +++x             | 是       | float    | x坐标                        |
| +++y             | 是       | float    | y坐标                        |
| ++right_ankle    | 是       | object   | 右脚踝                       |
| +++x             | 是       | float    | x坐标                        |
| +++y             | 是       | float    | y坐标                        |
| ++right_elbow    | 是       | object   | 右手肘                       |
| +++x             | 是       | float    | x坐标                        |
| +++y             | 是       | float    | y坐标                        |
| ++right_hip      | 是       | object   | 右髋部                       |
| +++x             | 是       | float    | x坐标                        |
| +++y             | 是       | float    | y坐标                        |
| ++right_knee     | 是       | object   | 右膝                         |
| +++x             | 是       | float    | x坐标                        |
| +++y             | 是       | float    | y坐标                        |
| ++right_shoulder | 是       | object   | 右肩                         |
| +++x             | 是       | float    | x坐标                        |
| +++y             | 是       | float    | y坐标                        |
| ++right_wrist    | 是       | object   | 右手腕                       |
| +++x             | 是       | float    | x坐标                        |
| +++y             | 是       | float    | y坐标                        |
| +location        | 是       | object   | 人体坐标信息                 |
| ++height         | 是       | float    | 人体区域的高度               |
| ++left           | 是       | float    | 人体区域离左边界的距离       |
| ++top            | 是       | float    | 人体区域离上边界的距离       |
| ++width          | 是       | float    | 人体区域的宽度               |
| log_id           | 是       | uint64   | 唯一的log id，用于问题定位   |

**人体关键点识别 返回示例**

```
{
  "person_num": 1,
  "person_info": [
    {
      "body_parts": {
        "neck": {
          "y": 288.7258911132812,
          "x": 845.4238891601562
        },
        "left_shoulder": {
          "y": 307.2262573242188,
          "x": 949.0001831054688
        },
        "left_knee": {
          "y": 582.1116333007812,
          "x": 568.9564819335938
        },
        "left_ankle": {
          "y": 593.6278076171875,
          "x": 421.5616149902344
        },
        "right_elbow": {
          "y": 0.0,
          "x": 0.0
        },
        "right_shoulder": {
          "y": 270.217529296875,
          "x": 744.0525512695312
        },
        "left_hip": {
          "y": 575.1278076171875,
          "x": 776.3110961914062
        },
        "right_hip": {
          "y": 505.7799377441406,
          "x": 700.2752075195312
        },
        "left_wrist": {
          "y": 586.6766357421875,
          "x": 1001.920715332031
        },
        "left_elbow": {
          "y": 468.8005676269531,
          "x": 965.2022094726562
        },
        "nose": {
          "y": 203.2225036621094,
          "x": 792.3795776367188
        },
        "right_ankle": {
          "y": 524.3399658203125,
          "x": 393.8475036621094
        },
        "right_knee": {
          "y": 339.5772399902344,
          "x": 591.977783203125
        },
        "right_wrist": {
          "y": 0.0,
          "x": 0.0
        }
      },
      "location": {
        "width": 212.8609619140625,
        "top": 105.8213119506836,
        "height": 532.152587890625,
        "left": 591.45361328125
      }
    }
  ],
  "log_id": 6900199721518695010
}
```

## 人体检测与属性识别

对于输入的一张图片（可正常解码，且长宽比适宜），**检测图像中的所有人体并返回每个人体的矩形框位置，识别人体的静态属性和行为，共支持20余种属性，包括：性别、年龄阶段、衣着（含类别/颜色）、是否戴帽子、是否戴眼镜、是否背包、是否使用手机、身体朝向等**。 **主要适用于监控场景的中低空斜拍视角**，支持人体轻度重叠、轻度遮挡、背面、侧面、动作变化等复杂场景。摄像头硬件选型无特殊要求，分辨率建议720p以上，更低分辨率的图片也能识别，只是效果可能有差异。**暂不适用夜间红外监控图片**，后续会考虑扩展。

注：接口返回的属性信息包括人体的遮挡、截断情况，在应用时可基于此过滤掉“无效人体”，比如严重遮挡、严重截断的人体。

人体检测的效果示例如下：

![img](https://ai.bdstatic.com/file/DB8F523BF5E84DD9A6568EF7F11FAA58)

非正常人体示例：严重截断

![img](https://ai.bdstatic.com/file/BF0E2AEC308C4CC5A920374934C250E3)

21种属性及其输出项如下，持续扩展更多属性，敬请期待。

| 序号 | 属性           | 接口字段           | 输出项说明                                                   |
| :--- | :------------- | :----------------- | :----------------------------------------------------------- |
| 1    | 性别           | gender             | 男性、女性、不确定                                           |
| 2    | 年龄阶段       | age                | 幼儿、青少年、青年、中年、老年、不确定                       |
| 3    | 上身服饰       | upper_wear         | 长袖、短袖、不确定                                           |
| 4    | 下身服饰       | lower_wear         | 长裤、短裤、长裙、短裙、不确定                               |
| 5    | 上身服饰颜色   | upper_color        | 红、橙、黄、绿、蓝、紫、粉、黑、白、灰、棕、不确定           |
| 6    | 下身服饰颜色   | lower_color        | 红、橙、黄、绿、蓝、紫、粉、黑、白、灰、棕、不确定           |
| 7    | 上身服饰纹理   | upper_wear_texture | 纯色、图案、碎花、条纹或格子、不确定                         |
| 8    | 背包           | bag                | 无背包、单肩包、双肩包、不确定                               |
| 9    | 上身服饰细分类 | upper_wear_fg      | T恤、无袖、衬衫、西装、毛衣、夹克、羽绒服、风衣、外套、不确定 |
| 10   | 是否戴帽子     | headwear           | 无帽、普通帽、安全帽、不确定                                 |
| 11   | 是否戴眼镜     | glasses            | 戴眼镜、戴墨镜、无眼镜、不确定                               |
| 12   | 是否撑伞       | umbrella           | 撑伞、无撑伞、不确定                                         |
| 13   | 是否使用手机   | cellphone          | 未使用手机、使用手机、不确定                                 |
| 14   | 身体朝向       | orientation        | 正面、背面、左侧面、右侧面、不确定                           |
| 15   | 是否吸烟       | smoke              | 吸烟、未吸烟、不确定                                         |
| 16   | 是否有手提物   | carrying_item      | 无手提物、有手提物、不确定                                   |
| 17   | 交通工具       | vehicle            | 无交通工具、骑摩托车、骑自行车、骑三轮车、不确定             |
| 18   | 上方截断       | upper_cut          | 无上方截断、有上方截断、不确定                               |
| 19   | 下方截断       | lower_cut          | 无下方截断、有下方截断、不确定                               |
| 20   | 遮挡           | occlusion          | 无遮挡、轻度遮挡、重度遮挡、不确定                           |
| 21   | 是否是正常人体 | is_human           | 非正常人体、正常人体、不确定；**正常人体**：身体露出大于二分之一的人体，一般以能看到腰部肚挤眼为标准；**非正常人体**：严重截断、严重遮挡的人体，一般是看不到肚挤眼的，比如只有个脑袋、一条腿 |

**注意：接口默认输出所有21个属性，如只需返回某几个特定属性，请将type 参数值设定属性可选值，用逗号分隔。**

```
public void sample(AipBodyAnalysis client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("type", "gender");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.bodyAttr(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.bodyAttr(file, options);
    System.out.println(res.toString(2));
}
```

**人体检测与属性识别 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 说明                                                         |
| :------- | :------- | :----- | :----------------------------------------------------------- |
| image    | 是       | mixed  | 本地图片路径或者图片二进制数据                               |
| type     | 否       | String | gender, age, lower_wear, upper_wear, headwear, glasses, upper_color, lower_color, cellphone, upper_wear_fg, upper_wear_texture, lower_wear_texture, orientation, umbrella, bag, smoke, vehicle, carrying_item, upper_cut, lower_cut, occlusion \| 1）可选值说明： gender-性别， age-年龄阶段， lower_wear-下身服饰， upper_wear-上身服饰， headwear-是否戴帽子， glasses-是否戴眼镜， upper_color-上身服饰颜色， lower_color-下身服饰颜色， cellphone-是否使用手机， upper_wear_fg-上身服饰细分类， upper_wear_texture-上身服饰纹理， orientation-身体朝向， umbrella-是否撑伞； bag-背包, smoke-是否吸烟, vehicle-交通工具, carrying_item-是否有手提物, upper_cut-上方截断, lower_cut-下方截断, occlusion-遮挡 2）type 参数值可以是可选值的组合，用逗号分隔；**如果无此参数默认输出全部20个属性** |

**人体检测与属性识别 返回数据参数详情**

| 字段                 | 是否必选 | 类型     | 说明                                                         |
| :------------------- | :------- | :------- | :----------------------------------------------------------- |
| person_info          | 是       | object[] | 人体姿态信息                                                 |
| +attributes          | 否       | object   | 人体属性内容                                                 |
| ++gender             | 否       | object   | 性别                                                         |
| +++name              | 否       | string   | 男性、女性、不确定                                           |
| +++score             | 否       | float    | 对应概率分数                                                 |
| ++age                | 否       | object   | 年龄阶段                                                     |
| +++name              | 否       | string   | 幼儿、青少年、青年、中年、老年、不确定                       |
| +++score             | 否       | float    | 对应概率分数                                                 |
| ++upper_color        | 否       | object   | 上半身衣着颜色                                               |
| +++name              | 否       | string   | 红、橙、黄、绿、蓝、紫、粉、黑、白、灰、棕、不确定           |
| +++score             | 否       | float    | 对应概率分数                                                 |
| ++lower_color        | 否       | object   | 下半身衣着颜色                                               |
| +++name              | 否       | string   | 红、橙、黄、绿、蓝、紫、粉、黑、白、灰、棕、不确定           |
| +++score             | 否       | float    | 对应概率分数                                                 |
| ++cellphone          | 否       | object   | 是否使用手机                                                 |
| +++name              | 否       | string   | 未使用手机、使用手机、不确定                                 |
| +++score             | 否       | float    | 对应概率分数                                                 |
| ++lower_wear         | 否       | object   | 下半身服饰                                                   |
| +++name              | 否       | string   | 长裤、短裤、长裙、短裙、不确定                               |
| +++score             | 否       | float    | 对应概率分数                                                 |
| ++upper_wear         | 否       | object   | 上半身服饰                                                   |
| +++name              | 否       | string   | 长袖、短袖、不确定                                           |
| +++score             | 否       | float    | 对应概率分数                                                 |
| ++headwear           | 否       | object   | 是否戴帽子                                                   |
| +++name              | 否       | string   | 无帽、普通帽、安全帽、不确定                                 |
| +++score             | 否       | float    | 对应概率分数                                                 |
| ++glasses            | 否       | object   | 是否戴眼镜                                                   |
| +++name              | 否       | string   | 戴眼镜、戴墨镜、无眼镜、不确定                               |
| +++score             | 否       | float    | 对应概率分数                                                 |
| ++upper_wear_fg      | 否       | object   | 上身服饰细分类                                               |
| +++name              | 否       | string   | T恤、无袖、衬衫、西装、毛衣、夹克、羽绒服、风衣、外套、不确定 |
| +++score             | 否       | float    | 对应概率分数                                                 |
| ++upper_wear_texture | 否       | object   | 上身服饰纹理                                                 |
| +++name              | 否       | string   | 纯色、图案、碎花、条纹或格子、不确定                         |
| +++score             | 否       | float    | 对应概率分数                                                 |
| ++orientation        | 否       | object   | 身体朝向                                                     |
| +++name              | 否       | string   | 正面、背面、侧面、不确定                                     |
| +++score             | 否       | float    | 对应概率分数                                                 |
| ++umbrella           | 否       | object   | 是否撑伞                                                     |
| +++name              | 否       | string   | 未撑伞、撑伞、不确定                                         |
| +++score             | 否       | float    | 对应概率分数                                                 |
| ++bag                | 否       | object   | 背包                                                         |
| +++name              | 否       | string   | 无背包、单肩包、双肩包、不确定                               |
| +++score             | 否       | float    | 对应概率分数                                                 |
| ++smoke              | 否       | object   | 是否吸烟                                                     |
| +++name              | 否       | string   | 未吸烟、吸烟、不确定                                         |
| +++score             | 否       | float    | 对应概率分数                                                 |
| ++vehicle            | 否       | object   | 交通工具                                                     |
| +++name              | 否       | string   | 无交通工具、骑摩托车、骑自行车、骑三轮车、不确定             |
| +++score             | 否       | float    | 对应概率分数                                                 |
| ++upper_cut          | 否       | object   | 上方截断                                                     |
| +++name              | 否       | string   | 无上方截断，有上方截断，不确定                               |
| +++score             | 否       | float    | 对应概率分数                                                 |
| ++lower_cut          | 否       | object   | 下方截断                                                     |
| +++name              | 否       | string   | 无下方截断，有下方截断，不确定                               |
| +++score             | 否       | float    | 对应概率分数                                                 |
| ++occlusion          | 否       | object   | 遮挡                                                         |
| +++name              | 否       | string   | 无遮挡，轻度遮挡，重度遮挡，不确定                           |
| +++score             | 否       | float    | 对应概率分数                                                 |
| ++is_human           | 否       | object   | 是否是正常人体                                               |
| +++name              | 否       | string   | 正常人体，非正常人体，不确定                                 |
| +++score             | 否       | float    | 对应概率分数                                                 |
| +location            | 是       | object   | 人体坐标信息                                                 |
| ++height             | 是       | float    | 人体区域的高度                                               |
| ++left               | 是       | float    | 人体区域离左边界的距离                                       |
| ++top                | 是       | float    | 人体区域离上边界的距离                                       |
| ++width              | 是       | float    | 人体区域的宽度                                               |
| person_num           | 是       | uint32   | **人体数目**                                                 |
| log_id               | 是       | uint64   | 唯一的log id，用于问题定位                                   |

**人体检测与属性识别 返回示例**

```
{
  "person_num": 1,
  "person_info":[
  {
    "attributes": {
      "upper_wear_fg": {
        "score": 0.6084373593330383,
        "name": "T恤"
      },
      "cellphone": {
        "score": 0.9986445307731628,
        "name": "未使用手机"
      },
      "lower_cut": {
        "score": 0.999593198299408,
        "name": "无下方截断"
      },
      "umbrella": {
        "score": 0.9999368190765381,
        "name": "未打伞"
      },
      "orientation": {
        "score": 0.6865364909172058,
        "name": "左侧面"
      },
      "headwear": {
        "score": 0.323924720287323,
        "name": "普通帽"
      },
      "gender": {
        "score": 0.8240132331848145,
        "name": "女性"
      },
      "age": {
        "score": 0.9495382308959961,
        "name": "青年"
      },
      "upper_cut": {
        "score": 0.9999241828918457,
        "name": "无上方截断"
      },
      "glasses": {
        "score": 0.9292197823524475,
        "name": "无眼镜"
      },
      "lower_color": {
        "score": 0.3373721539974213,
        "name": "黄"
      },
      "bag": {
        "score": 0.9770514369010925,
        "name": "无背包"
      },
      "upper_wear_texture": {
        "score": 0.7113875150680542,
        "name": "纯色"
      },
      "smoke": {
        "score": 0.9982085227966309,
        "name": "未吸烟"
      },
      "vehicle": {
        "score": 0.9907247424125671,
        "name": "无交通工具"
      },
      "lower_wear": {
        "score": 0.9739166498184204,
        "name": "短裤"
      },
      "carrying_item": {
        "score": 0.9735746383666992,
        "name": "无手提物"
      },
      "upper_wear": {
        "score": 0.7128473520278931,
        "name": "长袖"
      },
      "occlusion": {
        "score": 0.999269425868988,
        "name": "无遮挡"
      },
      "upper_color": {
        "score": 0.4870010614395142,
        "name": "白"
      },
      "is_human": {
        "score": 0.4870010614395142,
        "name": "正常人体"
      }
    },
    "location": {
      "height": 369,
      "width": 316,
      "top": 1,
      "score": 0.9756121635437012,
      "left": 58
    }
  }],
  "log_id": 6482262517986539034
}
```

## 人流量统计

对于输入的一张图片（可正常解码，且长宽比适宜），**识别和统计图像当中的人体个数（静态统计，暂不支持追踪和去重）**。 **适用于3米以上的中远距离俯拍，以头部为主要识别目标统计人数，无需正脸、全身照，适应各类人流密集场景（如：机场、车展、景区、广场等）**；默认识别整图中的人数，支持指定不规则区域的人数统计，同时可输出渲染图片。

摄像头硬件选型无特殊要求，分辨率建议720p以上，更低分辨率的图片也能识别，只是效果可能有差异。**暂不适用夜间红外监控图片**，后续会考虑扩展。

渲染图示意如下：

![img](https://ai.bdstatic.com/file/323B199A68444735B5396D0F79FA9C41)

注：**接口默认返回整张图片中的人数，如需统计特定框选区域的人数，请使用area参数添加识别区域坐标信息**。

```
public void sample(AipBodyAnalysis client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("area", "0,0,100,100,200,200");
    options.put("show", "false");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.bodyNum(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.bodyNum(file, options);
    System.out.println(res.toString(2));
}
```

**人流量统计 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 说明                                                         |
| :------- | :------- | :----- | :----------------------------------------------------------- |
| image    | 是       | mixed  | 本地图片路径或者图片二进制数据                               |
| area     | 否       | String | 特定框选区域坐标，逗号分隔，如‘x1,y1,x2,y2,x3,y3...xn,yn'，默认尾点和首点相连做闭合，**此参数为空或无此参数默认识别整个图片的人数** |
| show     | 否       | String | 是否输出渲染的图片，默认不返回，**选true时返回渲染后的图片(base64)**，其它无效值或为空则默认false |

**人流量统计 返回数据参数详情**

| 字段       | 是否必选 | 类型   |                                             |
| :--------- | :------- | :----- | :------------------------------------------ |
| person_num | 是       | uint64 | 识别出的人体数目                            |
| image      | 否       | string | 渲染后的图片，输入参数show=true时输出该字段 |

**人流量统计 返回示例**

```
{
    "log_id": 716033439,
    "person_num": 16,
    "image": "/9j/4AAoFS2P/9k="
}
```

## 手势识别

识别图片中的手势类型，返回手势名称、手势矩形框、概率分数，可识别24种手势，支持动态手势识别，适用于手势特效、智能家居手势交互等场景；支持的24类手势列表：拳头、OK、祈祷、作揖、作别、单手比心、点赞、Diss、我爱你、掌心向上、双手比心（3种）、数字（9种）、Rock、竖中指。 注：

1）上述24类以外的其他手势会划分到other类。

2）除识别手势外，若图像中检测到人脸，会同时返回人脸框位置。

可识别的24种手势示意图如下，**自拍场景、他人拍摄均适用**。

| 序号 | 手势名称            | classname      | 示例图                                                       |
| :--- | :------------------ | :------------- | :----------------------------------------------------------- |
| 1    | 数字1（原食指）     | One            | ![img](https://ai.bdstatic.com/file/49A55428683148D2B3A49E39862FDA84) |
| 2    | 数字5（原掌心向前） | Five           | ![img](https://ai.bdstatic.com/file/526729FA42864E30927B90839ECD9FF7) |
| 3    | 拳头                | Fist           | ![img](https://ai.bdstatic.com/file/E07EDA50F9154C549700D5974ECB0C5B) |
| 4    | OK                  | OK             | ![img](https://ai.bdstatic.com/file/68D278D304424922844DF9354D6FC302) |
| 5    | 祈祷                | Prayer         | ![img](https://ai.bdstatic.com/file/E8B21C5DEBD84BDD8D42860818084D87) |
| 6    | 作揖                | Congratulation | ![img](https://ai.bdstatic.com/file/42553BFA7954450E972BB659DAF58711) |
| 7    | 作别                | Honour         | ![img](https://ai.bdstatic.com/file/773582C20AF8486594D5D27BEB283459) |
| 8    | 单手比心            | Heart_single   | ![img](https://ai.bdstatic.com/file/6E1EDCD6A366474FA5AE4620CED6950C) |
| 9    | 点赞                | Thumb_up       | ![img](https://ai.bdstatic.com/file/2A7FD1F6F9CA44389478F982D0BF5C00) |
| 10   | Diss                | Thumb_down     | ![img](https://ai.bdstatic.com/file/A909595AE0DF4CA9A9768229777AA610) |
| 11   | Rock                | ILY            | ![img](https://ai.bdstatic.com/file/5C40E720A05043589ED0628B4C85ADE8) |
| 12   | 掌心向上            | Palm_up        | ![img](https://ai.bdstatic.com/file/E0AA97D41E48469CA19A50035F56E382) |
| 13   | 双手比心1           | Heart_1        | ![img](https://ai.bdstatic.com/file/8495BD1A7CDC4F9A87F445668F01D571) |
| 14   | 双手比心2           | Heart_2        | ![img](https://ai.bdstatic.com/file/3066188974234639A7367F07D3424216) |
| 15   | 双手比心3           | Heart_3        | ![img](https://ai.bdstatic.com/file/03BBFC09C1104CEF9A68591A566031F0) |
| 16   | 数字2               | two            | ![img](https://ai.bdstatic.com/file/84A5F4BB98484AF3A544E244B29D1B9A) |
| 17   | 数字3               | three          | ![img](https://ai.bdstatic.com/file/A9EACC02EBCB4C3E82D59A809FBE9E18) |
| 18   | 数字4               | four           | ![img](https://ai.bdstatic.com/file/BEA2ADCA77D64DB39BDC3EDA73F5CB8D) |
| 19   | 数字6               | six            | ![img](https://ai.bdstatic.com/file/5BF462823A174A76A1B0433DB60533B3) |
| 20   | 数字7               | seven          | ![img](https://ai.bdstatic.com/file/85426EE2C2914F28AB3518805788C84A) |
| 21   | 数字8               | eight          | ![img](https://ai.bdstatic.com/file/9A79F6E242B442288D734274F6F52E79) |
| 22   | 数字9               | nine           | ![img](https://ai.bdstatic.com/file/7FB1D639F5DD42F2BBCAFC8683D183D4) |
| 23   | Rock                | Rock           | ![img](https://ai.bdstatic.com/file/099D3FB301B140759CF2DDD2B6B4E780) |
| 24   | 竖中指              | Insult         | ![img](https://ai.bdstatic.com/file/3073D558422D426FABA5D1105115064C) |

```
public void sample(AipBodyAnalysis client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.gesture(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.gesture(file, options);
    System.out.println(res.toString(2));
}
```

**手势识别 请求参数详情**

| 参数名称 | 是否必选 | 类型  | 说明                           |
| :------- | :------- | :---- | :----------------------------- |
| image    | 是       | mixed | 本地图片路径或者图片二进制数据 |

**手势识别 返回数据参数详情**

| 字段         | 是否必选 | 类型     | 说明                                |
| :----------- | :------- | :------- | :---------------------------------- |
| result_num   | 是       | int      | 结果数量                            |
| result       | 是       | object[] | 检测到的目标，手势、人脸            |
| +classname   | 否       | string   | 目标所属类别，24种手势、other、face |
| +top         | 否       | int      | 目标框上坐标                        |
| +width       | 否       | int      | 目标框的宽                          |
| +left        | 否       | int      | 目标框最左坐标                      |
| +height      | 否       | int      | 目标框的高                          |
| +probability | 否       | float    | 目标属于该类别的概率                |
| log_id       | 是       | int64    | 唯一的log id，用于问题定位          |

**手势识别 返回示例**

```
{
  "log_id": 4466502370458351471,
  "result_num": 2,
  "result": [{
    "probability": 0.9844077229499817,
    "top": 20,
    "height": 156,
    "classname": "Face",
    "width": 116,
    "left": 173
  },
  {
    "probability": 0.4679304957389832,
    "top": 157,
    "height": 106,
    "classname": "Heart_2",
    "width": 177,
    "left": 183
  }]
}
```

## 人像分割

对于输入的一张图片（可正常解码，且长宽比适宜），**识别人体的轮廓范围，与背景进行分离，适用于拍照背景替换、照片合成、身体特效等场景。输入正常人像图片，返回分割后的二值结果图和分割类型（目前仅支持person）** 分割效果示意图：

1）原图

![img](https://ai.bdstatic.com/file/F64EFEFF3630474BA1F681544539C9FE)

2）二值图

![img](https://ai.bdstatic.com/file/74E9575413374D538FDA858E9348C272)

3）灰度图

![img](https://ai.bdstatic.com/file/7962AC3BFE76420A831371CE12ADB979)

4）前景人像图（透明背景）

![img](https://ai.bdstatic.com/file/5D1732181604442A80DCDDAC58491C77)

注：**返回的二值图像需要进行二次处理才可查看分割效果；灰度图和前景人像图不用处理，直接解码保存图片即可**。

```
public void sample(AipBodyAnalysis client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("type", "labelmap");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.bodySeg(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.bodySeg(file, options);
    System.out.println(res.toString(2));
}
```

**人像分割 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 说明                                                         |
| :------- | :------- | :----- | :----------------------------------------------------------- |
| image    | 是       | mixed  | 本地图片路径或者图片二进制数据                               |
| type     | 否       | String | 可以通过设置type参数，自主设置返回哪些结果图，避免造成带宽的浪费 1）可选值说明： labelmap - 二值图像，需二次处理方能查看分割效果 scoremap - 人像前景灰度图 foreground - 人像前景抠图，透明背景 2）type 参数值可以是可选值的组合，用逗号分隔；如果无此参数默认输出全部3类结果图 |

**人像分割 返回数据参数详情**

| 字段       | 是否必选 | 类型   | 说明                                                         |
| :--------- | :------- | :----- | :----------------------------------------------------------- |
| labelmap   | 否       | string | 分割结果图片，base64编码之后的二值图像，需二次处理方能查看分割效果 |
| scoremap   | 否       | string | 分割后人像前景的scoremap，归一到0-255，不用进行二次处理，直接解码保存图片即可。Base64编码后的灰度图文件，图片中每个像素点的灰度值 = 置信度 * 255，置信度为原图对应像素点位于人体轮廓内的置信度，取值范围[0, 1] |
| foreground | 否       | string | 分割后的人像前景抠图，透明背景，Base64编码后的png格式图片，不用进行二次处理，直接解码保存图片即可。将置信度大于0.5的像素抠出来，并通过image matting技术消除锯齿 |
| log_id     | 是       | int64  | 唯一的log id，用于问题定位                                   |

**人像分割 返回示例**

```
{
    "log_id": 716033439,
    "labelmap": "xxxx",
    "scoremap": "xxxx",
    "foreground": "xxxx"
}
```

## 驾驶行为分析

对于输入的一张车载监控图片（可正常解码，且长宽比适宜），**识别图像中是否有人体（驾驶员），若检测到至少1个人体，则进一步识别属性行为，可识别使用手机、抽烟、未系安全带、双手离开方向盘、视线未朝前方5种典型行为姿态**。 图片质量要求：

1、**服务只适用于车载司机场景，请使用驾驶室的真实监控图片测试，勿用网图、非车载场景的普通监控图片、或者乘客的监控图片测试**，否则效果不具备代表性。

2、车内摄像头硬件选型无特殊要求，分辨率建议720p以上，但更低分辨率的图片也能识别，只是效果可能有差异。

3、**车内摄像头部署方案建议：尽可能拍全驾驶员的身体**，并充分考虑背光、角度、方向盘遮挡等因素。

4、**暂不适用夜间红外监控图片**，后续会考虑扩展。

5、图片主体内容清晰可见，模糊、驾驶员遮挡严重、光线暗等情况下，识别效果肯定不理想。

示例图参考：

![img](https://ai.bdstatic.com/file/AF1FAD5FDD2148BD9B719557F78AE88A)

```
public void sample(AipBodyAnalysis client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("type", "smoke");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.driverBehavior(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.driverBehavior(file, options);
    System.out.println(res.toString(2));
}
```

**驾驶行为分析 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 说明                                                         |
| :------- | :------- | :----- | :----------------------------------------------------------- |
| image    | 是       | mixed  | 本地图片路径或者图片二进制数据                               |
| type     | 否       | String | smoke,cellphone, not_buckling_up, both_hands_leaving_wheel, not_facing_front |

**驾驶行为分析 返回数据参数详情**

| 字段                       | 是否必选 | 类型     | 说明                                                         |
| :------------------------- | :------- | :------- | :----------------------------------------------------------- |
| person_num                 | 是       | uint64   | 识别出的驾驶员检测框数目，0或者1，0代表未监测到驾驶员        |
| person_info                | 是       | object[] | 驾驶员的属性行为信息；若未检测到驾驶员，则该项为[]           |
| +location                  | 否       | object   | 检测出驾驶员的位置                                           |
| ++left                     | 否       | int      | 检测区域在原图的左起开始位置                                 |
| ++top                      | 否       | int      | 检测区域在原图的上起开始位置                                 |
| ++width                    | 否       | int      | 检测区域宽度                                                 |
| ++height                   | 否       | int      | 检测区域高度                                                 |
| +attributes                | 否       | object   | 驾驶员属性行为内容                                           |
| ++smoke                    | 否       | object   | 吸烟                                                         |
| +++score                   | 否       | float    | 对应概率分数                                                 |
| +++threshold               | 否       | float    | 建议阈值，仅作为参考，实际应用中根据测试情况选取合适的score阈值即可 |
| ++cellphone                | 否       | object   | 使用手机                                                     |
| +++score                   | 否       | float    | 对应概率分数                                                 |
| +++threshold               | 否       | float    | 建议阈值，仅作为参考，实际应用中根据测试情况选取合适的score阈值即可 |
| ++not_buckling_up          | 否       | object   | 未系安全带                                                   |
| +++score                   | 否       | float    | 对应概率分数                                                 |
| +++threshold               | 否       | float    | 建议阈值，仅作为参考，实际应用中根据测试情况选取合适的score阈值即可 |
| ++both_hands_leaving_wheel | 否       | object   | 双手离开方向盘                                               |
| +++score                   | 否       | float    | 对应概率分数                                                 |
| +++threshold               | 否       | float    | 建议阈值，仅作为参考，实际应用中根据测试情况选取合适的score阈值即可 |
| ++not_facing_front         | 否       | object   | 视角未朝前方                                                 |
| +++score                   | 否       | float    | 对应概率分数                                                 |
| +++threshold               | 否       | float    | 建议阈值，仅作为参考，实际应用中根据测试情况选取合适的score阈值即可 |

**驾驶行为分析 返回示例**

```
{
  "person_num": 1,
  "person_info": [{
    "attributes": {
      "cellphone": {
        "threshold": 0.9,
        "score": 0.500098466873169
      },
      "both_hands_leaving_wheel": {
        "threshold": 0.9,
        "score": 0.468360424041748
      },
      "not_facing_front": {
        "threshold": 0.9,
        "score": 0.08260071277618408
      },
      "not_buckling_up": {
        "threshold": 0.9,
        "score": 0.998087465763092
      },
      "smoke": {
        "threshold": 0.9,
        "score": 6.29425048828125e-05
      }
    },
    "location": {
      "width": 483,
      "top": 5,
      "height": 238,
      "left": 8
    }
  }],
  "log_id": 2320165720061799596
}
```

## 人流量统计-动态版

统计图像中的人体个数和流动趋势，主要适用于**低空俯拍、出入口场景，以人体头肩为主要识别目标**

```
public void sample(AipBodyAnalysis client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("case_id", "123");
    options.put("case_init", "123");
    options.put("show", "true");
    options.put("area", "100");
    
    String dynamic = "true";
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.bodyTracking(image, dynamic, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.bodyTracking(file, dynamic, options);
    System.out.println(res.toString(2));
}
```

**人流量统计-动态版 请求参数详情**

| 参数名称  | 是否必选 | 类型   | 可选值范围 | 默认值 | 说明                                                         |
| :-------- | :------- | :----- | :--------- | :----- | :----------------------------------------------------------- |
| image     | 是       | mixed  |            |        | 本地图片路径或者图片二进制数据                               |
| dynamic   | 是       | String | true false |        | true：动态人流量统计，返回总人数、跟踪ID、区域进出人数； false：静态人数统计，返回总人数 |
| case_id   | 否       | String |            |        | 任务ID（通过case_id区分不同视频流，自拟，不同序列间不可重复即可） |
| case_init | 否       | String | true false |        | 每个case的初始化信号，为true时对该case下的跟踪算法进行初始化，为false时重载该case的跟踪状态。当为false且读取不到相应case的信息时，直接重新初始化 |
| show      | 否       | String | true false | false  | 否返回结果图（含统计值和跟踪框渲染），默认不返回，选true时返回渲染后的图片(base64)，其它无效值或为空则默认false |
| area      | 否       | String |            |        | 静态人数统计时，只统计区域内的人，缺省时为全图统计。 动态人流量统计时，进出区域的人流会被统计。 逗号分隔，如‘x1,y1,x2,y2,x3,y3...xn,yn'，按顺序依次给出每个顶点的xy坐标（默认尾点和首点相连），形成闭合多边形区域。 服务会做范围（顶点左边需在图像范围内）及个数校验（数组长度必须为偶数，且大于3个顶点）。只支持单个多边形区域，建议设置矩形框，即4个顶点。**坐标取值不能超过图像宽度和高度，比如1280的宽度，坐标值最小建议从1开始，最大到1279**。 |

**area参数设置说明**

进出区域方向：从区域外走到区域内就是in，相反就是out，详见下方示例。

**示例1**： 如下图，area区域框三条边贴着图像左方边缘，从图像右方往左走到框里就是in，从图像左方往右走出框就是out，相当于只有图像中间那条线起作用。 如果想要从图像左方向右走是in，就把框画在图像右半部分，上、下、右三条边贴着图像边缘。

![img](https://ai.bdstatic.com/file/F90BB80793E34FA793CAB38F73D415D9)

同理，上下方向，如果area区域框三条边贴着图像下方边缘，从图像上方往下走到框里就是in，从图像下方往上走出框就是out，相当于只有图像中间那条线起作用。如果想要从图像下方向上走是in，就把框画在图像上半部分，上、左、右三条边贴着图像边缘。

**示例2**： 如下图，area区域是一个不规则多边形，将画面中门口以外的部分都框起来了，蓝色箭头的方向代表in，人从门外走进区域框里，红色箭头的方向代表out，人走出区域框，走向门外。

![img](https://ai.bdstatic.com/file/C5FF7C4789BB4650AE1496F8862A1977)

**人流量统计-动态版 返回数据参数详情**

| 字段         | 是否必选 | 类型     | 说明                                                         |
| :----------- | :------- | :------- | :----------------------------------------------------------- |
| person_num   | 是       | int      | 检测到的人体框数目                                           |
| person_info  | 否       | object[] | 每个框的具体信息                                             |
| +location    | 否       | object   | 跟踪到的人体框位置                                           |
| ++left       | 否       | int      | 人体框左坐标                                                 |
| ++top        | 否       | int      | 人体框顶坐标                                                 |
| ++width      | 否       | int      | 人体框宽度                                                   |
| ++height     | 否       | int      | 人体框高度                                                   |
| +ID          | 否       | int      | 人体的ID编号                                                 |
| person_count | 否       | object   | 进出区域的人流统计                                           |
| +in          | 否       | int      | 进入区域的人数                                               |
| +out         | 否       | int      | 离开区域的人数                                               |
| image        | 否       | string   | 结果图，含跟踪框和统计值（渲染jpg图片byte内容的base64编码，得到后先做base64解码再以字节流形式imdecode） |

**渲染结果图说明**

**画面里刚出现的人体头肩检测框都是红色，被跟踪锁定之后会变成其他颜色**（颜色随机，不同颜色没有特定规律），模型根据同颜色框的运动轨迹来判断进出移动方向；**人体被跟踪锁定后，检测框上方会出现人体的ID编号**，ID的取值逻辑为：每个case从1开始，不同人体向上递增但不一定连续。

**人流量统计-动态版 返回示例**

未检测到任何人：

```
    {
        "person_num":0,
        "person_info":[]
        “person_count”:
        {
            "in":0,
            "out":0
        }
    }
```

检测到2个人，无轨迹，无人进出区域：

```
    {
      "person_num":2,
      "person_info":[]
      “person_count”:
      {
          "in":0,
          "out":0
      }
    }
```

检测到2个人和2条轨迹，1人离开区域：

```
    {
      "person_num":2,
      "person_info":
       [
           {
              "ID":3
              "location":
              {
                  "left": 100,
                  "top": 200,
                  "width": 200,
                  "height": 400,
              }
          }
          {
              "ID": 5
              "location":
              {
                  "left": 400,
                  "top": 200,
                  "width": 200,
                  "height": 400,
              }
          }
      ]
      “person_count”:
      {
          "in":0,
          "out":1
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
| 1      | Unknown error                           | 服务器内部错误，请再次请求， 如果持续出现此类错误，请通过QQ群（860337848）或提交工单联系技术支持团队。 |
| 2      | Service temporarily unavailable         | 服务暂不可用，请再次请求， 如果持续出现此类错误，请通过QQ群（860337848）或提交工单联系技术支持团队。 |
| 3      | Unsupported openapi method              | 调用的API不存在，请检查请求URL后重新尝试，一般为URL中有非英文字符，如“-”，可手动输入重试 |
| 4      | Open api request limit reached          | 集群超限额，请再次请求， 如果持续出现此类错误，请通过QQ群（860337848）或提交工单联系技术支持团队。 |
| 6      | No permission to access data            | 无权限访问该用户数据，创建应用时未勾选相关接口               |
| 13     | Get service token failed                | 获取token失败                                                |
| 14     | IAM Certification failed                | IAM 鉴权失败                                                 |
| 15     | app not exsits or create failed         | 应用不存在或者创建失败                                       |
| 17     | Open api daily request limit reached    | 每天请求量超限额，可通过QQ群（860337848）联系群管、提交工单提升限额 |
| 18     | Open api qps request limit reached      | QPS超限额，可通过QQ群（860337848）联系群管、提交工单提升限额 |
| 19     | Open api total request limit reached    | 请求总量超限额，可通过QQ群（860337848）联系群管、提交工单提升限额 |
| 100    | Invalid parameter                       | 无效的access_token参数，请检查后重新尝试                     |
| 110    | Access token invalid or no longer valid | access_token无效                                             |
| 111    | Access token expired                    | access token过期                                             |
| 282000 | internal error                          | 服务器内部错误，请再次请求， 如果持续出现此类错误，请通过QQ群（860337848）或提交工单联系技术支持团队。 |
| 216100 | invalid param                           | 请求中包含非法参数，请检查后重新尝试                         |
| 216101 | not enough param                        | 缺少必须的参数，请检查参数是否有遗漏                         |
| 216102 | service not support                     | 请求了不支持的服务，请检查调用的url                          |
| 216103 | param too long                          | 请求中某些参数过长，请检查后重新尝试                         |
| 216110 | appid not exist                         | appid不存在，请重新核对信息是否为后台应用列表中的appid       |
| 216200 | empty image                             | 图片为空，请检查后重新尝试                                   |
| 216201 | image format error                      | 上传的图片格式错误，现阶段我们支持的图片格式为：PNG、JPG、BMP，请进行转码或更换图片 |
| 216202 | image size error                        | 上传的图片大小错误，现阶段我们支持的图片大小为：base64编码后小于4M，分辨率不高于4096 * 4096，请重新上传图片 |
| 216203 | image size error                        | 上传的图片base64编码有误，请校验base64编码方式，并重新上传图片 |
| 216630 | recognize error                         | 识别错误，请再次请求，如果持续出现此类错误，请通过QQ群（860337848）或提交工单联系技术支持团队。 |
| 216634 | detect error                            | 检测错误，请再次请求，如果持续出现此类错误，请通过QQ群（860337848）或提交工单联系技术支持团队。 |
| 282003 | missing parameters: {参数名}            | 请求参数缺失                                                 |
| 282005 | batch  processing error                 | 处理批量任务时发生部分或全部错误，请根据具体错误码排查       |
| 282006 | batch task  limit reached               | 批量任务处理数量超出限制，请将任务数量减少到10或10以下       |
| 282114 | url size error                          | URL长度超过1024字节或为0                                     |
| 282808 | request id: xxxxx not exist             | request id xxxxx 不存在                                      |
| 282809 | result type error                       | 返回结果请求错误（不属于excel或json）                        |
| 282810 | image recognize error                   | 图像识别错误                                                 |
| 283300 | Invalid argument                        | 入参格式有误，可检查下图片编码、代码格式是否有误             |

# 常见问题

**Q：人体分析服务如何收费？免费调用次数不够，如何提升限额？**

A：目前为免费公测期，每个接口每天500次免费调用，若免费限额不够，可以提交工单、或加入官方QQ群（860337848）联系群管提额。

**Q：人体分析可以直接分析视频流吗？**

A：目前仅支持对静态图片的人体分析，后续将陆续开放离线SDK、软硬一体方案，以满足视频流的处理分析需求。

**Q：人流统计接口可以实现对单位时间内去重人数的统计吗？**

A：请使用人流量统计（动态版）接口。

**Q：人流统计对摄像头的高度、角度有什么要求吗？**

A： 静态区域人流量统计的摄像头架设的建议：高度3m以上，5m以上为佳，垂直俯拍或者大角度斜拍，需要能拍全区域内的人头，适应人群密集场所。

动态人流量统计的摄像头架设的建议：适用于各种出入口，摄像头高度3m左右为佳，垂直俯拍或斜拍，需要拍到人体头部和肩部，人群过于密集会影响识别效果。

**Q：人体分析后期能支持离线吗？**

A：开发过程中，敬请期待。若着急接入，可在官网右下角浮窗“合作咨询”提交申请。

**Q：人流统计可以实现指定区域的识别与统计吗？**

A：目前接口已支持图片特定框选区域的人数统计，调用接口时设置area参数即可。

**Q：人流统计目前在什么场景下效果比较好？**

A：目前在室内场景，例如机场、商场、展馆等效果都比较好，室外场景，如交通场景、工地场景等，由于场景差异较大，建议先测试。若效果不理想可以提交工单、或加入官方QQ群（860337848）联系群管。

**Q：人流统计在我们的场景下效果不够理想怎么办？**

A：目前训练样本主要覆盖室内场景，例如机场、商场等，对于室外场景或是背景较为复杂的场景，可能由于过往训练样本覆盖不全，导致效果不够理想。若效果不理想可以提交工单、或加入官方QQ群（860337848），联系群管反馈问题。

**Q：图片质量会影响识别效果吗？**

A：如果图片质量差，会一定程度上影响模型效果。对于超高清图片建议压缩后识别，对于图片质量差的图片，建议提升图片清晰度，推荐分辨率720P以上的图片。

**Q：人流统计在车载场景识别效果好吗？**

A：针对车载场景，百度推出了一款驾驶行为分析接口，目前属于邀测状态。可以识别图片中是否有人体（驾驶员）、驾驶员是否在打手机、抽烟等行为。若有需求，可在官网右下角浮窗“合作咨询”提交申请。