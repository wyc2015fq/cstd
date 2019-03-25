# 图像搜索 Java SDK文档

# 简介

Hi，您好，欢迎使用百度图像搜索服务。

本文档主要针对Java开发者，描述百度图像搜索接口服务的相关技术内容。如果您对文档内容有任何疑问，可以通过以下几种方式联系我们：

- 在百度云控制台内[提交工单](http://ticket.bce.baidu.com/#/ticket/create)，咨询问题类型请选择**人工智能服务**；
- 如有疑问，进入[AI社区交流](http://ai.baidu.com/forum/topic/list/170)：http://ai.baidu.com/forum/topic/list/170
- 具有免费调用额度的接口，超过每天的免费额度后会返回错误码：17，错误信息：Open api daily request limit reached（每天流量超限额），**未上线计费的接口，可通过QQ群（630770006）联系群管、或提交工单手动提额；已上线计费的接口，可直接在控制台开通计费，调用量不受限制，按调用量阶梯计费，并保证10qps的并发**

## 服务说明

1、百度相同图/相似图/商品图检索API服务支持开发者基于自建图片库进行入库、检索、删除、更新操作。开发者在控制台创建应用后，需要在**控制台-应用详情页 申请建库**，建库成功后相关接口能力可正常调用。

2、申请建库时，图库的图片数量上限为50万，避免浪费资源；**如果容量不够用，可在控制台-应用详情页 申请扩容**，工作人员会在1-3个工作日内审批，每次扩容的申请上限也是50万，**可以反复申请扩容**，原则上图库总量无上限。 ![img](https://ai.bdstatic.com/file/C8E0EF4B4A804044A63D138DAE6F0211)

3、**图像搜索已上线图库管理功能，支持对图库里的图片进行可视化管理**，可小批量上传图片、预览大图、修改摘要和分类信息、删除图片、查询某张图是否在库里，入口在**控制台-应用详情页 管理图库**，注：大批量上传、修改、删除请调用API。 ![img](https://ai.bdstatic.com/file/40BBF19DD2894386A48F20C0577CE4E0)

## 接口能力

| 接口名称   | 接口能力简要描述                                             |
| :--------- | :----------------------------------------------------------- |
| 相同图检索 | 基于查询照片，返回自建图库中相同图片集合。                   |
| 相似图检索 | 基于查询照片，返回自建图库中的相似图片集合。                 |
| 商品检索   | 更适用于电商平台等场景下所使用的商品类型图片，基于查询商品类型照片，返回自建商品图库中的相同或相似图片集合。 |

## 版本更新记录

| 上线日期   | 版本号 | 更新内容                                        |
| :--------- | :----- | :---------------------------------------------- |
| 2018.12.29 | 4.10.0 | 相同、相似、商品图更新接口增加cont_sign参数支持 |
| 2018.7.20  | 4.5.0  | 新增相同图、相似图、商品图update接口            |
| 2017.12.22 | 4.0.0  | 接口统一升级                                    |
| 2017.11.28 | 3.4.0  | 新增商品检索接口                                |
| 2017.11.3  | 3.3.0  | 加入图像搜索服务                                |

# 快速入门

## 安装ImageSearch Java SDK

**ImageSearch Java SDK目录结构**

```
com.baidu.aip
       ├── auth                                //签名相关类
       ├── http                                //Http通信相关类
       ├── client                              //公用类
       ├── exception                           //exception类
       ├── imagesearch
       │       └── AipImageSearch           //AipImageSearch类
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

其中，`version`为版本号，添加完成后，用户就可以在工程中使用ImageSearch Java SDK。

## 新建AipImageSearch

AipImageSearch是图像搜索的Java客户端，为使用图像搜索的开发人员提供了一系列的交互方法。

用户可以参考如下代码新建一个AipImageSearch,初始化完成后建议**单例使用**,避免重复获取access_token：

```
public class Sample {
    //设置APPID/AK/SK
    public static final String APP_ID = "你的 App ID";
    public static final String API_KEY = "你的 Api Key";
    public static final String SECRET_KEY = "你的 Secret Key";

    public static void main(String[] args) {
        // 初始化一个AipImageSearch
        AipImageSearch client = new AipImageSearch(APP_ID, API_KEY, SECRET_KEY);

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
        JSONObject res = client.sameHqSearch(path, new HashMap<String, String>());
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

## 配置AipImageSearch

如果用户需要配置AipImageSearch的一些细节参数，可以在构造AipImageSearch之后调用接口设置参数，目前只支持以下参数：

| 接口                         | 说明                                                         |
| :--------------------------- | :----------------------------------------------------------- |
| setConnectionTimeoutInMillis | 建立连接的超时时间（单位：毫秒）                             |
| setSocketTimeoutInMillis     | 通过打开的连接传输数据的超时时间（单位：毫秒）               |
| setHttpProxy                 | 设置http代理服务器                                           |
| setSocketProxy               | 设置socket代理服务器 （http和socket类型代理服务器只能二选一） |

# 接口说明

## 相同图检索—入库

**该接口实现单张图片入库，入库时需要同步提交图片及可关联至本地图库的摘要信息（具体变量为brief，具体可传入图片在本地标记id、图片url、图片名称等）；同时可提交分类维度信息（具体变量为tags，最多可传入2个tag），方便对图库中的图片进行管理、分类检索。****注：重复添加完全相同的图片会返回错误。**

```
public void sample(AipImageSearch client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("brief", "{\"name\":\"周杰伦\", \"id\":\"666\"}");
    options.put("tags", "100,11");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.sameHqAdd(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.sameHqAdd(file, options);
    System.out.println(res.toString(2));
    
    // 相同图检索—入库, 图片参数为远程url图片
    JSONObject res = client.sameHqAddUrl(url, options);
    System.out.println(res.toString(2));

}
```

**相同图检索—入库 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 说明                                                         |
| :------- | :------- | :----- | :----------------------------------------------------------- |
| image    | 是       | mixed  | 本地图片路径或者图片二进制数据                               |
| url      | 是       | String | 图片完整URL，URL长度不超过1024字节，URL对应的图片base64编码后大小不超过4M，最短边至少15px，最长边最大4096px,支持jpg/png/bmp格式，当image字段存在时url字段失效 |
| brief    | 否       | String | 检索时原样带回,最长256B。                                    |
| tags     | 否       | String | 1 - 65535范围内的整数，tag间以逗号分隔，最多2个tag。样例："100,11" ；检索时可圈定分类维度进行检索 |

**相同图检索—入库 返回数据参数详情**

| 字段      | 是否必选 | 类型   | 说明                       |
| :-------- | :------- | :----- | :------------------------- |
| log_id    | 是       | uint64 | 唯一的log id，用于问题定位 |
| cont_sign | 是       | string | 输入图片签名，可用于删除   |

**相同图检索—入库 返回示例**

```
{
    "log_id": 2263663554,
    "cont_sign": "4261577168,501945506"
}
```

## 相同图检索—检索

完成入库后，可使用该接口实现相同图检索。**支持传入指定分类维度（具体变量tags）进行检索，返回结果支持翻页（具体变量pn、rn）。****请注意，检索接口不返回原图，仅反馈当前填写的brief信息，请调用入库接口时尽量填写可关联至本地图库的图片id或者图片url等信息。**

```
public void sample(AipImageSearch client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("tags", "100,11");
    options.put("tag_logic", "0");
    options.put("pn", "100");
    options.put("rn", "250");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.sameHqSearch(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.sameHqSearch(file, options);
    System.out.println(res.toString(2));
    
    // 相同图检索—检索, 图片参数为远程url图片
    JSONObject res = client.sameHqSearchUrl(url, options);
    System.out.println(res.toString(2));

}
```

**相同图检索—检索 请求参数详情**

| 参数名称  | 是否必选 | 类型   | 说明                                                         |
| :-------- | :------- | :----- | :----------------------------------------------------------- |
| image     | 是       | mixed  | 本地图片路径或者图片二进制数据                               |
| url       | 是       | String | 图片完整URL，URL长度不超过1024字节，URL对应的图片base64编码后大小不超过4M，最短边至少15px，最长边最大4096px,支持jpg/png/bmp格式，当image字段存在时url字段失效 |
| tags      | 否       | String | 1 - 65535范围内的整数，tag间以逗号分隔，最多2个tag。样例："100,11" ；检索时可圈定分类维度进行检索 |
| tag_logic | 否       | String | 检索时tag之间的逻辑， 0：逻辑and，1：逻辑or                  |
| pn        | 否       | String | 分页功能，起始位置，例：0。未指定分页时，默认返回前300个结果；接口返回数量最大限制1000条，例如：起始位置为900，截取条数500条，接口也只返回第900 - 1000条的结果，共计100条 |
| rn        | 否       | String | 分页功能，截取条数，例：250                                  |

**相同图检索—检索 返回数据参数详情**

| 字段       | 是否必选 | 类型     | 说明                                                         |
| :--------- | :------- | :------- | :----------------------------------------------------------- |
| log_id     | 是       | uint64   | 唯一的log id，用于问题定位                                   |
| result_num | 是       | uint32   | 检索结果数                                                   |
| result     | 是       | object[] | 结果数组                                                     |
| +cont_sign | 是       | string   | 图片签名，可以用来删除图片或定位问题                         |
| +score     | 是       | float    | 图片相关性，0-1                                              |
| +brief     | 是       | string   | 调用add接口添加的brief信息，为保证该结果有效性，请入库是填写有效可关联至本地图片库的有效id信息 |
| has_more   | 是       | bool     | 是否还有下一页，返回值：true、false；如果不分页，不用关注该字段 |

**相同图检索—检索 返回示例**

```
{
    "result_num": 1,
    "result": [
        {
            "score": 0.97976700290421,
            "brief": "./data/jay1.jpg",
            "cont_sign": "475124309,1080176642"
        }
    ],
    "log_id": 1968648150
}
```

## 相同图检索—更新

**更新图库中图片的摘要和分类信息（具体变量为brief、tags）**

```
public void sample(AipImageSearch client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("brief", "{\"name\":\"周杰伦\", \"id\":\"666\"}");
    options.put("tags", "100,11");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.sameHqUpdate(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.sameHqUpdate(file, options);
    System.out.println(res.toString(2));
    
    // 相同图检索—更新, 图片参数为远程url图片
    JSONObject res = client.sameHqUpdateUrl(url, options);
    System.out.println(res.toString(2));

}
```

**相同图检索—更新 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 说明                                                         |
| :------- | :------- | :----- | :----------------------------------------------------------- |
| image    | 是       | mixed  | 本地图片路径或者图片二进制数据                               |
| url      | 是       | String | 图片完整URL，URL长度不超过1024字节，URL对应的图片base64编码后大小不超过4M，最短边至少15px，最长边最大4096px,支持jpg/png/bmp格式，当image字段存在时url字段失效 |
| brief    | 否       | String | 更新的摘要信息，最长256B。样例：{"name":"周杰伦", "id":"666"} |
| tags     | 否       | String | 1 - 65535范围内的整数，tag间以逗号分隔，最多2个tag。样例："100,11" ；检索时可圈定分类维度进行检索 |

**相同图检索—更新 返回数据参数详情**

| 字段   | 是否必选 | 类型   | 说明                       |
| :----- | :------- | :----- | :------------------------- |
| log_id | 是       | uint64 | 唯一的log id，用于问题定位 |

**相同图检索—更新 返回示例**

```
{
    "log_id": 2263663554
}
```

## 相同图检索—删除

**删除图库中的图片，支持批量删除，批量删除时请传cont_sign参数，勿传image，最多支持1000个cont_sign**

```
public void sample(AipImageSearch client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.sameHqDeleteByImage(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.sameHqDeleteByImage(file, options);
    System.out.println(res.toString(2));
    
    // 删除相同图，图片参数为远程url图片
    JSONObject res = client.sameHqDeleteByUrl(url, options);
    System.out.println(res.toString(2));

    
    // 删除相同图，传入参数为图片签名
    JSONObject res = client.sameHqDeleteBySign(contSign, options);
    System.out.println(res.toString(2));

}
```

**相同图检索—删除 请求参数详情**

| 参数名称  | 是否必选 | 类型   | 说明                                                         |
| :-------- | :------- | :----- | :----------------------------------------------------------- |
| image     | 是       | mixed  | 本地图片路径或者图片二进制数据                               |
| url       | 是       | String | 图片完整URL，URL长度不超过1024字节，URL对应的图片base64编码后大小不超过4M，最短边至少15px，最长边最大4096px,支持jpg/png/bmp格式，当image字段存在时url字段失效 |
| cont_sign | 是       | String | 图片签名                                                     |

**相同图检索—删除 返回数据参数详情**

| 参数   | 类型   | 是否必须 | 说明                     | 示例      |
| :----- | :----- | :------- | :----------------------- | :-------- |
| log_id | number | 是       | 请求标识码，随机数，唯一 | 507499361 |

**相同图检索—删除 返回示例**

```
{
  "log_id": 50488417
}
```

## 相似图检索—入库

**该接口实现单张图片入库，入库时需要同步提交图片及可关联至本地图库的摘要信息（具体变量为brief，具体可传入图片在本地标记id、图片url、图片名称等）；同时可提交分类维度信息（具体变量为tags，最多可传入2个tag），方便对图库中的图片进行管理、分类检索。****注：重复添加完全相同的图片会返回错误。**

```
public void sample(AipImageSearch client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("brief", "{\"name\":\"周杰伦\", \"id\":\"666\"}");
    options.put("tags", "100,11");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.similarAdd(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.similarAdd(file, options);
    System.out.println(res.toString(2));
    
    // 相似图检索—入库, 图片参数为远程url图片
    JSONObject res = client.similarAddUrl(url, options);
    System.out.println(res.toString(2));

}
```

**相似图检索—入库 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 说明                                                         |
| :------- | :------- | :----- | :----------------------------------------------------------- |
| image    | 是       | mixed  | 本地图片路径或者图片二进制数据                               |
| url      | 是       | String | 图片完整URL，URL长度不超过1024字节，URL对应的图片base64编码后大小不超过4M，最短边至少15px，最长边最大4096px,支持jpg/png/bmp格式，当image字段存在时url字段失效 |
| brief    | 否       | String | 检索时原样带回,最长256B。                                    |
| tags     | 否       | String | 1 - 65535范围内的整数，tag间以逗号分隔，最多2个tag。样例："100,11" ；检索时可圈定分类维度进行检索 |

**相似图检索—入库 返回数据参数详情**

| 字段      | 是否必选 | 类型   | 说明                       |
| :-------- | :------- | :----- | :------------------------- |
| log_id    | 是       | uint64 | 唯一的log id，用于问题定位 |
| cont_sign | 是       | string | 输入图片签名，可用于删除   |

**相似图检索—入库 返回示例**

```
{
    "log_id": 2263663554,
    "cont_sign": "4261577168,501945506"
}
```

## 相似图检索—检索

完成入库后，可使用该接口实现相似图检索。**支持传入指定分类维度（具体变量tags）进行检索，返回结果支持翻页（具体变量pn、rn）。****请注意，检索接口不返回原图，仅反馈当前填写的brief信息，请调用入库接口时尽量填写可关联至本地图库的图片id或者图片url等信息。**

```
public void sample(AipImageSearch client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("tags", "100,11");
    options.put("tag_logic", "0");
    options.put("pn", "100");
    options.put("rn", "250");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.similarSearch(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.similarSearch(file, options);
    System.out.println(res.toString(2));
    
    // 相似图检索—检索, 图片参数为远程url图片
    JSONObject res = client.similarSearchUrl(url, options);
    System.out.println(res.toString(2));

}
```

**相似图检索—检索 请求参数详情**

| 参数名称  | 是否必选 | 类型   | 说明                                                         |
| :-------- | :------- | :----- | :----------------------------------------------------------- |
| image     | 是       | mixed  | 本地图片路径或者图片二进制数据                               |
| url       | 是       | String | 图片完整URL，URL长度不超过1024字节，URL对应的图片base64编码后大小不超过4M，最短边至少15px，最长边最大4096px,支持jpg/png/bmp格式，当image字段存在时url字段失效 |
| tags      | 否       | String | 1 - 65535范围内的整数，tag间以逗号分隔，最多2个tag。样例："100,11" ；检索时可圈定分类维度进行检索 |
| tag_logic | 否       | String | 检索时tag之间的逻辑， 0：逻辑and，1：逻辑or                  |
| pn        | 否       | String | 分页功能，起始位置，例：0。未指定分页时，默认返回前300个结果；接口返回数量最大限制1000条，例如：起始位置为900，截取条数500条，接口也只返回第900 - 1000条的结果，共计100条 |
| rn        | 否       | String | 分页功能，截取条数，例：250                                  |

**相似图检索—检索 返回数据参数详情**

| 字段       | 是否必选 | 类型     | 说明                                                         |
| :--------- | :------- | :------- | :----------------------------------------------------------- |
| log_id     | 是       | uint64   | 唯一的log id，用于问题定位                                   |
| result_num | 是       | uint32   | 检索结果数                                                   |
| result     | 是       | object[] | 结果数组                                                     |
| +cont_sign | 是       | string   | 图片签名，可以用来删除图片或定位问题                         |
| +score     | 是       | float    | 图片相关性，0-1                                              |
| +brief     | 是       | string   | add是添加的brief信息                                         |
| has_more   | 是       | bool     | 是否还有下一页，返回值：true、false；如果不分页，不用关注该字段 |

**相似图检索—检索 返回示例**

```
{
    "result_num": 1,
    "result": [
        {
            "score": 0.97976700290421,
            "brief": "./data/jay1.jpg",
            "cont_sign": "475124309,1080176642"
        }
    ],
    "log_id": 1968648150
}
```

## 相似图检索—更新

**更新图库中图片的摘要和分类信息（具体变量为brief、tags）**

```
public void sample(AipImageSearch client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("brief", "{\"name\":\"周杰伦\", \"id\":\"666\"}");
    options.put("tags", "100,11");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.similarUpdate(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.similarUpdate(file, options);
    System.out.println(res.toString(2));
    
    // 相似图检索—更新, 图片参数为远程url图片
    JSONObject res = client.similarUpdateUrl(url, options);
    System.out.println(res.toString(2));

}
```

**相似图检索—更新 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 说明                                                         |
| :------- | :------- | :----- | :----------------------------------------------------------- |
| image    | 是       | mixed  | 本地图片路径或者图片二进制数据                               |
| url      | 是       | String | 图片完整URL，URL长度不超过1024字节，URL对应的图片base64编码后大小不超过4M，最短边至少15px，最长边最大4096px,支持jpg/png/bmp格式，当image字段存在时url字段失效 |
| brief    | 否       | String | 更新的摘要信息，最长256B。样例：{"name":"周杰伦", "id":"666"} |
| tags     | 否       | String | 1 - 65535范围内的整数，tag间以逗号分隔，最多2个tag。样例："100,11" ；检索时可圈定分类维度进行检索 |

**相似图检索—更新 返回数据参数详情**

| 字段   | 是否必选 | 类型   | 说明                       |
| :----- | :------- | :----- | :------------------------- |
| log_id | 是       | uint64 | 唯一的log id，用于问题定位 |

**相似图检索—更新 返回示例**

```
{
    "log_id": 2263663554
}
```

## 相似图检索—删除

**删除图库中的图片，支持批量删除，批量删除时请传cont_sign参数，勿传image，最多支持1000个cont_sign**

```
public void sample(AipImageSearch client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.similarDeleteByImage(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.similarDeleteByImage(file, options);
    System.out.println(res.toString(2));
    
    // 删除相似图，图片参数为远程url图片
    JSONObject res = client.similarDeleteByUrl(url, options);
    System.out.println(res.toString(2));

    
    // 删除相似图，传入参数为图片签名
    JSONObject res = client.similarDeleteBySign(contSign, options);
    System.out.println(res.toString(2));

}
```

**相似图检索—删除 请求参数详情**

| 参数名称  | 是否必选 | 类型   | 说明                                                         |
| :-------- | :------- | :----- | :----------------------------------------------------------- |
| image     | 是       | mixed  | 本地图片路径或者图片二进制数据                               |
| url       | 是       | String | 图片完整URL，URL长度不超过1024字节，URL对应的图片base64编码后大小不超过4M，最短边至少15px，最长边最大4096px,支持jpg/png/bmp格式，当image字段存在时url字段失效 |
| cont_sign | 是       | String | 图片签名                                                     |

**相似图检索—删除 返回数据参数详情**

| 参数   | 类型   | 是否必须 | 说明                     | 示例      |
| :----- | :----- | :------- | :----------------------- | :-------- |
| log_id | number | 是       | 请求标识码，随机数，唯一 | 507499361 |

**相似图检索—删除 返回示例**

```
{
  "log_id": 50488417
}
```

## 商品检索—入库

**该接口实现单张图片入库，入库时需要同步提交图片及可关联至本地图库的摘要信息（具体变量为brief，具体可传入图片在本地标记id、图片url、图片名称等）。同时可提交分类维度信息（具体变量为class_id1、class_id2），方便对图库中的图片进行管理、分类检索。****注：重复添加完全相同的图片会返回错误。**

```
public void sample(AipImageSearch client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("brief", "{\"name\":\"手机\", \"id\":\"666\"}");
    options.put("class_id1", "1");
    options.put("class_id2", "1");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.productAdd(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.productAdd(file, options);
    System.out.println(res.toString(2));
    
    // 商品检索—入库, 图片参数为远程url图片
    JSONObject res = client.productAddUrl(url, options);
    System.out.println(res.toString(2));

}
```

**商品检索—入库 请求参数详情**

| 参数名称  | 是否必选 | 类型   | 说明                                                         |
| :-------- | :------- | :----- | :----------------------------------------------------------- |
| image     | 是       | mixed  | 本地图片路径或者图片二进制数据                               |
| url       | 是       | String | 图片完整URL，URL长度不超过1024字节，URL对应的图片base64编码后大小不超过4M，最短边至少15px，最长边最大4096px,支持jpg/png/bmp格式，当image字段存在时url字段失效 |
| brief     | 否       | String | 检索时原样带回,最长256B。**请注意，检索接口不返回原图，仅反馈当前填写的brief信息，所以调用该入库接口时，brief信息请尽量填写可关联至本地图库的图片id或者图片url、图片名称等信息** |
| class_id1 | 否       | String | 商品分类维度1，支持1-65535范围内的整数。检索时可圈定该分类维度进行检索 |
| class_id2 | 否       | String | 商品分类维度1，支持1-65535范围内的整数。检索时可圈定该分类维度进行检索 |

**商品检索—入库 返回数据参数详情**

| 字段      | 是否必选 | 类型   | 说明                       |
| :-------- | :------- | :----- | :------------------------- |
| log_id    | 是       | uint64 | 唯一的log id，用于问题定位 |
| cont_sign | 是       | string | 输入图片签名，可用于删除   |

**商品检索—入库 返回示例**

```
{
    "log_id": 2263663554,
    "cont_sign": "4261577168,501945506"
}
```

## 商品检索—检索

完成入库后，可使用该接口实现商品检索。**支持传入指定分类维度（具体变量class_id1、class_id2）进行检索，返回结果支持翻页（具体变量pn、rn）。****请注意，检索接口不返回原图，仅反馈当前填写的brief信息，请调用入库接口时尽量填写可关联至本地图库的图片id或者图片url等信息**

```
public void sample(AipImageSearch client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("class_id1", "1");
    options.put("class_id2", "1");
    options.put("pn", "100");
    options.put("rn", "250");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.productSearch(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.productSearch(file, options);
    System.out.println(res.toString(2));
    
    // 商品检索—检索, 图片参数为远程url图片
    JSONObject res = client.productSearchUrl(url, options);
    System.out.println(res.toString(2));

}
```

**商品检索—检索 请求参数详情**

| 参数名称  | 是否必选 | 类型   | 说明                                                         |
| :-------- | :------- | :----- | :----------------------------------------------------------- |
| image     | 是       | mixed  | 本地图片路径或者图片二进制数据                               |
| url       | 是       | String | 图片完整URL，URL长度不超过1024字节，URL对应的图片base64编码后大小不超过4M，最短边至少15px，最长边最大4096px,支持jpg/png/bmp格式，当image字段存在时url字段失效 |
| class_id1 | 否       | String | 商品分类维度1，支持1-65535范围内的整数。检索时可圈定该分类维度进行检索 |
| class_id2 | 否       | String | 商品分类维度1，支持1-65535范围内的整数。检索时可圈定该分类维度进行检索 |
| tag_logic | 否       | String | 检索时tag之间的逻辑， 0：逻辑and，1：逻辑or                  |
| pn        | 否       | String | 分页功能，起始位置，例：0。未指定分页时，默认返回前300个结果；接口返回数量最大限制1000条，例如：起始位置为900，截取条数500条，接口也只返回第900 - 1000条的结果，共计100条 |
| rn        | 否       | String | 分页功能，截取条数，例：250                                  |

**商品检索—检索 返回数据参数详情**

| 字段       | 是否必选 | 类型     | 说明                                                         |
| :--------- | :------- | :------- | :----------------------------------------------------------- |
| log_id     | 是       | uint64   | 唯一的log id，用于问题定位                                   |
| result_num | 是       | uint32   | 检索结果数                                                   |
| result     | 是       | object[] | 结果数组                                                     |
| +cont_sign | 是       | string   | 图片签名，可以用来删除图片或定位问题                         |
| +score     | 是       | float    | 图片相关性，0-1                                              |
| +brief     | 是       | string   | add是添加的brief信息                                         |
| has_more   | 是       | bool     | 是否还有下一页，返回值：true、false；如果不分页，不用关注该字段 |

**商品检索—检索 返回示例**

```
{
    "result_num": 1,
    "result": [
        {
            "score": 0.97976700290421,
            "brief": "./data/mobile.jpg",
            "cont_sign": "475124309,1080176642"
        }
    ],
    "log_id": 1968648150
}
```

## 商品检索—更新

**更新图库中图片的摘要和分类信息（具体变量为brief、class_id1/class_id2）**

```
public void sample(AipImageSearch client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    options.put("brief", "{\"name\":\"周杰伦\", \"id\":\"666\"}");
    options.put("class_id1", "1");
    options.put("class_id2", "1");
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.productUpdate(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.productUpdate(file, options);
    System.out.println(res.toString(2));
    
    // 商品检索—更新, 图片参数为远程url图片
    JSONObject res = client.productUpdateUrl(url, options);
    System.out.println(res.toString(2));

}
```

**商品检索—更新 请求参数详情**

| 参数名称  | 是否必选 | 类型   | 说明                                                         |
| :-------- | :------- | :----- | :----------------------------------------------------------- |
| image     | 是       | mixed  | 本地图片路径或者图片二进制数据                               |
| url       | 是       | String | 图片完整URL，URL长度不超过1024字节，URL对应的图片base64编码后大小不超过4M，最短边至少15px，最长边最大4096px,支持jpg/png/bmp格式，当image字段存在时url字段失效 |
| brief     | 否       | String | 更新的摘要信息，最长256B。样例：{"name":"周杰伦", "id":"666"} |
| class_id1 | 否       | String | 更新的商品分类1，支持1-65535范围内的整数。                   |
| class_id2 | 否       | String | 更新的商品分类2，支持1-65535范围内的整数。                   |

**商品检索—更新 返回数据参数详情**

| 字段   | 是否必选 | 类型   | 说明                       |
| :----- | :------- | :----- | :------------------------- |
| log_id | 是       | uint64 | 唯一的log id，用于问题定位 |

**商品检索—更新 返回示例**

```
{
    "log_id": 2263663554
}
```

## 商品检索—删除

**删除图库中的图片，支持批量删除，批量删除时请传cont_sign参数，勿传image，最多支持1000个cont_sign**

```
public void sample(AipImageSearch client) {
    // 传入可选参数调用接口
    HashMap<String, String> options = new HashMap<String, String>();
    
    
    // 参数为本地路径
    String image = "test.jpg";
    JSONObject res = client.productDeleteByImage(image, options);
    System.out.println(res.toString(2));

    // 参数为二进制数组
    byte[] file = readFile("test.jpg");
    res = client.productDeleteByImage(file, options);
    System.out.println(res.toString(2));
    
    // 删除商品，图片参数为远程url图片
    JSONObject res = client.productDeleteByUrl(url, options);
    System.out.println(res.toString(2));

    
    // 删除商品，传入参数为图片签名
    JSONObject res = client.productDeleteBySign(contSign, options);
    System.out.println(res.toString(2));

}
```

**商品检索—删除 请求参数详情**

| 参数名称  | 是否必选 | 类型   | 说明                                                         |
| :-------- | :------- | :----- | :----------------------------------------------------------- |
| image     | 是       | mixed  | 本地图片路径或者图片二进制数据                               |
| url       | 是       | String | 图片完整URL，URL长度不超过1024字节，URL对应的图片base64编码后大小不超过4M，最短边至少15px，最长边最大4096px,支持jpg/png/bmp格式，当image字段存在时url字段失效 |
| cont_sign | 是       | String | 图片签名                                                     |

**商品检索—删除 返回数据参数详情**

| 参数   | 类型   | 是否必须 | 说明                     | 示例      |
| :----- | :----- | :------- | :----------------------- | :-------- |
| log_id | number | 是       | 请求标识码，随机数，唯一 | 507499361 |

**商品检索—删除 返回示例**

```
{
  "log_id": 50488417
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
| 1      | Unknown error                           | 服务器内部错误，请再次请求， 如果持续出现此类错误，请通过QQ群（630770006）或工单联系技术支持团队。 |
| 2      | Service temporarily unavailable         | 服务暂不可用，请再次请求， 如果持续出现此类错误，请通过QQ群（630770006）或工单联系技术支持团队。 |
| 3      | Unsupported openapi method              | 调用的API不存在，请检查后重新尝试                            |
| 4      | Open api request limit reached          | 集群超限额                                                   |
| 6      | No permission to access data            | 无权限访问该用户数据                                         |
| 13     | Get service token failed                | 获取token失败                                                |
| 14     | IAM Certification failed                | IAM 鉴权失败                                                 |
| 15     | app not exsits or create failed         | 应用不存在或者创建失败                                       |
| 17     | Open api daily request limit reached    | 每天请求量超限额；未上线计费的接口，可通过QQ群（630770006）联系群管、或[提交工单](http://ticket.bce.baidu.com/#/ticket/create)手动提额；已上线计费的接口，可直接在控制台开通计费，调用量不受限制，按调用量阶梯计费，并保证10qps的并发 |
| 18     | Open api qps request limit reached      | QPS超限额；未上线计费的接口，可通过QQ群（630770006）联系群管、或[提交工单](http://ticket.bce.baidu.com/#/ticket/create)手动提额；已上线计费的接口，可直接在控制台开通计费，调用量不受限制，按调用量阶梯计费，并保证10qps的并发 |
| 19     | Open api total request limit reached    | 请求总量超限额，请通过QQ群（630770006）联系群管手动提额      |
| 100    | Invalid parameter                       | 无效的access_token参数，请检查后重新尝试                     |
| 110    | Access token invalid or no longer valid | access_token无效                                             |
| 111    | Access token expired                    | access token过期                                             |
| 282000 | internal error                          | 服务器内部错误，请再次请求， 如果持续出现此类错误，请通过QQ群（630770006）或工单联系技术支持团队。 |
| 216017 | not initialized                         | 未建库，请先前往控制台申请建库                               |
| 216100 | invalid param                           | 请求中包含非法参数，请检查后重新尝试                         |
| 216101 | not enough param                        | 缺少必须的参数，请检查参数是否有遗漏                         |
| 216102 | service not support                     | 请求了不支持的服务，请检查调用的url                          |
| 216103 | param too long                          | 请求中某些参数过长，请检查后重新尝试                         |
| 216110 | appid not exist                         | appid不存在，请重新核对信息是否为后台应用列表中的appid       |
| 216200 | empty image                             | 图片为空，请检查后重新尝试                                   |
| 216201 | image format error                      | 上传的图片格式错误，现阶段我们支持的图片格式为：PNG、JPG、JPEG、BMP，请进行转码或更换图片 |
| 216202 | image size error                        | 上传的图片大小错误，现阶段我们支持的图片大小为：base64编码后小于4M，分辨率不高于4096*4096，请重新上传图片 |
| 216203 | image size error                        | 上传图片错误，通常情况下为图片base64编码错误，请重新上传图片 |
| 216630 | recognize error                         | 识别错误，请再次请求，如果持续出现此类错误，请通过QQ群（630770006）或工单联系技术支持团队。 |
| 216631 | recognize bank card error               | 识别银行卡错误，出现此问题的原因一般为：您上传的图片非银行卡正面，上传了异形卡的图片或上传的银行卡正品图片不完整 |
| 216633 | recognize idcard error                  | 识别身份证错误，出现此问题的原因一般为：您上传了非身份证图片或您上传的身份证图片不完整 |
| 216680 | item not exist                          | item不在库里                                                 |
| 216681 | item exist                              | item已经在库里，不能重复入库                                 |
| 216634 | detect error                            | 检测错误，请再次请求，如果持续出现此类错误，请通过QQ群（630770006）或工单联系技术支持团队。 |
| 282003 | missing parameters: {参数名}            | 请求参数缺失                                                 |
| 282005 | batch processing error                  | 处理批量任务时发生部分或全部错误，请根据具体错误码排查       |
| 282006 | batch task limit reached                | 批量任务处理数量超出限制，请将任务数量减少到10或10以下       |
| 282112 | url download timeout                    | url下载超时，请检查url对应的图床/图片无法下载或链路状况不好，您可以重新尝试一下，如果多次尝试后仍不行，建议更换图片地址 |
| 282113 | url response invalid                    | URL返回无效参数，一般是图片URL抓图失败，由于图床的差异性，抓图服务无法适配所有的图床，部分URL可能抓不到图，请将图片下载到本地转码后上传 |
| 282114 | url size error                          | 图片URL长度超过1024字节或为0                                 |
| 282808 | request id: xxxxx not exist             | request id xxxxx 不存在                                      |
| 282809 | result type error                       | 返回结果请求错误（不属于excel或json）                        |
| 282810 | image recognize error                   | 图像识别错误                                                 |
| 283300 | image recognize error                   | 数据格式有误，可以检查base64编码、代码格式等                 |