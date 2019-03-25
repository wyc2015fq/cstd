# 语言处理基础技术 Java SDK文档

# 简介

Hi，您好，欢迎使用百度自然语言处理服务。

本文档主要针对Java开发者，描述百度自然语言处理接口服务的相关技术内容。如果您对文档内容有任何疑问，可以通过以下几种方式联系我们：

- 在百度云控制台内[提交工单](http://ticket.bce.baidu.com/#/ticket/create)，咨询问题类型请选择**人工智能服务**；
- 如有疑问，进入[AI社区交流](http://ai.baidu.com/forum/topic/list/169)：http://ai.baidu.com/forum/topic/list/169

## 接口能力

| 接口名称     | 接口能力简要描述                                             |
| :----------- | :----------------------------------------------------------- |
| 词法分析     | 分词、词性标注、专名识别                                     |
| 依存句法分析 | 自动分析文本中的依存句法结构信息                             |
| 词向量表示   | 查询词汇的词向量，实现文本的可计算                           |
| DNN语言模型  | 判断一句话是否符合语言表达习惯，输出分词结果并给出每个词在句子中的概率值 |
| 词义相似度   | 计算两个给定词语的语义相似度                                 |
| 短文本相似度 | 判断两个文本的相似度得分                                     |
| 评论观点抽取 | 提取一个句子观点评论的情感属性                               |
| 情感倾向分析 | 对包含主观观点信息的文本进行情感极性类别（积极、消极、中性）的判断，并给出相应的置信度 |
| 中文分词     | 切分出连续文本中的基本词汇序列（已合并到词法分析接口）       |
| 词性标注     | 为自然语言文本中的每个词汇赋予词性（已合并到词法分析接口）   |
| 新闻摘要     | 自动抽取新闻文本中的关键信息，进而生成指定长度的新闻摘要     |

## 版本更新记录

| 上线日期   | 版本号 | 更新内容                                                     |
| :--------- | :----- | :----------------------------------------------------------- |
| 2018.12.7  | 4.9.0  | 新增新闻摘要接口                                             |
| 2018.6.15  | 4.4.1  | 新增情感识别和文本纠错接口                                   |
| 2018.1.26  | 4.1.1  | 新增文本分类接口                                             |
| 2018.1.11  | 4.1.0  | 新增文本标签接口                                             |
| 2017.12.22 | 4.0.0  | 接口统一升级                                                 |
| 2017.10.18 | 3.2.1  | 使用proxy问题修复                                            |
| 2017.8.25  | 3.0.0  | 更新sdk打包方式：所有AI服务集成一个SDK                       |
| 2017.7.14  | 1.5.2  | 更新sdk打包方式                                              |
| 2017.6.30  | 1.5.1  | 新增句法依存接口                                             |
| 2017.6.15  | 1.5.0  | 短文本相似度接口升级                                         |
| 2017.5.25  | 1.4.0  | 词向量、评论观战、中文DNN接口升级，新增词相似度和情感分析接口 |
| 2017.4.20  | 1.3.3  | 新增词法分析接口，规范分词接口错误码                         |
| 2017.4.13  | 1.3.2  | AI SDK同步版本更新                                           |
| 2017.3.23  | 1.3    | 对安卓环境兼容问题进行修复                                   |
| 2017.3.2   | 1.2    | 增加设置超时接口                                             |
| 2017.1.20  | 1.1    | 对部分云用户调用不成功的错误修复                             |
| 2017.1.6   | 1.0    | 初始版本，上线中文分词、词性标注、词向量表示、中文DNN语言模型、短文本相似度和评论观点抽取接口 |

# 快速入门

## 安装NLP Java SDK

**NLP Java SDK目录结构**

```
com.baidu.aip
       ├── auth                                //签名相关类
       ├── http                                //Http通信相关类
       ├── client                              //公用类
       ├── exception                           //exception类
       ├── nlp
       │       └── AipNlp           //AipNlp类
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

其中，`version`为版本号，添加完成后，用户就可以在工程中使用NLP Java SDK。

## 新建AipNlp

AipNlp是自然语言处理的Java客户端，为使用自然语言处理的开发人员提供了一系列的交互方法。

用户可以参考如下代码新建一个AipNlp,初始化完成后建议**单例使用**,避免重复获取access_token：

```
public class Sample {
    //设置APPID/AK/SK
    public static final String APP_ID = "你的 App ID";
    public static final String API_KEY = "你的 Api Key";
    public static final String SECRET_KEY = "你的 Secret Key";

    public static void main(String[] args) {
        // 初始化一个AipNlp
        AipNlp client = new AipNlp(APP_ID, API_KEY, SECRET_KEY);

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
        String text = "百度是一家高科技公司";
        JSONObject res = client.lexer(text, null);
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

## 配置AipNlp

如果用户需要配置AipNlp的一些细节参数，可以在构造AipNlp之后调用接口设置参数，目前只支持以下参数：

| 接口                         | 说明                                                         |
| :--------------------------- | :----------------------------------------------------------- |
| setConnectionTimeoutInMillis | 建立连接的超时时间（单位：毫秒）                             |
| setSocketTimeoutInMillis     | 通过打开的连接传输数据的超时时间（单位：毫秒）               |
| setHttpProxy                 | 设置http代理服务器                                           |
| setSocketProxy               | 设置socket代理服务器 （http和socket类型代理服务器只能二选一） |

# 接口说明

## 词法分析

词法分析接口向用户提供分词、词性标注、专名识别三大功能；能够识别出文本串中的基本词汇（分词），对这些词汇进行重组、标注组合后词汇的词性，并进一步识别出命名实体。

```
public void sample(AipNlp client) {
    String text = "百度是一家高科技公司";

    // 传入可选参数调用接口
    HashMap<String, Object> options = new HashMap<String, Object>();
    
    // 词法分析
    JSONObject res = client.lexer(text, options);
    System.out.println(res.toString(2));

}
```

**词法分析 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 说明                                                 |
| :------- | :------- | :----- | :--------------------------------------------------- |
| text     | 是       | String | 待分析文本（目前仅支持GBK编码），长度不超过65536字节 |

**词法分析 返回数据参数详情**

| 参数名称      | 类型          | **必需** | 详细说明                                                     |
| :------------ | :------------ | :------- | :----------------------------------------------------------- |
| text          | string        | 是       | 原始单条请求文本                                             |
| items         | array(object) | 是       | 词汇数组，每个元素对应结果中的一个词                         |
| +item         | string        | 是       | 词汇的字符串                                                 |
| +ne           | string        | 是       | 命名实体类型，命名实体识别算法使用。词性标注算法中，此项为空串 |
| +pos          | string        | 是       | 词性，词性标注算法使用。命名实体识别算法中，此项为空串       |
| +byte_offset  | int           | 是       | 在text中的字节级offset（使用GBK编码）                        |
| +byte_length  | int           | 是       | 字节级length（使用GBK编码）                                  |
| +uri          | string        | 否       | 链指到知识库的URI，只对命名实体有效。对于非命名实体和链接不到知识库的命名实体，此项为空串 |
| +formal       | string        | 否       | 词汇的标准化表达，主要针对时间、数字单位，没有归一化表达的，此项为空串 |
| +basic_words  | array(string) | 是       | 基本词成分                                                   |
| +loc_details  | array(object) | 否       | 地址成分，非必需，仅对地址型命名实体有效，没有地址成分的，此项为空数组。 |
| ++type        | string        | 是       | 成分类型，如省、市、区、县                                   |
| ++byte_offset | int           | 是       | 在item中的字节级offset（使用GBK编码）                        |
| ++byte_length | int           | 是       | 字节级length（使用GBK编码）                                  |

**词法分析 返回示例**

```
{
  "status":0,
  "version":"ver_1_0_1",
  "results":[
    {
      "retcode":0,
      "text":"百度是一家高科技公司",
      "items":[
         {
           "byte_length":4,
           "byte_offset":0,
           "formal":"",
           "item":"百度",
           "ne":"ORG",
           "pos":"",
           "uri":"",
           "loc_details":[ ],
           "basic_words":["百度"]
         },
         {
           "byte_length":2,
           "byte_offset":4,
           "formal":"",
           "item":"是",
           "ne":"",
           "pos":"v",
           "uri":"",
           "loc_details":[ ],
           "basic_words":["是"]
         },
         {
           "byte_length":4,
           "byte_offset":6,
           "formal":"",
           "item":"一家",
           "ne":"",
           "pos":"m",
           "uri":"",
           "loc_details":[ ],
           "basic_words":["一","家"]
         },
         {
           "byte_length":6,
           "byte_offset":10,
           "formal":"",
           "item":"高科技",
           "ne":"",
           "pos":"n",
           "uri":"",
           "loc_details":[ ],
           "basic_words":["高","科技"]
         },
         {
           "byte_length":4,
           "byte_offset":16,
           "formal":"",
           "item":"公司",
           "ne":"",
           "pos":"n",
           "uri":"",
           "loc_details":[ ],
           "basic_words":["公司"]
         }
      ]
    }
  ]
}
```

**词性缩略说明**

| **词性** | **含义** | **词性** | **含义** | **词性** | **含义**   | **词性** | **含义** |
| :------- | :------- | :------- | :------- | :------- | :--------- | :------- | :------- |
| n        | 普通名词 | f        | 方位名词 | s        | 处所名词   | t        | 时间名词 |
| nr       | 人名     | ns       | 地名     | nt       | 机构团体名 | nw       | 作品名   |
| nz       | 其他专名 | v        | 普通动词 | vd       | 动副词     | vn       | 名动词   |
| a        | 形容词   | ad       | 副形词   | an       | 名形词     | d        | 副词     |
| m        | 数量词   | q        | 量词     | r        | 代词       | p        | 介词     |
| c        | 连词     | u        | 助词     | xc       | 其他虚词   | w        | 标点符号 |

**专名识别缩略词含义**

| **缩略词** | **含义** | **缩略词** | **含义** | **缩略词** | **含义** | **缩略词** | **含义** |
| :--------- | :------- | :--------- | :------- | :--------- | :------- | :--------- | :------- |
| PER        | 人名     | LOC        | 地名     | ORG        | 机构名   | TIME       | 时间     |

## 词法分析（定制版）

词法分析接口向用户提供分词、词性标注、专名识别三大功能；能够识别出文本串中的基本词汇（分词），对这些词汇进行重组、标注组合后词汇的词性，并进一步识别出命名实体。

```
public void sample(AipNlp client) {
    String text = "百度是一家高科技公司";

    // 传入可选参数调用接口
    HashMap<String, Object> options = new HashMap<String, Object>();
    
    // 词法分析（定制版）
    JSONObject res = client.lexerCustom(text, options);
    System.out.println(res.toString(2));

}
```

**词法分析（定制版） 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 说明                                                 |
| :------- | :------- | :----- | :--------------------------------------------------- |
| text     | 是       | String | 待分析文本（目前仅支持GBK编码），长度不超过65536字节 |

**词法分析（定制版） 返回数据参数详情**

| 参数名称      | 类型          | **必需** | 详细说明                                                     |
| :------------ | :------------ | :------- | :----------------------------------------------------------- |
| text          | string        | 是       | 原始单条请求文本                                             |
| items         | array(object) | 是       | 词汇数组，每个元素对应结果中的一个词                         |
| +item         | string        | 是       | 词汇的字符串                                                 |
| +ne           | string        | 是       | 命名实体类型，命名实体识别算法使用。词性标注算法中，此项为空串 |
| +pos          | string        | 是       | 词性，词性标注算法使用。命名实体识别算法中，此项为空串       |
| +byte_offset  | int           | 是       | 在text中的字节级offset（使用GBK编码）                        |
| +byte_length  | int           | 是       | 字节级length（使用GBK编码）                                  |
| +uri          | string        | 否       | 链指到知识库的URI，只对命名实体有效。对于非命名实体和链接不到知识库的命名实体，此项为空串 |
| +formal       | string        | 否       | 词汇的标准化表达，主要针对时间、数字单位，没有归一化表达的，此项为空串 |
| +basic_words  | array(string) | 是       | 基本词成分                                                   |
| +loc_details  | array(object) | 否       | 地址成分，非必需，仅对地址型命名实体有效，没有地址成分的，此项为空数组。 |
| ++type        | string        | 是       | 成分类型，如省、市、区、县                                   |
| ++byte_offset | int           | 是       | 在item中的字节级offset（使用GBK编码）                        |
| ++byte_length | int           | 是       | 字节级length（使用GBK编码）                                  |

**词法分析（定制版） 返回示例**

```
参考词法分析接口
```

## 依存句法分析

依存句法分析接口可自动分析文本中的依存句法结构信息，利用句子中词与词之间的依存关系来表示词语的句法结构信息（如“主谓”、“动宾”、“定中”等结构关系），并用树状结构来表示整句的结构（如“主谓宾”、“定状补”等）。

```
public void sample(AipNlp client) {
    String text = "张飞";

    // 传入可选参数调用接口
    HashMap<String, Object> options = new HashMap<String, Object>();
    options.put("mode", 1);
    
    // 依存句法分析
    JSONObject res = client.depParser(text, options);
    System.out.println(res.toString(2));

}
```

**依存句法分析 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 说明                                                         |
| :------- | :------- | :----- | :----------------------------------------------------------- |
| text     | 是       | String | 待分析文本（目前仅支持GBK编码），长度不超过256字节           |
| mode     | 否       | String | 模型选择。默认值为0，可选值mode=0（对应web模型）；mode=1（对应query模型） |

**依存句法分析 返回数据参数详情**

| 参数名称 | +类型  | 详细说明                                              |
| :------- | :----- | :---------------------------------------------------- |
| log_id   | uint64 | 随机数，本次请求的唯一标识码                          |
| id       | number | 词的ID                                                |
| word     | string | 词                                                    |
| postag   | string | 词性，请参照API文档中的**词性（postag)取值范围**      |
| head     | int    | 词的父节点ID                                          |
| +deprel  | string | 词与父节点的依存关系，请参照API文档的**依存关系标识** |

**依存句法分析 返回示例**

```
{
    "log_id": 12345,
    "text":"今天天气怎么样",
    "items":[
        {
            "id":"1", //id
            "word":"今天", //word
            "postag":"t", //POS tag
            "head":"2", //id of current word's parent
            "deprel":"ATT"  //depend relations between current word and parent
        },
        {
            "id":"2",
            "word":"天气",
            "postag":"n",
            "head":"3",
            "deprel":"SBV",
        },
        {
            "id":"3",
            "word":"怎么样",
            "postag":"r",
            "head":"0",
            "deprel":"HED",
        }
    ]
}
```

## 词向量表示

词向量表示接口提供中文词向量的查询功能。

```
public void sample(AipNlp client) {
    String word = "张飞";

    // 传入可选参数调用接口
    HashMap<String, Object> options = new HashMap<String, Object>();
    
    // 词向量表示
    JSONObject res = client.wordEmbedding(word, options);
    System.out.println(res.toString(2));

}
```

**词向量表示 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 说明                            |
| :------- | :------- | :----- | :------------------------------ |
| word     | 是       | String | 文本内容（GBK编码），最大64字节 |

**词向量表示 返回数据参数详情**

| 参数   | 类型   | 描述           |
| :----- | :----- | :------------- |
| log_id | uint64 | 请求唯一标识码 |
| word   | string | 查询词         |
| vec    | float  | 词向量结果表示 |

**词向量表示 返回示例**

```
{
  "word": "张飞",
  "vec": [
    0.233962,
    0.336867,
    0.187044,
    0.565261,
    0.191568,
    0.450725,
    ...
    0.43869,
    -0.448038,
    0.283711,
    -0.233656,
    0.555556
  ]
}
```

## DNN语言模型

中文DNN语言模型接口用于输出切词结果并给出每个词在句子中的概率值,判断一句话是否符合语言表达习惯。

```
public void sample(AipNlp client) {
    String text = "床前明月光";

    // 传入可选参数调用接口
    HashMap<String, Object> options = new HashMap<String, Object>();
    
    // DNN语言模型
    JSONObject res = client.dnnlmCn(text, options);
    System.out.println(res.toString(2));

}
```

**DNN语言模型 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 说明                                         |
| :------- | :------- | :----- | :------------------------------------------- |
| text     | 是       | String | 文本内容（GBK编码），最大512字节，不需要切词 |

**DNN语言模型 返回数据参数详情**

| 参数   | 类型   | 说明                                   |
| :----- | :----- | :------------------------------------- |
| log_id | uint64 | 请求唯一标识码                         |
| word   | string | 句子的切词结果                         |
| prob   | float  | 该词在句子中的概率值,取值范围[0,1]     |
| ppl    | float  | 描述句子通顺的值：数值越低，句子越通顺 |

```
{
  "text": "床前明月光",
  "items": [
    {
      "word": "床",
      "prob": 0.0000385273
    },
    {
      "word": "前",
      "prob": 0.0289018
    },
    {
      "word": "明月",
      "prob": 0.0284406
    },
    {
      "word": "光",
      "prob": 0.808029
    }
  ],
  "ppl": 79.0651
}
```

**DNN语言模型 返回示例**

## 词义相似度

输入两个词，得到两个词的相似度结果。

```
public void sample(AipNlp client) {
    String word1 = "北京";
    String word2 = "上海";

    // 传入可选参数调用接口
    HashMap<String, Object> options = new HashMap<String, Object>();
    options.put("mode", 0);
    
    // 词义相似度
    JSONObject res = client.wordSimEmbedding(word1, word2, options);
    System.out.println(res.toString(2));

}
```

**词义相似度 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 说明                                                         |
| :------- | :------- | :----- | :----------------------------------------------------------- |
| word_1   | 是       | String | 词1（GBK编码），最大64字节                                   |
| word_2   | 是       | String | 词1（GBK编码），最大64字节                                   |
| mode     | 否       | String | 预留字段，可选择不同的词义相似度模型。默认值为0，目前仅支持mode=0 |

**词义相似度 返回数据参数详情**

| 参数    | 类型   | 描述                  |
| :------ | :----- | :-------------------- |
| log_id  | number | 请求唯一标识码,随机数 |
| score   | number | 相似度分数            |
| words   | array  | 输入的词列表          |
| +word_1 | string | 输入的word1参数       |
| +word_2 | string | 输入的word2参数       |

**词义相似度 返回示例**

```
{
    "score": 0.456862,
    "words": {
      "word_1": "北京",
      "word_2": "上海"
    }
}
```

## 短文本相似度

短文本相似度接口用来判断两个文本的相似度得分。

```
public void sample(AipNlp client) {
    String text1 = "浙富股份";
    String text2 = "万事通自考网";

    // 传入可选参数调用接口
    HashMap<String, Object> options = new HashMap<String, Object>();
    options.put("model", "CNN");
    
    // 短文本相似度
    JSONObject res = client.simnet(text1, text2, options);
    System.out.println(res.toString(2));

}
```

**短文本相似度 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 可选值范围   | 说明                                  |
| :------- | :------- | :----- | :----------- | :------------------------------------ |
| text_1   | 是       | String |              | 待比较文本1（GBK编码），最大512字节   |
| text_2   | 是       | String |              | 待比较文本2（GBK编码），最大512字节   |
| model    | 否       | String | BOW CNN GRNN | 默认为"BOW"，可选"BOW"、"CNN"与"GRNN" |

**短文本相似度 返回数据参数详情**

| 参数    | 类型   | 描述               |
| :------ | :----- | :----------------- |
| log_id  | number | 请求唯一标识       |
| score   | number | 两个文本相似度得分 |
| texts   | array  | 输入文本           |
| +text_1 | string | 第一个短文本       |
| +text_2 | string | 第二个短文本       |

**短文本相似度 返回示例**

```
{
    "log_id": 12345,
    "texts":{
        "text_1":"浙富股份",
        "text_2":"万事通自考网"
    },
    "score":0.3300237655639648 //相似度结果
},
```

## 评论观点抽取

评论观点抽取接口用来提取一条评论句子的关注点和评论观点，并输出评论观点标签及评论观点极性。

```
public void sample(AipNlp client) {
    String text = "三星电脑电池不给力";

    // 获取美食评论情感属性
    JSONObject response = client.commentTag("这家餐馆味道不错", ESimnetType.FOOD, options);
    System.out.println(response.toString());

    // 获取酒店评论情感属性
    response = client.commentTag("喜来登酒店不错", ESimnetType.HOTEL, options);
    System.out.println(response.toString());

}
```

**评论观点抽取 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 可选值范围                                                   | 说明                               |
| :------- | :------- | :----- | :----------------------------------------------------------- | :--------------------------------- |
| text     | 是       | String |                                                              | 评论内容（GBK编码），最大10240字节 |
| type     | 否       | String | 1 - 酒店 2 - KTV3 - 丽人 4 - 美食餐饮 5 - 旅游 6 - 健康 7 - 教育 8 - 商业 9 - 房产 10 - 汽车 11 - 生活 12 - 购物 13 - 3C | 评论行业类型，默认为4（餐饮美食）  |

**评论观点抽取 返回数据参数详情**

| 参数      | 类型   | 描述                                                |
| :-------- | :----- | :-------------------------------------------------- |
| log_id    | uint64 | 请求唯一标识码                                      |
| prop      | string | 匹配上的属性词                                      |
| adj       | string | 匹配上的描述词                                      |
| sentiment | int    | 该情感搭配的极性（0表示消极，1表示中性，2表示积极） |
| begin_pos | int    | 该情感搭配在句子中的开始位置                        |
| end_pos   | int    | 该情感搭配在句子中的结束位置                        |
| abstract  | string | 对应于该情感搭配的短句摘要                          |

**评论观点抽取 返回示例**

```
{
    "items": [
        {
        "prop":"电池",
        "adj": "不给力",
        "sentiment": 0,
        "begin_pos": 8,
        "end_pos": 18,
        "abstract":"三星电脑<span>电池不给力</span>"
        }
    ]
}
```

## 情感倾向分析

对包含主观观点信息的文本进行情感极性类别（积极、消极、中性）的判断，并给出相应的置信度。

```
public void sample(AipNlp client) {
    String text = "苹果是一家伟大的公司";

    // 传入可选参数调用接口
    HashMap<String, Object> options = new HashMap<String, Object>();
    
    // 情感倾向分析
    JSONObject res = client.sentimentClassify(text, options);
    System.out.println(res.toString(2));

}
```

**情感倾向分析 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 说明                                |
| :------- | :------- | :----- | :---------------------------------- |
| text     | 是       | String | 文本内容（GBK编码），最大102400字节 |

**情感倾向分析 返回数据参数详情**

| 参数           | 是否必须 | 类型   | 说明                                         |
| :------------- | :------- | :----- | :------------------------------------------- |
| text           | 是       | string | 输入的文本内容                               |
| items          | 是       | array  | 输入的词列表                                 |
| +sentiment     | 是       | number | 表示情感极性分类结果, 0:负向，1:中性，2:正向 |
| +confidence    | 是       | number | 表示分类的置信度                             |
| +positive_prob | 是       | number | 表示属于积极类别的概率                       |
| +negative_prob | 是       | number | 表示属于消极类别的概率                       |

**情感倾向分析 返回示例**

```
{
    "text":"苹果是一家伟大的公司",
    "items":[
        {
            "sentiment":2,    //表示情感极性分类结果
            "confidence":0.40, //表示分类的置信度
            "positive_prob":0.73, //表示属于积极类别的概率
            "negative_prob":0.27  //表示属于消极类别的概率
        }
    ]
}
```

## 文章标签

文章标签服务能够针对网络各类媒体文章进行快速的内容理解，根据输入含有标题的文章，输出多个内容标签以及对应的置信度，用于个性化推荐、相似文章聚合、文本内容分析等场景。

```
public void sample(AipNlp client) {
    String title = "iphone手机出现“白苹果”原因及解决办法，用苹果手机的可以看下";
    String content = "如果下面的方法还是没有解决你的问题建议来我们门店看下成都市锦江区红星路三段99号银石广场24层01室。";

    // 传入可选参数调用接口
    HashMap<String, Object> options = new HashMap<String, Object>();
    
    // 文章标签
    JSONObject res = client.keyword(title, content, options);
    System.out.println(res.toString(2));

}
```

**文章标签 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 说明                      |
| :------- | :------- | :----- | :------------------------ |
| title    | 是       | String | 篇章的标题，最大80字节    |
| content  | 是       | String | 篇章的正文，最大65535字节 |

**文章标签 返回数据参数详情**

| 参数   | 是否必须 | 类型          | 说明                                           |
| :----- | :------- | :------------ | :--------------------------------------------- |
| items  | 是       | array(object) | 关键词结果数组，每个元素对应抽取到的一个关键词 |
| +tag   | 是       | string        | 关注点字符串                                   |
| +score | 是       | number        | 权重(取值范围0~1)                              |

**文章标签 返回示例**

```
{
    "log_id": 4457308639853058292,
    "items": [
        {
            "score": 0.997762,
            "tag": "iphone"
        },
        {
            "score": 0.861775,
            "tag": "手机"
        },
        {
            "score": 0.845657,
            "tag": "苹果"
        },
        {
            "score": 0.83649,
            "tag": "苹果公司"
        },
        {
            "score": 0.797243,
            "tag": "数码"
        }
    ]
}
```

## 文章分类

对文章按照内容类型进行自动分类，首批支持娱乐、体育、科技等26个主流内容类型，为文章聚类、文本内容分析等应用提供基础技术支持。

```
public void sample(AipNlp client) {
    String title = "欧洲冠军杯足球赛";
    String content = "欧洲冠军联赛是欧洲足球协会联盟主办的年度足球比赛，代表欧洲俱乐部足球最高荣誉和水平，被认为是全世界最高素质、最具影响力以及最高水平的俱乐部赛事，亦是世界上奖金最高的足球赛事和体育赛事之一。";

    // 传入可选参数调用接口
    HashMap<String, Object> options = new HashMap<String, Object>();
    
    // 文章分类
    JSONObject res = client.topic(title, content, options);
    System.out.println(res.toString(2));

}
```

**文章分类 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 说明                      |
| :------- | :------- | :----- | :------------------------ |
| title    | 是       | String | 篇章的标题，最大80字节    |
| content  | 是       | String | 篇章的正文，最大65535字节 |

**文章分类 返回数据参数详情**

| 参数名称      | 类型             | 详细说明                     |
| :------------ | :--------------- | :--------------------------- |
| item          | object           | 分类结果，包含一级与二级分类 |
| +lv1_tag_list | array of objects | 一级分类结果                 |
| +lv2_tag_list | array of objects | 二级分类结果                 |
| ++score       | float            | 类别标签对应得分，范围0-1    |
| ++tag         | string           | 类别标签                     |

**文章分类 返回示例**

```
{
    "log_id": 5710764909216517248,
    "item": {
        "lv2_tag_list": [
            {
                "score": 0.895467,
                "tag": "足球"
            },
            {
                "score": 0.794878,
                "tag": "国际足球"
            }
        ],
        "lv1_tag_list": [
            {
                "score": 0.88808,
                "tag": "体育"
            }
        ]
    }
}
```

## 文本纠错

识别输入文本中有错误的片段，提示错误并给出正确的文本结果。支持短文本、长文本、语音等内容的错误识别，纠错是搜索引擎、语音识别、内容审查等功能更好运行的基础模块之一。

```
public void sample(AipNlp client) {
    String text = "百度是一家人工只能公司";

    // 传入可选参数调用接口
    HashMap<String, Object> options = new HashMap<String, Object>();
    
    // 文本纠错
    JSONObject res = client.ecnet(text, options);
    System.out.println(res.toString(2));

}
```

**文本纠错 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 说明                        |
| :------- | :------- | :----- | :-------------------------- |
| text     | 是       | String | 待纠错文本，输入限制511字节 |

**文本纠错 返回数据参数详情**

| 参数           | 说明   | 描述             |
| :------------- | :----- | :--------------- |
| log_id         | uint64 | 请求唯一标识码   |
| correct_query  | string | 纠错后的文本     |
| score          | double | 模型置信度打分   |
| item           | object | 分析结果         |
| +vec_fragment  | list   | 替换候选片段信息 |
| ++ori_frag     | string | 原片段           |
| ++correct_frag | double | 替换片段         |
| ++begin_pos    | int    | 起始(长度单位)   |
| ++end_pos      | list   | 结尾(长度单位)   |

**文本纠错 返回示例**

```
{
    "log_id": 6770395607901559829,
    "item": {
        "vec_fragment": [
            {
                "ori_frag": "只能",
                "begin_pos": 21,
                "correct_frag": "智能",
                "end_pos": 27
            }
        ],
        "score": 0.875169,
        "correct_query": "百度是一家人工智能公司"
    },
    "text": "百度是一家人工只能公司"
}
```

## 对话情绪识别接口

针对用户日常沟通文本背后所蕴含情绪的一种直观检测，可自动识别出当前会话者所表现出的情绪类别及其置信度，可以帮助企业更全面地把握产品服务质量、监控客户服务质量

```
public void sample(AipNlp client) {
    String text = "本来今天高高兴兴";

    // 传入可选参数调用接口
    HashMap<String, Object> options = new HashMap<String, Object>();
    options.put("scene", "talk");
    
    // 对话情绪识别接口
    JSONObject res = client.emotion(text, options);
    System.out.println(res.toString(2));

}
```

**对话情绪识别接口 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 说明                                                         |
| :------- | :------- | :----- | :----------------------------------------------------------- |
| text     | 是       | String | 待识别情感文本，输入限制512字节                              |
| scene    | 否       | String | default（默认项-不区分场景），talk（闲聊对话-如度秘聊天等），task（任务型对话-如导航对话等），customer_service（客服对话-如电信/银行客服等） |

**对话情绪识别接口 返回数据参数详情**

| 参数    | 说明   | 描述                                                         |
| :------ | :----- | :----------------------------------------------------------- |
| log_id  | uint64 | 请求唯一标识码                                               |
| text    | string | 输入的对话文本内容                                           |
| items   | list   | 分析结果数组                                                 |
| ++label | string | item的分类标签；pessimistic（强烈负向情绪）、neutral（非强烈负向情绪） |
| ++prob  | double | item标签对应的概率                                           |

**对话情绪识别接口 返回示例**

```
{
    "log_id": 4258005459150262970,
    "text": "本来今天高高兴兴",
    "items": [
        {
            "prob": 0.998619,
            "label": "neutral"
        },
        {
            "prob": 0.00138141,
            "label": "pessimistic"
        },
    ]
}
```

## 新闻摘要接口

自动抽取新闻文本中的关键信息，进而生成指定长度的新闻摘要

```
public void sample(AipNlp client) {
    String content = "麻省理工学院的研究团队为无人机在仓库中使用RFID技术进行库存查找等工作，创造了一种...";
    int maxSummaryLen = 300;

    // 传入可选参数调用接口
    HashMap<String, Object> options = new HashMap<String, Object>();
    options.put("title", "标题");
    
    // 新闻摘要接口
    JSONObject res = client.newsSummary(content, maxSummaryLen, options);
    System.out.println(res.toString(2));

}
```

**新闻摘要接口 请求参数详情**

| 参数名称        | 是否必选 | 类型   | 说明                                                         |
| :-------------- | :------- | :----- | :----------------------------------------------------------- |
| content         | 是       | String | 字符串（限200字符数）字符串仅支持GBK编码，长度需小于200字符数（即400字节），请输入前确认字符数没有超限，若字符数超长会返回错误。标题在算法中具有重要的作用，若文章确无标题，输入参数的“标题”字段为空即可 |
| max_summary_len | 是       | int    | 此数值将作为摘要结果的最大长度。例如：原文长度1000字，本参数设置为150，则摘要结果的最大长度是150字；推荐最优区间：200-500字 |
| title           | 否       | String | 字符串（限200字符数）字符串仅支持GBK编码，长度需小于200字符数（即400字节），请输入前确认字符数没有超限，若字符数超长会返回错误。标题在算法中具有重要的作用，若文章确无标题，输入参数的“标题”字段为空即可 |

**新闻摘要接口 返回数据参数详情**

| 参数    | 说明   | 描述           |
| :------ | :----- | :------------- |
| log_id  | uint64 | 请求唯一标识码 |
| summary | string | 摘要结果       |

**新闻摘要接口 返回示例**

```
{
    "log_id": 4258005459150262970,
    "summary": "麻省理工学院的研究团队为无人机在仓库中使用RFID技术进行库存查找等工作，创造了一种聪明的新方式。使用RFID标签更换仓库中的条形码，将帮助提升自动化并提高库存管理的准确性。几家公司已经解决了无人机读取RFID的技术问题。麻省理工学院的新解决方案，名为Rfly，允许无人机阅读RFID标签，而不用捆绑巨型读卡器。无人机接收从远程RFID读取器发送的信号，然后转发它读取附近的标签。"
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
| 282000 | internal error                          | 服务器内部错误，请再次请求， 如果持续出现此类错误，请通过QQ群（632426386）或工单联系技术支持团队。 |
| 282002 | input encoding error                    | 编码错误，请使用GBK编码                                      |
| 282004 | invalid parameter(s)                    | 请求中包含非法参数，请检查后重新尝试                         |
| 282130 | no result                               | 当前查询无结果返回，出现此问题的原因一般为：参数配置存在问题，请检查后重新尝试 |
| 282131 | input text too long                     | 输入长度超限，请查看文档说明                                 |
| 282133 | param {参数名} not exist                | 接口参数缺失                                                 |
| 282300 | word error                              | word不在算法词典中                                           |
| 282301 | word_1 error                            | word_1提交的词汇暂未收录，无法比对相似度                     |
| 282302 | word_2 error                            | word_2提交的词汇暂未收录，无法比对相似度                     |
| 282303 | word_1&word_2 error                     | word_1和word_2暂未收录，无法比对相似度                       |